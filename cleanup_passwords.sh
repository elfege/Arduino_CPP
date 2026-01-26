#!/bin/bash
set +H  # Disable history expansion to handle ! in patterns

clear

. ~/.bash_utils --no-exec &>/dev/null

cleanup() {
	stop_spinner
	trap - EXIT INT TSTP ERR
	exit 0
}
trap 'cleanup' EXIT INT TSTP ERR

export SEARCH_PATH=${1:-"$HOME/0_ARDUINO"}

# ============================================================================
# GIT HISTORY REWRITE CONFIGURATION
# ============================================================================
# Set to true to enable git history rewriting (removes secrets from ALL commits)
# WARNING: This is DESTRUCTIVE and rewrites git history!
# - All collaborators will need to re-clone the repo
# - Requires force push to remote
# - Cannot be undone once pushed
REWRITE_GIT_HISTORY=false

# Set to true to automatically force push after history rewrite
# If false, you'll need to manually run: git push --force --all
AUTO_FORCE_PUSH=false

# Associative array: DEFINE_NAME => "search_pattern"
# The key is used as both the replacement text and the #define name in secrets.h
declare -A CREDENTIALS=(
	["WIFI_PASSWORD"]="WIFI_PASSWORD"
	["PASSWORD"]="(PASSWORD|PASSWORD)"
	["HUBITAT_API_KEY"]="HUBITAT_API_KEY"
	["WIFI_SSID"]="WIFI_SSID"
	["WIFI_SSID2"]="WIFI_SSID2"
)

# Directories to exclude
declare -a EXCLUDE_DIRS=(
	".git"
	".cache"
	"ARCHIVE"
	"OHVD"
	"OHVD_APP_PROD_no_delete"
	".vscode-server"
	"node_modules"
	"venv"
	"__pycache__"
	"Documents"
	".local"
	"snap"
	"docker"
	".docker"
	"mnt"
	"proc"
	"sys"
	"tmp"
	"var"
	"usr"
	"build"
)

# Files to exclude
declare -a EXCLUDE_FILES=(
	"secrets.h"
	"cleanup_passwords.sh"
	"*.bin"
	"*.elf"
	"*.o"
	"*.db"
	"*.ipch"
	"*.log"
	"*.pyc"
	"*.sock"
	"*.lock"
	"*.pid"
	"*.tmp"
	"*.swap"
	"*.swp"
	"$(basename "${BASH_SOURCE[0]}")"
)

# Build grep exclude arguments
GREP_EXCLUDES=()
for dir in "${EXCLUDE_DIRS[@]}"; do
	GREP_EXCLUDES+=("--exclude-dir=$dir")
done
for file in "${EXCLUDE_FILES[@]}"; do
	GREP_EXCLUDES+=("--exclude=$file")
done
export GREP_EXCLUDES

for item in "${GREP_EXCLUDES[@]}"; do
	clean="${item/--exclude-dir=/}"
	clean="${clean/--exclude=/}"
	echo -e "${clean} ${RED} pattern will be EXCLUDED $NC"
done

# Function to ensure secrets.h exists and contains the required define
# Arguments: $1 = directory path, $2 = define_name, $3 = original_value
ensure_secrets_h() {
	local dir="$1"
	local define_name="$2"
	local original_value="$3"
	local secrets_file="$dir/secrets.h"
	local dir_name=$(basename "$dir")
	local guard_name="${dir_name^^}_SECRETS_H"
	# Sanitize guard name: replace non-alphanumeric with underscore
	guard_name=$(echo "$guard_name" | sed 's/[^A-Z0-9_]/_/g')

	# Create secrets.h if it doesn't exist
	if [[ ! -f "$secrets_file" ]]; then
		echo -e "${YELLOW}Creating $secrets_file${NC}"
		cat > "$secrets_file" << EOF
#ifndef ${guard_name}
#define ${guard_name}

// WiFi credentials and sensitive data
// This file should NOT be committed to git

#endif // ${guard_name}
EOF
	fi

	# Check if the define already exists in secrets.h
	if ! grep -q "^#define ${define_name}" "$secrets_file"; then
		echo -e "${GREEN}Adding #define ${define_name} to $secrets_file${NC}"
		# Insert the define before the #endif line
		sed -i "/#endif/i #define ${define_name} \"${original_value}\"" "$secrets_file"
	else
		echo -e "${CYAN}#define ${define_name} already exists in $secrets_file${NC}"
	fi

	# Ensure secrets.h is in .gitignore for this directory
	local gitignore_file="$dir/.gitignore"
	if [[ ! -f "$gitignore_file" ]] || ! grep -q "^secrets\.h$" "$gitignore_file"; then
		echo "secrets.h" >> "$gitignore_file"
		echo -e "${YELLOW}Added secrets.h to $gitignore_file${NC}"
	fi
}

# Function to process files for a given credential
# Arguments: $1 = define_name (also used as replacement), $2 = pattern
process_credential() {
	local define_name="$1"
	local pattern="$2"

	repeat_print "═"
	echo -e "${ACCENT_YELLOW}Looking for: ${define_name} (pattern: ${pattern})${NC}"

	# Find files matching the pattern
	local grep_cmd="grep -rlE '${pattern}' \"$SEARCH_PATH\" ${GREP_EXCLUDES[*]}"
	readarray -t matching_files < <(eval "$grep_cmd" 2>/dev/null)

	echo "Found ${#matching_files[@]} files"

	if [[ ${#matching_files[@]} -eq 0 ]]; then
		return 0
	fi

	# List found files
	for file in "${matching_files[@]}"; do
		echo "  - $file"
	done
	echo

	# Process each file
	for file in "${matching_files[@]}"; do
		local filename=$(basename "$file")
		local dir=$(dirname "$file")

		# Extract the actual sensitive value from the file before redacting
		local original_value
		original_value=$(grep -oE "${pattern}" "$file" | head -1)

		if [[ -n "$original_value" ]]; then
			# Ensure secrets.h exists and contains the define
			ensure_secrets_h "$dir" "$define_name" "$original_value"
		fi

		# Remove from git cache
		echo "Removing $filename from git cache"
		git rm --cached "$file" &>/dev/null

		# Redact the sensitive data (replace pattern with define name)
		echo -e "${YELLOW}Redacting ${define_name} in $file${NC}"
		sed -i -E "s/${pattern}/${define_name}/g" "$file"

		# Backup to archive
		cp "$file" "/mnt/h/OneDrive/Documents/Arduino/ARCHIVE/$(basename "$file")" 2>/dev/null &
	done

	echo
}

# Main execution
repeat_print "═"
echo -e "${ACCENT_YELLOW}Starting credential cleanup${NC}"
repeat_print "═"
echo

# Process each credential
for define_name in "${!CREDENTIALS[@]}"; do
	pattern="${CREDENTIALS[$define_name]}"
	process_credential "$define_name" "$pattern"
done

repeat_print "═"
echo -e "${GREEN}File cleanup complete${NC}"
repeat_print "═"

# ============================================================================
# GIT HISTORY REWRITE (optional)
# ============================================================================
rewrite_git_history() {
	echo
	repeat_print "═"
	echo -e "${RED}WARNING: GIT HISTORY REWRITE${NC}"
	repeat_print "═"
	echo -e "${YELLOW}This will PERMANENTLY remove secrets from ALL git history.${NC}"
	echo -e "${YELLOW}This action:${NC}"
	echo -e "${YELLOW}  - Rewrites ALL commits in the repository${NC}"
	echo -e "${YELLOW}  - Requires force push to remote (--force)${NC}"
	echo -e "${YELLOW}  - Will break all existing clones (collaborators must re-clone)${NC}"
	echo -e "${YELLOW}  - CANNOT BE UNDONE once pushed to remote${NC}"
	echo
	echo -e "${CYAN}Patterns to be removed from history:${NC}"
	for define_name in "${!CREDENTIALS[@]}"; do
		echo -e "  - ${define_name}: ${CREDENTIALS[$define_name]}"
	done
	echo

	# Prompt for confirmation
	read -p "Are you sure you want to rewrite git history? (type 'YES' to confirm): " confirm
	if [[ "$confirm" != "YES" ]]; then
		echo -e "${GREEN}Git history rewrite cancelled.${NC}"
		return 0
	fi

	# Check if git-filter-repo is installed
	if ! command -v git-filter-repo &>/dev/null; then
		echo -e "${YELLOW}git-filter-repo not found. Installing...${NC}"
		pip install git-filter-repo || {
			echo -e "${RED}Failed to install git-filter-repo. Please install manually:${NC}"
			echo -e "${YELLOW}  pip install git-filter-repo${NC}"
			return 1
		}
	fi

	# Change to repo directory
	cd "$SEARCH_PATH" || return 1

	# Create replacement file for git-filter-repo
	local replace_file=$(mktemp)
	for define_name in "${!CREDENTIALS[@]}"; do
		pattern="${CREDENTIALS[$define_name]}"
		# For regex patterns, we need to handle them differently
		# git-filter-repo uses literal strings by default
		# Remove regex grouping for literal replacement
		clean_pattern=$(echo "$pattern" | sed 's/[()]//g' | sed 's/|/\n/g')
		while IFS= read -r p; do
			if [[ -n "$p" ]]; then
				echo "${p}==>${define_name}" >> "$replace_file"
			fi
		done <<< "$clean_pattern"
	done

	echo -e "${YELLOW}Replacement patterns:${NC}"
	cat "$replace_file"
	echo

	# Create a backup branch before rewriting
	local backup_branch="backup-before-history-rewrite-$(date +%Y%m%d_%H%M%S)"
	git branch "$backup_branch" 2>/dev/null
	echo -e "${GREEN}Created backup branch: $backup_branch${NC}"

	# Save remote URL before filter-repo (it removes remotes as a safety feature)
	local remote_url
	remote_url=$(git remote get-url origin 2>/dev/null)
	if [[ -n "$remote_url" ]]; then
		echo -e "${CYAN}Saving remote URL: $remote_url${NC}"
	fi

	# Run git-filter-repo
	echo -e "${YELLOW}Rewriting git history...${NC}"
	git filter-repo --replace-text "$replace_file" --force

	# Clean up
	rm -f "$replace_file"

	# Restore remote URL (filter-repo removes it intentionally)
	if [[ -n "$remote_url" ]]; then
		echo -e "${YELLOW}Restoring remote origin...${NC}"
		git remote add origin "$remote_url"
		echo -e "${GREEN}Remote restored: $remote_url${NC}"
	fi

	echo -e "${GREEN}Git history rewrite complete.${NC}"
	echo

	# Force push if enabled
	if [[ "$AUTO_FORCE_PUSH" == "true" ]]; then
		echo -e "${YELLOW}Force pushing to remote...${NC}"
		read -p "Confirm force push to remote? (type 'PUSH' to confirm): " push_confirm
		if [[ "$push_confirm" == "PUSH" ]]; then
			git push --force --all
			git push --force --tags
			echo -e "${GREEN}Force push complete.${NC}"
		else
			echo -e "${YELLOW}Force push cancelled. Run manually:${NC}"
			echo -e "${CYAN}  git push --force --all${NC}"
			echo -e "${CYAN}  git push --force --tags${NC}"
		fi
	else
		echo -e "${YELLOW}To push changes to remote, run:${NC}"
		echo -e "${CYAN}  git push --force --all${NC}"
		echo -e "${CYAN}  git push --force --tags${NC}"
	fi
}

# Execute git history rewrite if enabled
if [[ "$REWRITE_GIT_HISTORY" == "true" ]]; then
	rewrite_git_history
else
	echo
	echo -e "${CYAN}Note: Git history still contains old secrets.${NC}"
	echo -e "${CYAN}To remove from history, set REWRITE_GIT_HISTORY=true and re-run.${NC}"
fi

repeat_print "═"
echo -e "${GREEN}All operations complete${NC}"
repeat_print "═"

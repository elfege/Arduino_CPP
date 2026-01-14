#!/bin/bash

clear

. ~/.bash_utils --no-exec &>/dev/null
cleanup() {
	stop_spinner
	trap - EXIT INT TSTP ERR
	exit 0
}
trap 'cleanup' EXIT INT TSTP ERR

export PATH=${1:-"$HOME/0_ARDUINO"}
export PATTERN_1="${2:-'(WIFI_PASSWORD|PASSWORD|PASSWORD)'}"
export PATTERN_2="${3:-'WIFI_SSID'}"
export PATTERN_3="${4:-'WIFI_SSID2'}"

# Directories to exclude (grep format)
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
)

# Files to exclude (grep format)
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

#
# build the grep excludes string
GREP_EXCLUDES=""
for dir in "${EXCLUDE_DIRS[@]}"; do
	GREP_EXCLUDES+=("--exclude-dir=$dir ")
done
for file in "${EXCLUDE_FILES[@]}"; do
	GREP_EXCLUDES+=("--exclude=$file ")
done
export GREP_EXCLUDES

for item in "${GREP_EXCLUDES[@]}"; do
	clean="${item/--exclude-dir=/}"
	clean="${clean/--exclude=/}"
	echo -e "${clean} ${RED} pattern will be EXCLUDED $NC"
done

GREP_COMMAND_PWD=(grep -rlE \'${PATTERN_1}\' "$PATH" "${GREP_EXCLUDES[@]}")
GREP_COMMNAD_SSID1=(grep -rlE \'${PATTERN_2}\' "$PATH" "${GREP_EXCLUDES[@]}")
GREP_COMMAND_SSID2=(grep -rlE \'${PATTERN_3}\' "$PATH" "${GREP_EXCLUDES[@]}")

# echo "GREP_COMMAND_PWD=${GREP_COMMAND_PWD[@]}"
# echo "GREP_COMMNAD_SSID1=${GREP_COMMNAD_SSID1[@]}"
# echo "GREP_COMMAND_SSID2=${GREP_COMMAND_SSID2[@]}"

repeat_print "═"
echo -e "${ACCENT_YELLOW}Looking for files containing password patterns"
readarray -t FILES_WITH_PASSWORD < <(eval "${GREP_COMMAND_PWD[@]}")
repeat_print "═"
echo -e "${ACCENT_YELLOW}Looking for files containing $PATTERN_2"
readarray -t FILES_WITH_SSID_1 < <(eval "${GREP_COMMNAD_SSID1[@]}")
repeat_print "═"
echo -e "${ACCENT_YELLOW}Looking for files containing $PATTERN_3"
readarray -t FILES_WITH_SSID_2 < <(eval "${GREP_COMMAND_SSID2[@]}")

echo
echo "Found ${#FILES_WITH_PASSWORD[@]} files with password pattern."
echo
echo "Found ${#FILES_WITH_SSID_1[@]} files with ssid 1 pattern."
echo
echo "Found ${#FILES_WITH_SSID_2[@]} files with ssid 2 pattern."
echo

if [ "${#FILES_WITH_PASSWORD[@]}" -ne 0 ]; then
	echo
	repeat_print "═"
	echo -e "${ACCENT_YELLOW}files with password pattern. $NC"
	repeat_print "═"
	for file in "${FILES_WITH_PASSWORD[@]}"; do
		echo "- $file"
	done
fi

if [ "${#FILES_WITH_SSID_1[@]}" -ne 0 ]; then
	echo
	repeat_print "═"
	echo -e "${ACCENT_YELLOW}files with ssid 1 pattern. $NC"
	repeat_print "═"
	for file in "${FILES_WITH_SSID_1[@]}"; do
		echo "- $file"
	done
	echo
fi

if [ "${#FILES_WITH_SSID_2[@]}" -ne 0 ]; then
	echo
	repeat_print "═"
	echo -e "${ACCENT_YELLOW}files with ssid 2 pattern. $NC"
	repeat_print "═"
	for file in "${FILES_WITH_SSID_2[@]}"; do
		echo "- $file"
	done
fi

for file in "${FILES_WITH_PASSWORD[@]}"; do
	# echo "- $file"
	filename="$(basename "$file")"
	# start_spinner 120 "Redacting password in $filename"
	echo "Redacting password in $file"
	sed -i -E "s/${PATTERN_1}/PASSWORD/g" "$file" # &>/dev/null
	cp "$file" /mnt/h/OneDrive/Documents/Arduino/ARCHIVE/$(basename "$file") &
done

for file in "${FILES_WITH_SSID_1[@]}"; do
	# echo "- $file"
	filename="$(basename "$file")"
	# start_spinner 120 "Redacting password in $filename"
	echo "Redacting password in $file"
	sed -i -E "s/${PATTERN_2}/WIFI_SSID/g" "$file" # &>/dev/null
	cp "$file" /mnt/h/OneDrive/Documents/Arduino/ARCHIVE/$(basename "$file") &
done

for file in "${FILES_WITH_SSID_2[@]}"; do
	# echo "- $file"
	filename="$(basename "$file")"
	# start_spinner 120 "Redacting password in $filename"
	echo "Redacting password in $file"
	sed -i -E "s/${PATTERN_3}/WIFI_SSID2/g" "$file" # &>/dev/null
	cp "$file" /mnt/h/OneDrive/Documents/Arduino/ARCHIVE/$(basename "$file") &
done

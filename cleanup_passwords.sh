#!/bin/bash

ROOT_DIRECTORY=${1:-"$HOME/0_ARDUINO"}

. ~/.bash_utils --no-exec &>/dev/null
cleanup() {
	stop_spinner
	trap - EXIT INT TSTP ERR
	exit 0
}
trap 'cleanup' EXIT INT TSTP ERR

export ssid_pattern_1='WIFI_SSID'
export ssid_pattern_2='WIFI_SSID2'
export password_pattern='PASSWORD|PASSWORD|PASSWORD'

start_spinner 120 "Looking for files containing password patterns"
readarray -t FILES_WITH_PASSWORD < <(grep -rlE "$password_pattern" "$ROOT_DIRECTORY" --exclude="secrets.h" --exclude="$0" --exclude-dir=".git")
start_spinner 120 "Looking for files containing ssid"
readarray -t FILES_WITH_SSID_1 < <(grep -rlE "$ssid_pattern_1" "$ROOT_DIRECTORY" --exclude="secrets.h" --exclude="$(basename "${BASH_SOURCE[0]}")" --exclude-dir=".git")
readarray -t FILES_WITH_SSID_2 < <(grep -rlE "$ssid_pattern_2" "$ROOT_DIRECTORY" --exclude="secrets.h" --exclude="$(basename "${BASH_SOURCE[0]}")" --exclude-dir=".git")
stop_spinner

# echo
for file in "${FILES_WITH_PASSWORD[@]}"; do
	echo "files with password pattern $file"
done

echo
echo "Found ${#FILES_WITH_PASSWORD[@]} files with password pattern."
echo
echo "Found ${#FILES_WITH_SSID_1[@]} files with password pattern."
echo
echo "Found ${#FILES_WITH_SSID_2[@]} files with password pattern."
echo

exit 0

for file in "${FILES_WITH_PASSWORD[@]}"; do
	# echo "- $file"
	filename="$(basename "$file")"
	# start_spinner 120 "Redacting password in $filename"
	echo "Redacting password in $file"
	sed -i -E "s/${password_pattern}/PASSWORD/g" "$file" # &>/dev/null
	cp "$file" /mnt/h/OneDrive/Documents/Arduino/ARCHIVE/$(basename "$file") &
done

for file in "${FILES_WITH_SSID_1[@]}"; do
	# echo "- $file"
	filename="$(basename "$file")"
	# start_spinner 120 "Redacting password in $filename"
	echo "Redacting password in $file"
	sed -i -E "s/${ssid_pattern_1}/WIFI_SSID/g" "$file" # &>/dev/null
	cp "$file" /mnt/h/OneDrive/Documents/Arduino/ARCHIVE/$(basename "$file") &
done

for file in "${FILES_WITH_SSID_2[@]}"; do
	# echo "- $file"
	filename="$(basename "$file")"
	# start_spinner 120 "Redacting password in $filename"
	echo "Redacting password in $file"
	sed -i -E "s/${ssid_pattern_2}/WIFI_SSID2/g" "$file" # &>/dev/null
	cp "$file" /mnt/h/OneDrive/Documents/Arduino/ARCHIVE/$(basename "$file") &
done

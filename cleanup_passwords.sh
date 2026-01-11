#!/bin/bash

. ~/.bash_utils --no-exec &>/dev/null
cleanup(){
	stop_spinner
	trap - EXIT INT TSTP ERR
}
trap 'cleanup' EXIT INT TSTP ERR

export ssid_pattern_1='WIFI_SSID'
export ssid_pattern_2='WIFI_SSID2'
export password_pattern='WIFI_PASSWORD'

start_spinner 120 "Looking for files containing wifi password"
readarray -t FILES_WITH_PASSWORD < <(grep -rlE "$password_pattern" . --exclude="secrets.h" --exclude-dir=".git")
start_spinner 120 "Looking for files containing ssid"
readarray -t FILES_WITH_SSID_1 < <(grep -rlE "$ssid_pattern_1" . --exclude="secrets.h" --exclude-dir=".git")
readarray -t FILES_WITH_SSID_2 < <(grep -rlE "$ssid_pattern_2" . --exclude="secrets.h" --exclude-dir=".git")
stop_spinner

# echo 
# echo "${#FILES_WITH_PASSWORD[@]}"
echo
echo "Found ${#FILES_WITH_PASSWORD[@]} files with password pattern."
echo
echo "Found ${#FILES_WITH_SSID_1[@]} files with password pattern."
echo
echo "Found ${#FILES_WITH_SSID_2[@]} files with password pattern."
echo


for file in "${FILES_WITH_PASSWORD[@]}"; do
	# echo "- $file"
	filename="$(basename "$file")"
	# start_spinner 120 "Redacting password in $filename"
	echo "Redacting password in $file"
	sed -i -E "s/${password_pattern}/WIFI_PASSWORD/g" "$file" # &>/dev/null 
done

for file in "${FILES_WITH_SSID_1[@]}"; do
	# echo "- $file"
	filename="$(basename "$file")"
	# start_spinner 120 "Redacting password in $filename"
	echo "Redacting password in $file"
	sed -i -E "s/${ssid_pattern_1}/WIFI_SSID/g" "$file" # &>/dev/null 
done

for file in "${FILES_WITH_SSID_2[@]}"; do
	# echo "- $file"
	filename="$(basename "$file")"
	# start_spinner 120 "Redacting password in $filename"
	echo "Redacting password in $file"
	sed -i -E "s/${ssid_pattern_2}/WIFI_SSID2/g" "$file" # &>/dev/null 
done


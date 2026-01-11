#!/bin/bash

clear
. ~/.env.colors
. ~/.bash_utils --no-exec &>/dev/null

declare -A PIDS
declare -a HOSTS
declare -a NEW_DIRS
declare -a ARCHIVED_DIRS
declare -a PATTERNS_TO_FIND

# edit this array
PATTERNS_TO_FIND=(
	'_ONOFF_'
	'WEBSERVER_'
	'WIFI_WEBSERVER'
	'Webserver'
)

HOSTS=(
	officewsl
	laptopwsl
	server
	dellserver
)

################## NO EDITTING BELOW THIS LINE ##################

cleanup() {
	local exit_code=${1:-$?}
	stop_spinner
	ssh_socket_cleanup_all
	echo ""
	if [[ $exit_code -eq 0 ]]; then
		echo "$SUCCESS Cleaned up."
	else
		echo "$FAILED Interrupted. Cleaned up."
	fi
	trap - EXIT INT TERM TSTP
	exit $exit_code
}

trap 'cleanup' EXIT INT TERM TSTP

ARCHIVED_DIRS=(
	"_1buttonAndPingTest"
	"ADAFRUIT_CC3000"
	"ADVANCED_WEB_Button"
	"ANALYS_IR"
	"anotherWEbServer"
	"Arduino"
	"a_web_page_with_a_text_box"
	"backup"
	"BACKUPS"
	"BasicOTA"
	"Bridge"
	"bridge2"
	"buildtest"
	"Button1"
	"CC3000_BUILD_TEST"
	"CC3000_MEBO_CONTROL"
	"CC3000_WIFI_WEBSERVER"
	"CC3000_WIFI_WEBSERVER_2"
	"CSP_Watchdog_Backup_24_6_2020"
	"Dragino_YUN_TEST"
	"ENCODER_MOTOR"
	"ENCODER_MOTOR_2"
	"ENCODER_MOTOR_3"
	"ENCODER_MOTOR_4"
	"ENCODER_MOTOR_5"
	"ENCODER_MOTOR_6"
	"ESP32_ENCODER_SimpleCounter"
	"ESP32_Interrupt_Counter"
	"ESP_32_Interrupt_Trigger"
	"ESP32_KOBRA_2020_BACKUP_4.12.2020"
	"ESP32_ST_Anything_Everything_TEST"
	"ESP32_TEST_IR"
	"ESP8266_DOOR_INTERCOM_BELL - Copy"
	"esp8266_sketch"
	"ESP8266_WINDOW_OFFICE_SOUTH_Stepper_Motor_NOT_GOOD"
	"ESP_interrupt"
	"ESP_NTPClient_Advanced_WIFI_SSID"
	"ESP_ultrasonic"
	"ESPWebServer.html"
	"EspWifi_WebServerLed"
	"eternal_sunshine_backup_august_13__2020"
	"how_to_change_a_character_String_into_an_integer"
	"httpUpdate"
	"HTTP_UPDATE_TEST"
	"IR_decoder"
	"IR_decoder_2"
	"IR_decoder_3_shows_formated_codes"
	"IR_Receiver_MEBO"
	"IRsendDemo"
	"IR_TV_copy"
	"LinkNode_iLife_Vacuum_Switch_WebServer"
	"LinkNode_Switch_WebServer"
	"locator"
	"MEBO_ARCHIVE"
	"MEBO_BACKUPS"
	"MEBO_MAIN_BACKUP_April_8_2021"
	"Morse"
	"NANO_WIFI_1st_attempt"
	"NANO_WIFI_2nd_attempt"
	"NodeMCU"
	"OnEssayeEncore"
	"OnEssayeEncore2"
	"Ping_and_turn_on_or_off"
	"platePuhser"
	"RESET_NANO_BOARD"
	"scanner"
	"simpleADCcounter"
	"SimpleWiFiServer"
	"SimpleWiFiServer_ESP32"
	"Sketches_Examples_St_Anything"
	"sketch_jul29b"
	"sketch_jun14a"
	"sketch_mar9a"
	"sketch_may14a"
	"SmartThings_ESP8266WiFi_TEST"
	"SP8266_Sprite_Server"
	"SSID_RSSI_SCAN_good"
	"ST_Anything_Multiples_MEGAWiFiEsp"
	"STEPPER"
	"STEPPER_motor_D42HSC4409B"
	"STEPPER_motor_D42HSC4409B_UNO"
	"Stepper_test"
	"sweep"
	"TCP_Write_NEATO"
	"TCP_Write_NEATO_2"
	"TCP_Write_NEATO__iLife"
	"TCP_Write_NEATO__MEBO"
	"TemperatureWebPanel"
	"TEST"
	"TEST_360_FEEDBACK"
	"TEST_CAT_FEEDER_3"
	"test_IR_ac_bedroom"
	"test_java_buttons"
	"UDP_Write_NEATO"
	"UDP_Write_NEATO_2"
	"WakeOnLan"
	"WebServer1"
	"web_server_4"
	"web_server_4.2"
	"webserver_5"
	"webserver_6"
	"webserver7"
	"WEBSERVER_CATFEEDER_ATMega.ino"
	"WEBSERVER_CATFEEDER_ATMega_OLD"
	"WEBSERVER_CATFEEDER_ATMega_TEST"
	"WEBSERVER_CATFEEDER_ATMega_TEST_CC3000"
	"WEBSERVER_CATFEEDER_ATMega_TEST_YUN"
	"WEBSERVER_CONDITIONAL_ONOFF_Buzer_trial"
	"WEBSERVER_CONDITIONAL_ONOFF_OLD"
	"WebServer_esp13_Shield_TEST"
	"WebServerESPWIFI"
	"WebServer_EXAMPLE_CC3000"
	"WebserverTEST_F_MACRO"
	"WebserverTEST_F_MACRO_2"
	"WebserverTEST_F_MACRO___JAVABUTTONS___SUBMIT_TEXTBOX"
	"WebServerWith4ButtonPowerOFFandON_works"
	"WebServerWithBUTTON"
	"WebServerWithBUTTON3"
	"WiFiRSSI"
	"WIFI_SCAN"
	"WIFI_SCAN_good_2"
	"WIFI_WEBSERVER_ESP32_COMPUTER_OFFICE_TEST"
	"WIFI_WEBSERVER_ESP32_SMARTTHINGS_TEST"
	"WIFI_WEBSERVER_ESP8266_AC_BEDROOM_PROGMEM_TEST"
	"WiFiWebServer_WizFi"
	"WINDOW_BEDROOM_STEPPER"
)

echo "${ARCHIVED_DIRS[@]}"

for pattern in "${PATTERNS_TO_FIND[@]}"; do
	NEW_DIRS=()
	echo "searching pattern: $pattern"
	readarray NEW_DIRS < <(find "$HOME/0_ARDUINO" -maxdepth 1 -type d -name "*${pattern}*")

	if [ "${#NEW_DIRS[@]}" -eq 0 ]; then
		continue
	else
		for new in "${NEW_DIRS[@]}"; do
			new_d="$(basename "$new")"
			echo -e "$ORANGE adding ${new_d} $NC to ARCHIVED_DIRS array"
			ARCHIVED_DIRS+=("$new_d")
		done
	fi
done

for a in "${ARCHIVED_DIRS[@]}"; do
	echo -e "about to remove: "
	echo -e "$BLUE       - $a"
	echo -e "$NC from $(pwd)"
done

# local cleanup
for dir in "${ARCHIVED_DIRS[@]}"; do
	if [[ -d "$HOME/0_ARDUINO/$dir" && ! -d "$HOME/0_ARDUINO/ARCHIVE/$dir" ]]; then

		echo -e "$RED" "Archiving ${CYAN}$dir to ${ACCENT_YELLOW}./ARCHIVE/" "$NC"

		output=$(mv -f "$HOME/0_ARDUINO/$dir" "$HOME/0_ARDUINO/ARCHIVE/$dir")

		echo -e "$output"

	else
		if [[ -d "$HOME/0_ARDUINO/$dir" && -d "$HOME/0_ARDUINO/ARCHIVE/$dir" ]]; then
			output=$(cp -r "$HOME/0_ARDUINO/$dir" "$HOME/0_ARDUINO/ARCHIVE/$dir")
			echo "$output"
			if [ -z "$output" ]; then
				rm -r "$HOME/0_ARDUINO/$dir"
				echo -e "${RED}$dir removed $CHECKED$NC"
			fi
		elif [ -d "$HOME/0_ARDUINO/ARCHIVE/$dir" ]; then
			echo -e "${CYAN}$dir${GREEN} already in ./ARCHIVE $CHECKED$NC"
		else
			echo -e "$ERROR$BG_RED" "$dir is nowhere to be found... $FAILED$NC"
		fi
	fi
done
echo ""
echo -e "$BG_GREEN Local removal $NC$CHECKED"
echo ""
echo -e "$FLASH_ACCENT_YELLOW"
repeat_print "═"
echo ""
rsync -auz --info=progress2 --delete --exclude='*.git' ~/0_ARDUINO/ /mnt/h/OneDrive/Documents/Arduino/ &>/dev/null &
</dev/null &
echo "rsync --delete server in progress in the background"
rsync -auz --info=progress2 --delete --exclude='*.git' ~/0_ARDUINO/ server:0_ARDUINO/ &>/dev/null &
</dev/null &
echo "rsync --delete dellserver in progress in the background"
rsync -auz --info=progress2 --delete --exclude='*.git' ~/0_ARDUINO/ dellserver:0_ARDUINO/ &>/dev/null &
</dev/null &
echo "rsync --delete laptopwsl in progress in the background"
rsync -auz --info=progress2 --delete --exclude='*.git' ~/0_ARDUINO/ laptopwsl:0_ARDUINO/ &>/dev/null &
</dev/null &
repeat_print "═"
echo
echo -e "$NC"

echo "done $CHECKED"
exit 0

start_spinner 20 "Establishing ssh sockets..."
for host in "${HOSTS[@]}"; do
	if timeout 2 ssh -q -o ConnectTimeout=2 "$host" "exit 0" >/dev/null; then
		ssh_socket_start "$host" >/dev/null &
	else
		echo -e "$BG_RED" "$host seems offline$NC"
	fi
done
stop_spinner

for host in "${HOSTS[@]}"; do
	if [[ "$host" == "$(hostname)" ]]; then
		continue
	fi
	echo -e "$ORANGE" "$host" "$NC"
	if timeout 2 ssh -q -o ConnectTimeout=2 "$host" "exit 0" >/dev/null; then
		for dir in "${ARCHIVED_DIRS[@]}"; do

			(
				echo -e "${CYAN}Archiving $dir on $ORANGE$host$NC"

				if ! ssh_with_socket "$host" "rm -rf \$HOME/0_ARDUINO/$dir" &>/dev/null; then
					echo "DELAYING $dir to $host"
					(
						sleep 1
						if ! ssh_with_socket "$host" "rm -rf \$HOME/0_ARDUINO/$dir" &>/dev/null; then
							echo -e "$ERROR                $BG_RED failed to archive $dir on $host $NC"
						else
							echo -e "${BG_BLUE}DELAYED $dir to $host $SUCCESS$NC"
						fi
					) &
				else
					echo -e "$dir $SUCCESS"
				fi
				sleep 0.1
			) &

		done
	else
		echo ""
		echo -e "$BG_RED$BLACK" "$host unreachable" "$NC"
	fi

done

wait

trap - INT TERM
stop_spinner 2>/dev/null
ssh_socket_cleanup_all 2>/dev/null
echo "$SUCCESS Done."

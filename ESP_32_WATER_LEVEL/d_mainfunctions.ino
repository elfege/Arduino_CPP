
void mainHandler() {
  // WiFiHandler();
  checkTemperature();
  httpHandler();
  blinker();
  yield();
}

void sendToHub(String var) {
  st::Everything::sendSmartString(var);
}

// 1 = no water: input_pullup so 0 when up because sensor closes circuit (so current goes to GND)
boolean level_is_low() {
  const unsigned long debounceTime = 2000;  // 2 seconds
  const unsigned long samplingInterval = 100;
  const int requiredSamples = (debounceTime / samplingInterval);
  int consistentHighCount = 0;
  unsigned long lastSampleTime = 0;
  unsigned long startTime = millis();

  while (millis() - startTime < debounceTime) {
    if (millis() - lastSampleTime >= samplingInterval) {
      
      lastSampleTime = millis();
      if (digitalRead(sensor) == HIGH) {  // No water detected
        consistentHighCount++;
      } else {
        consistentHighCount = 0;  // Reset on any LOW reading (water high)
      }
    }
    blinker();
    _server.handleClient();
    term.handleClient();
  }

  return (consistentHighCount >= requiredSamples);
}

void httpHandler() {
  st::Everything::run();
  _server.handleClient();
  term.handleClient();  // WiFi terminal
  yield();
}

void blinker() {
  // heartbeat-like blinking as long as water level is high

  if (!digitalRead(sensor)) {  // water level is high (GPIO value is low)

    unsigned long currentMillis = millis();

    switch (currentState) {
      case LUB:
        if (currentMillis - previousMillisBlink >= heartbeatInterval1) {
          digitalWrite(LED, HIGH);  // LED ON
          previousMillisBlink = currentMillis;
          currentState = PAUSE_AFTER_LUB;
        }
        break;

      case PAUSE_AFTER_LUB:
        if (currentMillis - previousMillisBlink >= heartbeatInterval2) {
          digitalWrite(LED, LOW);  // LED OFF
          previousMillisBlink = currentMillis;
          currentState = DUB;
        }
        break;

      case DUB:
        if (currentMillis - previousMillisBlink >= heartbeatInterval3) {
          digitalWrite(LED, HIGH);  // LED ON
          previousMillisBlink = currentMillis;
          currentState = PAUSE_AFTER_DUB;
        }
        break;

      case PAUSE_AFTER_DUB:
        if (currentMillis - previousMillisBlink >= pauseInterval) {
          digitalWrite(LED, LOW);  // LED OFF
          previousMillisBlink = currentMillis;
          currentState = LUB;
        }
        break;
    }
  } else {
    digitalWrite(LED, 1);  // keep led off when level is low (GPIO value is high)
  }
}

void messageCallout(String message) {

  term.print("Received message: '");
  term.print(message);
  term.println("' ");
}

float celsiusToFahrenheit(float celsius) {
  return celsius * 9.0 / 5.0 + 32.0;
}

float readTemperature() {
  const int READ_DURATION_MS = 3000;  // Duration for taking readings (3 seconds)
  const int READ_INTERVAL_MS = 100;   // Interval between readings (100 ms)

  int numReadings = READ_DURATION_MS / READ_INTERVAL_MS;
  float temperatureSum = 0;
  int readingsCount = 0;

  unsigned long startTime = millis();

  while (millis() - startTime < READ_DURATION_MS) {
    int adcValue = analogRead(TEMP_SENSOR_PIN);
    float voltage = adcValue * 3.3 / 4095.0;
    float thermistor_resistance = SERIES_RESISTOR * voltage / (3.3 - voltage);

    float steinhart = log(thermistor_resistance / THERMISTOR_R0) / THERMISTOR_B;
    steinhart += 1.0 / THERMISTOR_T0;
    float temperatureC = 1.0 / steinhart - 273.15;
    float temperatureF = celsiusToFahrenheit(temperatureC);

    // Apply calibration offset
    temperatureF += TEMP_OFFSET;

    // Add to sum and increment count
    temperatureSum += temperatureF;
    readingsCount++;

    int delayTime = READ_INTERVAL_MS;
    // Wait for the specified interval before the next reading
    while (delayTime > 0) {
      yield();
      delay(1);  // Delay in 1 ms increments
      delayTime--;
    }
  }

  // Calculate average temperature
  float averageTemperature = temperatureSum / readingsCount;

  // Print debug information
  term.println("--------------------");
  term.println("Total Readings: " + String(readingsCount));
  term.println("Average Temperature: " + String(averageTemperature, 2) + "°F");

  return averageTemperature;
}


void checkTemperature() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastTempCheck >= tempCheckInterval) {
    float currentTemp = readTemperature();
    lastTempCheck = currentMillis;

    if (lastReportedTemp == 0.0 || abs(currentTemp - lastReportedTemp) >= TEMP_THRESHOLD) {
      lastReportedTemp = currentTemp;

      // Send temperature to hub
      String tempString = String(currentTemp, 2);
      sendToHub("temperature " + tempString);

      term.println("Reported new temperature to hub: " + tempString + "°F");
    } else {
      term.println("Temperature change less than threshold. Not reporting.");
    }

    temperature = currentTemp;  // Update the global temperature variable
  }
}

void testADC() {
  int adcValue = analogRead(TEMP_SENSOR_PIN);
  float voltage = adcValue * (3.3 / 4095.0);

  Serial.println("ADC Pin: " + String(TEMP_SENSOR_PIN));
  Serial.println("ADC Value: " + String(adcValue));
  Serial.println("Voltage: " + String(voltage, 3) + "V");
  Serial.println("--------------------");
}

void Reset() {
  term.println("RESETTING ESP................");
  term.handleClient();
  delay(2000);
  ESP.restart();
}

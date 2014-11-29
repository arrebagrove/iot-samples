// Copyright Victor Hurdugaci (http://victorhurdugaci.com). All rights reserved.
// Licensed under the Apache License, Version 2.0. See LICENSE in the project root for license information.

// See https://github.com/victorhurdugaci/SerialProtocol for
// installing Serial Protocol
#include <ArduinoSerialProtocol.h>

#include <LiquidCrystal.h> 

const int MOISTURE_PIN_ON_D = 8;
const int MOISTURE_PIN_A = 2;

const int LIGHT_PIN_A = 1;

struct Sensors 
{
  uint16_t moisture;
  uint16_t light;
} sensorData;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
ArduinoSerialProtocol protocol(&Serial, (uint8_t*)&sensorData, sizeof(sensorData));

char display_buffer[20] = "";

void setup() 
{
  pinMode(MOISTURE_PIN_ON_D, OUTPUT); 
  lcd.begin(20,4);
  
  Serial.begin(9600);
}

void loop() 
{
  // Read moisture
  digitalWrite(MOISTURE_PIN_ON_D, HIGH);
  delay(20);
  sensorData.moisture = analogRead(MOISTURE_PIN_A);
  digitalWrite(MOISTURE_PIN_ON_D, LOW);
  
  // Read light
  sensorData.light = analogRead(LIGHT_PIN_A);
  
  lcd.clear();
  
  sprintf(display_buffer, "Light: %d", sensorData.light);
  lcd.print(display_buffer);
  
  sprintf(display_buffer, "Water: %d", sensorData.moisture);
  lcd.setCursor(0,1);
  lcd.print(display_buffer);
  
  // Send data over serial using SerialProtocol
  protocol.send();
  
  // Wait 60 seconds until the next sensor read
  delay(60000);
}

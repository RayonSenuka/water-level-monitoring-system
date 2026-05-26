/************************************************************
 * WATER LEVEL MONITOR                                      *
 * Uses the Ultrasonic Sensor to measure the level of water *
 * in a tank & sound a buzzer & turn on the pump when empty *
 ************************************************************/

#include <HTTP_Method.h>
#include <Uri.h>
#include <WebServer.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include "waterlevel.h"


// Pin Definitions
#define LED_RED D2           // Red LED pin
#define TRIG_PIN D3          // Ultrasonic Sensor's TRIG pin
#define ECHO_PIN D4          // Ultrasonic Sensor's ECHO pin
#define LED_YELLOW D5        // Yellow LED pin
#define PUMP_PIN D7          // Pump pin
#define RESET_BUTTON_PIN D9  // Reset Button Pin
#define BUZZER_PIN D10       // Buzzer pin
#define BUTTON_PIN D11       // Push button pin
#define LED_GREEN D12        // Green LED pin




// Variable Definitions
float duration_us, distance_cm;
int empty_level, full_level;
int button_state = 0;
int pump_state = 0;                   // Pump status, 0 = off, 1 = running
int toggle = 0;
int water_level_pcnt = 0;
int water_level_prevpcnt = 0;
bool calibrating = false;
const char* ssid = "Raiya";
const char* password = "rayon1689S";
WebServer server(80);
LiquidCrystal_I2C lcd(0x27,16,2);


// Root Page
void handleRoot() {
 String homePage = MAIN_page; //Read HTML contents
 server.send(200, "text/html", homePage); //Send web page
}


void handleADC() {
  String adcValue = String(water_level_pcnt);
  server.send(200, "text/plain", adcValue); //Send water level percentage value
}



//===============================================================
// Setup
//===============================================================
void setup() {
  Serial.begin (9600);

  // Pin Assignments
    pinMode(TRIG_PIN, OUTPUT);                // Configure the trigger pin to output mode
    pinMode(ECHO_PIN, INPUT);                 // Configure the echo pin to input mode
    pinMode(LED_GREEN, OUTPUT);               // Configure the Green LED pin
    pinMode(LED_YELLOW, OUTPUT);              // Configure the Yellow LED pin
    pinMode(LED_RED, OUTPUT);                 // Configure the Red LED pin
    pinMode(BUTTON_PIN, INPUT_PULLUP);        // Configure the push button pin
    pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);  // Configure the push button pin
    pinMode(BUZZER_PIN, OUTPUT);              // Configure the buzzer pin
    pinMode(PUMP_PIN, OUTPUT);                // Configure the pump pin



  // On startup read Empty/Full Levels from EEPROM
    empty_level = EEPROM.read(0);
    full_level = EEPROM.read(1);
    if(empty_level == 0 and full_level == 0) {
      calibrating = true;
    }

  // BEGIN: LCD Display Segment
    lcd.init();       // Initialise the display
    lcd.clear();      // Clear the display
    lcd.backlight();  // Turn on the backlight
  // END: LCD Display Segment



  // BEGIN: Wi-Fi and WebServer Segment
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.print("Arduino Nano ESP32 Web Server's IP address: ");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot); // Home Page
    server.on("/readADC", handleADC); // Get update of water level  
    server.begin();
  // END: Wi-Fi and WebServer Segment

  pump_state = 0;
  water_level_prevpcnt = 0;
  water_level_pcnt = 0;
  digitalWrite(PUMP_PIN, LOW);  // Turn pump off on startup
}


//===============================================================
// Loop
//===============================================================
void loop() {
  // BEGIN: Ultrasonic Sensor Segment
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
  
    duration_us = pulseIn(ECHO_PIN, HIGH);                // Measure the pulse duration from the ECHO pin
    // distance_cm = 0.017 * duration_us;                 // calculate the distance
    distance_cm = ((int)((0.017 * duration_us) * 10));    // Used for demonstration purposes only.

    if (empty_level > 0 && full_level > 0) {              // Calculate the Water Level Percentage Value
      water_level_pcnt = ((distance_cm - empty_level)/(full_level - empty_level) * 100);
      if (water_level_pcnt <= 0 && water_level_prevpcnt <= 5) {                        // If percentage is less than zero, set it to zero
        water_level_pcnt = 0;
      }
      if (water_level_pcnt >= 100 && water_level_prevpcnt >= 95) {                      // If percentage is greater than 100, set it to 100
        water_level_pcnt = 100;
      }

      // START: Code segment to handle misreadings from the ultrasonic sensor
      if (calibrating == false) {
        if (pump_state == 1) {
          // Pump is ON, so percentage shouldn't decrease
          if (water_level_pcnt < water_level_prevpcnt) {
            water_level_pcnt = water_level_prevpcnt;
          } else {
            water_level_prevpcnt = water_level_pcnt;
          }
        } else {
          // Pump is OFF, so percentage shouldn't increase
          if (water_level_pcnt > water_level_prevpcnt) {
            water_level_pcnt = water_level_prevpcnt;
          } else {
            water_level_prevpcnt = water_level_pcnt;
          }
        }
      }
      // END: Code segment to handle misreadings from the ultrasonic sensor
    }

    Serial.print("Empty: ");
    Serial.print(empty_level);
    Serial.print(", Full: ");
    Serial.print(full_level);
    Serial.print(" Reading: ");
    Serial.print(distance_cm);
    Serial.print(" Pcnt: ");
    Serial.print(water_level_pcnt);
    Serial.print(" PrevPcnt: ");
    Serial.println(water_level_prevpcnt);
  // END: Ultrasonic Sensor Segment

  if(calibrating) { // Device is in calibration mode
    button_state = digitalRead(BUTTON_PIN);

    if (empty_level == 0) {
        lcd.init();
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("PUSH BUTTON TO");
        lcd.setCursor(0,1);
        lcd.print("SET EMPTY LEVEL");
        Serial.print("Empty - ");
        Serial.print(button_state);
        if(button_state == 0) {
          Serial.print("Button - ");
          Serial.print(button_state);
          Serial.println(distance_cm);
          EEPROM.write(0, (int)distance_cm);
          empty_level = (int)distance_cm;
          EEPROM.commit();
        }
    } else {
      if (full_level == 0) {
        lcd.init();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("PUSH BUTTON TO");
        lcd.setCursor(0,1);
        lcd.print("SET FULL LEVEL");
        if(button_state == 0) {
          EEPROM.write(1, (int)distance_cm);
          full_level = (int)distance_cm;
          EEPROM.commit();
          calibrating = false;
        }        
      } else {
        calibrating = false;
      }
    }
  } else { // Device is in normal running mode
    button_state = digitalRead(BUTTON_PIN);
    if(button_state == 0) {  // Reset device
      EEPROM.write(0, 0);
      EEPROM.write(1, 0);
      full_level = 0;
      empty_level = 0;
      EEPROM.commit();
      calibrating = true;
    }      

    // BEGIN: LCD Display Segment
      if(calibrating == false) { // Condition to show the water level on the LCD display
        lcd.init();
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("WATER LEVEL");
        lcd.setCursor(6,1);
        lcd.print(water_level_pcnt);  
        lcd.print("%");
      }
    // END: LCD Display Segment


    // BEGIN: LED Bulb Segment
      if(water_level_pcnt <= 0) { // Condition to turn on the red LED
        if (pump_state == 0) { // Turn on Pump
          pump_state = 1;
          digitalWrite(PUMP_PIN, HIGH);
        }
        if (toggle == 0) { // Blink red LED
          digitalWrite(LED_RED, HIGH);
          digitalWrite(BUZZER_PIN, HIGH);
          toggle = 1;
        } else {
          digitalWrite(LED_RED, LOW);
          digitalWrite(BUZZER_PIN, LOW);
          toggle = 0;
        }
      } else {
        digitalWrite(LED_RED, LOW);
        digitalWrite(BUZZER_PIN, LOW);
      }

      if(water_level_pcnt >= 100) { // Condition to turn on the green LED
        if (pump_state == 1) { // Turn off Pump
          pump_state = 0;
          digitalWrite(PUMP_PIN, LOW);
        }
        digitalWrite(LED_GREEN, HIGH); 
      } else {
        digitalWrite(LED_GREEN, LOW);
      }
      
      if (pump_state == 1) {  // If the punp is on, turn on the yellow LED
        digitalWrite(LED_YELLOW, HIGH);
      } else {
        digitalWrite(LED_YELLOW, LOW);
      }
    // END: LED Bulb Segment

    server.handleClient();
  }
  delay(0.5);
}
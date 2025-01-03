#include <SoftwareSerial.h>
#include "LowPower.h"

// Pin Definitions
const int buttonPin = 2;     // Button connected to pin 2
const int mosfetPin = 6;     // MOSFET control pin connected to pin 6
const int ledPin = 13;        // LED for indication
bool buttonPressed = false;  // Tracks if button is pressed

// Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(3, 4); // SIM800L Tx & Rx connected to Arduino pins #3 & #4

char buff[10]; // Buffer for voltage string

// Interrupt Service Routine (ISR) for waking up the Arduino
void wakeUp() {
  buttonPressed = true; // Set the flag to indicate button press
}

void setup() {
  // Configure pins
  pinMode(buttonPin, INPUT_PULLUP); // Button with pull-up resistor
  pinMode(mosfetPin, OUTPUT);       // MOSFET control pin
  pinMode(ledPin, OUTPUT);          // LED pin for feedback
  digitalWrite(mosfetPin, LOW);     // Ensure MOSFET starts off
  digitalWrite(ledPin, LOW);        // Ensure LED starts off

  // Begin serial communication
  Serial.begin(9600);
  mySerial.begin(9600);

  Serial.println("System Initialized. Waiting for button press...");
}

void loop() {
  // Attach interrupt to the button pin for wake-up
  attachInterrupt(digitalPinToInterrupt(buttonPin), wakeUp, FALLING);

  // Enter power-down sleep mode until button is pressed
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

  // Wakes up here when the button is pressed
  detachInterrupt(digitalPinToInterrupt(buttonPin)); // Disable interrupt
  if (buttonPressed) {
    Serial.println("Button Pressed! Waking up...");
    buttonPressed = false;

    // Indicate activity with LED
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);

    // Power on SIM module
    digitalWrite(mosfetPin, HIGH);
    Serial.println("Powering on SIM module...");
    delay(2000); // Allow SIM module to stabilize

    // Check network connection and send SMS
    if (waitForNetwork()) {
      float batteryVoltage = readBatteryVoltage();
      sendSMS(batteryVoltage); // Send SMS with battery voltage
      delay(10000); // Wait a few seconds after sending the SMS
    }

    // Turn off SIM module
    digitalWrite(mosfetPin, LOW);
    Serial.println("SIM module powered off.");

    // Indicate return to sleep mode
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
  }
}

// Function to wait for the SIM module to connect to the network
bool waitForNetwork() {
  Serial.println("Checking network connection...");
  for (int i = 0; i < 30; i++) { // Retry up to 30 seconds
    mySerial.println("AT+CREG?");
    delay(1000);
    while (mySerial.available()) {
      String response = mySerial.readString();
      Serial.println("Network Check Response: " + response);

      // Check if the module is registered to the network
      if (response.indexOf("+CREG: 0,1") > -1 || response.indexOf("+CREG: 0,5") > -1) {
        Serial.println("Network Connected!");
        return true;
      }
    }
    Serial.println("Waiting for network...");
  }
  Serial.println("Network connection failed.");
  return false;
}

// Function to send an SMS with the battery voltage
void sendSMS(float batteryVoltage) {
  Serial.println("Sending SMS...");
  mySerial.println("AT+CMGF=1"); // Set module to SMS text mode
  updateSerial();

  mySerial.println("AT+CMGS=\"+00000000000\""); // Replace with your phone number
  updateSerial();

  // Send the battery voltage in the message
  mySerial.print("Battery Voltage: ");
  dtostrf(batteryVoltage, 4, 3, buff); // Convert float to string
  mySerial.print(buff);
  mySerial.println("V");
  mySerial.write(26); // Ctrl+Z to send the SMS
  Serial.println("SMS Sent!");
}

// Function to forward serial data for debugging
void updateSerial() {
  delay(500);
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}

// Function to read the battery voltage using the voltage divider
float readBatteryVoltage() {
  // Read normal Arduino value
  int in0 = analogRead(A0);
  float val0 = in0 * 5.0 / 1024.0;

  // Read correct supply voltage
  float supply = readVcc() / 1000.0;
  float val0Corrected = supply / 5 * val0;

  // Return corrected voltage
  return val0Corrected;
}

// Function to read the supply voltage
long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
  ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  ADMUX = _BV(MUX3) | _BV(MUX2);
#else
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA, ADSC));
  result = ADCL;
  result |= ADCH << 8;
  result = 1126400L / result; // Calculate Vcc (in mV); 1126400 = 1.1*1024*1000
  return result;
}

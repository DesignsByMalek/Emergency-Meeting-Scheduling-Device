#include <SoftwareSerial.h>
#include "LowPower.h"

// Pin Definitions
const int buttonPin = 2;     // Button connected to pin 2
const int mosfetPin = 6;     // MOSFET control pin connected to pin 6
const int ledPin = 13;        // LED for indication
bool buttonPressed = false;  // Tracks if button is pressed
String ClientID = "001";     // for scalability change the client button if there are many buttons. 
// Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(3, 4); // SIM800L Tx & Rx connected to Arduino pins #3 & #4

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
  //Serial.begin(9600);
  mySerial.begin(9600);

  //Serial.println("System Initialized. Waiting for button press...");
}

void loop() {
  // Attach interrupt to the button pin for wake-up
  attachInterrupt(digitalPinToInterrupt(buttonPin), wakeUp, FALLING);

  // Enter power-down sleep mode until button is pressed
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

  // Wakes up here when the button is pressed
  detachInterrupt(digitalPinToInterrupt(buttonPin)); // Disable interrupt
  if (buttonPressed) {
    //Serial.println("Button Pressed! Waking up...");
    buttonPressed = false;

    // Indicate activity with LED
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);

    // Power on SIM module
    digitalWrite(mosfetPin, HIGH);
    //Serial.println("Powering on SIM module...");
    delay(2000); // Allow SIM module to stabilize

    // Check network connection and send SMS
    if (waitForNetwork()) {
      sendSMS();
      delay(5000); // Wait a few seconds after sending the SMS
    }

    // Turn off SIM module
    digitalWrite(mosfetPin, LOW);
    //Serial.println("SIM module powered off.");

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
      //Serial.println("Network Check Response: " + response);

      // Check if the module is registered to the network
      if (response.indexOf("+CREG: 0,1") > -1 || response.indexOf("+CREG: 0,5") > -1) {
        //Serial.println("Network Connected!");
        return true;
      }
    }
    //Serial.println("Waiting for network...");
  }
  //Serial.println("Network connection failed.");
  return false;
}

// Function to send an SMS
void sendSMS() {
  //Serial.println("Sending SMS...");
  mySerial.println("AT+CMGF=1"); // Set module to SMS text mode
  updateSerial();

  mySerial.println("AT+CMGS=\"+000000000\""); // Replace with your phone number
  updateSerial();

  mySerial.print("Message from Arduino: Button was pressed!"); //  //replace message with "001,BUTTON_PRESS,100%" when using with server to send emails. Change Client ID if there are more than 1 buttons
  // event type is either a button press or a check battery. the check battery is for future improvements not included in this code, where the arduino can monitor the battery voltage. another code is provided for monitoring battery health.
  mySerial.write(26); // Ctrl+Z to send the SMS
  //Serial.println("SMS Sent!");
}

// Function to forward serial data for debugging
void updateSerial() {
  delay(500);
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}

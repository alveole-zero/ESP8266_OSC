#define DEBUG_LEVEL 1

#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCBoards.h>
#include <OSCData.h>
//#include <SLIPEncodedUSBSerial.h>
//#include <SLIPEncodedSerial.h>
#include <OSCMatch.h>
#include <OSCTiming.h>

#include <FS.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ArduinoJson.h>  //Maximum version is with this code is 5.13 see https://articles.aquaiver.com/esp8266-nodemcu-arduino-ide-dynamicjsonbuffer-was-not-declared-in-this-scope/
                          //https://github.com/bblanchon/ArduinoJson

#include <Stepper.h>
const int stepsPerRevolution = 200;
Stepper myStepper(stepsPerRevolution, D5, D6, D7, D8);

#include <WiFiUdp.h>

WiFiUDP Udp;

const int recv_port = 10000;
const int send_port = 12000;

//needed for library
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>          //https://github.com/kentaylor/WiFiManager

// Constants

// select wich pin will trigger the configuraton portal when set to LOW
// ESP-01 users please note: the only pins available (0 and 2), are shared 
// with the bootloader, so always set them HIGH at power-up
// Onboard LED I/O pin on NodeMCU board
const int PIN_LED = 2; // D4 on NodeMCU and WeMos. Controls the onboard LED.
/* Trigger for inititating config mode is Pin D3 and also flash button on NodeMCU
 * Flash button is convenient to use but if it is pressed it will stuff up the serial port device driver 
 * until the computer is rebooted on windows machines.
 */     
const int TRIGGER_PIN = 0; // D3 on NodeMCU and WeMos.
/*
 * Alternative trigger pin. Needs to be connected to a button to use this pin. It must be a momentary connection
 * not connected permanently to ground. Either trigger pin will work.
 */

const char* CONFIG_FILE = "/config.json";

// Variables

// Indicates whether ESP has WiFi credentials saved from previous session
bool initialConfig = false;

// Default configuration values
char thingspeakApiKey[17] = "";
bool sensorDht22 = true;
unsigned int pinSda = 2;
unsigned int pinScl = 14;

// Function Prototypes

bool readConfigFile();
bool writeConfigFile();

// Setup function
void setup()
{
  // Put your setup code here, to run once
  Serial.begin(115200);
  Serial.println("\n Starting");

  // Initialize the LED digital pin as an output.
  pinMode(PIN_LED, OUTPUT);
  pinMode(D1, OUTPUT);
  // Initialize trigger pins
  pinMode(TRIGGER_PIN, INPUT_PULLUP);

  // Mount the filesystem
  bool result = SPIFFS.begin();
  if (DEBUG_LEVEL >=1 ) Serial.println("SPIFFS opened: " + result);

  if (!readConfigFile())
  {
    Serial.println("Failed to read configuration file, using default values");
  }

  //WiFi.printDiag(Serial); //Remove this line if you do not want to see WiFi password printed

  if (WiFi.SSID() == "")
  {
    Serial.println("We haven't got any access point credentials, so get them now");
    initialConfig = true;
  }
  else
  {
    digitalWrite(PIN_LED, HIGH); // Turn LED off as we are not in configuration mode.
    WiFi.mode(WIFI_STA); // Force to station mode because if device was switched off while in access point mode it will start up next time in access point mode.
    unsigned long startedAt = millis();
    if (DEBUG_LEVEL >=1 ) Serial.print("After waiting ");
    int connRes = WiFi.waitForConnectResult();
    float waited = (millis()- startedAt);
    if (DEBUG_LEVEL >=1 ) Serial.print(waited/1000);
    if (DEBUG_LEVEL >=1 ) Serial.print(" secs in setup() connection result is ");
    if (DEBUG_LEVEL >=1 ) Serial.println(connRes);
  }

  if (WiFi.status()!=WL_CONNECTED)
  {
    Serial.println("Failed to connect, finishing setup anyway");
  }
  else
  {
    Serial.print("Local ip: ");
    Serial.println(WiFi.localIP());
  }
  Udp.begin(recv_port);
  myStepper.setSpeed(60);
}  

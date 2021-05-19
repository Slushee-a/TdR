/*
 / I'd rather add too many comments than too few.
 / Code is easier to write than it is to read, so let's not risk it. 
 / Made by Slushee (Pol Fernàndez)
 / Alpha 1.8.3 (18/05/2021)
 */

#include <WiFi.h>                               // Load WiFi library (Part of the ESP family of libraries)
#include <Servo_ESP32.h>                        // Load Servo library (Part of the ESP family of libraries)
#include <WiFiManager.h>                        // Load WiFi manager library (By tzapu on Github: https://github.com/tzapu/WiFiManager)
#include "ESPmDNS.h"                            // Load mDNS library (Part of the ESP family of libraries)

const byte MotorPin1 = 2;                       // Define the GPIO pin number the led is attached to
const byte MotorPin2 = 15;                      // Define the GPIO pin number the led is attached to
String request;                                 // Define the string used to store the valid get request
String Val1;                                    // Define the value for the number of the get request
int Valint;                                     // Define the int value of the string above

const int wifi_led = 4;                         // Define the pin for the WiFi indicator LED

WiFiServer server(80);                          // Set the web server to port 80

HardwareSerial Sender(1);                       // Initialize the harware's serial port 1 with the name Sender (0 is for programming)

String header;                                  // Define the string where the request is stored

unsigned long currentTime = millis();           // Define the currrent time
unsigned long previousTime = 0;                 // Define the previous time
const long timeoutTime = 2000;                  // Define timeout time

Servo_ESP32 Motor1;                             // Define the first motor
Servo_ESP32 Motor2;                             // Define the second motor

void AdvertiseServices(const char *MyName)      // Create AdvertiseServices function (Will be called later on setup)
{
  if(MDNS.begin(MyName))                        // Start mDNS service, if it started correctly:
  {
    MDNS.addService("Configure_Robot", "tcp", 23);  // Start mDNS-sd service called n8i-mlp on tcp port 23        
  }

  else                                          // If mDNS server couldn't be started:
  {
    while(1)
    {
      delay(1000);                              // Wait a 1000ms      
    }
  }
}

uint16_t GetDeviceId()                          // Define GetDeviceId function
{
  #if defined(ARDUINO_ARCH_ESP32)               // If the device is an arduino arch esp32
    return ESP.getEfuseMac();                   // Get the Efuse Mac adress
  #else                                         // If it is any other ESP
    return ESP.getChipId();                     // Get the ESP chip ID
  #endif                                        // End condition
}

String MakeMine(const char *NameTemplate)       // Create the friendly name/ID for the device
{
  uint16_t uChipId = GetDeviceId();             // Get the chip's native ID
  String Result = String(NameTemplate) + String(uChipId, HEX);  // Concavinate the friendly name with the device ID
  return Result;                                // Return the value to the function
}

void setup()                                    // Run on startup:
{
  pinMode(wifi_led, OUTPUT);                    // Set the WiFi indicator LED as output

  WiFi.mode(WIFI_STA);                          // Set WiFi mode. ESP32 defaults to STA+AP
  
  Motor1.attach(MotorPin1);                     // Attach the first motor to a pin
  Motor2.attach(MotorPin2);                     // Attach the second motor to a pin

  Sender.begin(115200, SERIAL_8N1, 17, 16);     // Start the sender serial port (Baud Rate, Protocol, TXd pin, RXd pin)

  digitalWrite(wifi_led, LOW);                  // Make sure the WiFi indicator LED is off while there is no wifi.
    
  WiFiManager wm;                               // Initilaize Wifi Manager

  // wm.setSTAStaticIPConfig(IPAddress(192,168,1,254), IPAddress(10,0,1,1), IPAddress(255,255,255,0));  // Set static IP, static gateway and subnet

  //wm.resetSettings();                         // Deletes the saved credentials. Used for testing

  bool res;                                     // Define the res bool
    
  res = wm.autoConnect("Configure_Robot");      // Create an open AP where WiFi credentials will be set (if device isn't already connected to the internet)
  
  if(!res)                                      // If it failed to connect to WiFi or set up the AP
  {
    digitalWrite(wifi_led, LOW);                // Turn WiFi indicator LED off.                               
  } 
      
  else                                          // Otherwise,
  {   
    digitalWrite(wifi_led, HIGH);               // Turn WiFi indicator LED on.                                        
  }

  server.begin();                               // Start the local WiFi server
    
  String MyName = MakeMine("FindMobileRobot");  // Define a name for the AdvertiseServies function
  AdvertiseServices(MyName.c_str());            // Call AdvertiseServices function with the defined name
    
}

void loop()                                     // Run indefinitely
{ 
  WiFiClient client = server.available();       // Listen for incoming clients
  
  if(client) 
  {                                             // If a new client connects,

    String currentLine = "";                    // make a String to hold incoming data from the client
    currentTime = millis();                     // Update time
    previousTime = currentTime;                 // Update time
    while(client.connected() && currentTime - previousTime <= timeoutTime)  // Loop while the client is connected and not out of the timeout range
    {
      currentTime = millis();                   // Update the time       
      if(client.available())                    // if there's bytes to read from the client,
      {             
        char c = client.read();                 // read a byte, then
        header += c;                            // add the byte to the header
        
        if(c == '\n')                           // if the byte is a newline character
        {                    
           
          if(currentLine.length() == 0)         // if the current line is blank, you got two newline characters in a row. that's the end of the client HTTP request, so send a response:
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
             
            if(header.indexOf("GET /1/") >= 0)   // If the header starts with GET /1/
            {
              Val1 = header[7];                   // Set the 7th character of the request to the hundreds number 
              Valint = Val1.toInt();             // Transfotm the 3 digit number from a string to an integer and set it to the speed value
              Motor1.write(map(Valint, 0, 9, 48, 180));  // Set the motor 1 speed
            }
             
            else if(header.indexOf("GET /2/") >= 0)  // If the header starts with GET /2/
            {
              Val1 = header[7];                   // Set the 7th character of the request to the hundreds number 
              Valint = Val1.toInt();             // Transfotm the 3 digit number from a string to an integer and set it to the speed value
              Motor2.write(map(Valint, 0, 9, 48, 180));  // Set the motor 1 speed
            }

            else if(header.indexOf("GET /STOP") >= 0)
            {
              Motor1.write(48);
              Motor2.write(48);
            }

            else if(header.indexOf(/*GET REQUEST HERE*/) >= 0)
            {
              while (Sender.available()) 
              {               
                Sender.write(/*MESSAGE FOR MODULE MCU HERE*/);                
                Sender.write('\n');
              };
            }

            break;                             // Break out of the loop
          } 
              
          else                                 // if you got a newline, then clear currentLine
          { 
            currentLine = ""; 
          }  
        } 
        
        else if(c != '\r')                     // if you got anything else but a carriage return character,
        {  
          currentLine += c;                    // add it to the end of the currentLine
        }
      }
    } 
    
    header = "";                               // Clear the header variable
    client.stop();                             // Close the connection

  }
}

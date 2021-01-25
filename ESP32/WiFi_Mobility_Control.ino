 /*
 / I'd rather add too many comments than too few.
 / Code is easier to write than it is to read, so let's not risk it. 
 / Made by Slushee (Pol Fernàndez)
 / Alpha 1.3.1 (25/01/2021)
 */


#include <WiFi.h>                               // Load Wi-Fi library
#include <Servo_ESP32.h>                        // Load Servo Library

const byte led_gpio1 = 23;                      // Define the GPIO pin number the led is attached to
const byte led_gpio2 = 22;                      // Define the GPIO pin number the led is attached to
int brightness = 0;                             // Define the brighness value and default it to 0
String request;                                 // Define the string used to store the valid get request
String Val1;                                    // Define the value for the number of the get request
int Valint;                                     // Define the int value of the string above

const char* ssid     = "*************";         // Define the SSID to connect to
const char* password = "******************";    // Define the password for the SSID above

WiFiServer server(80);                          // Set the web server to port 80
String header;                                  // Define the string where the request is stored

unsigned long currentTime = millis();           // Define the currrent time
unsigned long previousTime = 0;                 // Define the previous time
const long timeoutTime = 2000;                  // Define timeout time

Servo_ESP32 servo1;
Servo_ESP32 servo2;

void setup()                                    // Run on startup:
  {
   servo1.attach(led_gpio1, 1000, 2000);
   servo2.attach(led_gpio2);
  
   Serial.begin(115200);                        // Begin serial port at 115200 baud rate
    
   Serial.print("Connecting to ");              // Print "Connecting to " to the serial port
   Serial.println(ssid);                        // Print the ssid to the serial port
   WiFi.begin(ssid, password);                  // Connect to Wi-Fi network with SSID and password
   while (WiFi.status() != WL_CONNECTED)        // While the wifi is not connected
     {
      delay(500);                               // Wait for it to connect
      Serial.print(".");                        // Print a dot to the serial port
     }
     
   Serial.println("");                          // Print a new line to the serial port
   Serial.println("WiFi connected.");           // Print "Wifi conneted." to the serial port
   Serial.println("IP address: ");              // Print "IP adress: " to the serial port
   Serial.println(WiFi.localIP());              // Print the ESP's web server IP to the serial port
   server.begin();                              // Start the web server
  }

void loop()                                     // Run indefinitely
  {
   WiFiClient client = server.available();      // Listen for incoming clients
  
   if (client) 
   {                                            // If a new client connects,
    String currentLine = "";                    // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime)  // Loop while the client is connected and not out of the timeout range
      {
        currentTime = millis();                 // Update the time       
        if (client.available())                 // if there's bytes to read from the client,
          {             
          char c = client.read();               // read a byte, then
          header += c;                          // add the byte to the header
          
          if (c == '\n')                      // if the byte is a newline character
            {                    
            
            if (currentLine.length() == 0)      // if the current line is blank, you got two newline characters in a row. that's the end of the client HTTP request, so send a response:
              {
               // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
               // and a content-type so the client knows what's coming, then a blank line:
               client.println("HTTP/1.1 200 OK");
               client.println("Content-type:text/html");
               client.println("Connection: close");
               client.println();
            
               
               if (header.indexOf("GET /1/") >= 0)   // If the header starts with GET /1/
                 {
                  request = header;                  // Set the header to be the request
                  Val1= header[7];                   // Set the 7th character of the request to the hundreds number 
                  Serial.println(Val1);              // Print the value to the serial monitor
                  brightness = Val1.toInt();         // Transfotm the 3 digit number from a string to an integer and set it to the brigness value
                  servo1.write(180);                 // Send the 180 degree value to the servo1
                 }
             
               else if (header.indexOf("GET /2/") >= 0)  // If the header starts with GET /2/
                {
                 request = header;                  // Set the header to be the request
                 Val1= header[7];                   // Set the 7th character of the request to the hundreds number 
                 Serial.println(Val1);              // Print the value to the serial monitor
                 brightness = Val1.toInt();         // Transfotm the 3 digit number from a string to an integer and set it to the brigness value
                 servo2.write(180);                 // Send the 180 degree value to the servo2
                }

                break;                               // Break out of the loop
              } 
              
            else                                     // if you got a newline, then clear currentLine
              { 
               currentLine = ""; 
              }  
        } 
        
        else if (c != '\r')                          // if you got anything else but a carriage return character,
          {  
          currentLine += c;                          // add it to the end of the currentLine
          }
        }
     } 
    
    
    header = "";                                     // Clear the header variable
    client.stop();                                   // Close the connection

   }
 }

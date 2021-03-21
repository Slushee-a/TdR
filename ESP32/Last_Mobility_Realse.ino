/*
 / I'd rather add too many comments than too few.
 / Code is easier to write than it is to read, so let's not risk it. 
 / Made by Slushee (Pol Fernàndez)
 / Alpha 1.6.0 (21/03/2021)
 */

#include <WiFi.h>                               // Load WiFi library
#include <Servo_ESP32.h>                        // Load Servo Library
#include "BluetoothSerial.h"                    // Load Bluetooth Library
#include "SPIFFS.h"                             // Load spiffs Library

const byte MotorPin1 = 17;                      // Define the GPIO pin number the led is attached to
const byte MotorPin2 = 18;                      // Define the GPIO pin number the led is attached to
String request;                                 // Define the string used to store the valid get request
String Val1;                                    // Define the value for the number of the get request
int Valint;                                     // Define the int value of the string above

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)            //Error handling conditions
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it   //Error message
#endif                                                                           //End of error handling definition

BluetoothSerial SerialBT;                       // Initialization of the bluetooth library
String btmessage = "";                          // Define the char array that will store the message recieved over bluetoth

char* ssid     = "";                            // Define the SSID parameter
char* password = "";                            // Define the password parameter

WiFiServer server(80);                          // Set the web server to port 80

String header;                                  // Define the string where the request is stored
String ssid_pass;                               // Define the string that will hold the incoming bluetooth message
String ssid1;                                   // Define the string for the recieved SSID
String pass1;                                   // Define the string for the recieved password
String ssid2;                                   // Define the string for the spiffs SSID
String pass2;                                   // Define the string for the spiffs password

unsigned long currentTime = millis();           // Define the currrent time
unsigned long previousTime = 0;                 // Define the previous time
const long timeoutTime = 2000;                  // Define timeout time

Servo_ESP32 Motor1;                             // Define the first motor
Servo_ESP32 Motor2;                             // Define the second motor

void setup()                                    // Run on startup:
  {

    SerialBT.begin("RoboPrototype");            // Define bluetooth beacon name and start it
   
    Motor1.attach(MotorPin1);                   // Attach the first motor to a pin
    Motor2.attach(MotorPin2);                   // Attach the second motor to a pin
  
    Serial.begin(115200);                       // Begin serial port at 115200 baud rate

    if (!SPIFFS.begin(true))                    // If spiffs fails to begin
      {
       Serial.println("An Error has occurred while mounting SPIFFS");  // Print "An Error has occurred while mounting SPIFFS" to the serial monitor
       return;                                  // Exit out of the loop
      }

    do                                          // Execute this code if the while condition on line 92 was true last loop
      {
    
       while(SerialBT.available())              // While the bluetooth is enabled
         { 
           ssid_pass = SerialBT.readString();   // Set whatever is recieved to the ssid_pass vairable
           Serial.println(ssid_pass);           // Print whatever was recieved
  
           for (int i = 0; i < ssid_pass.length(); i++) // Keep on counting up how many characters are there in the recieved message (until a comma is found)
             {
               if (ssid_pass.substring(i, i+1) == ",") // If there is a comma in the message
                {
                  ssid1 = ssid_pass.substring(0, i);  // Make whatever's before the comma the SSID string
                  pass1 = ssid_pass.substring(i+1);   // Make whatever's after it the Password string
                  Serial.print("Recieved SSID = " + ssid1);  // Print the recieved SSID to the serial port
                  Serial.print("Recieved password = " + pass1);  // Print the recieved password to the serial monitor
                  
                  File file = SPIFFS.open("/SSID.txt", FILE_WRITE);  // Open the file on the location /SSID.txt
 
                  if(!file)                     // If the SSID file fails to open
                    {
                      Serial.println("SSID file could not be opened for write");  // Print "SSID file could not be opened for write" to the serial monitor
                      return;                   // Break out of the loop
                    }
 
                  if(file.print(ssid1))         // If the SSID file could be written to
                    {
                     Serial.println("SSID was written");  // Print "SSID was written" to the serial monitor
                    }
                    
                  else                          // If it could't be written to        
                    {
                     Serial.println("SSID couldn't be written");  // Print "SSID couldn't be written" to the serial monitor
                    }
 
                  file.close();                 // Close the file

                  delay(2000);

                  File file2 = SPIFFS.open("/SSID.txt");  // Open the SSID file on another instance
 
                  if(!file2)                    // If the SSID file could not be opened
                    {
                     Serial.println("SSID file could not be opened for read");  // Print "SSID file could not be opened for read" to the serial monitor
                     return;  // Break out of the loop
                    }
  
                  Serial.println("SSID file Content: ");  // Print "SSID file Content: " to the serial monitor
 
                  while(file2.available())      // While the SSID file is available
                    {
                     Serial.write(file2.read());  // print it to the serial monitor
                     ssid2 = String(file2.read());  // Set the ssid2 string to whatever's in the file
                    }
 
                  file2.close();                // Close the file       

                  delay(2000);

                  File file3 = SPIFFS.open("/PASS.txt", FILE_WRITE);  // Open the file on the location /PASS.txt
 
                  if(!file3)                    // If the file could not be opened
                    {
                      Serial.println("Password file could not be opened for read");  // Print "Password file could not be opened for read" to the serial monitor
                      return;                   // Break out of the loop
                    }
 
                  if(file.print(pass1))         // If the password file could be written to
                    {
                     Serial.println("Password was written");  // Print "Password was written" to the serial monitor
                    }
                    
                  else                          // If the password file couldn't be written to
                    {
                     Serial.println("Password couldn't be written");  // Print "Password couldn't be written" to the serial monitor
                    }
 
                  file.close();                 // Close the file

                  delay(2000);

                  File file4 = SPIFFS.open("/PASS.txt");  // Open the password file in another instance
 
                  if(!file4)                    // If the file could not be opened
                    {
                     Serial.println("Password file could not be opened for read");  // Print "Password file could not be opened for read" to the serial monitor
                     return;                    // Break out of the loop
                    }
  
                  Serial.println("Password file Content: ");  // Print "Password file Content: " to the serial monitor
 
                  while(file4.available())      // While the password file is available 
                    {
                     Serial.write(file4.read());  // Print it to the serial monitor
                     pass2 = String(file4.read());  // Set whatever's on the file to the pass2 string
                    }
 
                  file4.close();                 // Close the password file
                  
                  delay(2000);                  // Wait for a bit
                  
                  int n1 = ssid2.length();      // Check how long is the SSID
                  char char_array1[n1 + 1];     // Create a char array for the SSID
                  strcpy(char_array1, ssid2.c_str());  // Copy the ssid string to the char array
  
                  int n2 = pass2.length();      // Check how long is the password        
                  char char_array2[n2 + 1];     // Create a char array for the password
                  strcpy(char_array2, pass2.c_str());  // Copy the password string to the char array
  
                  Serial.print("Connecting to: " + String(char_array1) + " | " + String(char_array2)); // Print to the serial monitor where you are connecting to
                 
                  while (WiFi.status() != WL_CONNECTED)  // While the wifi is not connected
                    {
                     delay(5000);                // Wait for it to connect
                     Serial.print(".");         // Send a dot to the serial port (to signal that it's still connecting)
                     WiFi.begin(char_array1, char_array2);  // Connect to the credentials
                    }
                  Serial.println();             // Create a new line in the serial port (To separate whatever's new from the dots) 
                  Serial.print("Connected, IP address: ");  // Print "Connected, IP address: " to the serial port
                  Serial.println(WiFi.localIP());  // Print the local server IP to the serial port

                  break;                        // Break out of the loop      
                }
             }  
         }    
     } while(WiFi.status() != WL_CONNECTED);    // Condition set for the do statement on line 50       
  }

void loop()                                     // Run indefinitely
  { 
   
   WiFiClient client = server.available();      // Listen for incoming clients
  
   if (client) 
   {                                            // If a new client connects,
    String currentLine = "";                    // make a String to hold incoming data from the client
    currentTime = millis();                     // Update time
    previousTime = currentTime;                 // Update time
    while (client.connected() && currentTime - previousTime <= timeoutTime)  // Loop while the client is connected and not out of the timeout range
      {
        currentTime = millis();                 // Update the time       
        if (client.available())                 // if there's bytes to read from the client,
          {             
          char c = client.read();               // read a byte, then
          header += c;                          // add the byte to the header
          
          if (c == '\n')                        // if the byte is a newline character
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
                  Valint = Val1.toInt();             // Transfotm the 3 digit number from a string to an integer and set it to the brigness value
                  Motor1.write(map(Valint, 0, 1, 0, 180));  // Set the motor 1 speed
                 }
             
               else if (header.indexOf("GET /2/") >= 0)  // If the header starts with GET /2/
                {
                 request = header;                  // Set the header to be the request
                 Val1= header[7];                   // Set the 7th character of the request to the hundreds number 
                 Serial.println(Val1);              // Print the value to the serial monitor
                 Valint = Val1.toInt();             // Transfotm the 3 digit number from a string to an integer and set it to the brigness value
                 Motor2.write(map(Valint, 0, 1, 0, 180));  // SSet the motor 2 speed
                }

                break;                          // Break out of the loop
              } 
              
            else                                // if you got a newline, then clear currentLine
              { 
               currentLine = ""; 
              }  
        } 
        
        else if (c != '\r')                     // if you got anything else but a carriage return character,
          {  
          currentLine += c;                     // add it to the end of the currentLine
          }
        }
     } 
    
    
    header = "";                                // Clear the header variable
    client.stop();                              // Close the connection

   }
 }

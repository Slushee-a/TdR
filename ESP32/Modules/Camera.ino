/*
 * This was created by Yoursunny and modified by Slushee.
 * Original version: https://github.com/yoursunny/esp32cam, Sketch: https://github.com/yoursunny/esp32cam/blob/main/examples/WifiCam/WifiCam.ino
 * Last modified by Slushee (Pol Fernàndez) on 12/6/2021. Version Alpha 1.0.0
 */

#include <WebServer.h>                                          // Load WebServer library (Part of the ESP family of libraries)
#include <WiFi.h>                                               // Load WiFi library (Part of the ESP family of libraries)
#include <esp32cam.h>                                           // Load esp32 cam library (Part of the ESP family of libraries)                        
#include "ESPmDNS.h"                                            // Load mDNS library (Part of the ESP family of libraries)
#include <WiFiManager.h>                                        // Load WiFi manager library (By tzapu on Github: https://github.com/tzapu/WiFiManager)

const int wifi_led = 2;                                         // Define the pin for the WiFi indicator LED

void AdvertiseServices(const char *MyName)                      // Create AdvertiseServices function (Will be called later on setup)
{
  if(MDNS.begin(MyName))                                        // Start mDNS service, if it started correctly:
  {
    MDNS.addService("FindMyCamera", "tcp", 23);                 // Start mDNS-sd service called FindMyCamera on tcp port 23        
  }

  else                                                          // If mDNS server couldn't be started:
  {
    while(1)
    {
      delay(1000);                                              // Wait 1000ms      
    }
  }
}

uint16_t GetDeviceId()                                          // Define GetDeviceId function
{
  #if defined(ARDUINO_ARCH_ESP32)                               // If the device is an arduino arch esp32
    return ESP.getEfuseMac();                                   // Get the Efuse Mac adress
  #else                                                         // If it is any other ESP
    return ESP.getChipId();                                     // Get the ESP chip ID
  #endif                                                        // End condition
}

String MakeMine(const char *NameTemplate)                       // Create the friendly name/ID for the device
{
  uint16_t uChipId = GetDeviceId();                             // Get the chip's native ID
  String Result = String(NameTemplate) + String(uChipId, HEX);  // Concavinate the friendly name with the device ID
  return Result;                                                // Return the value to the function
}

WebServer server(80);                                           // Start web server on port 80

static auto loRes = esp32cam::Resolution::find(320, 240);       // Define the camera's low resolution mode
static auto hiRes = esp32cam::Resolution::find(800, 600);       // Define the camera's high resolution mode

void handleBmp()
{
  if (!esp32cam::Camera.changeResolution(loRes))
  {
    //
  }

  auto frame = esp32cam::capture();

  if (frame == nullptr) 
  {
    server.send(503, "", "");
    return;
  }

  if (!frame->toBmp()) 
  {
    server.send(503, "", "");
    return;
  }

  server.setContentLength(frame->size());
  server.send(200, "image/bmp");
  WiFiClient client = server.client();
  frame->writeTo(client);
}

void
serveJpg()
{
  auto frame = esp32cam::capture();
  if (frame == nullptr) 
  {
    server.send(503, "", "");
    return;
  }

  server.setContentLength(frame->size());
  server.send(200, "image/jpeg");
  WiFiClient client = server.client();
  frame->writeTo(client);
}

void
handleJpgLo()
{
  if (!esp32cam::Camera.changeResolution(loRes)) 
  {
    //
  }
  serveJpg();
}

void
handleJpgHi()
{
  if (!esp32cam::Camera.changeResolution(hiRes)) 
  {
  //
  }
  serveJpg();
}

void
handleJpg()
{
  server.sendHeader("Location", "/cam-hi.jpg");
  server.send(302, "", "");
}

void
handleMjpeg()
{
  if (!esp32cam::Camera.changeResolution(hiRes))
  {
//
  }

  WiFiClient client = server.client();
  auto startTime = millis();
  int res = esp32cam::Camera.streamMjpeg(client);
  if (res <= 0) 
  {
    return;
  }
  auto duration = millis() - startTime;
}

void
setup()
{
  pinMode(wifi_led, OUTPUT);                                    // Set the WiFi indicator LED as output

  WiFiManager wm;                                               // Define WiFi Manager

  bool res;                                                     // Create an open AP where WiFi credentials will be set (if device isn't already connected to the internet)

  if(!res)                                                      // If it failed to connect to WiFi or set up the AP
  {
    digitalWrite(wifi_led, LOW);                                // Turn WiFi indicator LED off.                               
  } 
      
  else                                                          // Otherwise,
  {   
    digitalWrite(wifi_led, HIGH);                               // Turn WiFi indicator LED on.                                      
  }

  res = wm.autoConnect("FindMyCamera");

  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(hiRes);
    cfg.setBufferCount(2);
    cfg.setJpeg(80);

    bool ok = Camera.begin(cfg);
  }

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);

  /*
   * The structure for the requests is: http://[CAMERA_IP]/[MODE]
   * You can get the camera ip using an mDNS browser.
   *
   * Modes: 
   * Bmp image -> cam.bmp
   * Low res JPG -> cam-lo.jpg
   * High res JPG -> cam-hi.jpg
   * Video Stream -> cam.mjpeg
   */

  server.on("/cam.bmp", handleBmp);
  server.on("/cam-lo.jpg", handleJpgLo);
  server.on("/cam-hi.jpg", handleJpgHi);
  server.on("/cam.jpg", handleJpg);
  server.on("/cam.mjpeg", handleMjpeg);

  server.begin();

  String MyName = MakeMine("FindMyCamera");                      // Define a name for the AdvertiseServies function
  AdvertiseServices(MyName.c_str());                             // Call AdvertiseServices function with the defined name
}

void
loop()
{
  server.handleClient();

  WiFiManager wm;                                                // Define WiFi Manager

  if(wm.getWLStatusString(WiFiClass::status()) == "WL_CONNECTED")  // If WiFi is connected
  {
    digitalWrite(wifi_led, HIGH);                                // Turn WiFi indicator LED on.
  }

  else                                                           // Otherwise,
  {
    digitalWrite(wifi_led, LOW);                                 // Turn WiFi indicator LED off. 
  }      
}

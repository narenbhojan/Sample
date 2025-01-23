#include <HardwareSerial.h> // Reference the ESP32 built-in serial port library
#include <Preferences.h>
//#include <WiFi.h>
#include <TimeLib.h>

#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <ESP8266httpUpdate.h>

// Replace with your network credentials
const char* ssid     = "ESP32-Access-Point";
const char* password = "123456789";

const char* ssidhotspot = "ncubeed";
const char* passwordhotspot = "70030219";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;
//HardwareSerial lidarSerial(0); // Using serial port 2
#define RXD2 3
#define TXD2 1
#define LED 5
Preferences preferences;
bool LIGHT_ON = false;
int DISTANCE = 500;
time_t time_start;
time_t time_end;
int CNT_AP = 0;
int MEASURED_DISTANCE;
time_t aptime1;
time_t aptime2;
time_t apmodestarttime;
time_t apmodeendtime;


void softwareUpdate(){
  Serial.println("in sw update...");
  Serial.println("Checking internet");
  WiFi.mode(WIFI_STA);
  ESP8266WiFiMulti().addAP(ssidhotspot, passwordhotspot);
  while(ESP8266WiFiMulti().run() != WL_CONNECTED){
    Serial.println("internet good");
    WiFiClientSecure client;
    client.setInsecure();

    // The line below is optional. It can be used to blink the LED on the board during flashing
    // The LED will be on during download of one buffer of data from the network. The LED will
    // be off during writing that buffer to flash
    // On a good connection the LED should flash regularly. On a bad connection the LED will be
    // on much longer than it will be off. Other pins than LED_BUILTIN may be used. The second
    // value is used to put the LED on. If the LED is on with HIGH, that value should be passed
    ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);
      Serial.println("Update sketch...");
      t_httpUpdate_return ret = ESPhttpUpdate.update(client, "https://storage.googleapis.com/lidar-builds/ESP12F.ino");
      if (ret == HTTP_UPDATE_OK) {
        Serial.println("Success");
          digitalWrite(LED,HIGH);
          delay(10000);
          digitalWrite(LED,LOW);
      }else{
        Serial.println("Failed");
        Serial.println(ESPhttpUpdate.getLastErrorString().c_str());
      }
      ESP.restart();
      
    }
  
}

void APMode(){
  Serial.print("Setting AP (Access Point)…");
  apmodestarttime = now();
  //getSerial()
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  //Serial.print("AP IP address: ");
  //Serial.println(IP);
  
  server.begin();
  apmodeendtime = now();
  int apmodedifftime = apmodeendtime-apmodestarttime; 
  WiFiClient client;
while (apmodedifftime<60){
  //Serial.println(apmodedifftime);
  delay(1000);
  apmodeendtime = now();
  apmodedifftime = apmodeendtime - apmodestarttime;
  client = server.available();   // Listen for incoming clients
  
  if (client) {                             // If a new client connects,
    //Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /1meters") >= 0) {
              //Serial.println("Setting distance to 2");
              preferences.begin("imai-app", false); 
              preferences.putInt("LENGTH", 100);
              preferences.end();
              ESP.restart();
              return;
            }else if (header.indexOf("GET /2meters") >= 0) {
              //Serial.println("Setting distance to 2");
              preferences.begin("imai-app", false); 
              preferences.putInt("LENGTH", 200);
              preferences.end();
              ESP.restart();
              return;
            } else if (header.indexOf("GET /3meters") >= 0) {
              //Serial.println("Setting distance to 3");
              preferences.begin("imai-app", false); 
              preferences.putInt("LENGTH", 300);
              preferences.end();
              ESP.restart();
              return;
            } else if (header.indexOf("GET /4meters") >= 0) {
              //Serial.println("Setting distance to 4");
              preferences.begin("imai-app", false); 
              preferences.putInt("LENGTH", 400);
              preferences.end();
              ESP.restart();
              return;
            } else if (header.indexOf("GET /5meters") >= 0) {
              //Serial.println("Setting distance to 5");
              preferences.begin("imai-app", false); 
              preferences.putInt("LENGTH", 500);
              preferences.end();
              ESP.restart();
              return;
            }
            else if (header.indexOf("GET /6meters") >= 0) {
              //Serial.println("Setting distance to 5");
              preferences.begin("imai-app", false); 
              preferences.putInt("LENGTH", 600);
              preferences.end();
              ESP.restart();
              return;
            }
            else if (header.indexOf("GET /7meters") >= 0) {
              //Serial.println("Setting distance to 5");
              preferences.begin("imai-app", false); 
              preferences.putInt("LENGTH", 700);
              preferences.end();
              ESP.restart();
              return;
            }
            else if (header.indexOf("GET /8meters") >= 0) {
              //Serial.println("Setting distance to 5");
              preferences.begin("imai-app", false); 
              preferences.putInt("LENGTH", 800);
              preferences.end();
              ESP.restart();
              return;
            }
            else if  (header.indexOf("GET /softwareupdate") >= 0) {
              Serial.println("Software update");
              server.close();
              client.abort();
              softwareUpdate();


            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println(".buttonred { background-color: #FF0000; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>Set Distance of sensor</h1>");
            client.println("<h3>Current distance set(meters): </h3>");
            client.println("<p>");
            client.println(DISTANCE/100);
            client.println("</p>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>. Press the distance button once and wait for restart upto 1 minute</p>");
            client.println("<p><a href=\"/1meters\"><button class=\"button\">1</button></a></p>");
            client.println("<p><a href=\"/2meters\"><button class=\"button\">2</button></a></p>");
            client.println("<p><a href=\"/3meters\"><button class=\"button\">3</button></a></p>");
            client.println("<p><a href=\"/4meters\"><button class=\"button\">4</button></a></p>"); 
            client.println("<p><a href=\"/5meters\"><button class=\"button\">5</button></a></p>");
            client.println("<p><a href=\"/6meters\"><button class=\"button\">6</button></a></p>");
            client.println("<p><a href=\"/7meters\"><button class=\"button\">7</button></a></p>");
            client.println("<p><a href=\"/8meters\"><button class=\"button\">8</button></a></p>");
            client.println("<p><a href=\"/softwareupdate\"><button class=\"buttonred\">Software update</button></a></p>");
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    //Serial.println("Client disconnected.");
    //Serial.println("");
  }
  }
  client.stop();
  //Serial.println("returning");
  ESP.restart();
  return;
}

void setup() {
  Serial.begin(115200);
  setTime(0,0,0,1,1,2025); // alternative to above, yr is 2 or 4 digit yr

  pinMode(LED,OUTPUT);
  //Serial.begin(115200); // Initializing serial port
  //Serial.println("Starting...");
  preferences.begin("imai-app", false); 
  DISTANCE = preferences.getInt("LENGTH", 500);
  int distanceInMeters = DISTANCE/100;
  preferences.end();
  for (int j=0;j<distanceInMeters;j++){
    delay(500);
    digitalWrite(LED,HIGH);
    delay(500);
    digitalWrite(LED,LOW);
  }
  //Serial.println(DISTANCE);
  //lidarSerial.begin(115200, SERIAL_8N1, RXD2, TXD2); // Initializing serial port
  //lidarSerial.begin(115200);
  APMode();
}

void loop() {
  // uint8_t buf[9] = {0}; // An array that holds data
  // if (lidarSerial.available() > 0) {
  //   lidarSerial.readBytes(buf, 9); // Read 9 bytes of data
  //   if( buf[0] == 0x59 && buf[1] == 0x59)
  //   {
  //     uint16_t distance = buf[2] + buf[3] * 256;
  //     //
  //     //int16_t temperature = buf[6] + buf[7] * 256;
  //     if (distance < DISTANCE && distance >12){
  //       //Serial.println(distance);
  //       delay(10);
  //       lidarSerial.readBytes(buf, 9); // Read 9 bytes of data
  //       uint16_t distance1 = buf[2] + buf[3] * 256;
  //       delay(10);
  //       lidarSerial.readBytes(buf, 9); // Read 9 bytes of data
  //       uint16_t distance2 = buf[2] + buf[3] * 256;
  //       delay(10);
  //       lidarSerial.readBytes(buf, 9); // Read 9 bytes of data
  //       uint16_t distance3 = buf[2] + buf[3] * 256;
  //       delay(10);
  //       lidarSerial.readBytes(buf, 9); // Read 9 bytes of data
  //       uint16_t distance4 = buf[2] + buf[3] * 256;
  //       uint16_t strength = buf[4] + buf[5] * 256;
  //       if(distance1<DISTANCE && distance2<DISTANCE && distance3<DISTANCE && distance4<DISTANCE  && 
  //           distance1 >12 && distance2 >12 && distance3 >12 && distance4 >12 &&
  //           abs(distance1-distance2) <50 && abs(distance2-distance3) < 50 && abs(distance3-distance4) < 50 ){
  //         if (!LIGHT_ON){
  //             time_start = now();
  //             LIGHT_ON = true;
  //             digitalWrite(LED,HIGH);
  //             // Serial.print("Distance: ");
  //             //Serial.print(distance4);
  //             //Serial.print(" cm, strength: ");
  //             //Serial.println(strength);
  //             MEASURED_DISTANCE = distance4;
  //             int i=50;
  //             while(i>0){
  //               i--;
  //               lidarSerial.readBytes(buf, 9);
  //               delay(10);
  //             }
  //             // Serial.print(", temperature: ");
  //             // Serial.println(temperature / 8.0 - 256.0);
  //           }
  //       }
  //     }else{
  //       if (LIGHT_ON){
  //         time_end = now();
  //         int diff = time_end-time_start;
  //         //Serial.println(diff);
  //         LIGHT_ON = false;
  //         if (diff == 3 && MEASURED_DISTANCE > 12 && MEASURED_DISTANCE < 20){
  //           CNT_AP++;
  //           if (CNT_AP == 1){
  //             aptime1 = now();
  //           }
  //           else if (CNT_AP == 2){
  //             aptime2 = now();
  //             int rdiff = aptime2-aptime1;
  //             if (rdiff < 6){
  //               //Serial.println("AP received");
  //               APMode();
  //             }else{
  //               CNT_AP = 0;
  //             }
              
  //           }else{
  //             CNT_AP = 0;
  //           }
  //         }else{
  //           CNT_AP = 0;
  //         }
  //       }
  //       digitalWrite(LED,LOW);
  //     }
      
  //   }
  // }
  delay(10); 
}
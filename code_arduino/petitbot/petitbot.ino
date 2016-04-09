

#include <ESP8266WiFi.h>
//////////////////////
// WiFi Definitions //
//////////////////////
const char WiFiAPPSK[] = "1234567890"; //password

/////////////////////
// Pin Definitions //
/////////////////////

#include <Servo.h>

Servo myservo1;

Servo myservo2;

WiFiServer server(80);

void setup()
{
  initHardware();
  setupWiFi();
  server.begin();
  myservo1.attach(5);
  myservo2.attach(4);
  myservo1.write(91); 
  myservo2.write(93); 

}

void loop()
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  
  client.flush();

  // Match the request
  int val = -1; 

  if (req.indexOf("/stop") != -1)
    val = 0; 
  else if (req.indexOf("/avance") != -1)
    val = 1;
  else if (req.indexOf("/recule") != -1)
    val = 2; 
  else if (req.indexOf("/gauche") != -1)
    val = 3; 
  else if (req.indexOf("/droite") != -1)
    val = 4; 



  // Prepare the web page response. Start with the common header:
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  if (req.indexOf("/commande") != -1 ) {
    s += "<input type=\"button\" onclick=\"location.href='192.168.4.1/led/1';\" value=\" OFF \" />";
    s += "<input type=\"button\" onclick=\"location.href='192.168.4.1/led/0';\" value=\" ON \" />";

  }
  // If we're setting the LED, print out a message saying we did
  if (val == 2 ) //avance
  {
    s += " avance ";
    myservo2.write(0);  //avance
    myservo1.write(180);  //avance
  }
  if (val == 1)//this left over from the sparkfun demo, not currently used
  { // If we're reading pins, print out those values:
    s += " recule ";
    myservo2.write(180);  //recule
    myservo1.write(0);  //recule
  }
  if (val == 0)//this left over from the sparkfun demo, not currently used
  { // If we're reading pins, print out those values:
    s += " recule ";
    myservo1.write(91);  //recule
    myservo2.write(93);  //recule
  }
  if (val == 3)//this left over from the sparkfun demo, not currently used
  { // If we're reading pins, print out those values:
    s += " gauche ";
    myservo1.write(180);  //gauche
    myservo2.write(180);  //recule
  }
  if (val == 4)//this left over from the sparkfun demo, not currently used
  { // If we're reading pins, print out those values:
    s += " droite ";
    myservo1.write(0);  //droite
    myservo2.write(0);  //recule
  }


  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disconnected");
  client.flush();

}

void setupWiFi()
{
  WiFi.mode(WIFI_AP);
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "PetitBot";

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);//zero out AP_NameChar

  for (int i = 0; i < AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK, 7);
}

void initHardware()
{
  Serial.begin(115200);
} //See more at: http://www.esp8266.com/viewtopic.php?f=29&t=6419#sthash.gd1tJhwU.dpuf
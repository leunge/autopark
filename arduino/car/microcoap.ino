#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <stdint.h>
#include <string.h>
#include "coapa.h"
//#define UDP_TX_PACKET_MAX_SIZE 256
#define PORT 5683
static uint8_t mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};

int status = WL_IDLE_STATUS;
char ssid[] = "Joon_OpenWrt"; //  your network SSID (name) 
char pass[] = "joonopenwrt";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

//unsigned int localPort = 2390;      // local port to listen on
//char packetBuffer[255]; //buffer to hold incoming packet
//char  ReplyBuffer[] = "acknowledged";       // a string to send back

uint8_t packetbuf[256];
static uint8_t scratch_raw[32];
static coap_rw_buffer_t scratch_buf = {scratch_raw, sizeof(scratch_raw)};

int resetPin=12;
WiFiUDP udp;

void setup() {
  int i;
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  //Serial.print("Hello");
  //Serial.print(UDP_TX_PACKET_MAX_SIZE);
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if ( fv != "1.1.0" )
    Serial.println("Please upgrade the firmware");

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(2000);
  } //-------------try to connect 
  
  Serial.println("Connected to wifi");
  printWifiStatus();
  /*
  SSID: Joon_OpenWrt
  IP Address: 192.168.1.137
  signal strength (RSSI):-35 dBm
  */

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  udp.begin(PORT);
  pinMode(resetPin, OUTPUT);
  coap_setup();
  endpoint_setup();
    
}

void udp_send(const uint8_t *buf, int buflen)
{
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    Serial.println("");
    Serial.print("IP : ");
    if(!udp.remoteIP()){
      Serial.print("ERROR");
      stop();
     if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while(1);
  }
    }
    Serial.print(udp.remoteIP());
    Serial.print(" PORT : ");
    Serial.print(udp.remotePort());
    Serial.println("");
    Serial.print(" Sending : ");
    while(buflen--){
        Serial.print(*buf, HEX);
        udp.write(*buf++);
        Serial.print("  ");
    }
    Serial.println("");
    udp.endPacket();
}



void loop() {

    int sz;
    int rc;
    coap_packet_t pkt;
    
    int i;
    if ((sz = udp.parsePacket()) > 0)
    {
        udp.read(packetbuf, sizeof(packetbuf));

        for (i=0;i<sz;i++)
        {
            Serial.print(packetbuf[i], HEX);
            Serial.print(" ");
        }
        Serial.println("");

        if (0 != (rc = coap_parse(&pkt, packetbuf, sz)))
        {
            Serial.print("Bad packet rc=");
            Serial.println(rc, DEC);
        }
        else
        {
            size_t rsplen = sizeof(packetbuf);
            coap_packet_t rsppkt;
            coap_handle_req(&scratch_buf, &pkt, &rsppkt);
            
            coap_packet_t *p;
            p=&rsppkt;
            Serial.println("Header :");
            Serial.print("ver :");
            Serial.println(p->hdr.ver, HEX);
            Serial.print("t :");
            Serial.println(p->hdr.t, HEX);
            Serial.print("tkl :");
            Serial.println(p->hdr.tkl, HEX);
            Serial.print("code :");
            Serial.println(p->hdr.code, HEX);
            Serial.print("id :");
            Serial.print(p->hdr.id[0], HEX);
            Serial.print("  ");
            Serial.println(p->hdr.id[1], HEX);
            Serial.println("Options :");
            Serial.println(p->numopts, HEX);
            Serial.println(p->opts[0].num );
            
            
            Serial.println("result:");
            Serial.println(rsplen);
          
            memset(packetbuf, 0, UDP_TX_PACKET_MAX_SIZE);
            if (0 != (rc = coap_build(packetbuf, &rsplen, &rsppkt)))
            {
                Serial.print("coap_build failed rc=");
                Serial.println(rc, DEC);
            }
            else
            {
              
                //Serial.print(rsplen);
                udp_send(packetbuf, rsplen);
            }
        }
    }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

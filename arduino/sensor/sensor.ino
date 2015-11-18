#include <SPI.h>
#include <Ethernet2.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
char server[] = "192.168.1.136";

EthernetClient client;

int analogPin[3] = {0,1,2};
bool status[3]={false,false,false};

void send(int num, bool type){
    Serial.print(type);
    Serial.print(" ,");
    Serial.print(num);
    Serial.println("send...");
   // Make a HTTP request:
   if (client.connect(server, 80)) {
      Serial.println("connected");
    if(type){
      client.print("GET /parkout.php?no=");
    }
    else{
      client.print("GET /parked.php?no=");
    }
    client.print(num+1);
    client.println(" HTTP/1.1");
    client.println("Host: 192.168.1.136");
    client.println("Connection: close");
    client.println();
   }
   else {
      // kf you didn't get a connection to the server:
      Serial.println("connection failed");
    }
}


void setup(){
    Serial.begin(9600);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for Leonardo only
    }      
    if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
      // no point in carrying on, so do nothing forevermore:
      // try to congifure using IP address instead of DHCP:
      Ethernet.begin(mac, ip);
    }
    delay(1000);
    
  
}

void loop(){
 
  
  for(int i=0 ; i<3 ; i++){
    if(status[i] == false){
      if(analogRead(analogPin[i]) < 1000){  
        status[i] = true;
        Serial.print(analogPin[i]);
        Serial.println(" car in ");
        send(analogPin[i], false);
        //Serial.println(analogRead(analogPin1));
        delay(1000);
        while(client.connected()){
        if (client.available()) {
          char c = client.read();
          Serial.print(c);
        }
        
        } if (!client.connected()) {
            Serial.println();
            Serial.println("disconnecting.");
            client.stop();
        
            // do nothing forevermore:
            //while (true);
          }

      }  
    }else{
      if(analogRead(analogPin[i]) > 1000){  
        status[i] = false;
        Serial.print(analogPin[i]);
        Serial.println(" car out ");
        send(analogPin[i], true);
        //Serial.println(analogRead(analogPin1));
        delay(1000);
        while(client.connected()){
          if (client.available()) {
              char c = client.read();
              Serial.print(c);
            }
            
        }
        if (!client.connected()) {
            Serial.println();
            Serial.println("disconnecting.");
            client.stop();
        
            // do nothing forevermore:
            //while (true);
          }

      }  
    }
  }
  
  
 

}

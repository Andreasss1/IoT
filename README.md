# Internet of Things
**Count the number of people in a room based on Internet of Things (IoT)**

**Introduction:**
This IoT system is made to count the number of people in the room, by giving a certain output if the number of people in the room exceeds the number specified by the system.

**Tools and materials:**
1. ESP32
2. Infrared Sensor
3. Temperature Sensor
4. Relay
5. Servo
6. RFID
7. Push Button
8. User Interface
   The User Interface used is MIT Apps Inventor which is connected to Blynk and Firebase. Because in addition to the application of tools,     this application also uses a Register system with data storage in Firebase.

**How it works:**
The workings of this tool are NodeMCU ESP32 receives input data from RFID. Then the servo motor opens the door. When someone passes through the door, InfraRed sensor 1 will detect and after that followed by Infrared sensor 2 detects, the number of people entering the room increases and sends data to Blynk IoT and then displays it in the application made using MIT Inventor, but if infrared sensor 2 detects and then followed by infrared sensor 1 detects, then the number of people in the room will decrease. If the number of people in the room is more than 3 people or 0, the RELAY does not turn on the electricity and if the number of people in the room is less or equal to 3 and more than 0 then the electricity will turn on. Then to open the door from inside the room using a PushButton and when people pass through the door to get out.

**Code for ESP32:**
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <DHT.h> 
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define SS_PIN 15
#define RST_PIN 0
#define sensorPin1 5
#define sensorPin2 4
#define MAX_PEOPLE 4
#define relayPin 2
#define DHT_PIN A0
#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);

int sensorState1 = 0;
int sensorState2 = 0;
int count=0;
int ready_entrer=0;
int ready_sortir=0;
const int pushButton=1;
bool isDoorOpen = false;
bool isPowerOn = false;

MFRC522 rfid(SS_PIN, RST_PIN);
Servo servo ;

// TEMPLATE ID, TEMPLATE NAME, DAN AUTH TOKEN
#define BLYNK_TEMPLATE_ID "TMPL6Y-Eqzieh"
#define BLYNK_TEMPLATE_NAME "EFFES Door"
#define BLYNK_AUTH_TOKEN "DrYx9q1so27Ox49zdsrda6fZzlRRXyCz"
#define BLYNK_PRINT Serial

// AUTH TOKEN, SSID, DAN PASS
char auth[] = BLYNK_AUTH_TOKEN ;
char ssid[] = "ssid"; 
char pass[] = "MyPass"; 

void setup() {
  dht.begin();
  pinMode (sensorPin1,INPUT_PULLUP);
  pinMode (sensorPin2, INPUT_PULLUP);
  pinMode(pushButton, INPUT);
  pinMode(relayPin,OUTPUT); 
  Serial.begin(115200);
  servo.attach(16);
  servo.write(140);
  SPI.begin();
  rfid.PCD_Init();
  Blynk.begin(auth, ssid, pass);
}

void loop(){ 
  Blynk.run();
  
  // open the door from the outside
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String cardId = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      cardId.concat(String(rfid.uid.uidByte[i] < 0x10 ? "0" : ""));
      cardId.concat(String(rfid.uid.uidByte[i], HEX));
    }
    if (cardId == "0175ab26") {
      servo.write(10);
      isDoorOpen = true;
      Serial.print("PINTU TERBUKA");
      delay(5000);
      servo.write(140);
      isDoorOpen = false;
    }   
 }

  
  // Count people entering and leaving the room
{   
  sensorState1 = digitalRead(sensorPin1);
  sensorState2 = digitalRead(sensorPin2);

  if (sensorState1 == LOW){
    ready_entrer=1;
  } else if (sensorState2 == LOW){
    ready_sortir=1;
  }
   if (ready_entrer==1 && sensorState2 == LOW){
    count++; 
    ready_entrer=0;
    ready_sortir=0;
    Serial.print("Jumlah Orang :");
    Serial.println(count);
    delay(500);
  } else if (ready_sortir==1 && sensorState1 == LOW){
    count--; 
    ready_sortir=0;
    ready_entrer=0;
    Serial.print("Jumlah Orang :");
    Serial.println(count);
    delay(500);
  }
 // Display person count data 
  Blynk.virtualWrite(V2, count);
}

  // Electricity Off
   if (count <= 0 || count > 3){
     if ( !isPowerOn){ 
       Serial.println("Listrik telah dimatikan");
       delay(1000);
       digitalWrite(relayPin,LOW);}
 }

   // Electricity On
    if (count >= 1 && count <= 3){
      if (!isPowerOn){
       Serial.println("Listrik telah dinyalakan");
       delay(1000);
       digitalWrite(relayPin,HIGH);}
    }
}

BLYNK_WRITE(V1){
  int value = param.asInt(); 
  if (value == 1) {
    servo.write(10); 
    Serial.println("PINTU TERBUKA");
    delay (5000);
    servo.write(140);}
}

**advanced development:**
with object counting this can be used in warehouse or storage systems that are integrated with conveyors, besides that this can also be developed with other outputs depending on what parts will be used, for example security, convenience, monitoring, and production.

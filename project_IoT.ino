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
#define relayPin 1
#define DHT_PIN A0
#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);

int sensorState1 = 0;
int sensorState2 = 0;
int count=0;
int ready_entrer=0;
int ready_sortir=0;
const int pushButton=2;
bool isDoorOpen = false;
bool isPowerOn = false;

MFRC522 rfid(SS_PIN, RST_PIN);
Servo servo ;

//MENDEFINISIKAN TEMPLATE ID, TEMPLATE NAME, DAN AUTH TOKEN
#define BLYNK_TEMPLATE_ID "TMPL6i8hdvibA"
#define BLYNK_TEMPLATE_NAME "kontrol LED Buzzer dan Data suhu"
#define BLYNK_AUTH_TOKEN "22BNVJcZRlj-Reg1CkIShal-pxxgqf5m"
#define BLYNK_PRINT Serial

//MEMASUKAN AUTH TOKEN, SSID, DAN PASS
char auth[] = BLYNK_AUTH_TOKEN ;
char ssid[] = "nama_wifi"; 
char pass[] = "password_wifi"; 

void setup() {
  dht.begin();
  pinMode (sensorPin1,INPUT_PULLUP);
  pinMode (sensorPin2, INPUT_PULLUP);
  pinMode(pushButton, INPUT);
  pinMode(relayPin,OUTPUT); 
  Serial.begin(115200);
  servo.attach(16);
  SPI.begin();
  rfid.PCD_Init();
  Blynk.begin(auth, ssid, pass);
}

void loop(){ 
  Blynk.run();
  
  //MEMBUKA PINTU DARI LUAR 
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

 //MEMBUKA PINTU DARI DALAM
  int buttonState = digitalRead(pushButton);

  if (buttonState == LOW) {
    servo.write(10); 
    Serial.println("PINTU TERBUKA");
    delay (5000);
    servo.write(140);
   } 
  
  // MENGHITUNG ORANG MASUK DAN KELUAR
{   
  sensorState1 = digitalRead(sensorPin1);
  sensorState2 = digitalRead(sensorPin2);

  if(sensorState1 == LOW){
    ready_entrer=1;
  }else if(sensorState2 == LOW){
    ready_sortir=1;
  }
  if(ready_entrer==1 && sensorState2 == LOW){
    count++; 
    ready_entrer=0;
    ready_sortir=0;
    Serial.print("Jumlah Orang :");
    Serial.println(count);
    delay(500);
  }else if(ready_sortir==1 && sensorState1 == LOW){
    count--; 
    ready_sortir=0;
    ready_entrer=0;
    Serial.print("Jumlah Orang :");
    Serial.println(count);
    delay(500);
  }
 // MENAMPILKAN DATA JUMLAH ORANG 
  Blynk.virtualWrite(V2, count);
}

  // LISTRIK MATI
   if (count <= 0 || count > 3){
     if ( !isPowerOn){ 
       Serial.println("Listrik telah dimatikan");
       delay(1000);
       digitalWrite(relayPin,LOW);}
 }

   // LISTRIK MENYALA
    if (count >= 1 && count <= 3){
      if (!isPowerOn){
       Serial.println("Listrik telah dinyalakan");
       delay(1000);
       digitalWrite(relayPin,HIGH);}
    }

  BLYNK_WRITE(V1){
  int value = param.asInt(); 
  if (value == 1) {
    servo.write(10); 
    Serial.println("PINTU TERBUKA");
    delay (5000);
    servo.write(140);
  } 
  }

}

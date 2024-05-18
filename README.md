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

**advanced development:**
with object counting this can be used in warehouse or storage systems that are integrated with conveyors, besides that this can also be developed with other outputs depending on what parts will be used, for example security, convenience, monitoring, and production.

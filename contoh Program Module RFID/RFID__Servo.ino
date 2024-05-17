#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>

Servo myservo;
#define SS_PIN  21  // ESP32 pin GPIO5 
#define RST_PIN 22 // ESP32 pin GPIO27Servo myServo;
int zervo = 13;
byte keyTagUID[4] = {0x53, 0x27, 0x7C, 0x16};

//53 27 7C 16




MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin(); // init SPI bus
  myservo.attach(zervo);
  rfid.PCD_Init(); // init MFRC522
  

  Serial.println("Tap an RFID/NFC tag on the RFID-RC522 reader");
}

void loop() {
      if (rfid.PICC_IsNewCardPresent()) { // new tag is available
      if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
        MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
        
        if (rfid.uid.uidByte[0] == keyTagUID[0] &&
            rfid.uid.uidByte[1] == keyTagUID[1] &&
            rfid.uid.uidByte[2] == keyTagUID[2] &&
            rfid.uid.uidByte[3] == keyTagUID[3] ) 
            {
              Serial.println("Access is granted");
              myservo.write(40);  // unlock the door for 2 seconds
              delay(2000);
              myservo.write(80);
      }
      else
      {
        Serial.print("Access denied, UID:");
        for (int i = 0; i < rfid.uid.size; i++) {
          Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(rfid.uid.uidByte[i], HEX);
        }
        Serial.println();
      }

//      rfid.PICC_HaltA(); // halt PICC
//      rfid.PCD_StopCrypto1(); // stop encryption on PCD
      }
    }
}

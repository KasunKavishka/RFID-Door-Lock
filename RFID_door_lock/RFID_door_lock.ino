//Kasun Kavishka Herath
//Written for a Hackathon
//Team Elementry - RFID Door Lock

#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
#define LED_G 5 //define green LED pin
#define LED_R 4 //define red LED
#define RELAY 3 //relay pin
#define BUZZER 2 //buzzer pin
#define ACCESS_DELAY 2000
#define DENIED_DELAY 1000
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

// Program variables
char *RFIDTags[] = {"AA AA AA AA", "BB BB BB BB", "CC CC CC CC",
                    "DD DD DD DD", "EE EE EE EE", "FF FF FF FF"};
bool readApprovalDenied = false;
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  digitalWrite(RELAY, HIGH);
  Serial.println("Put your card to the reader...");
  Serial.println();



}
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }

  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();


  // array checking to check wheather ID matches with an ID in the array
  for (int i=0; i < sizeof(*RFIDTags)- 1; i++) {
    if (content.substring(1) == (*RFIDTags[i])) 
    {
      Serial.println("Authorized access");
      Serial.println();
      delay(500);
      digitalWrite(RELAY, LOW);
      digitalWrite(LED_G, HIGH);
      delay(ACCESS_DELAY);
      digitalWrite(RELAY, HIGH);
      digitalWrite(LED_G, LOW);
      readApprovalDenied = false;
    } else {
      readApprovalDenied = true;
    }

  }

  // when a card rejects, an error will be shown
  if (readApprovalDenied == true)  {
    Serial.println(" Access denied");
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 300);
    delay(DENIED_DELAY);
    digitalWrite(LED_R, LOW);
    noTone(BUZZER);
    readApprovalDenied = false;
  }
}


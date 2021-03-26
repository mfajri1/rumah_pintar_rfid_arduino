/*
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

//libarary rfid
#include <MFRC522.h>
#include <SPI.h>

//library lcd
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// library Servo
#include <Servo.h>

// library dht11
#include <DHT.h>
#include <Adafruit_Sensor.h>

//konfigurasi pin reset dan ss
#define RST_PIN         5          // Configurable, see typical pin layout above
#define SS_PIN          53         // Configurable, see typical pin layout above

#define PINTU 22
#define KIPAS 26
#define NFOKUS 28

#define limit1 14
#define limit2 15

#define ON 1
#define OFF 0

//konfig lcd
LiquidCrystal_I2C lcd(0x27, 16, 2);

//konfig dht11
DHT dht(2, DHT11); //Pin, Jenis DHT

//konfig rfid
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
MFRC522::MIFARE_Key key;

// konfigurasi setting id dari rfid
String uid1 = "44 29 FE 4C";
String uid2 = "26 14 98 E5";
String uid3 = "D4 12 F7 4C";
String uid4 = "84 5C FF 4C";

//dekalarasi variable
int pinBtn = 8;
int pinBtn2 = 10;
int papanKiri = 40;
int papanKanan = 41;
int ir1 = 32;
int ir2 = 33;
int ir3 = 34;
int ir4 = 35;
int ir5 = 36;
int ir6 = 37;
//
Servo servoAne;
//
int angka = 0;
int angka2 = 0;

int data1;

void setup() {
  pinMode(PINTU, OUTPUT);
  pinMode(KIPAS, OUTPUT);
  pinMode(NFOKUS, OUTPUT);
  pinMode(papanKiri, OUTPUT);
  pinMode(papanKanan, OUTPUT);
  pinMode(pinBtn, INPUT_PULLUP);
  pinMode(pinBtn2, INPUT_PULLUP);
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(ir3, INPUT);
  pinMode(ir4, INPUT);
  pinMode(ir5, INPUT);
  pinMode(ir6, INPUT);
  pinMode(limit1, INPUT);
  pinMode(limit2, INPUT);

  lcd.begin();                    
  lcd.backlight();

  servoAne.attach(9);
 
  dht.begin();
  
  Serial.begin(9600);   // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      
  mfrc522.PCD_Init();   
  delay(4);       
  
  digitalWrite(KIPAS, ON);
  digitalWrite(KIPAS, ON);
  servoAne.write(0);
  digitalWrite(papanKanan, OFF);
  digitalWrite(papanKiri, ON);

  bacaLimitAwal();
  
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  Serial.println(F("Scan a MIFARE Classic PICC to demonstrate read and write."));
  Serial.print(F("Using key (for A and B):"));
  dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
  Serial.println();
  Serial.println(F("BEWARE: Data will be written to the PICC, in sector #1"));

  tampilLcd("Selamat Datang", "Sekolah Pintar", 2000);
  
}

void loop() {
  bacaButton();
  bacaButton2();
  bacaSuhu();
  bacaInfrared1();
  bacaInfrared2();
  bacaInfrared3();
  bacaInfrared4();
  bacaInfrared5();
  bacaInfrared6();
  bacaRfid();
  bacaBluetooht();
} 

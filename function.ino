void bacaLimitAwal(){
  while(1){
    int limitStat = digitalRead(limit1);
    Serial.println(limitStat);
    if (limitStat == 0){
      digitalWrite(papanKanan, ON); 
      digitalWrite(papanKiri, OFF);
      delay(200);
      digitalWrite(papanKanan, OFF); 
      digitalWrite(papanKiri, OFF);
      return 1;
    }else{
      digitalWrite(papanKanan, OFF);
      digitalWrite(papanKiri, ON);  
    }
  }
}

void tampilLcd(String baris1, String baris2, int del){
  if(del == 0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(baris1);
    lcd.setCursor(0,1);
    lcd.print(baris2);  
  }else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(baris1);
    lcd.setCursor(0,1);
    lcd.print(baris2);
    delay(del);
  }
  
}

void prosesHapus(){
  while(1){
    digitalWrite(papanKanan, ON);
    digitalWrite(papanKiri, OFF);
    int statLimKiri = digitalRead(limit2);
    if(statLimKiri == 0){
      digitalWrite(papanKanan, OFF);
      digitalWrite(papanKiri, ON);
      while(1){
        int statLimKanan = digitalRead(limit1);
        if(statLimKanan == 0){
          digitalWrite(papanKanan, ON);
          digitalWrite(papanKiri, OFF);
          delay(200);
          digitalWrite(papanKanan, OFF);
          digitalWrite(papanKiri, OFF);
          return 1;
        }
      }    
    }
  }
}



void prosesPintu(String nama){
  if (nama != ""){
    Serial.print("nama : ");
    Serial.println(nama);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Nama : ");
    lcd.print(nama);
    lcd.setCursor(0,1);
    lcd.print("Silahkan Masuk");
    digitalWrite(PINTU, HIGH);
    delay(1000);
    servoAne.write(90);
    delay(4000);
    servoAne.write(0);
    delay(1000);
    digitalWrite(PINTU, LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Dekatkan Kartu ...");  
  }else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Dekatkan Kartu ...");
  }  
}

void bacaBluetooht(){

  if (Serial.available()> 0){
    char data1 = Serial.read(); // simpan data itu di variable 'data'
    
    if (data1 == '1'){
       tampilLcd("Proses", "Hapus Papan", 0);
       prosesHapus();
       return;
    }
    
    if(data1 == '2'){
      tampilLcd("Nfokus", "Aktif", 2000);
      delay(350);
      angka2++;
      if(angka2 == 1){
        digitalWrite(NFOKUS, HIGH); //maka LED1 nyala
      }else if(angka2 == 2){
        digitalWrite(NFOKUS, LOW); //maka LED1 nyala
        angka2 = 0;  
      }
      return;
    }
    return;
  }
}


void bacaRfid(){
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  // Dump debug info about the card; PICC_HaltA() is automatically called
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  
  String content= "";
  byte letter;

  for (byte i = 0; i < mfrc522.uid.size; i++){
    tampilLcd("Proses Scan", " ", 0);
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();

  // setting kondisi rfid jika terbaca  
  String nama="";
  int keadaan = 0;
  if (content.substring(1) == uid1){
    Serial.println("Authorized access");
    nama ="Joni";  
    keadaan = 1;
    prosesPintu(nama);
  }
  else if (content.substring(1) == uid2){
    Serial.println("Authorized access");
    nama = "Riki"; 
    keadaan = 2;
    prosesPintu(nama);
  }
  else if (content.substring(1) == uid3){
    Serial.println("Authorized access");
    nama = "bede";
    keadaan = 3;
    prosesPintu(nama);
  }
  else if (content.substring(1) == uid4){
    Serial.println("Authorized access");
    nama = "Andro";
    keadaan = 4;
    prosesPintu(nama);
  }
  else{
    tampilLcd("Access Denied", "", 2000);
  } 
  
}

void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}

void bacaButton(){
  int sensorValue = digitalRead(pinBtn);
   if (sensorValue == LOW){  
      prosesHapus();         
   }
}

void bacaButton2(){
  int sensorValue2= digitalRead(pinBtn2);
   if (sensorValue2 == LOW){  
      delay(350);
      angka2++;
      if(angka2 == 1){
        tampilLcd("Nfokus", "Aktif", 0);
        digitalWrite(NFOKUS, HIGH); //maka LED1 nyala
      }else if(angka2 == 2){
        tampilLcd("Nfokus", "Aktif", 1000);  
        digitalWrite(NFOKUS, LOW); //maka LED1 nyala
        angka2 = 0;  
      }
      return;         
   }
}


void bacaSuhu(){
  int kelembaban = dht.readHumidity();
  int suhu = dht.readTemperature();
  
  if(suhu > 34){
    digitalWrite(KIPAS, LOW);
  }else{
    digitalWrite(KIPAS, HIGH);
  }  
}

void bacaInfrared1(){
  int status1 = digitalRead(ir1);
  if (status1 == 0){
    tampilLcd("Bangku 1", "Status : Isi", 500); 
  }
}

void bacaInfrared2(){
  int status2 = digitalRead(ir2);    
  if (status2 == 0){
    tampilLcd("Bangku 2", "Status : Isi", 500);   
  }
}

void bacaInfrared3(){
  int status3 = digitalRead(ir3);
  if (status3 == 0){
    tampilLcd("Bangku 3", "Status : Isi", 500);  
  }
}

void bacaInfrared4(){
  int status4 = digitalRead(ir4);  
  if (status4 == 0){
    tampilLcd("Bangku 4", "Status : Isi", 500);
  }
}

void bacaInfrared5(){
  int status5 = digitalRead(ir5);  
  if (status5 == 0){
    tampilLcd("Bangku 5", "Status : Isi", 500);
  }
}

void bacaInfrared6(){
  int status6 = digitalRead(ir6);  
  if (status6 == 0){
    tampilLcd("Bangku 6", "Status : Isi", 500);
  }
}

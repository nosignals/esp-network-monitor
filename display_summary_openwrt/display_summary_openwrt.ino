/*
KONFIGURASI : 
1. WIFI
2. INTERFACE
3. SERVER API ( DI TAB parse.h)
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "struct.h"
#include "parse.h"
#include "dispChar.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* ssid = "Gterongers++"; // SSID WIFI ANDA
const char* password = ""; // PASSWORD WIFI ANDA ( JIKA TANPA PASSWORD, KOSONGIN AJA )
String iface = "eth0"; // INTERFACE YANG MAU DI MONITORING

void setup() {
  lcd.begin();
  lcd.createChar(0, up);
  lcd.createChar(1, down);
  lcd.createChar(2, cpu);
  lcd.createChar(3, ram);
  lcd.createChar(4, temp);
  lcd.createChar(5, celc);
  lcd.createChar(6, full);
  lcd.createChar(7, none);
  
  lcd.setCursor(0,1);
  lcd.print(String(ssid));
  lcd.setCursor(0,0);
  lcd.print("Connecting.");
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    lcd.print(".");
  }
  loadDisp();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    for(int x=1;x<=10;x++){
        parseNetwork(iface);
        disp0();
      }
    for(int x=1;x<=10;x++){
        parseSysinfo();
        parseTemp();
        disp1();
      }
    parseVnstat(iface);
    disp2();
    
  }
}
void loadDisp(){
  lcd.clear();
  lcd.print("Connected!");
  parseBoard();
  lcd.setCursor(0,1);
  lcd.print(String(board.hostName));
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(String(board.kernel));
  lcd.setCursor(0,1);
  lcd.print(String(board.version));
  delay(3000);
}
void disp2(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(String(iface)+" Month(GB)");
  lcd.setCursor(0,1);
  lcd.write(0);
  lcd.print(":"+String(vnstat.monthUsageTX/1000000));
  lcd.setCursor(8,1);
  lcd.write(1);
  lcd.print(":"+String(vnstat.monthUsageRX/1000000));

  delay(5000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(String(iface)+" Today(GB)");
  lcd.setCursor(0,1);
  lcd.write(0);
  lcd.print(":"+String(vnstat.todayUsageTX/1000000));
  lcd.setCursor(8,1);
  lcd.write(1);
  lcd.print(":"+String(vnstat.todayUsageRX/1000000));
  delay(5000);
}
void disp0(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SIG");
  if(netdata.DOWNLOAD >=1000 || netdata.UPLOAD <= -1000){
    lcd.setCursor(0,1);
    lcd.write(6);
    if(netdata.DOWNLOAD >=5000 || netdata.UPLOAD <= -5000){
      lcd.setCursor(1,1);
      lcd.write(6);
      if(netdata.DOWNLOAD >=10000 || netdata.UPLOAD <= -10000){
        lcd.setCursor(2,1);
        lcd.write(6);
        }else{
          lcd.setCursor(2,1);
          lcd.write(7);
        }
      }else{
        lcd.setCursor(1,1);
        lcd.write(7);
        }
    }else{
      lcd.setCursor(0,1);
      lcd.write(7);
      }
  lcd.setCursor(4,0);
  lcd.write(0);
  lcd.print(":"+String(netdata.DOWNLOAD/1000));
  lcd.print(" Mbps");
  lcd.setCursor(4,1);
  lcd.write(1);
  lcd.print(":"+String(netdata.UPLOAD/1000*-1));
  lcd.print(" Mbps");
  delay(2000);
}
void disp1(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write(2);
  lcd.print(":"+String(sysinfo.cpu));
  lcd.print("%");
  lcd.setCursor(9,0);
  lcd.write(4);
  lcd.print(":"+String(sysinfo.temp/1000));
  lcd.write(5);
  lcd.setCursor(0,1);
  lcd.write(3);
  lcd.print(":"+String(sysinfo.memUsage/1000000000));
  lcd.print(" / "+String(sysinfo.memTot/1000000000));
  lcd.print("GB");
  delay(2000);
}

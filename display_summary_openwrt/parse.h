/*
SEMUA FUNGSI DATA SUDAH TAK RINGKAS, JADI KALIAN BISA MEMODIFIKASI / MEMBUAT DASHBOARD BARU DENGAN FUNGSI DIBAWAH
hasil data :
1. getData(data);
1.1 String jsonData

2. parseVnstat(interface);
2.1 Hari ini (vnstat.todayUsageTX & vnstat.todayUsageRX)
2.2 Kemarin (vnstat.yesterdayUsageTX & vnstat.yesterdayUsageRX)
2.3 Bulan ini (vnstat.monthUsageTX &vnstat.monthUsageRX)
2.4 Bulan lalu (vnstat.lastMonthUsageTX & vnstat.lastMonthUsageRX)
2.5 Total penggunaan (vnstat.totalUsageTX & vnstat.totalUsageRX)

3. parseSysinfo();
3.1 Penggunaan Memory / RAM (sysinfo.memUsage , sysinfo.memTot & sysinfo.memFree)
3.2 Uptime (sysinfo.uptime) data berupa UnixTime
3.3 CPU (sysinfo.cpu) data berupa persentase

4. parseNetwork(interface);
4.1 Upload Speed (netdata.UPLOAD)
4.2 Download Speed (netdata.DOWNLOAD)

5.parseBoard();
5.1 board.kernel (Kernel device yang digunakan)
5.2 board.version (Versi OpenWrt)
5.3 board.hostName (Hostname)

6. parseTemp();
6.1 sysinfo.temp (Temperatur CPU)
*/
WiFiClient wifiClient;
StaticJsonDocument<16384> doc;

String apiUrl = "http://192.168.1.1/api/api.php?";
String jsonData = " ";

void getData(String dt){ // Usage : getData(data yang diambil);
  HTTPClient http;
  http.begin(wifiClient,apiUrl+dt);
  if (http.GET() > 0) {
      jsonData = http.getString();
      Serial.println("GET DATA OK");
    }
  else Serial.println("ERR : " + http.GET());
}
void parseVnstat(String interfaces){ // Usage : parseVnstat(Interfaces vnstat);
    getData("vnstat="+interfaces);
    DeserializationError err = deserializeJson(doc, jsonData);
    if (err){
      Serial.print("ERR : ");
      Serial.println(err.f_str());
    }else{
      vnstat.todayUsageTX = doc["vnstat"]["data"][0]["interfaces"][0]["traffic"]["days"][0]["tx"];
      vnstat.todayUsageRX = doc["vnstat"]["data"][0]["interfaces"][0]["traffic"]["days"][0]["rx"];
      
      vnstat.yesterdayUsageTX = doc["vnstat"]["data"][0]["interfaces"][0]["traffic"]["days"][1]["tx"];
      vnstat.yesterdayUsageRX = doc["vnstat"]["data"][0]["interfaces"][0]["traffic"]["days"][1]["rx"];
      
      vnstat.monthUsageTX = doc["vnstat"]["data"][0]["interfaces"][0]["traffic"]["months"][0]["tx"];
      vnstat.monthUsageRX = doc["vnstat"]["data"][0]["interfaces"][0]["traffic"]["months"][0]["rx"];
      
      vnstat.lastMonthUsageTX = doc["vnstat"]["data"][0]["interfaces"][0]["traffic"]["months"][1]["tx"];
      vnstat.lastMonthUsageRX = doc["vnstat"]["data"][0]["interfaces"][0]["traffic"]["months"][1]["rx"];
      
      vnstat.totalUsageTX = doc["vnstat"]["data"][0]["interfaces"][0]["traffic"]["total"]["tx"];
      vnstat.totalUsageRX = doc["vnstat"]["data"][0]["interfaces"][0]["traffic"]["total"]["rx"];
      
      Serial.println("VNSTAT PARSE OK");
    }
}
void parseSysinfo(){ // Usage : parseSysinfo();
    getData("netdata=system.cpu&system=info");
    DeserializationError err = deserializeJson(doc, jsonData);
    if (err){
      Serial.print("ERR : ");
      Serial.println(err.f_str());
    }else{
      sysinfo.memFree = doc["system"]["data"][0]["memory"]["free"];
      sysinfo.memTot = doc["system"]["data"][0]["memory"]["total"];
      sysinfo.memUsage = sysinfo.memTot-sysinfo.memFree;
      sysinfo.uptime = doc["system"]["data"][0]["uptime"];
      sysinfo.cpu = doc["netdata"]["data"][0]["data"][0][6];
      Serial.println("SYSINFO DATA PARSE OK");
    }
}
void parseNetwork(String iface){ // Usage : parseNetwork(interface network);
    getData("netdata=net."+iface);
    DeserializationError err = deserializeJson(doc, jsonData);
    if (err){
      Serial.print("ERR : ");
      Serial.println(err.f_str());
    }else{
      netdata.UPLOAD = doc["netdata"]["data"][0]["data"][0][2];
      netdata.DOWNLOAD = doc["netdata"]["data"][0]["data"][0][1];
      Serial.println(String(netdata.UPLOAD));
      Serial.println(String(netdata.DOWNLOAD));
      Serial.println("NETWORK DATA PARSE OK");
    }
}
void parseBoard(){ // Usage : parseBoard();
    getData("system=board&netdata=temp");
    DeserializationError err = deserializeJson(doc, jsonData);
    if (err){
      Serial.print("ERR : ");
      Serial.println(err.f_str());
    }else{
      board.kernel = doc["system"]["data"][0]["kernel"];
      board.hostName = doc["system"]["data"][0]["hostname"];
      board.version = doc["system"]["data"][0]["release"]["version"];
      Serial.println("BOARD DATA PARSE OK");
    }
}
void parseTemp(){
    getData("netdata=temp");
    DeserializationError err = deserializeJson(doc, jsonData);
    if (err){
      Serial.print("ERR : ");
      Serial.println(err.f_str());
    }else{
      sysinfo.temp = doc["netdata"]["data"][0]["temp"];
      Serial.println("TEMP DATA PARSE OK");
    }
}

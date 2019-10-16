
#include <WiFi.h>
//#include <WebServer.h>
#include <AutoConnect.h>
#include <AutoConnectCredential.h>

//WebServer Server;
AutoConnect       Portal;//(Server);
AutoConnectConfig  Config;

bool bPortalStarted;

void _setupMAC(){
  chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
 //Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));//print High 2 bytes
 //Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.
 s_chipid =  String((uint16_t)(chipid>>32),HEX)+String((uint32_t)chipid,HEX);
  writeSerial("ESP32 Chip ID String:"+s_chipid);
}

bool portalStarted(IPAddress ip) {
  bPortalStarted = true;
  writeSerial("Portal started at IP:" + WiFi.localIP().toString());
  return true;
}



String wl_status_to_string(int status) {
  switch (status) {
    case WL_NO_SHIELD: return "WL_NO_SHIELD";
    case WL_IDLE_STATUS: return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL: return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED: return "WL_SCAN_COMPLETED";
    case WL_CONNECTED: return "WL_CONNECTED";
    case WL_CONNECT_FAILED: return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED: return "WL_DISCONNECTED";
  }
}


void _updateWifiState(){
    writeSerial("_updateWifiState Wifi local IP: "+WiFi.localIP().toString());
    IPAddress Wifi_IP = WiFi.localIP();
    global_wifi_IP = Wifi_IP.toString();
}


String _checkWifiState(){
  if(!bPortalStarted) {
   writeSerial("_checkWifiState Wifi connection to WLAN "+WiFi.SSID());
    writeSerial("_checkWifiState Wifi connection status:"+wl_status_to_string( WiFi.status()));
    writeSerial("ap_portal:"+String(Config.apid)+" password:"+String(Config.psk)+" IP is:"+String(Config.staip));
   
    }
    else
    {
       writeSerial("_checkWifiState Wifi connection to portal");
       writeSerial("ap_portal:"+String(Config.apid)+" password:"+String(Config.psk));
    }
    writeSerial("Exit _checkWifiState");
    
}

uint8_t getCredentialNumber(){
  AutoConnectCredential  ac(CREDENTIAL_OFFSET);
  return ac.entries();          // Get number of entries.
}

bool isLastCredential(uint8_t index){
  if(index<getCredentialNumber()-1) return false;
  else return true;
}



station_config_t _loadNextCredential(uint8_t index ) {
  AutoConnectCredential  ac(CREDENTIAL_OFFSET);
  station_config_t  entry;
  String printcontent="";
   ac.load(index, &entry);
  printcontent += String("SSID Entry: ") + String((char *)entry.ssid)+" Password:"+ String((char *)entry.password);
  writeSerial("LoadCredential() the saved credentials are:"+printcontent);
  return entry;
}
 


bool retryConnection(){
  writeSerial("retryConnection() called"); 
  station_config_t sconfig;
  for(uint8_t index=0;index <getCredentialNumber() ;index++)
  {
    sconfig = _loadNextCredential(index);
    if(isLastCredential(index)){
      writeSerial("retryConnection() isLastCredential"); 
      Config.autoRise=true;
      Portal.config(Config);
        OLED_write("Attempt Connect SSID:"+ (String)(const char *)sconfig.ssid);
      return Portal.begin((const char *)sconfig.ssid,(const char *)sconfig.password,_SSID_CONNECT_TIMER);
    }
    else{
      OLED_write("Attempt Connect SSID:"+ (String)(const char *)sconfig.ssid);
      Portal.begin((const char *)sconfig.ssid,(const char *)sconfig.password,_SSID_CONNECT_TIMER);
    }
   
  }
}



void _setupAutoconnect() {
   bPortalStarted=false;
  writeSerial("_setupAutoconnect called");

  // Enable saved past credential by autoReconnect option,
  // even once it is disconnected.
  Config.autoReconnect = true;
  Config.apid = s_chipid;
  Config.portalTimeout = _PORTAL_TIMER;
  Config.autoRise=false;
  Portal.config(Config);
  Portal.onDetect(portalStarted);
   // Establish a connection with an autoReconnect option.
   writeSerial("Establish a connection with an autoReconnect option - Portal.begin to SSID:" _SSID_NAME " with password:" _SSID_PASSWORD " with timer: "+String(_SSID_CONNECT_TIMER)+ "(ms)");
  OLED_write("Connect SSID:" _SSID_NAME);
  if (Portal.begin(_SSID_NAME,_SSID_PASSWORD,_SSID_CONNECT_TIMER)) {
    OLED_write("Connect SSID success. SSID:"+WiFi.SSID());
    writeSerial("Connection established, AutoConnect service started with WIFI_STA mode. WiFi connected at:" + WiFi.localIP().toString());
     bPortalStarted=false; 
  }else if(retryConnection()){
      gcurrent_ssid = WiFi.SSID();
      OLED_write("Connect SSID success. SSID:"+WiFi.SSID());
     writeSerial("retryConnection - Connection established, AutoConnect service started with WIFI_STA mode. WiFi connected at:" + WiFi.localIP().toString());
     bPortalStarted=false; 
  }
  else{
    OLED_write("Connect SSID failed. Open Portal");
    writeSerial("WiFi NOT connected. Captive portal started with WIFI_AP_STA mode");
    bPortalStarted=true;
    //_checkWifiState();
  }
  writeSerial("_setupAutoconnect exit");
}




void _handlePortal(){
   writeSerial("_handlePortal called: Portal.handleClient");
    Portal.handleClient();
    if (WiFi.status() == WL_IDLE_STATUS) {
      writeSerial("WiFi.status is  WL_IDLE_STATUS. ESP.restart called");
      bPortalStarted=false;
      ESP.restart();
  
    }
}

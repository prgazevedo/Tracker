
/*
 * Copyright 2018 Pedro Azevedo (prgazevedo@gmail.com)
 * and other contributors as indicated by the @author tags.
 **********************************************************************
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <TinyGPS++.h>
// The TinyGPS++ object
TinyGPSPlus gps;
TinyGPSCustom vdop(gps, "GPGSA", 17);
// For stats that happen every 5 seconds
unsigned long last = 0UL;
//SLEEP REBOOT Vars
RTC_DATA_ATTR int bootCount = 0;
//MAC Address
uint64_t chipid;
String s_chipid;
//GPS 
//Structs to hold location values
//Temporary Struct to hold location values (bigger size version)
uint8_t precisionbillionths[4];

//Struct to hold location values (small size version)
typedef struct GPSCoord
{ 
   uint8_t deg;
   uint8_t billionths[3];
public:
   GPSCoord() : deg(0){}
}GPSCoord;

//Struct to hold LAT LNG values (small size version)
typedef struct GpsData{
 GPSCoord latitude,longitude;
 uint8_t hdop;
 uint8_t satellites;
 uint16_t altitude;
 public:
   GpsData() : hdop(0),satellites(0),altitude(0){}
   String toHEXString()
   {
      return (String(latitude.deg,HEX)+
      String(latitude.billionths[0],HEX)+
      String(latitude.billionths[1],HEX)+
      String(latitude.billionths[2],HEX)+
      String(longitude.deg,HEX)+
      String(longitude.billionths[0],HEX)+
      String(longitude.billionths[1],HEX)+
      String(longitude.billionths[2],HEX)+
      String(hdop,HEX)+
      String(satellites,HEX))+
      String((uint8_t)(altitude),HEX)+
      String((uint8_t)(altitude >> 8),HEX);
   }
}gpsData;
gpsData gdata;


// count of outgoing messages
byte packetNr = 0;            
//Time of last Lora data packet
long lastPacketTime = 0;
//Used for outputing to serial and html

String gs_current_latitude;
String gs_current_longitude;
String gs_current_hdop;
String gs_current_altitude;
String gs_current_satellites;
String gspath;
//For receiver only
String gs_rcv_latitude;
String gs_rcv_longitude;

typedef struct GpsPath{
  String lat;
  String lng;
}gpsPath;



//LORA radio signal data
int rssi_value = 0;
int snr_value = 0;

//WIFI IP
String global_wifi_IP = "Not Connected";
String gcurrent_ssid = _SSID_NAME;
//WIFI Connection State
bool global_wifi_conn_state = false;
//BATTERY
String sVBAT ="";
#endif

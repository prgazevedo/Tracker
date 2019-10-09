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
#include "_Definitions.h" 
#include "_Global.h" 


void writeSerial(String toWrite)
{
  if(_ROLE==0)
  {
    Serial.println("Module SENDER: "+toWrite);
  }
  else{
    Serial.println("Module RECEIVER: "+toWrite);
  }
}

void _SerialInit()
{

  Serial.begin(115200);
  while (!Serial);
  delay(50); 
  writeSerial("_SerialInit");
}


void _logRawGPSData()
{
    if(DEBUG_GPS){
      writeSerial("debugRawGPSData GPSDATA in DEBUG MODE! No REAL GPS data being collected");
      writeSerial("DEBUG VALUES ARE: LatBillionths = 775760333");
      writeSerial("DEBUG VALUES ARE: LNGBillionths = 95114333");
     uint32_t tmpLatBillionths = 775760333;
      writeSerial("debugRawGPSData debug - DEBUG_Lat().billionths.BIN:"+  int2binstr(tmpLatBillionths,32));
      uint32_t tmpLngBillionths = 95114333;
      writeSerial("debugRawGPSData debug - DEBUG_Lng().billionths.BIN:"+  int2binstr(tmpLngBillionths,32));
    }
    else{
      writeSerial("debugRawGPSData GPSDATA in REAL MODE! REAL GPS data being collected");
      writeSerial("debugRawGPSData debug - rawLat() negative:"+  String(gps.location.rawLat().negative));
      writeSerial("debugRawGPSData debug - rawLat().deg.DEC:"+  String(gps.location.rawLat().deg,DEC));
      writeSerial("debugRawGPSData debug - rawLat().billionths.DEC:"+  String(gps.location.rawLat().billionths,DEC));
      writeSerial("debugRawGPSData debug - rawLng() negative:"+  String(gps.location.rawLng().negative));
      writeSerial("debugRawGPSData debug - rawLng().deg.DEC:"+  String(gps.location.rawLng().deg,DEC));
      writeSerial("debugRawGPSData debug - rawLng().billionths.DEC:"+  String(gps.location.rawLng().billionths,DEC));
    }
    writeSerial("debugRawGPSData debug - gdata.latitude.billionths.BIN: longitude[3][2][1]" + 
    int2binstr(gdata.latitude.billionths[2])+
    int2binstr(gdata.latitude.billionths[1])+
    int2binstr(gdata.latitude.billionths[0]) );
    writeSerial("debugRawGPSData debug - gdata.longitude.billionths.BIN: longitude[3][2][1]" + 
    int2binstr(gdata.longitude.billionths[2])+
    int2binstr(gdata.longitude.billionths[1])+
    int2binstr(gdata.longitude.billionths[0]));
    writeSerial("debugRawGPSData debug - gdata.altitude: "+ String(gdata.altitude,DEC));
    writeSerial("debugRawGPSData debug - gdata.hdop: "+ String(gdata.hdop,DEC));
    writeSerial("debugRawGPSData debug - gdata.satellites: "+ String(gdata.satellites,DEC));
  
}

void _logCoordString(GPSCoord debugGData)
{
    writeSerial(" getCoordString Degrees was found to be  DEG "+String((debugGData.deg),BIN));
    writeSerial(" getCoordString Degrees was found to be  MASK "+String((0x80),BIN));
    writeSerial(" getCoordString Degrees was found to be  DEG&MASK "+String((debugGData.deg)&(0x80),BIN));
    writeSerial(" getCoordString Degrees in Decimal is "+  String((debugGData.deg),DEC));
   uint32_t out = *(uint32_t*)(debugGData.billionths);
    writeSerial(" getCoordString assigned Billionths in Decimal is "+  String((out),DEC));
    writeSerial(" getCoordString assigned Billionths in Binary is "+  String((out),BIN));
    out = (out & 0x00ffffff) ;
    writeSerial(" getCoordString masked Billionths in Decimal is "+  String((out),DEC));
    writeSerial(" getCoordString masked Billionths in Binary is "+  String((out),BIN));
    out = out << 8;
    writeSerial(" getCoordString shifted Billionths in Decimal is "+  String((out),DEC));
    writeSerial(" getCoordString shifted Billionths in Binary is "+  String((out),BIN));
}

void _log_packet_data( ){
   if(_ROLE==0)
    {
      writeSerial("packet sent:");
    }
    else{
      writeSerial("packet received:");
    }

   writeSerial(" latitude:"+gs_current_latitude);
   writeSerial(" longitude:"+gs_current_longitude);
   writeSerial(" altitude:"+gs_current_altitude);
   writeSerial(" hdop:"+gs_current_hdop);
   writeSerial(" satellites:"+gs_current_satellites);
   writeSerial(" total size of packet is:"+String(sizeof(gdata),DEC)+" bytes");
   if(_ROLE==1)
    {
        writeSerial(" RSSI: "+String(rssi_value));
        writeSerial(" SNR: "+String(snr_value));
    }
 

}

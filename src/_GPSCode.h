
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



#include "_Global.h"



// The serial connection to the GPS device
void _SetupGPS()
{

  Serial2.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin);
  
  Serial.print(F("Using TinyGPS++ library v. ")); 
  Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
}

void _writeGPSHeader(){
  Serial.println(F("Sats HDOP  Latitude   Longitude   Fix  Date       Time     Date Alt    Course Speed Card   Chars Sentences Checksum"));
  Serial.println(F("           (deg)      (deg)       Age                      Age  (m)    --- from GPS ----       RX    RX        Fail"));
  Serial.println(F("---------------------------------------------------------------------------------------------------------------------"));
}



void _processGPSData(){
  // Dispatch incoming characters
  while (Serial2.available() > 0)
  {
    char c;
    c = Serial2.read();
    Serial.write(c);
    gps.encode(c);
  }
}

bool _validGPSData(){
  if(DEBUG_GPS)  return false;
  else if( (gps.hdop.hdop()>21) && 
  (gps.satellites.value()>3) &&
  gps.hdop.isValid()  && 
  gps.satellites.isValid() && 
  gps.location.isValid()) return true;
}


void _encodeLocationSummary(){
  writeSerial("-----------------------------");
  writeSerial("_encodeLocationSummary Called");
  writeSerial("-----------------------------");
  _writeGPSHeader();
  printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
  printFloat(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);
  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
  printInt(gps.location.age(), gps.location.isValid(), 5);
  printDateTime(gps.date, gps.time);
  printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
  printFloat(gps.course.deg(), gps.course.isValid(), 7, 2);
  printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);
  printStr(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.deg()) : "*** ", 6);

   printInt(gps.charsProcessed(), true, 6);
  printInt(gps.sentencesWithFix(), true, 10);
  printInt(gps.failedChecksum(), true, 9);
  Serial.println();
  
  smartDelay(1000);

  if (millis() > 5000 && gps.charsProcessed() < 10)
    writeSerial(F("No GPS data received: check wiring"));
}


uint32_t restructureBillionths( uint8_t* billionths)
{
  //uint32_t out = *(uint32_t*)&gData.billionths;
  uint32_t out = *(uint32_t*)billionths;
  //Need to mask the unset memory of the MSB byte to zeros and left shift 1 byte to get the original value 
  out = (out & 0x00ffffff) ;
  out = out << 8;
  return out;
}

void shrinkGPSData(uint8_t *coord){
  (coord)[0]= precisionbillionths[1];
  (coord)[1]= precisionbillionths[2];
  (coord)[2]= precisionbillionths[3];
  restructureBillionths(coord);
}

void getAltitude()
{
  if(DEBUG_GPS) gdata.altitude=5000;
  else gdata.altitude = (uint16_t)(gps.altitude.meters());
}

void getHDOP()
{
  if(DEBUG_GPS) gdata.hdop = 3;
  else gdata.hdop = (uint16_t)(gps.hdop.hdop());
}

void getSatellites()
{
  if(DEBUG_GPS) gdata.satellites = 4;
  else gdata.satellites = (uint8_t)(gps.satellites.value());
}

void getLAT()
{
   if(DEBUG_GPS){
      gdata.latitude.deg = 38;
      uint32_t tmpLatBillionths = 775760333;
      memcpy(precisionbillionths, &tmpLatBillionths, sizeof(precisionbillionths));
   }
   else{
      if(gps.location.rawLat().negative) gdata.latitude.deg =  gps.location.rawLat().deg | 0x80;
      else gdata.latitude.deg =  gps.location.rawLat().deg;
      memcpy(precisionbillionths, &gps.location.rawLat().billionths, sizeof(precisionbillionths));
   }
  shrinkGPSData(gdata.latitude.billionths);
}

void getLNG()
{
   if(DEBUG_GPS){
      gdata.longitude.deg = 9;
      gdata.longitude.deg = (9 | 0x80);
      uint32_t tmpLngBillionths = 95114333;
      memcpy(precisionbillionths, &tmpLngBillionths, sizeof(precisionbillionths));
   }
  else{
    if(gps.location.rawLng().negative) gdata.longitude.deg =  gps.location.rawLng().deg | 0x80;
    else gdata.longitude.deg =  gps.location.rawLng().deg;
    memcpy(precisionbillionths, &gps.location.rawLng().billionths, sizeof(precisionbillionths));
  }
  shrinkGPSData(gdata.longitude.billionths);
}

void encodeGPSData()
{   
  getLAT();
  getLNG();
  getAltitude();
  getHDOP();
  getSatellites();
  _logRawGPSData();
}

String convertCoordString(GPSCoord gData){
  
  String gDataToReturn="";
  bool negative=false;
  if(gData.deg & 0x80) negative = true;
  //uint32_t out = restructureBillionths(&gData.billionths);
    uint32_t out = *(uint32_t*)gData.billionths;
  //Need to mask the unset memory of the MSB byte to zeros and left shift 1 byte to get the original value 
  out = (out & 0x00ffffff) ;
  out = out << 8;
  if(negative)
  {
    gDataToReturn+="-";
    gData.deg=(gData.deg)&(0x7F);
  }
  gDataToReturn += (String((gData.deg),DEC)+"."+String(out,DEC));

  return gDataToReturn;

}

void _convertGDataToString(){
  gs_current_latitude = convertCoordString(gdata.latitude);
  gs_current_longitude = convertCoordString(gdata.longitude);
  gs_current_altitude=String(gdata.altitude,DEC);
  gs_current_hdop=String(gdata.hdop,DEC);
  gs_current_satellites=String(gdata.satellites,DEC);
   _logCoord(gdata.latitude);
   _logCoord(gdata.longitude);
   _logAdditionalData();
}


void _encodeLocation(){
  writeSerial("_encodeLocation called"); 
  encodeGPSData();
  _convertGDataToString();

}


void basicGPSDebug(){
  writeSerial("basicGPSDebug called");
  if (Serial2.available()) Serial.println(Serial2.read());
  else Serial.println("No data available");
}

void _getGPS()
{
    writeSerial("_getGPS called");
    //basicGPSDebug();
    _processGPSData();
    //_encodeTestStream();
    //_encodeLocationSummary();
    _encodeLocation();

}

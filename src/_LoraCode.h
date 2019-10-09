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
#include "_LED.h" 
#include "_Global.h"
#include "_Path.h"



/*
byte destinationAddress = 0xFF;      // destination to send to 11111111
byte localAddress = 0x01;     // address of this device 00000001
*/

void startLora(){
    if (!LoRa.begin(_BAND)) writeSerial("ReStarting LoRa failed!");
    else writeSerial("ReStarted LoRa !");
}

void setupLoraSettings(){
    LoRa.enableCrc();
  if(_MAXPOWER){
    LoRa.setTxPower(_MAX_TX_POWER);
    writeSerial("LoRa is using Power:"+String(_MAX_TX_POWER));
  } else {
    LoRa.setTxPower(_DEFAULT_TX_POWER);
    writeSerial("LoRa is using Power:"+String(_MAX_TX_POWER));
  }
  //For Range
  if(_LONG_RANGE)
  {
     writeSerial("LoRa is using Long Range settings- SignalBandwidth: "+String(_DEFAULT_SIGNAL_BANDWIDTH)+"Hz SpreadingFactor: "+String(_DEFAULT_SPREADING_FACTOR)+"(6-12) CodingRate:"+String(_DEFAULT_CODING_RATE)); 
     LoRa.setSignalBandwidth(_DEFAULT_SIGNAL_BANDWIDTH); // Supported values are 7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3, 41.7E3, 62.5E3, 125E3, and 250E3.
     LoRa.setSpreadingFactor(_DEFAULT_SPREADING_FACTOR); // Supported values are between 6 and 12
     LoRa.setCodingRate4(_DEFAULT_CODING_RATE); // Supported values are between 5 and 8
  }
}


void _LoraInit(){
  writeSerial("_LoraInit");
    //LORA INIT
  SPI.begin(_SCK,_MISO,_MOSI,_SS); //INIT SERIAL WITH LORA
  LoRa.setPins(_SS,_RST,_DI00); //PINOUT to be used by LORA library (deve ser chamado antes do LoRa.begin)
  
  //INIT LORA BAND
  startLora();
  setupLoraSettings();
  OLED_write("_LoraInit success!");
  writeSerial("_LoraInit success!");
}


void putGPSData(){
  LoRa.write((uint8_t*)&gdata, sizeof(gdata));
  writeSerial("putGPSData test - size of gdata is:"+ String(sizeof(gdata)));
}

bool isLoRaRadioUnConfigured(){
  if( (LoRa.getSignalBandwidth()!= _DEFAULT_SIGNAL_BANDWIDTH) &&
  (LoRa.getSpreadingFactor()!= _DEFAULT_SPREADING_FACTOR) ) return false;
  else return true;
}

void restartLoraRadio(){
  writeSerial("restartLoraRadio");
  writeSerial("getSignalBandwidth returned: "+String(LoRa.getSignalBandwidth()));
  writeSerial("getSpreadingFactor returned: "+String(LoRa.getSpreadingFactor()));
  startLora();
  setupLoraSettings();
}

void increasePacketNr(){packetNr++;};

void _sendPacket() {
   writeSerial("_sendMessage");
  //INIT LORA BAND
  if( isLoRaRadioUnConfigured()) restartLoraRadio();
  LoRa.beginPacket();
  writeSerial("beginPacket success");                   
  putGPSData(); 
  writeSerial("putGPSData success");
  LoRa.endPacket(); 
  writeSerial("endPacket success");
  //writeSerial("goingToSleep");
  //LoRa.sleep();
  
}

void _postParse(){
    gs_current_latitude = getCoordString(gdata.latitude);
    gs_current_longitude = getCoordString(gdata.longitude);
    _log_packet_data();
    _OLED_print_data();
     _addCoordsToPath(gs_current_latitude,gs_current_longitude);
     _encodePath();
}

bool _parsePacket() {
  writeSerial("LoRa _parsePacket Called");
  LoRa.readBytes((uint8_t*)&gdata, sizeof(gdata));
  rssi_value = LoRa.packetRssi();
  snr_value = LoRa.packetSnr();
  writeSerial("LoRa _Receive _parsePacket success");  
  return true;

   
}

void _LoraSendPacket(){
  writeSerial("_LoraSendPacket"); 
   _sendPacket(); 
   increasePacketNr();
   _postParse();
 
}


void _LoraReceivePacket(){
  writeSerial("_LoraReceivePacket");
  int packetSize = LoRa.parsePacket();
  if (packetSize>0) { 
    writeSerial("A packet was received calling _parsePacket");
    if(_parsePacket()) increasePacketNr();
    _postParse();
  }
}

//For Sender
bool _sendTimer(){
  if (millis() - lastPacketTime > _LORA_SEND_INTERVAL){
      //time since last send
      lastPacketTime = millis();
      return true;
 } 
 else return false;

}
//For Receiver
bool _receiveTimer(){
  if (millis() > lastPacketTime + _LORA_SEND_INTERVAL ){
      //time since last receive
      lastPacketTime = millis();
      return true;
 } 
 else return false;

}

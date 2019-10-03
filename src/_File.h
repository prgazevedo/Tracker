#ifndef _FILE_H
#define _FILE_H


#include "SPIFFS.h"
 
void _Fileinit() {
    writeSerial("_Fileinit");
 
     if(!SPIFFS.begin(true)){
    writeSerial("An Error has occurred while mounting SPIFFS");
    return;
  }
   writeSerial("Currently used bytes"+(SPIFFS.usedBytes()));
  File root = SPIFFS.open("/");
 
  File file = root.openNextFile();
 
  while(file){
 
      writeSerial("FILE: "+String(file.name()));
      file = root.openNextFile();
  }
}

void _writeGPSDataLogFile(String gpsData){
  File file = SPIFFS.open("/gpsLog.txt", FILE_WRITE);
 
  if (!file) {
    writeSerial("There was an error opening the file for writing");
    return;
  }
}

String _readPassword(){
  String password = "";
  File file = SPIFFS.open("/password.txt");
  if(!file){
    writeSerial("Failed to open file for reading");
    return "";
  }

  
  writeSerial("Password file open");
  while(file.available()){
    password+=(char)(file.read());
  }
  writeSerial("Password file read");
  file.close();
  writeSerial("Password file close");
  writeSerial("Password is: "+(String)password);
  return password;
}

#endif

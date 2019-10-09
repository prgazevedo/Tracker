#include <SimpleList.h>
#include "_Global.h"

SimpleList<gpsPath> myList;

String lastLat;
String lastLng;

bool bFirstPathPoint = true;


bool checkNoMovement(String lat, String lng){
  if( (lat.equals(lastLat)) && (lng.equals(lastLng)) ) return true;
  else return false;
}

void _addCoordsToPath(String lat, String lng){
  if (checkNoMovement(lat,lng)) 
  {
    writeSerial("_addCoordsToPath no movement detected");
    return;
  }
  else{
    gpsPath gPath;
    gPath.lat = lat;
    gPath.lng = lng;
    lastLat = lat;
    lastLng = lng;
    myList.push_back(gPath);
    writeSerial("_addCoordsToPath added: lat: "+lat+" lng: "+lng);
    if(bFirstPathPoint)
    {
      bFirstPathPoint=false;
      gs_rcv_latitude=lat;
      gs_rcv_longitude=lng;
    }
  }

}

void _encodePath(){
  
  gspath="";
  for (SimpleList<gpsPath>::iterator itr = myList.begin(); itr != myList.end(); )
  {
    gspath+=("{lat: "+((gpsPath)(*itr)).lat+", lng: "+((gpsPath)(*itr)).lng+"}");
    ++itr;
    if(itr != myList.end()){gspath+=",";}
  }
  writeSerial("_encodePath has Path:"+gspath);
      
}



unsigned long globalstartTimer;
unsigned long startTimer;
unsigned long endTimer;

void _setupDeltaTime(){
    globalstartTimer= millis();
}

void _startTimer(){
  startTimer = millis();
}

void _endTimer(){
  endTimer = millis();
  writeSerial("Timing calculation - Spent: "+ String(endTimer-startTimer,DEC) + " ms");
  
}

void _totalTimer(){
    endTimer = millis();
   writeSerial("Timing calculation - Spent: "+ String(endTimer-globalstartTimer,DEC) + " ms"); 
}

//#include "rc4dkipe_plain.h" // 4309 ms   4136 flash    262 ram
//#include "rc4dkipe_opt.h"   // 4323 ms   4104 flash    262 ram
#include <rc4dkip/rc4dkip_plain.h>  // 1101 ms   3902 flash    518 ram
//#include "rc4dkip_opt.h"    // 1135 ms   3878 flash    518 ram
//#include "rc4dkip_nop.h"    //    0 ms   3412 flash    518 ram

#include <stdio.h>

// create a FILE structure to reference our UART output function

static FILE uartout = {0} ;

// create a output function
// This works because Serial.write, although of
// type virtual, already exists.
static int uart_putchar (char c, FILE *stream)
{
  Serial.write(c) ;
  return 0 ;
}


static String printHex( uint8_t* buf, const uint8_t len) {
  String bString="";
  for (uint8_t i = 0; i < len; i++) {
   bString+=String(buf[i],HEX);
   //printf("%02x ", buf[i]);
  }
  return (bString);
  //puts("");
}

const uint8_t key[16] = "Key____________";

void _setupCrypto() {
  _startTimer();

  const uint8_t key[16] = "Key____________";
  rc4dInit(key);
}

unsigned char buffer[sizeof(gdata)] = {0};


void _encode() {
  writeSerial("_encode called");
  int buffersize = sizeof(gdata);
  //serialize data
  writeSerial( "decoded gdata is: "+gdata.toHEXString());
  memcpy(buffer,(const unsigned char*)&gdata,buffersize);
  writeSerial( "decoded buffer data is: "+printHex(buffer, buffersize)); 
  rc4dEncrypt(buffer, buffersize);
  writeSerial( "RC4 encoded buffer data is: "+printHex(buffer, buffersize)); 
   memcpy(&gdata,buffer,buffersize);
  writeSerial( "RE-copied encoded gdata from buffer is: "+gdata.toHEXString());
}

void _decode() {
  writeSerial("_decode called");
  int buffersize = sizeof(gdata);
  writeSerial( "encoded gdata is"+gdata.toHEXString());
  //serialize data
  memcpy(buffer,(const unsigned char*)&gdata,buffersize);
  writeSerial( "encoded buffer data is: "+printHex(buffer, buffersize)); 
  rc4dDecrypt(buffer, buffersize);
  writeSerial( "decoded buffer data is: "+printHex(buffer, buffersize)); 
  memcpy(&gdata,buffer,buffersize);
  writeSerial( "RE-copied decoded gdata from buffer is: "+gdata.toHEXString());
}

void _testCrypto() {
  
  uint8_t in[] = "Hello World!";
  // make the key volatile to be not optimized away in the NOP version
  const uint8_t key[16] = "Key____________";
  const uint8_t inLen = strlen((char *)in);
  
  rc4dInit(key);
  puts((char *)in);
  rc4dEncrypt(in, inLen);
  printHex(in, inLen);
  rc4dDecrypt(in, inLen);
  puts((char *)in);


  uint8_t in2[32] = { 0 };
  const uint8_t in2Len = 32;
  // make the key volatile to be not optimized away in the NOP version
  const uint8_t key2[16] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10 };

  // currently there is no 128 bit test vector to verify against
  rc4dInit(key2);
  rc4dEncrypt(in2, in2Len);
  printHex(in2, in2Len);


}


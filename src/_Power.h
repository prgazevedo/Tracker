#include "_Definitions.h"
#include "_Global.h"
#include "esp_sleep.h"

#include "esp_pm.h"
#include "esp_clk.h"
#include "esp_timer.h"
#include "rom/uart.h"

int lastPowerReadTimer = 0;
void print_wakeup_reason();

void _setupBootBegin()
{
  writeSerial("_setupBootBegin");
  if(bootCount>0){
  print_wakeup_reason();  
  writeSerial("Boot number: " + String(bootCount));
  }
}

void _setupEnd()
{
  bootCount++;
  writeSerial("setup Ended");
  
}


void _setupLowPower()
{

  writeSerial("_setupLowPower called:" );
  writeSerial("Boot count is:" +String(bootCount) );
  writeSerial("DISABLE WIFI AND BLUETOOTH IN SENDER MODE");
  WiFi.mode(WIFI_OFF);
  btStop();
}

bool _readPowerTimer()
{
  if (millis() - lastPowerReadTimer > _POWER_READ_INTERVAL)
  {
    //time since last send
    lastPowerReadTimer = millis();
    return true;
  }
  else
    return false;
}

double readPrecisionVoltage(byte pin)
{
  double reading = analogRead(pin); // Reference voltage is 3v3 so maximum reading is 3v3 = 4095 in range 0 to 4095
  if (reading < 1 || reading >= 4095)
    //return 0;
    // Added an improved polynomial, use either, comment out as required
    // return -0.000000000009824 * pow(reading,3) + 0.000000016557283 * pow(reading,2) + 0.000854596860691 * reading + 0.065440348345433;
    return -0.000000000000016 * pow(reading, 4) + 0.000000000118171 * pow(reading, 3) - 0.000000301211691 * pow(reading, 2) + 0.001109019271794 * reading + 0.034143524634089;
}

void _readBatteryPower()
{
  adcAttachPin(BATTERY_PIN);
  double dVBAT = readPrecisionVoltage(BATTERY_PIN); // OR analogRead(BATTERY_PIN)*XS*MUL/_FULL_BATTERY;
  char TempString[10];                              //  Hold The Convert Data
  dtostrf(dVBAT, 3, 2, TempString);
  sVBAT = String(TempString);
  // dtostrf( [doubleVar] , [sizeBeforePoint] , [sizeAfterPoint] , [WhereToStoreIt] )
  //Unfortunately battery values are not available in this board
  //writeSerial("Remaining battery still has VBAT:"+sVBAT+"(mV)");
  writeSerial("Remaining battery still has VBAT:" + sVBAT + "(mV)");
}

int calculateTimeToSleep()
{
  return _LORA_SEND_INTERVAL * mS_TO_uS_FACTOR; //TIME_TO_SLEEP * mS_TO_uS_FACTOR
}

void _switch_freq(int mhz)
{
  writeSerial("switch_freq called for:" + (String)mhz);
  setCpuFrequencyMhz(mhz);

  writeSerial("Frequency is set to MHz..." + (String)mhz);
}

/*
Method to print the reason by which ESP32
has been awaken from sleep
*/
void print_wakeup_reason()
{
  
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT0:
    writeSerial("Wakeup caused by external signal using RTC_IO");
    break;
  case ESP_SLEEP_WAKEUP_EXT1:
    writeSerial("Wakeup caused by external signal using RTC_CNTL");
    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    writeSerial("Wakeup caused by timer");
    break;
  case ESP_SLEEP_WAKEUP_TOUCHPAD:
    writeSerial("Wakeup caused by touchpad");
    break;
  case ESP_SLEEP_WAKEUP_ULP:
    writeSerial("Wakeup caused by ULP program");
    break;
  default:
    Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
    break;
  }
}

void _light_sleep()
{
  /* Wake up in X seconds, or when button is pressed */
  int timeToSleep = calculateTimeToSleep();
  esp_sleep_enable_timer_wakeup(timeToSleep);
  writeSerial("Setup ESP32 to sleep for every " + String(timeToSleep) + " Seconds. LoRa radio has Duty Cycle send interval of:" + String(_LORA_SEND_INTERVAL) + " ms");
  /* To make sure the complete line is printed before entering sleep mode,
       * need to wait until UART TX FIFO is empty:
       */
  uart_tx_wait_idle(CONFIG_ESP_CONSOLE_UART_NUM);

  /* Get timestamp before entering sleep */
  int64_t t_before_us = esp_timer_get_time();

  /* Enter sleep mode */
  esp_light_sleep_start();
  /* Execution continues here after wakeup */
  print_wakeup_reason();
  /* Get timestamp after waking up from sleep */
  int64_t t_after_us = esp_timer_get_time();
  writeSerial("Returned from light sleep at: " + _printInt64(t_after_us / 1000, DEC) + "ms, slept for:" + _printInt64((t_after_us - t_before_us) / 1000, DEC) + " ms\n");
}
void _deep_sleep()
{
  /* Wake up in X seconds, or when button is pressed */
  int timeToSleep = calculateTimeToSleep();
  esp_sleep_enable_timer_wakeup(timeToSleep);
  writeSerial("Setup ESP32 to sleep for every " + String(timeToSleep) + " Seconds. LoRa radio has Duty Cycle send interval of:" + String(_LORA_SEND_INTERVAL) + " ms");
  /* To make sure the complete line is printed before entering sleep mode,
       * need to wait until UART TX FIFO is empty:
       */
  uart_tx_wait_idle(CONFIG_ESP_CONSOLE_UART_NUM);

  /*
  By default, ESP32 will automatically power down the peripherals
  not needed by the wakeup source, but if you want to be a poweruser
  this is for you. Read in detail at the API docs
  http://esp-idf.readthedocs.io/en/latest/api-reference/system/deep_sleep.html
  Left the line commented as an example of how to configure peripherals.
  The line below turns off all RTC peripherals in deep sleep.
  */
  //esp_deep_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  //Serial.println("Configured all RTC Peripherals to be powered down in sleep");

    /*
  Now that we have setup a wake cause and if needed setup the
  peripherals state in deep sleep, we can now start going to
  deep sleep.
  In the case that no wake up sources were provided but deep
  sleep was started, it will sleep forever unless hardware
  reset occurs.
  */
  LoRa.end();
  LoRa.sleep();
  delay(100);

  pinMode(5, INPUT);

  pinMode(14, INPUT);
  pinMode(15, INPUT);
  pinMode(16, INPUT);
  pinMode(17, INPUT);
  pinMode(18, INPUT);
  pinMode(19, INPUT);

  pinMode(26, INPUT);
  pinMode(27, INPUT);

  delay(100);
  Serial.println("Going to sleep now");
  delay(2);
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}

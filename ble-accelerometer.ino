#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

const int groundpin = 18;
const int powerpin = 19;

const int acc1_xpin = A0;
const int acc1_ypin = A1;
const int acc1_zpin = A2;

const int acc2_xpin = A3;
const int acc2_ypin = A4;
const int acc2_zpin = A5;

int acc1_currX;
int acc1_currY;
int acc1_currZ;

int acc2_currX;
int acc2_currY;
int acc2_currZ;

int32_t gattServiceId;
int32_t accel1CharId;
int32_t accel2CharId;

void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

void setup() {

  boolean success;

  Serial.begin(115200);
  randomSeed(micros());

  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in Command mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  /* Perform a factory reset to make sure everything is in a known state */
  Serial.println(F("Performing a factory reset: "));
  if (! ble.factoryReset() ){
       error(F("Couldn't factory reset"));
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println(F("Setting device name to 'BRA': "));

  if (! ble.sendCommandCheckOK(F("AT+GAPDEVNAME=BRA")) ) {
    error(F("Could not set device name?"));
  }

  /* Add the Generic Access Service definition */
  Serial.println(F("Adding the Custom GATT Service definition: "));
  success = ble.sendCommandWithIntReply( F("AT+GATTADDSERVICE=UUID128=00-77-13-12-11-00-00-00-00-00-AB-BA-0F-A1-AF-E1"), &gattServiceId);
  if (! success) {
    error(F("Could not add Custom service"));
  }


  Serial.println(F("Adding the Notifiable characteristic for Accelerometer 1 "));
  success = ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID128=00-68-42-01-14-88-59-77-42-42-AB-BA-0F-A1-AF-E1,PROPERTIES=0x10,MIN_LEN=1, MAX_LEN=11, VALUE=0x000000, DATATYPE=1, DESCRIPTION='Baseline Accelerometer'"), &accel1CharId);
  if (! success) {
    error(F("Could not add Accel1 characteristic"));
  }
  
  
  Serial.println(F("Adding the Notifiable characteristic for Acceleromerer 2 "));
  success = ble.sendCommandWithIntReply( F("AT+GATTADDCHAR=UUID128=00-69-43-02-15-89-60-78-43-43-AC-BB-0F-A2-AF-E2,PROPERTIES=0x10,MIN_LEN=1, MAX_LEN=11, VALUE=0x000000, DATATYPE=1, DESCRIPTION='Delta Accelerometer'"), &accel2CharId);
  if (! success) {
    error(F("Could not add Accel2 characteristic"));
  }

  /* Reset the device for the new service setting changes to take effect */
  Serial.print(F("Performing a SW reset (service changes require a reset): "));
  ble.reset();

  Serial.println();
  
}

void loop() {
  int acc1_currX = analogRead(acc1_xpin);
  int acc1_currY = analogRead(acc1_ypin);
  int acc1_currZ = analogRead(acc1_zpin);
  
  int acc2_currX = analogRead(acc2_xpin);
  int acc2_currY = analogRead(acc2_ypin);
  int acc2_currZ = analogRead(acc2_zpin);

  ble.print(F("AT+GATTCHAR="));
  ble.print(accel1CharId);
  ble.print(",");
  ble.print(acc1_currX);
  ble.print(F("|"));
  ble.print(acc1_currY);
  ble.print(F("|"));
  ble.println(acc1_currZ);

  ble.print(F("AT+GATTCHAR="));
  ble.print(accel2CharId);
  ble.print(F(","));
  ble.print(acc2_currX);
  ble.print(F("|"));
  ble.print(acc2_currY);
  ble.print(F("|"));
  ble.println(acc2_currZ);

  delay(500);
}

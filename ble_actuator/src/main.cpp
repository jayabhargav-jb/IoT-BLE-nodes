#include <Arduino.h>

#include <SPI.h>
#include <BLEPeripheral.h>

// To show actuation use LED pin
#define ACTUATION_PIN   18

BLEPeripheral blePeripheral = BLEPeripheral();

BLEService ledService = BLEService("EEE0");

BLECharCharacteristic switchCharacteristic = BLECharCharacteristic("EEE1", BLERead | BLEWrite);

void blePeripheralConnectHandler(BLECentral& central) {
  Serial.print(F("Connected event, central: "));
  Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLECentral& central) {
  Serial.print(F("Disconnected event, central: "));
  Serial.println(central.address());
}

void switchCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  Serial.print(F("Characteristic event, writen: "));

  if (switchCharacteristic.value()) 
  {
    Serial.println(F("Actuator Enabled"));
    digitalWrite(ACTUATION_PIN, HIGH);
  } 
  
  else 
  {
    Serial.println(F("Actuator Disabled"));
    digitalWrite(ACTUATION_PIN, LOW);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(ACTUATION_PIN, OUTPUT);

  blePeripheral.setLocalName("Actuator Node BLE400");
  blePeripheral.setAdvertisedServiceUuid(ledService.uuid());

  blePeripheral.addAttribute(ledService);
  blePeripheral.addAttribute(switchCharacteristic);

  blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  switchCharacteristic.setEventHandler(BLEWritten, switchCharacteristicWritten);

  blePeripheral.begin();

  Serial.println(F("Actuator Node BLE400"));
}

void loop() {
  blePeripheral.poll();
}

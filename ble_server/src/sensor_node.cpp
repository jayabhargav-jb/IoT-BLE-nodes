#include <Arduino.h>
#include <SPI.h>
#include <BLEPeripheral.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 3
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);    

BLEPeripheral blePeripheral = BLEPeripheral();

BLEService tempService = BLEService("CCC0");
BLECharCharacteristic tempCharacteristic = BLECharCharacteristic("CCC1", BLERead | BLENotify);
BLEDescriptor tempDescriptor = BLEDescriptor("CCC2", "Temp Celsius");

// BLEService humidityService = BLEService("DDD0");
BLECharCharacteristic humidityCharacteristic = BLECharCharacteristic("DDD1", BLERead | BLENotify);
BLEDescriptor humidityDescriptor = BLEDescriptor("DDD2", "Humidity Percent");

// int temp_reading = 10;
// int humidity_reading = 30;


void setup() 
{
    Serial.begin(9600);
    
    // pinMode(22, OUTPUT); // CODE TIMING
    // pinMode(26, OUTPUT);

    blePeripheral.setLocalName("Sensor Node BLE400");

    blePeripheral.setAdvertisedServiceUuid(tempService.uuid());
    blePeripheral.addAttribute(tempService);
    blePeripheral.addAttribute(tempCharacteristic);
    blePeripheral.addAttribute(tempDescriptor);

    // blePeripheral.setAdvertisedServiceUuid(humidityService.uuid());
    // blePeripheral.addAttribute(humidityService);
    blePeripheral.addAttribute(humidityCharacteristic);
    blePeripheral.addAttribute(humidityDescriptor);
    
    blePeripheral.begin();
    dht.begin();

    Serial.println(F("BLE400 sensor node"));
    // Serial.print(F("temp uuid: ")); Serial.println(tempService.uuid());
    Serial.print(F("service uuid: ")); Serial.println(tempService.uuid());
    // Serial.print(F("humidity uuid: ")); Serial.println(humidityService.uuid());
    
}


void loop() 
{

    // testing purposes, WIP
    BLECentral central = blePeripheral.central();
    // Serial.print(F("address: ")); Serial.println(central.address()); 
    if (central.connected())
    {
        // digitalWrite(22, HIGH);   
        Serial.print(F("address: ")); Serial.println(central.address()); 
        // temp_reading++;
        // humidity_reading = humidity_reading + 1;

        // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
        uint8_t humidity_reading = (uint8_t)    dht.readHumidity();
        // Read temperature as Celsius (the default)
        uint8_t temp_reading = dht.readTemperature();
        // short data = {humidity_reading_str, temp_reading_str};
        // char *data_send = 
        
        Serial.print(F("temperature: ")); Serial.println(temp_reading);
        Serial.print(F("humidity: ")); Serial.println(humidity_reading);

        humidityCharacteristic.setValue(humidity_reading);
        tempCharacteristic.setValue(temp_reading);

        Serial.println();
        delay(1000);
        // digitalWrite(22, LOW);
    }

    else
    {
        // digitalWrite(26, HIGH);
        Serial.println(F("Advertising but not connected."));
        // Serial.print(F("address: ")); Serial.print(central.address()); Serial.print(F(" connection status: ")); Serial.println(central.connected());   
        // Serial.print(F("uuid: ")); Serial.println(tempService.uuid());
        delay(1000);
        // digitalWrite(26, LOW);
    }

}
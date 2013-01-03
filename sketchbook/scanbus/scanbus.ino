/*
 * 1-wire Thermometer bus scanner
 * (C) 2012 Andrew Bythell <abythell@ieee.org>
 *
 * Scans the 1-wire bus and displays the addresses
 * of all 1-wire thermometers found.  The addresses can then be entered
 * into the DSC sketch.
 *
 * This is a stripped-down version of the "Multiple" example supplied with the
 * DallasTemperature library.
 */

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4 /* the io pin connected to the 1-wire data line */

OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses
DeviceAddress inlet, outlet, roof, tank;

void setup(void)
{
  Serial.begin(9600);
  sensors.begin();

  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println("devices.");

  DeviceAddress thermometer;
  for (int i=0; i< sensors.getDeviceCount(); i++) {
    sensors.getAddress(thermometer,i);
    printData(thermometer);
  }
  
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Temp F: ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress)
{
  Serial.print("Device Address: ");
  printAddress(deviceAddress);
  Serial.print(" ");
  printTemperature(deviceAddress);
  Serial.println();
}

void loop(void)
{ 
  sensors.requestTemperatures();
  delay(100000);
}


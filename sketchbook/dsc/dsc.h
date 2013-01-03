/*
  Differential Solar Controler Header
  Copyright 2009-2012 Andrew Bythell <abythell@ieee.org>
 
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DSC_H
#define DSC_H

/*
 * Modify the settings below to match your hardware.  Temperatures in degrees Celsuis.
 */

#define HIGHTEMP_LIMIT 	90	/* shut off when this temperature is reached */
#define OVERTEMP_LIMIT  85	/* restart after an over-temperature condition at this temperature */
#define DELTA_ON	10	/* collectors must be this much warmer than the tank before turning on */
#define DELTA_OFF	2	/* collectors must be this much warmer than the tank before turning off */

/* Define the addresses of the temperature sensors below.  The 'Multiple'
 * example sketch in the DallasTemperature 
 * library can be used to discover devices and addresses on the 1-Wire bus.
 */
DeviceAddress roofSensor = {0x10, 0x1f, 0x4d, 0x83, 0x02, 0x08, 0x00, 0xE9};
DeviceAddress tankSensor = {0x10, 0x25, 0x39, 0x57, 0x00, 0x08, 0x00, 0xf9};
DeviceAddress inletSensor = {0x10, 0xb2, 0x58, 0x83, 0x02, 0x08, 0x00, 0x51};
DeviceAddress outletSensor = {0x10, 0x76, 0xb4, 0xd2, 0x01, 0x08, 0x00, 0x5a};

/*
 * If using the dsc-1 PCB, you won't need to modify anything below this line.
 */

/* Pin Definitions */
static const int WATER_SENSOR = 2;	/* external interrupt 0 */
static const int ONE_WIRE_PIN = 4;
static const int PUMP_RELAY = 8;	
static const int POWER_LED = 9;
static const int PUMP_LED = 10;
static const int ALARM_PIN = 11;	/* timer2 PWM for alarm */
static const int OVERTEMP_LED = 12;
static const int WATER_LED = 13;

/* Hardware Macros */
#define PUMP_ON		digitalWrite(PUMP_LED, LOW); digitalWrite(PUMP_RELAY, HIGH)	/* turn pump and LED on */
#define PUMP_OFF	digitalWrite(PUMP_LED, HIGH); digitalWrite(PUMP_RELAY, LOW)	/* turn pump and LED off */
#define POWER_ON	digitalWrite(POWER_LED, LOW)
#define OVERTEMP_ON	digitalWrite(OVERTEMP_LED, LOW)
#define OVERTEMP_OFF	digitalWrite(OVERTEMP_LED, HIGH)
#define WATER_ON	digitalWrite(WATER_LED, LOW)
#define WATER_OFF	digitalWrite(WATER_LED, HIGH)
#define ALARM_ON	analogWrite(ALARM_PIN, 128)
#define ALARM_OFF	analogWrite(ALARM_PIN, 0)
#define WATER_LOW	false /* digitalRead(WATER_SENSOR) == HIGH ? false : true*/
#define FAULT_ON
#define FAULT_OFF	

/* Globals */
typedef enum {
	STOP_STATE,
	RUN_STATE,
	OVERTEMP_STATE,
	LOWWATER_STATE
} t_state;
static t_state state=STOP_STATE;	
OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature sensors(&oneWire);

#endif /* DSC__H  */

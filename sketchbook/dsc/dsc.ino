/*
  Differential Solar Controller for Arduino 1.0
  Copyright 2009-2012 Andrew Bythell <abythell@ieee.org>

  This sketch depends on the Dallas Temperature Control Library (milesburton.com/Dallas_Temperature_Control_Library)
  version 3.7.2, and the OneWire 2 Library (pjrc.com/teensy/td_libs_OneWire.html) version 2.1.

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

#include <OneWire.h>
#include <DallasTemperature.h>
#include "dsc.h"

/*
 * Setup
 */
void setup(void) 
{
  	pinMode(PUMP_LED, OUTPUT);
        pinMode(PUMP_RELAY, OUTPUT);
	pinMode(POWER_LED, OUTPUT);
	pinMode(OVERTEMP_LED, OUTPUT);
	pinMode(WATER_LED, OUTPUT);
	pinMode(ALARM_PIN, OUTPUT);
	pinMode(WATER_SENSOR, INPUT);

	Serial.begin(9600);
        sensors.begin();
	POWER_ON;
        PUMP_OFF;
	OVERTEMP_OFF;
	ALARM_OFF;
	FAULT_OFF;
	WATER_OFF;
}

/*
 * Check for Low Water Level Condition
 */
void CheckWaterLevel()
{
	if (WATER_LOW)
	{
                PUMP_OFF;
		WATER_ON;
		state = LOWWATER_STATE; 
	}
}

/*
 * Stop State
 */
void StopState()
{
	if (sensors.getTempC(roofSensor) >= sensors.getTempC(tankSensor) + DELTA_ON)
	{
		PUMP_ON;
		state = RUN_STATE;
	}
}

/*
 * Run State
 */
void RunState()
{
	if (sensors.getTempC(tankSensor) >= HIGHTEMP_LIMIT)
	{
		PUMP_OFF;
		OVERTEMP_ON;
		state = OVERTEMP_STATE;
	}
	else if (sensors.getTempC(tankSensor) + DELTA_OFF >= sensors.getTempC(roofSensor))
	{
		PUMP_OFF;
		state = STOP_STATE;  
	}
}

/*
 * Over Temperature State
 */
void OverTempState()
{
	if ((sensors.getTempC(tankSensor) + DELTA_OFF >= sensors.getTempC(roofSensor))
          || (sensors.getTempC(tankSensor) <= OVERTEMP_LIMIT))
	{
		PUMP_OFF;
                OVERTEMP_OFF;
		state = STOP_STATE;  
	}
}

/*
 * Low Water State - Remain in Low Water State until the level is topped up.
 */
void LowWaterState()
{
	if (!WATER_LOW)
	{
		WATER_OFF;
		state = STOP_STATE;
	}
}

/*
 * Print Status to serial port for data logging
 */
void PrintStatus()
{
	uint8_t roofTemp = sensors.getTempC(roofSensor);
	uint8_t tankTemp = sensors.getTempC(tankSensor);
	uint8_t inletTemp = sensors.getTempC(inletSensor);
	uint8_t outletTemp = sensors.getTempC(outletSensor);
	char ascii[10];
	sprintf(ascii, "%02x %02x %02x %02x %02x", state, roofTemp, tankTemp, inletTemp, outletTemp);
	Serial.println(ascii);
}

/*
 * Loop
 */
void loop(void) 
{
    sensors.requestTemperatures();
    
    /*
     * Wait for temperature conversion to complete
     */
    delay(1000);
    
    CheckWaterLevel();
    switch(state)
    {
	case STOP_STATE:
		StopState();
		break;
	case RUN_STATE:
		RunState();
		break;
	case OVERTEMP_STATE:
		OverTempState();
		break;
	case LOWWATER_STATE:
		LowWaterState();
		break;
	default:
                break;
    } 
    PrintStatus();
}

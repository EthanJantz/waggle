/***************************************************************************
  This is a library for the HMC5883 magnentometer/compass

  Designed specifically to work with the Adafruit HMC5883 Breakout
  http://www.adafruit.com/products/1746

  These displays use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution

  ###########################################################################

  Modified Wed Oct 28 15:01:41 CDT 2015:
  Original files obtained from -
  https://github.com/adafruit/HMC5883_Sensor

  Modifications  (WG, RxS) -
  1. Added support for using sensor on both Wire1 and Wire buses.
  2. Default output in gauss and not Tesla.
  3. Changed filenames.

 ***************************************************************************/
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#ifdef __AVR_ATtiny85__
  #include "TinyWireM.h"
  #define Wire TinyWireM
#else
  #include <Wire.h>
#endif

#include <limits.h>

#include "HMC5883L_waggle.h"

static float _hmc5883_Gauss_LSB_XY = 1100.0F;  // Varies with gain
static float _hmc5883_Gauss_LSB_Z  = 980.0F;   // Varies with gain

/***************************************************************************
 MAGNETOMETER
 ***************************************************************************/
/***************************************************************************
 PRIVATE FUNCTIONS
 ***************************************************************************/

/**************************************************************************/
/*!
    @brief  Abstract away platform differences in Arduino wire library
*/
/**************************************************************************/
void HMC5883_Sensor::write8(byte address, byte reg, byte value)
{
  _wire->beginTransmission(address);
  #if ARDUINO >= 100
    _wire->write((uint8_t)reg);
    _wire->write((uint8_t)value);
  #else
    _wire->send(reg);
    _wire->send(value);
  #endif
  _wire->endTransmission();
}

/**************************************************************************/
/*!
    @brief  Abstract away platform differences in Arduino wire library
*/
/**************************************************************************/
byte HMC5883_Sensor::read8(byte address, byte reg)
{
  byte value;

  _wire->beginTransmission(address);
  #if ARDUINO >= 100
    _wire->write((uint8_t)reg);
  #else
    _wire->send(reg);
  #endif
  _wire->endTransmission();
  _wire->requestFrom(address, (byte)1);
  #if ARDUINO >= 100
    value = _wire->read();
  #else
    value = _wire->receive();
  #endif
  _wire->endTransmission();

  return value;
}

/**************************************************************************/
/*!
    @brief  Reads the raw data from the sensor
*/
/**************************************************************************/
void HMC5883_Sensor::read()
{
  // Read the magnetometer
  _wire->beginTransmission((byte)HMC5883_ADDRESS_MAG);
  #if ARDUINO >= 100
    _wire->write(HMC5883_REGISTER_MAG_OUT_X_H_M);
  #else
    _wire->send(HMC5883_REGISTER_MAG_OUT_X_H_M);
  #endif
  _wire->endTransmission();
  _wire->requestFrom((byte)HMC5883_ADDRESS_MAG, (byte)6);


  if (able == false)
  {
    able = true;
    begin();
  }


  if (_wire->available() <= 0)
  {
    able = false;
  }


  // Note high before low (different than accel)
  #if ARDUINO >= 100
    uint8_t xhi = _wire->read();
    uint8_t xlo = _wire->read();
    uint8_t zhi = _wire->read();
    uint8_t zlo = _wire->read();
    uint8_t yhi = _wire->read();
    uint8_t ylo = _wire->read();
  #else
    uint8_t xhi = _wire->receive();
    uint8_t xlo = _wire->receive();
    uint8_t zhi = _wire->receive();
    uint8_t zlo = _wire->receive();
    uint8_t yhi = _wire->receive();
    uint8_t ylo = _wire->receive();
  #endif

  // Shift values to create properly formed integer (low byte first)
  _magData.x = (int16_t)(xlo | ((int16_t)xhi << 8));
  _magData.y = (int16_t)(ylo | ((int16_t)yhi << 8));
  _magData.z = (int16_t)(zlo | ((int16_t)zhi << 8));


  if (able == false)
  {
    _magData.x = 65535;
    _magData.y = 65535;
    _magData.z = 65535;
  }

  // ToDo: Calculate orientation
  _magData.orientation = 0.0;
}

/***************************************************************************
 CONSTRUCTOR
 ***************************************************************************/

/**************************************************************************/
/*!
    @brief  Instantiates a new Adafruit_HMC5883 class
*/
/**************************************************************************/
HMC5883_Sensor::HMC5883_Sensor(int32_t sensorID, TwoWire * wire) {
  _sensorID = sensorID;
  if (wire == NULL) {
    _wire = &Wire;
  } else {
    _wire = wire;
  }
}

HMC5883_Sensor::HMC5883_Sensor(TwoWire * wire) {
  _sensorID = -1;
  if (wire == NULL) {
    _wire = &Wire;
  } else {
    _wire = wire;
  }
}



/***************************************************************************
 PUBLIC FUNCTIONS
 ***************************************************************************/

/**************************************************************************/
/*!
    @brief  Setups the HW
*/
/**************************************************************************/
bool HMC5883_Sensor::begin()
{
  // Enable I2C
  _wire->begin();

  // Enable the magnetometer
  write8(HMC5883_ADDRESS_MAG, HMC5883_REGISTER_MAG_MR_REG_M, 0x00);

  // Set the gain to a known level
  setMagGain(HMC5883_MAGGAIN_1_3);

  return true;
}

/**************************************************************************/
/*!
    @brief  Sets the magnetometer's gain
*/
/**************************************************************************/
void HMC5883_Sensor::setMagGain(hmc5883MagGain gain)
{
  write8(HMC5883_ADDRESS_MAG, HMC5883_REGISTER_MAG_CRB_REG_M, (byte)gain);

  _magGain = gain;

  switch(gain)
  {
    case HMC5883_MAGGAIN_1_3:
      _hmc5883_Gauss_LSB_XY = 1100;
      _hmc5883_Gauss_LSB_Z  = 980;
      break;
    case HMC5883_MAGGAIN_1_9:
      _hmc5883_Gauss_LSB_XY = 855;
      _hmc5883_Gauss_LSB_Z  = 760;
      break;
    case HMC5883_MAGGAIN_2_5:
      _hmc5883_Gauss_LSB_XY = 670;
      _hmc5883_Gauss_LSB_Z  = 600;
      break;
    case HMC5883_MAGGAIN_4_0:
      _hmc5883_Gauss_LSB_XY = 450;
      _hmc5883_Gauss_LSB_Z  = 400;
      break;
    case HMC5883_MAGGAIN_4_7:
      _hmc5883_Gauss_LSB_XY = 400;
      _hmc5883_Gauss_LSB_Z  = 255;
      break;
    case HMC5883_MAGGAIN_5_6:
      _hmc5883_Gauss_LSB_XY = 330;
      _hmc5883_Gauss_LSB_Z  = 295;
      break;
    case HMC5883_MAGGAIN_8_1:
      _hmc5883_Gauss_LSB_XY = 230;
      _hmc5883_Gauss_LSB_Z  = 205;
      break;
  }
}

/**************************************************************************/
/*!
    @brief  Gets the most recent sensor event
*/
/**************************************************************************/
bool HMC5883_Sensor::getEvent(sensors_event_t *event) {
  /* Clear the event */
  memset(event, 0, sizeof(sensors_event_t));

    /* Read new data */
    read();

    event->version   = sizeof(sensors_event_t);
    event->sensor_id = _sensorID;
    event->type      = SENSOR_TYPE_MAGNETIC_FIELD;
    event->timestamp = 0;

    if (able == false)
    {
      event->magnetic.x = 127.44;
      event->magnetic.y = 127.44;
      event->magnetic.z = 127.44;
    }
    else
    {
      event->magnetic.x = _magData.x / _hmc5883_Gauss_LSB_XY;
      event->magnetic.y = _magData.y / _hmc5883_Gauss_LSB_XY;
      event->magnetic.z = _magData.z / _hmc5883_Gauss_LSB_Z;
    }
    
    return true;
}

/**************************************************************************/
/*!
    @brief  Gets the sensor_t data
*/
/**************************************************************************/
void HMC5883_Sensor::getSensor(sensor_t *sensor) {
  /* Clear the sensor_t object */
  memset(sensor, 0, sizeof(sensor_t));

    /* Insert the sensor name in the fixed length char array */
    strncpy (sensor->name, "HMC5883", sizeof(sensor->name) - 1);
    sensor->name[sizeof(sensor->name)- 1] = 0;
    sensor->version     = 1;
    sensor->sensor_id   = _sensorID;
    sensor->type        = SENSOR_TYPE_MAGNETIC_FIELD;
    sensor->min_delay   = 0;
    sensor->max_value   = 800; // 8 gauss == 800 microTesla
    sensor->min_value   = -800; // -8 gauss == -800 microTesla
    sensor->resolution  = 0.2; // 2 milligauss == 0.2 microTesla
}

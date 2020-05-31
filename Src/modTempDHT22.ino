#include <SimpleDHT.h>

SimpleDHT22 dht22;
int tempInputPin;

// *****************************************************
//  Function name:  tempSetInputPin
//  Params:         pinNum - Pin where's the sensor is connected
//  Return value:   None
// *****************************************************
void tempSetInputPin (int pinNum)
{
  tempInputPin = pinNum  ;
}

// *****************************************************
//  Function name:  tempReadData
//  Params:         temperature - Temp value read
//                  humidity - Humidity value read
//  Return value:   Error code (0 if no error)
// *****************************************************
int tempReadData (float &temperature, float &humidity)
{
  temperature = 5;
  int err = SimpleDHTErrSuccess;
  if ((err = dht22.read2 (tempInputPin, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess)
  {
    return err;
  }
  else
  {
    return 0; 
  }
}
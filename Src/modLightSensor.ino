int lightInputPin;

// *****************************************************
//  Function name:  lightSetInputPin
//  Params:         pinNum - Pin where's the sensor is connected
//  Return value:   None
// *****************************************************
void lightSetInputPin (int pinNum)
{
  lightInputPin = pinNum  ;
}

// *****************************************************
//  Function name:  lightReadData
//  Params:         none
//  Return value:   Value read
// *****************************************************
int lightReadData ()
{
  return analogRead (lightInputPin);
}

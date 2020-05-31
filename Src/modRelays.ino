int outputPin;

// *****************************************************
//  Function name:  releSetOuputPin
//  Params:         pinNum - Pin where's the sensor is connected
//  Return value:   None
// *****************************************************
void releSetOutputPin (int pinNum)
{
  outputPin = pinNum  ;
  pinMode (outputPin, OUTPUT);
}

// *****************************************************
//  Function name:  releStateChange
//  Params:         state - Relè state (HIGH/LOW - 1/0) 
//  Return value:   None
// *****************************************************
void releStateChange (int state)
{
  digitalWrite (outputPin, state);
}
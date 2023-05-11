float curveFilter(int input, int releasedValue, int pressedValue, int curvePush, float expFactor)
{
    float Input = input;
    int PressedValue = pressedValue;
    int ReleasedValue = releasedValue;

    if (curvePush == 0)
    {
        return Input;
    }

    if (curvePush < 0)
    {
        if (releasedValue < pressedValue)
        {
            PressedValue = PressedValue - clutchTopDeadzone;
            ReleasedValue = ReleasedValue + clutchBottomDeadzone;
            if (Input < ReleasedValue)
            {
                Input = ReleasedValue;
            }
            if (Input > PressedValue)
            {
                Input = PressedValue;
            }
            Input = (Input - ReleasedValue) * pow((1 + Input - ReleasedValue), expFactor);
        }
        else if (pressedValue < releasedValue)
        {
            PressedValue = PressedValue + clutchTopDeadzone;
            ReleasedValue = ReleasedValue - clutchBottomDeadzone;
            if (Input > ReleasedValue)
            {
                Input = ReleasedValue;
            }
            if (Input < PressedValue)
            {
                Input = PressedValue;
            }
            Input = (ReleasedValue - Input) * pow((1 + ReleasedValue - Input), expFactor);
        }
    }
    else if (curvePush > 0)
    {
        if (ReleasedValue < PressedValue)
        {
            if (Input < ReleasedValue)
            {
                Input = ReleasedValue;
            }
            if (Input > PressedValue)
            {
                Input = PressedValue;
            }
            Input = (PressedValue - Input) * pow((1 + PressedValue - Input), expFactor);
        }
        else if (PressedValue < ReleasedValue)
        {
            if (Input > ReleasedValue)
            {
                Input = ReleasedValue;
            }
            if (Input < PressedValue)
            {
                Input = PressedValue;
            }
            Input = (Input - PressedValue) * pow((1 + Input - PressedValue), expFactor);
        }
    }

    return Input;
}

void checkValue(int pin)
{
  if(pin < 50)
  {
    Serial.print("Pin ");
    Serial.print(String(pin));
    Serial.print(" reading: ");
    Serial.println(analogRead(pin));
  }
  #if (USING_CB1 == 1 || USING_ADS1115 == 1)
  else
  {
    Serial.print("ADC ");
    Serial.print(String(pin+1-ADC_CORR));
    Serial.print(" reading: ");
    Serial.println(ADS1115value[pin-ADC_CORR]);
  }
  #endif
}

void refreshRate()
{
  if(globalCounter == 10)
  {
    Serial.print("Refresh rate: ");
    Serial.print(10000000/(micros()-globalTimer));
    Serial.println(" Hz");
    globalTimer = micros();
    globalCounter = 0;
  }
}

#if (USING_CB1 == 1)
void CB1Oversampling()
{
  if(oversamples.available() >= 512)
  {
    long val1 = 0;
    long val2 = 0;
    long val3 = 0;
    long val4 = 0;
    for(int i = 0; i < 256; i++)
    {
      val1 += oversamples.read();
      val2 += oversamples.read();
      val3 += oversamples.read();
      val4 += oversamples.read();
    }
    ADS1115value[8] = val1 / 32;  
    ADS1115value[9] = val2 / 32;
    ADS1115value[10] = val3 / 32;
    ADS1115value[11] = val4 / 32;
  } 
}
#endif
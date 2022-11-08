uint32_t DeserializeU32(uint8_t* iArray)
{
  uint32_t retVal = 0;
  retVal |= iArray[0]; retVal <<= 8;
  retVal |= iArray[1]; retVal <<= 8;
  retVal |= iArray[2]; retVal <<= 8;
  retVal |= iArray[3];
  return retVal;
}

bool LoadInternalData()
{
  Load_Active_Config();
  Load_Pending_Config();
  return true;
}

bool CheckValueisValid(uint32_t iValue)
{
  if(iValue == 4294967295)
  {
    return false;
  }
  return true;
}

bool CONFIG_Set_CAM_Resolution(uint32_t i_LowRes, uint32_t i_HighRes)
{
  bool mLowRes  = false;
  bool mHighRes = false; 
  if(i_LowRes == OV5642_320x240 || i_LowRes == OV5642_640x480)
  {
    mLowRes = true;
  }

  if(i_HighRes == OV5642_1600x1200 || i_HighRes == OV5642_2592x1944)
  {
    mHighRes = true;
  }

  if(mLowRes == true && mHighRes == true)
  {
    mPending_CAM_LOW_RESOLUTION    = i_LowRes;
    mPending_CAM_HIGH_RESOLUTION   = i_HighRes;
    return true;
  }
  else
  {
    return false;
  }
}

bool CONFIG_Set_CAM_Quality(uint32_t i_NewValue)
{
   if(i_NewValue == high_quality ||  i_NewValue == default_quality || i_NewValue == low_quality)
   {
     mPending_CAM_QUALITY = i_NewValue;
     return true;
   }
   else
   {
    return false;
   }             
}

bool CONFIG_Set_CAM_Brightness(uint32_t i_NewValue)
{
  if(i_NewValue == Brightness4 || i_NewValue == Brightness3 || i_NewValue == Brightness2 || i_NewValue == Brightness1 || i_NewValue == Brightness0 || i_NewValue == Brightness_1 
  || i_NewValue == Brightness_2 || i_NewValue == Brightness_3 || i_NewValue == Brightness_4)
  {
      mPending_CAM_BRIGHTNESS = i_NewValue;
      return true;
  }
  else
  {
    return false;
  }
}

bool CONFIG_Set_CAM_Contrast(uint32_t i_NewValue)
{
  if(i_NewValue == Contrast4 || i_NewValue == Contrast3 || i_NewValue == Contrast2 || i_NewValue == Contrast1 || i_NewValue == Contrast0 || i_NewValue == Contrast_1 
  || i_NewValue == Contrast_2 || i_NewValue == Contrast_3 || i_NewValue == Contrast_4)
  {
    mPending_CAM_CONTRAST = i_NewValue;
    return true;

  }
  else
  {
    return false;
  }           
}

bool CONFIG_Set_CAM_Light(uint32_t i_NewValue)
{
  if(i_NewValue == Advanced_AWB || i_NewValue == Simple_AWB || i_NewValue == Manual_day || i_NewValue == Manual_A || i_NewValue == Manual_cwf || i_NewValue == Manual_cloudy) 
  {
    mPending_CAM_LIGHT = i_NewValue;
    return true;
  }
  else
  {
    return false;
  }           
}

bool CONFIG_Set_CAM_HUE(uint32_t i_NewValue)
{
  if(i_NewValue == degree_180 || i_NewValue == degree_150 || i_NewValue == degree_120 || i_NewValue == degree_90 || i_NewValue == degree_60 || i_NewValue == degree_30
  || i_NewValue == degree_0 || i_NewValue == degree30 || i_NewValue == degree60 || i_NewValue == degree90 || i_NewValue == degree120 || i_NewValue == degree150) 
  {
    mPending_CAM_HUE = i_NewValue;
    return true;
  }
  else
  {
    return false;
  }           
}

bool CONFIG_Set_CAM_SATURATION(uint32_t i_NewValue)
{
  if(i_NewValue == Saturation4 || i_NewValue == Saturation3 || i_NewValue == Saturation2 || i_NewValue == Saturation1 || i_NewValue == Saturation0 || i_NewValue == Saturation_1
  || i_NewValue == Saturation_2 || i_NewValue == Saturation_3 || i_NewValue == Saturation_4) 
  {
    mPending_CAM_SATURATION = i_NewValue;
    return true;
  }
  else
  {
    return false;
  }           
}

bool CONFIG_Set_CAM_SPECIAL(uint32_t i_NewValue)
{
  if(i_NewValue == Antique || i_NewValue == Bluish || i_NewValue == Greenish || i_NewValue == Reddish || i_NewValue == BW || i_NewValue == Negative
  || i_NewValue == BWnegative || i_NewValue == Normal || i_NewValue == Sepia || i_NewValue == Overexposure || i_NewValue == Solarize || i_NewValue == Blueish || i_NewValue == Yellowish) 
  {
    mPending_CAM_SPECIAL = i_NewValue;
    return true;
  }
  else
  {
    return false;
  }           
}

bool CONFIG_Set_CAM_EXPOSURE(uint32_t i_NewValue)
{
  if(i_NewValue == Exposure_17_EV || i_NewValue == Exposure_13_EV || i_NewValue == Exposure_10_EV || i_NewValue == Exposure_07_EV || i_NewValue == Exposure_03_EV || i_NewValue == Exposure_default
  || i_NewValue == Exposure03_EV || i_NewValue == Exposure07_EV || i_NewValue == Exposure10_EV || i_NewValue == Exposure13_EV || i_NewValue == Exposure17_EV) 
  {
    mPending_CAM_EXPOSURE = i_NewValue;
    return true;
  }
  else
  {
    return false;
  }           
}

bool CONFIG_Set_CAM_SHARPNESS(uint32_t i_NewValue)
{
  if(i_NewValue == Auto_Sharpness_default || i_NewValue == Auto_Sharpness1 || i_NewValue == Auto_Sharpness2 || i_NewValue == Manual_Sharpnessoff || i_NewValue == Manual_Sharpness1 || i_NewValue == Manual_Sharpness2
  || i_NewValue == Manual_Sharpness3 || i_NewValue == Manual_Sharpness4 || i_NewValue == Manual_Sharpness5) 
  {
    mPending_CAM_SHARPNESS = i_NewValue;
    return true;
  }
  else
  {
    return false;
  }           
}

bool CONFIG_Set_Temperature_Timer(uint32_t i_NewValue)
{
  if(i_NewValue >= 500)
  {
    mPending_TEMPERATURE_TIMER = i_NewValue;  
    return true;

  }
  else
  {
    return false;
  }
}

bool CONFIG_Set_Temperature_Scheduler(uint32_t i_NewValue)
{
  if(i_NewValue == 0 || i_NewValue == 1)
  {
    mPending_TEMPERATURE_SCHEDULER = i_NewValue;
    return true;
  }
  else
  {
    return false;
  }
}

bool Load_Active_Config()
{
  uint16_t mLenData = 200;
  uint8_t mInternalData[mLenData] = {0};
  uint8_t mAddress[4] = {0};
  
  mAddress[0] = ACTIVE_CONFIG_ADDRESS >> 24 & 0xFF;
  mAddress[1] = ACTIVE_CONFIG_ADDRESS >> 16 & 0xFF;
  mAddress[2] = ACTIVE_CONFIG_ADDRESS >>  8 & 0xFF;
  mAddress[3] = ACTIVE_CONFIG_ADDRESS >>  0 & 0xFF;
  digitalWrite(NOR_CS, HIGH);
  digitalWrite(NOR_CS, LOW);
  NOR_SPI(NOR_4_BYTE_READ);
  NOR_SPI(mAddress[0]);
  NOR_SPI(mAddress[1]);
  NOR_SPI(mAddress[2]);
  NOR_SPI(mAddress[3]);
  for (uint16_t i = 0; i < mLenData; i++)
  {
    mInternalData[i] = NOR_SPI(NOR_DUMMY_BYTE);
  }
  digitalWrite(NOR_CS, HIGH);    

  uint8_t mArrayData[4] = {0};
  uint8_t mIndex = 0;
  
  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  uint32_t mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mActive_CAM_LOW_RESOLUTION = DEFAULT_CAMERA_LOW_RESOLUTION;
  }
  else
  {
    mActive_CAM_LOW_RESOLUTION = DeserializeU32(mArrayData);    
  }

  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mActive_CAM_HIGH_RESOLUTION = DEFAULT_CAMERA_HIGH_RESOLUTION;
  }
  else
  {
    mActive_CAM_HIGH_RESOLUTION = DeserializeU32(mArrayData);    
  }

  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mActive_CAM_QUALITY = DEFAULT_CAMERA_QUALITY;
  }
  else
  {
    mActive_CAM_QUALITY = DeserializeU32(mArrayData);    
  }

  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mActive_CAM_BRIGHTNESS = DEFAULT_CAMERA_BRIGHTNESS;
  }
  else
  {
    mActive_CAM_BRIGHTNESS = DeserializeU32(mArrayData);    
  }

  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mActive_CAM_CONTRAST = DEFAULT_CAMERA_CONTRAST;
  }
  else
  {
    mActive_CAM_CONTRAST = DeserializeU32(mArrayData);    
  }

  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mActive_TEMPERATURE_TIMER = DEFAULT_TEMPERATURE_TIMER_COUNTER;
  }
  else
  {
     mActive_TEMPERATURE_TIMER = DeserializeU32(mArrayData);    
  }

  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mActive_TEMPERATURE_SCHEDULER = DEFAULT_TEMPERATURE_SCHEDULER;
  }
  else
  {
    mActive_TEMPERATURE_SCHEDULER = DeserializeU32(mArrayData);    
  }

  
  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mActive_CAM_LIGHT = DEFAULT_CAM_LIGHT;
  }
  else
  {
    mActive_CAM_LIGHT = DeserializeU32(mArrayData);    
  }

  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mActive_CAM_HUE = DEFAULT_CAM_HUE;
  }
  else
  {
    mActive_CAM_HUE = DeserializeU32(mArrayData);    
  }

  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mActive_CAM_SATURATION = DEFAULT_CAM_SATURATION;
  }
  else
  {
    mActive_CAM_SATURATION = DeserializeU32(mArrayData);    
  }

  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mActive_CAM_SPECIAL = DEFAULT_CAM_SPECIAL;
  }
  else
  {
    mActive_CAM_SPECIAL = DeserializeU32(mArrayData);    
  }
         
  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mActive_CAM_EXPOSURE = DEFAULT_CAM_EXPOSURE;
  }
  else
  {
    mActive_CAM_EXPOSURE = DeserializeU32(mArrayData);    
  }
           
  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mActive_CAM_SHARPNESS = DEFAULT_CAM_SHARPNESS;
  }
  else
  {
    mActive_CAM_SHARPNESS = DeserializeU32(mArrayData);    
  }

  return true;
}

bool Load_Pending_Config()
{
  uint16_t mLenData = 200;
  uint8_t mInternalData[mLenData] = {0};
  uint8_t mAddress[4] = {0};
  
  mAddress[0] = PENDING_CONFIG_ADDRESS >> 24 & 0xFF;
  mAddress[1] = PENDING_CONFIG_ADDRESS >> 16 & 0xFF;
  mAddress[2] = PENDING_CONFIG_ADDRESS >>  8 & 0xFF;
  mAddress[3] = PENDING_CONFIG_ADDRESS >>  0 & 0xFF;
  digitalWrite(NOR_CS, HIGH);
  digitalWrite(NOR_CS, LOW);
  NOR_SPI(NOR_4_BYTE_READ);
  NOR_SPI(mAddress[0]);
  NOR_SPI(mAddress[1]);
  NOR_SPI(mAddress[2]);
  NOR_SPI(mAddress[3]);
  for (uint16_t i = 0; i < mLenData; i++)
  {
    mInternalData[i] = NOR_SPI(NOR_DUMMY_BYTE);
  }
  digitalWrite(NOR_CS, HIGH);    

  uint8_t mArrayData[4] = {0};
  uint8_t mIndex = 0;
  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  uint32_t mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mPending_CAM_LOW_RESOLUTION = DEFAULT_CAMERA_LOW_RESOLUTION;
  }
  else
  {
    mPending_CAM_LOW_RESOLUTION = DeserializeU32(mArrayData);    
  }

  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mPending_CAM_HIGH_RESOLUTION = DEFAULT_CAMERA_HIGH_RESOLUTION;
  }
  else
  {
    mPending_CAM_HIGH_RESOLUTION = DeserializeU32(mArrayData);    
  }

  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mPending_CAM_QUALITY = DEFAULT_CAMERA_QUALITY;
  }
  else
  {
    mPending_CAM_QUALITY = DeserializeU32(mArrayData);    
  }

  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mPending_CAM_BRIGHTNESS = DEFAULT_CAMERA_BRIGHTNESS;
  }
  else
  {
    mPending_CAM_BRIGHTNESS = DeserializeU32(mArrayData);    
  }

  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mPending_CAM_CONTRAST = DEFAULT_CAMERA_CONTRAST;
  }
  else
  {
    mPending_CAM_CONTRAST = DeserializeU32(mArrayData);    
  }

  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mPending_TEMPERATURE_TIMER = DEFAULT_TEMPERATURE_TIMER_COUNTER;
  }
  else
  {
    mPending_TEMPERATURE_TIMER = DeserializeU32(mArrayData);    
  }

  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mPending_TEMPERATURE_SCHEDULER = DEFAULT_TEMPERATURE_SCHEDULER;
  }
  else
  {
    mPending_TEMPERATURE_SCHEDULER = DeserializeU32(mArrayData);    
  }

  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mPending_CAM_LIGHT = DEFAULT_CAM_LIGHT;
  }
  else
  {
    mPending_CAM_LIGHT = DeserializeU32(mArrayData);    
  }

  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mPending_CAM_HUE = DEFAULT_CAM_HUE;
  }
  else
  {
    mPending_CAM_HUE = DeserializeU32(mArrayData);    
  }

  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mPending_CAM_SATURATION = DEFAULT_CAM_SATURATION;
  }
  else
  {
    mPending_CAM_SATURATION = DeserializeU32(mArrayData);    
  }

  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mPending_CAM_SPECIAL = DEFAULT_CAM_SPECIAL;
  }
  else
  {
    mPending_CAM_SPECIAL = DeserializeU32(mArrayData);    
  }
         
  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mPending_CAM_EXPOSURE = DEFAULT_CAM_EXPOSURE;
  }
  else
  {
    mPending_CAM_EXPOSURE = DeserializeU32(mArrayData);    
  }
           
  memcpy(&mArrayData[0], &mInternalData[mIndex], 4U);
  mValue = DeserializeU32(mArrayData); 
  mIndex += 4U;
  if(CheckValueisValid(mValue) == false)
  {
    mPending_CAM_SHARPNESS = DEFAULT_CAM_SHARPNESS;
  }
  else
  {
    mPending_CAM_SHARPNESS = DeserializeU32(mArrayData);    
  }

  return true;
}


bool CONFIG_RELOAD_CONFIGURATION()
{
  mActive_CAM_LOW_RESOLUTION    = mPending_CAM_LOW_RESOLUTION;
  mActive_CAM_HIGH_RESOLUTION   = mPending_CAM_HIGH_RESOLUTION;
  mActive_CAM_QUALITY           = mPending_CAM_QUALITY;
  mActive_CAM_BRIGHTNESS        = mPending_CAM_BRIGHTNESS;
  mActive_CAM_CONTRAST          = mPending_CAM_CONTRAST;
  mActive_TEMPERATURE_TIMER     = mPending_TEMPERATURE_TIMER;
  mActive_TEMPERATURE_SCHEDULER = mPending_TEMPERATURE_SCHEDULER;
  mActive_CAM_LIGHT             = mPending_CAM_LIGHT;            
  mActive_CAM_HUE               = mPending_CAM_HUE;              
  mActive_CAM_SATURATION        = mPending_CAM_SATURATION;       
  mActive_CAM_SPECIAL           = mPending_CAM_SPECIAL;          
  mActive_CAM_EXPOSURE          = mPending_CAM_EXPOSURE;         
  mActive_CAM_SHARPNESS         = mPending_CAM_SHARPNESS;        

  if(CONFIG_FLUSH_PENDING_VALUE() == true)
  {
    if(CONFIG_FLUSH_ACTIVE_VALUE() == true)
    {
        return true;          
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}

bool CONFIG_Read_All_Configuration()
{
  uint16_t mLenData = 200;
  uint8_t mInternalData[mLenData] = {0};
  uint8_t mAddress[4] = {0};
  
  mAddress[0] = ACTIVE_CONFIG_ADDRESS >> 24 & 0xFF;
  mAddress[1] = ACTIVE_CONFIG_ADDRESS >> 16 & 0xFF;
  mAddress[2] = ACTIVE_CONFIG_ADDRESS >>  8 & 0xFF;
  mAddress[3] = ACTIVE_CONFIG_ADDRESS >>  0 & 0xFF;
  digitalWrite(NOR_CS, HIGH);
  digitalWrite(NOR_CS, LOW);
  NOR_SPI(NOR_4_BYTE_READ);
  NOR_SPI(mAddress[0]);
  NOR_SPI(mAddress[1]);
  NOR_SPI(mAddress[2]);
  NOR_SPI(mAddress[3]);
  for (uint16_t i = 0; i < mLenData; i++)
  {
    mInternalData[i] = NOR_SPI(NOR_DUMMY_BYTE);
  }
  digitalWrite(NOR_CS, HIGH);
  uint8_t mACKData = true;
  SendALLOverI2C(mInternalData,200, mACKData, mRecvData[1], mRecvData[2]);       

  return true;
}

bool setAllConfigToDefault()
{
  bool mReturn = false;
  
  mPending_CAM_LOW_RESOLUTION    = DEFAULT_CAMERA_LOW_RESOLUTION;
  mPending_CAM_HIGH_RESOLUTION   = DEFAULT_CAMERA_HIGH_RESOLUTION;
  mPending_CAM_QUALITY           = DEFAULT_CAMERA_QUALITY;
  mPending_CAM_BRIGHTNESS        = DEFAULT_CAMERA_BRIGHTNESS;
  mPending_CAM_CONTRAST          = DEFAULT_CAMERA_CONTRAST;
  mPending_CAM_LIGHT             = DEFAULT_CAM_LIGHT;
  mPending_CAM_HUE               = DEFAULT_CAM_HUE;
  mPending_CAM_SATURATION        = DEFAULT_CAM_SATURATION;
  mPending_CAM_SPECIAL           = DEFAULT_CAM_SPECIAL;
  mPending_CAM_EXPOSURE          = DEFAULT_CAM_EXPOSURE;
  mPending_CAM_SHARPNESS         = DEFAULT_CAM_SHARPNESS;
  mPending_TEMPERATURE_TIMER     = DEFAULT_TEMPERATURE_TIMER_COUNTER;
  mPending_TEMPERATURE_SCHEDULER = DEFAULT_TEMPERATURE_SCHEDULER;

  mActive_CAM_LOW_RESOLUTION    = DEFAULT_CAMERA_LOW_RESOLUTION;
  mActive_CAM_HIGH_RESOLUTION   = DEFAULT_CAMERA_HIGH_RESOLUTION;
  mActive_CAM_QUALITY           = DEFAULT_CAMERA_QUALITY;
  mActive_CAM_BRIGHTNESS        = DEFAULT_CAMERA_BRIGHTNESS;
  mActive_CAM_CONTRAST          = DEFAULT_CAMERA_CONTRAST;
  mActive_CAM_LIGHT             = DEFAULT_CAM_LIGHT;
  mActive_CAM_HUE               = DEFAULT_CAM_HUE;
  mActive_CAM_SATURATION        = DEFAULT_CAM_SATURATION;
  mActive_CAM_SPECIAL           = DEFAULT_CAM_SPECIAL;
  mActive_CAM_EXPOSURE          = DEFAULT_CAM_EXPOSURE;
  mActive_CAM_SHARPNESS         = DEFAULT_CAM_SHARPNESS;
  mActive_TEMPERATURE_TIMER     = DEFAULT_TEMPERATURE_TIMER_COUNTER;
  mActive_TEMPERATURE_SCHEDULER = DEFAULT_TEMPERATURE_SCHEDULER;
  
  bool mPendingFlush = CONFIG_FLUSH_PENDING_VALUE();
  bool mActiveFlush  = CONFIG_FLUSH_ACTIVE_VALUE();

  if(mPendingFlush == true && mActiveFlush == true)
  {
    mReturn = true;
  }

  return mReturn; 
}

bool CONFIG_FLUSH_PENDING_VALUE()
{
  uint16_t mCounter = 0;
  uint8_t mData[200] = {0};
  uint8_t mConfigDataArray[4] = {0};

  uint32_t mConfigData = mPending_CAM_LOW_RESOLUTION;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

  mConfigData = mPending_CAM_HIGH_RESOLUTION;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

  mConfigData = mPending_CAM_QUALITY;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;
  
  mConfigData = mPending_CAM_BRIGHTNESS;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

  mConfigData = mPending_CAM_CONTRAST;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

  mConfigData = mPending_TEMPERATURE_TIMER;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

  mConfigData = mPending_TEMPERATURE_SCHEDULER;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

  mConfigData = mPending_CAM_LIGHT;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

  mConfigData = mPending_CAM_HUE;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

  mConfigData = mPending_CAM_SATURATION;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

  mConfigData = mPending_CAM_SPECIAL;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

  mConfigData = mPending_CAM_EXPOSURE;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

  mConfigData = mPending_CAM_SHARPNESS;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

  if(CONFIG_Erase_Pending_Sector() == true)
  {
    NOR_WriteSector(PENDING_CONFIG_ADDRESS, mData, 200);
    return true;    
  }
  else
  {
    return false;
  }
}

bool CONFIG_Erase_Pending_Sector()
{
  bool mRetData = 0;
  for (uint8_t i = 0; i < 50; i++)
  {
    NOR_WriteEnable();
    mRetData = NOR_GetStatus();
    if (mRetData == 2)
    {
      break;
    }
  }
  NOR_EraseSector(PENDING_CONFIG_ADDRESS);
  delay(10);
  NOR_WriteDisable();

  return true;
}

bool CONFIG_FLUSH_ACTIVE_VALUE()
{
  uint16_t mCounter = 0;
  uint8_t mData[200] = {0};
  uint8_t mConfigDataArray[4] = {0};

  uint32_t mConfigData = mActive_CAM_LOW_RESOLUTION;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

  mConfigData = mActive_CAM_HIGH_RESOLUTION;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

  mConfigData = mActive_CAM_QUALITY;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;
  
  mConfigData = mActive_CAM_BRIGHTNESS;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

  mConfigData = mActive_CAM_CONTRAST;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

  mConfigData = mActive_TEMPERATURE_TIMER;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

  mConfigData = mActive_TEMPERATURE_SCHEDULER;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

    mConfigData = mActive_CAM_LIGHT;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

  mConfigData = mActive_CAM_HUE;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

  mConfigData = mActive_CAM_SATURATION;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

  mConfigData = mActive_CAM_SPECIAL;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

  mConfigData = mActive_CAM_EXPOSURE;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

  mConfigData = mActive_CAM_SHARPNESS;
  mConfigDataArray[0] = mConfigData >> 24 & 0xFF;
  mConfigDataArray[1] = mConfigData >> 16 & 0xFF;
  mConfigDataArray[2] = mConfigData >>  8 & 0xFF;
  mConfigDataArray[3] = mConfigData >>  0 & 0xFF;
  memcpy(&mData[mCounter], &mConfigDataArray[0], 4U);
  mCounter += 4;

  if(CONFIG_Erase_Active_Sector() == true)
  {
    NOR_WriteSector(ACTIVE_CONFIG_ADDRESS, mData, 200);    
    return true;
  }
  else
  {
    return false;
  }  
}

bool CONFIG_Erase_Active_Sector()
{
  bool mRetData = 0;
  for (uint8_t i = 0; i < 50; i++)
  {
    NOR_WriteEnable();
    mRetData = NOR_GetStatus();
    if (mRetData == 2)
    {
      break;
    }
  }
  NOR_EraseSector(ACTIVE_CONFIG_ADDRESS);
  delay(10);
  NOR_WriteDisable();

  return true;
}

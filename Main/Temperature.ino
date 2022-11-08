uint32_t mTemperatureBuffer[200] = {0};
uint8_t TemptoNOR[256];

void GetTemperatureService(bool isDirect)
{
  analogReadResolution(12);
  bool mWriteToBuffer = false;
  uint32_t mTemperatureValue = 0;
  uint32_t mADCValue = analogRead(A3);
  double mVoltageValue = (mADCValue * 3.3)/4095;
  double mCelciusValue = ((mVoltageValue * 1000) / 10) - 273;
  mCelciusValue *= 100;
  mTemperatureValue = (uint32_t)mCelciusValue;  

  uint8_t mTemperatureArray[4] = {0};
  mTemperatureArray[0] = mTemperatureValue >> 24 & 0xFF;
  mTemperatureArray[1] = mTemperatureValue >> 16 & 0xFF;
  mTemperatureArray[2] = mTemperatureValue >>  8 & 0xFF;
  mTemperatureArray[3] = mTemperatureValue >>  0 & 0xFF;
  
  if(isDirect == false)
  {
    mTemperatureBuffer[mTemperatureIndex] = mTemperatureValue;
    uint16_t mIndex = 4*mTemperatureIndex;
    NOR_WriteSector((TEMPERATURE_SECTOR_ADDRESS+mIndex), mTemperatureArray, 4);
    mTemperatureIndex++;    
  }
  else
  {
      uint8_t mACKData = true;
      SendALLOverI2C(mTemperatureArray, 4, mACKData, mRecvData[1], mRecvData[2]);    
  }
}

bool EraseTemperatureDataSector()
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
  NOR_EraseSector(TEMPERATURE_SECTOR_ADDRESS);
  delay(10);
  NOR_WriteDisable();
  return true;
}

void ReadTemperatureSector(bool isSendDirect)
{
  uint16_t mLenData = 800;
  uint32_t iAddress = TEMPERATURE_SECTOR_ADDRESS;
  uint8_t mTemperatureData[mLenData] = {0};
  uint8_t mAddress[4];
  
  mAddress[0] = iAddress >> 24 & 0xFF;
  mAddress[1] = iAddress >> 16 & 0xFF;
  mAddress[2] = iAddress >>  8 & 0xFF;
  mAddress[3] = iAddress >>  0 & 0xFF;
  digitalWrite(NOR_CS, HIGH);
  digitalWrite(NOR_CS, LOW);
  NOR_SPI(NOR_4_BYTE_READ);
  NOR_SPI(mAddress[0]);
  NOR_SPI(mAddress[1]);
  NOR_SPI(mAddress[2]);
  NOR_SPI(mAddress[3]);
  for (uint16_t i = 0; i < mLenData; i++)
  {
    mTemperatureData[i] = NOR_SPI(NOR_DUMMY_BYTE);
  }  

  if(isSendDirect == true)
  {
    uint8_t mACKData = true;
    SendALLOverI2C(mTemperatureData, 800,mACKData, mRecvData[1], mRecvData[2]);    
  }
  digitalWrite(NOR_CS, HIGH);  
}

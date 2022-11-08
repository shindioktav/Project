bool is4ByteAddressingEnabled = false;
bool mBlockingStatus = false;
uint8_t mFlashAddressByte = 0;

void NOR_Init()
{
  NOR_GetID();
  NOR_WriteDisable();
  NOR_GetStatus();
  NOR_WriteEnable();
  NOR_GetStatus();
  delay(500);
}

void NOR_GetID()
{
  uint32_t mReturnData  = 0;
  uint8_t returnData[3] = {0};
  digitalWrite(NOR_CS, HIGH);
  digitalWrite(NOR_CS, LOW);
  
  digitalWrite(selector, LOW);

  NOR_SPI(NOR_READ_ID_COMMAND);
  returnData[0] = NOR_SPI(NOR_DUMMY_BYTE);
  returnData[1] = NOR_SPI(NOR_DUMMY_BYTE);
  returnData[2] = NOR_SPI(NOR_DUMMY_BYTE);

  mReturnData |= returnData[0]; mReturnData <<= 8;
  mReturnData |= returnData[1]; mReturnData <<= 8;
  mReturnData |= returnData[2];
  if ((mReturnData & MEM_TYPE_MASK) != MEM_TYPE_MICRON)
  {
    while(1)
    {
      Serial.println("EEPR ERROR !!!");
      NOR_Init();      
    }
  }
  else
  {
    digitalWrite(NOR_CS, HIGH);
  }
}

uint8_t NOR_GetStatus()
{
  uint8_t mData;
  digitalWrite(NOR_CS, HIGH);
  digitalWrite(NOR_CS, LOW);
  NOR_SPI(NOR_READ_STATUS_REGISTER);
  mData = NOR_SPI(NOR_DUMMY_BYTE);
  digitalWrite(NOR_CS, HIGH);
  return mData;
}

void NOR_WriteDisable()
{
  digitalWrite(NOR_CS, HIGH);
  digitalWrite(NOR_CS, LOW);
  NOR_SPI(NOR_WRITE_DISABLE_COMMAND);
  digitalWrite(NOR_CS, HIGH);
  delay(5);
}

void NOR_WriteEnable()
{
  digitalWrite(NOR_CS, HIGH);
  digitalWrite(NOR_CS, LOW);
  NOR_SPI(NOR_WRITE_ENABLE_COMMAND);
  digitalWrite(NOR_CS, HIGH);
  delay(5);
}

bool NOR_TestConnection()
{
  /*
    Erase one sector address TEST_CONNECTION_ADDRESS
    write 4 byte in 4 first address of sector
    read 4 byte thats has been written
  */
  uint8_t mTestData[4] = {0x37, 0x37, 0x37, 0x37};
  bool ret = NOR_EraseSector(TEST_CONNECTION_ADDRESS);
  if(ret)
  {
    ret = NOR_WriteSector(TEST_CONNECTION_ADDRESS, mTestData, 4U);
    if (ret)
    {
      if (NOR_ReadByAddress(TEST_CONNECTION_ADDRESS, 4U) == mTestData)
      {
        return true;
      }
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

  return NOR_TestConnection;
}

bool NOR_EraseAllImageByImageStatus(uint8_t i_Resolution)
{
  uint16_t mLenData = NUM_OF_IMAGE;
  uint32_t iAddress = 0x00000000;
  uint8_t mImageStatusData[mLenData] = {0};
  uint8_t mAddress[4] = {0};
  uint32_t mStartAddress = 0x00000000;
  uint32_t mEraseAddress = 0x00000000;
  uint8_t mRetData = 0;

  if(i_Resolution == LOW_RES)
  {
    iAddress = START_IMAGE_STATUS_LOW_RES_ADDRESS;   
  }
  else if(i_Resolution == HIGH_RES)
  {
    iAddress = START_IMAGE_STATUS_HIGH_RES_ADDRESS;
  }
  else
  {
    return false;
  }

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
    mImageStatusData[i] = NOR_SPI(NOR_DUMMY_BYTE);
  }  
  if(i_Resolution == LOW_RES)
  {
    for(uint8_t mIndex = 0; mIndex < NUM_OF_IMAGE; mIndex++)
    {
      if(mImageStatusData[mIndex] == 1U)
      {
        mStartAddress = ((0xFFFF * (mIndex)) * NUM_OF_SECTOR_IMAGE_LOW_RES + (mIndex * 4U));  
        for(uint8_t mCount = 0; mCount < NUM_OF_SECTOR_IMAGE_LOW_RES; mCount++)
        {
          mEraseAddress = mStartAddress + ((0xFFFF * mCount) + mCount);    
          for (uint8_t i = 0; i < 50; i++)
          {
            NOR_WriteEnable();
            mRetData = NOR_GetStatus();
            if (mRetData == 2)
            {
              break;
            }
          }
          NOR_EraseSector(mEraseAddress);
          delay(1);
          NOR_WriteDisable();
        }
      }
    }
    
    return NOR_EraseImageStatusSector(LOW_RES);
  }
  else if(i_Resolution == HIGH_RES)
  {
    for(uint8_t mIndex = 0; mIndex < NUM_OF_IMAGE; mIndex++)
    {
      if(mImageStatusData[mIndex] == 1U)
      {
        mStartAddress = START_IMAGE_HIGH_RES_ADDRESS + (mIndex*(NUM_OF_SECTOR_IMAGE_HIGH_RES*0xFFFF)) + (20U*mIndex);  
        for(uint8_t mCount = 0; mCount < NUM_OF_SECTOR_IMAGE_HIGH_RES; mCount++)
        {
          mEraseAddress = mStartAddress + ((0xFFFF * mCount) + mCount);    
          for (uint8_t i = 0; i < 50U; i++)
          {
            NOR_WriteEnable();
            mRetData = NOR_GetStatus();
            if (mRetData == 2U)
            {
              break;
            }
          }
          NOR_EraseSector(mEraseAddress);
          delay(1);
          NOR_WriteDisable();
        }
      }   
    }
    
    return NOR_EraseImageStatusSector(HIGH_RES);
  }
  else
  {
    return false;    
  }
}

uint8_t NOR_ReadImageStatusByResolution(uint8_t i_Resolution)
{
  uint16_t mLenData = NUM_OF_IMAGE;
  uint32_t iAddress = 0x00000000;
  uint8_t mImageStatusData[mLenData] = {0};
  uint8_t mAddress[4] = {0};

  if(i_Resolution == LOW_RES)
  {
    iAddress = START_IMAGE_STATUS_LOW_RES_ADDRESS;   
  }
  else if(i_Resolution == HIGH_RES)
  {
    iAddress = START_IMAGE_STATUS_HIGH_RES_ADDRESS;
  }
  else
  {
    return 255;
  }

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
    mImageStatusData[i] = NOR_SPI(NOR_DUMMY_BYTE);
  }  
  digitalWrite(NOR_CS, HIGH);    
  if(i_Resolution == LOW_RES)
  {
    for(uint8_t mCount = 0; mCount < NUM_OF_IMAGE; mCount++)
    {
      if(mImageStatusData[mCount] == 255)
      {
        return mCount;
      }
    }
  }
  else if(i_Resolution == HIGH_RES)
  {
    for(uint8_t mCount = 0; mCount < NUM_OF_IMAGE; mCount++)
    {
      if(mImageStatusData[mCount] == 255)
      {
        return mCount;
      }
    }        
  }

  return NOR_ReadImageStatusByResolution(i_Resolution);
}

bool NOR_ReadAllImageStatus()
{
  uint16_t mLenData = (NUM_OF_IMAGE*2);
  uint32_t iAddress = 0x00000000;
  uint8_t mImageStatusData[mLenData] = {0};
  uint8_t mAddress[4];
  uint8_t mSendBuffer[mLenData] = {0};
  
  iAddress = START_IMAGE_STATUS_LOW_RES_ADDRESS;   
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
  for (uint16_t i = 0; i < NUM_OF_IMAGE; i++)
  {
    mImageStatusData[i] = NOR_SPI(NOR_DUMMY_BYTE);
//    Serial.println("Image Status");
//    Serial.println(mImageStatusData[i]);
  }  

  iAddress = START_IMAGE_STATUS_HIGH_RES_ADDRESS;
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
  for (uint16_t i = 20; i < (NUM_OF_IMAGE*2); i++)
  {
    mImageStatusData[i] = NOR_SPI(NOR_DUMMY_BYTE);
  }
  uint8_t mACKData = true;  
  SendALLOverI2C(mImageStatusData, mLenData, mACKData, mRecvData[1], mRecvData[2]);
  
  digitalWrite(NOR_CS, HIGH);  
  return true;
  
}

uint8_t NOR_ReadOneImageStatus(uint8_t i_Index, uint8_t iResolution)
{
  uint32_t iAddress = 0x00000000;
  uint8_t mLenData  = NUM_OF_IMAGE;
  uint8_t mImageStatusData[mLenData] = {0};
  uint8_t mAddress[4];

  if(iResolution == LOW_RES)
  {
    iAddress = START_IMAGE_STATUS_LOW_RES_ADDRESS;   
  }
  else if(iResolution == HIGH_RES)
  {
    iAddress = START_IMAGE_STATUS_HIGH_RES_ADDRESS;
  }
  else
  {
    return 255;
  }

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
    mImageStatusData[i] = NOR_SPI(NOR_DUMMY_BYTE);
  }  
  uint8_t mIndex = i_Index - 1U;
  return mImageStatusData[mIndex];
}

bool NOR_UpdateImageStatus(uint8_t i_Resolution, uint8_t i_Index)
{
  uint32_t iAddress = 0x00000000;
  uint8_t mLenData = NUM_OF_IMAGE;
  uint8_t mImageStatusData[mLenData] = {0};
  uint8_t mAddress[4];

  if(i_Resolution == LOW_RES)
  {
    iAddress = START_IMAGE_STATUS_LOW_RES_ADDRESS;   
  }
  else if(i_Resolution == HIGH_RES)
  {
    iAddress = START_IMAGE_STATUS_HIGH_RES_ADDRESS;
  }
  else
  {
    return false;
  }

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
    mImageStatusData[i] = NOR_SPI(NOR_DUMMY_BYTE);
  }  
  uint8_t mIndex = i_Index;
  
  if(mImageStatusData[mIndex] != 0xFF)
  {
    return false;
  }
  else
  {
    NOR_WriteByte((iAddress+mIndex), 1U);      
    return true;
  }
}

bool EraseAllImageStatus(uint8_t iResolution)
{
  uint32_t iAddress = 0x00000000;
  uint8_t mLenData = NUM_OF_IMAGE;
  uint8_t mImageStatusData[mLenData] = {0};
  uint8_t mAddress[4];

  if(iResolution == LOW_RES)
  {
    iAddress = START_IMAGE_STATUS_LOW_RES_ADDRESS;   
  }
  else if(iResolution == HIGH_RES)
  {
    iAddress = START_IMAGE_STATUS_HIGH_RES_ADDRESS;
  }
  else
  {
    return false;
  }

  return NOR_EraseSector(iAddress);
}

bool NOR_GetSingleImageData(uint32_t iNumOfImage, uint8_t iResolution)
{
  uint8_t mNumOfImage       = iNumOfImage - 1; //Image address start from 0
  uint32_t mStartAddres     = 0x00000000;
  uint32_t mCount           = 0x00000000;
  uint32_t mTotalCount      = 0x00000000;
  uint8_t mAddress[4]       = {0};
  uint8_t mImageData[128]   = {0};

  if(iResolution == LOW_RES)
  {
    return NOR_GetImageLowRes(mNumOfImage);  
  }
  else if(iResolution == HIGH_RES)
  {
    return NOR_GetImageHighRes(mNumOfImage); 
  }
  else
  {
    return false;
  }
}

bool NOR_ReadMultipleImageData(uint32_t iStartIndexImageNumber, uint32_t iTotalImageRequested, uint8_t iResolution)
{
  for(uint8_t x = 0; x<iTotalImageRequested; x++)
  {
      bool ret = NOR_GetSingleImageData(iStartIndexImageNumber, iResolution);   
      if(ret == true)
      {
        iStartIndexImageNumber++;
      }
      else
      {
        return false;
      } 
  }  
  return true;
}

bool NOR_GetImageLowRes(uint32_t iNumOfImage)
{
  //One Image     = 262140 Bytes
  uint8_t mNumOfImage       = iNumOfImage;
  uint32_t mStartAddres     = START_IMAGE_LOW_RES_ADDRESS;
  uint32_t mTotalCount      = 0x00000000;
  uint8_t mAddress[4]       = {0};
  uint8_t mImageData[128];

  if(mNumOfImage > 20)
  {
    return false;
  }

  Wire.beginTransmission(5);
  mStartAddres = START_IMAGE_LOW_RES_ADDRESS + (START_IMAGE_LOW_RES_ADDRESS+((NUM_OF_SECTOR_IMAGE_LOW_RES*0xFFFF)*iNumOfImage)) + (NUM_OF_SECTOR_IMAGE_LOW_RES*iNumOfImage);
  for (uint8_t mCountSector = 0; mCountSector < NUM_OF_SECTOR_IMAGE_LOW_RES; mCountSector++)
  {
    //First Address of image
    mStartAddres = mStartAddres + ((0xFFFF * mCountSector) + mCountSector);
    mAddress[0] = mStartAddres >> 24 & 0xFF;
    mAddress[1] = mStartAddres >> 16 & 0xFF;
    mAddress[2] = mStartAddres >>  8 & 0xFF;
    mAddress[3] = mStartAddres >>  0 & 0xFF;
    digitalWrite(NOR_CS, HIGH);
    digitalWrite(NOR_CS, LOW);
    NOR_SPI(NOR_4_BYTE_READ);
    NOR_SPI(mAddress[0]);
    NOR_SPI(mAddress[1]);
    NOR_SPI(mAddress[2]);
    NOR_SPI(mAddress[3]);
    if (mTotalCount == SIZE_OF_IMAGE_LOW_RES)
    {
      break;
    }

    
    for (uint16_t mIterationI = 0; mIterationI < 512U; mIterationI++)
    {
      memset(DataWire,0,255);
      for (uint8_t mIterationJ = 0; mIterationJ < 128U; mIterationJ++)
      {
          mImageData[mIterationJ] = NOR_SPI(NOR_DUMMY_BYTE);
//          Serial.println(mImageData[mIterationJ]);
      }
      
      if(mCountSector == 3 && mIterationI == 511)
      {
        lenDataWire = 137U;
        memcpy(&DataWire[0],&mImageData[0],124U);
        mTotalCount += 124U;
        break;
      }
      else
      {
        lenDataWire = 128U;
        memcpy(&DataWire[0],&mImageData[0],lenDataWire);
        Wire.write((byte*)DataWire,128U);    
        mTotalCount += 128U;
      }
//      Serial.println(mTotalCount);      
      if (mTotalCount == SIZE_OF_IMAGE_LOW_RES)
      {
        break;
      }
    }
    if (mTotalCount == SIZE_OF_IMAGE_LOW_RES)
    {
//      Serial.println("BREAK");
//      Serial.println(mTotalCount);      
      break;
    }
    digitalWrite(NOR_CS, HIGH);
  }
  
  uint8_t mReturnData[13] = {0};
  mReturnData[0]    = 126;
  mReturnData[1]    = 1;
  mReturnData[2]    = 3;
  mReturnData[3]    = 0;
  mReturnData[4]    = 0;
  mReturnData[5]    = 0;
  mReturnData[6]    = 1;
  mReturnData[7]    = 0;
  mReturnData[8]    = 0;
  mReturnData[9]    = 0;
  mReturnData[10]   = 0;
  mReturnData[11]   = 0; //CRC
  mReturnData[12]   = 0; //CRC 

  memcpy(&DataWire[lenDataWire-13],&mReturnData[0],13);
  Wire.write((byte*)DataWire,lenDataWire);
  Wire.endTransmission();
  return true;
}

uint8_t NOR_ReadTotalImageByResolution(uint8_t iResolution)
{
  uint16_t mLenData = NUM_OF_IMAGE;
  uint32_t iAddress = 0x00000000;
  uint8_t mImageStatusData[mLenData] = {0};
  uint8_t mAddress[4];

  if(iResolution == LOW_RES)
  {
    iAddress = START_IMAGE_STATUS_LOW_RES_ADDRESS;   
  }
  else if(iResolution == HIGH_RES)
  {
    iAddress = START_IMAGE_STATUS_HIGH_RES_ADDRESS;
  }
  else
  {
    return false;
  }

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
    mImageStatusData[i] = NOR_SPI(NOR_DUMMY_BYTE);
  }  
  digitalWrite(NOR_CS, HIGH);  

  uint8_t mTotal = 0;
  for(uint8_t mCount = 0; mCount < 20; mCount++)
  {
    if(mImageStatusData[mCount] != 255)
    {
      mTotal++;  
    }
    else
    {
      return mTotal;
    }
  }

  return NOR_ReadTotalImageByResolution(iResolution);
}

bool NOR_ReadAllImageLowResolution(uint8_t iTotalImage)
{
  if(iTotalImage > NUM_OF_IMAGE)
  {
    return false;  
  }

//  uint8_t TotalImage[1] = {iTotalImage};
//  Wire.beginTransmission(5);
//  Wire.write(TotalImage,1);
//  Wire.endTransmission();
//  
  for(uint8_t mIndex = 0; mIndex < iTotalImage; mIndex++)
  {
//    Serial.print("INDEX");
//    Serial.println(mIndex);
    bool ret = NOR_GetImageLowRes(mIndex);   
//    Serial.print("RESULT");
//    Serial.println(ret);

    if(ret == false)
    {
      return false;
    }
    else
    {
      uint8_t mFlagOneImage[4] = {0xF0,0xF0,0xF0,0xF0};
//      SendDataOverI2C(mFlagOneImage, 4U);        
    }
  }
  return true;
}

bool NOR_GetImageHighRes(uint32_t iNumOfImage)
{
  //One Image               = 1310700 Bytes
  uint8_t mNumOfImage       = iNumOfImage;
  uint32_t mStartAddres     = 0x00000000;
  uint32_t mTotalCount      = 0x00000000;
  uint8_t mAddress[4]       = {0};
  uint8_t mImageData[128]   = {0};

  if(iNumOfImage >= NUM_OF_IMAGE)
  {
    return false;
  }
   
  Wire.beginTransmission(5);
  for (uint8_t mCountSector = 0; mCountSector < NUM_OF_IMAGE; mCountSector++)
  {
    //First Address of image
    mStartAddres = START_IMAGE_HIGH_RES_ADDRESS + (mNumOfImage*(NUM_OF_SECTOR_IMAGE_HIGH_RES*0xFFFF)) + (NUM_OF_SECTOR_IMAGE_HIGH_RES*mNumOfImage);
    mStartAddres = mStartAddres + ((0xFFFF * mCountSector) + mCountSector);
    mAddress[0] = mStartAddres >> 24 & 0xFF;
    mAddress[1] = mStartAddres >> 16 & 0xFF;
    mAddress[2] = mStartAddres >>  8 & 0xFF;
    mAddress[3] = mStartAddres >>  0 & 0xFF;
    digitalWrite(NOR_CS, HIGH);
    digitalWrite(NOR_CS, LOW);
    NOR_SPI(NOR_4_BYTE_READ);
    NOR_SPI(mAddress[0]);
    NOR_SPI(mAddress[1]);
    NOR_SPI(mAddress[2]);
    NOR_SPI(mAddress[3]);
    if (mTotalCount == SIZE_OF_IMAGE_HIGH_RES)
    {
      break;
    }
    
      
    for (uint16_t mIterationI = 0; mIterationI < 512U; mIterationI++)
    {
      memset(DataWire,0,255);
      for (uint8_t mIterationJ = 0; mIterationJ < 128U; mIterationJ++)
      {
        mImageData[mIterationJ] = NOR_SPI(NOR_DUMMY_BYTE);
      }
      
      if(mCountSector == 19U && mIterationI == 511)
      {
        //Here send
        lenDataWire = 125U;        
        mTotalCount += 108U;
        memcpy(&DataWire[0],&mImageData[0],108U);
        break;
      }
      else
      {
        lenDataWire = 128U;
        memcpy(&DataWire[0],&mImageData[0],lenDataWire); 
        mTotalCount += 128U;
      }      
      if (mTotalCount == SIZE_OF_IMAGE_HIGH_RES){break;}
    }
    if (mTotalCount == SIZE_OF_IMAGE_HIGH_RES){break;}
    digitalWrite(NOR_CS, HIGH);
  }  
  
  uint8_t mFlagOneImage[4] = {0xF0,0xF0,0xF0,0xF0};
  memcpy(&DataWire[(lenDataWire-17)],&mFlagOneImage[0],4);
  
  uint8_t mReturnData[13] = {0};
  mReturnData[0]    = 126;
  mReturnData[1]    = 1;
  mReturnData[2]    = 3;
  mReturnData[3]    = 0;
  mReturnData[4]    = 0;
  mReturnData[5]    = 0;
  mReturnData[6]    = 1;
  mReturnData[7]    = 0;
  mReturnData[8]    = 0;
  mReturnData[9]    = 0;
  mReturnData[10]   = 0;
  mReturnData[11]   = 0; //CRC
  mReturnData[12]   = 0; //CRC
  
  memcpy(&DataWire[(lenDataWire-13)],&mReturnData[0],13);
  Wire.write((byte*)DataWire,lenDataWire);
  Wire.endTransmission();        
  return true;
}

bool NOR_ReadAllImageHighResolution(uint8_t iTotalImage)
{
  if(iTotalImage >= 20U)
  {
    return false;
  }

//  uint8_t TotalImage[1] = {iTotalImage};

//  Wire.beginTransmission(5);
//  Wire.write(TotalImage,sizeof(TotalImage));
//  Wire.endTransmission();
  
  for(uint8_t mImageNumb = 0; mImageNumb < iTotalImage; mImageNumb++)
  {
      bool ret = NOR_GetImageHighRes(mImageNumb);   
      if(ret == false)
      {
        return false;
      }
  }  
  return true;
}

bool NOR_ReadTemperatureData()
{
  /*
     Read Image Status on address TEMPERATURE_SECTOR_ADDRESS
     Send to I2C
  */
  uint16_t mLenData = 400;
  uint32_t iAddress = TEMPERATURE_SECTOR_ADDRESS;
  uint8_t mTemperatureData[mLenData] = {0};
  uint8_t mAddress[4];
  uint8_t mSendBuffer[200] = {0};

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

  for(uint8_t x = 0; x<2; x++)
  {
    memcpy(&mSendBuffer[0], &mTemperatureData[(x*200)], 200);
//    SendDataOverI2C(mSendBuffer, 200U);
  }

  digitalWrite(NOR_CS, HIGH);  
  return true;
}

bool NOR_ReadCameraSetting()
{
  uint32_t iAddress = ACTIVE_CONFIG_ADDRESS;
  uint8_t mCameraSetting[LEN_OF_ACTIVE_CONFIG] = {0};
  uint8_t mAddress[4];
  uint8_t mSendBuffer[200] = {0};

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
  for (uint16_t i = 0; i < LEN_OF_ACTIVE_CONFIG; i++)
  {
    mCameraSetting[i] = NOR_SPI(NOR_DUMMY_BYTE);
  }  
  memcpy(&mSendBuffer[0], &mCameraSetting[0], 200);
  uint8_t mACKData = true;
  SendALLOverI2C(mSendBuffer, 200U, mACKData, mRecvData[1], mRecvData[2]);
  digitalWrite(NOR_CS, HIGH);  
  return true;
}

bool NOR_EraseImageStatusSector(uint8_t iResolution)
{
  uint8_t mState = 0;
  uint8_t mAddress = 0x00000000;
  bool mRet = false;
  for (uint8_t i = 0; i < 50; i++)
  {
    NOR_WriteEnable();
    mState = NOR_GetStatus();
    if (mState == 2)
    {
      break;
    }
  }

  if(iResolution == LOW_RES)
  {
    mRet = NOR_EraseSector(START_IMAGE_STATUS_LOW_RES_ADDRESS);
    NOR_WriteDisable();    
  }
  else if(iResolution == HIGH_RES)
  {
    mRet = NOR_EraseSector(START_IMAGE_STATUS_HIGH_RES_ADDRESS);
    NOR_WriteDisable();        
  }
  else if(iResolution == 2U)
  {
    bool mLowReturn = NOR_EraseSector(START_IMAGE_STATUS_LOW_RES_ADDRESS);
    NOR_WriteDisable();        
    bool mHighReturn = NOR_EraseSector(START_IMAGE_STATUS_HIGH_RES_ADDRESS);
    NOR_WriteDisable();        

    if(mLowReturn == true && mHighReturn == true)
    {
      mRet = true;
    }
  }
  return mRet;
}

bool NOR_EraseAll()
{
  uint32_t mDestinationAddress = 0x00000000;
  uint8_t mRetData = 0;  

  for (uint16_t mSector = 0; mSector < 730U; mSector++)
  {
    mDestinationAddress = (mSector * 0xFFFF) + mSector;
    for (uint8_t i = 0; i < 50; i++)
    {
      NOR_WriteEnable();
      mRetData = NOR_GetStatus();
      if (mRetData == 2)
      {
        break;
      }
    }
    NOR_EraseSector(mDestinationAddress);
    delay(1);
    NOR_WriteDisable();
  }
  return true;
}

bool NOR_EraseSector(uint32_t i_Address)
{
  uint8_t mAddress[4] = {0};
  mAddress[0] = i_Address >> 24 & 0xFF;
  mAddress[1] = i_Address >> 16 & 0xFF;
  mAddress[2] = i_Address >>  8 & 0xFF;
  mAddress[3] = i_Address >>  0 & 0xFF;

  digitalWrite(NOR_CS, HIGH);
  digitalWrite(NOR_CS, LOW);
  NOR_SPI(NOR_ERASE_SECTOR_4_BYTE);

  //Transfer Address
  NOR_SPI(mAddress[0]);
  NOR_SPI(mAddress[1]);
  NOR_SPI(mAddress[2]);
  NOR_SPI(mAddress[3]);
  digitalWrite(NOR_CS, HIGH);
  return true;
}

void NOR_EraseSubSector(uint32_t i_Address)
{
  uint8_t mAddress[4] = {0};
  mAddress[0] = i_Address >> 24 & 0xFF;
  mAddress[1] = i_Address >> 16 & 0xFF;
  mAddress[2] = i_Address >>  8 & 0xFF;
  mAddress[3] = i_Address >>  0 & 0xFF;

  digitalWrite(NOR_CS, HIGH);
  digitalWrite(NOR_CS, LOW);
  NOR_SPI(NOR_ERASE_SUBSECTOR_4_BYTE);

  //Transfer Address
  NOR_SPI(mAddress[0]);
  NOR_SPI(mAddress[1]);
  NOR_SPI(mAddress[2]);
  NOR_SPI(mAddress[3]);
  digitalWrite(NOR_CS, HIGH);
}

bool NOR_EraseSingleImageLowRes(uint8_t iNumOfImage)
{
  iNumOfImage = (iNumOfImage - 1); //Image address start from 0
  uint32_t mStartAddres = 0x00000000;
  uint8_t mRetData = 0;
  for (uint8_t mSector = 0; mSector < 4; mSector++)
  {
    mStartAddres = ((0xFFFF * (iNumOfImage)) * 4 + (iNumOfImage * 4));
    //Next Address of image following iteration
    mStartAddres = mStartAddres + ((0xFFFF * mSector) + mSector);
    for (uint8_t i = 0; i < 50; i++)
    {
      NOR_WriteEnable();
      mRetData = NOR_GetStatus();
      if (mRetData == 2)
      {
        break;
      }
    }
    NOR_EraseSector(mStartAddres);
    delay(20);
    NOR_WriteDisable();
  }

  return true;
}

bool NOR_EraseMultipleImageLowRes(uint32_t mStartNumber, uint32_t mTotalNumber)
{
   for(uint8_t x = 0; x < mTotalNumber; x++)
   {
     bool ret = NOR_EraseSingleImageLowRes(mStartNumber);
     if(ret)
     {
       mStartNumber++;
     }
     else
     {
       return false;
     }
   }
   return true;
}

bool NOR_EraseSingleImageHighRes(uint8_t iNumOfImage)
{
  iNumOfImage = (iNumOfImage - 1);
  uint32_t mAddress = 0x00000000;
  uint8_t mRetData = 0;
  for (uint8_t mSector = 0; mSector < NUM_OF_SECTOR_IMAGE_HIGH_RES; mSector++)
  {
    mAddress = START_IMAGE_HIGH_RES_ADDRESS + (START_IMAGE_HIGH_RES_ADDRESS+((NUM_OF_SECTOR_IMAGE_HIGH_RES*0xFFFF)*iNumOfImage)) + (NUM_OF_SECTOR_IMAGE_HIGH_RES*iNumOfImage);
    for (uint8_t i = 0; i < 50; i++)
    {
      NOR_WriteEnable();
      mRetData = NOR_GetStatus();
      if (mRetData == 2)
      {
        break;
      }
    }
    NOR_EraseSector(mAddress);
    delay(20);
    NOR_WriteDisable();
  }

  return true;
}

bool NOR_EraseMultipleImageHighRes(uint32_t mStartIndex, uint32_t mTotalImage)
{
   for(uint8_t x = 0; x < mTotalImage; x++)
   {
     bool ret = NOR_EraseSingleImageHighRes(mStartIndex);
     if(ret)
     {
       mStartIndex++;
     }
     else
     {
       return false;
     }
   }
   return true;
}

bool NOR_WriteByte(uint32_t i_Address, uint8_t i_Data)
{
  uint8_t mAddress [4];
  uint8_t mRetData = 0;
  for (uint8_t i = 0; i < 50; i++)
  {
    NOR_WriteEnable();
    mRetData = NOR_GetStatus();
    if (mRetData == 2)
    {
      break;
    }
  }

  mAddress[0] = i_Address >> 24 & 0xFF;
  mAddress[1] = i_Address >> 16 & 0xFF;
  mAddress[2] = i_Address >>  8 & 0xFF;
  mAddress[3] = i_Address >>  0 & 0xFF;
  digitalWrite(NOR_CS, HIGH);
  digitalWrite(NOR_CS, LOW);
  NOR_SPI(NOR_4_BYTE_PROGRAM);

  //Transfer Address
  NOR_SPI(mAddress[0]);
  NOR_SPI(mAddress[1]);
  NOR_SPI(mAddress[2]);
  NOR_SPI(mAddress[3]);
  NOR_SPI(i_Data);
  digitalWrite(NOR_CS, HIGH);
  return true;
}

bool NOR_WriteSector(uint32_t i_Address, uint8_t *i_Data, uint16_t iLen)
{
  uint8_t mAddress [4];
  uint8_t mWrittenData[256] = {0};
  memcpy(&mWrittenData[0], &i_Data[0], iLen);

  //Get Status Here Before Execute Writing
  for (uint8_t i = 0; i < 50; i++)
  {
    NOR_WriteEnable();
    uint8_t mRetData = NOR_GetStatus();
    if (mRetData == 2)
    {
      break;
    }
  }

  mAddress[0] = i_Address >> 24 & 0xFF;
  mAddress[1] = i_Address >> 16 & 0xFF;
  mAddress[2] = i_Address >>  8 & 0xFF;
  mAddress[3] = i_Address >>  0 & 0xFF;
  digitalWrite(NOR_CS, HIGH);
  digitalWrite(NOR_CS, LOW);
  NOR_SPI(NOR_4_BYTE_PROGRAM);

  //Transfer Address
  NOR_SPI(mAddress[0]);
  NOR_SPI(mAddress[1]);
  NOR_SPI(mAddress[2]);
  NOR_SPI(mAddress[3]);

  for (uint16_t i = 0; i < iLen; i++)
  {
    NOR_SPI(mWrittenData[i]);
  }
  digitalWrite(NOR_CS, HIGH);
  return true;
}

uint8_t *NOR_ReadByAddress(uint32_t i_Address, uint16_t mReturnLen)
{
  uint8_t mAddress[4];
  uint8_t mRetData[mReturnLen] = {0};

  mAddress[0] = i_Address >> 24 & 0xFF;
  mAddress[1] = i_Address >> 16 & 0xFF;
  mAddress[2] = i_Address >>  8 & 0xFF;
  mAddress[3] = i_Address >>  0 & 0xFF;

  digitalWrite(NOR_CS, HIGH);
  digitalWrite(NOR_CS, LOW);

  NOR_SPI(NOR_4_BYTE_READ);
  NOR_SPI(mAddress[0]);
  NOR_SPI(mAddress[1]);
  NOR_SPI(mAddress[2]);
  NOR_SPI(mAddress[3]);

  for (uint16_t i = 0; i < mReturnLen; i++)
  {
    mRetData[i] = NOR_SPI(NOR_DUMMY_BYTE);
  }
  digitalWrite(NOR_CS, HIGH);
  return mRetData;
}

void NOR_ReadSector(uint32_t i_Address)
{
  uint8_t mAddress[4];
  uint8_t mRetData[100] = {0};

  mAddress[0] = i_Address >> 24 & 0xFF;
  mAddress[1] = i_Address >> 16 & 0xFF;
  mAddress[2] = i_Address >>  8 & 0xFF;
  mAddress[3] = i_Address >>  0 & 0xFF;

  digitalWrite(NOR_CS, HIGH);
  digitalWrite(NOR_CS, LOW);

  NOR_SPI(NOR_4_BYTE_READ);
  NOR_SPI(mAddress[0]);
  NOR_SPI(mAddress[1]);
  NOR_SPI(mAddress[2]);
  NOR_SPI(mAddress[3]);

  for (uint16_t i = 0; i < 655; i++)
  {
    for (uint16_t j = 0; j < 100; j++)
    {
      mRetData[j] = NOR_SPI(NOR_DUMMY_BYTE);
    }
    Serial.write(mRetData, 100);
  }
  digitalWrite(NOR_CS, HIGH);
}

uint8_t NOR_ReadAddress(uint32_t i_Address)
{
  uint8_t mAddress[4];
  uint8_t mReadData = 0;
  mAddress[0] = i_Address >> 24 & 0xFF;
  mAddress[1] = i_Address >> 16 & 0xFF;
  mAddress[2] = i_Address >>  8 & 0xFF;
  mAddress[3] = i_Address >>  0 & 0xFF;

  digitalWrite(NOR_CS, HIGH);
  digitalWrite(NOR_CS, LOW);

  NOR_SPI(NOR_4_BYTE_READ);
  NOR_SPI(mAddress[0]);
  NOR_SPI(mAddress[1]);
  NOR_SPI(mAddress[2]);
  NOR_SPI(mAddress[3]);

  mReadData = NOR_SPI(NOR_DUMMY_BYTE);
  digitalWrite(NOR_CS, HIGH);
  return mReadData;
}

bool isNOR_Busy()
{
  NOR_SPI(NOR_READ_STATUS_REGISTER);
  uint8_t res =  NOR_SPI(NOR_DUMMY_BYTE);
  if (res & 1)
  {
    return true;
  }
  else
  {
    return false;
  }
}

uint8_t NOR_SPI(uint8_t iData)
{
  uint8_t ret;
  ret = SPI_NOR.transfer(iData);
  return ret;
}

bool NOR_Access(uint8_t MUX_Mode)
{
  uint8_t MUXtoLocana = 0;
  uint8_t MUXtoICU = 1;
  if(MUX_Mode == MUXtoLocana)
  {
    digitalWrite(selector,LOW);
  }

  else if(MUX_Mode == MUXtoICU)
  {
    digitalWrite(selector,HIGH);
  }

  return true;
}

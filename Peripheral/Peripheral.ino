#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function
#include <Wire.h>

//COMMAND LIST
#define NOR_COMMAND 1 
#define CAMERA_COMMAND 2
#define TEMP_SENSOR_COMMAND 3
#define NOR_ACCESS_COMMAND 4
#define COMM_ACCESS 5

#define CHECK_NOR_HARDWARE_CONNECTION 1
#define READ_ALL_IMAGE_STATUS 2
#define READ_SINGLE_IMAGE 3
#define READ_MULTIPLE_IMAGE 4
#define READ_TEMPERATURE_DATA 5
#define READ_TOTAL_IMAGE 6
#define ERASE_ALL_SECTOR 8
#define ERASE_SINGLE_IMAGE 9
#define ERASE_MULTIPLE_IMAGE 10
#define RELOAD_CONFIGURATION 12
#define READ_ALL_CONFIGURATION 13
#define SET_ALL_CONFIG_TO_DEFAULT 14
#define ERASE_SINGLE_IMAGE_LOW_RES 15
#define ERASE_SINGLE_IMAGE_HIGH_RES 16
#define ERASE_MULTIPLE_IMAGE_LOW_RES 17
#define ERASE_MULTIPLE_IMAGE_HIGH_RES 18

#define CAMERA_REINIT 1
#define CAMERA_TAKE_SINGLE_IMAGE 3
#define CAMERA_TAKE_MULTIPLE_IMAGE 4
#define UPDATE_SETTING_BRIGHTNESS 5      
#define UPDATE_SETTING_CONTRAST 6        
#define UPDATE_SETTING_LIGHT 7           
#define UPDATE_SETTING_SATURATION 8     
#define UPDATE_SETTING_HUE 9             
#define UPDATE_SETTING_SPECIAL 10         
#define UPDATE_SETTING_EXPOSURE 11        
#define UPDATE_SETTING_SHARPNESS 12       
#define UPDATE_SETTING_MIRROR 13          
#define UPDATE_SETTING_COMPRESS_QUALITY 14
#define UPDATE_SETTING_RESOLUTION 15  
#define UPDATE_ALL_CONFIG_TO_CAMERA 16    

#define READ_SINGLE_TEMP 3
#define READ_TEMP_SCHEDULER 4
#define READ_TEMP_TIMER 5
#define READ_TEMP_SECTOR 6
#define UPDATE_TEMP_TIMER 7
#define UPDATE_TEMP_SCHEDULER 8

#define REQUEST_NOR_ACCESS_COMMAND 1

#define COMM_SWITCH_PORT 1

static const uint8_t mlenCommand = 13;
uint8_t   mRecvData[mlenCommand];

uint8_t   CommandHeader       = 0;
uint8_t   CommandSector       = 0;
uint8_t   CommandSubSector    = 0;
uint8_t   CommandValue1[4]    = {0};
uint8_t   CommandValue2[4]    = {0};
uint8_t   CommandArrayCRC[2]  = {0};

byte DataWire[255];
byte ACKWire[13];
uint16_t lenDataWire = 255;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);

}

void loop() {
  while(1)
  {
      while(Serial.available())
      {
        for(uint8_t mCommandByteLen = 0U; mCommandByteLen < mlenCommand; mCommandByteLen++)
        {
          mRecvData[mCommandByteLen] = Serial.read();
        }              

        if(mRecvData[0] == 125U)
        {
          SendDataOverI2C(mRecvData,13U);
          ProcessReceivedCommand(mRecvData);
//          RecvACKOverI2C();
        }
      }
  }

}

uint32_t DeserializeU32(uint8_t* iArray)
{
  uint32_t retVal = 0;
  retVal |= iArray[0]; retVal <<= 8;
  retVal |= iArray[1]; retVal <<= 8;
  retVal |= iArray[2]; retVal <<= 8;
  retVal |= iArray[3];
  return retVal;
}

void ProcessReceivedCommand(uint8_t* iCommand)
{
  CommandHeader       = iCommand[0];
  CommandSector       = iCommand[1];
  CommandSubSector    = iCommand[2];
  CommandValue1[0]    = iCommand[3];
  CommandValue1[1]    = iCommand[4];
  CommandValue1[2]    = iCommand[5];
  CommandValue1[3]    = iCommand[6];
  CommandValue2[0]    = iCommand[7];
  CommandValue2[1]    = iCommand[8];
  CommandValue2[2]    = iCommand[9];
  CommandValue2[3]    = iCommand[10];
  CommandArrayCRC[0]  = iCommand[11];
  CommandArrayCRC[1]  = iCommand[12];

  uint32_t mCommValue1 = DeserializeU32(CommandValue1);
  uint32_t mCommValue2 = DeserializeU32(CommandValue2);

  if(isHeaderOK(CommandHeader))
  {
    if(CommandSector == NOR_COMMAND)
    {
        if(CommandSubSector == CHECK_NOR_HARDWARE_CONNECTION)
        {
          RecvACKOverI2C();
        }
        else if(CommandSubSector == READ_ALL_IMAGE_STATUS)
        {
          uint16_t mLenData = 40;
          RecvALLOverI2C(mLenData);                           
        }
//        else if(CommandSubSector == READ_SINGLE_IMAGE)
//        {
//          uint8_t mData[20][512][128];
//          uint8_t mACK[13];
//          uint8_t DataWireLen = 128U;
//          uint8_t FlagInd[4];
//          uint8_t NUM_OF_SECTOR_IMAGE_LOW_RES = 4;
//          
//          if(mCommValue1 == 0)
//          {
//            for (uint8_t mCountSector = 0; mCountSector < NUM_OF_SECTOR_IMAGE_LOW_RES; mCountSector++)
//            {
//                for (uint16_t mIterationI = 0; mIterationI < 512U; mIterationI++)
//                {
//                    if(mCountSector == 3 && mIterationI == 511)
//                    {
//                      Wire.requestFrom(5,137U);
//                      while(Wire.available() != 137U)
//                      {
//                        ;
//                      }
//
//                      if(Wire.available() == 137U)
//                      {
//                        Wire.readBytes((byte*)DataWire,137U);
//                        memcpy(&mData[mCountSector][mIterationI][0],&DataWire[0],124U);
//                        memcpy(&mACK[0],&DataWire[124U],13U);
//                      }
//                    }
//
//                    else
//                    {
//                      Wire.requestFrom(5,128U);
//                      while(Wire.available() != 128U)
//                      {
//                        ;
//                      }
//
//                      if(Wire.available() == 128U)
//                      {
//                        Wire.readBytes((byte*)DataWire,128U);
//                        memcpy(&mData[mCountSector][mIterationI][0],&DataWire[0],128U);
//                      }
//                    }
//                }
//            }
//
//            for (uint8_t mCountSector = 0; mCountSector < NUM_OF_SECTOR_IMAGE_LOW_RES; mCountSector++)
//            {
//                for (uint16_t mIterationI = 0; mIterationI < 512U; mIterationI++)
//                {
//                    if(mCountSector == 3 && mIterationI == 511)
//                    {
//                      SendDataOverUART0(mData[mCountSector][mIterationI],124U);
//                    }
//
//                    else
//                    {
//                      SendDataOverUART0(mData[mCountSector][mIterationI],128U);
//                    }
//                  
//                }
//            }
//          }
//
//          else if(mCommValue1 == 1)
//          {
//            for (uint8_t mCountSector = 0; mCountSector < NUM_OF_SECTOR_IMAGE_LOW_RES; mCountSector++)
//            {
//                for (uint16_t mIterationI = 0; mIterationI < 512U; mIterationI++)
//                {
//                    if(mCountSector == 3 && mIterationI == 511)
//                    {
//                      Wire.requestFrom(5,125U);
//                      while(Wire.available() != 125U)
//                      {
//                        ;
//                      }
//
//                      if(Wire.available() == 125U)
//                      {
//                        Wire.readBytes((byte*)DataWire,125U);
//                        memcpy(&mData[mCountSector][mIterationI][0],&DataWire[0],108U);
//                        memcpy(&FlagInd[0],&DataWire[108U],4U);
//                        memcpy(&mACK[0],&DataWire[112U],13U);
//                      }
//                    }
//
//                    else
//                    {
//                      Wire.requestFrom(5,128U);
//                      while(Wire.available() != 128U)
//                      {
//                        ;
//                      }
//
//                      if(Wire.available() == 128U)
//                      {
//                        Wire.readBytes((byte*)DataWire,128U);
//                        memcpy(&mData[mCountSector][mIterationI][0],&DataWire[0],128U);
//                      }
//                    }
//                }
//            }
//
//            for (uint8_t mCountSector = 0; mCountSector < NUM_OF_SECTOR_IMAGE_LOW_RES; mCountSector++)
//            {
//                for (uint16_t mIterationI = 0; mIterationI < 512U; mIterationI++)
//                {
//                    if(mCountSector == 3 && mIterationI == 511)
//                    {
//                        SendDataOverUART0(mData[mCountSector][mIterationI],108U);
//                        SendDataOverUART0(FlagInd,4U);                  
//                    }
//
//                    else
//                    {
//                      SendDataOverUART0(mData[mCountSector][mIterationI],128U);
//                    }
//                  
//                }
//            }
//          }
//
//          SendACKOverUART0(mACK,13);
//        }
        else if(CommandSubSector == READ_MULTIPLE_IMAGE)
        {
          
        }
        else if(CommandSubSector == READ_TOTAL_IMAGE)
        {
            RecvACKOverI2C();
        }
        else if(CommandSubSector == READ_TEMPERATURE_DATA)
        {
//          uint16_t mLenData = 200U;
//          for(uint8_t x = 0; x < 2; x++)
//          {
//            RecvDataOverI2C(mLenData);
//          }
        }
        else if(CommandSubSector == ERASE_ALL_SECTOR)
        {
                 RecvACKOverI2C();               
        }
        else if(CommandSubSector == RELOAD_CONFIGURATION)
        {
          RecvACKOverI2C();
        }
        else if(CommandSubSector == READ_ALL_CONFIGURATION)
        {
          uint16_t mLenData = 200U;
          RecvALLOverI2C(mLenData);
        }
        else if(CommandSubSector == SET_ALL_CONFIG_TO_DEFAULT)
        {
            RecvACKOverI2C();                 
        }
        else if(CommandSubSector == ERASE_SINGLE_IMAGE_LOW_RES)
        {
             RecvACKOverI2C();                
        }
        else if(CommandSubSector == ERASE_SINGLE_IMAGE_HIGH_RES)
        {
             RecvACKOverI2C();                
        }
        else if(CommandSubSector == ERASE_MULTIPLE_IMAGE_LOW_RES)
        {
             RecvACKOverI2C();               
        }
        else if(CommandSubSector == ERASE_MULTIPLE_IMAGE_HIGH_RES)
        {
             RecvACKOverI2C();                 
        }
    }
    else if(CommandSector == CAMERA_COMMAND)
    {
      if (CommandSubSector == CAMERA_REINIT)
      {
           RecvACKOverI2C();       
      }
      else if(CommandSubSector == CAMERA_TAKE_SINGLE_IMAGE)
      {
        RecvACKOverI2C();
      }        
      else if(CommandSubSector == CAMERA_TAKE_MULTIPLE_IMAGE)
      {
        RecvACKOverI2C();
      }
      else if(CommandSubSector == UPDATE_SETTING_BRIGHTNESS)
      {
        RecvACKOverI2C();
      }
      else if(CommandSubSector == UPDATE_SETTING_CONTRAST)        
      {
        RecvACKOverI2C();
      }
      else if(CommandSubSector == UPDATE_SETTING_LIGHT)           
      {
        RecvACKOverI2C();
      }
      else if(CommandSubSector == UPDATE_SETTING_SATURATION)     
      {
        RecvACKOverI2C();
      }
      else if(CommandSubSector == UPDATE_SETTING_HUE)             
      {
        RecvACKOverI2C();
      }
      else if(CommandSubSector == UPDATE_SETTING_SPECIAL)         
      {
        RecvACKOverI2C();
      }
      else if(CommandSubSector == UPDATE_SETTING_EXPOSURE)        
      {
        RecvACKOverI2C();
      }
      else if(CommandSubSector == UPDATE_SETTING_SHARPNESS)       
      {
        RecvACKOverI2C();
      }
      else if(CommandSubSector == UPDATE_SETTING_COMPRESS_QUALITY)
      {
        RecvACKOverI2C();
      }
      else if(CommandSubSector == UPDATE_SETTING_RESOLUTION)      
      {
        RecvACKOverI2C();
      }
      else if(CommandSubSector == UPDATE_ALL_CONFIG_TO_CAMERA)
      {
        RecvACKOverI2C();
      }
    }        
    else if(CommandSector == TEMP_SENSOR_COMMAND)
    {
      if(CommandSubSector == READ_SINGLE_TEMP)
      {
          uint16_t mLenData = 4;
          RecvALLOverI2C(mLenData);                    
      }
      else if(CommandSubSector == READ_TEMP_SCHEDULER)
      {
         RecvACKOverI2C(); 
      }
      else if(CommandSubSector == READ_TEMP_TIMER)
      {                                                  
          uint16_t mLenData = 4;
          RecvALLOverI2C(mLenData);
      }
      else if(CommandSubSector == READ_TEMP_SECTOR)
      {
          uint16_t mLenData = 800;
          RecvALLOverI2C(mLenData);
      }
      else if(CommandSubSector == UPDATE_TEMP_TIMER)
      {
        RecvACKOverI2C();
      }
      else if(CommandSubSector == UPDATE_TEMP_SCHEDULER)
      {
        
      }RecvACKOverI2C();
    }
    else if(CommandSector == NOR_ACCESS_COMMAND)
    {
      if(CommandSubSector == REQUEST_NOR_ACCESS_COMMAND)
      {
        //TODO: FIXME!!! IMPLEMENT THIS BEHAVIOUR !!!             
      }
    }
    else if(CommandSector == COMM_ACCESS)
    {
      if(CommandSubSector == COMM_SWITCH_PORT)
      {
        
      }
    }
  }
}

bool SendDataOverI2C(uint8_t* iData, uint16_t iLenData)
{
  uint8_t mData[iLenData] = {0};
  memcpy(&mData[0], &iData[0], iLenData);
  Wire.beginTransmission(5);    //Transmit To Device #5
  Wire.write(mData, iLenData);   
  Wire.endTransmission();       //Stop Transmitting
  return true;
}

void RecvALLOverI2C(uint16_t iLenData)
{
  uint8_t mData[800] = {0};
  uint8_t mACK[13];
  uint16_t AllLen = iLenData + 13;
  if(iLenData <= 255)
  {
    Wire.requestFrom(5,AllLen);
    while(Wire.available() != AllLen)
    {
      ;
    }

    if(Wire.available() == AllLen)
    {
      Wire.readBytes((byte*)DataWire,AllLen);
    }
    memcpy(&mData[0],&DataWire[0],iLenData);
    memcpy(&mACK[0],&DataWire[iLenData],13);
    
    SendDataOverUART0(mData,iLenData);
    SendACKOverUART0(mACK,13);
  }

  else if(iLenData > 255)
  {
    uint8_t CalcLoop = iLenData/255;
    uint8_t ModCalcLoop = iLenData%255;
    if(ModCalcLoop == 0)
    {
      for(uint16_t LoopTransmit = 0; LoopTransmit < CalcLoop; LoopTransmit++)
      {
        Wire.requestFrom(5,255);
        while(Wire.available() != 255)
        {
          ;
        }

        if(Wire.available() == 255)
        {
          Wire.readBytes((byte*)DataWire,255);
        }
          memcpy(&mData[0],&DataWire[0],255);
          SendDataOverUART0(mData,255);
          memset(DataWire,0,sizeof(mData));
      }
    }

    else if(ModCalcLoop != 0)
    {
      for(uint16_t LoopTransmit = 0; LoopTransmit < CalcLoop; LoopTransmit++)
      {
        Wire.requestFrom(5,255);
        while(Wire.available() != 255)
        {
          ;
        }

        if(Wire.available() == 255)
        {
          Wire.readBytes((byte*)DataWire,255);
          memcpy(&mData[LoopTransmit*255],&DataWire[0],255);
        }
          
//          SendDataOverUART0(mData,255);
          memset(DataWire,0,sizeof(DataWire));
      }

      Wire.requestFrom(5,ModCalcLoop+13);
      while(Wire.available() != ModCalcLoop+13)
      {
        ;
      }

      if(Wire.available() == ModCalcLoop+13)
      {
        Wire.readBytes((byte*)DataWire,(ModCalcLoop+13));
        memcpy(&mData[(iLenData-ModCalcLoop)],&DataWire[0],ModCalcLoop);
        memcpy(&mACK[0],&DataWire[ModCalcLoop],13);
        SendDataOverUART0(mData,iLenData);
        SendACKOverUART0(mACK,13);
      }


    }
  }
}

void RecvACKOverI2C()
{
  uint8_t mReturnData[13];
  Wire.requestFrom(5,13);
  while(Wire.available() != 13)
  {
    ;
  }

  if(Wire.available() == 13)
  {
    Wire.readBytes((byte*)ACKWire,sizeof(ACKWire));
  }
  memcpy(&mReturnData[0],&ACKWire[0],sizeof(mReturnData));
  SendACKOverUART0(mReturnData,sizeof(mReturnData));
}


bool SendDataOverUART0(uint8_t* iData, uint16_t iLenData)
{
  uint8_t mData[iLenData] = {0};
  memcpy(&mData[0], &iData[0], iLenData);    
  Serial.write(mData, iLenData);   
  return true;
  memset(mData,0,sizeof(mData));
  Serial.flush();  
}

bool SendACKOverUART0(uint8_t* iDataACK, uint16_t iLenDataACK)
{
  uint8_t mDataACK[iLenDataACK] = {0};
  memcpy(&mDataACK[0], &iDataACK[0], iLenDataACK);    
  Serial.write(mDataACK, iLenDataACK);   
  return true;
  memset(mRecvData,0,sizeof(mRecvData));
  Serial.flush();
  memset(mDataACK,0,sizeof(mDataACK));
  Serial.flush();  
}

bool isHeaderOK(uint8_t i_Header)
{
  if(i_Header == 125)
  {
    return true;
  }
  else
  {
    return false;
  }
}

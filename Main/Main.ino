//LOCANA PERIPHERAL

#include <Arduino.h>   // required before wiring_private.h
#include <SPI.h>
#include "wiring_private.h" // pinPeripheral() function
#include <Wire.h>
#include <ArduCAM.h>
#define NOR_CS 6
#define CAM_CS 10
#define selector 9

//ADDRESS
#define START_IMAGE_LOW_RES_ADDRESS 0x00000000
#define START_IMAGE_HIGH_RES_ADDRESS 0x00640000
#define START_IMAGE_STATUS_LOW_RES_ADDRESS 0x02440000
#define START_IMAGE_STATUS_HIGH_RES_ADDRESS 0x02480000
#define TEMPERATURE_SECTOR_ADDRESS 0x024C0000
#define ACTIVE_CONFIG_ADDRESS 0x02540000
#define PENDING_CONFIG_ADDRESS 0x025C0000
#define TEST_CONNECTION_ADDRESS 0x02640000

//SIZE OF SECTOR FOR EACH STRUCTURE
#define NUM_OF_SECTOR_IMAGE_LOW_RES 4
#define NUM_OF_SECTOR_IMAGE_HIGH_RES 20
#define NUM_OF_SECTOR_IMAGE_STATUS_LOW_RES 4
#define NUM_OF_SECTOR_IMAGE_STATUS_HIGH_RES 4
#define NUM_OF_SECTOR_TEMPERATURE_DATA 8
#define NUM_OF_SECTOR_ACTIVE_CONFIGuRATION 8
#define NUM_OF_SECTOR_PENDING_CONFIGuRATION 8
#define NUM_OF_SECTOR_FOR_CHECK_MEM 4
#define NUM_OF_IMAGE 20
#define SIZE_OF_IMAGE_LOW_RES 262140
#define SIZE_OF_IMAGE_HIGH_RES 1310700
#define LEN_OF_ACTIVE_CONFIG 200



//NOR 
#define NOR_DUMMY_BYTE 0x00
#define NOR_READ_ID_COMMAND 0x9F
#define NOR_READ_STATUS_REGISTER 0X05
#define NOR_WRITE_ENABLE_COMMAND 0x06
#define NOR_WRITE_DISABLE_COMMAND 0x04
#define NOR_WRITE_ENABLE 0x02
#define NOR_PAGE_PROGRAM_COMMAND 0x02
#define NOR_WRITE_STATUS_REGISTER 0x01
#define NOR_ENABLE_4BYTE_ADDRESSING_COMMAND 0XB7
#define NOR_READ_BYTE_COMMAND 0X03
#define NOR_FLASH_INS_RDFSR 0x70
#define NOR_ERASE_SECTOR_4_BYTE 0XDC
#define NOR_ERASE_SUBSECTOR_4_BYTE 0X21
#define Lo(param) ((char *)&param)[0] 
#define Hi(param) ((char *)&param)[1] 
#define Higher(param) ((char *)&param)[2]
#define Highest(param) ((char *)&param)[3]
#define Low(param) ((int *)&param)[0]
#define Top(param) ((int *)&param)[1]
#define MEM_TYPE_MASK 0xFFF000
#define MEM_TYPE_MICRON 0x20B000
#define DISCOVERY_TABLE1        0x0C
#define DTABLE1_FLASH_SIZE        0x04
#define NOR_4_BYTE_PROGRAM 0x12
#define NOR_4_BYTE_READ 0x13
#define LOW_RES 0
#define HIGH_RES 1

//COMMAND LIST
#define NOR_COMMAND 1 
#define CAMERA_COMMAND 2
#define TEMP_SENSOR_COMMAND 3
#define NOR_ACCESS_COMMAND 4
#define COMM_ACCESS 5
#define ICU_COMMAND 6

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

#define HANDSHAKE_ICU 1
#define CLASSIFICATION_ICU 2

//INTERNAL DATA
#define DEFAULT_TEMPERATURE_TIMER_COUNTER 2000
#define DEFAULT_TEMPERATURE_SCHEDULER 0
#define DEFAULT_CAMERA_LOW_RESOLUTION OV5642_640x480
#define DEFAULT_CAMERA_HIGH_RESOLUTION OV5642_1600x1200
#define DEFAULT_CAMERA_QUALITY default_quality
#define DEFAULT_CAMERA_BRIGHTNESS Brightness0
#define DEFAULT_CAMERA_CONTRAST Contrast0
#define DEFAULT_CAM_LIGHT       Auto
#define DEFAULT_CAM_HUE         degree_0
#define DEFAULT_CAM_SATURATION  Saturation0
#define DEFAULT_CAM_SPECIAL     Normal
#define DEFAULT_CAM_EXPOSURE    Exposure_default
#define DEFAULT_CAM_SHARPNESS   Sharpness_auto

#define COMM_SWITCH_PORT 1

uint32_t   mActive_CAM_LOW_RESOLUTION    = 0;
uint32_t   mActive_CAM_HIGH_RESOLUTION   = 0;
uint32_t   mActive_CAM_QUALITY           = 0;
uint32_t   mActive_CAM_BRIGHTNESS        = 0;
uint32_t   mActive_CAM_CONTRAST          = 0;
uint32_t   mActive_CAM_LIGHT             = 0;
uint32_t   mActive_CAM_HUE               = 0;
uint32_t   mActive_CAM_SATURATION        = 0;
uint32_t   mActive_CAM_SPECIAL           = 0;
uint32_t   mActive_CAM_EXPOSURE          = 0;
uint32_t   mActive_CAM_SHARPNESS         = 0;
uint32_t   mActive_TEMPERATURE_TIMER     = 0;
uint32_t   mActive_TEMPERATURE_SCHEDULER = 0;

uint32_t   mPending_CAM_LOW_RESOLUTION    = 0;
uint32_t   mPending_CAM_HIGH_RESOLUTION   = 0;
uint32_t   mPending_CAM_QUALITY           = 0;
uint32_t   mPending_CAM_BRIGHTNESS        = 0;
uint32_t   mPending_CAM_CONTRAST          = 0;
uint32_t   mPending_CAM_LIGHT             = 0;
uint32_t   mPending_CAM_HUE               = 0;
uint32_t   mPending_CAM_SATURATION        = 0;
uint32_t   mPending_CAM_SPECIAL           = 0;
uint32_t   mPending_CAM_EXPOSURE          = 0;
uint32_t   mPending_CAM_SHARPNESS         = 0;
uint32_t   mPending_TEMPERATURE_TIMER     = 0;
uint32_t   mPending_TEMPERATURE_SCHEDULER = 0;

//TEMPERATURE
uint8_t mTemperatureIndex = 0;

//Variabel for Communication
static const uint8_t mSentCommunicationHeader = 126;
static const uint8_t mlenCommand = 13;
uint8_t   mRecvData[mlenCommand];
uint8_t   mSendData[mlenCommand];
uint8_t   CommandHeader       = 0;
uint8_t   CommandSector       = 0;
uint8_t   CommandSubSector    = 0;
uint8_t   CommandValue1[4]    = {0};
uint8_t   CommandValue2[4]    = {0};
uint8_t   CommandArrayCRC[2]  = {0};
uint32_t  CommandCRC          = 0;
uint8_t   mIndexCommand       = 0;
bool      is_header           = false;
bool      mCommunicationState = false; //false=Comm for OBC, true=Comm for ICU

uint16_t  lenDataWire         = 255;
byte      DataWire[255];


ArduCAM CAM(OV5642, CAM_CS);
SPIClass SPI_NOR(&sercom1, 12, 13, 11, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3);

void setup()
{
  SPI.begin();
  SPI_NOR.begin();
  pinMode(NOR_CS, OUTPUT);
  pinMode(selector, OUTPUT);
  pinPeripheral(11, PIO_SERCOM);
  pinPeripheral(12, PIO_SERCOM);
  pinPeripheral(13, PIO_SERCOM);
  Wire.begin(5);
  Serial.begin(115200);
  //Initialize NOR
  digitalWrite(selector, LOW);
  NOR_Init();
  LoadInternalData();  
//  EraseTemperatureDataSector();

  delay(100);
  CAM_INIT();
  Wire.onRequest(requestEvent); 
}

void loop()
{
  /*
  *  TIMER FOR GET TEMPERATURE DATA
  */
//  if(millis() % mActive_TEMPERATURE_TIMER == 0)
//  {
//    if(mActive_TEMPERATURE_SCHEDULER == true)
//    {      
//      GetTemperatureService(false);
//      if(mTemperatureIndex >= 200)
//      {
//        EraseTemperatureDataSector();
//        mTemperatureIndex = 0;                
//      }
//    }
//  }
    
  while(1)
  {
//    char a;
//    if(Serial.available() > 0)
//    {
//      a = Serial.read();
//      if(a == '0')
//      {
//          uint8_t mIndexImageStatus = NOR_ReadImageStatusByResolution(LOW_RES);
//          uint32_t mAddress = 0x00000000;            
//          mAddress = (mIndexImageStatus*(0xFFFF*NUM_OF_SECTOR_IMAGE_LOW_RES))+(NUM_OF_SECTOR_IMAGE_LOW_RES*mIndexImageStatus);
//          CAM_TakePicAndPrintImageData(CAM, mAddress);   
//          bool test = NOR_UpdateImageStatus(LOW_RES, mIndexImageStatus);
//          Serial.println(test);
//      }
//      else if(a == '1')
//      {
//        NOR_ReadAllImageStatus();                                   
//      }
//      else if(a == '2')
//      {
//        uint8_t mNumOfImage = NOR_ReadTotalImageByResolution(LOW_RES);
//        Serial.print("NUM PIC");
//        Serial.println(mNumOfImage);
//        NOR_ReadAllImageLowResolution(1U);
//      }
//      else if(a == '3')
//      {
//        NOR_EraseAll();
//        Serial.println("ERASE DONE!");
//      }
//      else if(a == '4')
//      {
//        CAM_TakePicAndPrintImageData(CAM, 0x00000000);
//      }
//      else if(a == '5')
//      {
//        NOR_GetImageLowRes(0U);
//      }
//    }

    if(mCommunicationState == false)
    {
      Wire.onReceive(receiveEvent);
    }
    else if(mCommunicationState == true)
    {
      while(Serial.available())
      {
        for(uint8_t mCommandByteLen = 0U; mCommandByteLen < mlenCommand; mCommandByteLen++)
        {
          mRecvData[mCommandByteLen] = Serial.read();
        }              

        if(mRecvData[0] == 125U)
        {
//          bool mResult = ProcessReceivedCommand(mRecvData);
//          SendACKData(mResult, mRecvData[1], mRecvData[2]);
        }
      }             
    }    
  }
}

void SendACKData(bool iACKData,uint8_t iCommandSector,uint8_t iCommandSubSector)
{
  ExecuteACKData(iACKData,iCommandSector,iCommandSubSector);        
  //Clear Len Received Command and flushing serial port here !!!
  memset(mRecvData, 0, sizeof(mRecvData));
  mIndexCommand = 0;
}

bool ProcessReceivedCommand(uint8_t* iCommand)
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

  //Deserialize Payload Here
  uint32_t mCommValue1 = DeserializeU32(CommandValue1);
  uint32_t mCommValue2 = DeserializeU32(CommandValue2);
  uint8_t mACKData = false;
  CommandCRC = 0;       

  if(isHeaderOK(CommandHeader))
  {
    if(CommandSector == NOR_COMMAND)
    {
        if(CommandSubSector == CHECK_NOR_HARDWARE_CONNECTION)
        {
          mACKData = NOR_TestConnection();
          SendACKData(mACKData, mRecvData[1], mRecvData[2]);
        }
        else if(CommandSubSector == READ_ALL_IMAGE_STATUS)
        {
          mACKData = NOR_ReadAllImageStatus();                                   
        }
        else if(CommandSubSector == READ_SINGLE_IMAGE)
        {
          mACKData = NOR_GetSingleImageData(mCommValue2, mCommValue1);
        }
        else if(CommandSubSector == READ_MULTIPLE_IMAGE)
        {
          uint8_t mNumOfImage = 0;
          if(mCommValue1 == LOW_RES)
          {
            mNumOfImage = NOR_ReadTotalImageByResolution(LOW_RES);
            mACKData = NOR_ReadAllImageLowResolution(mNumOfImage);
            if(mACKData == true)
            {
              mACKData = NOR_EraseAllImageByImageStatus(LOW_RES);                     
              delay(250);
            }
          }
          else if(mCommValue1 == HIGH_RES)
          {
            mNumOfImage = NOR_ReadTotalImageByResolution(HIGH_RES);
//            mACKData = NOR_ReadAllImageHighResolution(mNumOfImage);   
            if(mACKData == true)
            {
              mACKData = NOR_EraseAllImageByImageStatus(HIGH_RES);       
            }
          }        
        }
        else if(CommandSubSector == READ_TOTAL_IMAGE)
        {
          uint8_t mResolution = mCommValue1;
          mACKData = NOR_ReadTotalImageByResolution(mResolution);
          SendACKData(mACKData, mRecvData[1], mRecvData[2]);   
        }
        else if(CommandSubSector == READ_TEMPERATURE_DATA)
        {
//          mACKData = NOR_ReadTemperatureData();
        }
        else if(CommandSubSector == ERASE_ALL_SECTOR)
        {
//          mACKData = NOR_EraseAll();
          SendACKData(mACKData, mRecvData[1], mRecvData[2]);                      
        }
        else if(CommandSubSector == RELOAD_CONFIGURATION)
        {
          mACKData = CONFIG_RELOAD_CONFIGURATION();
          SendACKData(mACKData, mRecvData[1], mRecvData[2]); 
        }
        else if(CommandSubSector == READ_ALL_CONFIGURATION)
        {
          mACKData = CONFIG_Read_All_Configuration();
        }
        else if(CommandSubSector == SET_ALL_CONFIG_TO_DEFAULT)
        {
          mACKData = setAllConfigToDefault();
          SendACKData(mACKData, mRecvData[1], mRecvData[2]);                    
        }
        else if(CommandSubSector == ERASE_SINGLE_IMAGE_LOW_RES)
        {
          mACKData = NOR_EraseSingleImageLowRes(mCommValue1);
          SendACKData(mACKData, mRecvData[1], mRecvData[2]);                    
        }
        else if(CommandSubSector == ERASE_SINGLE_IMAGE_HIGH_RES)
        {
          mACKData = NOR_EraseSingleImageHighRes(mCommValue1);
          SendACKData(mACKData, mRecvData[1], mRecvData[2]);                    
        }
        else if(CommandSubSector == ERASE_MULTIPLE_IMAGE_LOW_RES)
        {
          mACKData = NOR_EraseMultipleImageLowRes(mCommValue1, mCommValue2);
          SendACKData(mACKData, mRecvData[1], mRecvData[2]);                     
        }
        else if(CommandSubSector == ERASE_MULTIPLE_IMAGE_HIGH_RES)
        {
          mACKData = NOR_EraseMultipleImageHighRes(mCommValue1, mCommValue2);
          SendACKData(mACKData, mRecvData[1], mRecvData[2]);                    
        }
    }
    else if(CommandSector == CAMERA_COMMAND)
    {
      if (CommandSubSector == CAMERA_REINIT)
      {
         mACKData = CAM_INIT();
         memset(DataWire,0,255);
         delay(2);
         SendACKData(mACKData, mRecvData[1], mRecvData[2]);          
      }
      else if(CommandSubSector == CAMERA_TAKE_SINGLE_IMAGE)
      {
        uint8_t mResolution = mCommValue1;
        bool LoadConfig = CAM_Update_All_Config_To_Camera(mResolution);
        if(LoadConfig == true)
        {
          uint8_t mIndexImageStatus = NOR_ReadImageStatusByResolution(mResolution);
          uint32_t mAddress = 0x00000000;            

          if(mIndexImageStatus == 19) /** index image is full **/
          {
            mACKData = 3;
          }
          else
          {
            if(mResolution == LOW_RES)
            {
              mAddress = START_IMAGE_LOW_RES_ADDRESS + (mIndexImageStatus*(0xFFFF*NUM_OF_SECTOR_IMAGE_LOW_RES))+(NUM_OF_SECTOR_IMAGE_LOW_RES*mIndexImageStatus);
            }
            else if(mResolution == HIGH_RES)
            {
              mAddress = START_IMAGE_HIGH_RES_ADDRESS + (mIndexImageStatus*(NUM_OF_SECTOR_IMAGE_HIGH_RES*0xFFFF)) + (NUM_OF_SECTOR_IMAGE_HIGH_RES*mIndexImageStatus);
            }              
            
            mACKData = CAM_TakePicAndPrintImageData(CAM, mAddress);   
            if(mACKData == true)
            {
              mACKData = NOR_UpdateImageStatus(mResolution, mIndexImageStatus);
            }            
          }   
        }
        else
        {
          mACKData = 4;
        }
        SendACKData(mACKData, mRecvData[1], mRecvData[2]);
        Serial.println("Take single pic DONE");
      }        
      else if(CommandSubSector == CAMERA_TAKE_MULTIPLE_IMAGE)
      {
        uint8_t mResolution = mCommValue1;
        uint8_t mNumImage = mCommValue2;

        bool LoadConfig = CAM_Update_All_Config_To_Camera(mResolution);
        if(LoadConfig == true)
        {
          bool ret = true;                     
          uint32_t mAddress = 0x00000000;  
          uint8_t mStartIndex = NOR_ReadImageStatusByResolution(mResolution);
          if(mResolution == LOW_RES)
          {
            for(uint8_t mCount = mStartIndex; mCount < mNumImage; mCount++)
            {
              mAddress = START_IMAGE_LOW_RES_ADDRESS + (mCount*(0xFFFF*NUM_OF_SECTOR_IMAGE_LOW_RES))+(NUM_OF_SECTOR_IMAGE_LOW_RES*mCount);
              ret = CAM_TakePicAndPrintImageData(CAM, mAddress); 
              if(ret == true)
              {
                mACKData = NOR_UpdateImageStatus(mResolution, mCount);                  
              }
            }
          }
          else if(mResolution == HIGH_RES)
          {
            for(uint8_t mCount = mStartIndex; mCount < mNumImage; mCount++)
            {
              mAddress = START_IMAGE_HIGH_RES_ADDRESS + (mCount*(NUM_OF_SECTOR_IMAGE_HIGH_RES*0xFFFF)) + (NUM_OF_SECTOR_IMAGE_HIGH_RES*mCount);
              ret = CAM_TakePicAndPrintImageData(CAM, mAddress); 
              if(ret == true)
              {
                mACKData = NOR_UpdateImageStatus(mResolution, mCount);                  
              }
            }
          }
        }
        else
        {
          mACKData = false;
        }
        SendACKData(mACKData, mRecvData[1], mRecvData[2]); 
      }
      else if(CommandSubSector == UPDATE_SETTING_BRIGHTNESS)
      {
        mACKData = CONFIG_Set_CAM_Brightness(mCommValue1);  
        if(mACKData == true)
        {
            CONFIG_RELOAD_CONFIGURATION();
        }
        SendACKData(mACKData, mRecvData[1], mRecvData[2]);
      }
      else if(CommandSubSector == UPDATE_SETTING_CONTRAST)        
      {
        mACKData = CONFIG_Set_CAM_Contrast(mCommValue1);                    
        if(mACKData == true)
        {
            CONFIG_RELOAD_CONFIGURATION();
        }
        SendACKData(mACKData, mRecvData[1], mRecvData[2]); 
      }
      else if(CommandSubSector == UPDATE_SETTING_LIGHT)           
      {
        mACKData = CONFIG_Set_CAM_Light(mCommValue1);       
        if(mACKData == true)
        {
            CONFIG_RELOAD_CONFIGURATION();
        }
        SendACKData(mACKData, mRecvData[1], mRecvData[2]); 
      }
      else if(CommandSubSector == UPDATE_SETTING_SATURATION)     
      {
        mACKData = CONFIG_Set_CAM_SATURATION(mCommValue1);       
        if(mACKData == true)
        {
            CONFIG_RELOAD_CONFIGURATION();
        }
        SendACKData(mACKData, mRecvData[1], mRecvData[2]); 
      }
      else if(CommandSubSector == UPDATE_SETTING_HUE)             
      {
        mACKData = CONFIG_Set_CAM_HUE(mCommValue1);                       
        if(mACKData == true)
        {
            CONFIG_RELOAD_CONFIGURATION();
        }
        SendACKData(mACKData, mRecvData[1], mRecvData[2]);
      }
      else if(CommandSubSector == UPDATE_SETTING_SPECIAL)         
      {
        mACKData = CONFIG_Set_CAM_SPECIAL(mCommValue1);                       
        if(mACKData == true)
        {
            CONFIG_RELOAD_CONFIGURATION();
        }
        SendACKData(mACKData, mRecvData[1], mRecvData[2]);
      }
      else if(CommandSubSector == UPDATE_SETTING_EXPOSURE)        
      {
        mACKData = CONFIG_Set_CAM_EXPOSURE(mCommValue1);                       
        if(mACKData == true)
        {
            CONFIG_RELOAD_CONFIGURATION();
        }
        SendACKData(mACKData, mRecvData[1], mRecvData[2]); 
      }
      else if(CommandSubSector == UPDATE_SETTING_SHARPNESS)       
      {
        mACKData = CONFIG_Set_CAM_SHARPNESS(mCommValue1);                       
        if(mACKData == true)
        {
            CONFIG_RELOAD_CONFIGURATION();
        }
        SendACKData(mACKData, mRecvData[1], mRecvData[2]);
      }
      else if(CommandSubSector == UPDATE_SETTING_COMPRESS_QUALITY)
      {
        mACKData = CONFIG_Set_CAM_Quality(mCommValue1);                                                              
        if(mACKData == true)
        {
            CONFIG_RELOAD_CONFIGURATION();
        }
        SendACKData(mACKData, mRecvData[1], mRecvData[2]);
      }
      else if(CommandSubSector == UPDATE_SETTING_RESOLUTION)      
      {
        mACKData = CONFIG_Set_CAM_Resolution(mCommValue1, mCommValue2);                                              
        if(mACKData == true)
        {
            CONFIG_RELOAD_CONFIGURATION();
        }
        SendACKData(mACKData, mRecvData[1], mRecvData[2]);
      }
      else if(CommandSubSector == UPDATE_ALL_CONFIG_TO_CAMERA)
      {
        mACKData = CAM_Update_All_Config_To_Camera(LOW_RES);
        SendACKData(mACKData, mRecvData[1], mRecvData[2]); 
      }
    }        
    else if(CommandSector == TEMP_SENSOR_COMMAND)
    {
      if(CommandSubSector == READ_SINGLE_TEMP)
      {
        GetTemperatureService(true); 
        mACKData = true;                      
      }
      else if(CommandSubSector == READ_TEMP_SCHEDULER)
      {
        mACKData = mActive_TEMPERATURE_SCHEDULER;
        SendACKData(mACKData, mRecvData[1], mRecvData[2]);   
      }
      else if(CommandSubSector == READ_TEMP_TIMER)
      {
        uint8_t mTemperatureTimer[4] = {0};
        mTemperatureTimer[0] = mActive_TEMPERATURE_TIMER >> 24 & 0xFF;
        mTemperatureTimer[1] = mActive_TEMPERATURE_TIMER >> 16 & 0xFF;
        mTemperatureTimer[2] = mActive_TEMPERATURE_TIMER >>  8 & 0xFF;
        mTemperatureTimer[3] = mActive_TEMPERATURE_TIMER >>  0 & 0xFF;                                                  
        mACKData = true;
        SendALLOverI2C(mTemperatureTimer, 4,mACKData, mRecvData[1], mRecvData[2]);
      }
      else if(CommandSubSector == READ_TEMP_SECTOR)
      {
        ReadTemperatureSector(true);
        mACKData = true;
      }
      else if(CommandSubSector == UPDATE_TEMP_TIMER)
      {
        mACKData = CONFIG_Set_Temperature_Timer(mCommValue1);
        if(mACKData == true)
        {
            CONFIG_RELOAD_CONFIGURATION();
        }
        SendACKData(mACKData, mRecvData[1], mRecvData[2]);
      }
      else if(CommandSubSector == UPDATE_TEMP_SCHEDULER)
      {
        mACKData = CONFIG_Set_Temperature_Scheduler(mCommValue1); 
        if(mACKData == true)
        {
            CONFIG_RELOAD_CONFIGURATION();
        }
        SendACKData(mACKData, mRecvData[1], mRecvData[2]);
      }
    }
    else if(CommandSector == NOR_ACCESS_COMMAND)
    {
      if(CommandSubSector == REQUEST_NOR_ACCESS_COMMAND)
      {
         mACKData = NOR_Access(mCommValue1);
         SendACKData(mACKData, mRecvData[1], mRecvData[2]);          
      } 
    }
    else if(CommandSector == COMM_ACCESS)
    {
      if(CommandSubSector == COMM_SWITCH_PORT)
      {
        SwitchCommunicationPort(mCommValue1);
        return true;
      } 
    }
  }

  return mACKData;
}

void ExecuteACKData(uint8_t iACKData, uint8_t iCommandSector, uint8_t iCommandSubSector)
{
  uint8_t mReturnData[13] = {0};
  mReturnData[0]    = 126;
  mReturnData[1]    = iCommandSector;
  mReturnData[2]    = iCommandSubSector;
  mReturnData[3]    = 0;
  mReturnData[4]    = 0;
  mReturnData[5]    = 0;
  mReturnData[6]    = iACKData;
  mReturnData[7]    = 0;
  mReturnData[8]    = 0;
  mReturnData[9]    = 0;
  mReturnData[10]   = 0;
  mReturnData[11]   = 0; //CRC
  mReturnData[12]   = 0; //CRC

  lenDataWire = 13;
  memcpy(&DataWire[0],&mReturnData[0],lenDataWire);

  if(mCommunicationState == true)
  {
//    SendDataOverUART0(mReturnData, sizeof(mReturnData));
  }
  else
  {
    Wire.beginTransmission(5);
    Wire.write((byte*)DataWire, lenDataWire); 
    Wire.endTransmission();          
  }
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

void SwitchCommunicationPort(uint8_t iPort)
{
  if(iPort == 1) //I2C Active, Serial Deactive
  {
    mCommunicationState = false;
  }
  else if(iPort == 2)//Serial Active, I2C Deactive
  {
    mCommunicationState = true;
  }
}

bool SendALLOverI2C(uint8_t* iData, uint16_t iLenData, uint8_t iACKData, uint8_t iCommandSector, uint8_t iCommandSubSector)
{
  uint8_t mReturnData[13] = {0};
  mReturnData[0]    = 126;
  mReturnData[1]    = iCommandSector;
  mReturnData[2]    = iCommandSubSector;
  mReturnData[3]    = 0;
  mReturnData[4]    = 0;
  mReturnData[5]    = 0;
  mReturnData[6]    = iACKData;
  mReturnData[7]    = 0;
  mReturnData[8]    = 0;
  mReturnData[9]    = 0;
  mReturnData[10]   = 0;
  mReturnData[11]   = 0; //CRC
  mReturnData[12]   = 0; //CRC
  
  uint8_t mData[255];
  uint16_t AllLen = iLenData + 13;
  
  if(iLenData <= 255)
  {
    lenDataWire = AllLen;
    memcpy(&DataWire[0], &iData[0], iLenData);
    memcpy(&DataWire[iLenData],&mReturnData[0],sizeof(mReturnData));
    Wire.beginTransmission(5);    //Transmit To Device #5
    Wire.write((byte*)DataWire, AllLen);   
    Wire.endTransmission();       //Stop Transmitting
  }

  else if(iLenData > 255)
  {
    lenDataWire = 255;
    uint16_t CalcLoop = iLenData/255;
    uint16_t ModCalcLoop = iLenData%255;
    if(ModCalcLoop == 0)
    {
      Wire.beginTransmission(5);
      for(uint16_t LoopTransmit = 0; LoopTransmit < CalcLoop; LoopTransmit++)
      {
        memcpy(&DataWire[0],&iData[(255*LoopTransmit)],lenDataWire);
        Wire.write((byte*)DataWire,lenDataWire);
      }
      lenDataWire = 13;
      memcpy(&DataWire[0],&mReturnData[0],13);
      Wire.write((byte*)DataWire,lenDataWire);
      Wire.endTransmission();
    }

    else if(ModCalcLoop != 0)
    {
      lenDataWire = 255;
      Wire.beginTransmission(5);
      for(uint16_t LoopTransmit = 0; LoopTransmit < CalcLoop; LoopTransmit++)
      {
        memcpy(&DataWire[0],&iData[(255*LoopTransmit)],lenDataWire);
        Wire.write((byte*)DataWire,lenDataWire);
      }
        lenDataWire = AllLen;
        memcpy(&DataWire[0],&iData[(255*CalcLoop)],ModCalcLoop);
        memcpy(&DataWire[ModCalcLoop],&mReturnData[0],13);
        Wire.write((byte*)DataWire,ModCalcLoop);
        Wire.endTransmission();
    }
  }
  return true;
}

void receiveEvent(int howMany)
{
  if(howMany == 13)
  {
      while(Wire.available())
      {
        for(uint8_t mCount = 0U; mCount < mlenCommand; mCount++)
        {
          mRecvData[mCount] = Wire.read();
        }
    
        if(mRecvData[0] == 125U)
        {
//          SendACKData(mResult, mRecvData[1], mRecvData[2]);
          for(uint8_t mCount = 0U; mCount < mlenCommand; mCount++)
          {
            Serial.println(mRecvData[mCount]);
          }
          bool mResult = ProcessReceivedCommand(mRecvData);
        }
      }
  }  
}

void requestEvent()
{
   Wire.write((byte*)DataWire,lenDataWire);
}

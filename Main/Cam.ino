bool CAM_TakePicAndPrintImageData(ArduCAM CAM, uint32_t iAddress)
{
    bool mRet = false;
    CAM.flush_fifo();
    CAM.clear_fifo_flag();
    CAM.start_capture();
    delay(200);
    while(!CAM.get_bit(ARDUCHIP_TRIG , CAP_DONE_MASK));
    delay(50);
    mRet = CAM_GetData(CAM, iAddress);
    return mRet;
}


bool CAM_INIT()
{
  //SET CS PIN
  pinMode(CAM_CS, OUTPUT);
  digitalWrite(CAM_CS, HIGH);
  CAM.write_reg(0x07, 0x80);
  delay(200);
  CAM.write_reg(0x07, 0x00);
  while(1)
  {
    //Check if the ArduCAM SPI bus is OK
    CAM.write_reg(ARDUCHIP_TEST1, 0x55);
    uint8_t temp = CAM.read_reg(ARDUCHIP_TEST1);
    if (temp != 0x55)
    {
      Serial.println("CAM ERROR");
    }
    else
    {
      Serial.println("CAM OK");
      break;
    }
  }
  delay(500);
  CAM.write_reg(ARDUCHIP_TEST1, 0x55);
  CAM.wrSensorReg16_8(0xff, 0x01);
  CAM.set_format(JPEG);  
  CAM.InitCAM();
  CAM.write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);
  CAM.OV5642_set_JPEG_size(OV5642_640x480);
  CAM.OV5642_set_Compress_quality(high_quality);
//  CAM.OV5642_set_Brightness(Brightness0);       
//  CAM.OV5642_set_Contrast(Contrast0);         
//  CAM.OV5642_set_Light_Mode(Auto);
//  CAM.OV5642_set_Sharpness(Sharpness_auto);        
//  CAM.OV5642_set_hue(degree_0);              
//  CAM.OV5642_set_Color_Saturation(Saturation0);                   
//  CAM.OV5642_set_Special_effects(Normal);          
//  CAM.OV5642_set_Exposure_level(Exposure_default);         

  
  delay(1000);
  CAM.clear_fifo_flag();
  CAM.write_reg(ARDUCHIP_FRAMES, 0x00);
  return true;
}

bool CAM_Update_All_Config_To_Camera(uint8_t i_Res)
{
  pinMode(CAM_CS, OUTPUT);
  digitalWrite(CAM_CS, HIGH);
  CAM.write_reg(0x07, 0x80);
  delay(200);
  CAM.write_reg(0x07, 0x00);
  while(1)
  {
    //Check if the ArduCAM SPI bus is OK
    CAM.write_reg(ARDUCHIP_TEST1, 0x55);
    uint8_t temp = CAM.read_reg(ARDUCHIP_TEST1);
    if (temp != 0x55)
    {
      return false;
    }
    else
    {
      break;
    }
  }
  delay(200);
  CAM.write_reg(ARDUCHIP_TEST1, 0x55);
  CAM.wrSensorReg16_8(0xff, 0x01);
  CAM.set_format(JPEG);  
  CAM.InitCAM();
  CAM.write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);

  uint32_t mResolution = mActive_CAM_LOW_RESOLUTION;
  if(i_Res == LOW_RES)
  {
    mResolution = mActive_CAM_LOW_RESOLUTION;
  }
  else if(i_Res == HIGH_RES)
  {
    mResolution = mActive_CAM_HIGH_RESOLUTION;    
  }
  
  CAM.OV5642_set_JPEG_size(mResolution);  
  CAM.OV5642_set_Compress_quality(high_quality);          
  CAM.OV5642_set_Brightness(mActive_CAM_BRIGHTNESS);       
//  CAM.OV5642_set_Contrast(mActive_CAM_CONTRAST);         
//  CAM.OV5642_set_Sharpness(mActive_CAM_SHARPNESS);        
//  CAM.OV5642_set_Light_Mode(mActive_CAM_LIGHT);            
//  CAM.OV5642_set_hue(mActive_CAM_HUE);              
//  CAM.OV5642_set_Color_Saturation(mActive_CAM_SATURATION);       
//  CAM.OV5642_set_Special_effects(mActive_CAM_SPECIAL);          
//  CAM.OV5642_set_Exposure_level(mActive_CAM_EXPOSURE);         
  
  delay(1000);
  CAM.clear_fifo_flag();
  CAM.write_reg(ARDUCHIP_FRAMES, 0x00);
  return true;
}

bool CAM_GetData(ArduCAM CAM, uint32_t iAddress)
{
  uint8_t temp              = 0; 
  uint8_t temp_last         = 0;
  uint32_t length           = 0;
  uint8_t mImageBuffer[256] = {0};
  uint16_t mPosX            = 0;
  bool mRetData             = false;  
  memset(mImageBuffer, 0, sizeof(mImageBuffer));  
  length = CAM.read_fifo_length();
  if (length >= MAX_FIFO_SIZE){return 0;} /** Take Pic Error !!!**/
  if(length == 0){return 0;} /** Take Pic Error !!!**/
  CAM.CS_LOW();
  CAM.set_fifo_burst(); //Set fifo burst mode
  length --;  

  while(length--)
  {
//    Serial.println("GET PIC");
    temp_last = temp;
    temp =  SPI.transfer(0x00);    

    if (is_header == true)
    {
//      Serial.println(temp);
      if(mPosX < 255)
      {
        mImageBuffer[mPosX] = temp;
        mPosX++;
      }
      else
      {
        //WRITE DATA TO NOR HERE !!!
        mImageBuffer[mPosX] = temp;
        NOR_WriteSector(iAddress, mImageBuffer, 256);
        memset(mImageBuffer, 0, sizeof(mImageBuffer));
        digitalWrite(NOR_CS, HIGH);
        mPosX = 0;
        iAddress += 256U;          
      }
    }
    else if ((temp == 0xD8) & (temp_last == 0xFF))
    {
      is_header = true;
      mImageBuffer[mPosX] = temp_last;
      mPosX++;
      mImageBuffer[mPosX] = temp;
      mPosX++;
//      Serial.println(temp_last);
//      Serial.println(temp);
    }      
    if((temp == 0xD9) && (temp_last == 0xFF)) //If find the end ,break while,
    {
      mRetData = true;
      break;
    }
  }  
  if(mPosX > 0U)
  {
      NOR_WriteSector(iAddress, mImageBuffer, mPosX);          
  }  
  digitalWrite(NOR_CS, HIGH);
  CAM.CS_HIGH();
  is_header = false;
  delay(200);
  return mRetData;
}

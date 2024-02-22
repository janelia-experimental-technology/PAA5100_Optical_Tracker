/*
PAA5100.cpp - Simple example in creating your own Arduino Library

 
*/
 
#include <Arduino.h>
#include "PAA5100_Optical_Tracker.h"



//---------- constructor ----------------------------------------------------

 
PAA5100::PAA5100(int8_t cs, int8_t reset, int8_t motion)
{
 pinMode(cs, OUTPUT);
 digitalWrite(cs, HIGH);
 _csPin = cs;
 pinMode(reset, OUTPUT);
 digitalWrite(reset, HIGH);
 _resetPin = reset;
 pinMode(motion, INPUT);
 _motionPin = motion; 
}


//------------------ private -----------------------------------------------	

SPISettings PAASPI(2000000, MSBFIRST, SPI_MODE3);


void PAA5100::writeReg(uint8_t adr, uint8_t val)
{
  SPI.beginTransaction(PAASPI);
  digitalWrite(_csPin, LOW);
  delayMicroseconds(1);
  
  //  send in the address and value via SPI:
  SPI.transfer(adr | 0x80);
  SPI.transfer(val);
  delayMicroseconds(1);

  digitalWrite(_csPin, HIGH);
  SPI.endTransaction(); 
}

uint8_t PAA5100::readReg(uint8_t adr)
{
  uint8_t inByte = 0;                              // incoming byte from the SPI
  
  SPI.beginTransaction(PAASPI);
  digitalWrite(_csPin, LOW); 
  delayMicroseconds(2);
  
  //  send in the address via SPI:
  SPI.transfer(adr & 0x7f);
  delayMicroseconds(1);
 
  inByte = SPI.transfer(0x00);                 // byte to read:

  delayMicroseconds(2);
  digitalWrite(_csPin, HIGH); 
  SPI.endTransaction();
  
  return(inByte);     
}



void PAA5100::initRegs(void)  // This follows the recommended initialization procedure
{
   writeReg(0x7F, 0x00);
   writeReg(0x55, 0x01);
   writeReg(0x50, 0x07);
   writeReg(0x7f, 0x0e);
   writeReg(0x43, 0x10);

   uint8_t v = readReg(0x67);
   if(v & 0x80)
   {
    writeReg(0x48, 0x04);
   }
   else
   {
    writeReg(0x48, 0x02);
   }
   writeReg(0x7F, 0x00);
   writeReg(0x51, 0x7b);
   writeReg(0x50, 0x00);
   writeReg(0x55, 0x00);

   writeReg(0x7F, 0x0e);
   v = readReg(0x73);
   if (v == 0)
   {
    uint8_t c1 = readReg(0x70);
    if (c1 <= 28)
     c1 = c1 + 14;
    else
     c1 = c1 + 11;
      
    if (c1 > 0x3F)
     c1 = 0x3F;
     
    uint8_t c2 = readReg(0x71);
    c2 = ((unsigned short)c2 * 45)/100;
     
    writeReg(0x7f, 0x00);
    writeReg(0x61, 0xAD);
    writeReg(0x51, 0x70);
    writeReg(0x7f, 0x0e);
    writeReg(0x70, c1);
    writeReg(0x71, c2);
   }
  
   writeReg(0x7F, 0x00);
   writeReg(0x61, 0xAD);
   writeReg(0x7F, 0x03);
   writeReg(0x40, 0x00);
   writeReg(0x7F, 0x05);
   writeReg(0x41, 0xB3);
   writeReg(0x43, 0xF1);
   writeReg(0x45, 0x14);
   writeReg(0x5F, 0x34);
   writeReg(0x7B, 0x08);
   writeReg(0x5e, 0x34);
   writeReg(0x5b, 0x11);
   writeReg(0x6d, 0x11);
   writeReg(0x45, 0x17);
   writeReg(0x70, 0xe5);
   writeReg(0x71, 0xe5);
   writeReg(0x7F, 0x06);
   writeReg(0x44, 0x1B);
   writeReg(0x40, 0xBF);
   writeReg(0x4E, 0x3F);
   writeReg(0x7F, 0x08);
   writeReg(0x66, 0x44);
   writeReg(0x65, 0x20);
   writeReg(0x6a, 0x3a);
   writeReg(0x61, 0x05);
   writeReg(0x62, 0x05);
   writeReg(0x7F, 0x09);
   writeReg(0x4F, 0xAF);
   writeReg(0x5F, 0x40);
   writeReg(0x48, 0x80);
   writeReg(0x49, 0x80);
   writeReg(0x57, 0x77);
   writeReg(0x60, 0x78);
   writeReg(0x61, 0x78);
   writeReg(0x62, 0x08);
   writeReg(0x63, 0x50);
   writeReg(0x7F, 0x0A);
   writeReg(0x45, 0x60);
   writeReg(0x7F, 0x00);
   writeReg(0x4D, 0x11);
   writeReg(0x55, 0x80);
   writeReg(0x74, 0x21);
   writeReg(0x75, 0x1F);
   writeReg(0x4A, 0x78);
   writeReg(0x4B, 0x78);
   writeReg(0x44, 0x08);
   writeReg(0x45, 0x50);
   writeReg(0x64, 0xFF);
   writeReg(0x65, 0x1F);
   writeReg(0x7F, 0x14);
   writeReg(0x65, 0x67);
   writeReg(0x66, 0x08);
   writeReg(0x63, 0x70);
   writeReg(0x6f, 0x1c);
   writeReg(0x7F, 0x15);
   writeReg(0x48, 0x48);
   writeReg(0x7F, 0x07);
   writeReg(0x41, 0x0D);
   writeReg(0x43, 0x14);
   writeReg(0x4B, 0x0E);
   writeReg(0x45, 0x0F);
   writeReg(0x44, 0x42);
   writeReg(0x4C, 0x80);
   writeReg(0x7F, 0x10);
   writeReg(0x5B, 0x02);
   writeReg(0x7F, 0x07);
   writeReg(0x40, 0x41);

   delay(10); // delay 10ms

   writeReg(0x7F, 0x00);
   writeReg(0x32, 0x00);
   writeReg(0x7F, 0x07);
   writeReg(0x40, 0x40);
   writeReg(0x7F, 0x06);
   writeReg(0x68, 0xF0);
   writeReg(0x69, 0x00);
   writeReg(0x7F, 0x0D);
   writeReg(0x48, 0xC0);
   writeReg(0x6F, 0xD5);
   writeReg(0x7F, 0x00);
   writeReg(0x5B, 0xA0);
   writeReg(0x4E, 0xA8);
   writeReg(0x5A, 0x90);
   writeReg(0x40, 0x80);
   writeReg(0x73, 0x1f);
   
   delay(10); // delay 10ms
   
   writeReg(0x73, 0x00);
  
}


void PAA5100::powerUp(void)
{
   digitalWrite(_resetPin, LOW);
   delay(1);
   digitalWrite(_resetPin, HIGH);
   writeReg(0x3a, 0x5a);
   delay(2);
   readReg(0x02);
   readReg(0x03);
   readReg(0x04);
   readReg(0x05);
   readReg(0x06);

   initRegs();
}


// ---------------- public -----------------------------------------------------------
 
boolean PAA5100::begin(void)
{
	powerUp();
	
	return( readReg(0x00) == 0x49 ) ;

}

uint8_t PAA5100::productID(void)
{
	return( readReg(0x00));
}	
	
uint8_t  PAA5100::revisionID(void)
{
	return( readReg(0x01) );
}

uint8_t PAA5100::readRegister(uint8_t reg)
{
	return( readReg(reg));
}	
	

// boolean PAA5100::isMotion(void)
// { 
	// return ( (motionData.motion = readReg(MOTION)) & 0x80 );
// }

boolean PAA5100::goodData(void)
{ 
	return ( motionData.isGood);
}

boolean PAA5100::readMotion(void)
{
	SPI.beginTransaction(PAASPI);
	digitalWrite(_csPin, LOW);
	delayMicroseconds(2);
  
    SPI.transfer(0x16);
	delayMicroseconds(1);
	motionData.motion = SPI.transfer(0x00);
	if( motionData.motion & 0x80 )
   	{	
		motionData.observation = SPI.transfer(0x00);  
		motionData.deltaX.c[0] = SPI.transfer(0x00);
		motionData.deltaX.c[1] = SPI.transfer(0x00);
		motionData.deltaY.c[0] = SPI.transfer(0x00);
		motionData.deltaY.c[1] = SPI.transfer(0x00);
		motionData.squal = SPI.transfer(0x00);
		motionData.rd_sum = SPI.transfer(0x00);
		motionData.rd_max = SPI.transfer(0x00);
		motionData.rd_min = SPI.transfer(0x00); 
		motionData.shutter.c[1] = SPI.transfer(0x00);  // must read high first
		motionData.shutter.c[0] = SPI.transfer(0x00);    
		if( (motionData.squal < 0x19) && (motionData.shutter.c[1] == 0x1f) )
			motionData.isGood = false;
		else
			motionData.isGood = true; 
	}
	else
	{
		motionData.isGood = false;
	}	
    digitalWrite(_csPin, HIGH);
	SPI.endTransaction();
	
	return( motionData.isGood);
}

int16_t PAA5100::deltaX(void)
{
	return(motionData.deltaX.si);
}

int16_t PAA5100::deltaY(void)
{
	return(motionData.deltaY.si);
}

int16_t PAA5100::squal(void)
{
	return(motionData.squal);
}

int8_t PAA5100::frameCapture(uint8_t frame_data[][ARRAY_COL])
{
int i, j;
int retry = 0; 
unsigned char val;

   writeReg(0x7F, 0x07);
   writeReg(0x4C, 0x00);
   writeReg(0x7F, 0x08);
   writeReg(0x6A, 0x38);
   writeReg(0x7F, 0x00);
   writeReg(0x55, 0x04);
   writeReg(0x40, 0x80);
   writeReg(0x4D, 0x11);
   
   writeReg(0x7F, 0x00);
   writeReg(0x58, 0xff);
  
  // Read register 0x59 until bit 6 & 7 = '1'
  retry = 0;
  while (retry < READ_MAX_CYCLE)
  {
    if( (readReg(0x59) & 0xc0) == 0xc0 )
    {
      break;
    }
    delayMicroseconds(2);
    retry++;
  }
  if (retry >= READ_MAX_CYCLE) 
  {
    // Error reading back 0x59
    return readReg(0x58);
  }

 // Read row by row
  for (i = 0; i < ARRAY_ROW; i++)
  {
    // Read col by col for each row
    for (j = 0; j < ARRAY_COL; j++)
    {
      // Get upper 6 bits
      retry = 0;
      while (retry < READ_MAX_CYCLE)
      {
        val = readReg(0x58);
        if (val & 0x40)
        {
          frame_data[i][j] = (val << 2) & 0xFC;
          break;
        }
        delayMicroseconds(2);
        retry++;
      }
      if (retry >= READ_MAX_CYCLE) 
      {
        // Error reading back 0x58 for upper 6 bits
        return -2;
      }

      // Get lower 2 bits
      retry = 0;
      while (retry < READ_MAX_CYCLE)
      {
        val = readReg(0x58);
        if (val & 0x80)
        {
          frame_data[i][j] |= ((val >> 2) & 0x03);
          break;
        }
        delayMicroseconds(2);
        retry++;
      }
      if (retry >= READ_MAX_CYCLE) 
      {
        // Error reading back 0x58 for lower 2 bits
        return -3;
      }
    }
  }
  
  return 0;
}


/* 	motionData.motion = SPI.transfer(MOTION);
	if( motionData.motion & 0x80 )
   	{	
		motionData.observation = SPI.transfer(0x00);  
		motionData.deltaX.c[0] = SPI.transfer(0x00);
		motionData.deltaX.c[1] = SPI.transfer(DELTA_X_H);
		motionData.deltaY.c[0] = SPI.transfer(DELTA_Y_L);
		motionData.deltaY.c[1] = SPI.transfer(DELTA_Y_H);
		motionData.squal = SPI.transfer(SQUAL);
		motionData.rd_sum = SPI.transfer(RAWDATA_SUM);
		motionData.rd_max = SPI.transfer(RAWDATA_MIN);
		motionData.rd_min = SPI.transfer(RAWDATA_MAX); 
		motionData.shutter.c[1] = SPI.transfer(SHUTTER_H);  // must read high first
		motionData.shutter.c[0] = SPI.transfer(SHUTTER_L);    
		if( (motionData.squal < 0x19) && (motionData.shutter.c[1] == 0x1f) )
			motionData.isGood = false;
		else
			motionData.isGood = true;  */




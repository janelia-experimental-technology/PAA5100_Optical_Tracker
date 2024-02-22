/*
 PAA5100.h - PixArt PAA5100 Optical Tracker Chip Interface
 
 20210530 sws jET Janelia HHMI
 
 Sets up and comunicates with a PixArt Optical Tracking chip over SPI
 
 Methods:
 
 PAA5100(cs, reset, motion - Constructor. Specify pins for chip select, HW reset, and motion detect
 begin() - Init SPI and PAA5100 registers
 
Janelia Open-Source Software (3-clause BSD License)

Copyright 2022 Howard Hughes Medical Institute

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE

*/
 
#ifndef PAA5100_h
#define PAA5100_h
 
#include <Arduino.h>
#include <SPI.h>
 
class PAA5100
{
public:
 // Array define for frame capture
    #define ARRAY_COL        35
    #define ARRAY_ROW        35
	
	PAA5100(int8_t c, int8_t r, int8_t m);
	
	boolean begin(void); //uint8_t cs, uint8_t reset, uint8_t motion);  //Constructor. attach pins and init them
	uint8_t productID(void);
	uint8_t revisionID(void);
	uint8_t readRegister(uint8_t reg);
	boolean readMotion(void);
    int8_t frameCapture(uint8_t frame_data[][ARRAY_COL]);
	int16_t deltaX(void);
	int16_t deltaY(void);
	int16_t squal(void);
	boolean goodData(void);
	 
private:

#define READ_MAX_CYCLE  2000    // Maximum cycles of read before error
	
	int _csPin;
	int _resetPin;
	int _motionPin;
	
	void powerUp(void);
	void writeReg(uint8_t adr, uint8_t val);
	uint8_t readReg(uint8_t adr);
	void initRegs(void);	
	
	// Union for 16-bit value
	union n16Bit
	{
		unsigned char c[2];    // c[0] = LO byte, c[1] = HI byte
		unsigned short i;
		signed short si;
	};

	struct
	{
		unsigned char motion;        // BYTE 0
		unsigned char observation;   // BYTE 1
		union n16Bit deltaX;         // BYTE 2 & 3
		union n16Bit deltaY;         // BYTE 4 & 5
		unsigned char squal;         // BYTE 6
		unsigned char rd_sum;        // BYTE 7
		unsigned char rd_max;        // BYTE 8
		unsigned char rd_min;        // BYTE 9
		union n16Bit shutter;        // BYTE 10 & 11  // this need to be reversed after reading in byte form, as the first read byte is upper
		boolean  isGood; 			 // is data good?
	} motionData;


};
 
#endif
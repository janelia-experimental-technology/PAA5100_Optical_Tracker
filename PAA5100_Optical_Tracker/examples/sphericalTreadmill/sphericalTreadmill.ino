// VERSION 20230106 dks
// Universal 'guest' and 'host' - host becomes effective after any Serial USB input

#include <SPI.h>
#include <PAA5100_Optical_Tracker.h>

#define motion1Pin 9
#define testPin 2

PAA5100 cam1(10, 14, 9);

IntervalTimer frameTimer;
#define FRAMECLK 24 // 1000 //250     // usec per frame

bool host;
static int inState = 0;

volatile int32_t Xcounts = 0; 
volatile int32_t Ycounts = 0;
volatile int32_t X1counts = 0; 
volatile int32_t Y1counts = 0;

int32_t Xcnts;
int32_t Ycnts;

volatile int motionFlag = 0;

bool video = false;
bool firstFrame = false;
bool sampling = false;

// PACKET STRUCTURE
// byte use
//  0 - 0x00 - signifies packet start
//  1 - sample count
//  2 - X0
//  3 - Y0
//  4 - X1
//  5 - Y1

volatile uint8_t samples = 1;
uint8_t outStream[] = {0, 1, 128, 128, 128, 128};

void motion1Int()
{
   if( cam1.readMotion() )
   {
      Xcounts += (int32_t) cam1.deltaX();  
      Ycounts += (int32_t) cam1.deltaY();
      motionFlag += 1; 
   }  
}

void isHost()  // Am I connected to USB (host) or am I controlled by a host
{
   if (Serial.available()){
   host = true;  
   }
   else{
   host = false;
   }
}

void frameSend()
{ 
   if (host)
   {
     outStream[0] = 0;
    
     if(samples == 0) samples++;   // running count of packets
     outStream[1] = samples++; 

     noInterrupts();
     outStream[2] = 128 + (int8_t)Xcounts; //motionData.deltaX.c[0];
     outStream[3] = 128 + (int8_t)Ycounts; //motionData.deltaY.c[0];
     outStream[4] = 128 + (int8_t)X1counts;
     outStream[5] = 128 + (int8_t)Y1counts;

     Xcounts = 0;
     Ycounts = 0;
     X1counts = 0;
     Y1counts = 0;    
     interrupts();

     Serial.write(outStream, 6); 
   } 
}

void setup()
{
   Serial.begin(1250000);
   Serial1.begin(1000000);

   pinMode(testPin, OUTPUT);
   digitalWrite(testPin, LOW);

   SPI.begin();

   if( !cam1.begin() ) 
   { 
      Serial.println("cam1 error");
      digitalWriteFast(testPin, HIGH); 
   }
   
   attachInterrupt(motion1Pin, motion1Int, FALLING);
   
}

void loop() 
{
   if ( host && Serial1.available() )  // check for incoming if host
   {
      int8_t dataIn = Serial1.read();
      switch (inState)
      {
         case 0:  // waiting
            if( dataIn == 0 ) 
              inState = 1;
            break;  
         case 1:  // X
            if( dataIn > 0)
            { 
              X1counts += (dataIn - 128);
              inState = 2;
            }  
            if( dataIn < 0)
            { 
              X1counts += (dataIn + 128);
              inState = 2;
            }  
            break;
         case 2:  // Y
            if( dataIn == 0 )
            {
               inState = 1;
            }
            if (dataIn > 0)
            {
              Y1counts += (dataIn - 128);
              inState = 1;
            }  
            if (dataIn < 0)
            {
              Y1counts += (dataIn + 128);
              inState = 2;
            }
            break;  
         default:
            inState = 0; 
      }                      
   }

   if( !host && (motionFlag > 0) ) // motion detected in guest module
   {
    digitalWriteFast(testPin, HIGH); 

      noInterrupts();
      int32_t Xcnts = Xcounts;
      int32_t Ycnts = Ycounts;
      Xcounts = 0;
      Ycounts = 0;   
      motionFlag = 0;       
      interrupts();   
      
      constrain( Xcnts, -127, 127);
      constrain( Ycnts, -127, 127);
      Serial1.write( 0); 
      Serial1.write( (uint8_t)(Xcnts + 128) );
      Serial1.write( (uint8_t)(Ycnts + 128) );
      
    digitalWriteFast(testPin, LOW); 
   }
               
   if (Serial.available())
   {
      uint8_t cmdin = Serial.read();
      host = true;   // any command over USB tells us it's connected
      switch(cmdin)  // we will just process everything - only 254 and 255 matter, parameters (0,1) will be processed as commands and ignored
      {
       case 0: 
          break;
       case 1: 
          break;
       case 246:               // Data return state.  0 = rotation coordinates, 1 = raw camera x/y
          break;
       case 250:               // stop video
       case 'f':
          video = false;
          break;
       case 251:               // start video
       case 'v':
          firstFrame = true;   // need extra register accesses the first time
          video = true;
          break;   
       case 254:               //'e': // //Stop Data Acquisition
       case 'e':
          frameTimer.end();       
          sampling = false;
          samples = 1;        
          break;
       case 255:               // 'b': // //Start Data Acquisition  
       case 'b':      
          frameTimer.begin(frameSend, FRAMECLK); 
          sampling = true;   
          break;
       default:
          break;   
      }
   }    
}
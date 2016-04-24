#include "pitches.h"            //the definition of the notes in Hz.

#define SENSOR_PIN 0            //analog pin 0
#define SENSOR_THRESHOLD 55     //sound level level which triggers a melody, you may need to adjust this to suit your piezo

//this melody is play when the Arduino is started to let you know the speaker is connected.
int startMelody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

//this is the tone that is played to mess with peoples heads
//high tones work best as they fill spaces better making it 
//harder to locate the source
int worryTone[] = {
   NOTE_AS7, NOTE_B7, NOTE_C8, NOTE_AS7, NOTE_B7, NOTE_C8 
};

                                          //All times are in milliseconds!!!
int           sensorLevel  = 0;           //This holds the current value of the sound level detected by the piezo sensor
unsigned long nextRun = 0;                //How many millisends until the next random instance of the tone occurs. Has to be unsigned long as that is what millis() returns
long          intensity = 60000;          //Defines how often a random tone occurs. This can be from current time till intensity * 2.
                                          //e.g. If intensity is 60000 (1 min) the next random tone will happenen between current time and two minutes time. 
long          startDelay  = 10000;        //How long must pass before random tones start. This gives you a chance to hide the device.
long          shockRepeatDelay = 10000;   //How long between shock/sound triggers.
unsigned long lastShockTime = 0;    

void setup() {
   // put your setup code here, to run once:
   Serial.begin(9600);
   Serial.println("Hello world");
   play_tone(startMelody,  8);
   nextRun = startDelay;
   delay(2000);// Give reader a chance to see the output.
}

void loop() {  
   sensorLevel  = analogRead(SENSOR_PIN); 
   unsigned long milli = millis();
   if ((sensorLevel > SENSOR_THRESHOLD)
        && (milli > 5000) 
        && (milli >= (lastShockTime + shockRepeatDelay)))  {
      play_tone(worryTone,  6);
      lastShockTime = milli;
      return;  //no need to continue, let loop coninue;
   } 

   if(milli >= nextRun && milli >= startDelay)
   {
      nextRun = random((milli + 10000), (intensity *2));
      play_tone(worryTone, 6);
   }
}

void play_tone(int melody[], int melodySize)
{
    for (int thisNote = 0; thisNote < melodySize; thisNote++) 
    {   
      tone(8, melody[thisNote]);  
      delay(300); //the length of each note
      noTone(8);
    }    
}


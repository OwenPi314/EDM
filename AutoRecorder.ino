#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>

#define SD_ChipSelectPin 10  

//Declare variables
TMRpcm audio;
boolean isRecording; //whether the device is recording
int counter;         //how many files have been recorded (used for file naming)
char * fileName;     //the file to record to 
int lastCheck = 0;   //when was the last check for if there is sound
boolean sound;       //whether there is sound

void setup() 
{
  Serial.begin(115200);

  //Initialize variables
  isRecording = false;
  counter = 0;
  fileName = "";
  sound = false;
  
  //Check SD card
  if (!SD.begin(SD_ChipSelectPin)) {  
    Serial.println("SD Fail");
    return;
  }else{
    Serial.println("SD OK"); 
  }
  audio.CSPin = SD_ChipSelectPin;

  //Wait for setup (maybe unnecessary)
  delay(1000);
}

void loop() 
{
  if(!isRecording) //constantly check if a sentence has started when not recording
  {
    lastCheck = millis();
    sound = soundExist();
  }
  else //during recording, constantly check if there has been no sound recorded for a second
  {
    sound = false;
    while(millis() - lastCheck < 1000)
    {
      sound = sound || soundExist();
    }
    lastCheck = millis();
  }

  if(sound) //If there is sound and recording hasn't started, start recording
  {
    if(!isRecording)
    {
      startRecord();
    }
  }
  else //if there is no sound and the device is recording, stop recording
  {
    if(isRecording)
    {     
      stopRecord();
    }
  }
}

//function to check whether there is input to the digital audio sensor
boolean soundExist()
{
  return (digitalRead(4) == 0);
}

//this function starts the recording
void startRecord()
{
  sprintf(fileName, "%i%s", counter, "audio.wav");
  Serial.println("start");
  isRecording = true;
  audio.startRecording(fileName, 16000, A0);
}

//this function stops the recording
void stopRecord()
{
  Serial.println("stop");

  isRecording = false;
  counter++;
  audio.stopRecording(fileName);
}

#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>

#define SD_ChipSelectPin 10  // 如果使用 arduino nano 328 可使用Pin4

TMRpcm audio;   
boolean isRecording;
int counter;
char * fileName;
int lastCheck = 0;
boolean sound = false;

void setup() {
  
  Serial.begin(115200);
  
  isRecording = false;
  counter = 0;
  fileName = "";
  if (!SD.begin(SD_ChipSelectPin)) {  
    Serial.println("SD Fail");
    return;
  }else{
    Serial.println("SD OK"); 
  }
  audio.CSPin = SD_ChipSelectPin;
  delay(1000);
}




void loop() {
  if(sound == false)
  {
    lastCheck = millis();
    sound = soundExist();
  }
  else if(millis() - lastCheck > 1000)
  {
    lastCheck = millis();
    sound = soundExist();
  }

  if(sound)
  {
    if(!isRecording)
    {
      startRecord();
    }
  }
  else
  {
    if(isRecording)
    {      
      stopRecord();
    }
  }
}






boolean soundExist()
{
  boolean temp = false;

  for(int i = 0; i < 500; i++)
  {
    boolean input = digitalRead(4) == 1 ? false : true;
    
    temp = (temp || input);
    if(temp == true)
    {
      return temp;
    }
  }
  return temp;
}


void startRecord()
{
  sprintf(fileName, "%i%s", counter, "audio.wav");
  Serial.print("start ");
  Serial.println(fileName);
  isRecording = true;
  audio.startRecording(fileName, 16000, A0);
}

void stopRecord()
{
  Serial.print("stop ");
  Serial.println(fileName);
  isRecording = false;
  counter++;
  audio.stopRecording(fileName);
}

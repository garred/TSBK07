// Simple demonstration of the sound player CallMeAL.
// Variant that can play sounds on command. Type a number 1-4
// followed by return. "q" to stop.

#include "CallMeAL.h"
// For Mac:
// uses framework Carbon
// uses framework OpenAL

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
  // Init with 2 channels
  if (!InitCallMeAL(2))
  {
    printf("FAILED TO INIT OPEN AL\n");
    return 0;
  }
  
  ALuint buffer1 = LoadSound("sounds/Win.wav");
  ALuint buffer2 = LoadSound("sounds/Blip.wav");
  ALuint buffer3 = LoadSound("sounds/toff16.wav");
  ALuint buffer4 = LoadSound("sounds/Lose.wav");
  if (buffer1)
  {
    char buf[20];
    buf[0] = 0;
    fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);    
    while (buf[0] != 'q')
    {
       int numRead = read(0,buf,10);
       if(numRead > 0)
       {
           buf[numRead] = 0;
           if (buf[0] == '1')
           		PlaySoundInChannel(buffer1, 0);
           if (buf[0] == '2')
           		PlaySound(buffer2);
           if (buf[0] == '3')
           		PlaySound(buffer3);
           if (buf[0] == '4')
           		PlaySound(buffer4);
           if (buf[0] == 'q') return 0;
       }
       usleep(100);
    }
    
    alDeleteBuffers(1,&buffer1);
    alDeleteBuffers(1,&buffer2);
    alDeleteBuffers(1,&buffer3);
    alDeleteBuffers(1,&buffer4);
    printf("ALL DONE!\n");
  }
  else
    printf("FAILED TO CREATE BUFFER!\n");
  
  // Clean up
  HaltCallMeAL();
  
  return 0;
}

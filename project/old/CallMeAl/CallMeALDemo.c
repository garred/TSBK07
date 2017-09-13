// Simple demonstration of the sound player CallMeAL.

#include "CallMeAL.h"
// uses framework Carbon
// uses framework OpenAL

int main()
{
  // Init with 2 channels
  if (!InitCallMeAL(2))
  {
    printf("FAILED TO INIT OPEN AL\n");
    return 0;
  }

  ALuint buffer = LoadSound("sounds/piska.wav");
//  ALuint buffer = LoadSound("sounds/Blip.wav");
//  ALuint buffer = LoadSound("sounds/toff16.wav");
//  ALuint buffer = LoadSound("sounds/Lose.wav");
  if (buffer)
  {
    PlaySoundInChannel(buffer, 0);
    // Play until finished
    do
    {
       //usleep(100); // or do other tasks while it plays
    }
    while (ChannelIsPlaying(0));

    alDeleteBuffers(1,&buffer);
    printf("ALL DONE!\n");
  }
  else
    printf("FAILED TO CREATE BUFFER!\n");

  // Clean up
  HaltCallMeAL();

  return 0;
}

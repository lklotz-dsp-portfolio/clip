// Source - https://stackoverflow.com/a/76077391
// Posted by EmilioPeJu, modified by community. See post 'Timeline' for change history
// Retrieved 2026-04-07, License - CC BY-SA 4.0

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <portaudio.h>

/*
    *
    * Most of this code is from 
    *     https://stackoverflow.com/questions/76059971/how-do-i-write-a-wav-file-in-c
    * I just converted it to fit the requirements of this assignment and added some comments
    * 
*/

typedef struct
{
    float *sine;
    int phaseIndex;
} paTestData;

//generate a mono sinewave and write it to a wav file. The wav file is then read and played back using PortAudio.
static int PaCallback(const void *inputBuffer, void *outputBuffer,
                      unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo* timeInfo,
                      PaStreamCallbackFlags statusFlags,
                      void *userData)
{
    paTestData *data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    unsigned int i;
    (void) inputBuffer;
    for (i=0; i<framesPerBuffer; i++)
    {
        //*out++ = data->sine[data->index]; 
        //generate a sine wave on the fly
        *out++ = (sinf(2.0f * M_PI * 440.0f * data->phaseIndex/ 48000.0f) * 0.1f);; 

        if (data->phaseIndex >= 48000) // reset phase index after 1 second
            data->phaseIndex = 0;
        data->phaseIndex++;
    }
    return 0;
}

static paTestData data;

int main()
{
    PaStream *stream;
    PaError err;

    err = Pa_Initialize();

    /* Open an audio I/O stream. */
    err = Pa_OpenDefaultStream( &stream,
                      0,          /* no input channels */
                      1,          /* mono output */
                      paFloat32,  /* 32 bit floating point output */
                      48000,
                      256,        /* frames per buffer */
                      PaCallback,
                      &data );
  
    err = Pa_StartStream( stream );
  
    /* Sleep for several seconds. */
    Pa_Sleep(1000);
  
    err = Pa_StopStream( stream );

    return 0;
}

//compile with: gcc clip.c -o clip -lm
// -lm is needed to link the math library, which contains the sinf function used to generate the sine wave. 

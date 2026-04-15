// Source - https://stackoverflow.com/a/76077391
// Posted by EmilioPeJu, modified by community. See post 'Timeline' for change history
// Retrieved 2026-04-07, License - CC BY-SA 4.0

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include </opt/homebrew/Cellar/portaudio/19.7.0/include/portaudio.h>

# define SAMPLERATE 48000 
# define AMP 0.50f
# define FREQ 440.0f
# define DURATION 1
# define FRAMES DURATION * SAMPLERATE
# define CHANNELS 1
# define BITS 16
# define LEN FRAMES*CHANNELS*BITS / 8
# define SYNC 0.50
# define MULT 32767 * SYNC


typedef struct
{
    float *sine;
    int phaseIndex;
} paTestData;

void write_sine(void);
void write_sine_clipped(void);

//play a clipped sin wave 
static int PaCallback(const void *inputBuffer, void *outputBuffer,
                      unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo* timeInfo,
                      PaStreamCallbackFlags statusFlags,
                      void *userData)
{
    paTestData *data = (paTestData*)userData;
    int16_t *out = (int16_t*)outputBuffer;
    unsigned int i;
    (void) inputBuffer;
    for (i=0; i<framesPerBuffer; i++)
    {
        //*out++ = data->sine[data->index]; 
        //generate a sine wave on the fly
        int16_t sample = (int16_t)(sinf(2.0f * M_PI * FREQ * data->phaseIndex/ SAMPLERATE) * MULT);
 
        //clip
        if (sample >= 8196) sample = 8196;
        if (sample <= -8196) sample = -8196;

        *out++ = sample;

        if (data->phaseIndex >= SAMPLERATE) // reset phase index after 1 second
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
                      paInt16,  
                      SAMPLERATE,
                      256,        /* frames per buffer */
                      PaCallback,
                      &data );
  
    err = Pa_StartStream( stream );
  
    /* Sleep for several seconds. */
    Pa_Sleep(1000);
  
    err = Pa_StopStream( stream );

    write_sine();
    write_sine_clipped();

    return 0;
}

//generate 1 second sine and write to sine.wav
void write_sine(void){

    int16_t channels = CHANNELS;
    int32_t sampleRate = SAMPLERATE;
    int16_t bitsPerSample = BITS;
    int32_t len = FRAMES*CHANNELS*BITS / 8;
    int16_t byte;


    int16_t *channel = (int16_t *) malloc(FRAMES * sizeof(int16_t));
    for (int i=0; i < FRAMES; i++) {
        // generate a sine wave
        channel[i] = (int16_t)(sinf(2.0 * M_PI * FREQ * data.phaseIndex / SAMPLERATE ) * MULT);

        if(data.phaseIndex >= SAMPLERATE) data.phaseIndex = 0;
        data.phaseIndex++;
   }

    // Writes data to wav file
    FILE *fp = fopen("sine.wav", "wb"); // Open the file in binary mode for writing
    //
    //// WAVE Header Data
    fwrite("RIFF", 1, 4, fp);
    int32_t chunk_size = LEN + 36; 
    fwrite(&chunk_size, 4, 1, fp);
    fwrite("WAVE", 1, 4, fp);
    fwrite("fmt ", 1, 4, fp);
    int32_t subchunk1_size = 16;
    fwrite(&subchunk1_size, 4, 1, fp);
    int16_t fmt_type = 1;  // 1 = PCM
    fwrite(&fmt_type, 2, 1, fp);
    fwrite(&channels, 2, 1, fp);
    fwrite(&sampleRate, 4, 1, fp);
    // (Sample Rate * BitsPerSample * Channels) / 8
    int32_t byte_rate = SAMPLERATE * BITS * CHANNELS / 8;
    fwrite(&byte_rate, 4, 1, fp);
    int16_t block_align = CHANNELS * BITS / 8;
    fwrite(&block_align, 2, 1, fp);
    fwrite(&bitsPerSample, 2, 1, fp);

    // Marks the start of the data
    fwrite("data", 1, 4, fp);
    fwrite(&len, 4, 1, fp);  // Data size
    
    for (int i = 0; i < FRAMES; i++)
    {
        byte = channel[i];
        fwrite(&byte, 2, 1, fp);
    }

    fclose(fp);
    free(channel); 
}

//generate 1 second clipped sine and write to clipped.wav
void write_sine_clipped(void){

    int16_t channels = CHANNELS;
    int32_t sampleRate = SAMPLERATE;
    int16_t bitsPerSample = BITS;
    int32_t len = FRAMES*CHANNELS*BITS / 8;
    int16_t byte;


    int16_t *channel = (int16_t *) malloc(FRAMES * sizeof(int16_t));
    for (int i=0; i < FRAMES; i++) {
        // generate a sine wave
        channel[i] = (int16_t)(sinf(2.0 * M_PI * FREQ * data.phaseIndex / SAMPLERATE ) * MULT);

        if(channel[i] >= 8196) channel[i] = 8196;
        if(channel[i] <= -8186) channel[i] = -8196;

        if(data.phaseIndex >= SAMPLERATE) data.phaseIndex = 0;
        data.phaseIndex++;
   }

    // Writes data to wav file
    FILE *fp = fopen("clipped.wav", "wb"); // Open the file in binary mode for writing
    //
    //// WAVE Header Data
    fwrite("RIFF", 1, 4, fp);
    int32_t chunk_size = LEN + 36; 
    fwrite(&chunk_size, 4, 1, fp);
    fwrite("WAVE", 1, 4, fp);
    fwrite("fmt ", 1, 4, fp);
    int32_t subchunk1_size = 16;
    fwrite(&subchunk1_size, 4, 1, fp);
    int16_t fmt_type = 1;  // 1 = PCM
    fwrite(&fmt_type, 2, 1, fp);
    fwrite(&channels, 2, 1, fp);
    fwrite(&sampleRate, 4, 1, fp);
    // (Sample Rate * BitsPerSample * Channels) / 8
    int32_t byte_rate = SAMPLERATE * BITS * CHANNELS / 8;
    fwrite(&byte_rate, 4, 1, fp);
    int16_t block_align = CHANNELS * BITS / 8;
    fwrite(&block_align, 2, 1, fp);
    fwrite(&bitsPerSample, 2, 1, fp);

    // Marks the start of the data
    fwrite("data", 1, 4, fp);
    fwrite(&len, 4, 1, fp);  // Data size
    
    for (int i = 0; i < FRAMES; i++)
    {
        byte = channel[i];
        fwrite(&byte, 2, 1, fp);
    }

    fclose(fp);
    free(channel); 
}



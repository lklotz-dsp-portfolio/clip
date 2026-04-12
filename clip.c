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



int main()
{
    int32_t duration = 1; // Duration of the audio in seconds
    int32_t rate = 48000;  // Sample rate
    int32_t frame_count = duration * rate;
    int16_t chan_num = 1;  // mono
    int16_t bits = 16;  // Bit depth
    int32_t length = frame_count*chan_num*bits / 8; 
    int16_t byte;

    float sync_level = 0.25;
    float multiplier = 32767 / sync_level;

    float pitch = 440.0;

    int isHighNote = 1;

    int16_t *channel = (int16_t *) malloc(frame_count * sizeof(int16_t));
    for (int i=0; i < frame_count; i++) {

        /*
        //alternate pitch
        if((i % 2000) == 0){
           if(isHighNote == 0) {
                pitch = pitch / 2; 
                isHighNote = 1;
           }
           else {
                pitch *= 2; 
                isHighNote = 0;
           }
        }
        */

        // generate a sine wave
        channel[i] = (int16_t)(sync_level * sinf(2.0 * 3.14159265 * pitch * i / rate)*multiplier);
        channel[i] = channel[i] / 2;

        /*
        //adsr (decay)
        if (multiplier > 0){
            multiplier -= 1;
            printf("Multiplier: %fl\n", multiplier);
        }

        */
        /* 
        //clip
        if (channel[i] > 8192) {
            channel[i] = 8192;
        } else if (channel[i] < -8192) {
            channel[i] = -8192;
        }
        */
    }

    // Writes data to wav file
    FILE *fp = fopen("sine.wav", "wb"); // Open the file in binary mode for writing
    if (fp == NULL)
    {
        printf("Output file couldn't be opened!\n");
        return 4;
    }

    //// WAVE Header Data
    fwrite("RIFF", 1, 4, fp);
    int32_t chunk_size = length + 36; 
    fwrite(&chunk_size, 4, 1, fp);
    fwrite("WAVE", 1, 4, fp);
    fwrite("fmt ", 1, 4, fp);
    int32_t subchunk1_size = 16;
    fwrite(&subchunk1_size, 4, 1, fp);
    int16_t fmt_type = 1;  // 1 = PCM
    fwrite(&fmt_type, 2, 1, fp);
    fwrite(&chan_num, 2, 1, fp);
    fwrite(&rate, 4, 1, fp);
    // (Sample Rate * BitsPerSample * Channels) / 8
    int32_t byte_rate = rate * bits * chan_num / 8;
    fwrite(&byte_rate, 4, 1, fp);
    int16_t block_align = chan_num * bits / 8;
    fwrite(&block_align, 2, 1, fp);
    fwrite(&bits, 2, 1, fp);

    // Marks the start of the data
    fwrite("data", 1, 4, fp);
    fwrite(&length, 4, 1, fp);  // Data size
    for (int i = 0; i < frame_count; i++)
    {
        byte = channel[i];
        fwrite(&byte, 2, 1, fp);
    }

    fclose(fp);
    free(channel);
    return 0;
}

//compile with: gcc clip.c -o clip -lm
// -lm is needed to link the math library, which contains the sinf function used to generate the sine wave. 

/**
* The full credit to this source code goes to Adam Berg
* whose code is shown here: https://www.youtube.com/watch?v=8nOi-0kBv2Y&list=LL&index=57&t=189s
*
* This is just a very cool script to generate pure tones to WAV files
*
*
*
*
*
*
*
*
*/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

struct wav_header
{
  char riff[4];           // "RIFF"                                  
  int32_t flength;        // file length in bytes                    
  char wave[4];           // "WAVE"                                  
  char fmt[4];            // "fmt "                                  
  int32_t chunk_size;     // size of FMT chunk in bytes (usually 16) 
  int16_t format_tag;     // 1=PCM, 257=Mu-Law, 258=A-Law, 259=ADPCM 
  int16_t num_chans;      // 1=mono, 2=stereo                        
  int32_t srate;          // Sampling rate in samples per second     
  int32_t bytes_per_sec;  // bytes per second = srate*bytes_per_samp 
  int16_t bytes_per_samp; // 2=16-bit mono, 4=16-bit stereo          
  int16_t bits_per_samp;  // Number of bits per sample               
  char data[4];           // "data"                                  
  int32_t dlength;        // data length in bytes (filelength - 44)  
};

struct wav_header wavh;

const float MIDDLE_C = 256.00;


const int header_length = sizeof(struct wav_header);

// expects output filename, frequency, and duration
int main(int argc, char** args)
{
  if (argc != 4) {
    printf("Expected filename, frequency, and duration for arguments\n");
    return -1;
  }
  char* end;

  char* filename = args[1];
  double frequency = strtod(args[2], &end);
  double duration_seconds = strtod(args[3], &end);

  printf("%f\n", frequency);
  printf("%f\n", duration_seconds);

  const int sample_rate = 8000;
  const int buffer_size = (int)((double)sample_rate * duration_seconds);
  short int* buffer = malloc(buffer_size * sizeof(short int));

  strncpy(wavh.riff, "RIFF", 4);
  strncpy(wavh.wave, "WAVE", 4);
  strncpy(wavh.fmt, "fmt ", 4);
  strncpy(wavh.data, "data", 4);

  wavh.chunk_size = 16;
  wavh.format_tag = 1;
  wavh.num_chans = 1;
  wavh.srate = sample_rate;
  wavh.bits_per_samp = 16;
  wavh.bytes_per_sec = wavh.srate * wavh.bits_per_samp / 8 * wavh.num_chans;
  wavh.bytes_per_samp = wavh.bits_per_samp / 8 * wavh.num_chans;

  for (int i = 0; i < buffer_size; i++) {
    buffer[i] = (short int)((cos((2 * M_PI * frequency * i) / sample_rate) * 1000))
      + (short int)((cos((2 * M_PI * 137.0 * i) / sample_rate) * 1000));
  }

  wavh.dlength = buffer_size * wavh.bytes_per_samp;
  wavh.flength = wavh.dlength + header_length;

  FILE *fp = fopen(args[1], "w");
  fwrite(&wavh, 1, header_length, fp);
  fwrite(buffer, 2, buffer_size, fp);
}

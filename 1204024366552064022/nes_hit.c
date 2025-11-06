//windows x86_64-w64-mingw32-gcc -O2 nes_hit.c -o nes_hit -lwinmm
//Linux/mac gcc nes_hit -o nes -lportaudio -lm


/*
The MIT License (MIT)
Copyright � 2025 abnsoftware

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the �Software�), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED �AS IS�, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#else
#include <portaudio.h>
#endif
#define SAMPLE_RATE 44100
#define TEMPO_BPM 85
#define FRAMES_PER_SECOND 60
#define SIXTEENTH_NOTES_PER_BEAT 4
#define SIXTEENTH_NOTE_FRAMES ((int)((FRAMES_PER_SECOND*60.0)/(TEMPO_BPM*SIXTEENTH_NOTES_PER_BEAT)))
#define REST 0
typedef struct { int note; int frames; int type; } SongNote;
static double noteFreq(int midi) { return midi<=0 ? 0.0 : 440.0*pow(2.0, (midi-69)/12.0); }
typedef struct { int enabled; int duty; double freq; double phase; int volume; int decay_level; int envelope_start; int envelope_counter; } SquareChan;
typedef struct { int enabled; double freq; double phase; } TriangleChan;
typedef struct { int enabled; uint16_t shift; int periodIndex; int periodCounter; int volume; int decay_level; int envelope_start; int envelope_counter; } NoiseChan;
static int dutyTable[4][8]={ {0,1,0,0,0,0,0,0},{0,1,1,0,0,0,0,0},{0,1,1,1,1,0,0,0},{1,0,0,1,1,1,1,1} };
static int noisePeriods[16]={ 4,8,16,32,64,96,128,160,202,254,380,508,762,1016,2034,4068 };

double mixSample(double s1, double s2, double t, double n) {
	double square=s1+s2;
	double square_out=(square==0) ? 0 : 95.88/(8128.0/square+100);
	double tnd=(t/8227.0)+(n/12241.0);
	double tnd_out=(tnd==0) ? 0 : 159.79/(1.0/tnd+100);
	return square_out+tnd_out;
}

SongNote melody[]={
	{76, 2}, {71, 1}, {72, 1}, {74, 2}, {72, 1}, {71, 1},
	{69, 2}, {69, 1}, {72, 1}, {76, 2}, {74, 1}, {72, 1},
	{71, 2}, {71, 1}, {72, 1}, {74, 2}, {76, 2},
	{72, 2}, {69, 2}, {69, 4},
	{REST, 2}, {74, 2}, {77, 2}, {81, 2},
	{79, 2}, {77, 1}, {74, 1}, {76, 2}, {76, 1}, {72, 1},
	{76, 2}, {74, 2}, {72, 2}, {71, 2},
	{71, 2}, {72, 2}, {74, 2}, {76, 2},
	{72, 2}, {69, 2}, {69, 4}
};
int melodyCount=sizeof(melody)/sizeof(melody[0]);
SongNote harmony[]={
	{64,1},{67,1},{71,1}, {64,1},{67,1},{71,1}, {64,1},{67,1},{71,1}, {64,1},{67,1},{71,1},
	{62,1},{67,1},{71,1}, {62,1},{67,1},{71,1}, {62,1},{67,1},{71,1}, {62,1},{67,1},{71,1},
	{60,1},{64,1},{67,1}, {60,1},{64,1},{67,1}, {60,1},{64,1},{67,1}, {60,1},{64,1},{67,1},
	{59,1},{62,1},{65,1}, {59,1},{62,1},{65,1}, {59,1},{62,1},{65,1}, {59,1},{62,1},{65,1},
};
int harmonyCount=sizeof(harmony)/sizeof(harmony[0]);
SongNote bass[]={
	{52, 4}, {45, 4}, {47, 4}, {52, 2}, {50, 2},
	{48, 4}, {48, 4}, {47, 4}, {45, 4},
	{52, 4}, {45, 4}, {47, 4}, {52, 2}, {50, 2},
	{48, 4}, {48, 2}, {55, 2}, {54, 4}
};
int bassCount=sizeof(bass)/sizeof(bass[0]);
SongNote drums[]={
	{REST, 4, 0},
	{REST, 4, 2},
	{REST, 4, 0},
	{REST, 4, 2},
};
int drumCount=sizeof(drums)/sizeof(drums[0]);

int main() {
	int samplesPerFrame=SAMPLE_RATE/FRAMES_PER_SECOND;
	int maxFrames=1500;
	int totalSamples=maxFrames*samplesPerFrame;
#ifdef _WIN32
	int16_t* buf=(int16_t*)malloc(totalSamples*sizeof(int16_t));
#else
	float* buf=(float*)malloc(sizeof(float)*totalSamples);
#endif
	if(!buf) { printf("alloc fail\n"); return 1; }

	SquareChan sq1={ 1,1,0,0,12,0,1,0 };
	SquareChan sq2={ 1,2,0,0,9,0,1,0 };
	TriangleChan tri={ 1,0,0 };
	NoiseChan noise={ 1,1,0,0,2,0,1,0 };

	int mIndex=0, hIndex=0, bIndex=0, dIndex=0;
	int mRemain=0, hRemain=0, bRemain=0, dRemain=0;
	int pos=0;

	for(int frame=0;frame<maxFrames&&pos<totalSamples;frame++) {
		if(mRemain<=0) {
			mRemain=melody[mIndex].frames*SIXTEENTH_NOTE_FRAMES;
			if(melody[mIndex].note!=REST) { sq1.freq=noteFreq(melody[mIndex].note); sq1.phase=0; sq1.envelope_start=1; }
			else sq1.decay_level=0;
			mIndex=(mIndex+1)%melodyCount;
		}
		if(hRemain<=0) {
			hRemain=harmony[hIndex].frames*SIXTEENTH_NOTE_FRAMES;
			if(harmony[hIndex].note!=REST) { sq2.freq=noteFreq(harmony[hIndex].note); sq2.phase=0; sq2.envelope_start=1; }
			else sq2.decay_level=0;
			hIndex=(hIndex+1)%harmonyCount;
		}
		if(bRemain<=0) {
			bRemain=bass[bIndex].frames*SIXTEENTH_NOTE_FRAMES;
			if(bass[bIndex].note!=REST) { tri.enabled=1; tri.freq=noteFreq(bass[bIndex].note); tri.phase=0; }
			else tri.enabled=0;
			bIndex=(bIndex+1)%bassCount;
		}
		if(dRemain<=0) {
			dRemain=drums[dIndex].frames*SIXTEENTH_NOTE_FRAMES*4;
			if(drums[dIndex].type==2) { noise.shift=1; noise.periodIndex=10; noise.volume=2; noise.envelope_start=1; }
			dIndex=(dIndex+1)%drumCount;
		}

		if(sq1.envelope_start) { sq1.decay_level=15; sq1.envelope_counter=sq1.volume; sq1.envelope_start=0; }
		else if(sq1.decay_level>0) { sq1.envelope_counter--; if(sq1.envelope_counter<=0) { sq1.envelope_counter=sq1.volume; sq1.decay_level--; } }

		if(sq2.envelope_start) { sq2.decay_level=15; sq2.envelope_counter=sq2.volume; sq2.envelope_start=0; }
		else if(sq2.decay_level>0) { sq2.envelope_counter--; if(sq2.envelope_counter<=0) { sq2.envelope_counter=sq2.volume; sq2.decay_level--; } }

		if(noise.envelope_start) { noise.decay_level=15; noise.envelope_counter=noise.volume; noise.envelope_start=0; }
		else if(noise.decay_level>0) { noise.envelope_counter--; if(noise.envelope_counter<=0) { noise.envelope_counter=noise.volume; noise.decay_level--; } }

		if(pos+samplesPerFrame>totalSamples) samplesPerFrame=totalSamples-pos;

		for(int i=0;i<samplesPerFrame;i++) {
			int step1=(int)(sq1.phase*8.0);
			double s1=dutyTable[sq1.duty][step1] ? sq1.decay_level : 0;
			sq1.phase=fmod(sq1.phase+sq1.freq/SAMPLE_RATE, 1.0);

			int step2=(int)(sq2.phase*8.0);
			double s2=dutyTable[sq2.duty][step2] ? sq2.decay_level : 0;
			sq2.phase=fmod(sq2.phase+sq2.freq/SAMPLE_RATE, 1.0);

			double t=0;
			if(tri.enabled) { t=(fabs(fmod(tri.phase, 1.0)*2.0-1.0)*2.0-1.0)*15.0; tri.phase=fmod(tri.phase+tri.freq/SAMPLE_RATE, 1.0); }

			noise.periodCounter++;
			if(noise.periodCounter>=noisePeriods[noise.periodIndex]) { int fb=(noise.shift^(noise.shift>>1))&1; noise.shift=(noise.shift>>1)|(fb<<14); noise.periodCounter=0; }
			double n=(noise.shift&1) ? noise.decay_level : 0;

#ifdef _WIN32
			buf[pos++]=(int16_t)(mixSample(s1, s2, t, n)*12000);
#else
			buf[pos++]=(float)(mixSample(s1, s2, t, n)/15.0*2.0);
#endif

		}
		mRemain--; hRemain--; bRemain--; dRemain--;
	}

#ifdef _WIN32
	WAVEFORMATEX wfx={ WAVE_FORMAT_PCM,1,SAMPLE_RATE,SAMPLE_RATE*2,2,16,0 };
	HWAVEOUT hwo; WAVEHDR wh;
	waveOutOpen(&hwo, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
	wh.lpData=(LPSTR)buf; wh.dwBufferLength=pos*2; wh.dwFlags=0;
	while(1) {
		waveOutPrepareHeader(hwo, &wh, sizeof(wh)); waveOutWrite(hwo, &wh, sizeof(wh));
		while(!(wh.dwFlags&WHDR_DONE)) Sleep(5);
		waveOutUnprepareHeader(hwo, &wh, sizeof(wh));
	}
#else
	Pa_Initialize();
	PaStream* stream;
	Pa_OpenDefaultStream(&stream, 0, 1, paFloat32, SAMPLE_RATE, 256, NULL, NULL);
	Pa_StartStream(stream);
	Pa_WriteStream(stream, buf, pos);
	Pa_StopStream(stream);
	Pa_CloseStream(stream);
	Pa_Terminate();
#endif

	free(buf);
	return 0;
}

#include <JuceHeader.h>
#include "customdelay.h"

customdelay::customdelay() {
	delaybufL.resize(BUFSIZE);
	delaybufR.resize(BUFSIZE);
	for (size_t idx = 0; idx < BUFSIZE; idx++) {
		delaybufL[idx] = delaybufR[idx] = 0;
	}
	currPos = 0;
	fbSampleL = 0;
	fbSampleR = 0;
}

void customdelay::setSampleRate(double param) {
	sampleRate = param;
}

void customdelay::clearDelayBuffers() {
	for (size_t idx = 0; idx < BUFSIZE; idx++)
		delaybufL[idx] = delaybufR[idx] = 0;

	currPos = 0;
	fbSampleL = 0;
	fbSampleR = 0;
}

float customdelay::getSyncToMillisec(int numerator, int denominator, float bpm) {
	return (60 * 1000 / bpm) * 4 * numerator / denominator;
}

void customdelay::process(float* bufferL, float* bufferR, int numSamples, float delayTime, float feedback, float wet, bool ispingpong) {
	int numDelayed = (int)(sampleRate * (delayTime / 1000.0f));
	float dry = 1 - wet;
	if (ispingpong) {
		for (int idx = 0; idx < numSamples; idx++) {
			delaybufL[currPos] = (bufferL[idx] + bufferR[idx]) / 2 + fbSampleR * feedback;
			delaybufR[currPos] =  fbSampleL * feedback;

			int readIndex = currPos - numDelayed;
			if (readIndex < 0) readIndex += BUFSIZE;

			fbSampleL = delaybufL[readIndex];
			fbSampleR = delaybufR[readIndex];

			bufferL[idx] = bufferL[idx] * dry + fbSampleL * wet;
			bufferR[idx] = bufferR[idx] * dry + fbSampleR * wet;

			currPos++;
			if (currPos == BUFSIZE) currPos = 0;
		}
	}
	else {
		for (int idx = 0; idx < numSamples; idx++) {
			delaybufL[currPos] = bufferL[idx] + (fbSampleL * feedback);
			delaybufR[currPos] = bufferR[idx] + (fbSampleR * feedback);

			int readIndex = currPos - numDelayed;
			if (readIndex < 0) readIndex += BUFSIZE;

			fbSampleL = delaybufL[readIndex];
			fbSampleR = delaybufR[readIndex];

			/*fbSampleL = delaybufL[(idx - numDelayed) % BUFSIZE];
			fbSampleR = delaybufR[(idx - numDelayed) % BUFSIZE];*/

			bufferL[idx] = bufferL[idx] * dry + fbSampleL * wet;
			bufferR[idx] = bufferR[idx] * dry + fbSampleR * wet;

			currPos++;
			if(currPos == BUFSIZE) currPos = 0;
		}
	}
}

void customdelay::process(float* bufferL, float* bufferR, int numSamples, delayParameters params) {
	process(bufferL, bufferR, numSamples, params.delayTime, params.feedback, params.wet, params.ispingpong);
}


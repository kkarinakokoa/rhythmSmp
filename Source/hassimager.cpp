#include <JuceHeader.h>
#include "hassimager.h"

hassimager::hassimager() {
	delaybufR.resize(BUFSIZE);
	for (size_t idx = 0; idx < BUFSIZE; idx++)
		delaybufR[idx] = 0;

	currPos = 0;
	fbSampleL = 0;
	fbSampleR = 0;
}

void hassimager::setSampleRate(double param) {
	sampleRate = param;
}

void hassimager::clearImagerBuffers() {
	for (size_t idx = 0; idx < BUFSIZE; idx++)
		delaybufR[idx] = 0;

	currPos = 0;
	fbSampleL = 0;
	fbSampleR = 0;
}

void hassimager::process(float* bufferL, float* bufferR, int numSamples, float dry, float wet, float balance, float delayTime) {
	int numDelayed = (int)(sampleRate * (delayTime / 1000.0f));
	
	for (int idx = 0; idx < numSamples; idx++) {
		delaybufR[currPos] = (bufferL[idx] + bufferR[idx]) / 2;

		int readIndex = currPos - numDelayed;
		if (readIndex < 0) readIndex += BUFSIZE;

		//fbSampleL = ((bufferL[idx] + bufferR[idx]) / 2) * balance;
		fbSampleL = delaybufR[currPos] * balance;
		fbSampleR = delaybufR[readIndex];

		bufferL[idx] = bufferL[idx] * dry + fbSampleL * wet;
		bufferR[idx] = bufferR[idx] * dry + fbSampleR * wet;

		currPos++;
		if (currPos == BUFSIZE) currPos = 0;
	}
}

void hassimager::process(float* bufferL, float* bufferR, int numSamples, imagerParameters params) {
	process(bufferL, bufferR, numSamples, params.dry, params.wet, params.balance, params.delayTime);
}


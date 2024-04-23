#include <JuceHeader.h>
#include <algorithm>
#include "UltimatePhraser.h"

UltimatePhraser::UltimatePhraser()
{
	
}

void UltimatePhraser::setSampleRate(double param)
{
	sampleRate = param;
}

std::vector<struct UltimatePhraser::cell> UltimatePhraser::createPhrase(int noteNum, bool downbeat, bool tenuto)
{
	std::vector<int> startTemp(noteNum, -1);
	std::vector<int> lenTemp(noteNum, -1);
	std::vector<juce::uint8> velTemp(noteNum, 0);
	std::vector<struct UltimatePhraser::cell> phrase;

	for (int idx = 0; idx < noteNum; idx++) {
		int revelation;
		if ((idx == 0) && downbeat) {
			startTemp[idx] = 0;
			continue;
		}
		do {
			revelation = divining.nextInt(16) * 2; // 0, 2, 4, ..., 30th grid
		} while (!(std::find(startTemp.begin(), startTemp.end(), revelation) == startTemp.end()));
		startTemp[idx] = revelation;
		//DBG("[[[[[[revelation]]]]]] " << revelation);
	}
	std::sort(startTemp.begin(), startTemp.end());
	startTemp.push_back(32); // ¾²·¹±â

	for (int idx = 0; idx < noteNum; idx++) {
		int fullLen = startTemp[idx + 1] - startTemp[idx];
		int finalLen;
		if (tenuto) {
			finalLen = fullLen;
		}
		else {
			if ((fullLen >= 2) && (fullLen < 8)) { // 16 ~ 4
				finalLen = divining.nextInt(fullLen) + 1;
			}
			else if ((fullLen >= 8) && (fullLen < 12)) { // 4 ~ 2.5
				finalLen = divining.nextInt(fullLen) + 1;
			}
			else if ((fullLen >= 12) && (fullLen < 24)) { // 2.5 ~ 1.5
				finalLen = divining.nextInt(fullLen) + 1;
			}
			else { // 1.5 ~ 1
				finalLen = divining.nextInt(fullLen) + 1;
			}
		}
		lenTemp[idx] = finalLen;
	}

	int velType = divining.nextInt(3); // 0: ordinary, 1: jump & down, 2: linear

	if (velType == 0) {
		for (int idx = 0; idx < noteNum; idx++) {
			juce::uint8 vel = (juce::uint8)(divining.nextInt(63) + 65); // 65 ~ 127
			velTemp[idx] = vel;
		}

	}
	else if (velType == 1) {
		int Vel1 = divining.nextInt(48) + 80; // 80 ~ 127
		int Vel2 = divining.nextInt(36) + 25; // 25 ~ 60
		int step = divining.nextInt(3) + 2; // 2, 3, 4, 5
		int starting = divining.nextInt(step), count = 0;
		if (divining.nextBool()) {
			Vel1 ^= Vel2;
			Vel2 ^= Vel1;
			Vel1 ^= Vel2;
		}
		for (int idx = 0; idx < noteNum; idx++) {
			int sign = divining.nextInt(3) - 1;
			int oscillation = divining.nextInt(6);
			int vel;

			if (count == starting)
				vel = Vel1 + sign * oscillation;
			else
				vel = Vel2 + sign * oscillation;

			velTemp[idx] = (juce::uint8)vel;
			count = (count + 1) % step;
		}
	}
	else if (velType == 2) {
		int step = divining.nextInt(4) + 3; // 3, 4, 5, 6
		std::vector<int> vel;
		for (int idx = 0; idx < step; idx++)
			vel.push_back(divining.nextInt(96) + 25);

		bool isupward = divining.nextBool();
		if (isupward) std::sort(vel.begin(), vel.end());
		else std::sort(vel.rbegin(), vel.rend());

		int starting = divining.nextInt(step), count = 0;
		for (int idx = 0; idx < noteNum; idx++) {
			int realIdx = ((idx - starting) + step) % step;
			int sign = divining.nextInt(3) - 1;
			int oscillation = divining.nextInt(6);
			
			velTemp[idx] = (juce::uint8)(vel[realIdx] + sign * oscillation);
			count++;
			if (count == step) {
				bool upordown = divining.nextBool();
				if (!upordown && isupward) std::sort(vel.rbegin(), vel.rend());
				else if (upordown && !isupward) std::sort(vel.begin(), vel.end());
				count = 0;
				isupward = upordown;
			}
		}
	}

	int count = 0, startPos=0, len=0;

	
	for (int idx = 0; idx < noteNum; idx++) {
		struct UltimatePhraser::cell pause, note;
		
		note.startPos = startTemp[idx];
		note.len = lenTemp[idx];
		note.isNote = true;
		note.velocity = velTemp[idx];

		len = startTemp[idx] - startPos;

		if (len > 0) {
			pause.startPos = startPos;
			pause.len = len;
			pause.isNote = false;
			pause.velocity = 0;
			
			phrase.push_back(pause);
		}
		phrase.push_back(note);

		startPos = startTemp[idx] + lenTemp[idx]; // new startPos
		len = 0; // new len
	}
	if (startPos < 32) {
		struct UltimatePhraser::cell pause;
		pause.startPos = startPos;
		pause.len = 32 - startPos;
		pause.isNote = false;
		pause.velocity = 0;

		phrase.push_back(pause);
	}

	return phrase;
}

void UltimatePhraser::createPhrases(int min, int max, bool downbeat, bool tenuto) {
	for (int idx = 0; idx < 12; idx++) {
		int num = divining.nextInt(max - min + 1) + min;
		phrases[idx] = createPhrase(num, downbeat, tenuto);
	}
	reset();
}

void UltimatePhraser::reset()
{
	for (int idx = 0; idx < 12; idx++) idxNextPop[idx] = 0;
}

void UltimatePhraser::reset(int index)
{
	idxNextPop[index] = 0;
}

int UltimatePhraser::popNextLength(int index, float bpm)
{
	int len = (int)(sampleRate * (60.0f / bpm) * 4.0f * ((float)phrases[index][idxNextPop[index]].len / 32));
	return len;
}

void UltimatePhraser::updateNext(int index)
{
	idxNextPop[index] = (idxNextPop[index] + 1) % phrases[index].size();
}

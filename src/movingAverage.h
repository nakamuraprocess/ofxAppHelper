#pragma once
#include "ofMain.h"

class movingAverage {
private:
	vector <double> data;
	int size;
	float rate;
	int iCurrent;
	double dAverage;
	int iMode;
	
public:
	enum MODE {LOOP, STOP};

	void setup(int size, MODE mode) {
		this->size = size;
		this->rate = 1.0f / size;
		data.assign(size, 0);
		this->iMode = mode;
		iCurrent = 0;
	}

	void input(double iNew) {
		if (this->iMode == STOP) {
			if (this->size < iCurrent) {
				return;
			}
			iCurrent++;
		}

		data.push_back(iNew);
		data.erase(data.begin());

		double dSum = 0.0f;
		for (auto value : data) {
			dSum += value;
		}

		dAverage = dSum * rate;
	}

	double output() {
		return dAverage;
	}

	void reset() {
		for (auto& value : data) {
			value = 0;
		}
		iCurrent = 0;
	}
};
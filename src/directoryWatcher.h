#pragma once
#include "ofMain.h"

class DirectoryWatcher : public ofThread {
public:
	~DirectoryWatcher() {
		waitForThread();
		stopThread();
	}

	void setup(const int sleeptime, const string targetDir, const string allowExt){
		this->sleepTime = sleeptime;
		this->targetDir = targetDir;
		this->allowExt = allowExt;
		ofDirectory dir(targetDir);
		dir.allowExt(allowExt);
		dir.listDir(targetDir);
		dir.sortByDate();
		currentFileSize = prevFileSize =(int)dir.size();
		startThread();
	}

	void threadedFunction() {
		while (isThreadRunning()) {
			ofDirectory dir(targetDir);
			dir.allowExt(allowExt);
			dir.listDir(targetDir);
			dir.sortByDate();
			currentFileSize = (int)dir.size();

			if (currentFileSize > prevFileSize) {
				string strNew = dir.getPath(currentFileSize - 1);
				cout << strNew << endl;
				ofNotifyEvent(eventSendNewFilePath, strNew);
				prevFileSize = currentFileSize;
			}
			sleep(sleepTime);
		}
	}

protected:
	int sleepTime;
	int	currentFileSize = 0;
	int	prevFileSize = 0;

	string targetDir;
	string allowExt;

public:
	ofEvent<string> eventSendNewFilePath;

};
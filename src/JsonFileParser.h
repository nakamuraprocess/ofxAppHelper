#pragma once
#include "ofMain.h"

class JsonFileParser {
public:
	int size = 0;

	void readFile(const string path){
		ofFile file(path);

		if (file.exists()) {
			ofLogNotice() << path;
			ofLogNotice() << "Parsing json is starting.";
			file >> json;
			ofLogNotice() << "Parsing json is completed.";
			if (json.empty()) {
				ofLogError() << "json is empty.";
			}
			else{
				size = json.size();
			}
		}
		else {
			ofLogError() << "json file Not found " + path + ".";
		}
	}

	bool toBool(const string keyName) {
		return json[keyName];
	}

	auto toNumber(const string keyName) {
		return json[keyName];
	}

	string toString(const string keyName) {
		return json[keyName];
	}

	vector <string> toVectorAsString(const string keyName) {
		return json[keyName];
	}

	bool toBool(const int index, const string keyName) {
		return json[index][keyName];
	}

	auto toNumber(const int index, const string keyName) {
		return json[index][keyName];
	}

	string toString(const int index, const string keyName) {
		return json[index][keyName];
	}

	vector <string> toVectorAsString(const int index, const string keyName) {
		return json[index][keyName];
	}

protected:
	ofJson json;
	
};
#pragma once

#include <sys/types.h> 
#include <algorithm>
#include <random>
#include <regex>

#include "movingAverage.h"
#include "directoryWatcher.h"
#include "JsonFileParser.h"
#ifdef TARGET_WIN32
#include "Win32CreateProcess.h"
#endif

// Hide Console Window
// Add to top in main.cpp
// #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

static class ofxAppHelper {
public:

	static void setLog() {
		ofLogToFile("log/" + ofxAppHelper::getToday() + ".log", true);
	}

	static void numberLoop(int& iInput, int iSize) {
		int i = iInput;
		iInput = ((i + 1) % iSize);
	}

	static int numberLoopMinus(int& iInput, int iSize) {
		int i = iInput;
		iInput = ((i + 1) % iSize);
		return iSize - (i + 1);
	}

	static string addZeroForDate(const int& iNumber) {
		string out;
		if (iNumber < 10) {
			out = "0" + ofToString(iNumber, 0);
		}
		else {
			out = ofToString(iNumber, 0);
		}
		return out;
	}

	static string getToday() {
		return ofGetTimestampString("%Y%m%d");
	}

	static string getToday(string delimiter) {
		return ofGetTimestampString("%Y" + delimiter + "%m" + delimiter + "%d");
	}

	static string getTime() {
		return ofGetTimestampString("%H%M%S");
	}

	static string getTimeAMPM() {
		int Hours = ofGetHours();
		string Minutes = addZeroForDate(ofGetMinutes());

		string timeStamp = "";
		if (Hours < 13) {
			timeStamp = "AM " + ofToString(Hours) + ":" + Minutes;
		}
		else {
			timeStamp = "PM " + ofToString(Hours - 12) + ":" + Minutes;
		}
		return timeStamp;
	}

	static void setJapaneseFont(ofTrueTypeFont& ttf, string fontName, int size) {
		ofTrueTypeFontSettings fontSettings(fontName, size);
		fontSettings.antialiased = true;
		fontSettings.dpi = 72;
		fontSettings.direction = OF_TTF_LEFT_TO_RIGHT;
		fontSettings.addRanges(ofAlphabet::Latin);
		fontSettings.addRanges(ofAlphabet::Japanese);
		ttf.load(fontSettings);
	}

#ifdef TARGET_WIN32
	static string SjistoUTF8(string srcSjis) {
		int lenghtUnicode = MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, NULL, 0);
		wchar_t* bufUnicode = new wchar_t[lenghtUnicode];
		MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, bufUnicode, lenghtUnicode);
		int lengthUTF8 = WideCharToMultiByte(CP_UTF8, 0, bufUnicode, -1, NULL, 0, NULL, NULL);
		char* bufUTF8 = new char[lengthUTF8];
		WideCharToMultiByte(CP_UTF8, 0, bufUnicode, lenghtUnicode + 1, bufUTF8, lengthUTF8, NULL, NULL);
		string strUTF8(bufUTF8);
		delete bufUnicode;
		delete bufUTF8;
		return strUTF8;
	}

	static string UTF8toSjis(string srcUTF8) {
		int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, NULL, 0);
		wchar_t* bufUnicode = new wchar_t[lenghtUnicode];
		MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, bufUnicode, lenghtUnicode);
		int lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);
		char* bufShiftJis = new char[lengthSJis];
		WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, NULL, NULL);
		string strSJis(bufShiftJis);
		delete bufUnicode;
		delete bufShiftJis;
		return strSJis;
	}

	static string cmdCurlPost(const string contentType, const string paramPath, const string targatUrl) {
		string cmd = "curl -X POST ";
		if (contentType == "json") {
			cmd += "-H \"Content-Type: application/json\" -d @";
		}
		else if (contentType == "multipart") {
			cmd += "-H \"Content-Type: multipart/form-data\" -F file=@";
		}
		cmd += paramPath + " " + targatUrl;
		return execCmd(cmd.c_str());
	}

	static string execCmd(const char* cmd) {
		string stdOut;
		int exitCode;

		std::shared_ptr<FILE> pipe(_popen(cmd, "r"), [&](FILE* p) {exitCode = _pclose(p); });
		if (!pipe) {
			return "Standard output failed.";
		}
		std::array<char, 256> buf;
		while (!feof(pipe.get())) {
			if (fgets(buf.data(), buf.size(), pipe.get()) != nullptr) {
				stdOut += buf.data();
			}
		}
		return stdOut;
	}

	static void setKeyboard(LPINPUT lpKey, WORD wVKey, BOOL bKeyDown) {
		lpKey->type = INPUT_KEYBOARD;
		lpKey->ki.wVk = wVKey;
		lpKey->ki.wScan = (WORD)MapVirtualKey(wVKey, 0);
		lpKey->ki.dwFlags = (bKeyDown ? 0 : KEYEVENTF_KEYUP);
		lpKey->ki.dwExtraInfo = 0;
		lpKey->ki.time = 0;
	}

	static void setConsoleEnableQuickEditMode() {
		HANDLE hInput;
		DWORD prev_mode;
		hInput = GetStdHandle(STD_INPUT_HANDLE);
		GetConsoleMode(hInput, &prev_mode);
		SetConsoleMode(hInput, prev_mode & ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE));
	}

#endif

	static void saveCapture(ofImage* imageSaved, const int width, const int height, const string subDir = "") {
		imageSaved->grabScreen(0, 0, width, height);
		string targetPath = "screenshot/" + getToday();
		ofDirectory dir;
		if (!dir.doesDirectoryExist(targetPath)) {
			dir.createDirectory(targetPath);
		}
		if (subDir != "") {
			targetPath += "/" + subDir;
			if (!dir.doesDirectoryExist(targetPath)) {
				dir.createDirectory(targetPath);
			}
		}
		string strPictName = getTime() + ".jpg";
		imageSaved->save(targetPath + "/" + strPictName);
	}

	static void saveCaptureSequence(ofImage* imageSaved, const int width, const int height, unsigned long saveImageCounter, const string subDir = "") {
		imageSaved->grabScreen(0, 0, width, height);
		string targetPath = "screenshot/" + getToday();
		ofDirectory dir;
		if (!dir.doesDirectoryExist(targetPath)) {
			dir.createDirectory(targetPath);
		}
		if (subDir != "") {
			targetPath += "/" + subDir;
			if (!dir.doesDirectoryExist(targetPath)) {
				dir.createDirectory(targetPath);
			}
		}

		string zeroes = "00000000";
		zeroes = zeroes.substr(0, zeroes.length() - (ofToString(saveImageCounter).length()));
		string fileName = targetPath + "/a_" + zeroes + ofToString(saveImageCounter) + ".png";
		imageSaved->save(targetPath + "/" + fileName);
	}

	static bool createDir(const string& path) {
		ofDirectory dir;
		if (!dir.doesDirectoryExist(path)) {
			 return dir.createDirectory(path, true);
		}
		else return false;
	}

	static vector <string> stringSplit(const string &str, char sep) {
		vector<string> v;
		stringstream ss(str);
		string buffer;
		while (getline(ss, buffer, sep)) {
			v.push_back(buffer);
		}
		return v;
	}

	static void shuffle(vector<int>& i) {
		random_device seed_gen;
		mt19937 engine(seed_gen());
		std::shuffle(i.begin(), i.end(), engine);
	}

	static string replace(const string baseText, const string replaceTarget, const string replaceText) {
		string str = regex_replace(baseText, regex(replaceTarget), replaceText);
		return str;
	}
}ofxAppHelper;
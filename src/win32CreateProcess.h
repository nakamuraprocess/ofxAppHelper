#pragma once
#include "ofMain.h"

#ifdef TARGET_WIN32
class win32CreateProcess {
protected:
	bool bCanForceTerminate;
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

public:
	ofEvent<unsigned long> exitCode;

	// This stop main thread
	void createProcessForeground(const char* cmd, bool windowShow) {
		DWORD flag = 0;
		BOOL fWait = true;
		DWORD dwExitCode;
		memset(&si, 0, sizeof(STARTUPINFO));
		memset(&pi, 0, sizeof(PROCESS_INFORMATION));
		si.cb = sizeof(STARTUPINFO);

		if(!windowShow){
			si.dwFlags = STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_HIDE;
		}

		CreateProcessA(NULL, LPSTR(cmd), NULL, NULL, FALSE, flag, NULL, NULL, &si, &pi);
		if (fWait) {
			DWORD dwResult = WaitForSingleObject(pi.hProcess, INFINITE);
			if (WAIT_OBJECT_0 == dwResult) {
				GetExitCodeProcess(pi.hProcess, &dwExitCode);
				ofNotifyEvent(exitCode, dwExitCode);
			}
		}

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

	// This none stop main thread
	void createProcessBackground(const char* cmd, bool windowShow) {
		si = { sizeof(STARTUPINFOA) };
		pi = { 0 };

		if (!windowShow) {
			si.dwFlags = STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_HIDE;
		}

		CreateProcessA(NULL, LPSTR(cmd), NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
		bCanForceTerminate = true;
	}

	void forceTerminate() {
		if (bCanForceTerminate) {
			TerminateProcess(pi.hProcess, 0);
			// work in progress, if exit cmmand line proc with CTRL+C ...
			// GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0);
		}
	}

};
#endif

//ofAddListener(cp.exitCode, this, &ofApp::callback);

//ofApp::callback(unsigned long& value){
//
//}
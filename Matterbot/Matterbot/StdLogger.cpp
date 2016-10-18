#include "StdLogger.h"
#include <iostream>

using namespace lospi;
using namespace std;

void StdLogger::info(const wstring &msg) {
	wclog << msg << endl;
}

void StdLogger::warn(const wstring &msg) {
	wclog << msg << endl;
}

void StdLogger::error(const wstring &msg) {
	wcerr << msg << endl;
}
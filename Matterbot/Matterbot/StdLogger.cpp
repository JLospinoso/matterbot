#include "StdLogger.h"
#include <iostream>

using namespace lospi;
using namespace std;
using utility::string_t;

void StdLogger::info(const string_t &msg) {
	ucout << msg << endl;
}

void StdLogger::warn(const string_t &msg) {
	ucout << msg << endl;
}

void StdLogger::error(const string_t &msg) {
	ucerr << msg << endl;
}

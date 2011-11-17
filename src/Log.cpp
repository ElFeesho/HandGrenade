/*
 * Log.cpp
 *
 * Logging class that prints out warnings, errors, debug statements and information
 *
 *  Created on: Jul 2, 2011
 *      Author: feesh
 */

#include "Log.hpp"

#include <sstream>

using std::stringstream;

#define GREEN_COL "\033[32;1m"
#define YELLOW_COL "\x1b[33;1m"
#define RED_COL "\x1b[31;1m"
#define NORMAL_COL = "\033[33;0m"

namespace HG
{

Log::Log()
{
}

Log::~Log()
{
}

void Log::w(const std::string &msg)
{
	cerr << "\x1b[33;1m" << "WARN: " << "\033[33;0m" << msg << std::endl;
}

void Log::e(const std::string &msg)
{
	cerr << "\x1b[31;1m" << " ERR: " << "\033[33;0m" << msg << std::endl;
}

void Log::d(const std::string &msg)
{
	cout << "\033[36;1m" << "DBUG: " << "\033[33;0m" << msg << std::endl;
}

void Log::i(const std::string &msg)
{
	cout << "\033[32;1m" << "INFO: " << "\033[33;0m" << msg << std::endl;
}

}

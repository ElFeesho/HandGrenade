/*
 * Log.h
 *
 *  Created on: Jul 2, 2011
 *      Author: feesh
 */

#ifndef LOG_H_
#define LOG_H_

#include <string>
#include <sstream>
#include <iostream>

using std::string;
using std::cout;
using std::cerr;
using std::stringstream;

namespace HG
{

class Log
{
public:
	Log();
	~Log();

	template<typename T>
	static void w(const std::string &msg, T arg1 = NULL)
	{
		stringstream buf;
		buf << msg;
		buf << arg1;
		cout << "\x1b[33;1m" << "WARN: " << "\033[33;0m" << buf.str() << std::endl;
	}

	template<typename T>
	static void e(const std::string &msg, T arg1 = NULL)
	{
		stringstream buf;
		buf << msg;
		buf << arg1;
		cout << "\x1b[31;1m" << " ERR: " << "\033[33;0m" << buf.str() << std::endl;
	}

	template<typename T>
	static void d(const std::string &msg, T arg1 = NULL)
	{
		stringstream buf;
		buf << msg;
		buf << arg1;
		cout << "\x1b[36;1m" << "DBUG: " << "\033[33;0m" << buf.str() << std::endl;
	}

	template<typename T>
	static void i(const std::string &msg, T arg1 = NULL)
	{
		stringstream buf;
		buf << msg;
		buf << arg1;
		cout << "\x1b[32;1m" << "INFO: " << "\033[33;0m" << buf.str() << std::endl;
	}

	static void w(const std::string &);
	static void e(const std::string &);
	static void d(const std::string &);
	static void i(const std::string &);
};

}

#endif /* LOG_H_ */

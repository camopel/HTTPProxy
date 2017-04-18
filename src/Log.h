/*
 * Log.h
 *
 *  Created on: Apr 16, 2017
 *      Author: yduan
 */

#ifndef LOG_H_
#define LOG_H_
#include <fstream>
#include <stdio.h>
#include <stdarg.h>
#include <string>
using namespace std;
/*
static void Log(char * format, ...)
{
  char buffer[2048];
  va_list args;
  va_start (args, format);
  vsnprintf (buffer, 2048, format, args);
  va_end (args);
  ofstream log_file("block.log", ios_base::out | ios_base::app );
  time_t curTime;
  time(&curTime);
  string curTimeStr = ctime(&curTime);
  log_file<<curTimeStr<<endl<<buffer<<endl; log_file.close();
  log_file.close();
}
*/
static void Log(string info)
{
	ofstream log_file("block.log", ios_base::out|ios_base::app);
	time_t     now = time(0);
	struct tm  tstruct;
	char       time[20];
	tstruct = *localtime(&now);
	strftime(time, sizeof(time), "%Y-%m-%d %X", &tstruct);
	cout<<"**Blocked@"<<time<<endl;
	log_file<<time<<"\r\n---------------\r\n"<<info<<"\r\n---------------\r\n";
	log_file.flush();
	log_file.close();
}

#endif /* LOG_H_ */

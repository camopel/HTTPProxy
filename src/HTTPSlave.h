/*
 * HTTPSlave.h
 *
 *  Created on: Apr 16, 2017
 *      Author: yduan
 */

#ifndef HTTPSLAVE_H_
#define HTTPSLAVE_H_
#include"HTTPServer.h"
#include<string>
class HTTPSlave : public HTTPServer{
	public:
		HTTPSlave(int);
		~HTTPSlave();
	private:
		bool ParseRequest();
};
#endif /* HTTPSLAVE_H_ */

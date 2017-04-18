/*
 * HTTPServer.h
 *
 *  Created on: Apr 15, 2017
 *      Author: yduan
 */

#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_
using namespace std;
#include<arpa/inet.h>
#include<sys/socket.h>
#include <string>
using namespace std;
class HTTPServer{
	public:
		HTTPServer(int );
		virtual ~HTTPServer();
		int ans_socket;
		int Run();
		virtual void HandleRequest();
		virtual bool ParseRequest();
		virtual bool SendRequest();
		virtual bool GetResponse();
		virtual bool ReplyResponse();
		void Send_notfound();
		void PrintRequestInfo();
		bool PrintRequestHeaderAndCheckIfSupport();
		static const int buf_sz = 1500;
		static const int large_buf_sz = 1000000;
		int ID;//socket id, start from 0, increase by one
		int reqPort;
		int req_Socket;
		string reqHost;
		char* buf;
		size_t buflen;
		char* pool;
		size_t poollen;
		struct hostent* reqIP;
		int port;//listen port
	private:
		int InitSocket();
		int listen_socket;
		struct sockaddr_in listen_addr, ans_Addr;
};


#endif /* HTTPSERVER_H_ */

/*
 * HTTPSlave.cpp
 *
 *  Created on: Apr 16, 2017
 *      Author: yduan
 */
#include<iostream>
#include<string>
#include<cstring>
#include<sstream>
#include<ctime>

#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netdb.h>
#include<netinet/in.h>
#include"HTTPSlave.h"
#include"HTTPServer.h"
using namespace std;


HTTPSlave::HTTPSlave(int p):HTTPServer(p){}
HTTPSlave::~HTTPSlave(){}

bool HTTPSlave::ParseRequest(){
	string req(buf);
	istringstream iss(req);
	string tmp,url;
	iss>>tmp>>url;
	if(tmp.compare("GET")!=0){
		Send_notfound();
		return false;
	}
	//cout<<"Slave parseRequest"<<endl;
	size_t pos = url.find("//");
	if(pos==std::string::npos) pos=0;
	else pos+=2;
	size_t beg=pos,len=string::npos;
	pos=url.find("/",pos);
	if(pos!=std::string::npos) len=pos-beg;
	reqHost = url.substr(beg,len);
	pos=reqHost.find(":");
	reqPort=80;
	if(pos!=std::string::npos){
		reqPort = atoi(reqHost.substr(pos+1).c_str());
		reqHost=reqHost.substr(0,pos);
	}
	reqIP=gethostbyname(reqHost.c_str());

//	PrintRequestInfo();
	return true;
}


/*
 * HTTPMaster.cpp
 *
 *  Created on: Apr 16, 2017
 *      Author: yduan
 */
#include<iostream>
#include<string>
#include<cstring>
#include<sstream>
#include<ctime>

#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include "HTTPMaster.h"
#include "HTTPServer.h"
#include "Balance.h"

using namespace std;

HTTPMaster::HTTPMaster(int p):HTTPServer(p){}
HTTPMaster::~HTTPMaster(){}
void HTTPMaster::SetPolicy(list<Policy*> inbound,list<Policy*>outbound){
	inbound_policies = inbound;
	outbound_policies = outbound;
}
bool HTTPMaster::ParseRequest(){
	string req(buf);
	//PrintRequestHeaderAndCheckIfSupport();
	if(!PrintRequestHeaderAndCheckIfSupport()) return false;
	bool pass=Policy::Filter(req, outbound_policies);
	if(pass){
		string slave = Balance::Pick();
		size_t pos = slave.find(':');
		reqHost = slave.substr(0,pos);
		string sport = slave.substr(pos+1,slave.length()-pos-1);
		reqPort = atoi(sport.c_str());
		reqIP=gethostbyname(reqHost.c_str());
		//PrintRequestHeaderAndCheckIfSupport();
		cout<<ID<<"...Slave:"<<reqHost<<":"<<reqPort<<endl;
		return true;
	}
	return false;
}
bool HTTPMaster::ReplyResponse(){
	string req(pool);
	bool pass=Policy::Filter(req, inbound_policies);
	if(pass) {
		int len = send(ans_socket,pool,poollen,0);
		cout<<ID<<"...HTTPMaster::ReplyResponse:"<<len<<endl;
	}
	else Send_notfound();
	return true;
}


/*
 * Policy.cpp
 *
 *  Created on: Apr 16, 2017
 *      Author: yduan
 */

#include"Policy.h"
using namespace std;
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include <list>
#include <netinet/in.h>
#include <sstream>
#include <iostream>
#include "Log.h"
Policy::Policy():p_type(UD),p_ip(0),p_mask(0),p_port(0),test_ip(0),test_port(0){}

Policy::Policy(unsigned int t_ip,unsigned int t_mask):p_type(IM),p_port(0),test_ip(0),test_port(0){
	p_ip = t_ip;
	p_mask = t_mask;
}
Policy::Policy(int t_port):p_type(PT),p_ip(0),p_mask(0),test_ip(0),test_port(0){
	p_port=t_port;
}
Policy::Policy(string t_kw_host,Type t):p_ip(0),p_mask(0),p_port(0),test_ip(0),test_port(0){
	if(t==HT){
		p_type=t;
		p_host=t_kw_host;
	}
	else if(t==KW){
		p_type=t;
		p_keyword=t_kw_host;
	}
}
Policy::~Policy(){}

bool Policy::endWith(string const &fullString, string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    }
    else  return false;
}
void Policy::Prepare(string request){
	test_content = request;
	istringstream iss(request);
	string url,method;
	iss>>method>>url;
	if(method.compare("GET")!=0) return;
	size_t pos = url.find("//");
	if(pos==std::string::npos) pos=0;
	else pos+=2;
	size_t beg=pos,len=string::npos;
	pos=url.find("/",pos);
	if(pos!=std::string::npos) len=pos-beg;
	test_host = url.substr(beg,len);
	pos= test_host.find(":");
	test_port=80;
	if(pos!=std::string::npos){
		test_port = atoi(test_host.substr(pos+1).c_str());
		test_host=test_host.substr(0,pos);
	}
	struct hostent* ht=gethostbyname(test_host.c_str());
	struct in_addr addr;
	memcpy(&addr,ht->h_addr,4);
	test_ip=(unsigned int)inet_addr(inet_ntoa(addr));
}
bool Policy::Filter(string request,list<Policy*> rules){//true pass, false not pass
	Policy* policy = new Policy();
	policy->Prepare(request);
	bool pass=true;
	Policy* p;
	for (list<Policy*>::iterator it=rules.begin(); pass && it != rules.end(); ++it){
		p = *it;
		if(p->p_type==KW){
			if(policy->test_content.find(p->p_keyword)!=string::npos) pass=false;
		}
		else if(p->p_type==HT){
			if(policy->endWith(policy->test_host,p->p_host)) pass=false;
		}
		else if(p->p_type==IM){
			unsigned int p_subnet = p->p_ip & p->p_mask;
			unsigned int req_subnet = policy->test_ip & p->p_mask;
			if(p_subnet==req_subnet) pass=false;
		}
		else if(p->p_type==PT){
			if(policy->test_port==p->p_port) pass=false;
		}
	}
	if(pass) return true;
	else {
		Log(p->toString()+"\r\n"+request);
		return false;
	}
}

string Policy::toString(){
	ostringstream s;
	if(p_type==UD) s<<"Type:undef";
	else if(p_type==IM) {
		s<<"Type:IP_MASK";
		struct in_addr addr;
		memcpy(&addr,&p_ip,4);
		s<<" IP:"<<inet_ntoa(addr);
		memcpy(&addr,&p_mask,4);
		s<<" MASK:"<<inet_ntoa(addr);
	}
	else if(p_type==PT){
		s<<"Type:Port:"<<p_port;
	}
	else if(p_type==KW){
		s<<"Type:Keyword:"<<p_keyword;
	}
	else if(p_type==HT){
		s<<"Type:Host:"<<p_host;
	}
	return s.str();
}

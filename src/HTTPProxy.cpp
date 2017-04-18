//============================================================================
// Name        : HTTPProxy.cpp
// Author      : Yun Duan
// Version     :
// Copyright   : SJSU
// Description : Hello World in C++, Ansi-style
//============================================================================
#include "HTTPServer.h"
#include "HTTPSlave.h"
#include "HTTPMaster.h"
#include "Balance.h"
#include "Policy.h"
using namespace std;
#include <iostream>
#include <stdlib.h>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
map<string, string> options;
list<Policy*> out_Policies;
list<Policy*> in_Policies;
vector<string> slaves;

bool LoadConfig(){
	options.clear();
	out_Policies.clear();
	in_Policies.clear();
	int state=0;//0-config 1-policy inboud 2 outbound

	ifstream cfgfile;
	cfgfile.open("config.txt");
	//cfgfile.open("./Debug/config.txt");
	if(!cfgfile) {
		cout<<"can not open config file"<<endl;
		return false;
	}
	string line;
	string id, eq, val;
	while(getline(cfgfile,line)){
		if(line.size()<5) continue;
		istringstream iss(line);
		bool error = false;
		iss>>id;
		if (id[0] == '#') {
			if(id.compare("#Inbound")==0) state=1;
			else if(id.compare("#Outbound")==0) state=2;
			else if(id.compare("#Slaves")==0) state=3;
			continue;
		}
		else{
			iss >> eq >> val >> std::ws;
			if(eq.compare("=")!=0 || iss.get() != EOF)  error = true;
		}

		if (error){
			cout<<"can not parse config file"<<endl;
			return false;
		}
		else{
			if(state==0) options[id] = val;
			else if(state==1){
				if(id.compare("keyword")==0){
					//cout<<state<<val<<endl;
					in_Policies.push_back(new Policy(val,Policy::KW));
				}
			}
			else if(state==2){
				if(id.compare("ip_mask")==0) {
					size_t pos = val.find(',');
					string sip = val.substr(0,pos);
					string smask = val.substr(pos+1,val.length()-pos-1);
					unsigned int ip = (unsigned int)inet_addr(sip.c_str());
					unsigned int mask = (unsigned int)inet_addr(smask.c_str());
					//cout<<sip<<","<<smask<<","<<ip<<","<<mask<<endl;
					out_Policies.push_back(new Policy(ip,mask));
				}
				else if(id.compare("port")==0){
					int p = atoi(val.c_str());
					//cout<<"port:"<<p<<endl;
					out_Policies.push_back(new Policy(p));
				}
				else if(id.compare("host")==0){
					//cout<<"host:"<<val<<endl;
					out_Policies.push_back(new Policy(val,Policy::HT));
				}
			}
			else if(state==3){
				//cout<<val<<endl;
				slaves.push_back(val);
			}
		}
	}
	cfgfile.close();
	return true;
}

int main(int argc, char* argv[]) {
	if(LoadConfig()){
		int p = atoi(options["port"].c_str());
		if(options["role"].compare("master")==0){
//			cout<<p<<endl;
			HTTPMaster* http = new HTTPMaster(p);
			http->SetPolicy(in_Policies,out_Policies);
			Balance::Initial(slaves);
			//http->SetSlaves(slaves);
			http->Run();
		}
		else{
			HTTPSlave* http = new HTTPSlave(p);
			http->Run();
		}
	}
	return 0;
}

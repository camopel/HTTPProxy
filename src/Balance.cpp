/*
 * Balance.cpp
 *
 *  Created on: Apr 17, 2017
 *      Author: yduan
 */
#include "Balance.h"
#include <iostream>
#include <climits>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <string.h>
using namespace std;

void Balance::Initial(vector<string> slaves){
	fstream file;
	file.open("Balance.txt", fstream::out | fstream::trunc);
	if(!file){
		cout<<"can not initial Balance"<<endl;
		return;
	}
	for(size_t i=0;i<slaves.size();i++){
		file<<slaves[i]<<" "<<0<<endl;
	}
	file.flush();
	file.close();
}
string Balance::Pick(){
	fstream file;
	file.open("Balance.txt", fstream::in);
	if(!file) return "0.0.0.0:0";
	string slave;
	int count;
	int min=INT_MAX;
	string pick;
	map<string,size_t> balance;
	string line;
	while(getline(file,line)){
		istringstream iss(line);
		iss>>slave>>count;
//		cout<<slave<<"+"<<count<<endl;
		balance[slave]=count;
		if(count<min) {
			min=count;
			pick=slave;
		}
	}
	file.close();
	file.open("Balance.txt", fstream::out | fstream::trunc);

	for (map<string,size_t>::iterator it=balance.begin(); it!=balance.end(); ++it){
		if(it->first.compare(pick)==0) it->second++;
		file<<it->first<<" "<<it->second<<endl;
//		cout<<it->first<<"-"<<it->second<<endl;
	}
	file.flush();
	file.close();
	return pick;
}
Balance::Balance(){}
Balance::~Balance(){}

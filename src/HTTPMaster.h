/*
 * HTTPMaster.h
 *
 *  Created on: Apr 16, 2017
 *      Author: yduan
 */

#ifndef HTTPMASTER_H_
#define HTTPMASTER_H_
#include "Policy.h"
#include"HTTPServer.h"

class HTTPMaster : public HTTPServer{
	public:
		HTTPMaster(int);
		~HTTPMaster();
		void SetPolicy(list<Policy*>,list<Policy*>);
//		void SetSlaves(vector<string>);
		bool ReplyResponse();
	private:
		list<Policy*> inbound_policies;
		list<Policy*> outbound_policies;

//		vector<string> slaves;
		int BalancePickSlave();
		bool ParseRequest();

};



#endif /* HTTPMASTER_H_ */

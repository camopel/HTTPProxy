/*
 * Balance.h
 *
 *  Created on: Apr 17, 2017
 *      Author: yduan
 */

#ifndef BALANCE_H_
#define BALANCE_H_
#include <map>
#include <vector>
#include<string>
#include <sys/ipc.h>
#include <sys/shm.h>
using namespace std;
class Balance{
	public:
		static void Initial(vector<string>);
		static string Pick();
		Balance();
		~Balance();
	private:

};



#endif /* BALANCE_H_ */

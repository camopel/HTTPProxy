/*
 * Policy.h
 *
 *  Created on: Apr 16, 2017
 *      Author: yduan
 */

#ifndef POLICY_H_
#define POLICY_H_
using namespace std;
#include <string>
#include <list>
#include <netinet/in.h>
class Policy{
	public:
		enum Type{UD, IM, PT, KW, HT };
		Policy();
		Policy(unsigned int,unsigned int);
		Policy(int);
		Policy(string,Type);
		~Policy();
		void Prepare(string);
		static bool Filter(string,list<Policy*>);
		string toString();
	private:
		Type p_type;
		unsigned int p_ip;
		unsigned int p_mask;
		int p_port;
		string p_keyword;
		string p_host;

		unsigned int test_ip;
		int test_port;
		string test_host;
		string test_content;
		bool endWith(string const &, string const &);
};

#endif /* POLICY_H_ */

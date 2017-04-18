/*
 * HTTPServer.cpp
 *
 *  Created on: Apr 15, 2017
 *      Author: yduan
 */

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <ctime>

#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include "HTTPServer.h"
#include "Log.h"
using namespace std;

HTTPServer::HTTPServer(int p):ans_socket(0),ID(0),reqPort(0),req_Socket(0),buflen(0),poollen(0),reqIP(NULL),listen_socket(0){
	port=p;
	buf = new char[buf_sz];
	pool = new char[large_buf_sz];
}
HTTPServer::~HTTPServer(){
	close(listen_socket);
	close(ans_socket);
	free(buf);
	free(pool);
}

int HTTPServer::InitSocket(){
	string funcNm = "HTTPServer::InitSocket:";

	if((listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))<0){
		cout<<funcNm<<"Failed to create listen socket"<<endl;
		return -1;
	}

	listen_addr.sin_family = AF_INET;
	listen_addr.sin_port = htons(port);
	listen_addr.sin_addr.s_addr = INADDR_ANY;

	if((bind(listen_socket, (struct sockaddr*)&listen_addr, sizeof(listen_addr)))<0){
		cout<<funcNm<<"Failed to bind to port "<<port<<endl;
		return -1;
	}
	cout<<"Listen Port:"<<port<<endl;
	if(listen(listen_socket, 5)){
		cout<<funcNm<<"Listen on port "<<port<<" failed"<<endl;
		return -1;
	}

	return 0;
}
int HTTPServer::Run(){
	string funcName = "HTTPServer::Run:";
	if(InitSocket()<0){
		cout<<funcName<<"Failed to initialize socket"<<endl;
		return -1;
	}
	while(1){
		socklen_t ans_Len = sizeof(ans_Addr);
		ans_socket = accept(listen_socket, (struct sockaddr*)&ans_Addr, &ans_Len);
		if(ans_socket<0){
			cout<<funcName<<"Accept call failed"<<endl;
			return -1;
		}
		ID++;
		if(fork() == 0){
			HandleRequest();
			exit(0);
		}
		close(ans_socket);
	}

	return 0;
}
void HTTPServer::HandleRequest(){
//cout<<ID<<"...HTTPServer HanleRequest"<<endl;
	bzero(buf,buf_sz);
	buflen = recv(ans_socket, buf, buf_sz, 0);
	if(buflen>0){
		if(ParseRequest()){
			if(SendRequest()){
				if(GetResponse()){
					if(!ReplyResponse()) {
						Log("HTTPServer ReplyResponse return False!");
					}
					return;
				}
			}
		}
	}
	Send_notfound();
}
bool HTTPServer::ParseRequest(){
//cout<<ID<<"...HTTPServer virtual parseRequest"<<endl;
	return true;
}
bool HTTPServer::SendRequest(){
	struct sockaddr_in host_addr;
	bzero((char*)&host_addr,sizeof(host_addr));
	host_addr.sin_port=htons(reqPort);
	host_addr.sin_family=AF_INET;
	bcopy((char*)reqIP->h_addr,(char*)&host_addr.sin_addr.s_addr,reqIP->h_length);
	req_Socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	int status=connect(req_Socket,(struct sockaddr*)&host_addr,sizeof(struct sockaddr));
	if(status!=0) return false;
	size_t sentBytes = send(req_Socket,buf,buflen,0);
//cout<<ID<<"...HTTPServer sendRequest,sentBytes:"<<sentBytes<<",buflen:"<<buflen<<endl;
	return (sentBytes==buflen);
}
bool HTTPServer::GetResponse(){
	bzero(pool,large_buf_sz);
	poollen=0;
	char* tmp = pool;
	size_t needToRead=large_buf_sz;
	do {
		bzero(buf,buf_sz);
		buflen = recv(req_Socket, buf, buf_sz, 0);
//cout<<"recv buflen:"<<buflen<<endl;
		if(buflen>0){
			memcpy(tmp+poollen,buf,buflen);
			poollen+=buflen;
			if(needToRead==large_buf_sz){
				string str("");
				string str_found("");
				str.append(buf,buflen);
				size_t pos = str.find("Content-Length:");
				if(pos==std::string::npos) needToRead=0;
				else {
					size_t beg = pos+16;
					pos=str.find('\r',beg);
					size_t len = pos-beg;
					str_found = str.substr(beg, len);
					beg+=len;
					len = atoi(str_found.c_str());
					pos = str.find("\r\n\r\n",beg);
					needToRead=pos+4+len;
				}
				str.clear();
				str_found.clear();
			}
		}
	}while(buflen>0 && poollen<needToRead);
	return (poollen>0)?true:false;
//cout<<ID<<"...HTTPServer GetResponse:"<<poollen<<endl;
}
bool HTTPServer::ReplyResponse(){
cout<<ID<<"...HTTPServer::ReplyResponse:"<<poollen<<endl;
	size_t len = send(ans_socket,pool,poollen,0);
	return (len==poollen);
}
void HTTPServer::PrintRequestInfo(){
	struct in_addr addr;
	memcpy(&addr,reqIP->h_addr,4);
	cout<<ID<<"...Host:"<<reqHost<<" Port:"<<reqPort<<" IP:"<<inet_ntoa(addr)<<endl;
}
bool HTTPServer::PrintRequestHeaderAndCheckIfSupport(){
	string req(buf);
	istringstream iss(req);
	string method,url;
	iss>>method>>url;
	cout<<ID<<"..."<<method<<" "<<url<<endl;
	return (method.compare("GET")==0)?true:false;
}
void HTTPServer::Send_notfound(){
	ostringstream dataStream;
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[20];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
	string str;
	dataStream<<"HTTP/1.1 404 Not Found\r\n";
	dataStream<<"Content-Type: text/html\r\n";
	dataStream<<"Date: "<<buf<<"\r\n";
	dataStream<<"Connection: close\r\n\r\n";
	dataStream<<"<html>Not Allow</html>";
	str = dataStream.str();
	send(ans_socket,str.c_str(),str.size(),0);
	str.clear();
cout<<ID<<"...404 Not Found"<<endl;
}

# Distributed HTTP Proxy

This program written in C++, support simple HTTP proxy function with customized policy rules, like hostname, ip, port, keyward filter. Currently only support GET method. 

Proxy is splitted as master and slaves. Master handle request from browers, and send to slaves balanced by work load. Slaves in charge of request to remote server and send response to master which forword to browser. 

Policies are used on master side to do request filtering.
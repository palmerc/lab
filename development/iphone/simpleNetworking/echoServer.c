/*
 *  echoServer.c
 *  simpleNetworking
 *
 *  Created by Cameron Lowell Palmer on 20.12.09.
 *  Copyright 2009 Bird And Bear Productions. All rights reserved.
 *
 */

#include "echoServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#define ECHOPORT "50007"
#define BACKLOG 10

int errno;

int main(int argc, char *argv[]) {
	// define two addrinfo structs
	struct addrinfo hints, *results;
	struct sockaddr their_addr;
	socklen_t addr_size;
	int status, s, n;
	int yes = 1;
	
	// create socket
	s = socket(AF_INET, SOCK_STREAM, 0);
	
	// zero out the hints struct
	memset(&hints, 0, sizeof(hints));
	// both ipv4 and ipv6 support
	hints.ai_family = AF_UNSPEC;
	// tcp
	hints.ai_socktype = SOCK_STREAM;
	// fill out with my address since we are listening
	hints.ai_flags = AI_PASSIVE;
	
	
	if ((status = getaddrinfo(NULL, ECHOPORT, &hints, &results)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(1);
	}
	
	s = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
	if (s == -1) {
		fprintf(stderr, "socket error: %d\n", errno);
	}
	
	if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}
	
	// bind
	if ((status = bind(s, results->ai_addr, results->ai_addrlen)) == -1) {
		fprintf(stderr, "bind error: %d\n", errno);
	}
	
	// listen
	if ((status = listen(s, BACKLOG)) == -1) {
		fprintf(stderr, "listen error: %d\n", errno);
	}
	
	while (1) {
		char *recv_buf[1024];
		int bytes_recvd;
		
		// accept
		n = accept(s, &their_addr, &addr_size);
		if (n == -1) {
			fprintf(stderr, "accept error: %d\n", n);
		}
		
		bytes_recvd = recv(n, recv_buf, sizeof(recv_buf), 0);
		if (bytes_recvd == 0) {
			close(n);
		} else if (bytes_recvd == -1) {
			fprintf(stderr, "recv error: %d\n", n);
		} else {
			send(n, recv_buf, bytes_recvd, 0);
		}
	}
	close(s);
	freeaddrinfo(results);
	
	return 0;
}
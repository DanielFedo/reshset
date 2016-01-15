/*
 * TCPSocketsListener.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: user
 */

#include "TCPSocketsListener.h"

using namespace std;

void TCPSocketsListener::addSocketToList(TCPSocket* socket){
	this->sockets.push_back(socket);
}

void TCPSocketsListener::initSocketsList(vector<TCPSocket *> socketVec){
	this->sockets = socketVec;
}

TCPSocket* TCPSocketsListener::listenToSocket(int timeout){
	struct timeval tv = {timeout, 0};
	tSockets::iterator socketsIter;
	tSockets::iterator socketsEnd = sockets.end();

	fd_set fdset;
	FD_ZERO(&fdset);
	int highfd = 0;

	// Fill sockets with file descriptors
	for (socketsIter = sockets.begin();
			socketsIter != socketsEnd; socketsIter++){
		highfd++;
		FD_SET((*socketsIter)->socket_fd, &fdset);
	}

	// Select
	int result;

	if (timeout > 0) {
		result = select(sizeof(fdset) * 8, &fdset, NULL, NULL, &tv);
	} else {
		result = select(sizeof(fdset) * 8, &fdset, NULL, NULL, NULL);
	}

	if (result)
	{
		for (int i = 0; i < highfd; i++) {
			TCPSocket* tmpSocket = sockets[i];

			if (FD_ISSET(tmpSocket->socket_fd, &fdset)){
				return tmpSocket;
			}
		}
	}

	return NULL;
}

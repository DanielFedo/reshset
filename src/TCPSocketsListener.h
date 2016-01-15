/*
 * TCPSocketsListener.h
 *
 *  Created on: Jan 13, 2016
 *      Author: user
 */

#ifndef TCPSOCKETSLISTENER_H_
#define TCPSOCKETSLISTENER_H_

#include <vector>
#include <stdio.h>
#include <iostream>
#include "TCPSocket.h"

class TCPSocketsListener {

private:
	typedef vector<TCPSocket*> tSockets;
	tSockets sockets;

public:
	virtual ~TCPSocketsListener();

	/*
	 * Add the socket to the socket list, and start to listen to it
	 */
	void addSocketToList(TCPSocket* socket);

	/*
	 * Add the given sockets to the socket list to be listen on
	 */
	void initSocketsList(vector<TCPSocket*> socketVec);

	/*
	 * This method checks to see if any of the previous given sockets are ready for reading
	 * It returns one of the Sockets that are ready.
	 */
	TCPSocket* listenToSocket(int timeout=0);
};



#endif /* TCPSOCKETSLISTENER_H_ */

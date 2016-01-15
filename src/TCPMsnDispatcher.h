/*
 * dispatcher.h
 *
 *  Created on: Jan 15, 2016
 *      Author: user
 */

#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#include <strings.h>
#include <map>
#include <vector>
#include <set>
#include "MThread.h"
#include "TCPSocket.h"
#include "Broker.h"
#include "TCPSocketsListener.h"
#include "TCPMessengerProtocol.h"

using namespace std;

/**
 * The dispatcher server reads incoming commands from open peers and performs the required operations
 */
class TCPMsnDispatcher: public MThread{
	typedef map<string,TCPSocket*> peersMap;;

	bool isRunning;
	TCPSocketsListener* socketsListener;

public:
	peersMap *openedPeers;
	peersMap *busyPeers;

	/**
	 * constructor that receive a reference to the parent messenger server
	 */
	TCPMsnDispatcher(peersMap* openedPeers, peersMap* busyPeers);

	void addPeer(TCPSocket *peerSocket);

	/**
	 * The Dispatcher main loop
	 */
	void run();

	/**
	 * End the dispatcher main loop
	 */
	void close();

	TCPSocket* searchOpenPeer(string address);

	TCPSocket* searchBusyPeer(string address);
};

#endif /* DISPATCHER_H_ */

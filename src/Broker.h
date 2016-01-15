/*
 * Broker.h
 *
 *  Created on: Jan 15, 2016
 *      Author: user
 */

#ifndef BROKER_H_
#define BROKER_H_

#include <map>
#include <strings.h>
#include "TCPSocket.h"
#include "TCPSocketsListener.h"
#include "TCPMessengerProtocol.h"
#include <vector>
#include <string.h>


using namespace std;

class Broker {

	typedef map<string, TCPSocket*> peersMap;
	peersMap* openPeers;
	peersMap* busyPeers;
	vector<TCPSocket *> session;
	TCPSocket *peer1;
	TCPSocket *peer2;

public:
	bool isAlive;

	Broker(TCPSocket* p1, TCPSocket* p2, peersMap* openPeersMaps, peersMap* busyPeersMaps);
	Broker();

	void Action();

	virtual ~Broker();
};

#endif /* BROKER_H_ */

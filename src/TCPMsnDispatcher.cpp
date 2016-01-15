/*
 * dispatcher.cpp
 *
 *  Created on: Jan 15, 2016
 *      Author: user
 */

#include "TCPMsnDispatcher.h"


TCPMsnDispatcher::TCPMsnDispatcher(peersMap* openedPeers, peersMap* busyPeers){
	this->socketsListener = new TCPSocketsListener();
	this->openedPeers = openedPeers;
	this->busyPeers = busyPeers;
}

void TCPMsnDispatcher::run(){
	this->isRunning = true;
	vector<TCPSocket*>* openedPeersSockets = new vector<TCPSocket*>();

	int cmd = -1;
	peersMap::iterator peerIter;

	while (this->isRunning){
		for (peerIter = this->openedPeers->begin();
				peerIter != this->openedPeers->end();
				peerIter++){
			openedPeersSockets->push_back(peerIter->second);
		}

		socketsListener->initSocketsList(*openedPeersSockets);

		TCPSocket* openerSocket = NULL;
		openerSocket = socketsListener->listenToSocket(2);

		if (openerSocket == NULL){
			openedPeersSockets->clear();
			continue;
		}

		openerSocket->recv((char*)&cmd, 4);
		cmd = ntohl(cmd);

		if (cmd == OPEN_SESSION_WITH_PEER){
			char buffer[50];
			bzero(buffer, 50);
			int len = 0;

			openerSocket->recv((char*)&len, 4);
			len = ntohl(len);
			openerSocket->recv(buffer, len);

			TCPSocket* receiverSocket = NULL;
			receiverSocket = searchOpenPeer(buffer);

			if (receiverSocket == NULL){
				cout << "Peer was not found :(" << endl;
				continue;
			}
			else {
				cmd = htonl(cmd);
				openerSocket->send((char*)&cmd, 4);
				len = htonl(len);
				openerSocket->send((char*)&len, 4);

				string receiver = receiverSocket->fromAddr();
				string opener = openerSocket->fromAddr();

				openerSocket->send(opener.data(), opener.length());

				cout << "Open session command: " << opener << " :: " << receiver << endl;
				cmd = htonl(SESSION_ESTABLISHED);
				receiverSocket->send((char*)&cmd, 4);

				this->busyPeers->insert(pair<string, TCPSocket*>(opener, openerSocket));
				this->busyPeers->insert(pair<string, TCPSocket*>(receiver, receiverSocket));

				this->openedPeers->erase(opener);
				this->openedPeers->erase(receiver);

				new Broker(openerSocket, receiverSocket, this->openedPeers, this->busyPeers);
			}
		}
	}

}

void TCPMsnDispatcher::close(){
	// TODO: Stop the dispatcher
	isRunning = false;
	waitForThread();
	cout << "Dispatcher closed" << endl;
}

void TCPMsnDispatcher::addPeer(TCPSocket* peerSocket){
	// TODO: Add peer

}

TCPSocket* TCPMsnDispatcher::searchOpenPeer(string address){
	peersMap::iterator iter = this->openedPeers->find(address);

	return iter->second;
}

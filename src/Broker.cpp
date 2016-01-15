/*
 * Broker.cpp
 *
 *  Created on: Jan 15, 2016
 *      Author: user
 */

#include "Broker.h"

Broker::Broker() {
	// TODO Auto-generated constructor stub
	this->openPeers = NULL;
	this->busyPeers = NULL;

	this->peer1 = NULL;
	this->peer2 = NULL;

	this->isAlive = true;
}

Broker::~Broker() {
	// TODO Auto-generated destructor stub
}

Broker::Broker(TCPSocket* p1, TCPSocket* p2, peersMap* openPeersMaps, peersMap* busyPeersMaps){
	cout << "The Broker has Started" << endl;

	this->isAlive = true;
	this->peer1 = p1;
	this->peer2 = p2;

	this->busyPeers = busyPeersMaps;
	this->openPeers = openPeersMaps;

	this->session.push_back(peer1);
	this->session.push_back(peer2);

	this->Action();
}

void Broker::Action() {
	int command = 0;
	int rc = 0;
	this->isAlive = true;

	// The main loop that manages the open peers
	while(this->isAlive)
	{
		TCPSocketsListener listener;
		TCPSocket* sender = NULL;
		TCPSocket* receiver = NULL;

		listener.initSocketsList(this->session);
		sender = listener.listenToSocket(1);

		if (sender == NULL){
			continue;
		} else {
			command = 0;
			rc = sender->recv((char*)&command, 4);

			if (rc < 4){
				break;
			}
			if(sender == peer1){
				receiver = peer2;
			} else {
				receiver = peer1;
			}

			string recevierIpAndPort = receiver->fromAddr();
			string senderIpAndPort = sender->fromAddr();

			// React to each message according to the protocol
			switch (ntohl(command))
			{
				case SEND_MSG_TO_PEER:
				{
					char buffer[100];
					int len;
					len = 0;

					memset((void*)buffer, 0, 100);
					sender->recv((char*)&len, 4);
					len = ntohl(len);
					sender->recv(buffer, len);

					string s(buffer);
					int command = htonl (SEND_MSG_TO_PEER);
					int add = htonl (s.length());

					receiver->send((char*)&command, 4);
					receiver->send((char*)&add, 4);
					receiver->send(s.data(), s.length());

					cout << "sending MSG:" << senderIpAndPort << "-->" << recevierIpAndPort << endl;
					break;
				}
				case CLOSE_SESSION_WITH_PEER: {
					int command1 = htonl (CLOSE_SESSION_WITH_PEER);
					receiver->send((char*)&command1, 4);

					peersMap::iterator iter12 = openPeers->begin();
					cout<<"clients b4:"<<endl;
					for(;iter12 != openPeers->end();iter12++) {
						string address= iter12->first;
						cout<<address<<endl;
					}

					busyPeers->erase(senderIpAndPort);
					busyPeers->erase(recevierIpAndPort);
					openPeers->insert(pair<string , TCPSocket*> (recevierIpAndPort, receiver));
					openPeers->insert(pair<string , TCPSocket*> (senderIpAndPort, sender));

					peersMap::iterator iter13 = openPeers->begin();
					cout<<"clients adf:"<<endl;
					for(;iter13 != openPeers->end();iter13++) {
						string address= iter13->first;
						cout<<address<<endl;
					}

					this->isAlive = false;
					cout << "Closing session:" << senderIpAndPort << "-->" << recevierIpAndPort << endl;
					cout << "Closing broker:" << senderIpAndPort << "-" << recevierIpAndPort << endl;
					break;
				}
			}
		}
	}
}




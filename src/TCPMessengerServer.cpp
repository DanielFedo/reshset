
#include "TCPMessengerServer.h"
#include "TCPMessengerProtocol.h"

/***********************   TCPMessengerServer implementation ******************************/

TCPMessengerServer::TCPMessengerServer(){
	tcpSocket = new TCPSocket(MSNGR_PORT);
	isRunning = false;
	dispatcher = NULL;

	this->start();
}


void TCPMessengerServer::run(){
	// Init dispatcher
	dispatcher = new TCPMsnDispatcher(&openPeers, &busyPeers);
	dispatcher->start();

	isRunning = true;
	TCPSocket* peerSocket;
	string peerUsername;
	string peerPassword;

	// While the server is running
	while (isRunning){
		// Get a connection from a peer
		peerSocket = tcpSocket->listenAndAccept();

		// If connection is established
		if (peerSocket != NULL) {
			dispatcher->addPeer(peerSocket);
			//openedPeers[peerSocket->destIpAndPort()] = peerSocket;
			cout << "Connected to: " << peerSocket->fromAddr() << endl;
		}
	}
}

void TCPMessengerServer::close(){
	cout << "closing server" << endl;

	dispatcher->close();

	// Disconnect all opened peers
	peersMap::iterator peersIterator;
	peersMap::iterator peersEnd = this->openPeers.end();

	for (peersIterator = this->openPeers.begin();
		 peersIterator != peersEnd; peersIterator++){
		cout << "Closing: " << peersIterator->second << endl;
		peersIterator->second->cclose();
	}

	// Disconnect all busyPeers

	dispatcher->waitForThread();
	peersEnd = this->busyPeers.end();

	for (peersIterator = this->busyPeers.begin();
		 peersIterator != peersEnd; peersIterator++){
		cout << "Closing: " << peersIterator->second << endl;
		peersIterator->second->cclose();
	}

	isRunning = false;
	tcpSocket->cclose();
	dispatcher->waitForThread();

	cout << "server closed" << endl;
}

TCPMessengerServer::~TCPMessengerServer(){
}

int TCPMessengerServer::readCommandFromPeer(TCPSocket* peer){
	int command = 0;
	int rt = peer->recv((char*) &command, 4);

	if (rt < 1){
		return rt;
	}

	command = ntohl(command);

	return command;
}

string TCPMessengerServer::readDataFromPeer(TCPSocket* peer){
	//TODO: read a string from socket
	return "";
}

void TCPMessengerServer::sendCommandToPeer(TCPSocket* peer, int command){
	//TODO: send command to socket
}

void TCPMessengerServer::sendDataToPeer(TCPSocket* peer, string msg){
	//TODO: send string to socket
}

void TCPMessengerServer::listPeers(){
	peersMap::iterator iter;

	cout << "Peers list: " << endl;

	for (iter = this->openPeers.begin();
		 iter != this->openPeers.end(); iter++){
		string add = iter->first;

		cout << add << endl;
	}

}



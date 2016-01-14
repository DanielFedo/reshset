
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
	dispatcher = new TCPMsnDispatcher(this);
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
	tOpenedPeers::iterator peersIterator;
	tOpenedPeers::iterator peersEnd = dispatcher->openedPeers.end();

	for (peersIterator = dispatcher->openedPeers.begin();
		 peersIterator != peersEnd; peersIterator++){
		cout << "Closing: " << peersIterator->second << endl;
		peersIterator->second->cclose();
	}

	// Disconnect all busyPeers

	dispatcher->waitForThread();
	peersEnd = dispatcher->busyPeers.end();

	for (peersIterator = dispatcher->busyPeers.begin();
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
	tOpenedPeers::iterator iter;

	cout << "Peers list: " << endl;

	for (iter = this->dispatcher->openedPeers.begin();
		 iter != this->dispatcher->openedPeers.end(); iter++){
		string add = iter->first;

		cout << add << endl;
	}

}


/***********************   Dispatcher implementation ******************************/

TCPMsnDispatcher::TCPMsnDispatcher(TCPMessengerServer *mesgr){
	//this->openedPeers = new tOpenedPeers();
	//this->busyPeers = new tOpenedPeers();
	this->messenger = mesgr;
}

void TCPMsnDispatcher::run(){
	this->isRunning = true;
	vector<TCPSocket*>* openedPeersSockets = new vector<TCPSocket*>();

	int cmd = -1;
	tOpenedPeers::iterator peerIter;

	while (this->isRunning){
		for (peerIter = this->openedPeers.begin();
				peerIter != this->openedPeers.end();
				peerIter++){
			openedPeersSockets->push_back(peerIter->second);
		}


	}

}

void TCPMsnDispatcher::close(){
	// TODO: Stop the dispatcher
}

void TCPMsnDispatcher::addPeer(TCPSocket* peerSocket){
	// TODO: Add peer

}

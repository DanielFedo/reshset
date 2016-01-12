
#include "TCPMessengerServer.h"
#include "TCPMessengerProtocol.h"

/***********************   TCPMessengerServer implementation ******************************/

TCPMessengerServer::TCPMessengerServer(){
	tcpSocket = new TCPSocket(MSNGR_PORT);
	isRunning = false;
	dispatcher = NULL;

	run();
}


void TCPMessengerServer::run(){
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
		}
	}
}

void TCPMessengerServer::close(){
	//TODO: close the server
	cout << "closing server" << endl;

	isRunning = false;
	tcpSocket->cclose();

	//TODO: Disconnect users

	// Disconnect peers
	tOpenedPeers::iterator peersIterator = dispatcher->openedPeers.begin();
	tOpenedPeers::iterator peersEnd = dispatcher->openedPeers.end();

	for (; peersIterator != peersEnd; peersIterator++){
		(*peersIterator).second->cclose();
	}

	dispatcher->waitForThread();
	peersIterator = dispatcher->openedPeers.begin();
	peersEnd = dispatcher->openedPeers.end();

	for (; peersIterator != peersEnd; peersIterator++){
		delete (*peersIterator).second;
	}

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
	//TODO: print the connected peers
}


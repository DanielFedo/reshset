//============================================================================
// Name        : TCP Messenger Server
// Author      : Eliav Menachi
// Version     :
// Copyright   :
// Description : TCP Messenger application
//============================================================================

#ifndef TCPMESSENGERSERVER_H__
#define TCPMESSENGERSERVER_H__

#include <strings.h>
#include <map>
#include <vector>
#include <set>
#include "MThread.h"
#include "TCPSocket.h"

using namespace std;
typedef map<string, TCPSocket*> tOpenedPeers;

class TCPMessengerServer;

/**
 * The dispatcher server reads incoming commands from open peers and performs the required operations
 */
class TCPMsnDispatcher: public MThread{
	TCPMessengerServer* messenger;

public:
	tOpenedPeers openedPeers;
	tOpenedPeers busyPeers;

	/**
	 * constructor that receive a reference to the parent messenger server
	 */
	TCPMsnDispatcher(TCPMessengerServer* mesgr);

	void addPeer(TCPSocket *peerSocket);

	/**
	 * The Dispatcher main loop
	 */
	void run();
};

class TCPMessengerServer{
	//TODO: add class properties
	TCPSocket* tcpSocket;
	TCPMsnDispatcher* dispatcher;
	bool isRunning;

public:
	/**
	 * Build the Messenger server
	 */
	TCPMessengerServer();
	~TCPMessengerServer();

	 /*
	  * Run Server
	 */
	void run();

	/**
	 * print out the list of connected clients (for debug)
	 */
	void listPeers();

	/**
	 * close the server
	 */
	void close();

	/**
	 * read command from peer
	 */
	static int readCommandFromPeer(TCPSocket* peer);

	/**
	 * read data from peer
	 */
	static string readDataFromPeer(TCPSocket* peer);

	/**
	 * send command to peer
	 */
	static void sendCommandToPeer(TCPSocket* peer, int command);

	/**
	 * send data to peer
	 */
	static void sendDataToPeer(TCPSocket* peer, string msg);

private:

};

#endif

#include "enet.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <cstring>
#include <thread>

using namespace std;

volatile bool cquit = false;

void (*ccevent)(ENetPeer*, ENetHost*, ENetEvent) = nullptr;
void (*crevent)(ENetPeer*, ENetHost*, ENetEvent) = nullptr;
void (*cdevent)(ENetPeer*, ENetHost*, ENetEvent) = nullptr;

void client::connect(void (*a)(ENetPeer*, ENetHost*, ENetEvent)){ccevent = a;}
void client::recieve(void (*a)(ENetPeer*, ENetHost*, ENetEvent)){crevent = a;}
void client::disconnect(void (*a)(ENetPeer*, ENetHost*, ENetEvent)){cdevent = a;}

void client::send(std::string data, ENetPeer* peer){
	ENetPacket *packet = enet_packet_create(data.c_str(), data.size()+1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer,0,packet);
}

void runThread(string ip, int port, bool retry){
	ENetHost *client;
	ENetPeer *peer;
	ENetAddress address;
	client = enet_host_create(NULL,1,2,57600/8, 14400/8);
	enet_address_set_host(&address, ip.c_str());
	address.port = port;
	peer = enet_host_connect(client, &address,2,0);
	ENetEvent talking;
	while(!cquit && (enet_host_service(client, &talking, 10000)>0 || retry)){
		switch(talking.type){
		case ENET_EVENT_TYPE_CONNECT:
			if(ccevent != nullptr)
				ccevent(peer, client, talking);
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			if(crevent != nullptr)
				crevent(peer, client, talking);		
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			if(cdevent != nullptr)
				cdevent(peer, client, talking);
			break;
		}
	}
	enet_peer_reset(peer);
	enet_host_destroy(client);
	enet_deinitialize();
}

std::thread cthr;

void client::init(string ip, int port, bool retry) {
	cthr = std::thread(runThread, ip, port, retry);
}

void client::quit(){
	cquit = true;
	cthr.join();
}

#pragma once
#include <iostream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <SFML/Network.hpp>

#include "Algorithms/guid.h"
#include "Algorithms/StateMachine.h"

#include "SendBinary.h"
#include "NetworkPacket.h"

class Server : public BaseState
{
	virtual void enterState(BaseState* previousState);

	virtual void updateState(float delta);

	virtual bool isValidNextState(BaseState* state);

	virtual void exitState(BaseState* nextState);
};

class Client : public BaseState
{
	virtual void enterState(BaseState* previousState);

	virtual void updateState(float delta);

	virtual bool isValidNextState(BaseState* state);

	virtual void exitState(BaseState* nextState);
};

class NetworkManager : public StateMachine
{

public:
	NetworkManager();
	~NetworkManager();
	sf::UdpSocket socket;
	std::string type;


	// SETTINGS
	sf::IpAddress ownAddress = sf::IpAddress::getPublicAddress();
	sf::IpAddress serverAddress = "umami.ddns.net";
	unsigned short serverPort = 54000;

	//fields blah
	sf::IpAddress serverIP;
	Guid networkID;
	SendBinary sendBinary;
	int temp_FindServerAndLoadMapState = 0;

	std::unordered_map<Guid, std::pair<sf::IpAddress, unsigned short>> networkGameObjects;
	std::unordered_map<Guid, std::pair<sf::IpAddress, unsigned short>> peers;
	std::unordered_map<Guid, std::stack<NetworkPlayerEvent>> incomingEvents;
	std::unordered_map<Guid, std::stack<NetworkPlayerEvent>> outgoingEvents;
	std::unordered_map<Guid, float> timeOut;

	// GameLoop
	void processSocket(float dt);
	void sendEvents();

	void setType(std::string type);
	void reset();

	//playerInterace
	std::stack<NetworkPlayerEvent> getEvents(Guid entityID);
	void queueEvent(NetworkPlayerEvent& event);


	void broadcastAlive();
	void findServer();

	

	void sendInitAlive(Guid entity);

	void sendToPeers(const Guid& guid, const sf::IpAddress& ip, const unsigned short& port, const PacketType& type);

	void AddGameEntity(Guid entity);
	void broadcastGameObjects();

private:
	
	void processPacketBody(sf::Packet& packet, PacketType& type, sf::IpAddress sender, unsigned short port);

	void sendGameMap(Guid entity);	
};


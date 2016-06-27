#include "stdafx.h"
#include "NetworkManager.h"
#include "GameSettingConstants.h"
#include "Console.h"

#include <fstream>



NetworkManager::NetworkManager()
{
	socket.setBlocking(false);
	GuidGenerator gen;
	networkID = gen.newGuid();

	Command c = [this](std::vector<std::string> str) -> std::string {
		std::string returnString;
		if (str.size() == 1)
		{
			std::vector<std::string> input;
			tokenize(str.front(),input, ":", true);
			auto tmp = sf::IpAddress(input.front());
			if (tmp != sf::IpAddress::None)
			{
				this->serverAddress = tmp;
			}
			else
			{
				returnString = "Invalid Server address. \n";
			}
			if (input.size() == 2)
			{
				unsigned int port = std::stoi(input.back());
				if (port >= 49152 && port <= 65535)
				{
					this->serverPort = port;
				}
				else
				{
					returnString = "Invalid port, must be between 49152 and 65535. \n";
				}
			}
		}
		std::ostringstream ss;
		ss << "Server address is set to: " << serverAddress << ":" << std::dec << serverPort;
		returnString = ss.str();
		return returnString;
	};
	Console::Instance().Add("server", c);

	c = [this](std::vector<std::string> str) -> std::string {
		std::string returnString;
		std::ostringstream ss;
		ss << "This client external address is:  " << ownAddress << ":" << std::dec << socket.getLocalPort();
		returnString = ss.str();
		return returnString;
	};
	Console::Instance().Add("ip", c);
}

void NetworkManager::setType(std::string type)
{
	this->type = type;
	if (type == "server")
	{
		socket.bind(serverPort);
	}
	if (type == "client")
	{
		socket.bind(socket.AnyPort);
	}
}

NetworkManager::~NetworkManager()
{
	socket.unbind();
}

void NetworkManager::reset()
{
	this->type = "single";
	this->socket.unbind();

	networkGameObjects.clear();
	peers.clear();
	incomingEvents.clear();
	outgoingEvents.clear();
	timeOut.clear();

	serverIP = sf::IpAddress::None;
	sendBinary = SendBinary();
	temp_FindServerAndLoadMapState = 0;
	Console::Instance() << "Debug: networkManager.reset()" << std::endl;
}

void NetworkManager::processSocket(float dt)
{
	sf::Packet packet;
	sf::IpAddress sender;
	unsigned short port;

	for (auto& t : timeOut)
	{ 
		t.second += dt;
	}

	if (sendBinary.isSending == true)
	{
		if (sendBinary.timeoutClock.getElapsedTime().asMilliseconds() >= 300)
		{
			sf::Packet resPacket = sendBinary.sendChunk();
			socket.send(resPacket, sendBinary.receiver, sendBinary.port);
		}
	}

	while (true)
	{
		if (socket.receive(packet, sender, port) == sf::UdpSocket::Done)
		{
			NetworkHeader header;
			if (packet >> header)
			{
				processPacketBody(packet,header.type, sender,port);
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
}

void NetworkManager::processPacketBody(sf::Packet& packet, PacketType& type, sf::IpAddress sender, unsigned short port)
{
	switch (type)
	{
		case PacketType::p_UPDATE:
		{
			NetworkPlayerEvent event;
			packet >> event;
			incomingEvents[event.entityID].emplace(event);
			timeOut[event.entityID] = 0;
			break;
		}

		case PacketType::p_NEW:
		{
			NetworkEntityInfo update;
			packet >> update;
			if (networkGameObjects.count(update.entityID) == 0)
			{
				Console::Instance() << "GameUpdate: Added new Player " << update.entityID << " from " << sender << ":" << std::dec << port << std::endl;
				networkGameObjects[update.entityID] = std::make_pair(sender, port);
				this->broadcastGameObjects();
			}
			break;
		}
		case PacketType::n_NEW:
		{
			NetworkEntityInfo update;
			packet >> update;
			if (this->peers.count(update.entityID) == 0 && update.entityID != networkID)
			{
				this->peers.insert_or_assign(update.entityID, std::make_pair(update.ip, update.port));
				//this->peers[update.entityID] = std::make_pair(update.ip, update.port);
				Console::Instance() << "NetworkUpdate: Found new GameClient " << update.entityID << " at " << sender << ":" << std::dec << port << std::endl;
				NetworkHeader resHeader;
				NetworkEntityInfo resUpdate;
				resHeader.type = PacketType::n_ACK;
				resUpdate.entityID = this->networkID;
				resUpdate.ip = this->ownAddress;
				resUpdate.port = this->socket.getLocalPort();
				sf::Packet resPacket;
				resPacket << resHeader;
				resPacket << resUpdate;
				this->socket.send(resPacket, sender, port);
				if (this->type == "server")
				{
					sendGameMap(update.entityID);
				}				
				sendToPeers(update.entityID, sender, port, PacketType::n_NEW);
			}
			break;
		}
		case PacketType::n_ACK:
		{
			NetworkEntityInfo update;
			packet >> update;
			serverIP = sender;
			if (this->peers.count(update.entityID) == 0)
			{
				this->peers[update.entityID] = std::make_pair(sender, port);
				Console::Instance() << "NetworkUpdate: Found GameServer with " << update.entityID << " at " << sender << ":" << std::dec << port << std::endl;
				this->temp_FindServerAndLoadMapState = 1;
				this->broadcastGameObjects();
			}
			break;
		}
		case PacketType::n_ALIVE:
		{
			NetworkEntityInfo update;
			packet >> update;
			if (this->peers.count(update.entityID) == 0)
			{
				Console::Instance() << "NetworkUpdate: New Alive from " << sender << ":" << std::dec << port << std::endl;
			}
			else
			{
				this->timeOut[update.entityID] = 0;
				Console::Instance() << "NetworkUpdate: Still Alive from " << sender << ":" << std::dec << port << std::endl;
			}
			break;
		}
		case PacketType::b_NEW:
		{
			NetworkBinary bin;
			packet >> bin;
			if (this->sendBinary.counter == 0 && this->sendBinary.totalPacketNumber == 0)
			{
				Console::Instance() << "GameUpdate: New map with " << sendBinary.totalPacketNumber << " packets " << std::endl;
				this->sendBinary.totalPacketNumber = bin.totalPacketNumber;
			}
			if (this->sendBinary.counter == bin.packetNumber)
			{
				Console::Instance() << "GameUpdate: New mapchunk " << sendBinary.counter << " / " <<sendBinary.totalPacketNumber << std::endl;
				this->sendBinary.buffer.insert(sendBinary.buffer.end(), &bin.buffer[0], &bin.buffer[bin.lastValidByte]);
				++this->sendBinary.counter;
			}
			if (this->sendBinary.counter < this->sendBinary.totalPacketNumber)
			{
				sf::Packet resPacket;
				NetworkHeader resHeader;
				resHeader.type = PacketType::b_ACK;
				resPacket << resHeader;
				this->socket.send(resPacket, sender, port);
			}
			else
			{
				std::ofstream file(GAME_MAP_NAME_REMOTE, std::ios::binary | std::ios::ate);
				if (file.write(this->sendBinary.buffer.data(), this->sendBinary.buffer.size()))
				{
					Console::Instance() << "GameUpdate: Finished loading map" << std::endl;
					this->temp_FindServerAndLoadMapState = 2;
					this->sendBinary = SendBinary();
				}
				sf::Packet resPacket;
				NetworkHeader resHeader;
				resHeader.type = PacketType::b_END;
				resPacket << resHeader;
				this->socket.send(resPacket, sender, port);
			}
			break;
		}
		case PacketType::b_ACK:
		{
			//Console::Instance() << "GameUpdate: received m_ACK, sending next chunk " << std::endl;
			if (this->sendBinary.receiver == sender && this->sendBinary.port == port)
			{
				this->sendBinary.advanceToNextChunk();
				sf::Packet resPacket = sendBinary.sendChunk();
				this->socket.send(resPacket, sendBinary.receiver, sendBinary.port);
			}
			break;
		}
		case PacketType::b_END:
		{
			//Console::Instance() << "GameUpdate: Last map package received, stop sending" << std::endl;
			this->sendBinary = SendBinary();
		}
	}
}

void NetworkManager::sendToPeers(const Guid& guid, const sf::IpAddress& ip, const unsigned short& port, const PacketType& type)
{
	auto peer = peers.begin();
	while(peer != peers.end())
	{
		Console::Instance() << " send to " << peer->second.first << ":" << std::dec << peer->second.second << std::endl;
		NetworkEntityInfo resUpdate;
		resUpdate.entityID = guid;
		resUpdate.ip = ip;
		resUpdate.port = port;
		sf::Packet packet;
		NetworkHeader header;
		header.type = type;
		packet << header;
		packet << resUpdate;
		socket.send(packet, peer->second.first, peer->second.second);

		auto it = timeOut.find(peer->first);
		if (it != timeOut.end() && it->second >= PLAYER_TIMEOUT)
		{
			timeOut.erase(it);
			auto itt = peers.find(peer->first);
			peer = peers.erase(itt);
		}
		else {
			++peer;
		}
	}
}

void NetworkManager::broadcastGameObjects()
{
	Console::Instance() << "NetworkUpdate: BroadCast Players " << std::endl;
	for (auto& ob : networkGameObjects)
	{
		sendToPeers(ob.first, ob.second.first, ob.second.second, PacketType::p_NEW);
	}
}

void NetworkManager::broadcastAlive()
{
	sendToPeers(networkID, this->ownAddress, this->socket.getLocalPort(), PacketType::n_ALIVE);
}

void NetworkManager::sendInitAlive(Guid entity)
{
	NetworkEntityInfo update;
	update.entityID = entity;
	update.ip = ownAddress;
	update.port = socket.getLocalPort();
	NetworkHeader header;
	header.type = PacketType::p_NEW;
	sf::Packet packet;
	
	packet << header;
	packet << update;
	
	socket.send(packet, serverIP, serverPort);	

	Console::Instance() << "GameUpdate: Sending Player: " << update.entityID << " to known Server: " << serverIP << ":" << std::dec << serverPort << std::endl;
}

void NetworkManager::findServer()
{
	NetworkHeader header;
	header.type = PacketType::n_NEW;
	NetworkEntityInfo update;
	update.entityID = this->networkID;
	update.ip = this->ownAddress;
	update.port = this->socket.getLocalPort();

	sf::Packet packet;
	packet << header;
	packet << update;
	
	Console::Instance() << "NetworkUpdate: Searching Server at " << serverAddress << ":" << std::dec << serverPort << std::endl;
	Console::Instance() << "\t Own Address is " << ownAddress << ":" << std::dec << socket.getLocalPort() << std::endl;
	this->socket.send(packet, serverAddress, serverPort);
}

std::stack<NetworkPlayerEvent> NetworkManager::getEvents(Guid entityID)
{
	std::unordered_map<Guid, std::stack<NetworkPlayerEvent>>::iterator it;
	std::stack<NetworkPlayerEvent> tmp;
	while(true)
	{
		it = incomingEvents.find(entityID);
		if (it != incomingEvents.end())
		{
			tmp = it->second;
			incomingEvents.erase(it);
		}
		else
		{
			break;
		}
	}
	return tmp;
}

void NetworkManager::sendEvents()
{
	for (std::unordered_map<Guid, std::stack<NetworkPlayerEvent>>::iterator it = outgoingEvents.begin(); it != outgoingEvents.end(); ++it)
	{
		while(!it->second.empty())
		{
			sf::Packet packet;
			NetworkPlayerEvent event = it->second.top();
			it->second.pop();
			NetworkHeader header;
			header.type = PacketType::p_UPDATE;
			packet << header;
			packet << event;

			for (auto& rem : peers)
			{
				if(rem.first != networkID)
					socket.send(packet, rem.second.first, rem.second.second);
			}
		}
	}
}

void NetworkManager::queueEvent(NetworkPlayerEvent& event)
{
	this->outgoingEvents[event.entityID].emplace(event);
}

void NetworkManager::AddGameEntity(Guid entity)
{
	Console::Instance() << "GameUpdate: Added own Player" << std::endl;
	this->networkGameObjects[entity] = std::make_pair(ownAddress, socket.getLocalPort());
}

void NetworkManager::sendGameMap(Guid peer)
{
	std::ifstream file(GAME_MAP_NAME, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::hash<Guid> hashOf;
	for(auto& it : peers)
	{
			//std::cout << peer() << std::endl;
		std::cout << "peersID " << it.first << std::endl;
		std::cout << "peersHash " << hashOf(it.first) << std::endl;
	}
	std::cout << "peerID " << peer << std::endl;
	std::cout << "peerHash " << hashOf(peer) << std::endl;

	std::vector<char> buffer(size);
	if (file.read(buffer.data(), size))
	{
		sendBinary.addBinary(buffer);
		auto it = peers.find(peer);
		sendBinary.receiver = it->second.first;
		sendBinary.port = it->second.second;
		sf::Packet packet = sendBinary.sendChunk();
		
		Console::Instance() << "GameUpdate: Sending map to " << it->second.first << ":" << std::dec << it->second.second << std::endl;
		socket.send(packet, sendBinary.receiver, sendBinary.port);
	}
}
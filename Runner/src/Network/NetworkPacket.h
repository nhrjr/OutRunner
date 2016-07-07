#pragma once
//this player is doing something
struct NetworkPlayerEvent
{
	Guid entityID;
	//unsigned short eventType;
	float x;
	float y;
	float angle;
	int hitpoints;
	int action;
	int alternateAction;
	unsigned int equippedWeapon;
};

// This entity lives at this address
struct NetworkEntityInfo
{
	Guid entityID;
	unsigned short port;
	sf::IpAddress ip;
};

struct NetworkBinary
{
	unsigned int packetNumber;
	unsigned int totalPacketNumber;
	unsigned int lastValidByte;
	unsigned char buffer[NETWORK_BINARYBUFFER_SIZE];
};

//types of packet body
enum class PacketType
{
	n_NEW,
	n_ACK,
	n_ALIVE,

	p_UPDATE,
	p_NEW,
	p_ACK,

	b_NEW,
	b_ACK,
	b_END,
};

//packet header, with type of body
struct NetworkHeader
{
	PacketType type;
};

inline sf::Packet& operator<<(sf::Packet& packet, const NetworkPlayerEvent& event)
{
	return packet << event.entityID << event.x << event.y << event.angle << event.hitpoints << event.action << event.alternateAction << event.equippedWeapon;
}

inline sf::Packet& operator >> (sf::Packet& packet, NetworkPlayerEvent& event)
{
	return packet >> event.entityID >> event.x >> event.y >> event.angle >> event.hitpoints >> event.action >> event.alternateAction >> event.equippedWeapon;
}

inline sf::Packet& operator << (sf::Packet& packet, const sf::Uint8(&uArray)[NETWORK_BINARYBUFFER_SIZE])
{
	for (int i = 0; i < NETWORK_BINARYBUFFER_SIZE; ++i)
	{
		packet << uArray[i];
	}
	return packet;
}
inline sf::Packet& operator >> (sf::Packet& packet, sf::Uint8(&uArray)[NETWORK_BINARYBUFFER_SIZE])
{
	for (int i = 0; i < NETWORK_BINARYBUFFER_SIZE; ++i)
	{
		packet >> uArray[i];
	}
	return packet;
}

inline sf::Packet& operator<<(sf::Packet& packet, const NetworkBinary& bin)
{
	return packet << bin.buffer << bin.packetNumber << bin.totalPacketNumber << bin.lastValidByte;
}
inline sf::Packet& operator >> (sf::Packet& packet, NetworkBinary& bin)
{
	return packet >> bin.buffer >> bin.packetNumber >> bin.totalPacketNumber >> bin.lastValidByte;
}



inline sf::Packet& operator<<(sf::Packet& packet, const sf::IpAddress address)
{
	std::string addr = address.toString();
	return packet << addr;
}
inline sf::Packet& operator >> (sf::Packet& packet, sf::IpAddress address)
{
	std::string addr;
	packet >> addr;
	address = sf::IpAddress(addr);
	return packet;
}

inline sf::Packet& operator<<(sf::Packet& packet, const NetworkEntityInfo& update)
{
	//return packet << update.entityID << update.ip << update.port;
	packet << update.entityID;
	packet << update.ip.toString();
	packet << update.port;
	return packet;
}
inline sf::Packet& operator >> (sf::Packet& packet, NetworkEntityInfo& update)
{
	//return packet >> update.entityID >> update.ip << update.port;
	packet >> update.entityID;
	std::string addr;
	packet >> addr;
	update.ip = sf::IpAddress(addr);
	packet >> update.port;
	return packet;
}

inline sf::Packet& operator <<(sf::Packet& packet, const NetworkHeader& header)
{
	return packet << static_cast<int32_t>(header.type); // << header.packetNumber << header.totalPacketNumber;
}
inline sf::Packet& operator >> (sf::Packet& packet, NetworkHeader& header)
{
	int32_t t;
	packet >> t;
	header.type = static_cast<PacketType>(t);
	//packet >> header.packetNumber;
	//packet >> header.totalPacketNumber;
	return packet;
}

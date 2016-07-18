#include "Algorithms/guid.h"
#include "Core/GameSettingConstants.h"

enum class NetMsgType{
	GameState,
	ClientInput,
	ClientInfo,
	Binary
};

class NetMsg_Header
{
	//eventually replace len by known length of type, which then will be used in input and output
	NetMsgType type;
	unsigned short len;
};

class NetMessage_GameState
{
	Guid entityID;
	//unsigned short commandType;
	float x;
	float y;
	float angle;
	int hitpoints;
	int action;
	int alternateAction;
	unsigned int equippedWeapon;
};

#define BUTTONS_SIZE

class NetMessage_ClientInput
{
	Guid entityID;
	char buttons[BUTTONS_SIZE];
};

// This entity lives at this address
class NetMessage_ClientInfo
{
	Guid entityID;
	unsigned short port;
	sf::IpAddress ip;
};

class NetMessage_Binary 
{
	unsigned int packetNumber;
	unsigned int totalPacketNumber;
	unsigned int lastValidByte;
	unsigned char buffer[NETWORK_BINARYBUFFER_SIZE];
};

inline sf::Packet& operator << (sf::Packet& packet, const NetMsg& command)
{

}

inline sf::Packet& operator >> (sf::Packet& packet, NetMsg& command)
{
	
}

inline sf::Packet& operator<<(sf::Packet& packet, const NetMessage_Command& command)
{
	packet << command.type << command.size;
	return packet << command.entityID << command.x << command.y << command.angle << command.hitpoints << command.action << command.alternateAction << command.equippedWeapon;
}

inline sf::Packet& operator >> (sf::Packet& packet, NetMessage_Command& command)
{
	packet >> command.type >> command.size;
	return packet >> command.entityID >> command.x >> command.y >> command.angle >> command.hitpoints >> command.action >> command.alternateAction >> command.equippedWeapon;
}

inline sf::Packet& operator<<(sf::Packet& packet, const NetMessage_Binary& bin)
{
	packet << command.type << command.size;
	return packet << bin.buffer << bin.packetNumber << bin.totalPacketNumber << bin.lastValidByte;
}
inline sf::Packet& operator >> (sf::Packet& packet, NetMessage_Binary& bin)
{
	packet >> command.type >> command.size;
	return packet >> bin.buffer >> bin.packetNumber >> bin.totalPacketNumber >> bin.lastValidByte;
}

inline sf::Packet& operator<<(sf::Packet& packet, const NetMessage_ClientInfo& update)
{
	packet << command.type << command.size;
	//return packet << update.entityID << update.ip << update.port;
	packet << update.entityID;
	packet << update.ip.toString();
	packet << update.port;
	return packet;
}
inline sf::Packet& operator >> (sf::Packet& packet, NetMessage_ClientInfo& update)
{
	packet >> command.type >> command.size;
	//return packet >> update.entityID >> update.ip << update.port;
	packet >> update.entityID;
	std::string addr;
	packet >> addr;
	update.ip = sf::IpAddress(addr);
	packet >> update.port;
	return packet;
}

inline sf::Packet& operator <<(sf::Packet& packet, const NetMessageType& type)
{
	return packet << static_cast<int32_t>(type); // << header.packetNumber << header.totalPacketNumber;
}
inline sf::Packet& operator >> (sf::Packet& packet, NetMessageType& type)
{
	int32_t t;
	packet >> t;
	type = static_cast<PacketType>(t);
	//packet >> header.packetNumber;
	//packet >> header.totalPacketNumber;
	return packet;
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
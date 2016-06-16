#pragma once

struct NetworkBinary;

class SendBinary
{
public:
	SendBinary();
	~SendBinary();

	sf::IpAddress receiver;
	unsigned short port;

	bool isSending = false;

	std::vector<char> buffer;
	unsigned int pointer = 0;
	unsigned int counter = 0;
	unsigned int totalPacketNumber = 0;
	unsigned int lastValidByte = 0;

	sf::Clock timeoutClock;

	void addBinary(std::vector<char>& b);
	
	sf::Packet sendChunk();
	void advanceToNextChunk();
};


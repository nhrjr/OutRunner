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
	size_t pointer = 0;
	size_t counter = 0;
	size_t totalPacketNumber = 0;
	size_t lastValidByte = 0;

	sf::Clock timeoutClock;

	void addBinary(std::vector<char>& b);
	
	sf::Packet sendChunk();
	void advanceToNextChunk();
};


#include "stdafx.h"
#include "SendBinary.h"
#include "GameSettingConstants.h"
#include "NetworkPacket.h"

#include <cmath>


SendBinary::SendBinary()
{
}


SendBinary::~SendBinary()
{
}

void SendBinary::addBinary(std::vector<char>& b) {

	buffer = b;

	this->totalPacketNumber = (buffer.size() - 1) / NETWORK_BINARYBUFFER_SIZE + 1;
	this->timeoutClock.restart();
	this->isSending = true;
}

sf::Packet SendBinary::sendChunk() {
	sf::Packet packet;
	NetworkBinary bin;
	int i = 0;

	for (int k = pointer; (k < buffer.size()) && (k < (pointer + NETWORK_BINARYBUFFER_SIZE)); k++)
	{
		bin.buffer[i] = buffer[k];
		i++;
	}
	bin.lastValidByte = i;
	bin.packetNumber = this->counter;
	bin.totalPacketNumber = this->totalPacketNumber;
	this->timeoutClock.restart();

	NetworkHeader header;
	header.type = PacketType::b_NEW;

	packet << header;
	packet << bin;

	return packet;
}

void SendBinary::advanceToNextChunk()
{
	pointer += NETWORK_BINARYBUFFER_SIZE; 
	counter++;
}
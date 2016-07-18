#include "stdafx.h"
#include "NetChannel.h"
#include "NetMessage.h"
#include "NetSocket.h"

NetChannel::NetChannel(sf::UdpSocket& s, Guid peerId, sf::IpAddress peerAddress, unsigned short peerPort)
                      : socket(s), peerId(peerId), peerAddress(peerAadress), peerPort(peerPor)
{

}

void NetChannel::addReliableMsg(NetMsg* msg)
{
    if(msg != nullptr)
        reliableMessageBuffer.emplace(msg);
}

void NetChannel::addUnreliableMsg(NetMsg* msg)
{
    if(msg != nullptr)
        unreliableOutgoingBuffer.emplace(msg);
}

void NetChannel::getMsgs(std::stack<NetMsg>& messages)
{
    messages = incomingBuffer;
    incomingBuffer.clear();
}

void NetChannel::sendPackets()
{
    sf::Packet outgoingPacket;
    NetChannelHeader outgoingHeader;

    if(reliableOutgoingBuffer.size() != 0)
    {
        outgoingHeader.packet_type = NetChannelPacketType::rel_msg;
        outgoingHeader.packet_number = this->sequence;
        this->lastReliablePacketNumber = this->sequence;
        outgoingPacket << outgoingHeader;
        
        for(auto& msg : reliableOutgoingBuffer)
        {
            if(outgoingPacket.getDataSize() + msg.len <= MSG_DATA_SIZE)
                outgoingPacket << msg;
            else
                break;
        }
    }
    while(outgoingPacket.getDataSize() + unreliableOutgoingBuffer.front()->len <= MSG_DATA_SIZE)
    {
        outgoingPacket << *unreliableOutgoingBuffer.front();
        unreliableOutgoingBuffer.pop();

    }
}

void NetChannel::receivePackets()
{
    sf::Packet packet;
	sf::IpAddress sender;
	unsigned short port;

    while (true)
	{
		if (socket.receive(packet, peerId))
		{
			NetChannelHeader header;
			if (packet >> header)
			{
                switch(header)
                {
                    case NetChannelPacketType::unrel_msg
                    {
                        NetMgsHeader msgHeader;
                        while(packet >> msgHeader)
                        {
                            switch(msgHeader)
                            {
                                case NetMsgType::GameState
                                {
                                    NetMessage_GameState* msg = new NetMessage_GameState();
                                    packet >> *msg;
                                    incomingBuffer.emplace(msg);
                                    break;
                                }
                                case NetMsgType::ClientInput
                                {
                                    NetMessage_ClientInput* msg = new NetMessage_ClientInput();
                                    packet >> *msg;
                                    incomingBuffer.emplace(msg);
                                    break;
                                }
                                case NetMsgType::ClientInfo
                                {
                                    NetMessage_ClientInfo* msg = new NetMessage_ClientInfo();
                                    packet >> *msg;
                                    incomingBuffer.emplace(msg);
                                    break;
                                }
                                case NetMsgType::Binary
                                {
                                    NetMessage_Binary* *msg = new NetMessage_Binary();
                                    packet >> msg;
                                    incomingBuffer.emplace(msg);
                                    break;
                                }
                            }
                        }
                        break;
                    }
                    case NetChannelPacketType::unrel_msg_ack
                    {
                        this->ackedSequence = header.packet_number;
                        break;
                    }
                }
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



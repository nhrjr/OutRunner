#include "Algorithms/guid.h"
#include "NetMessage.h"
#include "NetSocket.h"
#include "Core/GameSettingConstants.h"
/*  
    Open a NetChannel per peer;


    netchannel header design:
    - send a a sequence number
    - send the sequencenumber of the last received package
    - reliable indicator bit, showing that the acked package contained reliabe
    - lastly the payload respecting the packet size of 1400bits

    buffers:
    - distinguish between reliable and unreliable commands:
    
    reliable message get stored in a buffer: messageBuf -> reliableBuf, if reliableBuf is empty, showing, that the previous packet has been acked
    from reliableBuf and unreliableBuf fill next packet as:
    header | reliableBuf | unreliableBuf until 1400 is reached, or buffers are empty
    use messageBuf as timeout counter, if messageBuf overflows, drop client
    */

/*
    Networking design:
    class NetChannel - construct NetChannelPackets, Header + NetMessage. Checks ACKs of reliable Messages, keeps track of sequence etc. buffers all messages etc.
            
*/

#define RELIABLE_MSG_BUF_SIZE 64

enum class NetChannelPacketType
{
    control_pkt,
    control_pkt_ack,
    rel_msg,
    rel_msg_ack,
    unrel_msg,
    unrel_msg_ack
}


struct NetChannelHeader {
    NetChannelPacketType packet_type;
    unsigned short packet_number;
};   

class NetController
{
    std::vector<NetChannel> connections;
    NetSocket socket;
}; 

class NetChannel
{
private:
    std::vector<NetMsg*> reliableMessageBuffer;
    std::queue<NetMsg*> unreliableOutgoingBuffer;
    std::vector<NetMsg*> reliableOutgoingBuffer;
    std::stack<NetMsg*> incomingBuffer;

    unsigned short sequence;
    unsigned short ackedSequence;
    unsigned short lastReliablePacketNumber

    NetSocket& socket;
    Guid peerId;
    sf::IpAddress peerAddress;
    unsigned short peerPort;

public:
    NetChannel(NetSocket& s, Guid peerId, sf::IpAddress peerAddress, unsigned short peerPort);
    ~NetChannel() = default;

    // think about templating this adding and gettingfckt, with respect to message classes
    // as these are going to be finite at the end.
    void addReliableMsg(NetMsg& msg);
    void addUnreliableMsg(NetMsg& msg);
    void getMsgs(std::stack<NetMsg*>& messages);

    void sendPackets();
    void receivePackets();
};

inline sf::Packet& operator<<(sf::Packet& packet, const NetChannelHeader& header)
{
    return packet << header.packet_type << header.packet_number;
}

inline sf::Packet& operator >> (sf::Packet& packet, NetChannelHeader& header)
{
    return packet >> header.packet_type >> header.packet_number;
}
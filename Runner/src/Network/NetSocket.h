#include "Algorithms/guid.h"

class NetSocket
{
    NetSocket(){
        socket.setBlocking(false);
    };
    ~NetSocket() = default;
    
    sf::UdpSocket socket;

public:
    void send(sf::Packet& packet, const sf::IpAddress& remoteAddress, unsigned short remotePort);
    bool receive(sf::Packet& packet, Guid id);

}
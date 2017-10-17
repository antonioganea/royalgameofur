/*
PacketID ( UINT32 )          DATA...
0                            bool side;   //which side a player is
1                            UINT32 DIE_ROLL, bool side // die roll for a team
2                            bool side, UINT32 PIECE, UINT32 POSITION // piece movement
3                            bool side // TURN
*/

#include <SFML/Network.hpp>
#include <iostream>
#include <time.h>
#include <stdlib.h>
using namespace std;

int main()
{
    //Open Listeners

    sf::TcpListener listener;
    listener.listen(2340);


    //ACCEPT CONNECTIONS
    sf::TcpSocket redSocket;
    if (listener.accept(redSocket) == sf::Socket::Done)
    {
        // A new client just connected!
        std::cout << "New connection received from " << redSocket.getRemoteAddress() << std::endl;
        //doSomethingWith(redSocket);
    }

    sf::TcpSocket blueSocket;
    if (listener.accept(blueSocket) == sf::Socket::Done)
    {
        // A new client just connected!
        std::cout << "New connection received from " << blueSocket.getRemoteAddress() << std::endl;
        //doSomethingWith(redSocket);
    }

    //SEND TEAMS

    sf::Uint32 packetID = 0;
    sf::Uint32 side = 0;

    sf::Packet packet;

    packet << packetID;
    packet << side;

    redSocket.send( packet );

    packet.clear();
    side = 1;
    packet << packetID;
    packet << side;


    blueSocket.send ( packet );

    //SEND TURN TO START THE GAME
    srand(time(NULL));
    sf::Uint32 turn = rand()%2;

    packetID = 3;
    packet.clear();
    packet << packetID;
    packet << turn;

    redSocket.send( packet );
    blueSocket.send ( packet );

    //PROXY \/

    packet.clear();

    redSocket.setBlocking(false);
    blueSocket.setBlocking(false);

    while( true )
    {
        if ( redSocket.receive(packet) == sf::Socket::Status::Done )
        {
            std::cout << "Receive red packet\n";
            blueSocket.send(packet);
            packet.clear();
        }
        else
        {
            if ( blueSocket.receive(packet) == sf::Socket::Status::Done )
            {
                std::cout << "Receive blue packet\n";
                redSocket.send(packet);
                packet.clear();
            }
        }
    }
}
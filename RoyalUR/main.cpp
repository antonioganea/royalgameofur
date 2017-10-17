#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "board.hpp"
#include "pieces.hpp"
#include "controller.h"
#include "piecemanager.h"
#include "dice.h"
#include "stdlib.h"

#include <iostream>

int main()
{
    srand (time(NULL));
    Board::loadBoard();
    Piece::loadSprites();
    Piece::loadTracks();
    PieceManager::createPieces();
    Controller::initButtons();
    //Controller::startGame();

    char address[100];

    std::cout << "Please enter the ip you want to connect to : ";
    std::cin >> address;

    sf::TcpSocket socket;
    sf::Socket::Status status = socket.connect( address, 2340, sf::seconds(1.0f) );
    socket.setBlocking(true);

    if ( status == sf::Socket::Status::Done )
    {
        std::cout << "Successfully connected to the server. Waiting for the other player\n";
    }
    else
    {
        std::cout << "Error connecting to server\n";
    }

    /*
    PacketID ( UINT32 )          DATA...
    0                            bool side;   //which side a player is
    1                            UINT32 DIE_ROLL, bool side // die roll for a team
    2                            bool side, UINT32 PIECE, UINT32 POSITION // piece movement
    3                            bool side // TURN
    */

    sf::Packet packet;

    status = socket.receive( packet );
    sf::Uint32 PacketID = -1;
    sf::Uint32 side = -1;//Your side
    packet >> PacketID;
    packet >> side;


    if ( PacketID == 0 )
    {
        std::cout << "Other player connected!\n";
        //packet >> side;
        std::cout << "You are on the side " << side << std::endl;
    }
    else
    {
        std::cout << "Received unexpected packet " << PacketID << " .... " << side << "\n";
    }

    std::cout << "Waiting for turn .. ";

    packet.clear();

    status = socket.receive ( packet );
    sf::Uint32 turn = -1;
    packet >> PacketID;
    packet >> turn;

    if ( PacketID == 3 )
    {
        std::cout << "received .. " << turn << "!\n";
        Controller::turn = turn;
    }
    else
    {
        std::cout << "Received unexpected packet " << PacketID << " .... " << turn << "\n";
    }

    socket.setBlocking(false);

    //socket.receive ( packet );

    sf::Vector2u windowSize = Board::texBoard.getSize();

    sf::RenderWindow window( sf::VideoMode( windowSize.x, windowSize.y + 400 ), "Royal Game Of Ur" );

    //sf::RenderWindow window( sf::VideoMode( 800,600 ), "Royal Game Of Ur" );

    window.setFramerateLimit(30);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if ( event.type == sf::Event::MouseButtonPressed )
            {
                //PieceManager::applyClick( event.mouseButton );
                Controller::processClick( event.mouseButton, side, socket );
                std::cout << event.mouseButton.x << ' ' << event.mouseButton.y << std::endl;
            }
        }

        Controller::processPeerInfo ( side, socket );

        window.clear(sf::Color::White);
        Board::draw(window);
        PieceManager::draw(window);
        Controller::draw(window);

        //button.draw(window);

        //window.draw(sprBoard);
        //window.draw( fucker->sprite );

        window.display();
    }


    //int temp;std::cin >> temp;
    return 0;
}
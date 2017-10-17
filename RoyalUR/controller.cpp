#include "controller.h"
#include "piecemanager.h"
#include <stdlib.h>
#include <stdio.h>

#include <iostream>

#include <SFML/Network.hpp>

int Controller::turn = -1;
int Controller::score = -1;
bool Controller::movable = true;

RollButton * Controller::buttons[2];

void Controller::draw( sf::RenderWindow & app ){
    Controller::buttons[0]->draw(app);
    Controller::buttons[1]->draw(app);
}

void Controller::initButtons()
{
    Controller::buttons[0] = new RollButton( 908, 0 );
    Controller::buttons[1] = new RollButton( 908, 762 );
}

void Controller::startGame()
{
    Controller::turn = rand()%2;
    if ( Controller::turn == Team::RED )
        std::puts( "It is red's turn!" );
    else
        std::puts( "It is blue's turn!" );
}

/*
PacketID ( UINT32 )          DATA...
1                            UINT32 DIE_ROLL, sf::Uint32 side // die roll for a team
2                            bool side, UINT32 PIECE, UINT32 POSITION // piece movement
3                            bool side // TURN
*/

void sendDieRoll( sf::TcpSocket & socket, const int team )
{
    sf::Packet packet;
    sf::Uint32 PacketID = 1;
    sf::Uint32 dieRoll = Controller::score;
    sf::Uint32 side = team;
    packet << PacketID << dieRoll << side;
    socket.send(packet);
}

void sendTurn( sf::TcpSocket & socket )
{
    sf::Packet packet;
    sf::Uint32 PacketID = 3;
    sf::Uint32 turn = Controller::turn;
    packet << PacketID << turn;
    socket.send(packet);
}

void sendPieceMovement( sf::TcpSocket & socket, const int team, const int _piece, const int _position )
{
    sf::Packet packet;
    sf::Uint32 PacketID = 2;
    sf::Uint32 side = team;
    sf::Uint32 piece = _piece;
    sf::Uint32 position = _position;
    packet << PacketID << side << piece << position;
    socket.send(packet);
}

void Controller::processClick( const sf::Event::MouseButtonEvent & click, const int team, sf::TcpSocket & socket )
{
    //Die Process
    if ( Controller::buttons[team]->collisionBox.contains( click.x, click.y ) )
    {
        if ( team == Controller::turn )
        {
            if ( Controller::score == -1 )
            {
                Controller::score = Controller::buttons[team]->rollDice();
                Controller::movable = Piece::movable( team, PieceManager::pieces, Controller::score );
                std::printf( "score %d, movable %d\n", Controller::score, Controller::movable );
                sendDieRoll( socket, team );
                return;
            }
            if ( Controller::score == 0 || !movable ) // in case you cannot move with the points you have
            {
                Controller::buttons[team]->resetDice();
                Controller::turn = 1 - Controller::turn; // invert teams
                Controller::score = -1;
                sendTurn( socket );
                return;
            }
        }
    }

    //Pieces Process
    for ( int i = 0; i < 7; i++ ){
        if ( PieceManager::pieces[team][i]->collisionBox.contains( click.x, click.y ) )
        {
            std::printf( "Registered click on piece %d from team %d\n", i, team );
            //second condition can be used to bypass whole for routine, kept for debug reasons:
            if ( Controller::turn == team && Controller::score > 0 )
            {
                if ( PieceManager::pieces[team][i]->canMove(PieceManager::pieces, Controller::score ) ){
                    int target = PieceManager::pieces[team][i]->nextPosition( Controller::score );

                    if ( target >= 11 && target <= 18 ){
                        for ( int i = 0; i < 7; i++ )
                            if ( PieceManager::pieces[1-team][i]->position == target ){
                            PieceManager::pieces[1-team][i]->restart( PieceManager::pieces );
                            sendPieceMovement( socket, 1-team, i, PieceManager::pieces[1-team][i]->position );
                            break;
                        }
                    }

                    PieceManager::pieces[team][i]->move( target );
                    sendPieceMovement( socket, team, i, PieceManager::pieces[team][i]->position );

                    Controller::buttons[team]->resetDice();
                    Controller::score = -1;

                    //if not landing on rozette
                    if ( !PieceManager::isRozette(target) ){
                        Controller::turn = 1 - Controller::turn; // invert teams
                        sendTurn( socket );
                    }

                    //Calculate if all pieces are on 21.
                }
            }
            return;
        }
    }
}

/*
PacketID ( UINT32 )          DATA...
1                            UINT32 DIE_ROLL, sf::Uint32 side // die roll for a team
2                            bool side, UINT32 PIECE, UINT32 POSITION // piece movement
3                            bool side // TURN
*/

void Controller::processPeerInfo ( int side, sf::TcpSocket & socket )
{
    sf::Packet packet;
    sf::Socket::Status status = socket.receive(packet);

    if ( status == sf::Socket::Status::Done )
    {
        sf::Uint32 PacketID;
        packet >> PacketID;

        switch ( PacketID )
        {
        case 1: // DIE ROLL
            sf::Uint32 dieRoll, rollSide;

            packet >> dieRoll >> rollSide;

            std::cout << "DIE ROLL ON OTHER TEAM " << dieRoll << " " << rollSide << std::endl;

            Controller::buttons[rollSide]->setValue(dieRoll);
            break;

        case 2:
            sf::Uint32 team, piece, position;

            packet >> team >> piece >> position;

            std::cout << "Piece movement on other team! " << team << " " << piece << " " << position << std::endl;

            PieceManager::pieces[team][piece]->move(position);

            break;
        case 3:
            sf::Uint32 turn;
            packet >> turn;

            Controller::buttons[0]->resetDice();
            Controller::buttons[1]->resetDice();
            Controller::turn = turn;
            break;
        }
    }
}

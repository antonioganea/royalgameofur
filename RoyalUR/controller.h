#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

#include "dice.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Network/TcpSocket.hpp>

class Controller
{
public:
    static int turn;
    static int score;
    static bool movable;

    static RollButton * buttons[2];

    static void initButtons();
    static void startGame();
    static void draw( sf::RenderWindow & app );
    static void processClick( const sf::Event::MouseButtonEvent & click, const int team, sf::TcpSocket & socket );
    static void processPeerInfo ( int side, sf::TcpSocket & socket );
};

#endif // CONTROLLER_H_INCLUDED

#ifndef BOARD_HPP_INCLUDED
#define BOARD_HPP_INCLUDED

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Board
{
public :
    static sf::Texture texBoard;
    static sf::Sprite sprBoard;
    static void draw( sf::RenderWindow & app );
    static void loadBoard();
};

#endif // BOARD_HPP_INCLUDED

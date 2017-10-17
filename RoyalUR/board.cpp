#include "board.hpp"

#define BOARD_X 0
#define BOARD_Y 200

sf::Texture Board::texBoard;
sf::Sprite Board::sprBoard;

void Board::loadBoard()
{
    Board::texBoard.loadFromFile("board.jpg");
    Board::sprBoard.setTexture( texBoard );
    Board::sprBoard.move( BOARD_X, BOARD_Y );
}

void Board::draw( sf::RenderWindow & app )
{
    app.draw( sprBoard );
}
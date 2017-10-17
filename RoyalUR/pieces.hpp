#ifndef PIECES_HPP_INCLUDED
#define PIECES_HPP_INCLUDED

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>

//#include <iostream>
//#include <fstream>

enum Team{ RED, BLUE };

class Piece
{
public:
    Team team;
    int position;
    sf::Sprite sprite;
    sf::Rect<int> collisionBox;

    static sf::Vector2u track[2][100];//track[team][index]
    static sf::Texture texPiece[2];//texPiece[team]

    Piece( int team, int position );//constructor

    static void loadSprites();
    static void loadTracks();
    static bool movable( int team, Piece * pieces[2][7], int points );

    int nextPosition( int points );
    void move( int position );
    bool canMove( Piece * pieces[2][7], int points );
    void restart( Piece * pieces[2][7] );
};

#endif // PIECES_HPP_INCLUDED

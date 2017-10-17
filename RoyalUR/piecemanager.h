#ifndef PIECEMANAGER_H_INCLUDED
#define PIECEMANAGER_H_INCLUDED

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include "pieces.hpp"

class PieceManager
{
private:
public:
    static Piece * pieces[2][7];
    static void draw( sf::RenderWindow & app);
    static void createPieces();
    static bool isRozette( int target );
    //static void applyClick( const sf::Event::MouseButtonEvent & click );
};

#endif // PIECEMANAGER_H_INCLUDED

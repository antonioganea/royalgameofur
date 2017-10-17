#include "piecemanager.h"
#include <iostream>

Piece * PieceManager::pieces[2][7];

void PieceManager::createPieces()
{
    //for both teams
    for ( int team = 0; team < 2; team++ ){
        //instantiate pieces
        for ( int i = 0; i < 7; i++ ){
            pieces[team][i] = new Piece( team, i );
        }
    }
}

void PieceManager::draw( sf::RenderWindow & app )
{
    for ( int team = 0; team < 2; team++ ){
        //draw pieces
        for ( int i = 0; i < 7; i++ ){
            app.draw( pieces[team][i]->sprite );
        }
    }
}

bool PieceManager::isRozette( int target )
{
    switch ( target )
    {
    case 10:
        return true;
    case 14:
        return true;
    case 20:
        return true;
    default:
        return false;
    }
}

/*
void PieceManager::applyClick( const sf::Event::MouseButtonEvent & click )
{
    for ( int team = 0; team < 2; team++ ){
        for ( int i = 0; i < 7; i++ ){
            if ( pieces[team][i]->collisionBox.contains( click.x, click.y ) )
            {
                std::printf( "Registered click on piece %d from team %d\n", i, team );
                pieces[team][i]->move(9);
                break;
            }
        }
    }
}*/
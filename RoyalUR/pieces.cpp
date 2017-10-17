#include "pieces.hpp"
#include <stdio.h>
#include <cstring>

#define RED_TRACK_PATH "red.txt"
#define BLUE_TRACK_PATH "blue.txt"

#define RED_SPRITE_PATH "red.png"
#define BLUE_SPRITE_PATH "blue.png"

sf::Vector2u Piece::track[2][100];
char trackPaths[2][100];

void Piece::loadTracks()
{
    strcpy( trackPaths[Team::RED], RED_TRACK_PATH );
    strcpy( trackPaths[Team::BLUE], BLUE_TRACK_PATH );

    std::FILE * fin;

    for ( int team = 0; team < 2; team++ )
    {
        fin = std::fopen( trackPaths[team], "r" );

        for ( int i = 0 ; i < 7; i++ )
        {
            Piece::track[ team ][ i ].x = 50+(6-i)*75;
            Piece::track[ team ][ i ].y = 50+team*750;
        }

        for ( int i = 7; i < 22; i++ )
        {
            std::fscanf( fin, "%d", &( Piece::track[ team ][ i ].x ) );
            std::fscanf( fin, "%d", &( Piece::track[ team ][ i ].y ) );
            //std::printf("%d - %d ... %d\n",team,Piece::track[ team ][ i ].x, Piece::track[ team ][ i ].y);
        }
        //std::puts("");
        std::fclose(fin);
    }
}


sf::Texture Piece::texPiece[2];

void Piece::loadSprites()
{
    texPiece[Team::RED].loadFromFile(RED_SPRITE_PATH);
    texPiece[Team::BLUE].loadFromFile(BLUE_SPRITE_PATH);
}

Piece::Piece( int team, int position )
{
    this->team = (Team)team;
    this->position = position;
    this->sprite.setTexture( texPiece[team] );
    this->sprite.setOrigin(38,38);

    sf::Vector2u trackPoint = Piece::track[team][position];

    this->sprite.setPosition(trackPoint.x,trackPoint.y);

    this->collisionBox.left = trackPoint.x - 38;
    this->collisionBox.top = trackPoint.y - 38;
    this->collisionBox.width = 75;
    this->collisionBox.height = 75;

    //std::printf("%d %d\n",trackPoint.x, trackPoint.y);
}

bool Piece::movable( int team, Piece * pieces[2][7], int points )
{
    for ( int i = 0; i < 7; i++ )
        if ( pieces[team][i]->canMove( pieces, points ) )
            return true;

    return false;
}

int Piece::nextPosition( int points )
{
    int target = position + points;
    if ( position < 7 ) // if in deck
        target = points + 6;
    return target;
}

bool Piece::canMove( Piece * pieces[2][7], int points )
{
    int target = nextPosition( points );

    if ( target > 21 )
        return false;
    if ( target == 21 )
        return true;

    //scan same team for pieces in same position;
    for ( int i = 0; i < 7; i++ )
        if ( pieces[team][i]->position == target )
            return false;

    //scan the enemy team for pieces in bridge rozette guarded position
    for ( int i = 0; i < 7; i++ )
        if ( pieces[1-team][i]->position == 14 && target == 14 )
            return false;

    //by default, it can move
    return true;
}

//send back to the deck
void Piece::restart( Piece * pieces[2][7] )
{
    bool deckScan[7];//lookup stuff

    //generate deckScan
    for ( int i = 0; i < 7; i++ )
        deckScan[i] = 0;

    for ( int i = 0; i < 7; i++ )
        if ( pieces[team][i]->position < 7 )
            deckScan [ pieces[team][i]->position ] = true;

    //move in free space
    for ( int i = 0; i < 7; i++ ){
        if ( deckScan[i] == false ){
            move( i );
            break;
        }
    }
}

void Piece::move( int position )
{
    this->position = position;

    sf::Vector2u trackPoint = Piece::track[team][position];

    std::printf("MOVED %d ... %d\n",trackPoint.x, trackPoint.y);
    this->sprite.setPosition(trackPoint.x,trackPoint.y);

    //update collisionBox
    this->collisionBox.left = trackPoint.x - 38;
    this->collisionBox.top = trackPoint.y - 38;
}
#include "dice.h"

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define BUTTON_WIDTH 300
#define BUTTON_HEIGHT 100

sf::Font RollButton::font;
bool RollButton::loadedFont;

RollButton::RollButton(int x, int y)
{
    if ( !RollButton::loadedFont )
    {
        RollButton::font.loadFromFile("OpenSans-Light.ttf");
        RollButton::loadedFont = true;
    }

    collisionBox = *( new sf::IntRect() );
    collisionBox.left = x;
    collisionBox.top = y;
    collisionBox.width = BUTTON_WIDTH;
    collisionBox.height = BUTTON_HEIGHT;

    //sf::RectangleShape button( sf::Vector2f(100,100) );
    button.setSize( sf::Vector2f(BUTTON_WIDTH,BUTTON_HEIGHT) );
    button.setFillColor(sf::Color::Green);

    button.setPosition( x, y );

    //sf::Text text("Roll Dice",RollButton::font);
    text.setString( "Roll Dice" );
    text.setFont( RollButton::font );
    text.setColor(sf::Color::Black);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setPosition( x + BUTTON_WIDTH/2 - bounds.width/2, y + BUTTON_HEIGHT/2 - bounds.height/2 );
}

/**Returns a random value*/
int RollButton::rollDice()
{
    //generate value
    int value = 0;
    for ( int i = 0; i < 4; i++ )
        value += rand()%2;
    std::cout << "Die Roll .. " << value << "\n";

    setValue ( value );

    return value;
}

void RollButton::setValue ( int value )
{
    //generate string
    char buffer[50] = "Dice rolled ";
    char number[10];
    itoa ( value, number, 10 );
    strcat( buffer, number );

    //set string graphically
    text.setString( buffer );
    text.setFont( RollButton::font );
    text.setColor(sf::Color::Black);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setPosition( collisionBox.left + BUTTON_WIDTH/2 - bounds.width/2, collisionBox.top + BUTTON_HEIGHT/2 - bounds.height/2 );

    //return die value
    rolledValue = value;
}

/**Returns a random value*/
void RollButton::resetDice()
{
    rolledValue = 0;

    text.setString( "Roll Dice" );
    text.setFont( RollButton::font );
    text.setColor(sf::Color::Black);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setPosition( collisionBox.left + BUTTON_WIDTH/2 - bounds.width/2, collisionBox.top + BUTTON_HEIGHT/2 - bounds.height/2 );
}

void RollButton::draw( sf::RenderWindow & app )
{
    //std::cout << button.getSize().x << std::endl;
    app.draw( button );
    app.draw( text );
}
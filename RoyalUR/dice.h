#ifndef DICE_H_INCLUDED
#define DICE_H_INCLUDED

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class RollButton
{
public:
    static sf::Font font;
    static bool loadedFont;

    sf::IntRect collisionBox;
    sf::RectangleShape button;
    sf::Text text;

    RollButton(int x, int y);
    int rollDice();
    void setValue ( int value );
    int rolledValue;
    void draw( sf::RenderWindow & app );
    void resetDice();
};

#endif // DICE_H_INCLUDED

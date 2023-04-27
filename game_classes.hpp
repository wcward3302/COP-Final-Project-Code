#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

// ---------------------------------------------------------- Classes ----------------------------------------------------------

// class for ship, holds basic information about ship position and size
class Ship {

public:
    double velocity;
    double x;
    double y;
    double height;
    double width;

    sf::Sprite sprite;
    sf::Sprite dead_sprite;

    Ship()=default;

    void update();
    bool collision_detect (float , float , float , float , float , float , float , float );
};

// game class holds information about game instance, like the score and high score, as well as a frame counter variable
class Game {

public:
    int score_text;
    int high_score_text;
    int frames;
    std::string enter_message_text = "Welcome!\nMay the mass x acceleration be with you...";
    std::string instruct_text = "Use arrow keys to move up and down, esc to restart";
    
    int game_state; // 0 is running, 1 is stopped

    sf::Sprite background;
    sf::Text score;
    sf::Text high_score;
    sf::Text enter_message;
    sf::Text instruct;
    sf::Font font;

    Game()=default;

    void get_high_score();
    void set_high_score();
};


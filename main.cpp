#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include "game_classes.hpp"

int main (){

    // will use time to seed random generator for random wall heights
    srand(time(0));

// ---------------------------------------------------------- Window setup ----------------------------------------------------------

    // create window with size of 1000 by 600, set framerate limit to 90fps, 
    sf::RenderWindow window (sf::VideoMode(2400, 1400), "Project game!");
    window.setFramerateLimit(60);

// ---------------------------------------------------------- Sounds setup ----------------------------------------------------------

    // load sounds into buffer
    sf::SoundBuffer explode_buffer;
    sf::Sound explode;
    explode_buffer.loadFromFile("./Audio/explosion_x.wav");
    explode.setBuffer(explode_buffer);

    sf::SoundBuffer buzz_buffer;
    sf::Sound buzz;
    buzz_buffer.loadFromFile("./Audio/buzz.wav");
    buzz.setBuffer(buzz_buffer);

    sf::SoundBuffer boo_buffer;
    sf::Sound boo;
    boo_buffer.loadFromFile("./Audio/boo.wav");
    boo.setBuffer(boo_buffer);

// --------------------------------------------------------- Texture setup ---------------------------------------------------------

    // struct to hold textures for rendering, can just do sprite.settexture(texture.)
    struct Textures {
        sf::Texture ship; // make ship have 2 textures, one for alive and one for dead
        sf::Texture ship_dead;
        sf::Texture wall;
        sf::Texture background;
    } textures;
    
    // load all images into members of Textures struct
    textures.ship.loadFromFile("./Images/ship.png");
    textures.ship_dead.loadFromFile("./Images/explosion.png");
    textures.wall.loadFromFile("./Images/walls.png");
    textures.background.loadFromFile("./Images/deathstar_surface.jpeg");

// -------------------------------------------------------- Ship Sprite setup --------------------------------------------------------

    // create ship instance, set initial position (1/4 hori, 1/2 vert)
    Ship ship = Ship();
    ship.sprite.setPosition((window.getSize().x / 4) - (textures.ship.getSize().x), (window.getSize().y / 2) - (textures.ship.getSize().y / 2));
    ship.sprite.setTexture(textures.ship);
    ship.dead_sprite.setPosition((window.getSize().x + textures.ship_dead.getSize().x + 1), (window.getSize().y + textures.ship_dead.getSize().y + 1));
    ship.dead_sprite.setTexture(textures.ship_dead);
    ship.dead_sprite.setScale(0.5,0.5);

    // red outline around ship for testing
    sf::RectangleShape hitbox;
    hitbox.setFillColor(sf::Color::Red);

// -------------------------------------------------------- Walls Sprite setup --------------------------------------------------------

    // create a vector for the walls, so we can add and delete easily
    std::vector <sf::Sprite> walls;

// -------------------------------------------------------- Game Content setup --------------------------------------------------------

    // create Game class instance and update
    Game game = Game();

    // update with current highest score from fi
    game.get_high_score();

    game.background.setTexture(textures.background);
    game.background.setScale(1.5, 1.5);
    game.font.loadFromFile("./Fonts/font.ttf");

    game.score.setFont(game.font);
    game.score.setFillColor(sf::Color::White);
    game.score.setCharacterSize(50);
    game.score.setPosition(30, 30);

    game.high_score.setFont(game.font);
    game.high_score.setFillColor(sf::Color::White);
    game.high_score.setCharacterSize(50);
    game.high_score.setPosition(30, 90);

    game.instruct.setFont(game.font);
    game.instruct.setFillColor(sf::Color::White);
    game.instruct.setCharacterSize(50);
    game.instruct.setPosition(30, 1300);

    game.enter_message.setFont(game.font);
    game.enter_message.setFillColor(sf::Color::White);
    game.enter_message.setCharacterSize(65);
    game.enter_message.setPosition(window.getSize().x / 2 - (window.getSize().x / 3), window.getSize().y / 2 - 200);

    int score_keep = 0;

// ---------------------------------------------------------- Window Open ----------------------------------------------------------

    // will loop so long as window is open. Handle rendering and movement here
    while(window.isOpen()) {

// ------------------------------------------------------- Ship Sprite Updates -------------------------------------------------------

        // load ship sprite with initial texture unless game over, then set ship to dead
        if(game.game_state == 1){
            ship.dead_sprite.setPosition(ship.sprite.getPosition().x - 300, ship.sprite.getPosition().y - 300);
        }

        if(game.game_state == 0){
            ship.dead_sprite.setPosition((window.getSize().x + textures.ship_dead.getSize().x + 1), 
                                         (window.getSize().y + textures.ship_dead.getSize().y + 1));
        }        

        // track ship current position
        ship.x = ship.sprite.getPosition().x;
        ship.y = ship.sprite.getPosition().y;

        ship.width = textures.ship.getSize().x;
        ship.height =  textures.ship.getSize().y;

        // uncommenting will show ship hitbox
        //sf::Vector2f hitbox_size (ship.width-150, ship.height);
        //hitbox.setSize(hitbox_size);
        //hitbox.setPosition(ship.x+150, ship.y);
        

        // if player goes out of bounds of screen
        if (game.game_state == 0) {
			if (ship.y < 0) {
				ship.sprite.setPosition((window.getSize().x / 4) - (textures.ship.getSize().x), 0);
			} 
            else if (ship.y + ship.height > 1400) {
				game.game_state = 1;
				explode.play();
			}
		}

// -------------------------------------------------------- Game Scores setup -------------------------------------------------------

        // load texts with current score and high score, but need to to_string them to load
        game.score.setString(std::to_string(game.score_text));
        game.high_score.setString(std::to_string(game.high_score_text));
        game.enter_message.setString(game.enter_message_text);
        game.instruct.setString(game.instruct_text);

        // get score
		for (std::vector<sf::Sprite>::iterator itr = walls.begin(); itr != walls.end(); itr++) {
			
            if (ship.sprite.getPosition().x <= ((*itr).getPosition().x + textures.wall.getSize().x + 10) 
            && ship.sprite.getPosition().x >= ((*itr).getPosition().x + textures.wall.getSize().x)){
                
                game.score_text++; 

				buzz.play();

				if (game.score_text > game.high_score_text) {
					game.high_score_text = game.score_text;
				}
				break;        
            }
		}

// ------------------------------------------------------- Walls Sprite Update -------------------------------------------------------

        // generate a new set of walls every 100 frames
        if (game.frames % 100 == 0){

            // use random generator for heights but restrict them to be 2x ship height apart. 
            int random_height = rand() % 850;
            int gap = textures.ship.getSize().y * 2;

            sf::Sprite wall_lower;
            wall_lower.setTexture(textures.wall);
            wall_lower.setPosition(2500, 0 - random_height);

            sf::Sprite wall_upper;
            wall_upper.setTexture(textures.wall);
            wall_upper.setPosition(2500, (0 - random_height) + textures.wall.getSize().y + gap);

            walls.push_back(wall_lower);
            walls.push_back(wall_upper);
            
        }


        // move walls towards and past player
        if(game.game_state != 1){
            for (std::vector<sf::Sprite>::iterator itr = walls.begin(); itr != walls.end(); itr++) {
            
                (*itr).move(-15, 0);
            }
        }

// ---------------------------------------------------- Ship / Wall Collision Check -----------------------------------------------------

        // collision detection for game end
        if (game.game_state == 0){
            for (std::vector<sf::Sprite>::iterator itr = walls.begin(); itr != walls.end(); itr++) {
				
                // variables for coordinates of walls
                float wall_x, wall_y, wall_width, wall_height;


                // create variables to pass to collision function for each of the walls generated
                if((*itr).getScale().y > 0){
                    wall_x = (*itr).getPosition().x;
                    wall_y = (*itr).getPosition().y;
                    wall_width = textures.wall.getSize().x;
                    wall_height = textures.wall.getSize().y;
                }
                else{
                    wall_width = (*itr).getPosition().x;
                    wall_height = (*itr).getPosition().y;
                    wall_x = (*itr).getScale().x;
                    wall_y = (*itr).getScale().y;
                }

                // if collision is detected, set game to game over, explode
                if(ship.collision_detect(ship.x + 150, ship.y, ship.width-150, ship.height, wall_x, wall_y, wall_width, wall_height)){
                    game.game_state = 1;
                    explode.play();
                }
			}
        }

// ----------------------------------------------------- Keyboard Input -> Movement ------------------------------------------------------

        int direction = 0;
        // check for keypresses to update ship position or restart
        sf::Event event;
        while (window.pollEvent(event)){
            
            
            if (event.type == sf::Event::Closed){
                window.close();
            }

            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
                ship.sprite.setPosition((window.getSize().x / 4) - (textures.ship.getSize().x), 
                                        (window.getSize().y / 2) - (textures.ship.getSize().y / 2));
                
                game.score_text = 0;
                walls.clear();
                game.game_state = 0;
                boo.play();
            }

            if (game.game_state == 0){
                // keyboard input sets ships velocity which is always been applied, no input is 0 velocity
                if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up){
                    ship.velocity = 1; // velocity = up
                }

                else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down){
                    ship.velocity = 2; // velocity = down
                }

                else{
                    ship.velocity = 0; // velocity = 0
                }
            }
        }

// -------------------------------------------------------- Draw Content to Screen -------------------------------------------------------
    
        // clear screen, draw the background and ship sprite
        window.clear();
        window.draw(game.background);
        
        // uncomment this line to show ship hitbox
        //window.draw(hitbox);

        // using new function .update for sprite, which takes the velocity set via keyboard input and moves the position of the sprite. 
        if(game.game_state == 0){
            ship.update();
        }
        
        // draw ship sprite and the dead ship sprite (dead off screen)
        window.draw(ship.sprite);
        window.draw(ship.dead_sprite);
        
        // draw the walls
        for (std::vector<sf::Sprite>::iterator itr = walls.begin(); itr != walls.end(); itr++){
            window.draw(*itr);

            if ((*itr).getPosition().x < -200){     // erase the walls that pass the screen
                walls.erase(walls.begin());
            }
        }

        // display start message
        if(game.frames < 150){
            window.draw(game.enter_message);
        }

        // draw scores
        window.draw(game.score);
        window.draw(game.high_score);
        window.draw(game.instruct);
        
        // display and count frames
        window.display();
        game.frames++;
    }

    // write the current highscore to file
    game.set_high_score();

    return 0;
}
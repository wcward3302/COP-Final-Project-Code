#include "game_classes.hpp"

// ---------------------------------------------------------- Functions ----------------------------------------------------------

// Ship class function - No return or parameters, uses the current velocity variable to move ship.
void Ship::update(){
    if (velocity == 1){
        sprite.move(0,-15);
    }
    else if (velocity == 2){
        sprite.move(0,15);
    }
    else{
        sprite.move(0,0);
    }
}

// Ship class function - Returns true/false, parameters are 8 values representing the x, y, height, and width of 2 items. Compares locations to determine if collision
bool Ship::collision_detect (float x_coord_1, float y_coord_1, float width_1, float height_1, float x_coord_2, float y_coord_2, float width_2, float height_2) {
    if (x_coord_1 + width_1 >= x_coord_2 && x_coord_1 <= x_coord_2 + width_2 && y_coord_1 + height_1 >= y_coord_2 && y_coord_1 <= y_coord_2 + height_2) {
        return true;
    }
    return false;
}


// Game class function - No return or parameters, pulls value from file for the current highest score
void Game::get_high_score(){

// function to set current high score from file

    std::fstream score_file;
    score_file.open("high_scores.txt");

    if (score_file.is_open()){

        std::string line;

        // copy content of text file to lines class member
        while (std::getline(score_file, line)){
            high_score_text = std::stoi(line);
        }

        // if we made it here then the file was opened, so now close
        score_file.close();
    }   
    else{
        std::cout << "did not open\n";
    }
}

// Game class function - No return or parameters, takes the value in high score variable and writes to high score file. 
void Game::set_high_score(){

// Write high score to file. Will not make any real change unless high score changes

    std::fstream score_file;
    score_file.open("./high_scores.txt");

    if (score_file.is_open()){

        score_file << high_score_text;

        score_file.close();
    }
    else{
        std::cout << "did not open\n";
    }
}
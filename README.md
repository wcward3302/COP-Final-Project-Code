# COP-project-game
Small time, not overly complex, basic game using C++ and SFML

![game_sample](https://user-images.githubusercontent.com/122639149/234976748-88022279-8526-4e2a-be4b-b4431323bdfe.gif)

## For: COP-3003 course project

## Reason: 
what initially started as an idea to create a game launcher with multiple different games, I decided early on that trying SFML with C++ would provide a greater challenge. So I came up with a goal for a game that almost all components I made myself. The ship and walls seen were drawn by yours truly. The only component sourced from the internet is the sounds, the fonts, and the background. Everything else was made in house. 

## Functionality: 
- displays a window
- framerate locked at 60 frame/sec
- takes input from the keyboard to adjust velocity of the ship
- generates and displays the walls at random heights
- as player passes a pair of walls, score increases, and high score is adjusted

## Maintainers
Yours truly

## How to run on MacOS

- brew install sfml
- Download code and navigate to COP-Final-Project-Code
- g++ main.cpp game_classes.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -std=c++17
- ./a.out


## C++ Class member UML's

<img width="433" alt="image" src="https://user-images.githubusercontent.com/122639149/234980038-f1604e23-1d01-454c-866e-e23ce8414138.png">

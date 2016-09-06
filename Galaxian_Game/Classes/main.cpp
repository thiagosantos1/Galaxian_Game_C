#include <iostream>
#include <cstdlib>
#include <ctime>  
#include "ShipPlayer.h"
#include "Bullet.h"
#include "ManagerPlayer.h"
#include "Allien.h" 
#include "ManageAlliens.h"
#include <SFML/Graphics.hpp>
#include <stdio.h>      
#include <stdlib.h>   
#include <time.h> 
#include <cstdlib>
#include <SFML/Audio.hpp>
#include "Welcome.h"


using std::cout;
using std::endl;
using std::string;

const float Width = 1407;
const float Height = 1022;

void drawTextWindow(string txt, int xPos, int yPos, sf::RenderWindow & window, int size, sf::Color color);


int main()
{

    std::srand(std::time(0));
                                        // divisable by 32 - each picture is 32x32
    sf::RenderWindow window(sf::VideoMode(Width, Height), "Galaxian",  sf::Style::Close);
    window.setMouseCursorVisible(false);
    
    string  playerName = "";
    
    // load the main sounds
    sf::SoundBuffer bufferBackground;
    sf::Sound soundBackground;

    sf::SoundBuffer bufferWelcome;
    sf::Sound soundWelcome;

    try{
        bufferBackground.loadFromFile("../Audio/background.wav");
        bufferWelcome.loadFromFile("../Audio/welcome.wav");
    }
    catch(...){

        cout<<"Error loading the audio"<<endl;
    }

    try{

        
        soundBackground.setBuffer(bufferBackground);
        soundWelcome.setBuffer(bufferWelcome);

    }
    catch(...){

        cout<<"Error setting and playing the audio"<<endl;
    }
    

    // load background Image(stars)
    sf::Texture image;
    if(! image.loadFromFile("../Images/Background/background.png"))
        return -1;  // put a try catch in the future
    sf::Sprite background;
    background.setTexture(image);

    // create an Welcome object
    Welcome * welcomePage;
    welcomePage = new Welcome(window.getSize());
    bool welcomePageModeOn = true;
    soundWelcome.play();
    soundWelcome.setVolume(60);
    soundWelcome.setLoop(true);

    // get an divisible number by 10
    int middleWidth = Width/2;
    middleWidth /=10;
    middleWidth *=10;

    ManagerPlayer * player;

    ManageAlliens * alliens;

    float gameOverWait = 0;

    sf::Clock clock;

    while (window.isOpen())
    {
        
        sf::Time elapsedTime = clock.restart();
        
        // first of all, draw the background
        window.draw(background);
        // do everything welcome page has to do
        if(welcomePageModeOn){

            sf::Event event;
            while (window.pollEvent(event))
            {
               

                if (event.type == sf::Event::Closed)
                    window.close();
                
                else if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Space){

                        welcomePageModeOn = false;
                        // sound game
                        soundWelcome.stop();
                        soundBackground.play();
                        soundBackground.setVolume(60);
                        soundBackground.setLoop(true);
                        // create player and alliens
                        delete player;
                        delete alliens;
                        playerName = "";
                        player = new ManagerPlayer(middleWidth, Height-120);
                        alliens = new ManageAlliens(window.getSize()); 
                    }
                     
                
                }
            }

            welcomePage->drawAllAliens(window);

            try{
                welcomePage->drawBestScores(window);
            }catch(...){

                cout<< "Error opening File"<<endl;
            }

            welcomePage->moveAllAliens(elapsedTime.asSeconds(), window);


        }


        // user press start to start the game - gameMode = true
        else{

            
            sf::Event event;
            while (window.pollEvent(event))
            {
               

                if (event.type == sf::Event::Closed)
                    window.close();
                
                else if (event.type == sf::Event::KeyPressed)
                {   
                     if (event.key.code == sf::Keyboard::Left){
                        player->getPlayer()->setXVelocity(-7);
                    }
                    
                    if (event.key.code == sf::Keyboard::Right){
                        player->getPlayer()->setXVelocity(+7);
                       
                    }

                   // player shoot
                    if (event.key.code == sf::Keyboard::Space){

                        if(player->getPlayer()->getLives() >0 )
                            player->newBullet(elapsedTime.asSeconds()); // create a new bullete depending of how long time has passed 
                                                                   // since last shoot. Also, play player shoot sound
                       
                    }

                    if ( (event.key.code == sf::Keyboard::BackSpace) && playerName.size()!=0 ){
                        playerName.pop_back();
                        std::cout << playerName<< std::endl;
                       
                    }

                
                }
                else if (event.type == sf::Event::TextEntered){
                
                    if (event.text.unicode < 128) {
                       playerName.push_back((char)event.text.unicode);
                        std::cout << playerName<< std::endl;
                    }
                }
            }

            /*
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){

                cout<<"sdsd"<<endl;
                player->getPlayer()->setXVelocity(-1000);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                 player->getPlayer()->setXVelocity(1000);
            */
            
            //elapsedTime = clock.restart();
            if(player->getPlayer()->getLives() >0 )
                player->moveAll(elapsedTime.asMilliseconds(), window);
            
            alliens->moveAll(elapsedTime.asMilliseconds(), window, player->getPlayerBullets(), player->getPlayer());
           
            // if all alliens is gone, create a new fleet of aliens
            if (alliens->shouldCreateNewFleetOfAliens()){
                // have to fix - have to create a copy constructor to work
               // ManageAlliens newAlliens(window.getSize()); 
                alliens->createNewFleetOfAliens(window.getSize()); 
                player->getPlayer()->setFleetKilled(player->getPlayer()->getFleetKilled() +1);

            }

            try{
                alliens->attack(elapsedTime.asSeconds(), window, player->getPlayer());
            }
            catch(...){

                cout<<"Error in Allien Attack mode" <<endl;
            }   

            player->checkLives(elapsedTime.asSeconds(), alliens->getBullets(), alliens->getAlliens());

            // game over
            if(player->getPlayer()->getLives() <=0){

                drawTextWindow("Game Over!!!", Width/2 -100, Height/2, window, 30,sf::Color::White);
                // have wait for a while, to player see he is dead
                if(gameOverWait >=4){

                    welcomePageModeOn = true;
                    soundBackground.stop();
                    delete welcomePage;
                    welcomePage = new Welcome(window.getSize());
                    soundWelcome.play();
                    soundWelcome.setVolume(60);
                    soundWelcome.setLoop(true);
                    gameOverWait = 0;
                }
                else{
                    gameOverWait += elapsedTime.asSeconds();
                }
            }

            window.clear();
            
            window.draw(background);
            player->drawAll(window);
            alliens->drawAll(window);
            
            if(gameOverWait >0)
                drawTextWindow("Game Over!!!", Width/2 -100, Height/2, window, 30,sf::Color::White);
    
            drawTextWindow("Lives: ", Width - 160, Height - 30, window, 30,sf::Color::Red);
            drawTextWindow(std::to_string(player->getPlayer()->getLives()), Width - 60, Height - 30, window, 28,sf::Color::Red);

            drawTextWindow("Score: ", 0, Height - Height*0.999 , window, 30,sf::Color::Red);
            drawTextWindow(std::to_string(player->getPlayer()->getScore()), 120, 4, window, 28,sf::Color::Red);

            drawTextWindow("Fleets Killed: ", 0, Height - 30 , window, 28,sf::Color::Red);
            drawTextWindow(std::to_string(player->getPlayer()->getFleetKilled()), 195, Height - 30 , window, 28,sf::Color::Red);
            
        }


        window.display();
    }

    delete player;
    delete alliens;
    delete welcomePage;

    return 0;
}

void drawTextWindow(string txt, int xPos, int yPos, sf::RenderWindow & window, int size, sf::Color color){

    try{
        // Declare and load a font
        sf::Font font;
        if (!font.loadFromFile("../Fonts/Arial.ttf"))
        {
            cout<<"Erro"<<endl;
        }
        // Create a text
        sf::Text text(txt, font);
        text.setCharacterSize(size);
        text.setStyle(sf::Text::Bold);
        text.setColor(color);
        text.setPosition(xPos,yPos);
        // Draw it
        window.draw(text);
    }
    catch(std::exception e){

        cout<<"Error loading Font"<<endl;

    }
}





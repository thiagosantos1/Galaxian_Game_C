#include "ManageAlliens.h"
#include <iostream>
#include "Allien.h"
#include "Bullet.h"
#include "ShipPlayer.h"
#include <SFML/Graphics.hpp>
#include <cmath>  
#include <string>
#include <SFML/Audio.hpp>
#include <vector>

using std::cout;
using std::endl;
using std::string;



ManageAlliens::ManageAlliens(const sf::Vector2u & window_size)
    : numAlliens(24), // game has 24 alliens/ divide in 3 lines
      elapsedTimeAttackShot(0),
      elapsedTimeAttackDown(0),
      elapsedTimeExplosionDuration(0),
      primaryRectangleModeOn(false),
      secondaryRectangleModeOn(false),
      xPosExplosion(0),
      explosionModeOn(false),
      yPosExplosion(0),
      allienInAttack(false),
      allienAttackDirection("center"),
      allienInAttackPosition(0) // no allien choose
{
	
	float xStart = 256; // set the first allien
	float yStart = 60;  

	float xPos = xStart;
    float yPos = yStart;

    float xSpaceAlliens = 50; // save how much space bettew each allien
    float ySpaceAlliens = 20; // save how much space bettew each allien

    float limetedX = window_size.x - 256; 

    for (int i = 0; i < numAlliens; i++)
    {
    	// first allien it's already set
    	if(i>0){
    		// get new position for next allien
    		xPos = alliens[i-1]->getPosition().x + alliens[i-1]->allien.getGlobalBounds().width + xSpaceAlliens;
    		if(xPos > limetedX){
    			xPos = xStart; // start next line of alliens
    			yPos = alliens[i-1]->getPosition().y + alliens[i-1]->allien.getGlobalBounds().height + ySpaceAlliens;
    		}
    	}
    	
        Allien * temp = new Allien(xPos,yPos);
        alliens.push_back(temp);
    }
    // load allien shoot sound
    try{

        bufferShoot.loadFromFile("../Audio/alienShoot.wav"); 
        bufferExplosion.loadFromFile("../Audio/alienExplosion.wav"); 
    }
    catch(...){

        cout<<"Error loading Alien Shoot  audio"<<endl;
    }

    try{

        
        soundShoot.setBuffer(bufferShoot);
        soundAlienExplosion.setBuffer(bufferExplosion);
    }
    catch(...){

        cout<<"Error setting Alien Shoot audio"<<endl;
    }

    try{

        string generalPathExplosion = "../Images/Explosion/explosion";
        string path = "";
        for(int i=0;i<9;i++){
            string index = std::to_string(i+1);
            path = generalPathExplosion + index + ".png";
            sf::Texture  tempTexture;
            tempTexture.loadFromFile(path);
            explosionTextures.push_back(tempTexture);
        
        }

        explosion.setTexture(explosionTextures[0]);

    }
    catch(...){

        cout<<"Exception setting explosion images" <<endl;
    }

    rectanglePrimary.setSize(sf::Vector2f(10, 10));
    rectanglePrimary.setPosition(0, 0);
    rectanglePrimary.setFillColor(sf::Color::White);

    rectangleSecondary.setSize(sf::Vector2f(10, 10));
    rectangleSecondary.setPosition(0, 200);
    rectangleSecondary.setFillColor(sf::Color::Red);


    
}

void ManageAlliens::moveAll(float time, sf::RenderWindow & window, std::vector<Bullet *>  & bullets, ShipPlayer* player)
{    
    // before move all, inclusive the rects, we get the x speed from the fleet, from some allien that is not in attack mode
    // or from the only allien in the fleet, if this is the case
    float newXVel = alliens[0]->getXVelocity();
    for (int i = 0; i < alliens.size(); i++){

        if( (alliens[i]->getInAttack() == false) and (alliens[i]->getAttackDone() == false) ){
            
            newXVel = alliens[i]->getXVelocity();
            
            break;
            
        }
        
    }


	// move all alliens, and also the rectangules that save the allies in attack old position
    for (int i = 0; i < alliens.size(); i++){
        alliens[i]->move(time, window.getSize(),rectanglePrimary, rectangleSecondary);
    

        // if allien is in attack, or he was in attack, we still moving his old posiiton
        if(alliens[i]->getInAttack() or alliens[i]->getAttackDone() ){
            // move both rectangles. They save the old position of the allien in attack mode
            if(alliens[i]->getRectTrackerOldPosition() == 1)
                rectanglePrimary.move(newXVel * time, 0);
            else
                rectangleSecondary.move(newXVel * time, 0);
        }
    }

    // if alliens is close to the edge, change ther direction
    for (int i = 0; i < alliens.size(); i++){
        // if allien in that position is not in attack, then we check if the fleet has to change direction
        if(alliens[i]->getInAttack() == false){
    	    if ( alliens[i]->isCloseToWall( alliens[i]->getPosition().x , window.getSize()) == 1 ) // 1 means close to the left edge
    	        {
    	        	for (int i = 0; i < alliens.size(); i++){
                        // change speed just if allien is not in attack
                        if(alliens[i]->getInAttack() == false)
    	        		     alliens[i]->setXVelocity( std::abs(alliens[i]->getXVelocity()) );
                    }
    	        	break;
    	            
    	        }  

    	    if ( alliens[i]->isCloseToWall( alliens[i]->getPosition().x , window.getSize()) == 2 ) // 2 means close to the right edge
    	        {

    	        	for (int i = 0; i < alliens.size(); i++){
                        // change speed just if allien is not in attack
                        if(alliens[i]->getInAttack() == false)
    	        		     alliens[i]->setXVelocity( -std::abs(alliens[i]->getXVelocity()) );
                    }
    	        	break;
    	            
    	        } 
        }
    }

    // always after move, each alien have to be able to ask him self if he should be delete or not
    for (int i = 0; i < alliens.size(); i++)
    {
        if (alliens[i]->shouldBeDeleted(bullets)) // if one of the player bullets hits the allien
        {
        	// evertime an player bullet his an alien, player gets the points of the alien
        	player->setScore(alliens[i]->getPointsForPlayer());
        	
            // if allien killed by player, is the same if the currenly in attack mode
            // we reset the allien in attack status, because he is no longer in attack
            // time to pass to next allien 
            if(allienInAttackPosition == i){
                allienInAttack = false;
                allienInAttackPosition = -1;
            }

            try{
                soundAlienExplosion.play();
                soundAlienExplosion.setVolume(15);
            }
            catch(...){

                cout<<"Error playing alien explosion sound"<<endl;
            }

            elapsedTimeExplosionDuration = 0.001; // set explosion on 
            xPosExplosion = alliens[i]->getPosition().x;
            yPosExplosion = alliens[i]->getPosition().y;
            explosionModeOn = true;


            // reset rectangule
            if(alliens[i]->getRectTrackerOldPosition() == 1){

                primaryRectangleModeOn = false;
            }
            else{

                secondaryRectangleModeOn = false;
            }

            // then "delete" the alien
            delete alliens[i];
            alliens.erase(alliens.begin() + i);  // order the array again
            i--;

            // after delete any allien, we have to update the allienInAttackPosition, because allien could change
            // inside of the vector

           

        }        
    }

    // explosion mode is on
    if(elapsedTimeExplosionDuration > 0 ){
        //cout<<elapsedTimeExplosionDuration <<"ga"<<endl;
        elapsedTimeExplosionDuration += time/1000; // to seconds
        //cout << elapsedTimeExplosionDuration <<"ff"<<endl;
        drawExplosion();

    }

    // after move and check if allien should be delete, we have to update the allienInAttackPosition, becuase coulde be change
    for (int i = 0; i < alliens.size(); i++)
    {
             // if allienInAttackPosition still valid(there's an alien in attack mode)
            if(allienInAttackPosition != -1){

                // if allien in the i position of the vector is in attack mode, means this's the currently allien in attack
                if(alliens[i]->getInAttack()){
                    allienInAttackPosition = i;
                }

            }
    }

}

// check if there's no more alliens. If so, create new "fleet" of aliens
bool ManageAlliens::shouldCreateNewFleetOfAliens(){

	return alliens.size() <=0;

}

void ManageAlliens::createNewFleetOfAliens(const sf::Vector2u & window_size){

	int numAlliens = 24; // game has 24 alliens/ divide in 3 lines
	float xStart = 256; // set the first allien
	float yStart = 60;  

	float xPos = xStart;
    float yPos = yStart;

    float xSpaceAlliens = 50; // save how much space bettew each allien
    float ySpaceAlliens = 20; // save how much space bettew each allien

    float limetedX = window_size.x - 256; 

    for (int i = 0; i < numAlliens; i++)
    {
    	// first allien it's already set
    	if(i>0){
    		// get new position for next allien
    		xPos = alliens[i-1]->getPosition().x + alliens[i-1]->allien.getGlobalBounds().width + xSpaceAlliens;
    		if(xPos > limetedX){
    			xPos = xStart; // start next line of alliens
    			yPos = alliens[i-1]->getPosition().y + alliens[i-1]->allien.getGlobalBounds().height + ySpaceAlliens;
    		}
    	}
    	
        Allien * temp = new Allien(xPos,yPos);
        alliens.push_back(temp);
    }


}

void ManageAlliens::drawAll(sf::RenderWindow & window)
{
	// draw all alliens in their currently position
    for (int i = 0; i < alliens.size(); i++)
        alliens[i]->draw(window,explosion, explosionModeOn);
    
    window.draw(rectanglePrimary);
    window.draw(rectangleSecondary);

    // draw all bullets alliens have shoot
    for (int i = 0; i < bullets.size(); i++)
        bullets[i]->draw(window);
}

void ManageAlliens::newBullet(Allien * allien){  // create a new bullete for allien shoot

                                                                                    // vy speed = 100px/s
    Bullet * sp = new Bullet(allien->getPosition().x +25 , allien->getPosition().y + allien->allien.getGlobalBounds().height -10, 
                            0 , + 600, "alien");
    // for allien, bullet moves just on Y direction
    bullets.push_back(sp);
    //bullets->setElapsedTime(0); // reset the time to 0, for the next bullet check
    
}

void ManageAlliens::attack(float elapsedTimeSeconds, const sf::RenderWindow & window, ShipPlayer* player){

    elapsedTimeAttackShot +=elapsedTimeSeconds;
    elapsedTimeAttackDown += elapsedTimeSeconds;

    // move all bullets that alliens have shoot against the player
    for (int i = 0; i < bullets.size(); i++)
        bullets[i]->move(elapsedTimeSeconds, window.getSize());

    // check to see if any bullet has to be deleted because it touch one of the edges
    // managePlayer is able to ask for his player to see if any allien bullet hits him, this's not a job for manageAlliens
    for (int i = 0; i < bullets.size(); i++)
    {
        // should the bullet be deleted from the window because touch a edge?
        if (bullets[i]->shouldBeDeleted(window))
        {
            delete bullets[i]; 
            bullets.erase(bullets.begin() + i);
            i--;
        } 
 
    }
    if (alliens.size() >0 and player->getLives() > 0 ){
        try{
            // if the time since last attack shoot, by any alien, is greather than 3 seconds, alliens are alowed to shoot again
            if(elapsedTimeAttackShot >=3.0){

                int ramdomAllienToShoot = rand() % alliens.size(); // selecte a ramdom allien from the vector of alliens
                newBullet(alliens[ramdomAllienToShoot]);

                // allien shoot sound
                soundShoot.play();
                soundShoot.setVolume(70);

                elapsedTimeAttackShot =0; // reset
            }


            // before enter in attack, check to see if any of the alliens that was already in attack, has get done with
            // the attack, to then reset the rectangles that holds old alliens position
            for (int i = 0; i < alliens.size(); i++){

                // if allien was in attack, and he is done and had back to his old position
                // reset everything we need to keep tracking
                if(alliens[i]->getResetRectOldPosition()==true) {

                    if(alliens[i]->getRectTrackerOldPosition() == 1){

                        primaryRectangleModeOn = false;
                    }
                    else{

                        secondaryRectangleModeOn = false;
                    }

                    alliens[i]->setResetRectOldPosition(false); 
                    
                }
            }

            
            // attack down mode - if time since the last attack down mode is greather than 7 seconds, 
            // and last allien in attack mode is done with his attack
            if(elapsedTimeAttackDown >=7 and allienInAttack == false){

                // select a ramdom alien to attack player
                int ramdomAllienAttackDown = rand() % alliens.size();
                while( (alliens[ramdomAllienAttackDown]->getInAttack()) or (alliens[ramdomAllienAttackDown]->getAttackDone()) ) {

                    ramdomAllienAttackDown = rand() % alliens.size();

                }
                allienInAttackPosition = ramdomAllienAttackDown;
                allienInAttack = true;
                elapsedTimeAttackDown =0;

                alliens[allienInAttackPosition]->setInAttack(true); // allien status set to in Attack
                alliens[allienInAttackPosition]->setAttackDone(false);
                alliens[allienInAttackPosition]->setResetRectOldPosition(false); 
                // set vy speed, that doesn't change until allien attack gets done
                alliens[allienInAttackPosition]->setYVelocity(std::abs(alliens[allienInAttackPosition]->getVelocity_yAttack()));

                // if primaryRectangle is not beeing use, we selected it
                if( primaryRectangleModeOn == false){

                    rectanglePrimary.setPosition(alliens[allienInAttackPosition]->getPosition().x,alliens[allienInAttackPosition]->getPosition().y);
                    primaryRectangleModeOn = true;
                    alliens[allienInAttackPosition]->setRectTrackerOldPosition(1); // knows that his old posiion is the primary Rectangle
                }
                else if(secondaryRectangleModeOn == false){

                    rectangleSecondary.setPosition(alliens[allienInAttackPosition]->getPosition().x,alliens[allienInAttackPosition]->getPosition().y);
                    secondaryRectangleModeOn = true;
                    alliens[allienInAttackPosition]->setRectTrackerOldPosition(2);
                }
            }


            // if ramdom position for next allien in attack is a valid alliens position, and allienInAttack mode is true
            // then we execute the attack, by moving down the allien chosen
            if (allienInAttackPosition >=0 and allienInAttack == true){

                // if allien x position is less than player x position, we set the x speed positive
                if(alliens[allienInAttackPosition]->getPosition().x < player->getPosition().x - 10){

                    /*
                    // allien was in the same y direction, now it's less
                    if(allienAttackDirection == "center" or allienAttackDirection == "left"){
                        // reset img
                        alliens[allienInAttackPosition]->setOriginalImage();
                        // then rotate
                        alliens[allienInAttackPosition]->setRotation(-45);
                        allienAttackDirection = "rigth";
                    }*/

                    alliens[allienInAttackPosition]->setXVelocity(std::abs(alliens[allienInAttackPosition]->getVelocity_xAttack()));
                }
                
                // if allien x position is greather than player x position, we set the x speed negative
                else if(alliens[allienInAttackPosition]->getPosition().x > player->getPosition().x + 10){
                    /*
                    if(allienAttackDirection == "center" or allienAttackDirection == "rigth"){
                        // reset img
                        alliens[allienInAttackPosition]->setOriginalImage();
                        // then rotate
                        alliens[allienInAttackPosition]->setRotation(45);
                        allienAttackDirection = "left";
                    }*/

                    alliens[allienInAttackPosition]->setXVelocity(-std::abs(alliens[allienInAttackPosition]->getVelocity_xAttack()));
                }
                // save position
                else{

                    allienAttackDirection = "center"; // allien is pointer center, no rotation
                   // alliens[allienInAttackPosition]->setOriginalImage();
                    alliens[allienInAttackPosition]->setXVelocity(0);

                }

                // check if allien should be delete(by hiting player)
                if(alliens[allienInAttackPosition]->shouldBeDeleted(player)){

                    if(player->getLives() > 0){

                        // reset rectangule
                        if(alliens[allienInAttackPosition]->getRectTrackerOldPosition() == 1){

                            primaryRectangleModeOn = false;
                        }
                        else{

                            secondaryRectangleModeOn = false;
                        }

                        delete alliens[allienInAttackPosition]; 
                        alliens.erase(alliens.begin() + allienInAttackPosition);
                        allienInAttack =false;
                   
                   
                        // player lose 1 life because allien hits him
                        player->setLives(player->getLives() -1);

                        allienAttackDirection = "center"; // reset allienAttackDirection
                        //allienInAttackPosition = -1;
                        elapsedTimeExplosionDuration = 0.001; // set explosion on 
                        xPosExplosion = player->getPosition().x;
                        yPosExplosion = player->getPosition().y;
                        player->getSoundLoseLife().play();
                        explosionModeOn = true;
                    }
                } 

                // check if allien should return for formation because cross the edges
                else if(alliens[allienInAttackPosition]->shouldReturnToFleet(window.getSize())) {
                    

                    // allien is no longer in attack mode
                    alliens[allienInAttackPosition]->setInAttack(false); 

                    // allien attack is done, time to go back to formation
                    alliens[allienInAttackPosition]->setAttackDone(true);
                    allienInAttack =false;
                    allienAttackDirection = "center"; // reset allienAttackDirection
                    //alliens[allienInAttackPosition]->setOriginalImage();
                    
                    // set the allien y position to 0, to restart in the top
                    int recTrack = alliens[allienInAttackPosition]->getRectTrackerOldPosition();
                    float newXPosition =0;
                    float newXVel = alliens[0]->getXVelocity();
                    if(recTrack ==1)
                        newXPosition = rectanglePrimary.getPosition().x;
                    else
                        newXPosition = rectangleSecondary.getPosition().x;
                    
                    alliens[allienInAttackPosition]->setPosition(newXPosition,0);
                    for (int i = 0; i < alliens.size(); i++){

                        if(alliens[i]->getInAttack() == false)
                            newXVel = alliens[i]->getXVelocity();

                        break;
                    }
                    alliens[allienInAttackPosition]->setXVelocity(newXVel);
                    // just to see if it's working - have to drop this
                    //delete alliens[allienInAttackPosition]; 
                    //alliens.erase(alliens.begin() + allienInAttackPosition);

                    //allienInAttackPosition = -1;

                    
                    
                }
               
            }


        }
        catch(...){

            cout<<"Error in Alliens shoot Atack"<<endl;
        }
    }


}

std::vector<Bullet *> & ManageAlliens::getBullets(){

    return bullets;
}

std::vector<Allien *> & ManageAlliens::getAlliens(){

    return alliens;
}

void ManageAlliens::drawExplosion()
{   
    
    // if time of explosion is set, the we draw
    if(elapsedTimeExplosionDuration > 0){

        // divede the explosion in 9(there 9 imgs)
        if(elapsedTimeExplosionDuration < 0.10){
            explosion.setTexture(explosionTextures[0]);
        }
        else if(elapsedTimeExplosionDuration < 0.15){
            explosion.setTexture(explosionTextures[1]);
        }
        else if(elapsedTimeExplosionDuration < 0.20){
            explosion.setTexture(explosionTextures[2]);
        }
        else if(elapsedTimeExplosionDuration < 0.25){
            explosion.setTexture(explosionTextures[3]);
        }
        else if(elapsedTimeExplosionDuration < 0.30){
            explosion.setTexture(explosionTextures[4]);
        }
        else if(elapsedTimeExplosionDuration < 0.35){
            explosion.setTexture(explosionTextures[5]);
        }
        else if(elapsedTimeExplosionDuration < 0.40){
            explosion.setTexture(explosionTextures[6]);
        }
        else if(elapsedTimeExplosionDuration < 0.45){
            explosion.setTexture(explosionTextures[7]);
        }
        else{
            explosion.setTexture(explosionTextures[8]);
        }
        
        explosion.setPosition(xPosExplosion, yPosExplosion);
        
        // reset the elapse to 0 if duration is done(2 seconds)
        if(elapsedTimeExplosionDuration > 0.6){
            elapsedTimeExplosionDuration = 0;
            explosionModeOn = false;
        }

    }

}

ManageAlliens::~ManageAlliens(){

	for (int i = 0; i < alliens.size(); i++)
        delete alliens[i];
}





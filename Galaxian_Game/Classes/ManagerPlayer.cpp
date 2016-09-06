#include "ManagerPlayer.h"
#include <iostream>
#include "ShipPlayer.h"
#include "Allien.h"
#include <string>

#include <vector>

using std::cout;
using std::endl;
using std::string;

ManagerPlayer::ManagerPlayer(float playerXposition, float playerYposition)
    :explosionModeOn(false),
    elapsedTimeExplosionDuration(0)
{
    player = new ShipPlayer(playerXposition,playerYposition);

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
}

void ManagerPlayer::drawAll(sf::RenderWindow & window)
{
	// draw all bullets
    for (int i = 0; i < bullets.size(); i++)
        bullets[i]->draw(window);

    // draw the player
    player->draw(window,explosion,explosionModeOn);
}

void ManagerPlayer::moveAll(float time, const sf::RenderWindow & window)
{    
	// move player
	player->move(time,window.getSize());

	// move all bullets
    for (int i = 0; i < bullets.size(); i++)
        bullets[i]->move(time, window.getSize());

    // check to see if any bullet has to be deleted
    for (int i = 0; i < bullets.size(); i++)
    {
    					// should the bullet be deleted from the window?
        if (bullets[i]->shouldBeDeleted(window))
        {
            delete bullets[i]; 
            bullets.erase(bullets.begin() + i);
            i--;
        }        
    }
}

void ManagerPlayer::newBullet(float timeElapsedTime){ 

	player->setElapsedTime(player->getElapsedTime() + timeElapsedTime);
	float timePassed = player->getElapsedTime();

	if(timePassed > 0.012){  // if time passed is greater than 0.2 seconds, player can shoot
		Bullet * sp = new Bullet(player->getPosition().x +30 , player->getPosition().y, 0 , -1, "player");
		// for player, bullet moves just on Y direction
		bullets.push_back(sp);
		player->setElapsedTime(0); // reset the time to 0, for the next bullet check
        getPlayer()->getSoundShoot().play(); // play the shoot sound when create a new bullet
        getPlayer()->getSoundShoot().setVolume(10);
	}
}

ShipPlayer* ManagerPlayer::getPlayer(){

	return player;
}

ManagerPlayer::~ManagerPlayer()
{
    for (int i = 0; i < bullets.size(); i++)
        delete bullets[i];

    delete player;
}

std::vector<Bullet *>  & ManagerPlayer::getPlayerBullets(){

    return bullets;
}

void ManagerPlayer::checkLives(float elapsedTime,std::vector<Bullet *>  & bullets, std::vector<Allien *>  & allien){

    for (int i = 0; i < bullets.size(); i++)
        {
         
            // should the bullet be deleted from the window because hit player?
            
            if(player->getLives() > 0){
                if (player->allienShootHit(bullets[i]) )  // if one of the allien bullets hits the player
                {   
                    // evertime an allien shoot and hit the player, he lose 1 live
                    player->setLives(player->getLives() - 1);

                    elapsedTimeExplosionDuration = 0.001; // set explosion on 
                    explosionModeOn = true;

                    player->getSoundLoseLife().play();
                    // then "delete" the bullet
                    delete bullets[i];
                    bullets.erase(bullets.begin() + i);  // order the array again
                    i--;
                }
            }        
             

        }

    // explosion mode is on
    if(elapsedTimeExplosionDuration > 0 ){
        //cout<<elapsedTimeExplosionDuration <<"ga"<<endl;
        elapsedTimeExplosionDuration += elapsedTime; // to seconds
        //cout << elapsedTimeExplosionDuration <<"ff"<<endl;
        drawExplosion();

    }
}


void ManagerPlayer::drawExplosion()
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
        
        explosion.setPosition(getPlayer()->getPosition().x,getPlayer()->getPosition().y );
        
        // reset the elapse to 0 if duration is done(2 seconds)
        if(elapsedTimeExplosionDuration > 0.6){
            elapsedTimeExplosionDuration = 0;
            explosionModeOn = false;
        }

    }

}











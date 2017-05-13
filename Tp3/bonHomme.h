/* En-tête du programme
====================
Programme:        bonHomme.h
Auteur:           Chloé et Sey
Date création:    28/04/2017
Description:	  Cette librairie regroupe le code nécessaire à l'objet bonhomme, 
					qui utilise les propriétés privés de position, d'indice de position,
					de vitesse, de counterwalking, de direction, de sprite, de posSprite,
					de touche, de texture, de bombe, un booléen bombedeposee.*/

/* Directives au pré-processeur
============================*/
#pragma once
#include <iostream>
#include  <SFML/Graphics.hpp>	//librairie graphique
#include <cassert>				//pour vérifier et afficher des messages d'erreur
#include "grille.h"				//accès à la grille (collisions)
#include "bomb.h"				//chaque bonhomme a  1 bombe
using namespace sf;				//pour SFML
using namespace std;

/*Définition de la classe bonhomme
==================================*/
class bonHomme
{
public:
	bonHomme();												//constructeur sans paramètres	
	bonHomme(int posX, int posY, const char *nomTexture);	//constructeur avec texture et positions							
	~bonHomme();											//destructeur

	int getIndiceI();							//accéder à l'indice I
	int getIndiceJ();							//accéder à l'indice J
	int getPosX();								//accéder à la position x
	int getPosY();								//accéder à la position y
	Vector2f getPosition();						//accéder aux positions x,y
	Sprite getSprite();							//accéder au sprite
	float getVitesse();							//accéder vitesse
	int getCounterWalking();					//accéder counterwalking
	int getDirection();							//accéder direction					
	bomb getBombe();							//accéder à la bombe
	Time getTempsBombe();						//accéder au temps de la bombe
	Keyboard::Key getTouche(int indiceTouche);	//accéder à la touche (haut,bas,gauche,droite)

	void setBombe(bomb bombe);					//modifier la bombe
	void setPosX(int x);						//modifier la position x
	void setPosY(int y);						//modifier la position y
	void setIndiceI(int I);						//modifier l'indice I
	void setIndiceJ(int J);						//modifier l'indice J
	void setVitesse(float vitesse);				//modifier vitesse
	void setCounterWalking(int counter);		//modifier CounterWalking
	void setDirection(int dir);					//modifier direction
	void setPosSprite();						//divise le charset + positions
	void setTexture(const char* nomTexture);	//modifier texture
	void setTouche(Keyboard::Key touche1,
					Keyboard::Key touche2, Keyboard::Key touche3, 
					Keyboard::Key touche4, Keyboard::Key touche5);			
					//modifier choix touches

	void init(int posX, int posY, const char *nomTexture);	//initialise à position, texture, touches voulues
	void update();											//refresh le position 
	void deplacer(int direction);							//pour le déplacement
	bool siCollisions(int direction, grille13x15 &grille);	//collisions (murs stables+destructibles)
	void deposerBombe();									//déposer sa bombe
	bool siBombeDeposee();									//verifie si bombe deposee
	void exploserBombe();									//pour changer de sprite bombe
	int searchTouche(Keyboard::Key touche);					//recherche touche enfoncée

private:

	/*Variables pour le déplacement
	===============================*/
	int _posX, _posY;
	Vector2f _position;
	int _indiceI, _indiceJ;
	float _vitesse = 32;		//nombre plus grand = vitesse plus grande
	int _counterWalking = 0;	//pour changer de sprite (créer l'animation)
	int _direction = 0;

	/*Autres
	=========*/
	Sprite _sprite;
	IntRect _posSprite[4][3];
	Keyboard::Key _touche[5];
	Texture _texture;
	bomb _maBombe;
	bool _maBombeDeposee = false;
	Clock _horlogeBombe;
};

// Constructeur par défaut (pos 32X32)
bonHomme::bonHomme()
{
	init(32, 32, "Power.png");
}

// Constructeur (positionX/Y, texture)
bonHomme::bonHomme(int posX, int posY, const char *nomTexture)
{
	init(posX,posY, nomTexture);
}

//Destructeur
bonHomme::~bonHomme()
{
}

//modifier la bombe
void bonHomme::setBombe(bomb bombe)
{
	bombe = _maBombe;
}


//modifier l'indice I
void bonHomme::setIndiceI(int I)
{
	_indiceI = I;

}

//modifier l'indice J						
void bonHomme::setIndiceJ(int J)
{
	_indiceJ = J;
}

//modifier la position x
void bonHomme::setPosX(int x) //assigne la coordonnée x
{
	assert(x < 416 && "La position du bonhomme dépasse la largeur de fenêtre disponible");
	_posX = x;
}

//modifier la position y
void bonHomme::setPosY(int y) //assigne la coordonnée y
{
	assert(y < 480 && "La position du bonhomme dépasse la hauteur de fenêtre disponible");
	_posY = y;
}

//divise le charset + positions
void bonHomme::setPosSprite()
{
	//pour diviser le charset
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			_posSprite[i][j].left = j * 32;
			_posSprite[i][j].top = i * 32;
			_posSprite[i][j].width = 32;
			_posSprite[i][j].height = 32;
		}
	}
}

// modifier la vitesse
void bonHomme::setVitesse(float vitesse)
{
	assert(vitesse < 1 && vitesse >0, "La vitesse du bonhomme doit être positive et inférieure à 1");
	_vitesse = vitesse;

}

// setteur CounterWalking
void bonHomme::setCounterWalking(int counter)
{
	assert(counter < 4 && counter >= 0, "Le compteur de positions de l'image du bonhomme dans le charset doit être positif et plus petit que 4");
	_counterWalking = counter;

}

// setteur Direction
void bonHomme::setDirection(int dir)
{
	assert(dir < 4 && dir >= 0, "La direction du bonhomme doit être positive et inférieure à 4");
	_direction = dir;

}

//modifier la texture utilisée
void bonHomme::setTexture(const char* nomTexture)
{
	//load texture du bouton
	//échec ouverture fichier de texture 
	assert(_texture.loadFromFile(nomTexture) && "La texture du bonhomme n'a pas chargé correctement");

	//pointeur vers texture, le rectangle a maintenant cette texture 
	_sprite.setTexture(_texture);
}

//set les touches
void bonHomme::setTouche(Keyboard::Key toucheBas, Keyboard::Key toucheGauche, Keyboard::Key toucheDroite, Keyboard::Key toucheHaut, Keyboard::Key toucheBombe)
{
	_touche[0] = toucheBas;		//Bas
	_touche[1] = toucheGauche;	//Gauche
	_touche[2] = toucheDroite;	//Droite
	_touche[3] = toucheHaut;	//Haut
	_touche[4] = toucheBombe;	//Bombe
}

//recherche touche enfoncée
int  bonHomme::searchTouche(Keyboard::Key touche)
{
	for (int i = 0; i < 5; i++)		//pour toutes les touches
		if (touche == _touche[i])	//touche enfoncée
			return i;				//retourne indice
	return 5;						//retourne taille
}


//accéder à la bombe
bomb bonHomme::getBombe()
{
	return _maBombe;

}

//accéder au temps de la bombe
Time bonHomme::getTempsBombe()						
{
	Time tempsBombe;
	tempsBombe = _horlogeBombe.getElapsedTime();
	return tempsBombe;
}

//accéder à la touche
Keyboard::Key bonHomme::getTouche(int indiceTouche)
{
	return _touche[indiceTouche];
}

//accéder à l'indice I
int bonHomme::getIndiceI()
{
	return _indiceI;
}

//accéder à l'indice J
int bonHomme::getIndiceJ()
{
	return _indiceJ;
}

//accéder à la position x
int bonHomme::getPosX() //retourne la coordonnée x
{
	_posX = _sprite.getPosition().x;

	return _posX;
}

//accéder à la position y
int bonHomme::getPosY() //retourne la coordonnée y
{
	_posY = _sprite.getPosition().y;
	return _posY;
}

//accéder aux positions x,y
Vector2f bonHomme::getPosition()
{
	_position = _sprite.getPosition();
	return _position;
}


//  getteur vitesse
float bonHomme::getVitesse()
{
	return _vitesse;
}

// getteur CounterWalking
int bonHomme::getCounterWalking()
{
	return _counterWalking;
}

// getteur direction
int bonHomme::getDirection()
{
	return _direction;
}

//getteur sprite
Sprite bonHomme::getSprite()
{
	return _sprite;
}

//détermine valeur de vérité collisions
bool bonHomme::siCollisions(int direction, grille13x15 &grille)
{
	int I = getIndiceI();
	int J = getIndiceJ();

	switch (direction)
	{
	case 0:	//bas
		I++;
		break;
	case 1: //gauche
		J--;
		break;
	case 2: //droite
		J++;
		break;
	case 3: //haut
		I--;
		break;
	}
	return grille.getValeurTuile(I,J) == 1 || grille.getValeurTuile(I, J) == 2; //deux murs 1 et 2
}

// Initialiser à la position voulue
void bonHomme::init(int posX, int posY, const char *nomTexture)
{
	//set la texture choisie
	setTexture(nomTexture);

	setPosSprite(); //division du charset

	//Initialise pos bonhomme avec origine fenêtre
	_sprite.setPosition(posX, posY);

	//Retenir l'indice du bonhomme
	setIndiceI(posY / 32);
	setIndiceJ(posX / 32);

	//Initialise texture du bonhomme au premier sprite
	_sprite.setTextureRect(IntRect(0, 0, 32, 32)); //avec les coordonnées x y dans le charset

	_maBombe.init(posX, posY, "bombesExplosions.png");
}

//refresh la position 
void bonHomme::update()
{
	_sprite.setPosition(_sprite.getPosition());
}

//pour le déplacement
void bonHomme::deplacer(int direction)
{
	switch (direction)
	{
	case 0:	//bas
		_sprite.move(0, _vitesse);
		_indiceI++;
		break;
	case 1: //gauche
		_sprite.move(-_vitesse, 0);
		_indiceJ--;
		break;
	case 2: //droite
		_sprite.move(_vitesse, 0);
		_indiceJ++;
		break;
	case 3: //haut
		_sprite.move(0, -_vitesse);
		_indiceI--;
		break; 
	}

	//pour utiliser la direction pour les sprites
	_direction = direction;

	//choisi le sprite
	_sprite.setTextureRect(_posSprite[_direction][_counterWalking]); //après chaq move

	_counterWalking++;

	//changer l'image dans sprite sheet
	if (_counterWalking == 3)
		_counterWalking = 0;
}

//deposer bombe
void bonHomme::deposerBombe()
{
	//prend la position du bonhomme
	_maBombe.setPosition(_sprite.getPosition());

	_maBombeDeposee = true;

	//temps de la bombe 
	_horlogeBombe.restart();

	//prend l'indice de grille du bonhomme
	_maBombe.setIndiceI(_indiceI);
	_maBombe.setIndiceJ(_indiceJ);
}

//vérifie si une bombe a été déposée
bool bonHomme::siBombeDeposee()
{
	if (_maBombeDeposee)
	{
		return true;
	}
	else
	{
		return false;
	}

}

////pour changer de sprite
//void bonHomme::exploserBombe()
//{
//	//choisi le sprite d'explosion
//	_maBombe.getSprite().setTextureRect(IntRect(1,2,32,32)); //avec les coordonnées x y dans le sprite sheet
//
//	//pour une raison que j'ignore, la texture ne change pas
//}



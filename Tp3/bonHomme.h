/* En-t�te du programme
====================
Programme:        bonHomme.h
Auteur:           Chlo� et Sey
Date cr�ation:    28/04/2017
Description:	  Cette librairie regroupe le code n�cessaire � l'objet bonhomme, 
					qui utilise les propri�t�s priv�s de position, d'indice de position,
					de vitesse, de counterwalking, de direction, de sprite, de posSprite,
					de touche, de texture, de bombe, un bool�en bombedeposee.*/

/* Directives au pr�-processeur
============================*/
#pragma once
#include <iostream>
#include  <SFML/Graphics.hpp>	//librairie graphique
#include <cassert>				//pour v�rifier et afficher des messages d'erreur
#include "grille.h"				//acc�s � la grille (collisions)
#include "bomb.h"				//chaque bonhomme a  1 bombe
using namespace sf;				//pour SFML
using namespace std;

/*D�finition de la classe bonhomme
==================================*/
class bonHomme
{
public:
	bonHomme();												//constructeur sans param�tres	
	bonHomme(int posX, int posY, const char *nomTexture);	//constructeur avec texture et positions							
	~bonHomme();											//destructeur

	int getIndiceI();							//acc�der � l'indice I
	int getIndiceJ();							//acc�der � l'indice J
	int getPosX();								//acc�der � la position x
	int getPosY();								//acc�der � la position y
	Vector2f getPosition();						//acc�der aux positions x,y
	Sprite getSprite();							//acc�der au sprite
	float getVitesse();							//acc�der vitesse
	int getCounterWalking();					//acc�der counterwalking
	int getDirection();							//acc�der direction					
	bomb getBombe();							//acc�der � la bombe
	Time getTempsBombe();						//acc�der au temps de la bombe
	Keyboard::Key getTouche(int indiceTouche);	//acc�der � la touche (haut,bas,gauche,droite)

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

	void init(int posX, int posY, const char *nomTexture);	//initialise � position, texture, touches voulues
	void update();											//refresh le position 
	void deplacer(int direction);							//pour le d�placement
	bool siCollisions(int direction, grille13x15 &grille);	//collisions (murs stables+destructibles)
	void deposerBombe();									//d�poser sa bombe
	bool siBombeDeposee();									//verifie si bombe deposee
	void exploserBombe();									//pour changer de sprite bombe
	int searchTouche(Keyboard::Key touche);					//recherche touche enfonc�e

private:

	/*Variables pour le d�placement
	===============================*/
	int _posX, _posY;
	Vector2f _position;
	int _indiceI, _indiceJ;
	float _vitesse = 32;		//nombre plus grand = vitesse plus grande
	int _counterWalking = 0;	//pour changer de sprite (cr�er l'animation)
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

// Constructeur par d�faut (pos 32X32)
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
void bonHomme::setPosX(int x) //assigne la coordonn�e x
{
	assert(x < 416 && "La position du bonhomme d�passe la largeur de fen�tre disponible");
	_posX = x;
}

//modifier la position y
void bonHomme::setPosY(int y) //assigne la coordonn�e y
{
	assert(y < 480 && "La position du bonhomme d�passe la hauteur de fen�tre disponible");
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
	assert(vitesse < 1 && vitesse >0, "La vitesse du bonhomme doit �tre positive et inf�rieure � 1");
	_vitesse = vitesse;

}

// setteur CounterWalking
void bonHomme::setCounterWalking(int counter)
{
	assert(counter < 4 && counter >= 0, "Le compteur de positions de l'image du bonhomme dans le charset doit �tre positif et plus petit que 4");
	_counterWalking = counter;

}

// setteur Direction
void bonHomme::setDirection(int dir)
{
	assert(dir < 4 && dir >= 0, "La direction du bonhomme doit �tre positive et inf�rieure � 4");
	_direction = dir;

}

//modifier la texture utilis�e
void bonHomme::setTexture(const char* nomTexture)
{
	//load texture du bouton
	//�chec ouverture fichier de texture 
	assert(_texture.loadFromFile(nomTexture) && "La texture du bonhomme n'a pas charg� correctement");

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

//recherche touche enfonc�e
int  bonHomme::searchTouche(Keyboard::Key touche)
{
	for (int i = 0; i < 5; i++)		//pour toutes les touches
		if (touche == _touche[i])	//touche enfonc�e
			return i;				//retourne indice
	return 5;						//retourne taille
}


//acc�der � la bombe
bomb bonHomme::getBombe()
{
	return _maBombe;

}

//acc�der au temps de la bombe
Time bonHomme::getTempsBombe()						
{
	Time tempsBombe;
	tempsBombe = _horlogeBombe.getElapsedTime();
	return tempsBombe;
}

//acc�der � la touche
Keyboard::Key bonHomme::getTouche(int indiceTouche)
{
	return _touche[indiceTouche];
}

//acc�der � l'indice I
int bonHomme::getIndiceI()
{
	return _indiceI;
}

//acc�der � l'indice J
int bonHomme::getIndiceJ()
{
	return _indiceJ;
}

//acc�der � la position x
int bonHomme::getPosX() //retourne la coordonn�e x
{
	_posX = _sprite.getPosition().x;

	return _posX;
}

//acc�der � la position y
int bonHomme::getPosY() //retourne la coordonn�e y
{
	_posY = _sprite.getPosition().y;
	return _posY;
}

//acc�der aux positions x,y
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

//d�termine valeur de v�rit� collisions
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

// Initialiser � la position voulue
void bonHomme::init(int posX, int posY, const char *nomTexture)
{
	//set la texture choisie
	setTexture(nomTexture);

	setPosSprite(); //division du charset

	//Initialise pos bonhomme avec origine fen�tre
	_sprite.setPosition(posX, posY);

	//Retenir l'indice du bonhomme
	setIndiceI(posY / 32);
	setIndiceJ(posX / 32);

	//Initialise texture du bonhomme au premier sprite
	_sprite.setTextureRect(IntRect(0, 0, 32, 32)); //avec les coordonn�es x y dans le charset

	_maBombe.init(posX, posY, "bombesExplosions.png");
}

//refresh la position 
void bonHomme::update()
{
	_sprite.setPosition(_sprite.getPosition());
}

//pour le d�placement
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
	_sprite.setTextureRect(_posSprite[_direction][_counterWalking]); //apr�s chaq move

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

//v�rifie si une bombe a �t� d�pos�e
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
//	_maBombe.getSprite().setTextureRect(IntRect(1,2,32,32)); //avec les coordonn�es x y dans le sprite sheet
//
//	//pour une raison que j'ignore, la texture ne change pas
//}



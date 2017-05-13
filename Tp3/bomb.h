/* En-t�te du programme
====================
Programme:        bonHomme.h
Auteur:           Chlo� Couture et Sey
Date cr�ation:    28/04/2017
Description:	  Cette librairie regroupe le code n�cessaire � l'objet bombe
					qui utilise les propri�t�s priv�es de texture, sprite, position, indices, posSprite*/

/* Directives au pr�-processeur
============================*/
#pragma once
#include <iostream>
#include  <SFML/Graphics.hpp>	//librairie graphique
#include <cassert>				//pour v�rifier et afficher des messages d'erreur
using namespace sf;				//pour SFML
using namespace std;

/*D�finition de la classe bomb
==================================*/
class bomb
{
public:

	bomb();										//constructeur bomb
	~bomb();									//destructeur

	Sprite getSprite();							//acc�der � sprite
	int getIndiceI();							//acc�der � l'indice I
	int getIndiceJ();							//acc�der � l'indice J

	void setTexture(const char* nomTexture);	//modifier la texture
	void initPosSprite();						//init tableau qui divise le sprite sheet
	void setPosSprite(int I, int J);			//set un rectangle de texture avec pos
	
	void setIndiceI(int I);						//modifier l'indice I
	void setIndiceJ(int J);						//modifier l'indice J

	void init(int posX, int posY, const char *nomTexture);		//init une bombe avec texture et pos.
	void setPosition(Vector2f  position);						//setteur position
	void exploserBombe();
	void disparaitre(grille13x15 grille, RenderWindow &window);	//efface un tuile avec valeur dans grille

private:

	/*Variables pour le sprite bombe
	===============================*/
	IntRect _posSprite[2][4];
	Sprite _bomb;
	Texture _textureBomb;
	Vector2f _position;
	int _indiceI;
	int _indiceJ;
};

// Constructeur pour le bomb (position par d�faut; 32 x 32)
bomb::bomb()
{
}

//Destructeur
bomb::~bomb()
{
}

//modifier l'indice I
void bomb::setIndiceI(int I)
{
	_indiceI = I;

}

//modifier l'indice J						
void bomb::setIndiceJ(int J)
{
	_indiceJ = J;
}

// modifier la texture utilis�e
void bomb::setTexture(const char* nomTexture)
{
	//�chec ouverture fichier de texture 
	assert(_textureBomb.loadFromFile(nomTexture) && "La texture de la bombe n'a pas charg� correctement");

	//pointeur vers texture, le rectangle a maintenant cette texture 
	_bomb.setTexture(_textureBomb);
}

//divise le charset + positions
void bomb::initPosSprite()
{
	//pour diviser le sprite sheet
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			_posSprite[i][j].left = j * 32;
			_posSprite[i][j].top = i * 32;
			_posSprite[i][j].width = 32;
			_posSprite[i][j].height = 32;
		}
	}
}

//setteur rectangle dans sprite sheet
void bomb::setPosSprite(int I, int J)
{
	_bomb.setTextureRect(_posSprite[I][J]);
}

// setteur position
void bomb::setPosition(Vector2f position)
{
	_position = position;

	_bomb.setPosition(_position);
}

//getteur sprite
Sprite bomb::getSprite()
{
	return _bomb;
}

//acc�der � l'indice I
int bomb::getIndiceI()
{
	return _indiceI;
}

//acc�der � l'indice J
int bomb::getIndiceJ()
{
	return _indiceJ;
}

// Initialiser � la position voulue
void bomb::init(int posX, int posY, const char *nomTexture)
{
	//set la texture choisie
	setTexture(nomTexture);

	initPosSprite(); //division du sprite sheet

	//set position de la bombe
	_position.x = posX;
	_position.y = posY;

	setPosition(_position);

	//Retenir l'indice de la bombe
	setIndiceI(posY / 32);
	setIndiceJ(posX / 32);

	//Initialise texture du bonhomme au premier sprite
	_bomb.setTextureRect(_posSprite[0][0]); //avec les coordonn�es x y dans le sprite sheet

}

//pour changer de sprite
void bomb::exploserBombe()
{
	//choisi le sprite d'explosion
	_bomb.setTextureRect(_posSprite[1][2]); //avec les coordonn�es x y dans le sprite sheet

																  //pour une raison que j'ignore, la texture ne change pas
}

//efface un tuile selon valeur dans grille
void bomb::disparaitre(grille13x15 grille, RenderWindow &window)
{
		grille.dessineTuileVide(_indiceI,_indiceJ, window);
}
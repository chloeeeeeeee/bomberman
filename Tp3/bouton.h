/* En-tête du programme
====================
Programme:        bouton.h
Auteur:           Chloé et Sey
Date création:    28/04/2017
Description:	  Cette librairie regroupe le code nécessaire à l'objet bouton, 
				  qui comporte une texture, une hauteur et une largeur, une position x,y, une couleur. 
				  La texture peut provenir d'un sprite sheet où plusieurs boutons sont présentés, avec des couleurs différentes 
				  pour chaque ligne et un état différent pour chaque colonne dans le sprite sheet */

/* Directives au pré-processeur
============================*/
#pragma once
#include <iostream>
#include  <SFML/Graphics.hpp>	//pour la librairie graphique
#include <cassert>				//pour vérifier et afficher des messages d'erreur
using namespace sf;				//pour la librairie graphique
using namespace std;

/*Constantes globales utilisées
================================*/
const int NBCOLONNESBOUTONS = 2;
const int NBLIGNESBOUTON = 9;

/*Définition de la classe bouton
==================================*/
class bouton
{
private:

	int _x, _y;												//coordonnées du coin supérieur gauche (propriété)
	int _largeur, _hauteur;									//largeur et hauteur (propriété)
	Texture _texture;										//texture du bouton (image)
	Sprite _bouton;											//rectangle graphique du bouton

	IntRect _posBouton[NBLIGNESBOUTON][NBCOLONNESBOUTONS];	//tableaux de rectangles de boutons dans un sprite sheet	
	int _boutonClique = 0;									//pour changer l'état du bouton (0 non cliqué, 1 cliqué)


	int _couleur = 0;										//pour choisir la couleur 0 au nombre max spécifié. 
															//Le sprite sheet utilisé dans le programme a pour couleurs:
															//0(gris),1(rose),2(orange),3(vert),4(bleu),5(mauve),6(turquoise),7(gris bleu) ou 8(gris foncé)

	Text _texte;											//texte utilisé pour le bouton
	Font _police;											//pour garder le pointeur de la police de texte du bouton

public:

	//constructeur avec position/dimensions et choix de texture, couleur, texte et police.
	bouton(int x, int y, int largeur, int hauteur, const char *nomTexture, int couleur, const char *texteBouton, const char *nomPolice);
																																	
	bouton();	//constructeur sans paramètres
	~bouton();	//destructeur

	int getPosX();								//accéder à la position x
	int getPosY();								//accéder à la position y
	int getLargeur();							//accéder à la largeur
	int getHauteur();							//accéder à la hauteur
	int getCouleur();							//accéder à la couleur
	Text getTexte();							//accéder au texte

	void setPosX(int x);						//modifier la position x
	void setPosY(int y);						//modifier la position y
	void setLargeur(int largeur);				//modifier la largeur
	void setHauteur(int hauteur);				//modifier la hauteur
	void setTexture(const char *nomTexture);	//modifier la texture
	void setCouleur(int couleur);				//modifier la couleur
	void setTexte(const char *texteBouton);		//modifier le texte
	void setPolice(const char *nomPolice);		//modifier la police de texte

	void setStyleTexte(const char *nomPolice, int tailleCaractere, Color couleur, bool souligne, bool gras); //modifier le style du texte

	void setPosition(int x, int y);				//modifer les positions
	void setDimension(int largeur, int hauteur);//modifier les dimensions

	//initialiser bouton (positions, largeur/hauteur, texture, couleur, texte, police)
	void init(int x, int y, int largeur, int hauteur, const char *nomTexture, int couleur, const char *texteBouton, const char *nomPolice);

	void update();								//réinitialiser le bouton lors d'évènements (ex; cliqué)
	void dessine(RenderWindow &window);			//dessiner le bouton 
};

/*Définitions des méthodes
============================*/
//constructeur sans paramètres
bouton::bouton()
{
}

//constructeur avec position/dimensions et choix de texture, couleur, texte et police
bouton::bouton(int x, int y, int largeur, int hauteur, const char *nomTexture, int couleur, const char *texteBouton, const char *nomPolice)
{
	init(x, y, largeur, hauteur, nomTexture, couleur, texteBouton, nomPolice);
}

//destructeur
bouton ::~bouton()
{
	_x = _y = _largeur = _hauteur = _boutonClique = _couleur = 0; //efface valeurs en mémoire pour mettre 0
}

//accéder à la position x
int bouton::getPosX() //retourne la coordonnée x
{
	//_x = _bouton.getPosition().x;
	return _x;
}

//accéder à la position y
int bouton::getPosY() //retourne la coordonnée y
{
	//_y = _bouton.getPosition().y;
	return _y;
}

//accéder à la largeur
int bouton::getLargeur() //retourne la largeur
{
	return _largeur;
}

//accéder à la hauteur
int bouton::getHauteur() //retourne la hauteur
{
	return _hauteur;
}

//accéder à la couleur
int bouton::getCouleur() //retourne le numéro de la couleur
{
	return _couleur;
}

//accéder au texte
Text bouton::getTexte() //retourne texte
{
	return _texte;
}

//modifier la position x
void bouton::setPosX(int x) //assigne la coordonnée x
{
	assert(x < 416 && "La position du bouton dépasse la largeur de fenêtre disponible");
	_x = x;
}

//modifier la position y
void bouton::setPosY(int y) //assigne la coordonnée y
{
	assert(y < 480 && "La position du bouton dépasse la hauteur de fenêtre disponible");
	_y = y;
}

//modifier la largeur
void bouton::setLargeur(int largeur) //assigne la largeur
{
	assert(largeur > 0 && "La largeur du bouton ne peut être nulle");
	_largeur = largeur;
}

//modifier la hauteur
void bouton::setHauteur(int hauteur) //assigne la hauteur
{
	assert(hauteur > 0 && "La hauteur du bouton ne peut être nulle");
	_hauteur = hauteur;
}

//modifier la texture utilisée
void bouton::setTexture(const char* nomTexture)
{
	//load texture du bouton
	assert(_texture.loadFromFile(nomTexture) && "La texture du bouton n'a pas chargé correctement");

	//pointeur vers texture, le rectangle a maintenant cette texture 
	_bouton.setTexture(_texture);
}

//modifier la couleur utilisée
void bouton::setCouleur(int couleur)
{
	assert(couleur < NBCOLONNESBOUTONS && "La couleur du bouton n'existe pas dans le sprite sheet des boutons");
	_couleur = couleur;
}

//modifier la police de texte du bouton
void bouton::setPolice(const char *nomPolice)
{
	//load de la police
	assert(_police.loadFromFile(nomPolice) && "La police de texte du bouton n'a pas chargée correctement");

	//sélection de la police
	_texte.setFont(_police);
}

//modifier les positions
void bouton::setPosition(int x, int y) //assigne les coordonnées
{
	setPosX(x); 
	setPosY(y);
}

//modifier les dimensions
void bouton::setDimension(int largeur, int hauteur) //assigne les dimensions
{
	setLargeur(largeur);
	setHauteur(hauteur);
}

//modifier le texte d'un bouton
void bouton::setTexte(const char *texteBouton)
{
	//change le texte
	_texte.setString(texteBouton);
}

//modifier le style du texte dans un bouton
void bouton::setStyleTexte(const char *nomPolice, int tailleCaractere, Color couleur, bool souligne, bool gras)
{
	//nouvelle police
	setPolice(nomPolice);

	//set taille de caractère
	_texte.setCharacterSize(tailleCaractere); //en pixels

	//set couleur du texte
	_texte.setFillColor(couleur);

	if (souligne)
		//texte souligné
		_texte.setStyle(Text::Bold);
	else if (gras)
		//texte en gras
		_texte.setStyle(Text::Underlined);
}

//initialise les positions, les dimensions, nom fichier texture, la couleur, le texte, la police
void bouton::init(int x, int y, int largeur, int hauteur, const char *nomTexture, int couleur, const char *texteBouton, const char *nomPolice)
{
	setPosition(x, y);
	setDimension(largeur, hauteur);
	setTexture(nomTexture);

	//division du sprite sheet
	for (int i = 0; i < NBLIGNESBOUTON; i++)
	{
		for (int j = 0; j < NBCOLONNESBOUTONS; j++)
		{
			_posBouton[i][j].left = largeur * j;
			_posBouton[i][j].top = hauteur * i;
			_posBouton[i][j].width = largeur;
			_posBouton[i][j].height = hauteur;
		}
	}

	//initialise pos avec origine fenêtre
	_bouton.setPosition(x, y);

	//initialise texture du bouton au premier sprite
	_bouton.setTextureRect(_posBouton[0][0]); //bouton gris non cliqué	

	//initialise le texte dans un bouton
	setTexte(texteBouton);

	//initialise un style de texte Xeron, 10 pixels, noir
	setStyleTexte(nomPolice, 10, Color::Black, false, false);

}

//pour réinitialiser le bouton lors d'évènements (ex; cliqué)
void bouton::update()	//change le sprite du bouton selon s'il est cliqué (bouton renfoncé ou non)
{
	//le bouton a été cliqué
	_boutonClique++;

	//changer l'image dans sprite sheet
	if (_boutonClique == NBCOLONNESBOUTONS)
		_boutonClique = 0;	
	
	//Initialise texture du bouton au sprite voulu
	_bouton.setTextureRect(_posBouton[_couleur][_boutonClique]);
}

//dessine un bouton
void bouton::dessine(RenderWindow &window)
{
	window.draw(_bouton);

	//dessine le texte du bouton à la même position que le bouton
	_texte.setPosition(_bouton.getPosition());

	window.draw(_texte);
}
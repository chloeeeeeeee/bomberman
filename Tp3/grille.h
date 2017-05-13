/*En-t�te de la librairie
=========================
Nom:				grille.h
Auteur:				Chlo� et Sey
Date de cr�ation :  25 avril 2017
Description:		Cette librairie regroupe le code n�cessaire � l'objet grille du jeu BomberMan,
					avec comme propri�t�s un tableau de valeurs de tuiles (0, 1 ou 2), des vecteurs de sprites,
					un tableau des textures des diff�rentes tuiles.*/

/*Librairies
=============*/
#pragma once
#include <iostream>
#include <SFML\graphics.hpp>	//librairie graphique
#include <fstream>				//fichiers
#include "securiserBomberMan.h"	//securiser fichiers
#include <cassert>				//pour v�rifier et afficher des messages d'erreur
using namespace sf;				//librairie graphique
using namespace std;

/*Constantes globales
======================*/
const int NBCOLONNES = 13;
const int NBLIGNES = 15;
const int NBIMAGE = 3;

/*Instructions de la classe grille
=====================================*/
class grille13x15
{
private:

	int _valeurTuile[NBLIGNES][NBCOLONNES];				//valeur tuiles dans fichier texte (0 = vide, 1 = murStable, 2 = mur)

	Sprite _spriteTuile[NBLIGNES][NBCOLONNES];			//sprites des tuiles de la grille
	Texture _textureTuile[NBIMAGE];						//3 textures de tuiles

public:
	int getValeurTuile(int ligne, int colonne);			//acc�der valeur 1 tuile
	Texture getTextureTuile(int ligne);					//acc�der texture 1 tuile
	void grille13x15::dessineTuileVide(int IndiceI, int IndiceJ, RenderWindow &window);	//dessine 1 tuile voulue avec indice

	void init(const char* nomFichier);					//initialise grille � partir du fichier texte
	void grille13x15::dessine(RenderWindow &window);	//dessine grille au complet avec textures
};

//acc�der valeur 1 tuile
int grille13x15::getValeurTuile(int ligne, int colonne)
{
	return _valeurTuile[ligne][colonne];

}

//acc�der texture 1 tuile
Texture grille13x15::getTextureTuile(int ligne)
{
	return _textureTuile[ligne];
}

//dessine tuile avec texture
void grille13x15::dessineTuileVide(int IndiceI, int IndiceJ, RenderWindow &window)
{
	//pointeur vers texture, le rectangle a maintenant cette texture 
	_spriteTuile[IndiceI][IndiceJ].setTexture(_textureTuile[0]);

	window.draw(_spriteTuile[IndiceI][IndiceJ]); //dessiner les tuiles dans la back buffer
}

//initialise grille � partir du fichier texte
void grille13x15::init(const char* nomFichier) //pour envoyer en param�tre une chaine de caracteres constante entre guillemets
{
	//Variables
	char nomImage[NBIMAGE][15]; //pour les noms des textures
	ifstream entree;			//fichier Labyrinthe.txt
	int noImage;				//variable tampon pour no d'image laiss� dans le fichier texte

	if (ouvrirFichier(entree, nomFichier)) //le fichier du labyrinthe a �t� ouvert
	{
		if (!fichierVide(entree)) //le fichier n'est pas vide
		{
			//lecture des valeurs 0, 1 et 2 et stockage des positions des tuiles
			for (int i = 0; i < NBLIGNES; i++)
			{
				for (int j = 0; j < NBCOLONNES; j++)
				{
					entree >> _valeurTuile[i][j]; //stock�es dans le tab valeurTuile
					_spriteTuile[i][j].setPosition(j*32.0, i*32.0); //position des tuiles 
				}
			}

			//lecture des noms des images
			for (int i = 0; i < NBIMAGE; i++)
			{
				entree >> noImage >> nomImage[i];

				//les textures n'ont pas �t� load�s
				assert(_textureTuile[i].loadFromFile(nomImage[i]) && "Les textures n'ont pas charg� correctement");
			}
		}
		else
			assert("Le fichier de donn�es de la grille de jeu est vide");
	}
	else
		assert("Le fichier de donn�es de la grille de jeu n'a pas pu s'ouvrir");
}

//dessine grille au complet avec textures
void grille13x15::dessine(RenderWindow &window)
{
	//pour toute la grille
	for (int i = 0; i < NBLIGNES; i++)
	{
		for (int j = 0; j < NBCOLONNES; j++)
		{
			//pointeur vers texture, le rectangle a maintenant cette texture 
			_spriteTuile[i][j].setTexture(_textureTuile[_valeurTuile[i][j]]);
			window.draw(_spriteTuile[i][j]); //dessiner les tuiles dans la back buffer
		}
	}
}
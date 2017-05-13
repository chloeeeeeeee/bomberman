/* En-tête du programme
====================
Programme:        elferMan.cpp
Auteurs:          Chloé et Sey
Date création:    28/04/2017
Description:	  Jeu BomberMan, où le but est de détruire les murs,
				  trouver des objets spéciaux et tuer d'autres joueurs à l'aide de bombes dans un temps donné*/

/* Directives au pré-processeur
============================*/
#include <iostream>
#include  <SFML/Graphics.hpp>	//librairie graphique
#include <cassert>				//pour vérifier et afficher des messages d'erreur
#include "bonHomme.h"			//objet bomhomme
#include "grille.h"				//objet grille
#include "bouton.h"				//objet bouton
#include "bomb.h"				//objet bombe, utilisé dans bonhomme
#include "SFML\Audio.hpp"		//pour musique
using namespace sf;				//librairie SFML
using namespace std;

/*Prototypes
=============*/
void initMenu(RenderWindow &windowMenu, bouton boutonsMenu[], Image &icon, RectangleShape &imageMenu, Texture &textureImageMenu);
void dessinerMenu(RenderWindow &windowMenu, bouton boutonsMenu[], RectangleShape &imageMenu);
void deroulementMenu(RenderWindow &windowMenu, bool &menu, bouton boutonsMenu[], int &indiceBoutonMenu, RectangleShape &imageMenu, Clock &clock, Time &tempsEcoule1, Time &t1, bool &jeu, bool &options);

/*Instructions principales
===========================*/
int main()
{
	/*Constantes
	=============*/
	const int nbJoueurs = 3;

	/*Variables
	===========*/
	grille13x15 grilleJeu;		//pour grille du jeu
	Image icon;					//pour icône coin fenêtre
	bonHomme joueur[nbJoueurs];	//création trois joueurs
	Event evnt;					//pour évènements du jeu
	bool menu = true;			//pour menu
	bool options = false;		//pour menu d'options
	bool jeu = false;			//pour jeu
	int direction;				//direction prise par les bonhommes

	/*Pour le menu de départ
	==========================*/
	bouton boutonsMenu[3];		//trois boutons menu
	int indiceBoutonMenu = 0;	//indice bouton cliqué
	RectangleShape imageMenu;	//image principale menu
	Texture textureImageMenu;	//texture image principale menu
	Time t1 = seconds(0.5f);	//0.5 seconde pour bouton renfoncé

	Time tempsEcoule1;			//temps menu
	Clock clock;				//compteur temps 

	Music musiqueJeu;			//musique jeu

	/*Pour le jeu
	=============*/
	int indTouche;					//indice touche pesée
	Time t2 = seconds(3.0f);		//3 secondes pour bombe
	Time t3 = seconds(5.0f);		//2 secondes pour explosion
	Time tempsBombe;				//temps bombe
	Time tempsJeu;					//temps jeu
	Time t4 = seconds(20.0f);		//20 sec jeu

	/*Instructions
	==============*/

	//création fenêtre 416x480 pixels
	RenderWindow windowMenu(VideoMode(416, 480), "ElferMan", sf::Style::Close | sf::Style::Titlebar);

	//initialise le menu
	initMenu(windowMenu, boutonsMenu, icon, imageMenu, textureImageMenu);

	/*Musique jeu
	=============*/
	musiqueJeu.openFromFile("15.-boss-battle.wav");
	musiqueJeu.play();
	musiqueJeu.setLoop(true); //musique en boucle

	//affiche tout
	windowMenu.display();

	//démarrer programme
	deroulementMenu(windowMenu, menu, boutonsMenu, indiceBoutonMenu, imageMenu, clock, tempsEcoule1, t1, jeu, options);

	//jeu est actif
	while (jeu)
	{
		/*Creation d'une fenêtre de jeu
		===============================*/
		RenderWindow window(VideoMode(416, 480), "ElferMan", sf::Style::Close | sf::Style::Titlebar);

		//Setter l'icone
		window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

		//Initialiser labyrinthe dans le buffer
		grilleJeu.init("Labyrinthe.txt");

		//Init 3 joueurs
		joueur[0].init(32, 32, "Power.png");
		joueur[1].init(352, 416, "actor110.png");
		joueur[2].init(32, 416, "actor3.png");

		//init les touches 
		joueur[0].setTouche(Keyboard::Key::S, Keyboard::Key::A, Keyboard::Key::D, Keyboard::Key::W, Keyboard::Key::Space); //ASWD + Espace(bombes)
		joueur[1].setTouche(Keyboard::Key::Down, Keyboard::Key::Left, Keyboard::Key::Right, Keyboard::Key::Up, Keyboard::Key::Numpad3); //flèches + numpad3(bombes)
		joueur[2].setTouche(Keyboard::Key::K, Keyboard::Key::J, Keyboard::Key::L, Keyboard::Key::I, Keyboard::Key::P); //JKIL + P(bombes)

		window.setKeyRepeatEnabled(false); //désactiver le repeat d'une touche

		//fenêtre de jeu
		while (window.isOpen())
		{
			//affiche grille selon fichier texte
			grilleJeu.dessine(window);

			// afficher les sprites
			for (int i = 0; i < nbJoueurs; i++)
				window.draw(joueur[i].getSprite());

			window.display(); //affiche tout


			//démarrer le jeu
			while (window.waitEvent(evnt))  //attend un évènement
			{
				//prend tempsJeu horloge
				tempsJeu = clock.getElapsedTime();
				cout << tempsJeu.asSeconds() << " secondes de jeu écoulées" << endl; //affiche temps jeu


				//ferme prog echap
				if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
				{
					window.close();	//ferme fenetre avec echap
					jeu = false;
				}

				switch (evnt.type)	//type d'évènement
				{
				case Event::Closed:
					window.close();	//fermeture fenêtre avec X
					jeu = false;
					break;
				}

				//pour 3 joueurs
				for (int i = 0; i < nbJoueurs; i++)
				{
					//si une touche a été pesée
					if (evnt.type == Event::KeyPressed)
					{
						//chercher l'indice touche
						indTouche = joueur[i].searchTouche(evnt.key.code);
						
						if (indTouche != 5)		//touche répertoriée
						{
							if (indTouche == 4) //touche pour bombe
							{
								joueur[i].deposerBombe();

								window.draw(joueur[i].getBombe().getSprite());
							}
							else //touche déplacement
							{
								window.display();		//pour la bombe

								direction = indTouche;	//direction = touche

								if (!joueur[i].siCollisions(direction, grilleJeu)) //pas de collisions
								{
									joueur[i].deplacer(direction);

									//update joueur (position)
									joueur[i].update();
								}
							}
						}
					}
				}

				// afficher tous les sprites						
				grilleJeu.dessine(window);

				//pour les 3 joueurs
				for (int i = 0; i < nbJoueurs; i++)
				{
					if (joueur[i].siBombeDeposee())
					{
						window.draw(joueur[i].getBombe().getSprite());
						tempsBombe = joueur[i].getTempsBombe();

						if (tempsBombe > t2) //3 secondes
						{
							joueur[i].getBombe().exploserBombe();
							window.draw(joueur[i].getBombe().getSprite());
		
							if (tempsBombe > t3) //5.0 secondes
												 //explosion de 2 secondes
								joueur[i].getBombe().disparaitre(grilleJeu, window);
						}
					}
					//pour dessiner 3 joueurs
					window.draw(joueur[i].getSprite());
				}

				//tout afficher
				window.display();

				//20 secondes passées
				if (clock.getElapsedTime() > t4)
				{
					window.close();
					jeu = false;

					/*Creation d'une fenêtre de message
					===================================*/
					window.clear();
					RenderWindow window(VideoMode(416, 480), "ElferMan", sf::Style::Close | sf::Style::Titlebar);

					//Setter l'icone
					window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

					// Load d'une police
					Font fontMessageFin;
					fontMessageFin.loadFromFile("Xeron.ttf");

					// texte
					Text textFin("Trop tard,vous avez perdu!", fontMessageFin);
					textFin.setCharacterSize(15);
					textFin.setStyle(Text::Bold);
					textFin.setFillColor(Color::White);

					// dessiner
					textFin.setPosition(10.0, 100.0);
					window.draw(textFin);
					window.display();

					//pour 3 secondes, montrer message
					Time tempsMessage = seconds(3.0);
					Clock horlogeMessage;

					while (horlogeMessage.getElapsedTime() < tempsMessage)
					{
						//rien faire
					}
				}
			}
		}
	}
}

void initMenu(RenderWindow &windowMenu, bouton boutonsMenu[], Image &icon, RectangleShape &imageMenu, Texture &textureImageMenu)
{
	//loader l'icone de fenêtre
	assert(icon.loadFromFile("bomb.png"), "L'icône de le fenêtre n'a pas pu être chargée correctement");

	//setter l'icône
	windowMenu.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	/*Image principale menu
	========================*/
	//position et taille dans fenêtre
	imageMenu.setSize(Vector2f(230, 142));
	imageMenu.setPosition(93, 10);

	//load texture
	assert(textureImageMenu.loadFromFile("logoBomberMan.bmp"), "Image principale du menu n'a pas pu se charger");
	imageMenu.setTexture(&textureImageMenu);

	/*Boutons
	=========*/
	//déclaration bouton Jouer gris style texte par défaut
	boutonsMenu[0].init(167, 200, 81, 32, "sprite-buttons.png", 0, "Jouer", "Xeron.ttf");

	//déclaration bouton Options gris style texte par défaut
	boutonsMenu[1].init(167, 250, 81, 32, "sprite-buttons.png", 0, "Options", "Xeron.ttf");

	//déclaration bouton Quitter gris style texte par défaut
	boutonsMenu[2].init(167, 300, 81, 32, "sprite-buttons.png", 0, "Quitter", "Xeron.ttf");

	dessinerMenu(windowMenu, boutonsMenu, imageMenu);
}

void dessinerMenu(RenderWindow &windowMenu, bouton boutonsMenu[], RectangleShape &imageMenu)
{
	/*Affiche menu
	===============*/
	//dessine bouton+textebouton
	for (int i = 0; i < 3; i++)
		boutonsMenu[i].dessine(windowMenu);

	//dessine imageMenu
	windowMenu.draw(imageMenu);
}

void deroulementMenu(RenderWindow &windowMenu, bool &menu, bouton boutonsMenu[], int &indiceBoutonMenu, RectangleShape &imageMenu, Clock &clock, Time &tempsEcoule1, Time &t1, bool &jeu, bool &options)
{
	Event evnt;
	//démarrer programme
	while (windowMenu.isOpen()) //fenêtre ouverte
	{
		while (menu) //dans le menu
		{
			while (windowMenu.pollEvent(evnt)) //attend un évènement
			{
				if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) //touche échappement
				{
					windowMenu.close();		//ferme fenêtre
					menu = false;           //ferme programme
				}

				switch (evnt.type)			//type d'évènement
				{
				case Event::Closed:			//ferm. fenêtre avec X
					windowMenu.close();
					menu = false;           //ferm. programme
					break;
				}
			}

			/*Vérifie clic dans menu
			========================*/
			if (Mouse::isButtonPressed(Mouse::Left)) //clic gauche
			{
				Vector2i posSouris = Mouse::getPosition(windowMenu); //avoir la pos. selon la fenêtre et non l'écran

																	 //trouver le bouton cliqué avec pos y boutons
				for (int i = 0; i < 3; i++)
				{
					int posY = boutonsMenu[i].getPosY();

					if (posSouris.y > posY && posSouris.y < (posY + 32))
						indiceBoutonMenu = i;
				}

				//vérifier pos x du bouton
				if (posSouris.x > boutonsMenu[indiceBoutonMenu].getPosX() && posSouris.x < 248)	//jouer, option ou quitter cliqué
				{
					boutonsMenu[indiceBoutonMenu].update(); //on renfonce le bouton

					/*Réaffiche le menu
					===================*/
					dessinerMenu(windowMenu, boutonsMenu, imageMenu);

					//afficher le bouton renfoncé
					boutonsMenu[indiceBoutonMenu].dessine(windowMenu);
					windowMenu.display();

					/*bouton renfoncé pendant 0.5 sec*/
					clock.restart(); //compteur temps recommence
					tempsEcoule1 = clock.getElapsedTime();

					while (tempsEcoule1 < t1) //pause 0.5 sec
					{
						tempsEcoule1 = clock.getElapsedTime();
					}

					//selon le bouton cliqué
					switch (indiceBoutonMenu)
					{
					case 0: //bouton jouer
						menu = false;	//quitter menu
						windowMenu.close();	//fermeture fenêtre
						jeu = true;
						break;
					case 1: //bouton options
						options = true;
						break;
					case 2:	//bouton quitter
						menu = false;	//quitter menu
						windowMenu.close();	//fermeture fenêtre
						break;
					}
				}
			}
		}

		while (options) //bouton options cliqué
		{
			//efface menu
			windowMenu.clear();

			//options additionelles avec poll event pour les touches, boutons, etc.

			//à un moment donné
			options = false;

			//puisque menu = true, on retourne au menu
		}
	}
}
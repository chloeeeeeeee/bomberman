/* En-t�te du programme
====================
Programme:        elferMan.cpp
Auteurs:          Chlo� et Sey
Date cr�ation:    28/04/2017
Description:	  Jeu BomberMan, o� le but est de d�truire les murs,
				  trouver des objets sp�ciaux et tuer d'autres joueurs � l'aide de bombes dans un temps donn�*/

/* Directives au pr�-processeur
============================*/
#include <iostream>
#include  <SFML/Graphics.hpp>	//librairie graphique
#include <cassert>				//pour v�rifier et afficher des messages d'erreur
#include "bonHomme.h"			//objet bomhomme
#include "grille.h"				//objet grille
#include "bouton.h"				//objet bouton
#include "bomb.h"				//objet bombe, utilis� dans bonhomme
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
	Image icon;					//pour ic�ne coin fen�tre
	bonHomme joueur[nbJoueurs];	//cr�ation trois joueurs
	Event evnt;					//pour �v�nements du jeu
	bool menu = true;			//pour menu
	bool options = false;		//pour menu d'options
	bool jeu = false;			//pour jeu
	int direction;				//direction prise par les bonhommes

	/*Pour le menu de d�part
	==========================*/
	bouton boutonsMenu[3];		//trois boutons menu
	int indiceBoutonMenu = 0;	//indice bouton cliqu�
	RectangleShape imageMenu;	//image principale menu
	Texture textureImageMenu;	//texture image principale menu
	Time t1 = seconds(0.5f);	//0.5 seconde pour bouton renfonc�

	Time tempsEcoule1;			//temps menu
	Clock clock;				//compteur temps 

	Music musiqueJeu;			//musique jeu

	/*Pour le jeu
	=============*/
	int indTouche;					//indice touche pes�e
	Time t2 = seconds(3.0f);		//3 secondes pour bombe
	Time t3 = seconds(5.0f);		//2 secondes pour explosion
	Time tempsBombe;				//temps bombe
	Time tempsJeu;					//temps jeu
	Time t4 = seconds(20.0f);		//20 sec jeu

	/*Instructions
	==============*/

	//cr�ation fen�tre 416x480 pixels
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

	//d�marrer programme
	deroulementMenu(windowMenu, menu, boutonsMenu, indiceBoutonMenu, imageMenu, clock, tempsEcoule1, t1, jeu, options);

	//jeu est actif
	while (jeu)
	{
		/*Creation d'une fen�tre de jeu
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
		joueur[1].setTouche(Keyboard::Key::Down, Keyboard::Key::Left, Keyboard::Key::Right, Keyboard::Key::Up, Keyboard::Key::Numpad3); //fl�ches + numpad3(bombes)
		joueur[2].setTouche(Keyboard::Key::K, Keyboard::Key::J, Keyboard::Key::L, Keyboard::Key::I, Keyboard::Key::P); //JKIL + P(bombes)

		window.setKeyRepeatEnabled(false); //d�sactiver le repeat d'une touche

		//fen�tre de jeu
		while (window.isOpen())
		{
			//affiche grille selon fichier texte
			grilleJeu.dessine(window);

			// afficher les sprites
			for (int i = 0; i < nbJoueurs; i++)
				window.draw(joueur[i].getSprite());

			window.display(); //affiche tout


			//d�marrer le jeu
			while (window.waitEvent(evnt))  //attend un �v�nement
			{
				//prend tempsJeu horloge
				tempsJeu = clock.getElapsedTime();
				cout << tempsJeu.asSeconds() << " secondes de jeu �coul�es" << endl; //affiche temps jeu


				//ferme prog echap
				if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
				{
					window.close();	//ferme fenetre avec echap
					jeu = false;
				}

				switch (evnt.type)	//type d'�v�nement
				{
				case Event::Closed:
					window.close();	//fermeture fen�tre avec X
					jeu = false;
					break;
				}

				//pour 3 joueurs
				for (int i = 0; i < nbJoueurs; i++)
				{
					//si une touche a �t� pes�e
					if (evnt.type == Event::KeyPressed)
					{
						//chercher l'indice touche
						indTouche = joueur[i].searchTouche(evnt.key.code);
						
						if (indTouche != 5)		//touche r�pertori�e
						{
							if (indTouche == 4) //touche pour bombe
							{
								joueur[i].deposerBombe();

								window.draw(joueur[i].getBombe().getSprite());
							}
							else //touche d�placement
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

				//20 secondes pass�es
				if (clock.getElapsedTime() > t4)
				{
					window.close();
					jeu = false;

					/*Creation d'une fen�tre de message
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
	//loader l'icone de fen�tre
	assert(icon.loadFromFile("bomb.png"), "L'ic�ne de le fen�tre n'a pas pu �tre charg�e correctement");

	//setter l'ic�ne
	windowMenu.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	/*Image principale menu
	========================*/
	//position et taille dans fen�tre
	imageMenu.setSize(Vector2f(230, 142));
	imageMenu.setPosition(93, 10);

	//load texture
	assert(textureImageMenu.loadFromFile("logoBomberMan.bmp"), "Image principale du menu n'a pas pu se charger");
	imageMenu.setTexture(&textureImageMenu);

	/*Boutons
	=========*/
	//d�claration bouton Jouer gris style texte par d�faut
	boutonsMenu[0].init(167, 200, 81, 32, "sprite-buttons.png", 0, "Jouer", "Xeron.ttf");

	//d�claration bouton Options gris style texte par d�faut
	boutonsMenu[1].init(167, 250, 81, 32, "sprite-buttons.png", 0, "Options", "Xeron.ttf");

	//d�claration bouton Quitter gris style texte par d�faut
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
	//d�marrer programme
	while (windowMenu.isOpen()) //fen�tre ouverte
	{
		while (menu) //dans le menu
		{
			while (windowMenu.pollEvent(evnt)) //attend un �v�nement
			{
				if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) //touche �chappement
				{
					windowMenu.close();		//ferme fen�tre
					menu = false;           //ferme programme
				}

				switch (evnt.type)			//type d'�v�nement
				{
				case Event::Closed:			//ferm. fen�tre avec X
					windowMenu.close();
					menu = false;           //ferm. programme
					break;
				}
			}

			/*V�rifie clic dans menu
			========================*/
			if (Mouse::isButtonPressed(Mouse::Left)) //clic gauche
			{
				Vector2i posSouris = Mouse::getPosition(windowMenu); //avoir la pos. selon la fen�tre et non l'�cran

																	 //trouver le bouton cliqu� avec pos y boutons
				for (int i = 0; i < 3; i++)
				{
					int posY = boutonsMenu[i].getPosY();

					if (posSouris.y > posY && posSouris.y < (posY + 32))
						indiceBoutonMenu = i;
				}

				//v�rifier pos x du bouton
				if (posSouris.x > boutonsMenu[indiceBoutonMenu].getPosX() && posSouris.x < 248)	//jouer, option ou quitter cliqu�
				{
					boutonsMenu[indiceBoutonMenu].update(); //on renfonce le bouton

					/*R�affiche le menu
					===================*/
					dessinerMenu(windowMenu, boutonsMenu, imageMenu);

					//afficher le bouton renfonc�
					boutonsMenu[indiceBoutonMenu].dessine(windowMenu);
					windowMenu.display();

					/*bouton renfonc� pendant 0.5 sec*/
					clock.restart(); //compteur temps recommence
					tempsEcoule1 = clock.getElapsedTime();

					while (tempsEcoule1 < t1) //pause 0.5 sec
					{
						tempsEcoule1 = clock.getElapsedTime();
					}

					//selon le bouton cliqu�
					switch (indiceBoutonMenu)
					{
					case 0: //bouton jouer
						menu = false;	//quitter menu
						windowMenu.close();	//fermeture fen�tre
						jeu = true;
						break;
					case 1: //bouton options
						options = true;
						break;
					case 2:	//bouton quitter
						menu = false;	//quitter menu
						windowMenu.close();	//fermeture fen�tre
						break;
					}
				}
			}
		}

		while (options) //bouton options cliqu�
		{
			//efface menu
			windowMenu.clear();

			//options additionelles avec poll event pour les touches, boutons, etc.

			//� un moment donn�
			options = false;

			//puisque menu = true, on retourne au menu
		}
	}
}
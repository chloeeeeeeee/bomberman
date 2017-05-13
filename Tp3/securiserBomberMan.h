/*En-t�te de la librairie
=========================
Nom:				saisieBomberMan.h
Auteur:				Chlo� Couture
Date de cr�ation :  25 avril 2017
Description:		Cette librairie regroupe le code n�cessaire � la s�curit� du jeu BomberMan.*/


/*Librairies
=============*/
#pragma once
#include <iostream>
#include <fstream>	//fichiers
#include <cassert>	//pour v�rifier et afficher des messages d'erreur
using namespace std;

//Valide l'ouverture d'un fichier entr�e et la pr�sence de contenu dans un fichier d'entr�e
bool ouvrirFichier(ifstream &fichier, const char *nomFichier)
{
	fichier.open(nomFichier); 	//ouverture du fichier d'entr�e

								//Test de l'ouverture des fichiers et message d'erreur
	if (!fichier.is_open())
	{
		assert("Le fichier n'a pas pu s'ouvrir");
		return false;
	}
	return true;
}

//valide la pr�sence de contenu dans un fichier
bool fichierVide(ifstream &fichier)
{
	setlocale(LC_CTYPE, "fra"); //Pour l'affichage d'accents

								//V�rification d'un fichier vide avec le pointeur seekg
	fichier.seekg(0, ios::end);
	int length = fichier.tellg();

	if (length == 0)
	{
		assert("Le fichier ext vide");
		return true;
	}
	fichier.seekg(0, ios::beg); /*Retour du pointeur au d�but du fichier*/
	return false;
}


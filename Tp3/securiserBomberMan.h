/*En-tête de la librairie
=========================
Nom:				saisieBomberMan.h
Auteur:				Chloé Couture
Date de création :  25 avril 2017
Description:		Cette librairie regroupe le code nécessaire à la sécurité du jeu BomberMan.*/


/*Librairies
=============*/
#pragma once
#include <iostream>
#include <fstream>	//fichiers
#include <cassert>	//pour vérifier et afficher des messages d'erreur
using namespace std;

//Valide l'ouverture d'un fichier entrée et la présence de contenu dans un fichier d'entrée
bool ouvrirFichier(ifstream &fichier, const char *nomFichier)
{
	fichier.open(nomFichier); 	//ouverture du fichier d'entrée

								//Test de l'ouverture des fichiers et message d'erreur
	if (!fichier.is_open())
	{
		assert("Le fichier n'a pas pu s'ouvrir");
		return false;
	}
	return true;
}

//valide la présence de contenu dans un fichier
bool fichierVide(ifstream &fichier)
{
	setlocale(LC_CTYPE, "fra"); //Pour l'affichage d'accents

								//Vérification d'un fichier vide avec le pointeur seekg
	fichier.seekg(0, ios::end);
	int length = fichier.tellg();

	if (length == 0)
	{
		assert("Le fichier ext vide");
		return true;
	}
	fichier.seekg(0, ios::beg); /*Retour du pointeur au début du fichier*/
	return false;
}


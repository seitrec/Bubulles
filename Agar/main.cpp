#include <vector>
#include <math.h>
#include <SFML/Graphics.hpp>
#include "Global.h"
#include "Entity.h"
#include "Cell.h"
#include "Food.h"
#include "Player.h"
#include "main.h"

using namespace std;

int worldSize = 2000; //Le monde est limité à un carré de 2000x2000px
int initialFood = 10; //Nourriture générée avant le début du jeu

int main()
{
	// Création de la fênêtre et de la vue et limitation du framerate
	sf::RenderWindow window(sf::VideoMode(800, 800), "AgarIO C++");
	sf::View view(sf::Vector2f(300, 300), sf::Vector2f(800, 800));
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(30);
	sf::RenderWindow *ptrWindow = &window;


	//Lancement du chrono, l'objet clock gère le temps.
	sf::Clock clock;

	//Génération de la nourriture initiale
	vector<Food> lFood;
	vector<Food> *ptrlFood = &lFood;
	for (int i(0); i < initialFood; ++i)
	{
		lFood.push_back(Food());
	}

	//Initialisation de la liste des joueurs (bots + real players) + Création player(s) initiaux
	vector<Player> lPlayer;
	vector<Player> *ptrlPlayer = &lPlayer;
	lPlayer.push_back(Player());

	//Création d'une cellule pour chaque Player
	//TO DO comprendre les iterator et changer for (vector<Player>::iterator i = lPlayer.begin(); i != lPlayer.end(); ++i)
	for (int i(0); i < lPlayer.size(); ++i)
	{
		lPlayer[i].addCell(Cell());
	}


	// on fait tourner le programme tant que la fen?tre n'a pas ?t? ferm?e
	while (window.isOpen())
	{
		sf::Vector2f mouseCoordonates= window.mapPixelToCoords(sf::Mouse::getPosition(window));

		// on traite tous les evenements de la fenetre qui ont ete genere depuis la derniere iteration de la boucle
		sf::Event event;
		while (window.pollEvent(event))
		{
			// on regarde le type de l'?v?nement...
			switch (event.type)
			{
				// fen?tre ferm?e
			case sf::Event::Closed:
				window.close();
				break;

				//Split, on ne split que si la taille minimum des bulles est > taille nourriture
			case sf::Event::KeyPressed:
				//TO DO refaire le split
				/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					lBubblesize = lBubble.size();
					float min_size = lBubble[0].getSize();

					for (int i(0); i< lBubblesize; ++i) {
						if (lBubble[i].getSize() < min_size) {
							min_size = lBubble[i].getSize();
						}
						if (min_size <= 5) {
							break;
						}
					}

					if (min_size > 5) {
						lBubblesize = lBubble.size();
						for (int i(0); i < lBubblesize; ++i) {
							lBubble.push_back(lBubble[i].Split(mouseX, mouseY));
						}
					}
				}*/
				break;

			default:
				break;
			}
		}

		window.clear(sf::Color::White);
		//TO DO Déplacer ça dans une fonction à part ?
		int entityGenerated = 0;
		if (static_cast<int>(clock.getElapsedTime().asSeconds()) % 3 == 1)
		{
			entityGenerated = 0;
		}
		if (static_cast<int>(clock.getElapsedTime().asSeconds()) % 3 == 0 && entityGenerated == 0)
		{
			lFood.push_back(Food());
			entityGenerated = 1;
		}
		//TO DO passer en iterator ?
		for (int i(0); i < lFood.size(); ++i)
		{
			window.draw(lFood[i]);
		}

		//On définit les target des players (par défaut le 0 suit la souris)
		lPlayer[0].setTarget(mouseCoordonates);
		// Les autres se déplace vers la plus proche nourriture
		for (int i=1; i < lPlayer.size(); ++i)
		{
			//TO DO réimplementer la définition de target des autres (bots)
		}


		//On fait bouger, on check les collisions et on dessine toutes les cellules de tous les joueurs
		for (int i = 0; i < lPlayer.size(); ++i)
		{
			lPlayer[i].move();
			lPlayer[i].checkCollision(ptrlFood);

			lPlayer[i].drawCells(ptrWindow);
		}

		//On dessine la nourriture
		//TO DO iterator ?
		for (int u(0); u < lFood.size(); ++u)
		{
			lFood[u].draw(ptrWindow);
		}
		//On centre sur le joueur 0
		view.setCenter(lPlayer[0].getViewCenter());
		window.display();
 		window.setView(view);
	}

	return 0;
}
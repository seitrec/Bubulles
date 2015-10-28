#include <vector>
#include <math.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Global.h"
#include "Entity.h"
#include "Cell.h"
#include "Food.h"
#include "Player.h"
#include "main.h"

//test. ˆ enlever si vous lisez.
using namespace std;

int worldSize = 2000; 
int windowSize = 750;
int initialFood = 100; //Nourriture gï¿½nï¿½rï¿½e avant le dï¿½but du jeu
int initialPlayers = 50;

int main()
{
	// Crï¿½ation de la fï¿½nï¿½tre et de la vue et limitation du framerate
	sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "AgarIO C++");
	sf::View view(sf::Vector2f(300, 300), sf::Vector2f(windowSize, windowSize));
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(30);
	sf::RenderWindow *ptrWindow = &window;

	sf::Texture texture;
	if (!texture.loadFromFile("bg.png")) {
		cout << "Echec chargement de la texture" << endl;
	}
	texture.setRepeated(true);
	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setScale(sf::Vector2f(0.75, 0.75));
	sprite.setTextureRect(sf::IntRect(00, 00, worldSize/0.75, worldSize / 0.75));
	
	sprite.setColor(sf::Color(255, 255, 255, 15));

	//Lancement du chrono, l'objet clock gï¿½re le temps.
	sf::Clock clock;

	//Gï¿½nï¿½ration de la nourriture initiale
	vector<Food> lFood;
	vector<Food> *ptrlFood = &lFood;
	for (int i(0); i < initialFood; ++i)
	{
		lFood.push_back(Food());
	}

	//Initialisation de la liste des joueurs (bots + real players) + Crï¿½ation player(s) initiaux
	vector<Player> lPlayer;
	vector<Player> *ptrlPlayer = &lPlayer;
	for (int i(0); i < initialPlayers; ++i)
	{
		lPlayer.push_back(Player());
	}

	//Crï¿½ation d'une cellule pour chaque Player
	//TO DO comprendre les iterator et changer for (vector<Player>::iterator i = lPlayer.begin(); i != lPlayer.end(); ++i)
	for (int i(0); i < lPlayer.size(); ++i)
	{
		lPlayer[i].addCell(Cell());
		if (i != 0) { lPlayer[i].getCells()[0].setSize(10); } // pour qu'on est un avantage sur les bots...
	}


	// on fait tourner le programme tant que la fen?tre n'a pas ?t? ferm?e
	while (window.isOpen())
	{
		sf::Vector2f mouseCoordonates = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		//sf::Vector2f mouseCoordonates = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

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
				//Le split grÃ¢ce au keyboard ne s'effectue que sur le player 0
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					lPlayer[0].split();
				}
				break;

			default:
				break;
			}
		}

		window.clear(sf::Color::White);
		window.draw(sprite);
		//TO DO Dï¿½placer ï¿½a dans une fonction ï¿½ part ?
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

		//On dï¿½finit les target des players (par dï¿½faut le 0 suit la souris)
		lPlayer[0].setTarget(mouseCoordonates);
		// Les autres se dï¿½place vers la plus proche nourriture
		for (int i=1; i < lPlayer.size(); ++i)
		{
			lPlayer[i].setCellZone();
			lPlayer[i].setTarget(lPlayer[i].getClosestLocation(lFood));

		}


		//On fait bouger, on check les collisions et on dessine toutes les cellules de tous les joueurs
		for (int i = 0; i < lPlayer.size(); ++i)
		{
			lPlayer[i].move();
			lPlayer[i].setCellZone();
			lPlayer[i].getCellZone();

			
			// On fait toutes les actions relatives ï¿½ manger les Foods
			for (int u = 0; u < lFood.size(); ++u)
			{
				for (int j = 0; j < lPlayer[i].getCells().size(); ++j)
				{
					if (lPlayer[i].getCells()[j].checkCollision(lFood[u]))
					{
						if (lPlayer[i].getCells()[j].getSize()>1.1*lFood[u].getSize())
						{
							lPlayer[i].getCells()[j].eat(lFood[u]);
							lFood.erase(lFood.begin() + u);
							--u;
			}}}}
			for (int u = 0; u < lPlayer.size(); ++u)
			{
				if (u != i)
				{
					for (int j = 0; j < lPlayer[i].getCells().size(); ++j)
					{
						for (int k = 0; k < lPlayer[u].getCells().size(); ++k)
						{
							if (lPlayer[i].getCells()[j].checkCollision(lPlayer[u].getCells()[k]) && lPlayer[i].getCells()[j].getSize()>1.05*lPlayer[u].getCells()[k].getSize())
							{
								lPlayer[i].getCells()[j].eat(lPlayer[u].getCells()[k]);
								lPlayer[u].delCell(k);
								--k;
							}
						}
					}
				}
			}
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
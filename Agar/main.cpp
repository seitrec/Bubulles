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


using namespace std;

std::string namePlayer = "Victor";
int worldSize = 2000; 
int windowSize = 750;
int initialFood = 500; //Nourriture g�n�r�e avant le d�but du jeu
int initialPlayers = 5;
#define PORT = 800;
int main()
{

	// Cr�ation de la f�n�tre et de la vue et limitation du framerate
	sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "AgarIO C++");
	sf::View view(sf::Vector2f(300, 300), sf::Vector2f(windowSize*2, windowSize*2));
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

	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
	{
		cout << "Echec chargement de la police" << endl;
	}

	//Lancement du chrono, l'objet clock g�re le temps.
	sf::Clock clock;

	//G�n�ration de la nourriture initiale
	vector<Food> lFood;
	vector<Food> *ptrlFood = &lFood;
	for (int i(0); i < initialFood; ++i)
	{
		lFood.push_back(Food());
	}

	//Initialisation de la liste des joueurs (bots + real players) + Cr�ation player(s) initiaux
	vector<Player> lPlayer;
	vector<Player> *ptrlPlayer = &lPlayer;
	for (int i(0); i < initialPlayers; ++i)
	{
		lPlayer.push_back(Player());
		if (i!=0){ lPlayer[i].setName("bot " + std::to_string(i)); }
		else { lPlayer[i].setName(namePlayer); }
	}

	//Cr�ation d'une cellule pour chaque Player
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

			case sf::Event::KeyPressed:
				//Le split grâce au keyboard ne s'effectue que sur le player 0
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					lPlayer[0].split();
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
				{
					window.close();
					main();
					
					break;
				}
				break;

			default:
				break;
			}
		}

		window.clear(sf::Color::White);
		window.draw(sprite);
		//TO DO D�placer �a dans une fonction � part ?
		int entityGenerated = 0;
		if (static_cast<int>(clock.getElapsedTime().asSeconds()) % 2 == 1)
		{
			entityGenerated = 0;
		}
		if (static_cast<int>(clock.getElapsedTime().asSeconds()) % 2 == 0 && entityGenerated == 0)
		{
			lFood.push_back(Food());
			lFood.push_back(Food());
			lFood.push_back(Food());
			entityGenerated = 1;
		}
		//TO DO passer en iterator ?
		for (int i(0); i < lFood.size(); ++i)
		{
			window.draw(lFood[i]);
		}

		//On d�finit les target des players (par d�faut le 0 suit la souris)
		lPlayer[0].setTarget(mouseCoordonates);
		// Les autres se d�place vers la plus proche nourriture
		for (int i=1; i < lPlayer.size(); ++i)
		{
			lPlayer[i].setCellZone();
			lPlayer[i].setTarget(lPlayer[i].getClosestLocation(lFood));

		}


		//On fait bouger, on check les collisions et on dessine toutes les cellules de tous les joueurs
		for (int i = 0; i < lPlayer.size(); ++i)
		{
			lPlayer[i].setMoved(false);
			lPlayer[i].move();
			lPlayer[i].setCellZone();
			lPlayer[i].getCellZone();

			
			// On fait toutes les actions relatives � manger les Foods
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
			lPlayer[i].drawCellsScore(window, font);
			lPlayer[i].drawName(window, font);
		}

		//On dessine la nourriture
		//TO DO iterator ?
		for (int u(0); u < lFood.size(); ++u)
		{
			lFood[u].draw(ptrWindow);
		}
		//On centre sur le joueur 0
		view.setCenter(lPlayer[0].getViewCenter());
		float viewSize= 200 * log(fabs(lPlayer[0].getCellZone()[0] - lPlayer[0].getCellZone()[1]));
		view.setSize(viewSize, viewSize);
		window.display();
 		window.setView(view);
	}
	return 0;
}
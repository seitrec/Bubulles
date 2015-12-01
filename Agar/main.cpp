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
string namePlayer = "Player";


int main()
{
	cout << "Quel est votre nom : " << endl;
	cin >> namePlayer;
	// Cr�ation de la f�n�tre et de la vue et limitation du framerate
	sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "AgarIO C++");
	sf::View view(sf::Vector2f(300, 300), sf::Vector2f(WINDOW_SIZE*2, WINDOW_SIZE*2));
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
	sprite.setTextureRect(sf::IntRect(00, 00, WORLD_SIZE/0.75, WORLD_SIZE / 0.75));
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
	for (int i(0); i < INITIAL_FOOD; ++i)
	{
		lFood.push_back(Food());
	}

	//Initialisation de la liste des joueurs (bots + real players) + Cr�ation player(s) initiaux
	vector<Player> lPlayer;
	vector<Player> *ptrlPlayer = &lPlayer;
    lPlayer.push_back(Player(Cell(20),0));
    lPlayer[0].setName(namePlayer);
    //bots ici
	for (int i(1); i < INITIAL_PLAYERS; ++i)
	{
		lPlayer.push_back(Player());
        lPlayer[i].setName("bot " + std::to_string(i));
    }

	//Cr�ation d'une cellule pour chaque Player....->PLUS BESOIN. Géré par le constructeur.
   

	int entityGenerated = 0;
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
					int splitTime = static_cast<int>(clock.getElapsedTime().asSeconds());
					lPlayer[0].split(splitTime);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
				{
					window.close();
					main();
				}
				break;

			default:
				break;
			}
		}

		window.clear(sf::Color::White);
		window.draw(sprite);
		//TO DO D�placer �a dans une fonction � part ?
		if (static_cast<int>(clock.getElapsedTime().asSeconds()) % 2 == 1)
		{
			entityGenerated = 0;
		}
		if (static_cast<int>(clock.getElapsedTime().asSeconds()) % 2 == 0 && entityGenerated == 0 && lFood.size()<MAX_FOOD)
		{
			for (int i = 0; i < FOOD_BY_SECOND;++i)
			{
			lFood.push_back(Food());
			}
			entityGenerated = 1;

		}

		//Les bots split s'ils le souhaitent
		for (int i = 0; i < lPlayer.size(); ++i)
		{
			lPlayer[i].splitIA(lFood, lPlayer, i, static_cast<int>(clock.getElapsedTime().asSeconds()));
		}
		

		//On d�finit les target des players en fonction de leur strategy
		for (int i=0; i < lPlayer.size(); ++i)
		{
			lPlayer[i].setIATarget(mouseCoordonates, lFood, lPlayer, i);
		}


		//On fait bouger, on check les collisions et on dessine toutes les cellules de tous les joueurs
		for (int i = 0; i < lPlayer.size(); ++i)
		{
			lPlayer[i].setMoved(false);
			lPlayer[i].move(static_cast<int>(clock.getElapsedTime().asSeconds()));
			lPlayer[i].setCellZone();
			lPlayer[i].getCellZone();

			
			// On fait toutes les actions relatives � manger les Foods
			for (int u = 0; u < lFood.size(); ++u)
			{
				for (int j = 0; j < lPlayer[i].getCells().size(); ++j)
				{
					if (lPlayer[i].getCells()[j].checkCollision(lFood[u]))
					{
						if (lPlayer[i].getCells()[j].getSize()>EATING_RATIO*lFood[u].getSize())
						{
                            lFood[u].getEaten(lPlayer[i].getCells()[j]);
							lFood.erase(lFood.begin() + u);
							--u;

			}}}}
			if (lPlayer[i].canMerge(static_cast<int>(clock.getElapsedTime().asSeconds())))
			{
				for (int j = 0; j < lPlayer[i].getCells().size(); ++j)
				{
					for (int k = 0; k < lPlayer[i].getCells().size(); ++k)
					{
						if (j != k && lPlayer[i].getCells()[j].checkCollision(lPlayer[i].getCells()[k]) && lPlayer[i].getCells()[j].getSize()>1.001*lPlayer[i].getCells()[k].getSize())
						{
							lPlayer[i].getCells()[k].getEaten(lPlayer[i].getCells()[j]);
							lPlayer[i].delCell(k);
							if (j > k) { --j; }
							--k;
						}
					}
				}
			}
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
                                lPlayer[u].getCells()[k].getEaten(lPlayer[i].getCells()[j]);
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
            //ptrWindow->draw(lFood[u]);
            lFood[u].draw(ptrWindow);
            //différence entre les deux ?
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
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
	// Create the window and the framerate limit
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

	// Start the time reference: the Clock clock 
	sf::Clock clock;

	// Initialise the first batch of food
	vector<Food> lFood;
	vector<Food> *ptrlFood = &lFood;
	for (int i(0); i < INITIAL_FOOD; ++i)
	{
		lFood.push_back(Food());
	}

	// Initialise the list of players (bots + human players)
	vector<Player> lPlayer;

	// Initialise the real player
	vector<Player> *ptrlPlayer = &lPlayer;
    lPlayer.push_back(Player(Cell(20),0));
    lPlayer[0].setName(namePlayer);

    // Initialise the bots
	for (int i(1); i < INITIAL_PLAYERS; ++i)
	{
		lPlayer.push_back(Player());
        lPlayer[i].setName("bot " + std::to_string(i));
    }

	int entityGenerated = 0;
	// The program continues until the window is closed. Each round of the loop processes one frame
	while (window.isOpen())
	{
		sf::Vector2f mouseCoordonates = window.mapPixelToCoords(sf::Mouse::getPosition(window));

		// Process every event generated since last round of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Check the event type
			switch (event.type)
			{
				// Window closed
			case sf::Event::Closed:
				window.close();
				break;

				// Keyboard key pressed, causing an reaction
			case sf::Event::KeyPressed:
				// Spacebar causes a split for player 0 (human)
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					int splitTime = static_cast<int>(clock.getElapsedTime().asSeconds());
					lPlayer[0].split(splitTime);
				}
				// Return causes a reset of the program
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

		// Clean the window in order to draw the next frame
		window.clear(sf::Color::White);
		window.draw(sprite);

		// One second we clean the buffer
		if (static_cast<int>(clock.getElapsedTime().asSeconds()) % 2 == 1)
		{
			entityGenerated = 0;
		}
		// The other we generate FOOD_BY_SECOND new foods and increment the buffer
		if (static_cast<int>(clock.getElapsedTime().asSeconds()) % 2 == 0 && entityGenerated == 0 && lFood.size()<MAX_FOOD)
		{
			for (int i = 0; i < 2*FOOD_BY_SECOND;++i)
			{
			lFood.push_back(Food());
			}
			entityGenerated = 1;

		}

		// Bots choose if they want to split (depending on their strategy)
		for (int i = 0; i < lPlayer.size(); ++i)
		{
			lPlayer[i].splitIA(lFood, lPlayer, i, static_cast<int>(clock.getElapsedTime().asSeconds()));
		}
		

		// All players (bots + human) choose the target of their move for this frame
		for (int i=0; i < lPlayer.size(); ++i)
		{
			lPlayer[i].setIATarget(mouseCoordonates, lFood, lPlayer, i);
		}

		// Players movements loop
		for (int i = 0; i < lPlayer.size(); ++i)
		{
			lPlayer[i].setMoved(false);
			lPlayer[i].move(static_cast<int>(clock.getElapsedTime().asSeconds())); // Each player makes their move
			lPlayer[i].setCellZone();
			lPlayer[i].getCellZone();

			
			// Food that are in covering collision with cells get eaten
			for (int u = 0; u < lFood.size(); ++u)
			{
				for (int j = 0; j < lPlayer[i].getCells().size(); ++j)
				{
					if (lPlayer[i].getCells()[j].checkCollisionCovering(lFood[u]))
					{
						if (lPlayer[i].getCells()[j].getSize()>EATING_RATIO*lFood[u].getSize())
						{
                            lFood[u].getEaten(lPlayer[i].getCells()[j]);
							lFood.erase(lFood.begin() + u);
							--u;

			}}}}

			// If a player is split, can merge, and happens to have cells in covering collision, they merge
			if (lPlayer[i].canMerge(static_cast<int>(clock.getElapsedTime().asSeconds())))
			{
				for (int j = 0; j < lPlayer[i].getCells().size(); ++j)
				{
					for (int k = 0; k < lPlayer[i].getCells().size(); ++k)
					{
						if (j != k && lPlayer[i].getCells()[j].checkCollisionCovering(lPlayer[i].getCells()[k]) && lPlayer[i].getCells()[j].getSize()>1.001*lPlayer[i].getCells()[k].getSize())
						{
							lPlayer[i].getCells()[k].getEaten(lPlayer[i].getCells()[j]);
							lPlayer[i].delCell(k);
							if (j > k) { --j; }
							--k;
						}
					}
				}
			}

			// If different players are in covering collision, the biggest one eats the other
			for (int u = 0; u < lPlayer.size(); ++u)
			{
				if (u != i)
				{
					for (int j = 0; j < lPlayer[i].getCells().size(); ++j)
					{
						for (int k = 0; k < lPlayer[u].getCells().size(); ++k)
						{
							if (lPlayer[i].getCells()[j].checkCollisionCovering(lPlayer[u].getCells()[k]) && lPlayer[i].getCells()[j].getSize()>1.05*lPlayer[u].getCells()[k].getSize())
							{
                                lPlayer[u].getCells()[k].getEaten(lPlayer[i].getCells()[j]);
                                lPlayer[u].delCell(k);
								--k;
							}
						}
					}
				}

			}
			// Draw Cells with their new positions
			lPlayer[i].drawCells(ptrWindow);
			lPlayer[i].drawCellsScore(window, font);
			lPlayer[i].drawName(window, font);
		}

		// Draw all foods remaining
		for (int u(0); u < lFood.size(); ++u)
		{
            lFood[u].draw(ptrWindow);
		}
		// Center the view on player 0 (the only human)
		view.setCenter(lPlayer[0].getViewCenter());
		// Adjust the zoom on the human player's cell zone
		float viewSize= 200 * log(fabs(lPlayer[0].getCellZone()[0] - lPlayer[0].getCellZone()[1]));
		view.setSize(viewSize, viewSize);
		window.display();
 		window.setView(view);
	}
	return 0;
}
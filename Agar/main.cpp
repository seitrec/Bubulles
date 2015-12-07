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
	for (int i(0); i < INITIAL_FOOD; ++i)
	{
		lFood.push_back(Food());
	}

	// Initialise the list of players (bots + human players)
	vector<Player*> lPlayer;

	// Initialise the real player
    lPlayer.push_back(new Player(0));

    // Initialise the bots
	for (int i(1); i < INITIAL_PLAYERS; ++i)
	{
		lPlayer.push_back(new Player(1));
        lPlayer[i]->setName("bot " + std::to_string(i));
    }

	int entityGenerated = 0;
	// The program continues until the window is closed. Each round of the loop processes one frame

	// Define de intro menu and naming of the player

	int menuId = 0;

	sf::Texture menuTexture;
	if (!menuTexture.loadFromFile("bulles.png")) {
		cout << "Echec chargement de la texture" << endl;
	}

	sf::Sprite menuSprite;
	menuSprite.setTexture(menuTexture);
	menuSprite.setTextureRect(sf::IntRect(00, 00, WINDOW_SIZE, WINDOW_SIZE));
	menuSprite.setColor(sf::Color(255, 255, 255, 150));

	sf::Text intro("Welcome in AgarioC++ !", font);
	intro.setColor(sf::Color::White);
	sf::FloatRect introRect = intro.getLocalBounds();
	intro.setStyle(sf::Text::Bold);
	intro.setOrigin(introRect.left + introRect.width/2.0f, introRect.top + introRect.height/2.0f);
	intro.setPosition(sf::Vector2f(375,225));

	sf::Text question("Enter your name on the screen :", font);
	question.setColor(sf::Color::White);
	sf::FloatRect questionRect = question.getLocalBounds();
	question.setStyle(sf::Text::Bold);
	question.setOrigin(questionRect.left + questionRect.width/2.0f, questionRect.top + questionRect.height/2.0f);
	question.setPosition(sf::Vector2f(375,300));

	sf::Text pseudo(" ", font);
	pseudo.setColor(sf::Color::White);
	sf::FloatRect pseudoRect = pseudo.getLocalBounds();
	pseudo.setStyle(sf::Text::Bold);
	pseudo.setOrigin(pseudoRect.left + pseudoRect.width/2.0f, pseudoRect.top + pseudoRect.height/2.0f);
	pseudo.setPosition(sf::Vector2f(325,375));

	sf::Text start("Press Enter to play !", font);
	start.setColor(sf::Color::White);
	sf::FloatRect startRect = start.getLocalBounds();
	start.setStyle(sf::Text::Bold);
	start.setOrigin(startRect.left + startRect.width/2.0f, startRect.top + startRect.height/2.0f);
	start.setPosition(sf::Vector2f(375,450));

	sf::Text end("		Game Over ! \n"
				 "Press Enter to try again", font);
	end.setColor(sf::Color::Black);
	sf::FloatRect endRect = start.getLocalBounds();
	end.setStyle(sf::Text::Bold);
	end.setCharacterSize(100);
	end.setOrigin(endRect.left + endRect.width/2.0f, endRect.top + endRect.height/2.0f);
	end.setPosition(sf::Vector2f(650,900));

	sf::Text highscore("", font);
	highscore.setColor(sf::Color::Black);
	sf::FloatRect highscoreRect = start.getLocalBounds();
	highscore.setStyle(sf::Text::Bold);
	highscore.setCharacterSize(40);
	highscore.setOrigin(highscoreRect.left + highscoreRect.width/2.0f, highscoreRect.top + highscoreRect.height/2.0f);
	highscore.setPosition(sf::Vector2f(650,900));

	string pseudoStr;

	int maxScore = 0;
	int bestPlayerId = 0;

	while (window.isOpen())
	{
		while (menuId==0)
		{
			window.clear(sf::Color::White);
			window.draw(menuSprite);

			sf::Event event;

			while (window.pollEvent(event)) {

				switch (event.type) {
					//Window closed
					case sf::Event::Closed:
						window.close();
						break;

					//Start playing
					case sf::Event::KeyPressed:
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
							menuId = 1;
							lPlayer[0]->setName(pseudoStr);
						}
						break;
					//Writing name
					case sf::Event::TextEntered:
						// Handle ASCII characters only
						if (event.text.unicode == '\b'){
							pseudoStr.pop_back();
						}
						else if (event.text.unicode < 128) {
							pseudoStr += static_cast<char>(event.text.unicode);
						}
						pseudo.setString(pseudoStr);
						break;

					default:
						break;
				}
			}

			window.draw(pseudo);
			window.draw(intro);
			window.draw(start);
			window.draw(question);

			window.display();
		}

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
					lPlayer[0]->split(splitTime);
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
			lPlayer[i]->splitIA(lPlayer, i, static_cast<int>(clock.getElapsedTime().asSeconds()));
		}
		

		// All players (bots + human) choose the target of their move for this frame
		for (int i=0; i < lPlayer.size(); ++i)
		{
			lPlayer[i]->setIATarget(mouseCoordonates, lFood, lPlayer, i);
		}

		// Players movements loop
		for (int i = 0; i < lPlayer.size(); ++i)
		{
			lPlayer[i]->setCellZone();
            lPlayer[i]->move(static_cast<int>(clock.getElapsedTime().asSeconds())); // Each player makes their move
			
			lPlayer[i]->getCellZone();

			
			// Food that are in covering collision with cells get eaten
			for (int u = 0; u < lFood.size(); ++u)
			{
				for (int j = 0; j < lPlayer[i]->getCells().size(); ++j)
				{
					if (lPlayer[i]->getCells()[j]->checkCollisionCovering(lFood[u]))
					{
						if (lPlayer[i]->getCells()[j]->getSize()>EATING_RATIO*lFood[u].getSize())
						{
                            lPlayer[i]->getCells()[j]->Eat(lFood[u], static_cast<int>(clock.getElapsedTime().asSeconds()));
							lFood.erase(lFood.begin() + u);
							--u;

						}
					}
				}
			}

			// If a player is split, can merge, and happens to have cells in covering collision, they merge
			if (lPlayer[i]->canMerge(static_cast<int>(clock.getElapsedTime().asSeconds())))
			{
				for (int j = 0; j < lPlayer[i]->getCells().size(); ++j)
				{
					for (int k = 0; k < lPlayer[i]->getCells().size(); ++k)
					{
						if (j != k && lPlayer[i]->getCells()[j]->checkCollisionCovering(*lPlayer[i]->getCells()[k]) && lPlayer[i]->getCells()[j]->getSize()>1.001*lPlayer[i]->getCells()[k]->getSize())
						{
							lPlayer[i]->getCells()[j]->Eat(*lPlayer[i]->getCells()[k]);;
							lPlayer[i]->delCell(k);
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
					for (int j = 0; j < lPlayer[i]->getCells().size(); ++j)
					{
						for (int k = 0; k < lPlayer[u]->getCells().size(); ++k)
						{
							if (lPlayer[i]->getCells()[j]->checkCollisionCovering(*lPlayer[u]->getCells()[k]) && lPlayer[i]->getCells()[j]->getSize()>1.05*lPlayer[u]->getCells()[k]->getSize())
							{
                                lPlayer[i]->getCells()[j]->Eat(*lPlayer[u]->getCells()[k]);
                                lPlayer[u]->delCell(k);
								--k;
							}
						}
					}
				}

			}
			// Draw Cells with their new positions
			lPlayer[i]->drawCells(ptrWindow);
			lPlayer[i]->drawCellsScore(window, font);
			lPlayer[i]->drawName(window, font);
			// Update the scores
			lPlayer[i]->setScore();
		}

		// Draw all foods remaining
		for (int u(0); u < lFood.size(); ++u)
		{
            lFood[u].draw(ptrWindow);
		}
		// Center the view on player 0 (the only human)
		view.setCenter(lPlayer[0]->getViewCenter());
		// Adjust the zoom on the human player's cell zone
		float viewSize= 200 * log(fabs(lPlayer[0]->getCellZone()[0] - lPlayer[0]->getCellZone()[1]));
		view.setSize(viewSize, viewSize);


		// Draw the highest score
		maxScore = 0;

		for (int u = 0; u < lPlayer.size(); ++u)
		{
			int scorePlayer = lPlayer[u]->getScore();
			if (maxScore < scorePlayer)
			{
				maxScore = scorePlayer;
				bestPlayerId = u;
			}

		}

		string highscoreString = "1st :" + lPlayer[bestPlayerId]->getName() + "\n(score : " + to_string(maxScore) + ")";
		highscore.setString(highscoreString);
		highscore.setPosition(window.mapPixelToCoords(sf::Vector2i(highscoreRect.left + highscoreRect.width / 2.0f, highscoreRect.top + highscoreRect.height / 2.0f)));
		window.draw(highscore);

		// End of the game
		if (lPlayer[0]->getCells().empty())
		{
			window.draw(end);
		}

		window.display();
 		window.setView(view);

	}
	return 0;
}
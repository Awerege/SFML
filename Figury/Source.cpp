#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Audio.hpp>
#include <random>
#include <ctime>

using namespace std;
using namespace sf;

int main(void)
{
	srand(time(NULL));
	ContextSettings settings;
	settings.antialiasingLevel = 16;
	RenderWindow w(VideoMode(1200, 400), "Gra", Style::Close, settings);
	w.setFramerateLimit(60);

	Font font;
	font.loadFromFile("GROBOLD.ttf");

	Shape *figury[10];
	Text *numery[10];

	for (int i = 0; i < 10; i++)
	{
		if (i % 2 == 0)
			figury[i] = (Shape*)(new CircleShape((i % 3 == 0) ? 25 : 30));
		else if (i % 2 == 1)
			figury[i] = (Shape*)(new RectangleShape((i % 3 == 0) ? Vector2f(30, 80) : Vector2f(70, 40)));
		figury[i]->setPosition(Vector2f(100 + rand() % (w.getSize().x - 200), 100 + rand() % (w.getSize().y - 200)));
		figury[i]->setFillColor(Color((i % 3 == 0) ? 255 : 0, (i % 3 == 1) ? 255 : 0, (i % 3 == 2) ? 255 : 0, 255));

		numery[i] = new Text;
		numery[i]->setFont(font);
		numery[i]->setColor(Color(0, 0, 0, 255));
		numery[i]->setString(to_string(i));
		numery[i]->setPosition(figury[i]->getPosition() + Vector2f(figury[i]->getGlobalBounds().width / 2, (figury[i]->getGlobalBounds().height / 2) - 10));
		numery[i]->setOrigin(numery[i]->getGlobalBounds().width / 2, numery[i]->getGlobalBounds().height / 2);

		for (int j = 0; j < i; j++)
		{
			if (figury[j]->getGlobalBounds().intersects(figury[i]->getGlobalBounds()))
			{
				delete figury[i];
				i -= 1;
				break;
			}
		}
	}
	
	int ostatnia_figura = -1;
	int przegrana = 0;
	
	CircleShape celownik(10, 3);
	celownik.setFillColor(Color(220, 20, 20, 60));
	celownik.setPosition(w.getSize().x / 2, w.getSize().y / 2);

	float pozostaly_czas = 60;
	int pozostale_figury = 10;

	Clock zegar;
	zegar.restart();

	Text tekst_zegara;
	tekst_zegara.setFont(font);
	tekst_zegara.setColor(Color(255, 0, 0, 255));

	Text komunikat;
	komunikat.setFont(font);
	komunikat.setColor(Color(255, 0, 0, 255));
	komunikat.setCharacterSize(60);
	komunikat.setPosition(w.getSize().x / 2, w.getSize().y / 2);

	

	SoundBuffer efekt_plik;
	efekt_plik.loadFromFile("POP.wav");

	Sound efekt;
	efekt.setBuffer(efekt_plik);
	efekt.setVolume(15);

	while (w.isOpen())
	{
		Event eve;
		while (w.pollEvent(eve))
		{
			switch (eve.type)
			{
			case Event::Closed:
				w.close();
				break;

			default:
				break;
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Key::Up))
		{
			celownik.move(Vector2f(0, -5));
		}
		if (Keyboard::isKeyPressed(Keyboard::Key::Down))
		{
			celownik.move(Vector2f(0, 5));
		}
		if (Keyboard::isKeyPressed(Keyboard::Key::Left))
		{
			celownik.move(Vector2f(-5, 0));
		}
		if (Keyboard::isKeyPressed(Keyboard::Key::Right))
		{
			celownik.move(Vector2f(5, 0));
		}

		if (Keyboard::isKeyPressed(Keyboard::Key::Space) && przegrana == 0)
		{
			for (int i = 0; i < 10; i++)
			{
				if (figury[i] != NULL)
				{
					if (figury[i]->getGlobalBounds().contains(celownik.getPosition()))
					{
						if (ostatnia_figura + 1 != i)
							przegrana = 1;
						else
							ostatnia_figura = i;
						delete figury[i];
						figury[i] = NULL;
						delete numery[i];
						numery[i] = NULL;
						pozostale_figury -= 1;
						efekt.play();
					}
				}
			}
		}

		if (pozostaly_czas > 0 && pozostale_figury > 0 && przegrana == 0)
		{
			pozostaly_czas -= zegar.restart().asSeconds();
		}

		if (pozostaly_czas <= 0)
		{
			przegrana = 1;
		}

		string tz = to_string((int)pozostaly_czas);

		tekst_zegara.setString(String(tz));

		w.clear(Color(255, 255, 255, 255));

		for (int i = 0; i < 10; i++)
		{
			if (figury[i] != NULL)
				w.draw(*figury[i]);
			if (numery[i] != NULL)
				w.draw(*numery[i]);
		}

		w.draw(celownik);

		w.draw(tekst_zegara);

		if (pozostale_figury == 0 && przegrana == 0)
		{
			komunikat.setString("WYGRANA");
			komunikat.setOrigin(komunikat.getGlobalBounds().width / 2, komunikat.getGlobalBounds().height / 2);
		}

		if (przegrana == 1)
		{
			komunikat.setString("PRZEGRANA");
			komunikat.setOrigin(komunikat.getGlobalBounds().width / 2, komunikat.getGlobalBounds().height / 2);
		}

		w.draw(komunikat);

		w.display();
	}
}
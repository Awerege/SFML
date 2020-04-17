#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <time.h>
#include "Source1.h"

using namespace sf;


class Balon : public CircleShape
{
public:
	float x, y, vx, vy;										//pozycja i predkosc balonu
	int licznik_klatek, licznik2;										//indywidualny licznik klatek
	int LicznikPierwotny;
	int LicznikWtórny;
	bool Zderzenie;												//pusty konstruktor (gdy chcemy zrobic tablice czy wektor balonow i dopiero potem uzupelnic ich parametry
	Balon()
	{}
																			//konstruktor z paramterami - czasem jest tak wygodniej
	Balon(float r, float sx, float sy, Color c)
	{
		this->setRadius(r);
		setScale(sx, sy);
		setFillColor(c);
		Zderzenie = false;

	}
																									//metoda ustawiajaca parametry balonu, takze jego pozycje
	void setParams(float r, float sx, float sy, Color c, float x, float y, float vx, float vy)
	{
		this->setRadius(r);
		setScale(sx, sy);
		setFillColor(c);
		this->x = x;
		this->y = y;
		this->vx = vx;
		this->vy = vy;
		setPosition(x, y);
	}

																				//ta metoda powinna aktualizowac paramtery balonika, czyli pozycje i predkosc, ktora to predkosc zmienia sie 
																				//w zaleznosci od tego ile klatek minelo
																				//kazdy balonik powinien wiec miec wlasny licznik klatek oraz wlasna zmienna licznik2, najlepiej kazdy inna - wylosowana
																				//wowczas kazdy balonik bedzie mogl poruszac sie niezaleznie i w nieco inny sposob
	void Update()
	{
		if (LicznikPierwotny == LicznikWtórny)
		{
			vx = -vx;
			LicznikPierwotny = 0;
			LicznikWtórny = rand() % 40 + 10;
		}
		LicznikPierwotny++;
		
		x += vx;
		vy = rand() % 3+1;
		y = y - vy;
		setPosition(x, y);
	}

};


int main()
{
	srand(time(NULL));																										 //inicjacja generatora pseudolosowego
	sf::RenderWindow window(sf::VideoMode(1600, 800), "Baloniki");
	window.setFramerateLimit(20);																					//ograniczenie wyswietlania do 20 klatek na sekunde

																															//balonik za pomoca standardowego kolka z biblioteki sfml
																																/*sf::CircleShape balon1(30.f, 30);
																																balon1.setFillColor(sf::Color::Green);
																																balon1.scale(1, 1.5);
																																balon1.setPosition(100, 400);

																																sf::CircleShape balon2(20.f, 30);
																																balon2.setFillColor(sf::Color::Blue);
																																balon2.scale(1, 1.5);
																																balon2.setPosition(100, 100);

																																float x = 100, y = 400, vx = 2, vy = -3;
																																int licznik_klatek = 0;
																																int licznik2 = rand() % 40 + 10;
																																std::cout << licznik2 << std::endl;*/

	SoundBuffer POPw;	
	POPw.loadFromFile("POP.wav");

	Sound POP;
	POP.setBuffer(POPw);

																					//dopiero tu ustawienie wszystkiego

	std::vector<Balon> baloniki(200);																																//wektor pustych balonow
	for (int i = 0; i<baloniki.size(); i++)
		baloniki[i].setParams(rand() % 15 + 5, 2, 3, Color(rand() % 255, rand() % 255, rand() % 255, rand() % 255), rand() % 1600, rand() %800, 1, 1); //i ich inicjacja; polozenie jest losowe

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
																																								/*licznik_klatek++; 
																																								if (licznik_klatek == licznik2)						//jak licznik klatek osiagnie wartosc licznik2, nastepuje zmiana predkosci vx
																																																					//jest to predkosc jednego z balonow
																																								{
																																									vx = -vx;
																																									licznik_klatek = 0;
																																									licznik2 = rand() % 40 + 10;						 // nie wracamy z licznikiem do stalej wartosci, ale ja losujemy zeby balonik zachowywal sie mniej regularnie
																																									std::cout << licznik2 << std::endl;			//to w zasadzie zbedne - sluzylo nam do podgladu czy 
																																								}
																																								x += vx;
																																								y = y + vy;
																																								balon1.setPosition(x, y);*/		

		for (int r = 0;r<baloniki.size();r++)																												 //sprawdzanie zderzenia balonow, jesli wystapi zderzenie dzwiek
		{
			for (int t = 0;t<baloniki.size();t++)
			{
				if (r != t)
				{
					if (baloniki[r].getGlobalBounds().intersects(baloniki[t].getGlobalBounds()))
					{
						baloniki[r].Zderzenie = true;
						baloniki[t].Zderzenie = true;
						POP.play();

					}
				}
			}
		}


		for (int r = 0;r<baloniki.size();r++)																																	 //usuwanie zderzonych balonów
		{
			std::vector<Balon>::iterator it = baloniki.begin() += r;
			if (baloniki[r].Zderzenie == true)
			{
				baloniki.erase(it);
				r--;
			}
		}











		
		//prosze zerknac do konstruktora i zastanowic sie co sie stanie po odkomentowaniu
		for (int i = 0; i < baloniki.size(); i++)
			baloniki[i].Update();

		window.clear();
																																		//sprawdzamy czy balony sie stykaja - jesli tak, to ich nie rysujemy
																																		/*if (!balon1.getGlobalBounds().intersects(balon2.getGlobalBounds()))
																																		{
																																			window.draw(balon1);
																																			window.draw(balon2);
																																		}
																																		window.draw(balon3);*/	
																																		//petla rysujaca wszystkie baloniki z wektora
		for (int i = 0; i<baloniki.size(); i++)
			window.draw(baloniki[i]);
		window.display();
	}

	return 0;
}
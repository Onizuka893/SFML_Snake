#include <SFML/Graphics.hpp>
#include <array>
#include <chrono>
#include "Headers/Global.hpp"
#include "Headers/Snake.hpp"
#include "Headers/RandomGenerator.hpp"
#include "Headers/UpdateGame.hpp"
#include "Headers/DrawText.hpp"

int main()
{
	bool pause = false;

	unsigned char update_timer = gbl::GAME::FRAMES_PER_UPDATE - 1;

	unsigned short lenght = 0;
	
	sf::RectangleShape cell_shape(sf::Vector2f(gbl::MAP::CELL_SIZE - gbl::MAP::CELL_OUTLINE_THICKNESS, gbl::MAP::CELL_SIZE - gbl::MAP::CELL_OUTLINE_THICKNESS));
	sf::RenderWindow window(sf::VideoMode(gbl::SCREEN::RESIZE * gbl::SCREEN::WIDTH, gbl::SCREEN::RESIZE * gbl::SCREEN::HEIGHT), "SFML_Snake", sf::Style::Close);
	window.setView(sf::View(sf::FloatRect(0, 0, gbl::SCREEN::WIDTH, gbl::SCREEN::HEIGHT)));

	sf::Texture font_texture;
	font_texture.loadFromFile("Resources/Images/Font.png");

	std::chrono::microseconds lag(0);

	std::chrono::steady_clock::time_point previous_time;

	sf::Event event;
	RandomGenerator random_generator;

	gbl::Map map;

	Snake snake = Snake(map, random_generator);

	add_food(map, random_generator);

	previous_time = std::chrono::steady_clock::now();

	while (window.isOpen())
	{
		std::chrono::microseconds delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time);

		lag += delta_time;

		previous_time += delta_time;

		while (gbl::SCREEN::FRAME_DURATION <= lag)
		{
			lag -= gbl::SCREEN::FRAME_DURATION;

			while (1 == window.pollEvent(event))
			{
				switch (event.type)
				{
				case sf::Event::Closed:
				{
					window.close();

					break;
				}
				case sf::Event::KeyPressed:
				{
					switch (event.key.code)
					{
					case sf::Keyboard::Space:
					{
						pause = 1 - pause;

						break;
					}
					}
				}
				}
			}

			if (0 == pause)
			{
				if (0 == update_timer)
				{
					update_timer = gbl::GAME::FRAMES_PER_UPDATE - 1;
					update_game(snake, map, random_generator);
				}
				else
				{
					update_timer--;
				}
			}

			//update_game(snake, map, random_generator);

			window.clear();

			for (unsigned char a = 0; a < gbl::MAP::COLUMNS; a++)
			{
				for (unsigned char b = 0; b < gbl::MAP::ROWS; b++)
				{
					if (0 < map[a][b].total_snake_bodies)
					{
						cell_shape.setFillColor(sf::Color::Green);
					}
					else if (1 == map[a][b].food)
					{
						cell_shape.setFillColor(sf::Color::Yellow);
					}
					else
					{
						cell_shape.setFillColor(sf::Color(162, 162, 162));
					}

					cell_shape.setPosition(gbl::MAP::ORIGIN_X + gbl::MAP::CELL_SIZE * a + floor(0.5f * gbl::MAP::CELL_OUTLINE_THICKNESS), gbl::MAP::ORIGIN_Y + gbl::MAP::CELL_SIZE * b + floor(0.5f * gbl::MAP::CELL_OUTLINE_THICKNESS));

					window.draw(cell_shape);
				}
			}
			lenght = snake.get_length();
			draw_text(1, 0, 136, 16, "length\n" + std::to_string(lenght), window, font_texture);

			window.display();
		}
	}

	return 0;
}
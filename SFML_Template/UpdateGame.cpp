#include <array>
#include <chrono>
#include <random>

#include "Headers/Global.hpp"
#include "Headers/Snake.hpp"
#include "Headers/RandomGenerator.hpp"
#include "Headers/MapFunctions.hpp"

void add_food(gbl::Map& i_map, RandomGenerator& i_random_generator)
{
	for (unsigned short a = 0; a < 1; a++)
	{
		gbl::Position<> food_position = get_random_cell(i_map, i_random_generator);

		//Because get_random_cell() returns Position(-1, -1) if it can't find an emtpy cell.
		//Hey, I misspelled the word "empty". Did you notice it?
		//I won't fix it though.
		//Enjoy!
		if (-1 != food_position.x)
		{
			i_map[food_position.x][food_position.y].food = 1;
		}
		else
		{
			return;
		}
	}
}

void update_game(Snake& i_snake, gbl::Map& i_map, RandomGenerator& i_random_generator)
{

	i_snake.update_direction();
	i_snake.move();

	//Here we're clearing the map.
	for (std::array<gbl::Cell, gbl::MAP::ROWS>& column : i_map)
	{
		for (gbl::Cell& cell : column)
		{
			cell.id = gbl::GAME::TOTAL_SNAKES;
			cell.total_snake_bodies = 0;
		}
	}

	i_snake.update_map();
	i_snake.update_status();

	for (std::array<gbl::Cell, gbl::MAP::ROWS>& column : i_map)
	{
		for (gbl::Cell& cell : column)
		{
			if (0 < cell.total_snake_bodies && 1 == cell.food)
			{
				//Here we're removing food if any snake has eaten it, and putting new food on the map.
				cell.food = 0;
				short s = 10;
				add_food(i_map, i_random_generator);
			}
		}
	}
}
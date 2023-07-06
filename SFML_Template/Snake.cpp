#include "Headers/Snake.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "Headers/MapFunctions.hpp"
#include <array>
#include "Headers/Global.hpp"
#include <algorithm>
#include <iostream>


Snake::Snake(gbl::Map& i_map, RandomGenerator& i_random_generator) :
	map_pointer(&i_map),
	random_generator_pointer(&i_random_generator)
{
	reset();
}

Snake::~Snake()
{
}

bool Snake::is_dead() const
{
	return dead;
}

float Snake::get_fitness() const
{
	return fitness;
}

unsigned short Snake::get_length() const
{
	return body.size();
}

void Snake::reset()
{
	dead = 0;

	fitness = 0;

	//-1 because... because... damn, I don't remember why we're subtracting 1.
	//But I'm pretty sure it was important so you better not touch it.
	food_timer = gbl::SNAKE::FOOD_TIMER_DURATION - 1;
	grow_length = 0;

	body.clear();
	body.push_back(get_random_cell(*map_pointer, *random_generator_pointer));

	direction = gbl::SNAKE::Direction::Right;
}

void Snake::grow()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		body.push_back(body[body.size()-1]);
	}
}

void Snake::move()
{
	if (0 < grow_length)
	{
		grow_length--;

		body.push_back(body.back());
	}

	if (0 == dead)
	{
		for (unsigned short a = body.size() - 1; 0 < a; a--)
		{
			body[a].x = body[a - 1].x;
			body[a].y = body[a - 1].y;
		}

		switch (direction)
		{
		case gbl::SNAKE::Direction::Down:
		{
			if(body[0].y < gbl::MAP::ROWS - 1)
				body[0].y++;

			break;
		}
		case gbl::SNAKE::Direction::Left:
		{
			if (body[0].x > 0)
				body[0].x--;

			break;
		}
		case gbl::SNAKE::Direction::Right:
		{
			if (body[0].x < gbl::MAP::COLUMNS - 1)
				body[0].x++;

			break;
		}
		case gbl::SNAKE::Direction::Up:
		{
			if (body[0].y > 0)
				body[0].y--;
			break;
		}
		}

	}
}

void Snake::update_direction()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		if(direction != gbl::SNAKE::Direction::Left)
			direction = gbl::SNAKE::Direction::Right;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		if (direction != gbl::SNAKE::Direction::Right)
			direction = gbl::SNAKE::Direction::Left;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		if (direction != gbl::SNAKE::Direction::Down)
			direction = gbl::SNAKE::Direction::Up;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		if (direction != gbl::SNAKE::Direction::Up)
			direction = gbl::SNAKE::Direction::Down;
	}
}

void Snake::update_map() const
{
	for (const gbl::Position<>& body_part : body)
	{
		if (1 == inside_map(body_part))
		{	
			if (0 == dead)
			{
				//If there are more than 1 snake body in 1 cell, we're gonna draw the snake with the lowest id.
				//There's no logical reason why we're doing this.
				//I'm just a crazy idiot.
				map_pointer->at(body_part.x).at(body_part.y).id = map_pointer->at(body_part.x).at(body_part.y).id;
				map_pointer->at(body_part.x).at(body_part.y).total_snake_bodies++;
			}
			else
			{
				//Uncomment this if you want snakes to eat each other.
				//map_pointer->at(body_part.x).at(body_part.y).food = 1;
			}
		}
	}
}

void Snake::update_status()
{
	if (0 == dead)
	{
		if (0 == inside_map(body[0]))
		{
			dead = 1;
		}
		else if (1 < map_pointer->at(body[0].x).at(body[0].y).total_snake_bodies)
		{
			//This means that snake's head touched its own body or some other snake's body.
			dead = 1;
		}
		else if (1 == map_pointer->at(body[0].x).at(body[0].y).food)
		{
			fitness++;

			food_timer = gbl::SNAKE::FOOD_TIMER_DURATION;
			grow_length++;
		}
		else
		{
			//I noticed that some snakes get stuck in an infinite loop. So to prevent them from moving forever, I made it so that they die when they don't eat food for a long time.
			food_timer--;

			if (0 == food_timer)
			{
				dead = 1;
			}
		}
	}
}


int Snake::get_x_pos() const
{
	return body[0].x;
}

int Snake::get_y_pos() const
{
	return body[0].y;
}

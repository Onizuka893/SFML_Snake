#pragma once
#include <vector>
#include "Global.hpp"
#include "RandomGenerator.hpp"

class Snake
{
public:
	Snake(gbl::Map& i_map, RandomGenerator& i_random_generator);
	~Snake();

	bool is_dead() const;
	float get_fitness() const;
	unsigned short get_length() const;
	int get_x_pos() const;
	int get_y_pos() const;

	void move();
	void update_map() const;
	void update_status();
	void update_direction();
	void grow();
	void reset();

private:
	RandomGenerator* random_generator_pointer;

	unsigned short food_timer;
	unsigned short grow_length;

	gbl::Map* map_pointer;

	gbl::SNAKE::Direction direction;

	bool dead;
	float fitness;
	std::vector<gbl::Position<>> body;
};
#pragma once
#include <vector>
#include "Global.hpp"
#include "Snake.hpp"
#include "RandomGenerator.hpp"

void update_game(Snake& i_snake, gbl::Map& i_map, RandomGenerator& i_random_generator);

void add_food(gbl::Map& i_map, RandomGenerator& i_random_generator);

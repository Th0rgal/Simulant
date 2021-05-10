#pragma once

#include "stdlib.h"

/**
 * Return a random int between two boundaries (included)
 * 
 * @param int lower boundary
 * @param int higher boundary
 * 
 * @return int random output
 * 
 * @confidence 5
 * @author Thomas Marchand
 **/
int random_index(int lower, int higher);

/**
 * Return a random double between two boundaries (included)
 * 
 * @param double lower boundary
 * @param double higher boundary
 * 
 * @return double random output
 * 
 * @confidence 5
 * @author Thomas Marchand
 **/
double random_double(double lower, double higher);

/**
 * Return a random boolean (50% true, 50% false)
 * 
 * @return bool random output
 * 
 * @confidence 5
 * @author Thomas Marchand
 **/
bool flip_a_coin();
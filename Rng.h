#pragma once
#include <algorithm>
#include <chrono>
#include <string>
#include <random>

class Rng	//Distribution is uniform by default
{
public:
	int CreateInt(int min, int max, bool NormalDistr = false);			//Other Create functions will call this one
	float CreateFloat(float min, float max, bool NormalDistr = false);	//Use range as two decimal or less
	unsigned char CreateChar(bool NormalDistr = false);					//Based on custom Ascci table
	std::string CreateString(int lenght, bool NormalDistr = false);
	void ReSeed();
private:
	unsigned int seed = (unsigned int)std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::mt19937 rg{ seed };
};
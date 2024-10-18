#include "Rng.h"

int Rng::CreateInt(int min, int max, bool NormalDistr)
{
	int range = max - min + 1;
	int maxrange = range;
	uint64_t out = rg();
	if (range == 0)
	{
		out = 0;
	}
	else
	{
		if (NormalDistr)
		{
			range *= 2;
		}
		out %= range;
		
		if (NormalDistr && out > static_cast<unsigned long long>(maxrange) + 1)
		{
			out /= 3;
		}
		out += min;
	}
	return (int)out;
}

float Rng::CreateFloat(float min, float max, bool NormalDistr)  //1.5 - 3.5
{
	float range = (max - min) * 100;
	int decimal = CreateInt(0, (int)range, NormalDistr);
	float out = ((float)decimal / 100) + min;
	return out;
}

unsigned char Rng::CreateChar(bool NormalDistr)
{
	int random = CreateInt(33, 129, NormalDistr);
	if (random == 126)
	{
		random = 132;
	}
	else if (random == 127)
	{
		random = 142;
	}
	else if (random == 128)
	{
		random = 149;
	}
	else if (random == 129)
	{
		random = 153;
	}
	unsigned char out = random;
	return out;
}

std::string Rng::CreateString(int lenght, bool NormalDistr)
{
	std::string out;
	for (int i = 0; i < lenght; i++)
	{
		char c = CreateChar(NormalDistr);
		out.push_back(c);
	}
	return out;
}

void Rng::ReSeed()
{
	seed = (unsigned int)std::chrono::high_resolution_clock::now().time_since_epoch().count();
	rg.seed(seed);
}
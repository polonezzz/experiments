#include "algo.h"

int waterFill(const std::array<int, 9>& land) 
{
	int leftMax = 0;
	int rightMax = 0;
	int left = 0;
	int right = land.size() - 1;
	int volume = 0;

	while(left < right) 
	{
		if(land[left] > leftMax) 
		{
			leftMax = land[left];
		}
		
		if(land[right] > rightMax) 
		{
			rightMax = land[right];
		}
		
		if(leftMax >= rightMax) 
		{
			volume += rightMax - land[right];
			right--;
		}
		else 
		{
			volume += leftMax - land[left];
			left++;
		}
	}
	return volume;
}

double sqrtHero(double a, double precision)
{
	auto seed = [](size_t value) -> size_t
	{
		// https://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Rough_estimation

		size_t count = 0;
		while (value > 1)
		{
			value >>= 2;
			++count;
		}
		return 1 << count; 
	};

	double x = seed(static_cast<size_t>(round(a)));

	while (abs(x*x - a) > precision)
	{
		x = 0.5 * (x + a / x);
		//cout << setprecision(numeric_limits<double>::digits10) << x << '\n';
	}

	//cout << '\n';
	return x;
}


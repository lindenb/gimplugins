#ifndef MY_RANDOM_HH
#define MY_RANDOM_HH

#include <ctime>
#include <cstdlib>


class Random
	{
	private:
		unsigned int seed;
	public:
		Random()
			{
			this->seed = (unsigned int)std::clock();
			std::srand(seed);
			}
		Random(unsigned int seed):seed(seed)
			{
			std::srand(seed);
			}
		int nextInt(int max)
			{
			return rand_r(&seed)%max;
			}
		double rnd()
			{
			return (double)rand_r(&seed)/(double)RAND_MAX;
			}
		bool nextBool()
			{
			return nextInt(2)==0;
			}
		
	};

#endif

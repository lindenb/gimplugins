#ifndef MY_RANDOM_HH
#define MY_RANDOM_HH

#include <ctime>
#include <cstdlib>

class Random
	{
	public:
		Random(unsigned int seed) {std::srand(seed);}
		Random() {std::srand(0U);}
		~Random() {}
		void reset(unsigned int seed) {std::srand(seed);}
		int nextInt(int i) { return std::rand()%i;}
		int nextInt(int beg,int end) { return beg+nextInt(end-beg);}
		double rnd() { return (double)std::rand()/(double)RAND_MAX;}
		int sign() { return (std::rand()%2==0?-1:1);}
	};

#endif

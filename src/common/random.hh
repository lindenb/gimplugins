#ifndef MY_RANDOM_HH
#define MY_RANDOM_HH

#include <ctime>
#include <cstdlib>

class Random
	{
	private:
		unsigned int _seed;
	public:
		Random(unsigned int seed):_seed(seed) {}
		Random():_seed(std::time(NULL)) {}
		~Random() {}
		void reset(unsigned int seed) {this->_seed=seed;}
		unsigned int seed() { return _seed;}
		int nextInt(int i) { return ::rand_r(&_seed)%i;}
		int nextInt(int beg,int end) { return beg+nextInt(end-beg);}
		double rnd() { return (double)::rand_r(&_seed)/(double)RAND_MAX;}
		bool boolean() {  return (::rand_r(&_seed)%2==0);}
		int sign() { return (boolean()?-1:1);}
		double rnd(double beg,double end) { return beg+(end-beg)*rnd();}
		double rnd(double beg) { return beg*rnd();}
	};

#endif

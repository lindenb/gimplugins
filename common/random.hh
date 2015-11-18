#ifndef MY_RANDOM_HH
#define MY_RANDOM_HH

#include <ctime>
#include <cstdlib>

class AbstractRandom
	{
	public:
		AbstractRandom() {}
		virtual ~AbstractRandom() {}
		virtual double rnd()=0;
		virtual int nextInt(int i) { return (int)(rnd()*i);}
		virtual int nextInt(int beg,int end) { return beg+nextInt(end-beg);}
		virtual bool boolean() {  return (rnd()<0.5);}
		virtual int sign() { return (boolean()?-1:1);}
		virtual double rnd(double beg,double end) { return beg+(end-beg)*rnd();}
		virtual double rnd(double beg) { return beg*rnd();}
	};


class Random : public AbstractRandom
	{
	private:
		unsigned int _seed;
	public:
		Random(unsigned int seed):AbstractRandom(),_seed(seed) {}
		Random():AbstractRandom(),_seed(std::time(NULL)) {}
		virtual ~Random() {}
		virtual void reset(unsigned int seed) {this->_seed=seed;}
		virtual unsigned int seed() { return _seed;}
		
		virtual int nextInt(int i) { return ::rand_r(&_seed)%i;}
		
		/** https://twitter.com/klmr/status/667005762565181440 */
		using AbstractRandom::rnd ;
		virtual double rnd() { return (double)::rand_r(&_seed)/(double)RAND_MAX;}
		virtual bool boolean() {  return (::rand_r(&_seed)%2==0);}
	};



#endif

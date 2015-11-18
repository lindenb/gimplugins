#include <vector>
#include "gribouille01.tab.hh"
#include "common/range.hh"
#include "common/wiggle.hh"
#include "common/trigonometry.hh"

using namespace std;

class Gribouille01:public AbstractGribouille01
	{ 
	public:
		
		Gribouille01() {}
		virtual ~Gribouille01() {}
		virtual void  paint(XCairo* ctx,gint image_width,gint image_height);
	};


void Gribouille01::paint(XCairo* ctx,gint image_width,gint image_height)
	{
	long occurences = (long)(((image_width)*(image_height))*prefs()->proba);
	vector<double> coords;
	double minlen = 50.0;
	double maxlen = 70.0;
	double x0 =  rand.rnd(image_width);
	double y0 =  rand.rnd(image_height);
	ctx->move_to(x0,y0);
	
	for(long i=0;i< occurences;++i) {
		double angle= rand.rnd(PI2);
		double L= minlen = rand.rnd(maxlen - minlen);
		double x1 = x0 + cos(angle)*L;
		double y1 = y0 + sin(angle)*L;
		if(x1<0 || x1>=image_width || y1<0 || y1>=image_height)
			{
			x1 =  rand.rnd(image_width);
			y1 =  rand.rnd(image_height);
			ctx->move_to(x1,y1);
			coords.clear();
			}
		
		coords.push_back(x1);
		coords.push_back(y1);
		x0 = x1;
		y0 = y1;
		
		if(coords.size()==6)
			{
			ctx->black();
			
			ctx->curve_to(
				coords[0],coords[1],
				coords[2],coords[3],
				coords[4],coords[5]
				);
			ctx->stroke();
			ctx->move_to(x0,y0);
			coords.clear();
			}
		}
	}


#ifdef STANDALONE
int main(int argc,char** argv)
	{
	Gribouille01 app;
	return app.main(argc,argv);
	}
#endif


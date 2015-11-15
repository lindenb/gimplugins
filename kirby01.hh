#include <vector>
#include "kirby01.tab.hh"
#include "common/range.hh"
#include "common/wiggle.hh"
#include "common/trigonometry.hh"

using namespace std;

class Kirby01:public AbstractKirby01
	{ 
	public:
		
		Kirby01() {}
		virtual ~Kirby01() {}
		virtual void  paint(XCairo* ctx,gint image_width,gint image_height);
	};


void Kirby01::paint(XCairo* ctx,gint image_width,gint image_height)
	{
	long occurences = (long)(((image_width)*(image_height))*prefs()->proba);
	double maxa=std::max( prefs()->minalpha,prefs()->maxalpha );
	double mina=std::min( prefs()->minalpha,prefs()->maxalpha );
	
	double maxr=std::max( prefs()->minradius,prefs()->maxradius );
	double minr=std::min( prefs()->minradius,prefs()->maxradius );

	for(long i=0;i< occurences;++i)
		{
  		
  		
  		double cy= rand.rnd( image_height ) ;
		double cx = ((exp(rand.rnd(1.0))-1)/(exp(1.0)-1))* image_width;

		double r = minr + ((image_width-cx)/image_width)*(maxr-minr);
		double a = mina + ((image_width-cx)/image_width)*(maxa-mina);
		
		ctx->black(a);
		if(prefs()->otherside && i%2==0)
			{
			cx = image_width-cx;
			ctx->white(a);
			}
		
		switch(prefs()->shape)
			{
			case E_shape_circle: ctx->circle(cx,cy,r); break;
			case E_shape_square: ctx->cube(cx,cy,r); break;
			default:
				{
				if(rand.boolean())
					{
					ctx->circle(cx,cy,r);
					}
				else
					{
					ctx->cube(cx,cy,r);
					}
				break;
				}
			}
		switch(prefs()->style)
			{
			case E_style_stroke: ctx->stroke(); break;
			default : ctx->fill(); break;
			}
		}
	}
#ifdef STANDALONE
int main(int argc,char** argv)
	{
	Kirby01 app;
	return app.main(argc,argv);
	}
#endif


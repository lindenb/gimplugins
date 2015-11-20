#include <vector>
#include "hatching01.tab.hh"
#include "common/wiggle.hh"
#include "common/trigonometry.hh"

using namespace std;


#define CLASSNAME Hatching01

class CLASSNAME:public AbstractHatching01
	{ 
	public:
		
		CLASSNAME() {}
		virtual ~CLASSNAME() {}
		virtual void  paint(XCairo* ctx,gint image_width,gint image_height);
	};


void CLASSNAME::paint(XCairo* ctx,gint image_width,gint image_height)
	{
	WiggleLine wiggle(&rand);
	MinMax alpha(prefs()->minalpha, prefs()->maxalpha);
	MinMax lwidth(prefs()->minlinewidth, prefs()->maxlinewidth);
	int dx=5;
	for(int side=0;side<2;++side)
		{
		int p0 =- rand.rnd(-dx,dx);
		while(p0 < (side==0 ? image_width: image_height) )
			{
			std::vector<double> pts;
			if( side==0)
				{
				wiggle.line(pts,p0,0,p0,image_height);
				}
			else
				{
				wiggle.line(pts,0,p0,image_width,p0);
				}
			
			ctx->line_width(lwidth.rnd(&rand));
			ctx->black(alpha.rnd(&rand));
			
			unsigned int i=0;
			while(i+1 < pts.size())
				{
				if(i==0)
					{
					ctx->move_to(pts[i],pts[i+1]);
					i+=2;
					}
				else if(i+5< pts.size())
					{
					ctx->curve_to(
						pts[i+0],pts[i+1],
						pts[i+2],pts[i+3],
						pts[i+4],pts[i+5]
						);
					
					i+=6;
					}
				else
					{
					ctx->line_to(pts[i],pts[i+1]);
					i+=2;
					}
				}
			ctx->stroke();
			
			p0 += ( 1 + rand.rnd(dx)) ;
			}
		}
	}


#ifdef STANDALONE
int main(int argc,char** argv)
	{
	CLASSNAME app;
	return app.main(argc,argv);
	}
#endif


#include <vector>
#include "hatching02.tab.hh"
#include "common/wiggle.hh"
#include "common/trigonometry.hh"

using namespace std;


#define CLASSNAME Hatching02

class CLASSNAME:public AbstractHatching02
	{ 
	public:
		
		CLASSNAME() {}
		virtual ~CLASSNAME() {}
		virtual void  paint_square(XCairo* ctx,double x,double y,double w,int side);
		virtual void  paint(XCairo* ctx,gint image_width,gint image_height);
	};


void CLASSNAME::paint_square(XCairo* ctx,double x,double y,double square,int side)
	{
	WiggleLine wiggle(&rand);
	MinMax alpha(prefs()->minalpha, prefs()->maxalpha);
	MinMax lwidth(prefs()->minlinewidth, prefs()->maxlinewidth);
	int dx=5;
	
	ctx->translate (x,y);
	
	int p0 =- rand.rnd(-dx,dx);
	while(p0 < square )
		{
		std::vector<double> pts;
		if( side==0)
			{
			wiggle.line(pts,p0,0,p0,square);
			}
		else
			{
			wiggle.line(pts,0,p0,square,p0);
			}
		
		ctx->line_width(lwidth.rnd(&rand));
		for(unsigned int i=0;i+1 < pts.size();i+=2)
			{
			if(i==0)
				{
				ctx->move_to(pts[i],pts[i+1]);
				}
			else
				{
				ctx->line_to(pts[i],pts[i+1]);
				ctx->black(alpha.rnd(&rand));
				ctx->stroke();
				ctx->move_to(pts[i],pts[i+1]);
				}
			}
		
		p0 += ( 1 + rand.rnd(dx)) ;
		}
	
	ctx->translate(-x,-y);
	}

void CLASSNAME::paint(XCairo* ctx,gint image_width,gint image_height)
	{
	double square = prefs()->quare_size;
	double x=0;
	int nx=0;
	while(x<image_width)
		{
		double y=0;
		int ny=0;
		while(y<image_width)
			{
			paint_square(ctx,x,y,square,(nx%2==ny%2?0:1));
			y+=square;
			ny++;
			}
		x+=square;
		nx++;
		}
	}


#ifdef STANDALONE
int main(int argc,char** argv)
	{
	CLASSNAME app;
	return app.main(argc,argv);
	}
#endif


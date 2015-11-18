#include <vector>
#include "hatching01.tab.hh"
#include "common/range.hh"
#include "common/wiggle.hh"
#include "common/trigonometry.hh"

using namespace std;

class Hatching01:public AbstractHatching01
	{ 
	public:
		
		Hatching01() {}
		virtual ~Hatching01() {}
		virtual void  paint(XCairo* ctx,gint image_width,gint image_height);
	};


void Hatching01::paint(XCairo* ctx,gint image_width,gint image_height)
	{
	Wiggle wiggle(&rand);
	int dx=5;
	for(int side=0;side<2;++side)
		{
		int p0 =- rand.rnd(-dx,dx);
		while(p0 < (side==0 ? image_width: image_height) )
			{
			std::vector<double> pts;
			if( side==0)
				{
				wiggle.drawWiggle(pts,p0,0,p0,image_height);
				}
			else
				{
				wiggle.drawWiggle(pts,0,p0,image_width,p0);
				}
			
			ctx->line_width(rand.rnd(prefs()->minlinewidth, prefs()->maxlinewidth));
			for(unsigned int i=0;i+1 < pts.size();i+=2)
				{
				if(i==0)
					{
					ctx->move_to(pts[i],pts[i+1]);
					}
				else
					{
					ctx->line_to(pts[i],pts[i+1]);
					ctx->black(rand.rnd(prefs()->minalpha, prefs()->maxalpha));
					ctx->stroke();
					ctx->move_to(pts[i],pts[i+1]);
					}
				}
			
			p0 += ( 1 + rand.rnd(dx)) ;
			}
		}
	}


#ifdef STANDALONE
int main(int argc,char** argv)
	{
	Hatching01 app;
	return app.main(argc,argv);
	}
#endif


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

	}
#ifdef STANDALONE
int main(int argc,char** argv)
	{
	Kirby01 app;
	return app.main(argc,argv);
	}
#endif


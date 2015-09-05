
#include <vector>
#include <cmath>
#include <cstring>
#include <vector>
#include <climits>
#include <map>
#include <unistd.h>
#include <getopt.h>
#include "xcairo.hh"
#include "common/random.hh"
#include "common/geometry.hh"
#include "common/range.hh"
#include "common/trigonometry.hh"
#include "common/wiggle.hh"

using namespace std;



static Random RANDOM;



class Hatch01
	{
	public:
		XCairo *ctx;
		std::vector<PointD> points;
		Rect<int> page;
		Point<int> center;
		int iteration; 
		Range stroke_width;
		Range gray;
		Range stroke_size;
		Range distance_between_strokes;
		Range distance_between_circles;
		int user_precision;
		
		Hatch01():stroke_width(1,3),
				gray(0.1,0),
				stroke_size(50,40),
				distance_between_strokes(3,1),
				distance_between_circles(3,0),
				user_precision(1)
			{
			ctx=0;
			page.x=0;
			page.y=0;
			page.width=1000;
			page.height=1000;
			center = page.center();
			iteration=0;
			}
		~Hatch01()
			{
			if(ctx!=0) delete ctx;
			}
		
		
		
		void circular()
			{
			Wiggle wiggle(&RANDOM);
			map<Radian,double>* rad2radius=0;
			map<Radian,double>* next_rad2radius=0;
			int max_iterations=20;
			
			ctx = new XCairo(page.width, page.height);
			ctx->line_width(1);
			ctx->rgb(0, 0, 0);
			double radius=0;
			double prev_radius=0;
			
			for(;;)
				{
				++iteration;
				next_rad2radius = new map<Radian,double>;
				
				radius += stroke_size(iteration,max_iterations);
				double perimeter = PI2*radius;
				double rad_between_strokes =   distance_between_strokes(iteration,max_iterations)/radius;
				
				Radian angle(PI2 * RANDOM.rnd());
				int n_strokes = (int)(PI2/rad_between_strokes); 
				int sens=1;
				cerr << "iter:"<< iteration << " "<<n_strokes << " r=" << radius << " p=" << perimeter << " r2=" << rad_between_strokes << endl;
				for(int n=0; n< n_strokes; ++n)
					{
					double a_radius = prev_radius+RANDOM.nextInt(-user_precision,user_precision);
					if( rad2radius!=0)
						{
						a_radius=0;
						double m=0;
						map<Radian,double>::iterator r= rad2radius->lower_bound(angle);
						while(r!=rad2radius->end())
							{
							if( r->first() > angle()+rad_between_strokes) break;
							a_radius+=r->second;
						++m;
							++r;
							}
						a_radius/=m;
						}
					
					
					double ax=
						center.x + angle.cos()*(a_radius)+RANDOM.nextInt(-user_precision,user_precision);
					
					double ay=
						center.y + angle.sin()*(a_radius)+RANDOM.nextInt(-user_precision,user_precision);
					
					Radian b_angle ( modulo_pi2(angle() + RANDOM.rnd()*0.001 * sens) ) ;
					
					
					
					double b_radius = radius+RANDOM.nextInt(-1,fabs(30.0*b_angle.sin()));
					
					double bx=
						center.x + b_angle.cos()*(b_radius)+RANDOM.nextInt(-user_precision,user_precision);
					
					double by=
						center.y + b_angle.sin()*(b_radius)+RANDOM.nextInt(-user_precision,user_precision);
					
					double b_distance = sqrt(pow(bx-center.x,2)+pow(by-center.y,2));
					//cerr << b_angle << " " << b_distance << endl;
					next_rad2radius->insert(make_pair<Radian,double>(b_angle,b_distance + distance_between_circles(iteration,max_iterations)));
					
					
					if(iteration>1)
						{
						vector<double> pts;
						wiggle.drawWiggle(pts,ax,  ay,  bx,  by);
						Range r(1.0,0.5);
						Range lw(1.0,0.8);
						for(size_t i=0;i+1< pts.size();i+=2)
							{
							double gray_v = gray(iteration,max_iterations);
							ctx->rgba(gray_v,gray_v,gray_v,r(i,pts.size()/2));
							if(i==0)
								{
								ctx->move_to(pts[0],pts[1]);
								}
							else
								{
								ctx->line_to(pts[i],pts[i+1]);
								ctx->line_width(stroke_width(iteration,max_iterations)*lw(i,pts.size()/2));
								ctx->stroke();
								ctx->move_to(pts[i],pts[i+1]);
								}
							}
						}
					angle += rad_between_strokes;
					angle = modulo_pi2(angle());
					}
				if(rad2radius!=0) delete rad2radius;
				rad2radius = next_rad2radius;
				
				
				if(radius>page.width) break;
				
				radius+=max(2.0,distance_between_circles(iteration,max_iterations));
				prev_radius=radius;
				}
			ctx->png("jeter.png");
			
			if(rad2radius!=0) delete rad2radius;
			}
		
		void linear()
			{
			Wiggle wiggle(&RANDOM);
			map<double,double>* x2y=0;
			map<double,double>* next_x2y=0;
			int max_iterations=20;
			
			ctx = new XCairo(page.width, page.height);
			ctx->line_width(1);
			ctx->rgb(0, 0, 0);
			double y=page.height;
			
			while(y + stroke_size(iteration,max_iterations) >=0)
				{
				++iteration;
				next_x2y = new map<double,double>;
				
				//double rad_between_strokes =   distance_between_strokes(iteration,max_iterations)/radius;
				
				double x(0);
				double min_y=y;
				
				int sens=1;
				
				Radian angle(PI/RANDOM.nextDouble(2,5));
				
				while(x< page.width)
					{
					cerr << "iter:"<< iteration  << " yx=" << y <<"/"  << x << endl;
					double ay = y;
					if( x2y!=0)
						{
						map<double,double>::iterator r= x2y->lower_bound(x);
						if(r!=x2y->end() )
							{
							double ay2=0;
							double m=0;
							while(r!=x2y->end())
								{
								if( r->first > x+ distance_between_strokes(iteration,max_iterations) ) break;
								ay2 += r->second;
								++m;
								++r;
								}
							ay2/=m;
							}
						}
					
					
					double ax= x + RANDOM.nextInt(-user_precision,user_precision);
					ay += RANDOM.nextInt(-user_precision,user_precision);
					
					
					double length= stroke_size(iteration,max_iterations) + RANDOM.nextInt(-user_precision,user_precision);
					
					double bx= ax + angle.cos()*length + RANDOM.nextInt(-user_precision,user_precision);
					
					double by= ay - angle.sin()*length;
					
					next_x2y->insert(make_pair<double,double>(bx,min(by,ay) + distance_between_circles(iteration,max_iterations)));
					
					min_y =std::min(ay,std::min(by,min_y));
					

						vector<double> pts;
						wiggle.drawWiggle(pts,ax,  ay,  bx,  by);
						Range r(1.0,0.5);
						Range lw(1.0,0.8);
						for(size_t i=0;i+1< pts.size();i+=2)
							{
							double gray_v = gray(iteration,max_iterations);
							ctx->rgba(gray_v,gray_v,gray_v,r(i,pts.size()/2));
							if(i==0)
								{
								ctx->move_to(pts[0],pts[1]);
								}
							else
								{
								ctx->line_to(pts[i],pts[i+1]);
								ctx->line_width(stroke_width(iteration,max_iterations)*lw(i,pts.size()/2));
								ctx->stroke();
								ctx->move_to(pts[i],pts[i+1]);
								}
							}
						
						
					x +=  distance_between_strokes(iteration,max_iterations) + RANDOM.nextInt(user_precision,-user_precision);
					}
				if(x2y!=0) delete x2y;
				x2y = next_x2y;
				
				
				y=min_y-max(2.0,distance_between_circles(iteration,max_iterations));
				}
			ctx->png("jeter.png");
			
			if(x2y!=0) delete x2y;
			}
		
		void run()
			{
			linear();
			}
		
	};

static void parse_range(const char* s,double* w,double* h)
	{
	if(s==0) return;
	char* colon=strchr((char*)s,'-');
	if(colon==0)
		{
		double v= strtod(s,NULL);
		*w=v;
		*h=v;
		}
	else
		{
		*w=strtod(s,NULL);
		*h=strtod(colon+1,NULL);
		}
	}

static void parse_range(const char* s,Range* r)
	{
	 parse_range(s,&(r->beg),&(r->end));
	}

static void parse_dimension(const char* s,int* w,int* h)
	{
	}

int main(int argc,char** argv)
	{
	Hatch01 h;
	char* filenameout = NULL;
	for(;;)
		{
			int c;
               int option_index = 0;
               static struct option long_options[] = {
                   {"output", required_argument, 0,  'o' },
                   {"stroke-width", required_argument, 0,   0 },
                   {"page", required_argument, 0,   0 },
                   {0,         0,                 0,  0 }
                   };

               c = getopt_long(argc, argv, "o:",
                        long_options, &option_index);
               if (c == -1)
                   break;
	       switch(c)
			{
			case 0:
				{
				if(strcmp(long_options[option_index].name,"stroke-width")==0)
					{
					parse_range(optarg,&h.stroke_width);
					}
				else if(strcmp(long_options[option_index].name,"page")==0)
					{
					parse_dimension(optarg,&h.page.width,&h.page.height);
					}
				break;
				}
			case 'o': filenameout = optarg; break;
			case '?':
				cerr << "Unknown option"<< endl;
				return EXIT_FAILURE;
			default:
				cerr << "getopt returned character code ??\n";
				return EXIT_FAILURE;
			}
	       }
	if(optind!=argc)
		{
		cerr << "Illegal number of arguments." << endl;
		return EXIT_FAILURE;
		}
	
	h.run();
	return 0;
	}
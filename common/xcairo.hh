#ifndef MY_CAIRO_HH
#define MY_CAIRO_HH
#include <iostream>
#include <cstdlib>
#include <cairo/cairo.h>
#include "common/trigonometry.hh"

class XCairo
	{
	private:
		cairo_surface_t *surface;
		cairo_t *cr;
	public:
		XCairo(cairo_surface_t *surface):surface(surface)
			{
			if(surface == NULL)
				{
				std::cerr << "Cannot create image surface "<< std::endl;
				std::exit(EXIT_FAILURE);
				}
			cr = ::cairo_create (surface);
			if(cr == NULL)
				{
				std::cerr << "Cannot create cairo "<< std::endl;
				std::exit(EXIT_FAILURE);
				}
#if CAIRO_VERSION_MAJOR >= 1 && CAIRO_VERSION_MINOR >= 12  
			::cairo_set_antialias(cr,CAIRO_ANTIALIAS_BEST);
#endif
			::cairo_set_line_cap (cr,CAIRO_LINE_CAP_ROUND);
			}
	
		XCairo(int width,int height)
			{
			surface = ::cairo_image_surface_create (CAIRO_FORMAT_ARGB32,width,height);
			if(surface == NULL)
				{
				std::cerr << "Cannot create image surface "<< std::endl;
				std::exit(EXIT_FAILURE);
				}
			cr = cairo_create (surface);
			if(cr == NULL)
				{
				std::cerr << "Cannot create cairo "<< std::endl;
				std::exit(EXIT_FAILURE);
				}
#if CAIRO_VERSION_MAJOR >= 1 && CAIRO_VERSION_MINOR >= 12  
			::cairo_set_antialias(cr,CAIRO_ANTIALIAS_BEST);
#endif
			::cairo_set_line_cap (cr,CAIRO_LINE_CAP_ROUND);
			}
		void flush()
			{
			::cairo_surface_flush(surface);
			}
		~XCairo()
			{
			flush();
			::cairo_destroy(cr);
			::cairo_surface_destroy(surface);
			}
		void png(const char* file)
			{
			::cairo_surface_write_to_png(surface,file);
			}
				
		void rgb(double r,double g,double b)
			{
			::cairo_set_source_rgb (cr,r,g,b);
			}
		
		void rgba(double r,double g,double b,double a)
			{
			::cairo_set_source_rgba (cr,r,g,b,a);
			}
		

		void gray(double g)
			{
			rgb(g,g,g);
			}
		
		void gray(double g,double a)
			{
			rgba(g,g,g,a);
			}
		void black()
			{
			gray(0);
			}		
		void black(double a)
			{
			gray(0,a);
			}		
		void white()
			{
			gray(1);
			}
		
		void white(double a)
			{
			gray(1,a);
			}

		
		void rectangle(double x,double y,double  w,double h)
			{
			::cairo_rectangle(cr,x,y,w,h);
			}
		
		void cube(double cx,double cy,double r)
			{
			rectangle(cx-r,cy-r,r*2,r*2);
			}

		void line_width(double d)
			{
			::cairo_set_line_width (cr,d);
			}
		double line_width()
			{
			return ::cairo_get_line_width(cr);
			}
		void move_to(double x,double y)
			{
			::cairo_move_to(cr,x,y);
			}
		void line_to(double x,double y)
			{
			::cairo_line_to(cr,x,y);
			}
		
		void curve_to(double x1,double y1,double x2,double y2,double x3,double y3)
			{
			::cairo_curve_to(cr,x1,y1,x2,y2,x3,y3);
			}
		
		void stroke()
			{
			::cairo_stroke (cr);
			}
		void fill()
			{
			::cairo_fill (cr);
			}
		void translate(double tx,double ty)
			{
			::cairo_translate(cr,tx,ty);
			}
		
		void arc(double xc,
           double yc,
           double radius,
           double angle1,
           double angle2)
           {
           ::cairo_arc(cr,xc,yc,radius,angle1,angle2);
           }
          
		void circle(double xc,  double yc,  double radius  )
           {
           this->arc(xc,yc,radius,0,PI2);
           }
		
		void status()
			{
			cairo_status_t status= ::cairo_surface_status(this->surface);
			if(status!=CAIRO_STATUS_SUCCESS)
				{
				std::cerr <<  cairo_status_to_string(status) << std::endl;
				}
			}
		
		
	};

#endif

/*
 * xgimp.hh
 *
 *  Created on: Aug 19, 2015
 *      Author: lindenb
 */

#ifndef SRC_COMMON_XGIMP_HH_
#define SRC_COMMON_XGIMP_HH_

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>
#include <libgimp/gimpcompat.h>
#include <cstring>
#include "common/debug.hh"
#include "common/xcairo.hh"

struct XColorRGB
	{
	gint r,g,b;
	};
	
struct XColorRGBA: XColorRGB
	{
	gint a;
	};

template<typename T>
struct XPoint
	{
	T x,y;
	};

template<typename T>
struct XRect
	{
	T x,y,width,height;
	};


template<typename T>
struct XBound
	{
	T x1,y1,x2,y2;
	T x() const { return x1;}
	T y() const { return y1;}
	T width() const { return x2-x1;}
	T height() const { return y2-y1;}
	XRect<T> rect() const
		{
		XRect<T> r;
		r.x=x();
		r.y=y();
		r.width = width();
		r.height = height();
		return r;
		}
	};


class XItem
	{
	public:
		XItem()
		{
		}
		virtual ~XItem()
		{

		}
		virtual gint32 id()=0;
		gboolean  is_visible()
			{
			return ::gimp_drawable_get_visible(id());
			}
	};

class XDrawable:public XItem
	{
	private:
		GimpDrawable* _drawable;
	public:
		XDrawable(GimpDrawable* drawable):_drawable(drawable)
			{

			}
		XDrawable(const XDrawable& cp):_drawable(cp._drawable)
			{

			}
		XDrawable& operator=(const XDrawable& cp)
			{
			if(this!=&cp)
				{
				this->_drawable = cp._drawable;
				}
			return *this;
			}
		GimpDrawable* drawable()
			{
			return this->_drawable;
			}
		virtual gint32 id()
			{
			return this->_drawable->drawable_id;
			}
		gint width()
		{
			return ::gimp_drawable_width(id());
		}

		gint height()
		{
			return ::gimp_drawable_height(id());
		}

		gint32 image()
		{
			 return gimp_drawable_get_image(id());
		}
		
		gboolean is_layer() { return ::gimp_drawable_is_layer(id());}
		gboolean is_layer_mask() { return ::gimp_drawable_is_layer_mask(id());}
		gboolean is_text_layer() { return ::gimp_drawable_is_text_layer(id());}
		gboolean selection_is_empty() 
			{
			return ::gimp_selection_is_empty (image());
			}
	

		void detach()
			{
			::gimp_drawable_detach(drawable());
			}
		void flush()
			{
			::gimp_drawable_flush(drawable());
			}
		guint8 *  get_pixel(gint x_coord,gint y_coord,gint *num_channels)
			{
			return ::gimp_drawable_get_pixel(id(),x_coord,y_coord,num_channels);
			}
		
		GimpImageType  type()
			{
			return ::gimp_drawable_type(id());
			}
		
		GimpImageType  type_with_alpha()
			{
			return ::gimp_drawable_type_with_alpha(id());
			}
		
		gboolean  is_gray()
			{
			return :: gimp_drawable_is_gray(id());
			}
		gboolean  is_rgb()
			{
			return ::gimp_drawable_is_rgb(id());
			}

		
		gboolean mask_bounds(gint *x1,gint *y1,gint *x2,gint *y2)
			{
			return ::gimp_drawable_mask_bounds(id(),x1,y1,x2,y2);
			}
		
		gboolean mask_bounds(XBound<gint>* b)
			{
			return this->mask_bounds(&(b->x1),&(b->y1),&(b->x2),&(b->y2));
			}
		XBound<gint> mask_bounds()
			{
			XBound<gint> b;
			if(!this->mask_bounds(&b))
				{
				b.x1=0;
				b.y1=0;
				b.x2=0;
				b.y2=0;
				}
			return b;
			}

		gboolean has_alpha()
			{
			return ::gimp_drawable_has_alpha(id());
			}
		gboolean  merge_shadow(gboolean undo)
			{
			return ::gimp_drawable_merge_shadow(id(),undo);
			}
		gboolean update(gint x,  gint y, gint width,  gint height)
			{
			return ::gimp_drawable_update(id(),x,y,width,height);
          	}
                gboolean update(XRect<gint> rect)
			{
			return update(rect.x,rect.y,rect.width,rect.height);
                        }
                 gboolean update()
			{
			return update(mask_bounds().rect());
                        }
         void gimp_detach()
         	{
            ::gimp_drawable_detach (this->drawable());
			}
		void offsets(gint      *offset_x, gint      *offset_y)
			{
			::gimp_drawable_offsets(id(),offset_x,offset_y);
			}
		gint x()
			{
			gint a,b;
			offsets(&a,&b);
			return a;
			}	
		gint y()
			{
			gint a,b;
			offsets(&a,&b);
			return b;
			}
		
		
	};


class XPreview
	{
	private:
		 GimpPreview* _preview;
	public:
		XPreview():_preview(0)
			{
			}
		XPreview( GimpPreview* preview):_preview(preview)
			{

			}
		XPreview(const XPreview& cp):_preview(cp._preview)
			{

			}
		XPreview& operator=(const XPreview& cp)
			{
			if(this!=&cp)
				{
				this->_preview = cp._preview;
				}
			return *this;
			}
			
		GimpPreview* preview() const
			{
			return this->_preview;
			}
		
		bool nil () const
			{
			return preview() == 0;
			}
			
		void draw()
			{
			if(!nil()) ::gimp_preview_draw(preview());
			}
		void invalidate()
			{
			if(!nil()) ::gimp_preview_invalidate(preview());
			}
		
		void get_position(gint* x,gint* y)
			{
			if(!nil()) gimp_preview_get_position (preview(),x,y);
			}
		
		gint x()
			{
			gint _x,_y;
			get_position(&_x,&_y);
			return _x;
			}
		gint y()
			{
			gint _x,_y;
			get_position(&_x,&_y);
			return _y;
			}
		
		void get_size(gint* w,gint* h)
			{
			if(!nil()) gimp_preview_get_size (preview(),w,h);
			}
		
		gint width()
			{
			gint _w,_h;
			get_size(&_w,&_h);
			return _w;
			}
		gint height()
			{
			gint _w,_h;
			get_size(&_w,&_h);
			return _h;
			}
      	operator bool () const
      		{
      		return !nil();
      		}
      	void draw_region(const GimpPixelRgn *region)
      		{
      		if(!nil()) ::gimp_drawable_preview_draw_region(GIMP_DRAWABLE_PREVIEW(preview()),region);
      		}
      
	};


class XTileIterator1
	{
	private:
		GimpPixelRgn _src_rgn, _dest_rgn;
		gpointer pr;
  		bool has_alpha;
  		XCairo* xcairo;
  		bool is_preview;
	public:
		XTileIterator1(XDrawable& drawable,XPreview& preview):pr(NULL),xcairo(0)
			{
			gint x,y,width,height;

			
			is_preview = !preview.nil();
			
			if(is_preview)
				{
				preview.get_position (&x, &y);
      			preview.get_size ( &width, &height);
				}
			else if(drawable.selection_is_empty())
				{
				x=0;
				y=0;
				width = drawable.width();
				height = drawable.height();
				}
			else
				{
				gint x2,y2;
				drawable.mask_bounds(&x,&y,&x2,&y2);
				width = x2-x;
				height = y2-y;
				}
			
			
			/* A "dirty" tile is one that has been changed. Tiles that are not dirty won't be written back to GIMP, whereas dirty ones will be. Initializing a pixel region as "dirty" indicates to gimp_pixel_rgns_process that it should treat tiles in that region as if you've dirtied them. 
			*/
			/* "Shadow tiles are merely an indication of the desire to use a temporary buffer for writing in to */
			::gimp_pixel_rgn_init (
				&_src_rgn,
				drawable.drawable(),
		   		x,y,width,height,
		   		FALSE, /* dirty */
		   		FALSE /* shadow */
		   		);
			
			::gimp_pixel_rgn_init (
				&_dest_rgn,
				drawable.drawable(),
		   		x,y,width,height,
		   		!is_preview,
		   		TRUE/* shadow */
		   		);
		       	
  			has_alpha = drawable.has_alpha();
  			pr = ::gimp_pixel_rgns_register (2, &_src_rgn, &_dest_rgn);
			}
		~XTileIterator1()
			{
			if( xcairo != 0)
				{
				xcairo->flush();
				xcairo->status();
				delete xcairo;
				xcairo=0;
				}
			if(!this->is_preview)
				{
				::gimp_progress_end();
				}
			}
		
		GimpPixelRgn* source()
			{
			return &_src_rgn;
			}
		GimpPixelRgn* destination()
			{
			return &_dest_rgn;
			}
		
		bool ok() const 
			{ 
			return pr!=NULL; 
			}
		operator bool() const 
			{ 
			return ok(); 
			}
		XTileIterator1& operator++()
			{
			if( xcairo != 0)
				{
				xcairo->flush();
				delete xcairo;
				xcairo = 0;
				}
			
			if(pr!=NULL)
				{
				pr = ::gimp_pixel_rgns_process(pr);
				if(!this->is_preview)
					{
					 :: gimp_progress_pulse();
					 }
				}
			return *this;
			}
		gint x() const { return _src_rgn.x;}
		gint y() const { return _src_rgn.y;}
		gint height() const { return _src_rgn.h;}
		gint width() const { return _src_rgn.w;}
	private:
		guchar* _at(GimpPixelRgn* base,gint y,gint x)
			{
			if(pr==NULL) return NULL;
			guchar* s= base->data;
			s+= (base->rowstride)*y;
			s+= (base->bpp)*x;
			return s;
			}	
	public:
		guchar* src_at(gint y,gint x)
			{
			return _at(source(),y,x);
			}
		guchar* dest_at(gint y,gint x)
			{
			return _at(destination(),y,x);
			}
		
		XCairo* cairo()
			{
			if( xcairo == 0 )
				{
				
				cairo_surface_t* s= ::cairo_image_surface_create_for_data (
					 _dest_rgn.data,
				     CAIRO_FORMAT_ARGB32,
				      _dest_rgn.w,
				      _dest_rgn.h,
				      _dest_rgn.rowstride
				     );
				
			    cairo_status_t status= ::cairo_surface_status (s);
				if(status!=CAIRO_STATUS_SUCCESS)
					{
					std::cerr <<  cairo_status_to_string(status) << std::endl;
					}
				
			    this->xcairo = new XCairo(s);
			    this->xcairo->translate(
		             -(_dest_rgn.x),
		             -(_dest_rgn.y)
		             );
			   
				}
			return xcairo;
			}
		/* copy source to destination */
		void copy()
			{
			/*
			std::memcpy(
				(void*)_src_rgn.data,
				(void*)_dest_rgn.data,
				(_dest_rgn.h*_dest_rgn.rowstride)
				);*/
			}

	};



#endif /* SRC_COMMON_XGIMP_HH_ */

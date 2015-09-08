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
	};


class XPreview
	{
	private:
		 GimpPreview* _preview;
	public:
		XPreview():_preview(0)
			{
			}
		XPreview( GimpPreview* preview):_preview(_preview)
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
			
		GimpPreview* preview()
			{
			return this->_preview;
			}
		
		bool nil ()
			{
			return preview()==0;
			}
			
		void draw()
			{
			if(!nil()) ::gimp_preview_draw(preview());
			}
		void invalidate()
			{
			if(!nil()) ::gimp_preview_invalidate(preview());
			}
		
		
	};


class XTileIterator1
	{
	private:
		GimpPixelRgn _src_rgn, _dest_rgn;
		gpointer pr;
  		bool has_alpha;
	public:
		XTileIterator1(XDrawable& drawable):pr(NULL)
			{
			XBound<gint> rect;
			drawable.mask_bounds(&rect);
			::gimp_pixel_rgn_init (
				&_src_rgn,
				drawable.drawable(),
		       		rect.x(),
		       		rect.y(),
		       		rect.width(),
		       		rect.height(),
		       		FALSE,
		       		FALSE
		       		);
		       	::gimp_pixel_rgn_init (
				&_dest_rgn,
				drawable.drawable(),
		       		rect.x(),
		       		rect.y(),
		       		rect.width(),
		       		rect.height(),
		       		TRUE,
		       		TRUE
		       		);
      			has_alpha = drawable.has_alpha();
      			pr = ::gimp_pixel_rgns_register (2, &_src_rgn, &_dest_rgn);
			}
		~XTileIterator1()
			{
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
			if(pr!=NULL)
				{
				pr = ::gimp_pixel_rgns_process(pr);
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
			return _at(&_src_rgn,y,x);
			}
		guchar* dest_at(gint y,gint x)
			{
			return _at(&_dest_rgn,y,x);
			}
			
		bool contains(gint y,gint x)
			{
			/*boolean b= !(
				y<0 ||
				x<0 ||
				x >= (this->x()+this->witdh()) ||
				y >= (this->y()+this->height())
				);
			return b;*/
			return true;
			}
		
		void setPixel(gint y,gint x,guchar* pix)
			{
			if(!contains(y,x)) return;
			guchar* d = dest_at(y,x);
			d[0]=pix[0];
			d[1]=pix[1];
			d[2]=pix[2];
			}
		
		struct Pixel
			{
			XTileIterator1* owner;
			gint y,x;
			};
		
		Pixel at(gint y,gint x)
			{
			Pixel p;
			p.owner = this;
			p.y = y;
			p.x = x;
			return p;
			}
	};



#endif /* SRC_COMMON_XGIMP_HH_ */

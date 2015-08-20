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
	T width() const { return x2-x1;}
	T height() const { return y2-y1;}
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
			return ::gimp_item_get_visible(id());
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
		XBound<gint> mask_bounds()
			{
			XBound<gint> b;
			if(!this->mask_bounds(&b.x1,&b.y1,&b.x2,&b.y2))
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
	};



#endif /* SRC_COMMON_XGIMP_HH_ */

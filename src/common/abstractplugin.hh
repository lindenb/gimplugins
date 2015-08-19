#ifndef ABSTRACT_PLUGIN_HH
#define ABSTRACT_PLUGIN_HH
#include "common/xgimp.hh"

class AbstractPlugin
	{
	public:
		AbstractPlugin()
			{
			}
		virtual ~AbstractPlugin()
			{
			}
		virtual void run(XDrawable drawable)=0;
	};

#endif

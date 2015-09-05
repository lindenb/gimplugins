#ifndef ABSTRACT_PLUGIN_HH
#define ABSTRACT_PLUGIN_HH
#include "common/xgimp.hh"


#define DEFAULT_GIMP_PLUGIN_QUERY(FUN,NAME,DESCRIPTION) \
static void FUN (void) {\
  static GimpParamDef args[] = {\
    { GIMP_PDB_INT32, (gchar*)"run_mode", (gchar*)"Interactive, non-interactive" },\
    { GIMP_PDB_IMAGE, (gchar*)"image", (gchar*)"Input image" },\
    { GIMP_PDB_DRAWABLE, (gchar*)"drawable", (gchar*)"Input drawable" }\
  };\
  gimp_install_procedure (NAME,\
			  DESCRIPTION,\
			  DESCRIPTION,\
			  "Pierre Lindenbaum  <plindenbaum@yahoo.fr>",\
			  "Pierre Lindenbaum",\
			  __DATE__,\
			  "<Image>/Filters/Kakaheska/" NAME,\
			  "RGB*",\
			  GIMP_PLUGIN,\
			  G_N_ELEMENTS(args), 0,\
			  args, NULL);\
	}


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

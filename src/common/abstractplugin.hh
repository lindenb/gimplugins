#ifndef ABSTRACT_PLUGIN_HH
#define ABSTRACT_PLUGIN_HH
#include "common/xgimp.hh"



template<typename T>
class AbstractPlugin
	{
	public:
		
		AbstractPlugin()
			{
			}
		virtual ~AbstractPlugin()
			{
			}
		virtual void run(XDrawable drawable,XPreview preview)=0;
		
		
		virtual void run(XDrawable drawable)
			{
			XPreview preview;
			this->run(drawable,preview);
			}
		
		
		virtual const char* name() const=0;
		virtual gboolean dialog(XDrawable drawable)=0;
		virtual T* prefs()=0;
		
		virtual void run1(
			 gchar   *name,
			 gint     nparams,
			 GimpParam  *param,
			 gint    *nreturn_vals,
			 GimpParam **return_vals
			 )
			 {
			 static GimpParam  values[1];
  			 GimpPDBStatusType status = GIMP_PDB_SUCCESS;

			  /* Setting mandatory output values */
			  *nreturn_vals = 1;
			  *return_vals  = values;

			  values[0].type = GIMP_PDB_STATUS;
			  values[0].data.d_status = status;

			  /* Getting run_mode - we won't display a dialog if 
			   * we are in NONINTERACTIVE mode */
			  GimpRunMode run_mode = (GimpRunMode)param[0].data.d_int32;

			  /*  Get the specified drawable  */
			  XDrawable drawable(::gimp_drawable_get (param[2].data.d_drawable));

			  switch (run_mode)
				{
				case GIMP_RUN_INTERACTIVE:
				  {
				  std::string pluginname("plug-in-");
				  pluginname.append(this->name());
				  /* Get options last values if needed */
				  ::gimp_get_data (pluginname.c_str(), (this->prefs()));
				  /* Display the dialog */
				  if (! this->dialog (drawable))
					return;
				  break;
				  }
				case GIMP_RUN_NONINTERACTIVE:
					{
					status = GIMP_PDB_CALLING_ERROR;
				  	break;
					}
				case GIMP_RUN_WITH_LAST_VALS:
					{
					std::string pluginname("plug-in-");
					pluginname.append(this->name());
					/* Get options last values if needed */
					::gimp_get_data (pluginname.c_str(),(this->prefs()));
				    break;
				  	}
				default: break;
				}
			  
			  this->run(drawable);

			  ::gimp_displays_flush ();
			  drawable.detach();

			 /*  Finally, set options in the core  */
			 if (run_mode == GIMP_RUN_INTERACTIVE)
			 	{
			 	std::string pluginname("plug-in-");
					pluginname.append(this->name());
				::gimp_set_data (pluginname.c_str(), this->prefs(), sizeof (T));
				}
			 }
		
		
	};

#endif

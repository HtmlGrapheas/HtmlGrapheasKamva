#ifndef WX_AGG_LION_WINDOW_H
#define WX_AGG_LION_WINDOW_H

#include "AGGWindow.h"

#include "agg_path_storage.h"
#include "agg_color_rgba.h"

#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"

namespace GUI {
    
    /// An example of an application-specific subclass of AGGWindow,
    /// in this case, a window that draws the lion example from AGG.
    class LionWindow: public AGGWindow {
        public:
        
        /// Initialize a lion window.
        /// Defaults are taken from AGGWindow::AGGWindow(), see that 
        /// documentation for more information.
        LionWindow(wxWindow* parent, wxWindowID id = wxID_ANY, 
                   const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, 
                   long style = wxTAB_TRAVERSAL);
        
        /// Clean up any resources held.
        virtual ~LionWindow();
        
        /// Draw the bitmap with the lion.
        virtual void draw();

        protected:
        agg::path_storage paths;                    ///< Storage for the lion paths
        agg::srgba8 colors[100];                     ///< Path colors
        unsigned pathIndices[100];                  ///< Path indices
        unsigned numPaths;                          ///< Number of paths
        unsigned width, height;                     ///< Size of the lion paths
        
        agg::rasterizer_scanline_aa<> rasterizer;   ///< Scanline rasterizer
        agg::scanline_p8  scanline;                 ///< Scanline container
    };
}


#endif
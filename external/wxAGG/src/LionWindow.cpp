
#include "LionWindow.h"

unsigned parse_lion(agg::path_storage& path, agg::rgba8* colors, unsigned* path_idx);

#include "agg_renderer_base.h"
#include "agg_renderer_primitives.h"
#include "agg_renderer_scanline.h"
#include "agg_bounding_rect.h"
#include "agg_trans_affine.h"

namespace GUI {
    
    LionWindow::LionWindow(wxWindow* parent, wxWindowID id, 
                           const wxPoint& pos, const wxSize& size, long style):
        AGGWindow(parent, id, pos, size, style) {
            
        // Load the lion
        numPaths = parse_lion(paths, colors, pathIndices);
        assert(numPaths < 100);     // We are using fixed-sized arrays, boo.

        // Find its bounding box
        unsigned bBox[4];
        agg::pod_array_adaptor<unsigned> indicesContainer(pathIndices, 100);
        agg::bounding_rect(paths, indicesContainer, 0, numPaths, &bBox[0], &bBox[1], &bBox[2], &bBox[3]);
        assert(bBox[0] <= bBox[2] && bBox[1] <= bBox[3]);
        width = bBox[2] - bBox[0];
        height = bBox[3] - bBox[1];
        
        // Set the colors to be partially transparent for kicks
        for (unsigned i = 0; i < numPaths; ++i) 
            colors[i].a = 200;
    }
            
    LionWindow::~LionWindow() {
    }
    
    void LionWindow::draw() {
        
        /// The AGG base renderer
        typedef agg::renderer_base<PixelFormat::AGGType> RendererBase;
        
        /// The AGG primitives renderer
        typedef agg::renderer_primitives<RendererBase> RendererPrimitives;
        
        /// The AGG solid renderer
        typedef agg::renderer_scanline_aa_solid<RendererBase> RendererSolid;
        
        PixelFormat::AGGType pixf(rBuf);
        RendererBase rbase(pixf);
        RendererPrimitives rprim(rbase);
        RendererSolid rsolid(rbase);
        
        // Draw a rectangle against a white background
        rbase.clear(PixelFormat::AGGType::color_type(255, 255, 255));
        rprim.fill_color(PixelFormat::AGGType::color_type(64, 64, 200));
        rprim.solid_rectangle(rBuf.width()/4, rBuf.height()/4, 3*rBuf.width()/4, 3*rBuf.height()/4);
        
        // Center and render the lion
        agg::trans_affine mat;
    
        if (rBuf.width() >= width ) 
            mat *= agg::trans_affine_translation(rBuf.width()/2 - width/2, 0);
        
        if (rBuf.height() >= height)
            mat *= agg::trans_affine_translation(0, rBuf.height()/2 - height/2);
        
        agg::conv_transform<agg::path_storage, agg::trans_affine> transformedPaths(paths, mat);
        agg::render_all_paths(rasterizer, scanline, rsolid, transformedPaths, colors, pathIndices, numPaths);
    }
}

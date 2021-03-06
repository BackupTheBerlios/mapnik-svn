/*****************************************************************************
 * 
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2006 Artem Pavlenko
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *****************************************************************************/
//$Id$

// stl
#include <iostream>
// boost
#include <boost/utility.hpp>
// agg
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgba.h"
#include "agg_path_storage.h"
#include "agg_span_allocator.h"
#include "agg_span_pattern_rgba.h"
#include "agg_image_accessors.h"
#include "agg_conv_stroke.h"
#include "agg_conv_dash.h"
#include "agg_conv_contour.h"
#include "agg_vcgen_stroke.h"
#include "agg_conv_adaptor_vcgen.h"
#include "agg_conv_smooth_poly1.h"
#include "agg_conv_marker.h"
#include "agg_arrowhead.h"
#include "agg_vcgen_markers_term.h"
#include "agg_renderer_outline_aa.h"
#include "agg_rasterizer_outline_aa.h"
#include "agg_rasterizer_outline.h"
#include "agg_renderer_outline_image.h"
#include "agg_span_allocator.h"
#include "agg_span_pattern_rgba.h"
#include "agg_renderer_scanline.h"
#include "agg_pattern_filters_rgba.h"
#include "agg_renderer_outline_image.h"
// mapnik
#include <mapnik/image_util.hpp>
#include <mapnik/agg_renderer.hpp>
#include <mapnik/unicode.hpp>
#include <mapnik/placement_finder.hpp>

namespace mapnik 
{
   class pattern_source : private boost::noncopyable
   {
      public:
         pattern_source(ImageData32 const& pattern)
            : pattern_(pattern) {}
	
         unsigned int width() const
         {
            return pattern_.width();
         }
         unsigned int height() const
         {
            return pattern_.height();
         }
         agg::rgba8 pixel(int x, int y) const
         {
            unsigned c = pattern_(x,y);
            return agg::rgba8(c & 0xff, 
                              (c >> 8) & 0xff, 
                              (c >> 16) & 0xff,
                              (c >> 24) & 0xff);
         }
      private:
         ImageData32 const& pattern_;
   };

   template <typename T>
   agg_renderer<T>::agg_renderer(Map const& m, T & pixmap, unsigned offset_x, unsigned offset_y)
      : feature_style_processor<agg_renderer>(m),
        pixmap_(pixmap),
        t_(m.getWidth(),m.getHeight(),m.getCurrentExtent(),offset_x,offset_y),
        finder_(Envelope<double>(0 ,0, m.getWidth(), m.getHeight()), 64),
        point_detector_(Envelope<double>(-64 ,-64, m.getWidth() + 64 ,m.getHeight() + 64)),
        tr_(new transcoder("utf-8"))
   {
      Color const& bg=m.getBackground();
      pixmap_.setBackground(bg);
#ifdef MAPNIK_DEBUG
      std::clog << "scale=" << m.scale() << "\n";
#endif
   }
    
   template <typename T>
   void agg_renderer<T>::start_map_processing(Map const& map)
   {
#ifdef MAPNIK_DEBUG
      std::clog << "start map processing bbox=" 
                << map.getCurrentExtent() << "\n";
#endif
   }

   template <typename T>
   void agg_renderer<T>::end_map_processing(Map const& )
   {
#ifdef MAPNIK_DEBUG
      std::clog << "end map processing\n";
#endif
   }
    
   template <typename T>
   void agg_renderer<T>::start_layer_processing(Layer const& lay)
   {
#ifdef MAPNIK_DEBUG
      std::clog << "start layer processing : " << lay.name()  << "\n";
      std::clog << "datasource = " << lay.datasource().get() << "\n";
#endif 
      datasource_ptr ds = lay.datasource();
      if (ds)
      {
         layer_descriptor desc = ds->get_descriptor();
         tr_ = boost::shared_ptr<transcoder>(new transcoder(desc.get_encoding()));
      }
   }
    
   template <typename T>
   void agg_renderer<T>::end_layer_processing(Layer const&)
   {
#ifdef MAPNIK_DEBUG
      std::clog << "end layer processing\n";
#endif
   }
    
   template <typename T>	
   void agg_renderer<T>::process(polygon_symbolizer const& sym,
                                 Feature const& feature,
                                 proj_transform const& prj_trans)
   {
      typedef  coord_transform2<CoordTransform,geometry_type> path_type;
      typedef agg::renderer_base<agg::pixfmt_rgba32> ren_base;    
      typedef agg::renderer_scanline_aa_solid<ren_base> renderer;
	    
      Color const& fill_  = sym.get_fill();
	    
      geometry_ptr const& geom=feature.get_geometry();
      if (geom && geom->num_points() > 2) 
      {
         unsigned width = pixmap_.width();
         unsigned height = pixmap_.height();
         path_type path(t_,*geom,prj_trans);
         agg::row_accessor<agg::int8u> buf(pixmap_.raw_data(),width,height,width * 4);
         agg::pixfmt_rgba32 pixf(buf);
         ren_base renb(pixf);	    
		
         unsigned r=fill_.red();
         unsigned g=fill_.green();
         unsigned b=fill_.blue();
         renderer ren(renb);
            
         agg::rasterizer_scanline_aa<> ras;
         agg::scanline_u8 sl;
         ras.clip_box(0,0,width,height);
         ras.add_path(path);
         ren.color(agg::rgba8(r, g, b, int(255 * sym.get_opacity())));
         agg::render_scanlines(ras, sl, ren);
      }
   }

   template <typename T>
   void agg_renderer<T>::process(line_symbolizer const& sym,
                                 Feature const& feature,
                                 proj_transform const& prj_trans)
   {   
      typedef agg::renderer_base<agg::pixfmt_rgba32> ren_base; 
      typedef coord_transform2<CoordTransform,geometry_type> path_type;
      typedef agg::renderer_outline_aa<ren_base> renderer_oaa;
      typedef agg::rasterizer_outline_aa<renderer_oaa> rasterizer_outline_aa;
      typedef agg::renderer_scanline_aa_solid<ren_base> renderer;
	    
      geometry_ptr const& geom=feature.get_geometry();
      if (geom && geom->num_points() > 1)
      {
         path_type path(t_,*geom,prj_trans);
         agg::row_accessor<agg::int8u> buf(pixmap_.raw_data(),
                                           pixmap_.width(),
                                           pixmap_.height(),
                                           pixmap_.width()*4);
         agg::pixfmt_rgba32 pixf(buf);
         ren_base renb(pixf);	    
            
         mapnik::stroke const&  stroke_ = sym.get_stroke();
		
         Color const& col = stroke_.get_color();
         unsigned r=col.red();
         unsigned g=col.green();
         unsigned b=col.blue();	    
		
         if (stroke_.has_dash())
         {
            renderer ren(renb);	
            agg::rasterizer_scanline_aa<> ras;
            agg::scanline_u8 sl;
            agg::conv_dash<path_type> dash(path);
            dash_array const& d = stroke_.get_dash_array();
            dash_array::const_iterator itr = d.begin();
            dash_array::const_iterator end = d.end();
            while (itr != end)
            {
               dash.add_dash(itr->first, itr->second);
               ++itr;
            }
            agg::conv_stroke<agg::conv_dash<path_type > > stroke(dash);
		    
            line_join_e join=stroke_.get_line_join();
            if ( join == MITER_JOIN)
               stroke.generator().line_join(agg::miter_join);
            else if( join == MITER_REVERT_JOIN) 
               stroke.generator().line_join(agg::miter_join);
            else if( join == ROUND_JOIN) 
               stroke.generator().line_join(agg::round_join);
            else
               stroke.generator().line_join(agg::bevel_join);
		    
            line_cap_e cap=stroke_.get_line_cap();
            if (cap == BUTT_CAP)    
               stroke.generator().line_cap(agg::butt_cap);
            else if (cap == SQUARE_CAP)
               stroke.generator().line_cap(agg::square_cap);
            else 
               stroke.generator().line_cap(agg::round_cap);
		    
            stroke.generator().miter_limit(4.0);
            stroke.generator().width(stroke_.get_width());
		    
            ras.clip_box(0,0,pixmap_.width(),pixmap_.height());
            ras.add_path(stroke);
            ren.color(agg::rgba8(r, g, b, int(255*stroke_.get_opacity())));
            agg::render_scanlines(ras, sl, ren);
         }
            
         //else if (stroke_.get_width() <= 1.0)
         //{
         //   agg::line_profile_aa prof;
         //   prof.width(stroke_.get_width());
         //   renderer_oaa ren_oaa(renb, prof);
         //   rasterizer_outline_aa ras_oaa(ren_oaa);
                
         //    ren_oaa.color(agg::rgba8(r, g, b, int(255*stroke_.get_opacity())));
         //    ren_oaa.clip_box(0,0,pixmap_.width(),pixmap_.height());
         //    ras_oaa.add_path(path);		
         //    }
         else 
         {
            renderer ren(renb);	
            agg::rasterizer_scanline_aa<> ras;
            agg::scanline_p8 sl;
            agg::conv_stroke<path_type>  stroke(path);
		    
            line_join_e join=stroke_.get_line_join();
            if ( join == MITER_JOIN)
               stroke.generator().line_join(agg::miter_join);
            else if( join == MITER_REVERT_JOIN) 
               stroke.generator().line_join(agg::miter_join);
            else if( join == ROUND_JOIN) 
               stroke.generator().line_join(agg::round_join);
            else
               stroke.generator().line_join(agg::bevel_join);
		    
            line_cap_e cap=stroke_.get_line_cap();
            if (cap == BUTT_CAP)    
               stroke.generator().line_cap(agg::butt_cap);
            else if (cap == SQUARE_CAP)
               stroke.generator().line_cap(agg::square_cap);
            else 
               stroke.generator().line_cap(agg::round_cap);
		
            stroke.generator().miter_limit(4.0);
            stroke.generator().width(stroke_.get_width());
		    
            ras.clip_box(0,0,pixmap_.width(),pixmap_.height());
            ras.add_path(stroke);
            ren.color(agg::rgba8(r, g, b, int(255*stroke_.get_opacity())));
            agg::render_scanlines(ras, sl, ren);
         }
      }
   }

   template <typename T>
   void agg_renderer<T>::process(point_symbolizer const& sym,
                                 Feature const& feature,
                                 proj_transform const& prj_trans)
   {
      geometry_ptr const& geom=feature.get_geometry();
      if (geom)
      {
         double x;
         double y;
         double z=0;
         boost::shared_ptr<ImageData32> const& data = sym.get_data();
         if ( data )
         {
            geom->label_position(&x,&y);
            prj_trans.backward(x,y,z);
            t_.forward(&x,&y);
            int w = data->width();
            int h = data->height();
            int px=int(floor(x - 0.5 * w));
            int py=int(floor(y - 0.5 * h));
                
            if (sym.get_allow_overlap() || 
                point_detector_.has_placement(Envelope<double>(floor(x - 0.5 * w),
                                                               floor(y - 0.5 * h),
                                                               ceil (x + 0.5 * w),
                                                               ceil (y + 0.5 * h))))
            {    
               pixmap_.set_rectangle_alpha(px,py,*data);
            }
         }
      }
   }
    
   template <typename T>
   void  agg_renderer<T>::process(shield_symbolizer const& sym,
                                  Feature const& feature,
                                  proj_transform const& prj_trans)
   {
      geometry_ptr const& geom=feature.get_geometry();
      if (geom && geom->num_points() > 0)
      {
         //std::wstring text = to_unicode(feature[sym.get_name()].to_string());
         std::string str = feature[sym.get_name()].to_string();
         boost::shared_ptr<ImageData32> const& data = sym.get_data();
           
         if (str.length() > 0 && data)
         {
            std::wstring text = tr_->transcode(str);
            face_ptr face = font_manager_.get_face(sym.get_face_name());
            if (face)
            {
               int w = data->width();
               int h = data->height();

               text_renderer<mapnik::Image32> ren(pixmap_,face);
               ren.set_pixel_size(sym.get_text_size());
               ren.set_fill(sym.get_fill());

               string_info info;
               ren.get_string_info(text, &info);
                    
               placement text_placement(&info, &t_, &prj_trans, geom, std::pair<double, double>(w, h) );
               text_placement.avoid_edges = sym.get_avoid_edges();
                  
               bool found = finder_.find_placements(&text_placement);
               if (!found) {
                  return;
               }
                    

               for (unsigned int ii = 0; ii < text_placement.placements.size(); ++ ii)
               {
                  double x = text_placement.placements[ii].starting_x;
                  double y = text_placement.placements[ii].starting_y;

                  int px=int(floor(x - 0.5 * w));
                  int py=int(floor(y - 0.5 * h));
                        
                  pixmap_.set_rectangle_alpha(px,py,*data);
                        
                  Envelope<double> dim = ren.prepare_glyphs(&text_placement.placements[ii].path);
                        
                  ren.render(x,y);
               }
            }
         }
      }
   }
    
   template <typename T>
   void  agg_renderer<T>::process(line_pattern_symbolizer const& sym,
                                  Feature const& feature,
                                  proj_transform const& prj_trans)
   {
      typedef  coord_transform2<CoordTransform,geometry_type> path_type;
      typedef agg::line_image_pattern<agg::pattern_filter_bilinear_rgba8> pattern_type;
      typedef agg::renderer_base<agg::pixfmt_rgba32> renderer_base;
      typedef agg::renderer_outline_image<renderer_base, pattern_type> renderer_type;
      typedef agg::rasterizer_outline_aa<renderer_type> rasterizer_type;

      geometry_ptr const& geom=feature.get_geometry();
      if (geom && geom->num_points() > 1)
      {
         unsigned width = pixmap_.width();
         unsigned height = pixmap_.height();
         ImageData32 const& pat = sym.get_pattern();
         path_type path(t_,*geom,prj_trans);
         agg::row_accessor<agg::int8u> buf(pixmap_.raw_data(), width, height,width*4);
         agg::pixfmt_rgba32 pixf(buf);
         renderer_base ren_base(pixf);  
         agg::pattern_filter_bilinear_rgba8 filter; 
         pattern_source source(pat);
         pattern_type pattern (filter,source);
         renderer_type ren(ren_base, pattern);
         ren.clip_box(0,0,width,height);
         rasterizer_type ras(ren);	    
         ras.add_path(path);    
      }
   }
    
   template <typename T>
   void agg_renderer<T>::process(polygon_pattern_symbolizer const& sym,
                                 Feature const& feature,
                                 proj_transform const& prj_trans)
   {
      typedef coord_transform2<CoordTransform,geometry_type> path_type;
      typedef agg::renderer_base<agg::pixfmt_rgba32> ren_base; 
      typedef agg::wrap_mode_repeat wrap_x_type;
      typedef agg::wrap_mode_repeat wrap_y_type;
      typedef agg::image_accessor_wrap<agg::pixfmt_rgba32, 
         wrap_x_type,
         wrap_y_type> img_source_type;
	
      typedef agg::span_pattern_rgba<img_source_type> span_gen_type;
	
      typedef agg::renderer_scanline_aa<ren_base, 
         agg::span_allocator<agg::rgba8>,
         span_gen_type> renderer_type;  
      geometry_ptr const& geom=feature.get_geometry();
      if (geom && geom->num_points() > 2)
      {
         ImageData32 const& pattern = sym.get_pattern();
	    
         unsigned width = pixmap_.width();
         unsigned height = pixmap_.height();
         path_type path(t_,*geom,prj_trans);
	    
         agg::row_accessor<agg::int8u> buf(pixmap_.raw_data(),width,height,width * 4);
         agg::pixfmt_rgba32 pixf(buf);
         ren_base renb(pixf);
	
         unsigned w=pattern.width();
         unsigned h=pattern.height();
         agg::row_accessor<agg::int8u> pattern_rbuf((agg::int8u*)pattern.getBytes(),w,h,w*4);  
	    
         double x0,y0;
         path.vertex(&x0,&y0);
         path.rewind(0);
	
         unsigned offset_x = unsigned(width - x0);
         unsigned offset_y = unsigned(height - y0);
            
         agg::span_allocator<agg::rgba8> sa;
         agg::pixfmt_rgba32 pixf_pattern(pattern_rbuf);
         img_source_type img_src(pixf_pattern);
         span_gen_type sg(img_src, offset_x, offset_y);
         renderer_type rp(renb,sa, sg);
            
         agg::rasterizer_scanline_aa<> ras;
         agg::scanline_u8 sl;
         ras.clip_box(0,0,width,height);
         ras.add_path(path);
         agg::render_scanlines(ras, sl, rp);   
      }
   }

   template <typename T>
   void agg_renderer<T>::process(raster_symbolizer const&,
                                 Feature const& feature,
                                 proj_transform const& prj_trans)
   {
      // TODO -- at the moment raster_symbolizer is an empty class 
      // used for type dispatching, but we can have some fancy raster
      // processing in a future (filters??). Just copy raster into pixmap for now.
      raster_ptr const& raster=feature.get_raster();
      if (raster)
      {
         Envelope<double> ext=t_.forward(raster->ext_);
         ImageData32 target((int)(ext.width() + 0.5),(int)(ext.height() + 0.5));
         scale_image<ImageData32>(target,raster->data_);
         pixmap_.set_rectangle(int(ext.minx()),int(ext.miny()),target);
      }
   }
    
   template <typename T>
   void agg_renderer<T>::process(text_symbolizer const& sym,
                                 Feature const& feature,
                                 proj_transform const& prj_trans)
   {
      geometry_ptr const& geom=feature.get_geometry();
       
      if (geom && geom->num_points() > 0)
      {
         std::string str = feature[sym.get_name()].to_string();
         if ( str.length() > 0 )
         {
            std::wstring text = tr_->transcode(str);
            Color const& fill  = sym.get_fill();
            face_ptr face = font_manager_.get_face(sym.get_face_name());
            if (face)
            {
               text_renderer<mapnik::Image32> ren(pixmap_,face);
               ren.set_pixel_size(sym.get_text_size());
               ren.set_fill(fill);
               ren.set_halo_fill(sym.get_halo_fill());
               ren.set_halo_radius(sym.get_halo_radius());
                    
               string_info info;
                    
               ren.get_string_info(text, &info);
                    
               placement text_placement(&info, &t_, &prj_trans, geom, 
                                        sym.get_displacement(),
                                        sym.get_label_placement());
               
               text_placement.text_ratio = sym.get_text_ratio();
               text_placement.wrap_width = sym.get_wrap_width();
               text_placement.label_spacing = sym.get_label_spacing();
               text_placement.label_position_tolerance = sym.get_label_position_tolerance();
               text_placement.force_odd_labels = sym.get_force_odd_labels();
               text_placement.max_char_angle_delta = sym.get_max_char_angle_delta();
               text_placement.avoid_edges = sym.get_avoid_edges();
                  
               if ( !finder_.find_placements(&text_placement)) return;
                    
               for (unsigned int ii = 0; ii < text_placement.placements.size(); ++ ii)
               {
                  double x = text_placement.placements[ii].starting_x;
                  double y = text_placement.placements[ii].starting_y;
                  
                  Envelope<double> dim = ren.prepare_glyphs(&text_placement.placements[ii].path);
                  
                  //Envelope<double> text_box(x + dim.minx() ,y - dim.maxy(), x + dim.maxx(),y - dim.miny());
		        
                  //if (sym.get_halo_radius() > 0)
                  //{
                  //   text_box.width(text_box.width() + sym.get_halo_radius()*2);
                  //   text_box.height(text_box.height() + sym.get_halo_radius()*2);
                  //}
                  ren.render(x,y);
               }
            }
         }  
      }
   }   
    
   template class agg_renderer<Image32>;
}

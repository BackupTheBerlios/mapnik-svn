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
//$Id: map.hpp 39 2005-04-10 20:39:53Z pavlenko $

#ifndef MAP_HPP
#define MAP_HPP

#include <mapnik/feature_type_style.hpp>
#include <mapnik/datasource.hpp>
#include <mapnik/layer.hpp>

namespace mapnik
{
    class MAPNIK_DECL Map
    {	
        static const unsigned MIN_MAPSIZE=16;
        static const unsigned MAX_MAPSIZE=2048;
        unsigned width_;
        unsigned height_;
        std::string  srs_;
        Color background_;
        std::map<std::string,feature_type_style> styles_;
        std::vector<Layer> layers_;
        Envelope<double> currentExtent_;
        
    public:
        typedef std::map<std::string,feature_type_style>::const_iterator const_style_iterator;
        typedef std::map<std::string,feature_type_style>::iterator style_iterator;
        
        Map();
        Map(int width, int height, std::string const& srs="+proj=latlong +datum=WGS84");
        Map(const Map& rhs);
        Map& operator=(const Map& rhs);
        const_style_iterator begin_styles() const;
        const_style_iterator end_styles() const;
        style_iterator begin_styles();
        style_iterator end_styles();
        bool insert_style(std::string const& name,feature_type_style const& style);
        void remove_style(const std::string& name);
        feature_type_style const& find_style(std::string const& name) const;
        size_t layerCount() const;
        void addLayer(const Layer& l);
        const Layer& getLayer(size_t index) const;
        Layer& getLayer(size_t index);
        void removeLayer(size_t index);
        std::vector<Layer> const& layers() const;
        std::vector<Layer> & layers();
        void remove_all();        
        unsigned getWidth() const;
        unsigned getHeight() const;
        void setWidth(unsigned width);
        void setHeight(unsigned height);
        void resize(unsigned width,unsigned height);
        std::string const& srs() const;
        void set_srs(std::string const& srs);
        void setBackground(const Color& c);
        const Color& getBackground() const;
        void zoom(double zoom);
        void zoomToBox(const Envelope<double>& box);
        void zoom_all();
        void pan(int x,int y);
        void pan_and_zoom(int x,int y,double zoom);
        const Envelope<double>& getCurrentExtent() const;
        double scale() const;
        CoordTransform view_transform() const;
        featureset_ptr query_point(unsigned index, double x, double y) const;
        featureset_ptr query_map_point(unsigned index, double x, double y) const;
        ~Map();
    private:
        void fixAspectRatio();
    };
}

#endif //MAP_HPP

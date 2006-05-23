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

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include "feature_type_style.hpp"

namespace mapnik
{
    class Layer;

    class MAPNIK_DECL Map
    {
        friend class boost::serialization::access;
        template <typename Archive>
        void serialize(Archive & ar, const unsigned int /*version*/)
        {
            ar  & boost::serialization::make_nvp("width",width_) 
                & boost::serialization::make_nvp("height",height_) 
                & boost::serialization::make_nvp("srid",srid_)
                & boost::serialization::make_nvp("layers",layers_);
        }
	
        static const unsigned MIN_MAPSIZE=16;
        static const unsigned MAX_MAPSIZE=2048;
        unsigned width_;
        unsigned height_;
        int srid_;
        Color background_;
        std::map<std::string,feature_type_style> styles_;
        std::vector<Layer> layers_;
        Envelope<double> currentExtent_;
        
    public:
        Map();
        Map(int width,int height,int srid=-1);
        Map(const Map& rhs);
        Map& operator=(const Map& rhs);        
        bool insert_style(std::string const& name,feature_type_style const& style);
        void remove_style(const std::string& name);
        feature_type_style const& find_style(std::string const& name) const;
        size_t layerCount() const;
        void addLayer(const Layer& l);
        const Layer& getLayer(size_t index) const;
        Layer& getLayer(size_t index);
        void removeLayer(size_t index);
        std::vector<Layer> const& layers() const;
        unsigned getWidth() const;
        unsigned getHeight() const;
        void setWidth(unsigned width);
        void setHeight(unsigned height);
        void resize(unsigned width,unsigned height);
        int srid() const;
        void setBackground(const Color& c);
        const Color& getBackground() const;
        void zoom(double zoom);
        void zoomToBox(const Envelope<double>& box);
        void zoom_all();
        void pan(int x,int y);
        void pan_and_zoom(int x,int y,double zoom);
        const Envelope<double>& getCurrentExtent() const;
        double scale() const;
        virtual ~Map();
    private:
        void fixAspectRatio();
    };
}

BOOST_CLASS_IMPLEMENTATION(std::vector<mapnik::Layer>, boost::serialization::object_serializable)
BOOST_CLASS_TRACKING(std::vector<mapnik::Layer>, boost::serialization::track_never)

BOOST_CLASS_IMPLEMENTATION(mapnik::Map, boost::serialization::object_serializable)
BOOST_CLASS_TRACKING(mapnik::Map, boost::serialization::track_never)

#endif //MAP_HPP

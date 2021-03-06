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
//$Id: image_symbolizer.hpp 39 2005-04-10 20:39:53Z pavlenko $

#ifndef POINT_SYMBOLIZER_HPP
#define POINT_SYMBOLIZER_HPP

#include <boost/shared_ptr.hpp>
#include <mapnik/graphics.hpp> 

namespace mapnik 
{   
    struct MAPNIK_DECL point_symbolizer
    {	
        explicit point_symbolizer();
        point_symbolizer(std::string const& file,
                         std::string const& type,
                         unsigned width,unsigned height);
        point_symbolizer(point_symbolizer const& rhs);
        void set_data (boost::shared_ptr<ImageData32> symbol);
        boost::shared_ptr<ImageData32> const& get_data() const;        
        void set_allow_overlap(bool overlap);
        bool get_allow_overlap() const;
        
    private:
        boost::shared_ptr<ImageData32> symbol_;
        bool overlap_;
    };
}

#endif // POINT_SYMBOLIZER_HPP

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

#ifndef POLYGON_PATTERN_SYMBOLIZER_HPP
#define POLYGON_PATTERN_SYMBOLIZER_HPP

#include <boost/shared_ptr.hpp>
#include <mapnik/graphics.hpp> 

namespace mapnik
{
    struct MAPNIK_DECL polygon_pattern_symbolizer
    {
	
        polygon_pattern_symbolizer(std::string const& file,
                                   std::string const& type,
                                   unsigned width,unsigned height);
        
        polygon_pattern_symbolizer(polygon_pattern_symbolizer const& rhs);
        
        ImageData32 const& get_pattern() const;
    private:
        boost::shared_ptr<ImageData32> pattern_;
    };
}

#endif //POLYGON_PATTERN_SYMBOLIZER_HPP

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
//$Id: image_reader.cpp 17 2005-03-08 23:58:43Z pavlenko $

#include <mapnik/factory.hpp>
#include <mapnik/image_reader.hpp>

namespace mapnik
{  
    typedef factory<ImageReader,std::string, 
                    ImageReader* (*)(const std::string&)>  ImageReaderFactory;
    
    
    bool register_image_reader(const std::string& type,ImageReader* (* fun)(const std::string&))
    {
        return ImageReaderFactory::instance()->register_product(type,fun);
    }
    
    ImageReader* get_image_reader(const std::string& type,const std::string& file) 
    {
        return ImageReaderFactory::instance()->create_object(type,file);
    }
}

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
//$Id: params.hpp 39 2005-04-10 20:39:53Z pavlenko $

#ifndef PARAMS_HPP
#define PARAMS_HPP

#include <string>
#include <map>

namespace mapnik
{

    typedef std::pair<const std::string,std::string> parameter;
    typedef std::map<const std::string,std::string> param_map;

    class parameters : public param_map
    {
    public:

        parameters() {}
        const std::string get(std::string const& key) const
        {
            param_map::const_iterator itr=find(key);
            if (itr != end())
            {
                return itr->second;
            }
            return std::string();
        }
        
        param_map::const_iterator iterator(std::string const& key) const
        {
	    return find(key);
        }
        
        param_map::iterator iterator(std::string const& key)
        {
	    return find(key);
        }
        
    };
}

#endif //PARAMS_HPP

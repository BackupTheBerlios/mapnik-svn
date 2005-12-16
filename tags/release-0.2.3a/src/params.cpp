/* This file is part of Mapnik (c++ mapping toolkit)
 * Copyright (C) 2005 Artem Pavlenko
 *
 * Mapnik is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

//$Id: params.cpp 17 2005-03-08 23:58:43Z pavlenko $

#include <string>
#include <map>
#include "params.hpp"

namespace mapnik
{

    const std::string Parameters::get(const std::string& name) const
    {
        std::map<std::string,std::string>::const_iterator itr;
        itr=data_.find(name);
        if (itr!=data_.end())
            return itr->second;
        return std::string();
    }

    void Parameters::add(const Parameter& param)
    {
        data_.insert(param);
    }

    void Parameters::add(const std::string& name,const std::string& value)
    {
        Parameter param(name,value);
        data_.insert(param);
    }

    std::map<std::string,std::string>::const_iterator Parameters::begin() const
    {
        return data_.begin();
    }

    std::map<std::string,std::string>::const_iterator Parameters::end() const
    {
        return data_.end();
    }

    Parameters::~Parameters() {}
}
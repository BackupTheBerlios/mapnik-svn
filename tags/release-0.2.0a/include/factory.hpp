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

//$Id: factory.hpp 39 2005-04-10 20:39:53Z pavlenko $

#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <stdexcept>
#include <map>
#include "utils.hpp"

namespace mapnik
{
    template <typename key_type,
	      typename product_type>
    class default_factory_error
    {
    public:
	struct factory_exception : public std::exception
	{
	    const char* what() const throw()
	    {
		return "uknown object type";
	    }
	};
	static product_type* on_unknown_type(const key_type&)
	{
	    return 0;
	}
    };

    template
    <
        typename product_type,
        typename key_type,
        typename product_creator=product_type* (*)(),
        template <typename,typename> class factory_error_policy=default_factory_error
    >
    class factory : public singleton<factory <product_type,key_type,product_creator,factory_error_policy> >,
					     factory_error_policy <key_type,product_type>
    {
    private:
	typedef std::map<key_type,product_creator> product_map;
	product_map map_;
    public:

	bool register_product(const key_type& key,product_creator creator)
	{
	    return map_.insert(typename product_map::value_type(key,creator)).second;
	}

	bool unregister_product(const key_type& key)
	{
	    return map_.erase(key)==1;
	}

	product_type* create_object(const key_type& key,const std::string& file)
	{
	    typename product_map::const_iterator pos=map_.find(key);
	    if (pos!=map_.end())
	    {
		return (pos->second)(file);
	    }
	    return on_unknown_type(key);
	}
    };
}
#endif                                            //FACTORY_HPP

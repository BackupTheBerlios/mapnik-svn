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

#ifndef QUERY_HPP
#define QUERY_HPP

#include <set>
#include <limits>
#include "filter.hpp"
#include "envelope.hpp"
#include "feature.hpp"

namespace mapnik
{
    class query 
    {
    private:
	Envelope<double> bbox_;
	unsigned width_;
	unsigned height_;
	filter<Feature>* filter_;
	std::set<std::string> names_;
    public:
	query(unsigned width,unsigned height) 
	    : bbox_(std::numeric_limits<double>::min(),
		    std::numeric_limits<double>::min(),
		    std::numeric_limits<double>::max(),
		    std::numeric_limits<double>::max()),
	      width_(width),
	      height_(height),
	      filter_(new all_filter<Feature>)
	{}
	
	query(const Envelope<double>& bbox,unsigned width,unsigned height)
	    : bbox_(bbox),
	      width_(width),
	      height_(height),
	      filter_(new all_filter<Feature>)
	{}
	
	query(const Envelope<double>& bbox,unsigned width,unsigned height,const filter<Feature>& f)
	    : bbox_(bbox),
	      width_(width),
	      height_(height),
	      filter_(f.clone())
	{}
	
	query(const query& other)
	    : bbox_(other.bbox_),
	      width_(other.width_),
	      height_(other.height_),
	      filter_(other.filter_->clone())
	{}
	
	query& operator=(const query& other)
	{
	    filter<Feature>* tmp=other.filter_->clone();
	    delete filter_;
	    filter_=tmp;
	    bbox_=other.bbox_;
	    width_=other.width_;
	    height_=other.height_;
	    names_=other.names_;
	    return *this;
	}
	
	const filter<Feature>* get_filter() const
	{
	    return  filter_;
	}
	
	const Envelope<double>& get_bbox() const
	{
	    return bbox_;
	}

	unsigned get_width() const
	{
	    return width_;
	}

	unsigned get_height() const
	{
	    return height_;
	}

	void set_filter(const filter<Feature>& f)
	{
	    filter<Feature>* tmp=f.clone();
	    delete filter_;
	    filter_=tmp;
	}
        
	void add_property_name(const std::string& name)
	{
	    names_.insert(name);
	} 
	
	const std::set<std::string>& property_names() const
	{
	    return names_;
	}
	
	~query() 
	{
	    delete filter_;
	}
    };
}


#endif //QUERY_HPP

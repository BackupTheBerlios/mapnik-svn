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

//$Id: shape_index_featureset.cc 36 2005-04-05 14:32:18Z pavlenko $

#include "shape_index_featureset.hpp"
#include "feature_factory.hpp"

template <typename filterT>
shape_index_featureset<filterT>::shape_index_featureset(const filterT& filter,
							const std::string& shape_file,
							const std::set<std::string>& attribute_names)
    : filter_(filter),
      shape_type_(0),
      shape_(shape_file),
      count_(0)

{
    shape_.shp().skip(100);
    std::string indexname(shape_file + ".index");
    std::ifstream file(indexname.c_str(),std::ios::in|std::ios::binary);
    if (file)
    {
        shp_index<filterT>::query(filter,file,ids_);
        file.close();
    }
    std::clog<< "query size=" << ids_.size() << "\n";
    itr_ = ids_.begin();

    // deal with attributes
    std::set<std::string>::const_iterator pos=attribute_names.begin();
    while (pos!=attribute_names.end())
    {
	for (int i=0;i<shape_.dbf().num_fields();++i)
	{
	    if (shape_.dbf().descriptor(i).name_ == *pos)
	    {
		attr_ids_.push_back(i);
		break;
	    }
	}
	++pos;
    }
}

template <typename filterT>
feature_ptr shape_index_featureset<filterT>::next()
{   
    
    if (itr_!=ids_.end())
    {
        int pos=*itr_++;
	shape_.move_to(pos);
        int type=shape_.type();
	feature_ptr feature(feature_factory::create(shape_.id_));
        
        if (type==shape_io::shape_point)
        {
            double x=shape_.shp().read_double();
	    double y=shape_.shp().read_double();	    
	    geometry_ptr point(new point_impl(-1));
	    point->move_to(x,y);
	    feature->set_geometry(point);
	    ++count_;
        }
	else if (type == shape_io::shape_pointm)
	{
	    double x=shape_.shp().read_double();
	    double y=shape_.shp().read_double();
	    shape_.shp().read_double();// m
	    geometry_ptr point(new point_impl(-1));
	    point->move_to(x,y);
	    feature->set_geometry(point);
	    ++count_;
	}
	else if (type == shape_io::shape_pointz)
	{
	    double x=shape_.shp().read_double();
	    double y=shape_.shp().read_double();
	    shape_.shp().read_double();// z
	    shape_.shp().read_double();// m
	    geometry_ptr point(new point_impl(-1));
	    point->move_to(x,y);
	    feature->set_geometry(point);
	    ++count_;
	}	
        else
        {
            while(!filter_.pass(shape_.current_extent()) && 
	    		  itr_!=ids_.end())
	    {
                pos=*itr_++;
                shape_.move_to(pos);
	    }
	    
            switch (type)
            {
	        case shape_io::shape_polyline:
		{
		    geometry_ptr line = shape_.read_polyline();
		    feature->set_geometry(line);
		    ++count_;
		    break;
		}
		case shape_io::shape_polylinem:
		{
		    geometry_ptr line = shape_.read_polylinem();
		    feature->set_geometry(line);
		    ++count_;
		    break;
		}
		case shape_io::shape_polylinez:
		{
		    geometry_ptr line = shape_.read_polylinez();
		    feature->set_geometry(line);
		    ++count_;
		    break;
		}
	        case shape_io::shape_polygon:
		{
		 
		    geometry_ptr poly = shape_.read_polygon();
		    feature->set_geometry(poly);
		    ++count_;
		    break;
		}
	        case shape_io::shape_polygonm:
		{ 
		    geometry_ptr poly = shape_.read_polygonm();
		    feature->set_geometry(poly);
		    ++count_;
		    break;
		}
	        case shape_io::shape_polygonz:
		{
		    geometry_ptr poly = shape_.read_polygonz();
		    feature->set_geometry(poly);
		    ++count_;
		    break;
		}
            }
        }
	if (attr_ids_.size())
	{
	    shape_.dbf().move_to(shape_.id_);
	    std::vector<int>::const_iterator pos=attr_ids_.begin();
	    while (pos!=attr_ids_.end())
	    {
		try 
		{
		    shape_.dbf().add_attribute(*pos,*feature);
		}
		catch (...)
		{
		    std::clog<<"exception caught\n";
		}
		++pos;
	    }
	}
	return feature;
    }
    else
    {
	std::clog<<count_<<" features\n";
	return feature_ptr();
    }
}


template <typename filterT>
shape_index_featureset<filterT>::~shape_index_featureset() {}

template class shape_index_featureset<filter_in_box>;


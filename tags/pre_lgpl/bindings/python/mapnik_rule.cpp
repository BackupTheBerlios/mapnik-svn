/* This file is part of python_mapnik (c++/python mapping toolkit)
 * Copyright (C) 2005 Artem Pavlenko, Jean-Francois Doyon
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

//$Id$


#include <boost/python.hpp>
#include <boost/python/implicit.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <mapnik.hpp>

using mapnik::rule_type;
using mapnik::filter;
using mapnik::filter_ptr;
using mapnik::Feature;

using mapnik::point_symbolizer;
using mapnik::line_symbolizer;
using mapnik::line_pattern_symbolizer;
using mapnik::polygon_symbolizer;
using mapnik::polygon_pattern_symbolizer;
using mapnik::raster_symbolizer;
using mapnik::text_symbolizer;
using mapnik::symbolizer;
using mapnik::symbolizers;

void export_rule()
{
    using namespace boost::python;

    implicitly_convertible<point_symbolizer,symbolizer>();
    implicitly_convertible<line_symbolizer,symbolizer>();
    implicitly_convertible<line_pattern_symbolizer,symbolizer>();
    implicitly_convertible<polygon_symbolizer,symbolizer>();
    implicitly_convertible<polygon_pattern_symbolizer,symbolizer>();
    implicitly_convertible<raster_symbolizer,symbolizer>();
    implicitly_convertible<text_symbolizer,symbolizer>();
    
    class_<symbolizers>("Symbolizers",init<>("TODO"))
    	.def(vector_indexing_suite<symbolizers>())
    	;
    
    class_<rule_type>("Rule",init<>("default constructor"))
	.def(init<std::string const&,
	     boost::python::optional<std::string const&,double,double> >())
	.add_property("name",make_function
		      (&rule_type::get_name,
		       return_value_policy<copy_const_reference>()),
		      &rule_type::set_name)
	.add_property("title",make_function
		      (&rule_type::get_title,return_value_policy<copy_const_reference>()),
		      &rule_type::set_title)
	.add_property("abstract",make_function
		      (&rule_type::get_abstract,return_value_policy<copy_const_reference>()),
		      &rule_type::set_abstract)
	.add_property("filter",make_function
		      (&rule_type::get_filter,return_value_policy<copy_const_reference>()),
		      &rule_type::set_filter)
	.add_property("min_scale",&rule_type::get_min_scale,&rule_type::set_min_scale)
	.add_property("max_scale",&rule_type::get_max_scale,&rule_type::set_max_scale)
	.def("set_else",&rule_type::set_else)
	.def("has_else",&rule_type::has_else_filter)
	.def("active",&rule_type::active)
	.add_property("symbols",make_function
		      (&rule_type::get_symbolizers,return_value_policy<reference_existing_object>()))
	;
}


/*****************************************************************************
 * 
 * This file is part of Mapnik (c++ mapping toolkit)
 *
 * Copyright (C) 2006 Artem Pavlenko, Jean-Francois Doyon
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
//$Id: mapnik_layer.cc 17 2005-03-08 23:58:43Z pavlenko $


#include <boost/python.hpp>
#include <boost/python/detail/api_placeholder.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <mapnik.hpp>

using mapnik::Layer;
using mapnik::parameters;

struct layer_pickle_suite : boost::python::pickle_suite
{
    
    static boost::python::tuple
    getinitargs(const Layer& l)
    {    
	using namespace boost::python;
        return boost::python::make_tuple(l.params());
    }
    static boost::python::tuple
    getstate(const Layer& l)
    {
	using namespace boost::python;
	
	std::vector<std::string> const& styles=l.styles();
	std::vector<std::string>::const_iterator itr=styles.begin();
	boost::python::list py_styles;
        
	while (itr!=styles.end())
	{
	    py_styles.append(*itr++);
	}
	
	return boost::python::make_tuple(l.getMinZoom(),
			  l.getMaxZoom(),
			  py_styles);
    }

    static void
    setstate (Layer& l, boost::python::tuple state)
    {
	using namespace boost::python;
	if (len(state) != 3)
	{
	    PyErr_SetObject(PyExc_ValueError,
			    ("expected 3-item tuple in call to __setstate__; got %s"
			     % state).ptr()
			    );
	    throw_error_already_set();
	}

	l.setMinZoom(extract<double>(state[0]));
	l.setMaxZoom(extract<double>(state[1]));

	boost::python::list styles=extract<boost::python::list>(state[2]);
	for (int i=0;i<len(styles);++i)
	{
	    l.add_style(extract<std::string>(styles[i]));
	}
    }
};

namespace  
{
    //user-friendly wrapper that uses Python dictionary
    using namespace boost::python;
    Layer create_layer(const dict& d)
    {
	parameters params;
	boost::python::list keys=d.keys();
	for (int i=0;i<len(keys);++i)
	{
	    std::string key=extract<std::string>(keys[i]);
            std::string value=extract<std::string>(d[key]);
	    params[key] = value;
	}
	return Layer(params);
    }
}

void export_layer()
{
    using namespace boost::python;
    class_<std::vector<std::string> >("Styles")
    	.def(vector_indexing_suite<std::vector<std::string>,true >())
    	;
    
    class_<Layer>("Layer","A map layer.",no_init)
        .def("name",&Layer::name,return_value_policy<copy_const_reference>(), "Return the name of the layer.")
        .def("title",&Layer::title,return_value_policy<copy_const_reference>(), "Return the title of the layer.")
        .def("abstract",&Layer::abstract,return_value_policy<copy_const_reference>(), "Return the abstract of the layer.")
        .def("params",&Layer::params,return_value_policy<reference_existing_object>(), "The configuration parameters of the layer.  These vary depending on the type of data source.")
        .def("envelope",&Layer::envelope, "Return the geographic envelope/bounding box of the data in the layer.")
	.add_property("minzoom",&Layer::getMinZoom,&Layer::setMinZoom)
	.add_property("maxzoom",&Layer::getMaxZoom,&Layer::setMaxZoom)
	.add_property("styles",make_function
		      (&Layer::styles,return_value_policy<reference_existing_object>()))
        .def_pickle(layer_pickle_suite())
        ;
    def("CreateLayer",&create_layer);
}

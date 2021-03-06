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

//#include "array.hpp"

//$Id$

#ifndef FILTER_FACTORY_HPP
#define FILTER_FACTORY_HPP

#include "filter_parser.hpp"

using std::string;

namespace mapnik
{
    template<typename FeatureT>
    class MAPNIK_DECL filter_factory
    {
    public:
	static filter_ptr compile(string const& str)
	{
	    stack<shared_ptr<filter<FeatureT> > > filters;
	    stack<shared_ptr<expression<FeatureT> > > exps;
	    filter_grammar<FeatureT> grammar(filters,exps);
	    char const *text = str.c_str();
	    parse_info<> info = parse(text,text+strlen(text),grammar,space_p);
	    if (info.full && !filters.empty())
	    {
		return filters.top();	
	    }
	    else 
	    {
		clog << "failed at :" << info.stop << "\n";
		return filter_ptr(new none_filter<FeatureT>());
	    }  
	}
    };
}

#endif //FILTER_FACTORY_HPP

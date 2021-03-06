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
#ifndef MEMORY_FEATURESET_HPP
#define MEMORY_FEATURESET_HPP

#include <boost/utility.hpp>
#include <mapnik/memory_datasource.hpp>

namespace mapnik {
    
    class memory_featureset : public Featureset, private boost::noncopyable
    {
    public:
        memory_featureset(Envelope<double> const& bbox, memory_datasource const& ds)
            : bbox_(bbox),
              pos_(ds.features_.begin()),
              end_(ds.features_.end())
        {}
        virtual ~memory_featureset() {}
        
        feature_ptr next()
        {
            while (pos_ != end_)
            {
                geometry_ptr geom = (*pos_)->get_geometry();
                if (geom && bbox_.intersects(geom->envelope()))
                {
                    return *pos_++;
                }
                ++pos_;
            }
            return feature_ptr();
        }
        
    private:
        Envelope<double> const& bbox_;
        std::vector<feature_ptr>::const_iterator pos_;
        std::vector<feature_ptr>::const_iterator end_; 
    };
}

#endif // MEMORY_FEATURESET_HPP

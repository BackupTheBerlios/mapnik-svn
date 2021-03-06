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

#ifndef SHAPE_FS_HH
#define SHAPE_FS_HH

#include "shape.hpp"
#include "geom_util.hpp"

using namespace mapnik;

template <typename filterT>
class MAPNIK_DECL shape_featureset : public Featureset
{
    filterT filter_;
    int shape_type_;
    shape_io shape_;
    Envelope<double> query_ext_;
    long file_length_;
    std::vector<int> attr_ids_;
    mutable Envelope<double> feature_ext_;
    mutable int total_geom_size;
    mutable int count_;
public:
    shape_featureset(const filterT& filter, const std::string& shape_file,
		     const std::set<std::string>& attribute_names,long file_length);
    virtual ~shape_featureset();
    feature_ptr next();
private:
    shape_featureset(const shape_featureset&);
    const shape_featureset& operator=(const shape_featureset&);
};

#endif                                            //SHAPE_FS_HH

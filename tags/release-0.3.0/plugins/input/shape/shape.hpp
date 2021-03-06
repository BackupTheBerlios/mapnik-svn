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

#ifndef SHAPE_HH
#define SHAPE_HH

#include "datasource.hpp"
#include "envelope.hpp"
#include "shape_io.hpp"

using namespace mapnik;

class MAPNIK_DECL shape_datasource : public datasource
{

public:
    int type() const;
    static std::string name();
    featureset_ptr features(const query& q) const;
    const Envelope<double>& envelope() const;
    shape_datasource(const parameters &params);
    layer_descriptor const& get_descriptor() const;
    virtual ~shape_datasource();
private:
    shape_datasource(const shape_datasource&);
    shape_datasource& operator=(const shape_datasource&);
    void init(shape_io& shape);
private:
    std::string shape_name_;
    int type_;
    long file_length_;
    mapnik::Envelope<double> extent_;
    bool indexed_;
    layer_descriptor desc_;
    static std::string name_;
    
};

#endif                                            //SHAPE_HH

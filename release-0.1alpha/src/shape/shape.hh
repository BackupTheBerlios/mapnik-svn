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

#ifndef SHAPE_HH
#define SHAPE_HH

#include "mapnik.hh"
#include "shape_io.hh"

using namespace mapnik;

class shape_datasource : public datasource
{
    std::string shape_name_;
    int type_;
    long file_length_;
    mapnik::Envelope<double> extent_;
    bool indexed_;
public:
    int type() const;
    static std::string name();
    FeaturesetPtr featuresAll(const CoordTransform& t) const;
    FeaturesetPtr featuresInBox(const CoordTransform& t,const mapnik::Envelope<double>& box) const;
    FeaturesetPtr featuresAtPoint(const CoordTransform& t,const mapnik::coord2d& pt) const;
    const mapnik::Envelope<double>& envelope() const;
    shape_datasource(const Parameters &params);
    virtual ~shape_datasource();
private:
    shape_datasource(const shape_datasource&);
    shape_datasource& operator=(const shape_datasource&);
    void init(shape_io& shape);
};

#endif                                            //SHAPE_HH

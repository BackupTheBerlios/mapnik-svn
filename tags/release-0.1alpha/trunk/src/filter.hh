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

// $Id$

#ifndef FILTER_HH
#define FILTER_HH

#include "envelope.hh"

namespace mapnik
{
    struct filter_in_box
    {
        Envelope<double> box_;
        explicit filter_in_box(const Envelope<double>& box)
            : box_(box) {}

        bool pass(const Envelope<double>& extent) const
        {
            return extent.intersects(box_);
        }
    };

    struct filter_at_point
    {
        coord2d pt_;
        explicit filter_at_point(const coord2d& pt)
            : pt_(pt) {}
        bool pass(const Envelope<double>& extent) const
        {
            return extent.contains(pt_);
        }
    };
}
#endif                                            //FILTER_HH
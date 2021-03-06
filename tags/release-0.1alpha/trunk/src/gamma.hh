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

//$Id$

#ifndef GAMMA_HH
#define GAMMA_HH

namespace mapnik 
{
    struct gamma
    {
	unsigned char g2l[256];
	unsigned char l2g[256];
	gamma(double gamma=2.0)
	{
	    int result;
	    for (int i=0;i< 256;i++)
	    {
		result=(int)(pow(i/255.0,gamma) * 255.0 + 0.5);
		g2l[i]=(unsigned short)result;
	    }
	    for (int i = 0; i < 256; i++)
	    {
		result = (int)(pow(i/255.0, 1/gamma) * 255.0 + 0.5);
		l2g[i] = (unsigned char)result;
	    }
	}
    };
}

#endif  //GAMMA_HH

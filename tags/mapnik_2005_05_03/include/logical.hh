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

#ifndef LOGICAL_HH
#define LOGICAL_HH

#include "filter.hh"

namespace mapnik
{
    template <typename FeatureT> 
    struct logical_and : public filter<FeatureT>  
    {
	logical_and(filter<FeatureT> const& filter1,
		    filter<FeatureT> const& filter2)
	    : filter<FeatureT>(),
	      filter1_(filter1.clone()),
	      filter2_(filter2.clone()) {}
	
	logical_and(logical_and const& other)
	    : filter<FeatureT>(),
	      filter1_(other.filter1_->clone()),
	      filter2_(other.filter2_->clone()) {}

	bool pass(const FeatureT& feature) const
	{
	    return (filter1_->pass(feature) && 
		filter2_->pass(feature));
	}

	filter<FeatureT>* clone() const
	{
	    return new logical_and(*this);
	}

	void accept(filter_visitor<FeatureT>& v)
	{
	    filter1_->accept(v);
	    filter2_->accept(v);
	    v.visit(*this);
	}

	virtual ~logical_and()
	{
	    delete filter1_;
	    delete filter2_;
	}
	
    private:
	filter<FeatureT>* filter1_;
	filter<FeatureT>* filter2_;
    };

    template <typename FeatureT> 
    struct logical_or : public filter<FeatureT>  
    {
	
	logical_or(const filter<FeatureT>& filter1,const filter<FeatureT>& filter2)
	    : filter<FeatureT>(),
	      filter1_(filter1.clone()),
	      filter2_(filter2.clone()) {}
	
	logical_or(logical_or const& other)
	    : filter<FeatureT>(),
	      filter1_(other.filter1_->clone()),
	      filter2_(other.filter2_->clone()) {}

	bool pass(const FeatureT& feature) const
	{
	    if (filter1_->pass(feature))
	    {
		return true;
	    }
	    else
	    {
		return filter2_->pass(feature);
	    }
	}
	filter<FeatureT>* clone() const
	{
	    return new logical_or(*this);
	}

	void accept(filter_visitor<FeatureT>& v)
	{
	    filter1_->accept(v);
	    filter2_->accept(v);
	    v.visit(*this);
	}

	virtual ~logical_or()
	{  
	    delete filter1_;
	    delete filter2_;
	}
    private:
	filter<FeatureT>* filter1_;
	filter<FeatureT>* filter2_;
    };

    template <typename FeatureT> 
    struct logical_not : public filter<FeatureT>  
    {
	logical_not(filter<FeatureT> const& _filter)
	    : filter<FeatureT>(),
	      filter_(_filter.clone()) {}
	logical_not(logical_not const& other)
	    : filter<FeatureT>(),
	      filter_(other.filter_->clone()) {}

	int type() const
	{
	    return filter<FeatureT>::LOGICAL_OPS;
	}

	bool pass(const FeatureT& feature) const
	{
	    return !(filter_->pass(feature));
	}

	filter<FeatureT>* clone() const
	{
	    return new logical_not(*this);
	}
	
	void accept(filter_visitor<FeatureT>& v)
	{
	    filter_->accept(v);
	    v.visit(*this);
	}
	
	~logical_not() 
	{
	    delete filter_;
	}
    private:
	filter<FeatureT>* filter_;
    };
}
 
#endif //LOGICAL_HH

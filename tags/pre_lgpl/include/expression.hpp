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

#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "value.hpp"
#include "filter_visitor.hpp"

namespace mapnik
{
    template <typename FeatureT> class filter_visitor;
    template <typename FeatureT>
    class expression
    {
    public:
	virtual value get_value(FeatureT const& feature) const=0;
	virtual void accept(filter_visitor<FeatureT>& v)=0;
	virtual expression<FeatureT>* clone() const=0;
	virtual std::string to_string() const=0;
	virtual ~expression() {}
    };

    template <typename FeatureT> 
    class literal : public expression<FeatureT>
    {
    public:
	literal(int val)
	    : expression<FeatureT>(),
	      value_(val) {}
	literal(double val)
	    : expression<FeatureT>(),
	      value_(val) {}
	literal(std::string const& val)
	    : expression<FeatureT>(),
	      value_(val) {}
	literal(literal const& other)
	    : expression<FeatureT>(),
	      value_(other.value_) {}
	
	value get_value(FeatureT const& /*feature*/) const
	{
	    return value_;
	}
	void accept(filter_visitor<FeatureT>& v)
	{
	    v.visit(*this);
	}
	expression<FeatureT>* clone() const
	{
	    return new literal(*this); 
	}
	std::string to_string() const
	{
	    return value_.to_expression_string();
	}
        ~literal() {}
    private:
	value value_;
    };
  

    template <typename FeatureT> 
    class property : public expression<FeatureT>
    {
    public:
	property(std::string const& name)
        : expression<FeatureT>(),
	    name_(name)
	    {}
	
	property(property const& other)
	    : expression<FeatureT>(),
	     name_(other.name_)
	    {}

	value get_value(FeatureT const& feature) const
	{
	    return feature[name_];
	}
	void accept(filter_visitor<FeatureT>& v)
	{
	    v.visit(*this);
	}
	expression<FeatureT>* clone() const
	{
	    return new property(*this); 
	}
	std::string const& name() const
	{
	    return name_;
	}

	std::string to_string() const
	{
	    return "["+name_+"]";
	}

    ~property() {}

    private:
	    std::string name_;
    };
}

#endif //EXPRESSION_HPP

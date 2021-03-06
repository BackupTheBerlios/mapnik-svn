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

//$Id: layer.cpp 17 2005-03-08 23:58:43Z pavlenko $


#include "style.hpp"
#include "datasource.hpp"
#include "datasource_cache.hpp"
#include "layer.hpp"

#include <string>
#include <iostream>

namespace mapnik
{
    using namespace std;
    Layer::Layer() {}
    Layer::Layer(const parameters& params)
        :params_(params),
	 name_(params_["name"]),
	 minZoom_(0),
	 maxZoom_(std::numeric_limits<double>::max()),
	 active_(true),
	 selectable_(false),
	 selection_style_("default_selection")
    {}
    
    Layer::Layer(const Layer& rhs)
        :params_(rhs.params_),
	 name_(rhs.name_),
	 minZoom_(rhs.minZoom_),
	 maxZoom_(rhs.maxZoom_),
	 active_(rhs.active_),
	 selectable_(rhs.selectable_),
	 //ds_(rhs.ds_),
	 styles_(rhs.styles_),
	 selection_style_(rhs.selection_style_) {}
    
    Layer& Layer::operator=(const Layer& rhs)
    {
        Layer tmp(rhs);
        swap(tmp);
        return *this;
    }

    bool Layer::operator==(Layer const& other) const
    {
	return (this == &other);
    }
    
    void Layer::swap(const Layer& rhs)
    {
        params_=rhs.params_;
        name_=rhs.name_;
        minZoom_=rhs.minZoom_;
        maxZoom_=rhs.maxZoom_;
        active_=rhs.active_;
        selectable_=rhs.selectable_;
        //ds_=rhs.ds_;
        styles_=rhs.styles_;
	selection_style_=rhs.selection_style_;
    }

    Layer::~Layer() {}

    parameters const& Layer::params() const
    {
        return params_;
    }
    
    const string& Layer::name() const
    {
        return name_;
    }

    void Layer::add_style(std::string const& stylename)
    {
        styles_.push_back(stylename);
    }

    std::vector<std::string> const& Layer::styles() const
    {
        return styles_;
    }

    void Layer::setMinZoom(double minZoom)
    {
        minZoom_=minZoom;
    }

    void Layer::setMaxZoom(double maxZoom)
    {
        maxZoom_=maxZoom;
    }

    double Layer::getMinZoom() const
    {
        return minZoom_;
    }

    double Layer::getMaxZoom() const
    {
        return maxZoom_;
    }

    void Layer::setActive(bool active)
    {
        active_=active;
    }

    bool Layer::isActive() const
    {
        return active_;
    }

    bool Layer::isVisible(double scale) const
    {
        return isActive() && scale>=minZoom_ && scale<maxZoom_;
    }

    void Layer::setSelectable(bool selectable)
    {
        selectable_=selectable;
    }

    bool Layer::isSelectable() const
    {
        return selectable_;
    }

    const datasource_p& Layer::datasource() const
    {
	if (!ds_)
	{
	    try
	    {
		ds_=datasource_cache::instance()->create(params_);
	    }
	    catch (...)
	    {
		std::cerr << "exception caught : can not create datasorce" << std::endl;  
	    }
	}
	return ds_;
    }

    Envelope<double> Layer::envelope() const
    {
	datasource_p const& ds = datasource();
	if (ds)
	{
	    return ds->envelope();
	}
    	return Envelope<double>();
    }

    void Layer::selection_style(const std::string& name) 
    {
	selection_style_=name;
    }
    
    const std::string& Layer::selection_style() const 
    {
	return selection_style_;
    }

    void Layer::add_to_selection(shared_ptr<Feature>& feature) const
    {
	selection_.push_back(feature);
    }
 
    vector<shared_ptr<Feature> >& Layer::selection() const
    {
	return selection_;
    }

    void Layer::clear_selection() const 
    {
	selection_.clear();
    }
}

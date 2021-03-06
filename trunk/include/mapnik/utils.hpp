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

//$Id: utils.hpp 39 2005-04-10 20:39:53Z pavlenko $

#ifndef UTILS_HPP
#define UTILS_HPP
// stl
#include <stdexcept>
#include <cstdlib>
#include <limits>
#include <ctime>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>
// boost
#include <boost/thread/mutex.hpp>

namespace mapnik
{
    using boost::mutex;
    
    template <typename T>
    class CreateUsingNew
    {
    public:
        static T* create()
        {
            return new T;
        }
        static void destroy(T* obj)
        {
            delete obj;
        }
    };

    template <typename T>
    class CreateStatic
    {
    private:
        union MaxAlign
        {
            char t_[sizeof(T)];
            short int shortInt_;
            int int_;
            long int longInt_;
            float float_;
            double double_;
            long double longDouble_;
            struct Test;
            int Test::* pMember_;
            int (Test::*pMemberFn_)(int);
        };

    public:
        
        static T* create()
        {
            static MaxAlign staticMemory;
            return new(&staticMemory) T;
        }
        
        static void destroy(volatile T* obj)
        {
            obj->~T();
        }
    };
    
    template <typename T,
              template <typename T> class CreatePolicy=CreateStatic> class singleton
              {
                  friend class CreatePolicy<T>;
                  static T* pInstance_;
                  static bool destroyed_;
                  singleton(const singleton &rhs);
                  singleton& operator=(const singleton&);
                  static void onDeadReference()
                  {
                      throw std::runtime_error("dead reference!");
                  }
                  
                  static void DestroySingleton()
                  {
                      CreatePolicy<T>::destroy(pInstance_);
                      pInstance_ = 0;
                      destroyed_=true;
#ifdef MAPNIK_DEBUG
                      std::clog << " destroyed singleton \n";
#endif
                  }
                  
              protected:
                  static mutex mutex_;
                  singleton() {}
              public:
                  static  T* instance()
                  {
                      if (!pInstance_)
                      {
                          mutex::scoped_lock lock(mutex_);
                          if (!pInstance_)
                          {
                              if (destroyed_)
                              {
                                  onDeadReference();
                              }
                              else
                              {
                                  pInstance_=CreatePolicy<T>::create();
                                  // register destruction
                                  std::atexit(&DestroySingleton);
                              }
                          }
                      }
                      return pInstance_;
                  }
              };

    template <typename T,
              template <typename T> class CreatePolicy> mutex singleton<T,CreatePolicy>::mutex_;
    template <typename T,
              template <typename T> class CreatePolicy> T* singleton<T,CreatePolicy>::pInstance_=0;
    template <typename T,
              template <typename T> class CreatePolicy> bool singleton<T,CreatePolicy>::destroyed_=false;
   
}


#endif //UTILS_HPP

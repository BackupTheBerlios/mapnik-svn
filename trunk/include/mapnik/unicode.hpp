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
#ifndef UNICODE_HPP
#define UNICODE_HPP

#include <string>
#include <boost/utility.hpp>

#ifdef USE_FRIBIDI
#include <fribidi/fribidi.h>
#endif

#include <iconv.h>

namespace mapnik {
    
/*
** Use FRIBIDI to encode the string.
** The return value must be freed by the caller.
*/

#ifdef USE_FRIBIDI
    inline wchar_t* bidi_string(const wchar_t *logical)
    {
        FriBidiCharType base = FRIBIDI_TYPE_ON;
        size_t len;

        len = wcslen(logical);

        FriBidiChar *visual;

        FriBidiStrIndex *ltov, *vtol;
        FriBidiLevel *levels;
        FriBidiStrIndex new_len;
        fribidi_boolean log2vis;
        
        visual = (FriBidiChar *) malloc (sizeof (FriBidiChar) * (len + 1));
        ltov = 0;
        vtol = 0;
        levels = 0;

        /* Create a bidi string. */
        log2vis = fribidi_log2vis ((FriBidiChar *)logical, len, &base,
                /* output */
                visual, ltov, vtol, levels);

        if (!log2vis) {
            return 0;
        }

        new_len = len;

        return (wchar_t *)visual;
    }
#endif


    inline std::wstring to_unicode(std::string const& text)
    {
        std::wstring out;
        unsigned long code = 0;
        int expect = 0;
        std::string::const_iterator itr=text.begin();
        std::string::const_iterator end=text.end();
        while ( itr != end)
        {
            unsigned p = (*itr++) & 0xff;
            if ( p >= 0xc0)
            {
                if ( p < 0xe0)      // U+0080 - U+07ff
                {
                    expect = 1;
                    code = p & 0x1f;
                }
                else if ( p < 0xf0)  // U+0800 - U+ffff
                {
                    expect = 2;
                    code = p & 0x0f;
                }
                else if ( p  < 0xf8) // U+1000 - U+10ffff
                {
                    expect = 3;
                    code = p & 0x07;
                }
                continue;
            }
            else if (p >= 0x80)
            {
                --expect;
                if (expect >= 0)
                {
                    code <<= 6;
                    code += p & 0x3f;
                }
                if (expect > 0)
                    continue;
                expect = 0;
            }
            else 
            {
                code = p;            // U+0000 - U+007f (ascii)
            }
            out.push_back(wchar_t(code));
        }
#ifdef USE_FRIBIDI
        wchar_t *bidi_text = bidi_string(out.c_str());
        out = bidi_text;
        free(bidi_text);
#endif
        
        return out;
    }
   
   inline std::wstring latin1_to_unicode(std::string const& text)
   {
      std::wstring out;
      std::string::const_iterator itr=text.begin();
      std::string::const_iterator end=text.end();
      while ( itr != end)
      {
         unsigned p = (*itr++) & 0xff;     
         out.push_back(wchar_t(p));
      }      
      return out;
   }

   class transcoder : private boost::noncopyable
   {
      public:
         explicit transcoder (std::string const& encoding)
         {
            desc_ = iconv_open("UCS-2",encoding.c_str());
         }
         
         std::wstring transcode(std::string const& input)
         {
            std::string buf(input.size() * 2,0);
            size_t inleft = input.size();
            char * in  = const_cast<char*>(input.data());
            size_t outleft = buf.size();
            char * out = const_cast<char*>(buf.data());
            
            iconv(desc_,&in,&inleft,&out,&outleft);
            
            std::string::const_iterator itr = buf.begin();
            std::string::const_iterator end = buf.end();
            wchar_t wch = 0;
            bool state = false;
            std::wstring unicode;
            size_t num_char = buf.size() - outleft;
            for ( ; itr != end; ++itr)
            {
               if (!state)
               {
                  wch = (*itr & 0xff);
                  state = true;
               }
               else 
               {
                  wch |= *itr << 8 ;
                  unicode.push_back(wchar_t(wch));
                  state = false;
               }
               if (!num_char--) break;
            }
                     
#ifdef USE_FRIBIDI
            wchar_t *bidi_text = bidi_string(unicode.c_str());
            unicode = bidi_text;
            free(bidi_text);
#endif
            return unicode;
         }
         
         ~transcoder()
         {
            iconv_close(desc_);
         }

   private:
      iconv_t desc_;
   };
}

#endif // UNICODE_HPP

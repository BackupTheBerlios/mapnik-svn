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

#include "image_reader.hh"
#include <png.h>

namespace mapnik
{
    class PngReader : public ImageReader 
    {
    private:
	std::string fileName_;
	unsigned width_;
	unsigned height_;
	int bit_depth_;
	int color_type_;
    public:
	explicit PngReader(const std::string& fileName);
	~PngReader();
	unsigned width() const;
	unsigned height() const;
	void read(unsigned x,unsigned y,ImageData32& image);	
    private:
	PngReader(const PngReader&);	
	PngReader& operator=(const PngReader&);
	void init();
    };
    
    namespace 
    {
	ImageReader* createPngReader(const std::string& file)
	{
	    return new PngReader(file);
	}
	const bool registered = register_image_reader("png",createPngReader);
    }

    PngReader::PngReader(const std::string& fileName) 
	: fileName_(fileName),
	  width_(0),
	  height_(0),
	  bit_depth_(0),
	  color_type_(0)
    {
	try 
	{
	    init();
	} 
	catch (const ImageReaderException& e) 
	{
	    std::cerr<<e.what()<<std::endl;
	    throw;
	}
    }

    PngReader::~PngReader() {}
    
    void PngReader::init()
    {
	FILE *fp=fopen(fileName_.c_str(),"r");
	if (!fp) throw ImageReaderException("cannot open image file "+fileName_);
	png_byte header[8];
	memset(header,0,8);
	fread(header,1,8,fp);
	int is_png=!png_sig_cmp(header,0,8);
	if (!is_png)
	{
	    fclose(fp);
	    throw ImageReaderException(fileName_ + " is not a png file");
	}
        png_structp png_ptr = png_create_read_struct
	    (PNG_LIBPNG_VER_STRING,0,0,0);
	
	if (!png_ptr) 
	{
	    fclose(fp);
	    throw ImageReaderException("failed to allocate png_ptr");
	}
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
	    png_destroy_read_struct(&png_ptr,0,0);
	    fclose(fp);
	    throw ImageReaderException("failed to create info_ptr");
	}

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr,8);
	png_read_info(png_ptr, info_ptr);

	png_uint_32  width, height;
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth_, &color_type_,0,0,0);
	
	width_=width;
	height_=height;
	
	std::cout<<"bit_depth="<<bit_depth_<<" color_type="<<color_type_<<std::endl;
	png_destroy_read_struct(&png_ptr,&info_ptr,0);	
	fclose(fp);
    }
    
    unsigned PngReader::width() const 
    {
	return width_;
    }
    
    unsigned PngReader::height() const 
    {
	return height_;
    }
    
    void PngReader::read(unsigned x0, unsigned y0,ImageData32& image) 
    {
	FILE *fp=fopen(fileName_.c_str(),"r");
	if (!fp) throw ImageReaderException("cannot open image file "+fileName_);
	
        png_structp png_ptr = png_create_read_struct
	    (PNG_LIBPNG_VER_STRING,0,0,0);
	
	if (!png_ptr) 
	{
	    fclose(fp);
	    throw ImageReaderException("failed to allocate png_ptr");
	}
	
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
	    png_destroy_read_struct(&png_ptr,0,0);
	    fclose(fp);
	    throw ImageReaderException("failed to create info_ptr");
	}

	png_init_io(png_ptr, fp);
	png_read_info(png_ptr, info_ptr);
	
	if (color_type_ == PNG_COLOR_TYPE_PALETTE)
	    png_set_expand(png_ptr);
	if (color_type_ == PNG_COLOR_TYPE_GRAY && bit_depth_ < 8)
	    png_set_expand(png_ptr);
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
	    png_set_expand(png_ptr);
	if (bit_depth_ == 16)
	    png_set_strip_16(png_ptr);
	if (color_type_ == PNG_COLOR_TYPE_GRAY ||
	    color_type_ == PNG_COLOR_TYPE_GRAY_ALPHA)
	    png_set_gray_to_rgb(png_ptr);
	
	// quick hack -- only work in >=libpng 1.2.7
	png_set_add_alpha(png_ptr,1,1);
 
	double gamma;
	if (png_get_gAMA(png_ptr, info_ptr, &gamma))
	    png_set_gamma(png_ptr, 2.2, gamma);

	png_read_update_info(png_ptr, info_ptr);
	
	//START read image rows
	unsigned w=std::min((unsigned)image.width(),width_);
	unsigned h=std::min((unsigned)image.height(),height_);
	
	unsigned rowbytes=png_get_rowbytes(png_ptr, info_ptr);
	unsigned char* row= new unsigned char[rowbytes];
	for (unsigned i=0;i<height_;++i)
	{
	    png_read_row(png_ptr,row,0);
	    if (i>=y0 && i<h) 
	    {
		image.setRow(i-y0,(unsigned*) &row[x0],w);
	    } 
	}
	//END
	delete [] row;
	png_read_end(png_ptr,0);
	png_destroy_read_struct(&png_ptr, &info_ptr,0);
	fclose(fp);
    }
}

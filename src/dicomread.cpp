/*
 * The GNU Octave dicom package is Copyright Andy Buckle 2010
 * Contact: blondandy using the sf.net system, 
 * <https://sourceforge.net/sendmessage.php?touser=1760416>
 * 
 * Changes Copyright Kris Thielemans 2011:
 * - support usage dicomread(struct-returned-by-dicominfo)
 * - return image in same order as matlab
 *
 * The GNU Octave dicom package is free software: you can redistribute 
 * it and/or modify it under the terms of the GNU General Public 
 * License as published by the Free Software Foundation, either 
 * version 3 of the License, or (at your option) any later version.
 * 
 * The GNU Octave dicom package is distributed in the hope that it 
 * will be useful, but WITHOUT ANY WARRANTY; without even the 
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 * 
 * Please see the file, "COPYING" for further details of GNU General 
 * Public License version 3.
 * 
 */

#include "octave/oct.h"
#include <octave/ov-struct.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gdcmImageReader.h"
#include "gdcmByteSwap.h"
using namespace gdcm;

#define DICOM_ERR -1
#define DICOM_OK 0

#define OCT_FN_NAME dicomread
#define QUOTED_(x) #x
#define QUOTED(x) QUOTED_(x)

inline bool host_is_little_endian()
{
    uint16_t x = 1;
    return *reinterpret_cast<uint8_t*>(&x) == 1;
}

template<typename T>
void swap_buffer(T* p, size_t n)
{
    //ByteSwap<T>::SwapRange(p, n);
    for (size_t i = 0; i < n; ++i)
        gdcm::ByteSwap<T>::Swap(p[i]);
}

DEFUN_DLD (dicomread, args, nargout,
  "-*- texinfo -*- \n\
@deftypefn {} @var{image} = dicomread (@var{filename}) \n\
@deftypefnx {} @var{image} = dicomread (@var{structure}) \n\
@deftypefnx {} @var{image} = dicomread (___, @var{propertyname}, @var{propertyvalue}) \n\
@deftypefnx {} [@var{image}, @var{cmap}] = dicomread (___) \n\
\n\
Load the image from a DICOM file. \n\
@subsubheading Inputs\n\
@var{filename} - a string giving the filename.*\n\
\n\
@var{structure} - a structure with a field @code{Filename} (such as returned by @code{dicominfo}).\n\
\n\
@var{propertyname}, @var{propertyvalue} - property name (string), property value for additional properties to function.\n\
\n\
Known properties are:\n\
@table @asis\n\
@item frames\n \
Either 'all' (default), a scalar frame value or vector of frame values.\n \
@item UseRVHeuristic\n\
Value is currently silently ignored.\n \
@end table\n\
\n\
@subsubheading Outputs\n\
@var{image} - An integer or float matrix will be returned, the number of bits will depend on the file. \
The image may be two or three dimensional, depending on the content of the file. \n\
\n\
@var{cmap} - Cx3 colormap associated with the image. Each row is a 3 element RGB triplet ion the range of 0 .. 1.\n \
If the image does not have a color map, colormap is []\n\
\n\
@subsubheading Examples\n\
Load the image data of a dcm file:\n\
\n\
@example\n\
> image = dicomread(file_in_loadpath('imdata/simple-test.dcm'))\n\
image =\n\
  0  0  0  0  0\n\
  0  0  0  0  0\n\
  0  0  0  0  0\n\
  0  0  0  0  0\n\
  0  0  0  0  0\n\
  0  0  0  0  0\n\
  0  0  0  0  0\n\
  0  0  0  0  0\n\
  0  0  0  0  0\n\
  0  0  0  0  0\n\
@end example\n\
\n\
Load the 1st and 10th frame of image data from a dcm file:\n\
\n\
@example\n\
> file = file_in_loadpath('imdata/US-PAL-8-10x-echo.dcm');\n\
> X = dicomread (file, 'frames;, [1, 10]);\n\
@end example\n\
\n\
@seealso{dicominfo} \n\
@end deftypefn \n\
")
{
  octave_value_list retval;  // create object to store return values
  if ( 0 == args.length()) 
    {
      error (QUOTED(OCT_FN_NAME)": one arg required: dicom filename");
      return retval; 
    }

  std::string filename;
  octave_value frames = octave_value("all");
  // argument processing
  // check if 1st argument is a string or a struct with field Filename
  // If so, assign to filename variable, otherwise exit.
  if (args(0).is_string ())
    {
      filename = args(0).string_value();
    }
  else
    {
      if (! args(0).OV_ISMAP ())
        {
          error(QUOTED(OCT_FN_NAME)": arg should be a filename, 1 row of chars, or a struct returned by dicominfo");
          return retval; 
        }
      octave_scalar_map arg0 = args(0).scalar_map_value ();
      if (!arg0.contains("Filename"))
        {
          error (QUOTED(OCT_FN_NAME)": if arg is a struct, it should have the Filename field");
          return retval; 
        }
      octave_value tmp = arg0.getfield ("Filename");
      filename = tmp.string_value();
    }

  // properties
  if (args.length() > 1)
    {
      for(int i=1;i<args.length();i+=2)
        {
          // first pair should be a property name
          if (! args (i).is_string ())
            {
              error ("Expected property name string");
              return octave_value ();
            }

          std::string name = args (i).string_value ();
          octave_value val = args (i+1);

          std::transform (name.begin (), name.end (), name.begin (), ::tolower);

          if (name == "frames")
            {
              // currently ignored
              if (val.is_string () && val.string_value() == "all")
                {
                  frames = val;
                }
	      else if (val.OV_ISINTEGER()  || val.is_real_scalar())
                {
	          Array<double> tdv = val.vector_value ();
	          frames = octave_value(tdv);
                }
	      else if(val.is_matrix_type())
	        {
	          Array<double> tdv = val.vector_value ();
	          frames = octave_value(tdv);
	        }
              else
                {
                  error (QUOTED(OCT_FN_NAME)": Expected frames as 'all', a scalar or vector of frames values");
	          return octave_value();
                }
            }
          else if (name == "usevrheuristic")
            {
              // currently ignored
            }
          else
            {
              error (QUOTED(OCT_FN_NAME)": unknown property name '%s'", args (i).string_value ().c_str());
	      return octave_value();
            }
        }
    }
    
#if 0 /* TODO support 'frames' stuff, see Matlab docs for dicomread */
  int i; // parse any additional args
  for (i=1; i<args.length(); i++)
    {
      charMatrix chex = args(i).char_matrix_value();
      if (chex.rows() != 1)
        {
          error (QUOTED(OCT_FN_NAME)": arg should be a string, 1 row of chars");
          return retval; 
        }
    }
#endif

  gdcm::ImageReader reader;
  reader.SetFileName (filename.c_str());
  if (!reader.Read())
    {
      error (QUOTED(OCT_FN_NAME)": Could not read DICOM file with image: %s",filename.c_str());
      return retval;
    }

  const gdcm::Image &image = reader.GetImage();

  const octave_idx_type ndim = image.GetNumberOfDimensions();
  const unsigned int * const dims = image.GetDimensions();
  // dim 0: cols (width)
  // dim 1: rows (height)
  // dim 2: number of frames
   
  gdcm::TransferSyntax ts =
    reader.GetFile().GetHeader().GetDataSetTransferSyntax();

  const bool host_le = host_is_little_endian();
  const bool file_le = (ts.GetSwapCode() == gdcm::SwapCode::LittleEndian);
  bool need_swap = (host_le != file_le);

  //printf("swap = %d h=%d f=%d\n", (int)need_swap, (int)host_le, (int)file_le);

  dim_vector dv;
  Array<octave_idx_type> perm_vect(dim_vector(ndim,1));
  perm_vect(0) = 1;
  perm_vect(1) = 0;

  // TODO check with non-square images if this needs to be dims[1],dims[0] etc
  if( 2 == ndim )
    {
      //this transposes first two dimensions
      dv = dim_vector(octave_idx_type(dims[0]), octave_idx_type(dims[1]));
    }
  else if (3 == ndim)
    {
      // should be (rows, cols, pages) in octave idiom
      dv = dim_vector(octave_idx_type(dims[0]), octave_idx_type(dims[1]), octave_idx_type(dims[2])); 
      perm_vect(2) = 2;
    }
  else
    {
      error(QUOTED(OCT_FN_NAME)": %i dimensions. not supported: %s",(int)ndim, filename.c_str());
      return retval;
    }

  if (gdcm::PixelFormat::UINT32 == image.GetPixelFormat())
    {
      //tested
      uint32NDArray arr(dv);
      image.GetBuffer ((char *)arr.fortran_vec());
      if (need_swap) swap_buffer<uint32_t>((uint32_t*)arr.fortran_vec(), arr.numel());
      retval(0) = octave_value (arr.permute(perm_vect));
    }
  else if ( gdcm::PixelFormat::UINT16 == image.GetPixelFormat() )
    {
      //tested
      uint16NDArray arr(dv);
      image.GetBuffer ((char *)arr.fortran_vec());
      if (need_swap) swap_buffer<uint16_t>((uint16_t*)arr.fortran_vec(), arr.numel());
      retval(0) = octave_value (arr.permute(perm_vect));
    }
  else if ( gdcm::PixelFormat::UINT8 == image.GetPixelFormat() )
    {
      //tested
      uint8NDArray arr(dv);
      image.GetBuffer ((char *)arr.fortran_vec());
      retval(0) = octave_value (arr.permute(perm_vect));
    }
  else if ( gdcm::PixelFormat::INT8 == image.GetPixelFormat() )
    { 
      // no example found to test
      int8NDArray arr(dv);
      image.GetBuffer((char *)arr.fortran_vec());
      retval(0) = octave_value(arr.permute(perm_vect));
    }
  else if ( gdcm::PixelFormat::INT16 == image.GetPixelFormat() )
    { 
      // tested
      int16NDArray arr(dv);
      image.GetBuffer((char *)arr.fortran_vec());
      // the dicom swap function appears to mess with the sign when unsigned values, so using unsigned swap here
      if (need_swap) swap_buffer<uint16_t>((uint16_t*)arr.fortran_vec(), arr.numel());
      retval(0) = octave_value(arr.permute(perm_vect));
    }
  else if ( gdcm::PixelFormat::INT32 == image.GetPixelFormat() )
    { 
      // no example found to test
      int32NDArray arr(dv);
      image.GetBuffer((char *)arr.fortran_vec());
      // the dicom swap function appears to mess with the sign when unsigned values, so using unsigned swap here
      if (need_swap) swap_buffer<uint32_t>((uint32_t*)arr.fortran_vec(), arr.numel());
      retval(0) = octave_value(arr.permute(perm_vect));
    }
  else
    {
      octave_stdout << image.GetPixelFormat() << '\n' ;
      error(QUOTED(OCT_FN_NAME)": pixel format not supported yet: %s", filename.c_str());
      return retval;
    }

  // Extract frames we want
  if (frames.is_string())
    {
      // all -> nothing to do
    }
  else
    {
      // need 0 indexing
      Array<octave_idx_type> frame_array = frames.octave_idx_type_vector_value();
      for (int i=0; i<frame_array.numel(); i++)
        {
          frame_array(i) = frame_array(i) - 1;
        }

      // get index image_data(:,_,[requested frames])
      octave_value_list idx;
      idx(0) = idx_vector::colon;       // All rows
      idx(1) = idx_vector::colon;       // All columns
      idx(2) = idx_vector(frame_array); // Specific pages

      retval(0) = retval(0).index_op(idx);
    }

  if (nargout > 1)
    {
      // we want to putput the cmap
      const gdcm::LookupTable &lut = image.GetLUT();
      bool has_lut = (lut.GetBitSample() > 0);
      unsigned short cmap_size = 0;
      if (has_lut) {
        unsigned short subscript, bitsize;
        lut.GetLUTDescriptor(LookupTable::RED, cmap_size, subscript, bitsize);
      }
      Matrix cmap(cmap_size, 3);
      if (has_lut)
        {
          std::vector<unsigned char> rgbaBuffer(cmap_size * 4);
          lut.GetBufferAsRGBA(rgbaBuffer.data());
          for (unsigned int idx = 0;idx < cmap_size; idx++)
            {
              cmap(idx, 0) = rgbaBuffer[idx*4 + 0]/256.0;
              cmap(idx, 1) = rgbaBuffer[idx*4 + 1]/256.0;
              cmap(idx, 2) = rgbaBuffer[idx*4 + 2]/256.0;
            }
        }

      retval(1) = cmap;
    }

  return retval;
}

/*
%!shared testfile
%! testfile = file_in_loadpath("imdata/CT-MONO2-16-ankle.dcm");

%!fail ("dicomread");
%!fail ("dicomread (1)");
%!fail ("dicomread ('hopefully_a_non_existant_file')");

%!test
%! rd=dicomread(testfile);
%! assert(rd(100,101),int16(128));

%!test
%! data={};
%! data.Filename = testfile;
%! rd=dicomread(data);
%! assert(rd(100,101),int16(128));

%!test
%! file = file_in_loadpath("imdata/US-PAL-8-10x-echo.dcm");
%! X = dicomread (file);
%! A = dicomread (file, "frames", [1]);
%! assert(X(:,:,1), A)
%! A = dicomread (file, "frames", [1, 10]);
%! assert(X(:,:,[1,10]), A)
*/

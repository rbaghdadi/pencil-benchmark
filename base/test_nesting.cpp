// UjoImro, 2013

#include <stdlib.h>
#include <opencv2/core/core.hpp>

#include "opencl.hpp"
#include "utility.hpp"


const int rows = 10;
const int cols = 15;

int main()
{

    carp::opencl::device device;
    device.compile( {"nesting.cl"}, {"transposeFloat"} );

    cv::Mat_<float> image(rows, cols);
    for ( int q=0; q<image.rows; q++ )
        for ( int w=0; w<image.cols; w++ )
        {
            image(q,w) = q*image.cols + w;            
        }
        
    carp::opencl::image<float> cl_image(device, rows, cols);
    carp::opencl::image<float> cl_trans(device, cols, rows);    

    cl_image.set(image);
    
    device["transposeFloat"]( cl_image.cl(), cl_image.ptr(), cl_trans.cl(), cl_trans.ptr() )
        .groupsize({32}, {32});

    cv::Mat_<float> trans = cl_trans.get();

    print_image( image, "image" );
    
    print_image( trans, "trans" );

    for (int q = 0; q < image.rows; q++ )
        for ( int w = 0; w < image.cols; w++ )
            assert( image(q,w) == trans(w,q) );
    
        
    return EXIT_SUCCESS;
    
} // main
    



// LuM end of file

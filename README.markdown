# Practical handout

## Problem statement

Write a simple Hough feature detector that can detect Circles of arbitrary size in
the input images.

The program must support the following functionality:

### Simple edge filter

A grey-scale image will need to be processed by a simple edge filter (see notes) followed by thresholding, to determine a binary image which contains background and edge pixels only

### Highlighting the extracted feature(s)

Once a feature has been detected, a circle corresponding to the feature must be plotted over the image to show the result. This image can be saved and then viewed.

### Multiple features

The procedure to determine accumulator maxima will need to search for multiple local maxima; each of these will be a new feature (circle).

Some sample test images with circles are available [here](http://www.cs.uct.ac.za/~patrick/hons/images)

# My solution

The solution is written in C++ using some Qt and cmake. Qt provides the convenience for loading and saving images, and cmake is there to generate build files. The solution works as follows:

* Load source image
* Run Sobel edge detection
* Hough transform for each radius of size 1..n, where n is the maximum radius possible
* Find the bright spots in the Hough-space images, and marks those off as circles in image space

This practical is very condusive towards parallelism. However, that is outside of the scope of the practical, and I specifically didn't do it so that the solution is as clean as possible.

Similarly, I have not included a GUI. If you wish to see the intermediate effects, it is possible to just return an image at any point in the detect method. For example, one could:
`return houghs[15];` to see the 15th image in Hough-space or `return binary;` to see the result of the edge detector.

## Building the solution

Change to a build director and run cmake from there. For example, in a Unix-like environment:

* `cd /path/to/hough-circle-detector`
* `cd build`
* `cmake ..` or add additional options such as `cmake -DCMAKE_BUILD_TYPE=debug ..`
* Use ccmake [ `ccmake ..` ] if you wish to change additional cmake options
* `make`

On Windows, the steps are equivalent - just set cmake to generate the correct files for your build system (Visual Studio, mingw..)

## Running the solution

By default, cmake will compile the binary into the src/ directory in the build folder. The binary requires at least a source image. I recommend running as:
`cd build; src/hcd --source=test.gif --output=test.out.jpg --minr=10 --maxr=40`
These settings are appropriate for the sample images, as circles tend to be of radius 15 and 32. Other values will work too, but will incur additional processing time.

The easiest way to run this program is by using a find script. For example, assuming you have the sample images in an `images/` directory, run:
`find /path/to/images/*.gif -exec /patth/to/hcd --source={} \;`
Assuming that the sample images are being used, you can add on the above mentioned min/max-r settings for good performance.

Qt does not support writing of all image formats. However, by default it does read a large number. I have not explicitly set the output format, so it will try and guess it from the filename. Therefore,
I recommend simply setting the output filename to anything ending in .jpg, as I know that this works.

For a listing of supported file formats, visit [Reading and Writing Image Files](http://doc.trolltech.com/4.5/qimage.html#reading-and-writing-image-files).

As a side note, it is possible to get additional file type support by adding plugins. Again, this is out of the scope of the practical, so I didn't bother including ones.

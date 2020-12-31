/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "hcd.h"

#include <QVector>
#include <QByteArray>
#include <QColor>
#include <cmath>

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))


/****************************************************************************
                __   ___                 __  __           __
     ___  __ __/ /  / (_)___  __ _  ___ / /_/ /  ___  ___/ /__
    / _ \/ // / _ \/ / / __/ /  ' \/ -_) __/ _ \/ _ \/ _  (_-<
   / .__/\_,_/_.__/_/_/\__/ /_/_/_/\__/\__/_//_/\___/\_,_/___/
  /_/

****************************************************************************/


/****************************************************************************
**
** Author: Marc Bowes
**
** Detects circles in the specified QImage
**
****************************************************************************/
QImage HoughCircleDetector::detect(const QImage &source, unsigned int min_r, unsigned int max_r)
{
  QImage binary = edges(source);
  QImage detection = source.convertToFormat(QImage::Format_RGB888);
  
  /* build a vector to hold images in Hough-space for radius 1..max_r, where
  max_r is specified or the maximum radius of a circle in this image */
  if(min_r == 0)
  {
    min_r = 5;
  }
  
  if(max_r == 0)
  {
    max_r = MIN(source.width(), source.height()) / 2;
  }
  
  QVector<Image> houghs(max_r - min_r);
  
  for(unsigned int i = min_r; i < max_r; i++)
  {
    /* instantiate Hough-space for circles of radius i */
    Image &hough = houghs[i - min_r];
    hough.resize(binary.width());
    for(unsigned int x = 0; x < hough.size(); x++)
    {
      hough[x].resize(binary.height());
      for(unsigned int y = 0; y < hough[x].size(); y++)
      {
        hough[x][y] = 0;
      }
    }
    
    /* find all the edges */
    for(unsigned int x = 0; x < binary.width(); x++)
    {
      for(unsigned int y = 0; y < binary.height(); y++)
      {
        /* edge! */
        if(binary.pixelIndex(x, y) == 1)
        {
          accum_circle(hough, QPoint(x, y), i);
        }
      }
    }
    
    /* loop through all the Hough-space images, searching for bright spots, which
    indicate the center of a circle, then draw circles in image-space */
    unsigned int threshold = 4.9 * i;
    for(unsigned int x = 0; x < hough.size(); x++)
    {
      for(unsigned int y = 0; y < hough[x].size(); y++)
      {
        if(hough[x][y] > threshold)
        {
          draw_circle(detection, QPoint(x, y), i, Qt::yellow);
        }
      }
    }
  }
    
  return detection;
}


/****************************************************************************
               _           __                  __  __           __
     ___  ____(_)  _____ _/ /____   __ _  ___ / /_/ /  ___  ___/ /__
    / _ \/ __/ / |/ / _ `/ __/ -_) /  ' \/ -_) __/ _ \/ _ \/ _  (_-<
   / .__/_/ /_/|___/\_,_/\__/\__/ /_/_/_/\__/\__/_//_/\___/\_,_/___/
  /_/

****************************************************************************/


/****************************************************************************
**
** Author: Marc Bowes
**
** Accumulates a circle on the specified image at the specified position with
** the specified radius, using the midpoint circle drawing algorithm
**
** Adapted from: http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
**
****************************************************************************/
void HoughCircleDetector::accum_circle(Image &image, const QPoint &position, unsigned int radius)
{
  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x = 0;
  int y = radius;
  
  accum_pixel(image, QPoint(position.x(), position.y() + radius));
  accum_pixel(image, QPoint(position.x(), position.y() - radius));
  accum_pixel(image, QPoint(position.x() + radius, position.y()));
  accum_pixel(image, QPoint(position.x() - radius, position.y()));
  
  while(x < y)
  {
    if(f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    
    x++;
    ddF_x += 2;
    f += ddF_x;
    
    accum_pixel(image, QPoint(position.x() + x, position.y() + y));
    accum_pixel(image, QPoint(position.x() - x, position.y() + y));
    accum_pixel(image, QPoint(position.x() + x, position.y() - y));
    accum_pixel(image, QPoint(position.x() - x, position.y() - y));
    accum_pixel(image, QPoint(position.x() + y, position.y() + x));
    accum_pixel(image, QPoint(position.x() - y, position.y() + x));
    accum_pixel(image, QPoint(position.x() + y, position.y() - x));
    accum_pixel(image, QPoint(position.x() - y, position.y() - x));
  }
}

/****************************************************************************
**
** Author: Marc Bowes
**
** Accumulates at the specified position
**
****************************************************************************/
void HoughCircleDetector::accum_pixel(Image &image, const QPoint &position)
{
  /* bounds checking */
  if(position.x() < 0 || position.x() >= image.size() ||
     position.y() < 0 || position.y() >= image[position.x()].size())
  {
    return;
  }
  
  image[position.x()][position.y()]++;
}

/****************************************************************************
**
** Author: Marc Bowes
**
** Draws a circle on the specified image at the specified position with
** the specified radius, using the midpoint circle drawing algorithm
**
** Adapted from: http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
**
****************************************************************************/
void HoughCircleDetector::draw_circle(QImage &image, const QPoint &position, unsigned int radius, const QColor &color)
{
  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x = 0;
  int y = radius;
  
  draw_pixel(image, QPoint(position.x(), position.y() + radius), color);
  draw_pixel(image, QPoint(position.x(), position.y() - radius), color);
  draw_pixel(image, QPoint(position.x() + radius, position.y()), color);
  draw_pixel(image, QPoint(position.x() - radius, position.y()), color);
  
  while(x < y)
  {
    if(f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    
    x++;
    ddF_x += 2;
    f += ddF_x;
    
    draw_pixel(image, QPoint(position.x() + x, position.y() + y), color);
    draw_pixel(image, QPoint(position.x() - x, position.y() + y), color);
    draw_pixel(image, QPoint(position.x() + x, position.y() - y), color);
    draw_pixel(image, QPoint(position.x() - x, position.y() - y), color);
    draw_pixel(image, QPoint(position.x() + y, position.y() + x), color);
    draw_pixel(image, QPoint(position.x() - y, position.y() + x), color);
    draw_pixel(image, QPoint(position.x() + y, position.y() - x), color);
    draw_pixel(image, QPoint(position.x() - y, position.y() - x), color);
  }
}

/****************************************************************************
**
** Author: Marc Bowes
**
** Draws at the specified position
**
****************************************************************************/
void HoughCircleDetector::draw_pixel(QImage &image, const QPoint &position, const QColor &color)
{
  /* bounds checking */
  if(position.x() < 0 || position.x() >= image.width() ||
     position.y() < 0 || position.y() >= image.height())
  {
    return;
  }
  
  image.setPixel(position, color.rgb());
}

/****************************************************************************
**
** Author: Marc Bowes
**
** Detects edges in the specified QImage
**
****************************************************************************/
QImage HoughCircleDetector::edges(const QImage &source)
{
  /* initialisation */
  QImage binary = QImage(source.size(), QImage::Format_Mono);
  
  /*** Sobel edge detection ***/
  
  /* Set up Lx, Ly */
  QVector<QByteArray> Lx(3), Ly(3);
  
  Lx[0][0] = -1;  Lx[0][1] = +0;  Lx[0][2] = +1;
  Lx[1][0] = -2;  Lx[1][1] = +0;  Lx[1][2] = +2;
  Lx[2][0] = -1;  Lx[2][1] = +0;  Lx[2][2] = +1;
  
  Ly[0][0] = +1;  Ly[0][1] = +2;  Ly[0][2] = +1;
  Ly[1][0] = +0;  Ly[1][1] = +0;  Ly[1][2] = +0;
  Ly[2][0] = -1;  Ly[2][1] = -2;  Ly[2][2] = -1;
  
  for(unsigned int x = 0; x < source.width(); x++)
  {
    for(unsigned int y = 0; y < source.height(); y++)
    {
      double new_x = 0.0, new_y = 0.0;
      
      /* gradient */
      for(int i = -1; i <= 1; i++)
      {
        for(int j = -1; j <= 1; j++)
        {
          /* these are offset co-ords */
          int _x = x + i;
          int _y = y + j;
          
          /* bounds checking */
          if (_x < 0)                     _x = -_x;
          else if (_x >= source.width())  _x = 2 * source.width() - _x - 2;
          
          if (_y < 0)                     _y = -_y;
          else if (_y >= source.height()) _y = 2 * source.height() - _y - 2;
          
          /* accumulate */
          int gray = qGray(source.pixel(_x, _y));
          new_x += static_cast<signed char>(Lx[i + 1][j + 1]) * gray;
          new_y += static_cast<signed char>(Ly[i + 1][j + 1]) * gray;
        }
      }
      
      /* using 128 as a threshold, decide if the steepness is sufficient (= edge = 1) */
      int pixel = sqrt(pow(new_x, 2) + pow(new_y, 2)) > 128 ? 1 : 0;
      binary.setPixel(x, y, pixel);
    }
  }
  
  return binary;
}


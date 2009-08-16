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
QImage HoughCircleDetector::detect(const QImage &source)
{
  QImage binary = edges(source);
  QImage detection(source);
    
  /* build a vector to hold images in Hough-space for radius 1..max_r, where
  max_r is the maximum radius of a circle in this image */
  unsigned int max_r = MIN(source.width(), source.height()) / 2; max_r = 35;
  QVector<QImage> houghs(max_r);
  for(unsigned int i = 1; i < max_r; i++)
  {
    /* instantiate Hough-space for circles of radius i, with a 24-bit image of
    dimensions equal to the source image, filled with black */
    houghs[i] = QImage(source.size(), QImage::Format_RGB888); /* 24 bit */
    QImage &hough = houghs[i];
    hough.fill(NULL);
    
    /* find all the edges */
    for(unsigned int x = 0; x < binary.width(); x++)
    {
      for(unsigned int y = 0; y < binary.height(); y++)
      {
        /* edge! */
        if(binary.pixelIndex(x, y) == 1)
        {
          circle(hough, QPoint(x, y), i);
        }
      }
    }
  }
  
  /* loop through all the Hough-space images, searching for bright spots, which
  indicate the center of a circle, then draw circles in image-space */
  for(unsigned int i = 1; i < max_r; i++)
  {
    QImage &hough = houghs[i];
    unsigned int threshold = -1;
    
    /* find all the bright spots */
    for(unsigned int x = 0; x < binary.width(); x++)
    {
      for(unsigned int y = 0; y < binary.height(); y++)
      {
        if(binary.pixel(x, y) > threshold)
        {
          circle(detection, QPoint(x, y), i, true);
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
** Draws a circle on the specified image at the specified position with the
** specified radius, using the midpoint circle drawing algorithm
**
** Adapted from: http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
**
****************************************************************************/
void HoughCircleDetector::circle(QImage &image, const QPoint &position, unsigned int radius,
  bool mode)
{
  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x = 0;
  int y = radius;
  
  draw(image, QPoint(position.x(), position.y() + radius), mode);
  draw(image, QPoint(position.x(), position.y() - radius), mode);
  draw(image, QPoint(position.x() + radius, position.y()), mode);
  draw(image, QPoint(position.x() - radius, position.y()), mode);
  
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
    
    draw(image, QPoint(position.x() + x, position.y() + y), mode);
    draw(image, QPoint(position.x() - x, position.y() + y), mode);
    draw(image, QPoint(position.x() + x, position.y() - y), mode);
    draw(image, QPoint(position.x() - x, position.y() - y), mode);
    draw(image, QPoint(position.x() + y, position.y() + x), mode);
    draw(image, QPoint(position.x() - y, position.y() + x), mode);
    draw(image, QPoint(position.x() + y, position.y() - x), mode);
    draw(image, QPoint(position.x() - y, position.y() - x), mode);
  }
}

/****************************************************************************
**
** Author: Marc Bowes
**
** Accumulates intensity at the specified position
**
****************************************************************************/
void HoughCircleDetector::draw(QImage &image, const QPoint &position, bool mode)
{
  /* bounds checking */
  if(position.x() < 0 || position.x() >= image.width() ||
     position.y() < 0 || position.y() >= image.height())
  {
    return;
  }
  
  if(mode)
  {
    image.setPixel(position, Qt::yellow);
  }
  else
  {
    QColor color(image.pixel(position));
    color.setRgb(color.red() + 1, color.green() + 1, color.blue() + 1);
    image.setPixel(position, color.rgb());
  }
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
          new_x += Lx[i + 1][j + 1] * gray;
          new_y += Ly[i + 1][j + 1] * gray;
        }
      }
      
      /* using 128 as a threshold, decide if the steepness is sufficient (= edge = 1) */
      int pixel = sqrt(pow(new_x, 2) + pow(new_y, 2)) > 128 ? 1 : 0;
      binary.setPixel(x, y, pixel);
    }
  }
  
  return binary;
}


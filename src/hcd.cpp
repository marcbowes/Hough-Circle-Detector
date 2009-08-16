/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include "hcd.h"

#include <QVector>
#include <QByteArray>
#include <cmath>

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
  
  return binary;
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


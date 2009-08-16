/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QImage>

class HoughCircleDetector
{
  public: /* class */
  
    HoughCircleDetector() {}
   ~HoughCircleDetector() {}
  
  public: /* methods */
  
    QImage detect(const QImage &source);
  
  private: /* methods */
  
    QImage edges(const QImage &source);
};


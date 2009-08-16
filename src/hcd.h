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
  
    void accumulate(QImage &image, const QPoint &position);
    void circle(QImage &image, const QPoint &position, unsigned int radius);
    QImage edges(const QImage &source);
};


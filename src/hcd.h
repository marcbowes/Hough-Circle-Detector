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
  
    void circle(QImage &image, const QPoint &position, unsigned int radius,
      bool mode=false);
    void draw(QImage &image, const QPoint &position, bool mode);
    QImage edges(const QImage &source);
};


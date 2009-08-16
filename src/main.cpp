/****************************************************************************
**
** For Copyright & Licensing information, see COPYRIGHT in project root
**
****************************************************************************/

#include <QString>
#include <QRegExp>
#include <QDebug>

#include "hcd.h"

/****************************************************************************
**
** Author: Marc Bowes
**
** Arguments: --source=file [--output=file]
**
****************************************************************************/
int main(int argc, char **argv)
{
  /*** Process command line inputs ***/
  
  QString source, output;
  
  /* .. I guess this could be done in a more general sense (with a QMap..) but
  I think the extra effort for just two arguments isn't worth it */
  QRegExp rx_source("^--source=(.*)$");
  QRegExp rx_output("^--output=(.*)$");
  for(int i = 0; i < argc; i++)
  {
    QString arg = argv[i];
    
    if(rx_source.indexIn(arg) != -1)
    {
      source = rx_source.cap(1);
    }
    else if(rx_output.indexIn(arg) != -1)
    {
      output = rx_output.cap(1);
    }
  }
  
  if(source.isEmpty())
  {
    qDebug() << "Missing source file, please set with `--source=file`";
    return 1;
  }
  
  QImage source_image(source);
  if (source_image.isNull())
  {
    qDebug() << "Could not load source file";
    return 1;
  }
  
  if(output.isEmpty())
  {
    output = QString("%1.out").arg(source);
    
    qDebug() << "Missing output file, assuming" << output << 
    "please kill the process if you do not agree with this decision" <<
    "(rerun with `--output=file`)";
  }
  
  /*** Run the circle detection on the source file, and write the results out to
  the specified output file ***/
  
  HoughCircleDetector hcd;
  QImage result = hcd.detect(source_image);
  qDebug() << "saving.." << result.save(output);
}


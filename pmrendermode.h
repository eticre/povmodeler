//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2001-2006 by Andreas Zehender
    email                : zehender@kde.org
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#ifndef PMRENDERMODE_H
#define PMRENDERMODE_H




#include <QList>
#include <QStringList>
#include <QtXml/QDomElement>
#include "pmradiosity.h"
#include <QObject>

/**
 * Class that represents the render options for povray
 *
 * See povray documentation, output options.
 */
class PMRenderMode
{
public:
   /**
    * Sampling method for antialiasing. See povray documentation.
    */
   enum AASamplingMethod { AntialiasingNonRecursive = 0, AntialiasingRecursive = 1 };
   
   /**
    * Default constructor
    */
   PMRenderMode();
   /**
    * Reads the attributes from the QDomElement
    */
   PMRenderMode( const QDomElement& e );

   void setDescription( const QString& descr ) { m_description = descr; }
   QString description() const { return m_description; }
   
   void setHeight( int height );
   int height() const { return m_height; }
   void setWidth( int width );
   int width() const { return m_width; }

   void setSubSection( bool on ) { m_subSection = on; }
   bool subSection() const { return m_subSection; }
   void setStartRow( double startRow );
   double startRow() const { return m_startRow; }
   void setEndRow( double endRow );
   double endRow() const { return m_endRow; }
   void setStartColumn( double startColumn );
   double startColumn() const { return m_startColumn; }
   void setEndColumn( double endColumn );
   double endColumn() const { return m_endColumn; }

   void setQuality( int quality );
   int quality() const { return m_quality; }
   
   void setRadiosity( bool on ) { m_radiosity = on; }
   bool radiosity() const { return m_radiosity; }

   void setAntialiasing( bool on ) { m_antialiasing = on; }
   bool antialiasing() const { return m_antialiasing; }
   void setSamplingMethod( int method );
   int samplingMethod() const { return m_samplingMethod; }
   void setAntialiasingThreshold( double t ) { m_antialiasThreshold = t; }
   double antialiasingThreshold() const { return m_antialiasThreshold; }
   void setAntialiasingJitter( bool on ) { m_antialiasJitter = on; }
   bool antialiasingJitter() const { return m_antialiasJitter; }
   void setAntialiasingJitterAmount( double amount ) { m_antialiasJitterAmount = amount; }
   double antialiasingJitterAmount() const { return m_antialiasJitterAmount; }
   void setAntialiasingDepth( int depth );
   int antialiasingDepth() const { return m_antialiasDepth; }

   void setAlpha( bool on ) { m_alpha = on; }
   bool alpha() const { return m_alpha; }
   
   bool m_preview;
   /**
    * Returns the settings as povray command line switches
    */
   QStringList commandLineSwitches() const;
   /**
    * Saves the data
    */
   void serialize( QDomElement& e ) const;

signals:
   //void s_radYN();
   
private:
   void init();
   
   QString m_description;
   int m_height, m_width;
   bool m_subSection;
   double m_startRow, m_endRow, m_startColumn, m_endColumn;
   
   int m_quality;
   bool m_radiosity;
   bool m_antialiasing;
   int m_samplingMethod;
   double m_antialiasThreshold;
   bool m_antialiasJitter;
   double m_antialiasJitterAmount;
   int m_antialiasDepth;

   bool m_alpha;

};

typedef QList<PMRenderMode> PMRenderModeList;

#endif

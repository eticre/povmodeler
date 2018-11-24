/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Andreas Zehender
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

#ifndef PMPOVRAY31_FORMAT_H
#define PMPOVRAY31_FORMAT_H



#include "pmpovrayformat.h"

/**
 * Description class for POV-Ray 3.1
 */
class PMPovray31Format : public PMPovrayFormat
{
public:
   /**
    * Default constructor
    */
   PMPovray31Format();
   /**
    * Destructor
    */
   virtual ~PMPovray31Format();

   /** */
   virtual QString name() const { return "povray31"; }
   /** */
   virtual QString description() const { return "POV-Ray 3.1"; }
   /** */
   virtual int services() const { return AllServices; }
   /** */
   virtual PMParser* newParser( PMPart*, QIODevice* ) const;
   /** */
   virtual PMParser* newParser( PMPart*, const QByteArray& ) const;
   /** */
   virtual PMSerializer* newSerializer( QIODevice* );
   /** */
   virtual PMRenderer* newRenderer( PMPart* ) const;
   /** */
   virtual QString mimeType() const;
   /** */
   virtual QStringList importPatterns() const;
   /** */
   virtual QStringList exportPatterns() const;
};

#endif

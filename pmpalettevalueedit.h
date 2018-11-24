//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2002 by Luis Passos Carvalho
    email                : lpassos@mail.telepac.pt
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/


#ifndef PMPALETTEVALUEEDIT_H
#define PMPALETTEVALUEEDIT_H



#include "pmdialogeditbase.h"

class PMFloatEdit;
class PMIntEdit;

/**
 * Dialog edit class for @ref PMPaletteValue.
 */
class PMPaletteValueEdit: public QWidget
{
   Q_OBJECT
public:
   /** */
   PMPaletteValueEdit( QWidget* parent );
   /** */
   void setIndex( int idx );
   /** */
   int index();
   /** */
   void setValue( double val );
   /** */
   double value();
   /**
    * Returns true is both fields are valid numbers
    */
   bool isDataValid();
   /** */
   void setReadOnly( bool yes = true );
   /** */
   virtual void setEnabled( bool yes );

signals:
   /** */
   void dataChanged();

private:
   PMIntEdit* m_pIndexEdit;
   PMFloatEdit* m_pValueEdit;
};
   
#endif

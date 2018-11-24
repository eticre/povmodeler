//-*-C++-*-
/*
**************************************************************************
                                 description
                             --------------------
    copyright            : (C) 2003 by Leon Pennington
    email                : leon@leonscape.co.uk
**************************************************************************

**************************************************************************
*                                                                        *
*  This program is free software; you can redistribute it and/or modify  *
*  it under the terms of the GNU General Public License as published by  *
*  the Free Software Foundation; either version 2 of the License, or     *
*  (at your option) any later version.                                   *
*                                                                        *
**************************************************************************/
#ifndef PMHEIGHTFIELDROAM_H
#define PMHEIGHTFIELDROAM_H

class QString;

/**
 * ROAM display class for @ref PMHeightField
*/
class PMHeightFieldROAM
{
   int m_size;
   int m_numPoints;
   int m_usedPoints;
   int m_numLines;
   int m_numNodes;
   bool m_fail;
   int m_maxLevel;
   int m_displayDetail;
   int m_waterLevel;
   bool m_mapMod;
   bool m_levelMod;

   /**
    * Point Structure holds most of the info
    */
   struct pointStructure
   {
      unsigned short hgt;
      pointStructure* lines[ 8 ];
      int pos;
      bool used;
   };
   /**
    * The points array
    */
   pointStructure* m_pPoints;

   /**
    * The Triangle Node structure used in the ROAM algorithm
    */
   struct triNodeStructure
   {
      triNodeStructure* lchd;
      triNodeStructure* rchd;
      triNodeStructure* base;
      triNodeStructure* lnbr;
      triNodeStructure* rnbr;
      int vari;
      bool split;
   };
   /**
    * Tree Root node
    */
   triNodeStructure* m_pTree;
   /**
    * Next available node
    */
   triNodeStructure* m_pNextNode;

   /**
    * Loads and formats the image to the correct size, creates the points
    * array. Then fills the points array with the heights and zero's the
    * rest of the info
    * @param filename The name of the file to load
    * @return true if succesful false it it fails
    */
    bool imageToData( const QString &fileName );

   /**
    * Sets the Maximum Level of the tree.
    */
   void calcLevel();

   /**
    * Generates the Variance for each node.
    * @param current The current node
    * @param x1-y1 The position of the first corner
    * @param x2-y2 The position of the second corner
    * @param x3-y3 The position of the third corner
    * @param level The current level within a tree
    */
   void varNode( triNodeStructure* current,
                 int x1, int y1,
                 int x2, int y2,
                 int x3, int y3,
                 int level );

   /**
    * Generates the Split for the Tree.
    * @param current The current node
    * @param level The current level within a tree
    */
   void sptNode( triNodeStructure* current, int level );

   /**
    * Request the splitting of a node, checks too see if it can be
    * Split or if its base neighbour requires splitting.
    * @param current node to split
    */
   void split( triNodeStructure* current );

   /**
    * Counts up the lines and points in a model, and sets the points
    * structure ready for returning
    * @param current The current node
    * @param x1-y1 The position of the first corner
    * @param x2-y2 The position of the second corner
    * @param x3-y3 The position of the third corner
    */
   void pntNode( triNodeStructure* current,
                 int x1, int y1,
                 int x2, int y2,
                 int x3, int y3 );

   /**
    * Adds a line makes sure that this line does not already exist
    * @param pts1 The start point in the line
    * @param pts2 The end point of the line
    */
   void addLine( pointStructure* pts1, pointStructure* pts2 );

   /**
    * creates the points array and clears it
    * @return true if succesful
    */
   bool createPoints();
   /**
    * Clears some of the points data for recalculation or all
    * of it for initialization
    * @param all true if all the data is to be cleared
    */
   void clearPoints( bool all = false );

   /**
    * creates the nodes array and clears it
    * @return true if succesful
    */
   bool createNodes();
   /**
    * Clears nodes for reuse in splitting recalculation or
    * all of the data for initialization and variance
    * @param all true if all the data is to be cleared
    */
   void clearNodes( bool all = false );

   /**
    * Sets the height of a point
    * @param x the position of the point in X
    * @param y the position of the point in Y
    * @param hgt the new height
    */
   void setHeight( int x, int y, unsigned short hgt ) const
         { m_pPoints[ x + ( y * m_size ) ].hgt = hgt; }

public:
   /**
    * Constructor for class
    * @param fileName Source file for the map
    */
   PMHeightFieldROAM( const QString &fileName );
   /**
    * Class Destructor relases all the memory
    */
   ~PMHeightFieldROAM();

   /**
    * Returns true if there has been a problem
    */
   bool isFailed() { return m_fail; }

   /**
    * Creates the model based on the current map
    * display detail and water level
    */
   void updateModel();

   /**
    * Sets the display detail for the model
    * @param detail The new level of detail
    */
   void setDisplayDetail( int detail );
   /**
    * Returns the current display detail
    */
   int displayDetail() const { return m_displayDetail; }

   /**
    * Sets the water level
    * @param waterLevel the water level
    */
   void setWaterLevel( double m_waterLevel );
   /**
    * Returns the current water level
    */
   double waterLevel() const;

   /**
    * Return The size of the map in either direction
    */
   int size() const { return m_size; }
   /**
    * Return The total number of points in the model
    */
   int numPoints() const { return m_numPoints; }
   /**
    * Return The number of used points
    */
   int usedPoints() const { return m_usedPoints; }
   /**
    * Return The number of lines
    */
   int numLines() const { return m_numLines; }
   /**
    * Return the number of nodes
    */
   int numNodes() const { return m_numNodes; }

   /**
    * Returns a height of a point
    * @param x the position of the point in X
    * @param y the position of the point in Y
    * @param waterLevel whether to return a point offset by water level
    * @return the height of the point
    */
   unsigned short height( int x, int y, bool waterLevel = false  ) const;

   /**
    * Determines if the point is used
    * @param x The position of the point on the x axis.
    * @param y The position of the point on the y axis.
    * @return true if the point is used else false
    */
   bool usedPoint( int x, int y ) const
         { return m_pPoints[ x + ( y * m_size ) ].used; }

   /**
    * Gets the used postion of a point
    * @param x The position of the point on the x axis.
    * @param y The position of the point on the y axis.
    * @return the used position
    */
   int posPoint( int x, int y ) const
         { return m_pPoints[ x + ( y * m_size ) ].pos; }

   /**
    * Returns the used position of a point at the end point of a line.
    * @param x The position of the start point on the x axis.
    * @param y The position of the start point on the y axis.
    * @param line The Line Index
    * @return The used positon of the end point
    */
   int endPoint( int x, int y, int line ) const
         { return m_pPoints[ x + ( y * m_size ) ].lines[ line ]->pos; }

   /**
    * Returns whether this line exists
    * @param x The position of the start point on the x axis.
    * @param y The position of the start point on the y axis.
    * @param line The Line Index
    * @return Whether the line exists
    */
   bool lineExist( int x, int y, int line ) const;

};

#endif

/*
 * @(#)MBroInit.h
 *
 * Copyright 2000-2001, Aaron Ardiri     (mailto:aaron@ardiri.com)
 *                      Charles Kerchner (mailto:timberdogsw@hotmail.com)
 * All rights reserved.
 *
 * This file was generated as part of the "maryobros" program developed 
 * for the Palm Computing Platform designed by Palm: 
 *
 *   http://www.palm.com/ 
 *
 * The contents of this file is confidential and proprietrary in nature 
 * ("Confidential Information"). Redistribution or modification without 
 * prior consent of the original author is prohibited. 
 */

//
// Maryo Bros - Initialization and Destruction
//
// Written by Chip Kerchner
//

//
// Function prototypes
//

// Initialization routines
extern Boolean InitializeGraphics(void) __DEVICE__;

// Destruction routines
extern void DestroyGraphics(void) __DEVICE__;
extern void ReleaseData(register MemPtr dataPtr, register MemHandle handle, register Boolean free) __DEVICE__;

//
// Global varaibles
//

//
// Definitions
//

#define USE_VGA_COLORS

#define baseResourceNum			    4000

// Resource definitions
#define maskResource			      50

#define charResourceNum			    (0 + baseResourceNum)
#define charResourceID			    'char'

#define colorResourceNum		    (25 + baseResourceNum)
#define colorResourceID			    'ctbl'

#define charCount				        128

#define piperResourceNum		    (100 + baseResourceNum)
#define piperResourceID			    'pipr'

#define piperNumSprites		    	10

#define objectWidth				      10
#define objectHeight			      10
#define objectMaxHeight		    	13
#define objectCheckWidth	    	((objectWidth + (4 - 1)) / 4)

#define animateWidth		      	objectWidth
#define animateHeight		      	objectHeight

#define piperWidth			      	objectWidth
#define piperHeight			      	13

#define turtleResourceNum	    	(200 + baseResourceNum)
#define turtleResourceID	    	'turt'

#define turtleWidth			      	objectWidth
#define turtleHeight		      	objectHeight

#define turtleNumSprites	    	10

#define crabResourceNum		    	(300 + baseResourceNum)
#define crabResourceID		    	'crab'

#define crabWidth			        	objectWidth
#define crabHeight		      		objectHeight

#define crabNumSprites	    		10

#define fighterFlyResourceNum	  (400 + baseResourceNum)
#define fighterFlyResourceID	  'ffly'

#define fighterFlyWidth		    	objectWidth
#define fighterFlyHeight	    	objectHeight

#define fighterFlyNumSprites  	6

#define coinResourceNum		    	(500 + baseResourceNum)
#define coinResourceID		    	'coin'

#define coinWidth			        	objectWidth
#define coinHeight		      		objectHeight

#define coinNumSprites		    	14

#define iceMachineResourceNum	  (600 + baseResourceNum)
#define iceMachineResourceID	  'icem'

#define iceMachineWidth		    	objectWidth
#define iceMachineHeight	    	objectHeight

#define iceMachineNumSprites  	5

#define fireBallResourceNum	  	(700 + baseResourceNum)
#define fireBallResourceID	  	'firb'

#define fireBallWidth			      objectWidth
#define fireBallHeight		    	objectHeight

#define fireBallNumSprites		  9

#define paintBallResourceNum	  (800 + baseResourceNum)
#define paintBallResourceID		  'pntb'

#define paintBallWidth			    objectWidth
#define paintBallHeight			    objectHeight

#define paintBallNumSprites		  9

#define icicleResourceNum		    (900 + baseResourceNum)
#define icicleResourceID		    'icic'

#define icicleWidth				      objectWidth
#define icicleHeight			      objectHeight

#define icicleNumSprites		    6

#define powResourceNum			    (1000 + baseResourceNum)
#define powResourceID			      'powb'

#define powWidth				        objectWidth
#define powHeight				        objectHeight

#define powNumSprites			      3

#define flipResourceNum			    (1100 + baseResourceNum)
#define flipResourceID			    'flip'

#define flipWidth				        objectWidth
#define flipHeight				      5

#define flipNumSprites			    1

#define flipMaskBits			      (flipWidth * flipHeight)
#define flipMaskSize			      ((objectXBytes * flipHeight) + maskRectSize)
#define flipMaskCompactSize		  (((flipMaskBits + bitsInByte - 1) / bitsInByte) + maskRectSize)

#define flipColorMaskSize		    ((objectColorXPixels * flipHeight) + maskRectSize)

#define animateResourceNum		  (2000 + baseResourceNum)
#define animateResourceID		    'anim'

#define animateNumSprites		    61

#define musicResourceNum		    (2100 + baseResourceNum)
#define musicResourceID			    'musc'

#define musicNumSamples		    	30

// Color definitions
#define numColorTables			    17
#define sizeDefinedColor		    3
//#define startDefinedColor       0xE7
#define startDefinedColor		    0x00

#ifdef USE_VGA_COLORS
#define numDefinedColors		    16

#define whiteColor				      (0  + startDefinedColor)
#define blackColor				      (15 + startDefinedColor)

#define textFGColor				      whiteColor
#define textBGColor				      blackColor
#else
#define numDefinedColors	    	24

#define whiteColor				      (0  + startDefinedColor)
#define blackColor				      (5  + startDefinedColor)

#define textFGColor				      whiteColor
#define textBGColor			      	(3 + startDefinedColor)
#endif

#define transparentColor		    (numDefinedColors + startDefinedColor)

#define textFGEntry				      (screenPixels - 1)
#define textBGEntry			      	1

#define	gameColors			      	Color_Swap

// Floor Animation Tile definitions
#define floorStart				      3
#define floorEnd				        32

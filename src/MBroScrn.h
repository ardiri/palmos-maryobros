/*
 * @(#)MBroScrn.h
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
// Maryo Bros - Graphics Information
//
// Written by Chip Kerchner
//

//
// Definitions
//

#define USE_4_COLOR_GRAYSCALE
//#define USE_16_COLOR_GRAYSCALE

// Size definitions

#define screenWidth				      160
#define screenHeight			      160
#ifdef USE_16_COLOR_GRAYSCALE
#define screenDepth				      4
#else
#define screenDepth				      2
#endif
#define screenPixels			      (bitsInByte / screenDepth)
#define screenPad				        (sizeof(UInt32) * screenPixels)
#define screenColors		      	(1 << screenDepth)
#define screenXBytes		      	((UInt16)(screenWidth) / screenPixels)

//#define USE_8BIT_COLOR
#define USE_4BIT_COLOR

#ifdef USE_8BIT_COLOR
#define screenColorDepth	    	8
#else
#define screenColorDepth	    	4
#endif
#define screenColorPixels	    	(bitsInByte / screenColorDepth)
#define screenColorPad		    	(sizeof(UInt32) * screenColorPixels)
#define screenColorXBytes	    	((UInt16)(screenWidth) / screenColorPixels)

// Color Table definitions
#define colorTableIndex			    1
#define colorTableSize		    	(sizeof(UInt8) * screenDepth)

#define tableSize				        (1 << bitsInByte)

// Object definitions
#define objectXPixels	    	  	((objectWidth + screenPixels - 1) & ~(screenPixels - 1))
#define objectXBytes	    	  	(((objectWidth * screenDepth) + bitsInByte - 1) / bitsInByte)
#define objectSrcBytes    			(objectXPixels / screenPixels)

#define objectColorXPixels	  	((objectWidth + screenColorPixels - 1) & ~(screenColorPixels - 1))
#define objectColorXBytes	    	(((objectWidth * screenColorDepth) + bitsInByte - 1) / bitsInByte)
#define objectColorSrcBytes	  	(objectColorXPixels / screenColorPixels)

// Mask definitions
#define maskRectSize	      		4
#define maskRectBit			      	4
#define maskRectCompactSize	  	(((maskRectSize * maskRectBit) + bitsInByte - 1) / bitsInByte)
#define objectMaskBits		    	(objectWidth * objectHeight)
#define maxMaskBits				      (objectWidth * objectMaxHeight)

#define objectMaskSize		    	((objectXBytes * objectHeight) + maskRectSize)
#define maxMaskSize				      ((objectXBytes * objectMaxHeight) + maskRectSize)
#define objectMaskCompactSize 	(((objectMaskBits + bitsInByte - 1) / bitsInByte) + maskRectCompactSize)
#define maxMaskCompactSize	  	(((maxMaskBits + bitsInByte - 1) / bitsInByte) + maskRectCompactSize)

#define objectColorMaskSize	  	((objectColorSrcBytes * objectHeight) + maskRectSize)
#define maxColorMaskSize		    ((objectColorSrcBytes * objectMaxHeight) + maskRectSize)

#define animateMaskBits		    	(animateWidth * animateHeight)

#define animateColorXPixels	  	((animateWidth + screenColorPixels - 1) & ~(screenColorPixels - 1))
#define animateColorXBytes	  	(((animateWidth * screenColorDepth) + bitsInByte - 1) / bitsInByte)
#define animateColorSrcBytes  	(animateColorXPixels / screenColorPixels)

#define animateColorMaskSize	  ((animateColorSrcBytes * animateHeight) + maskRectSize)

#define animateMaskSize		    	((objectXBytes * animateHeight) + maskRectSize)
#define animateMaskCompactSize	(((animateMaskBits + bitsInByte - 1) / bitsInByte) + maskRectCompactSize)

#define maskValue				        (screenColors - 1)
#define maskRectMask		      	((1 << maskRectBit) - 1)
#define maskColorValue	    		((1 << screenColorDepth) - 1)

// Sprite definitions
#define spriteSize			       	(((objectWidth + bitsInByte - 1) / screenPixels) * objectHeight)
#define spriteColorSize	    		(((objectWidth + bitsInByte - 1) / screenColorPixels) * objectHeight)

// Character definitions
#define charXSize		        		5
#define charYSize		        		5
#define charBits		        		(charXSize * charYSize)

#define charXNum			        	(screenWidth / charXSize)
#define charYNum			        	(screenHeight / charYSize)
#define charNum				        	(charXNum * charYNum)

#define charXPixels		      		((charXSize + screenPixels - 1) & ~(screenPixels - 1))
#define charSrcBytes	      		(charXPixels / screenPixels)
#define charDestBytes	      		((screenWidth / screenPixels) - charSrcBytes)
#define charMaskLeft	      		((1 << ((charXPixels - charXSize) * screenDepth)) - 1)
#define charMaskRight	      		(~((1 << screenDepth) - 1))

#define charColorXPixels	    	((charXSize + screenColorPixels - 1) & ~(screenColorPixels - 1))
#define charColorSrcBytes	    	(charColorXPixels / screenColorPixels)
#define charColorMaskLeft	    	((1 << ((charColorXPixels - charXSize) * screenColorDepth)) - 1)
#define charColorMaskRight    	(~((1 << screenColorDepth) - 1))

#define charMaskSize			      ((charSrcBytes * charYSize) + maskRectSize)
#define charMaskCompactSize		  (((charBits + bitsInByte - 1) / bitsInByte) + maskRectCompactSize)
#define charColorMaskSize		    ((charColorSrcBytes * charYSize) + maskRectSize)

#define charColorXPixels		    ((charXSize + screenColorPixels - 1) & ~(screenColorPixels - 1))
#define charColorDestBytes		  ((screenWidth / screenColorPixels) - charColorSrcBytes)

// Location definitions
#define topScreen				        8
//#define topDisplay              0
#define topDisplay				      topScreen
#define maxPixels				        132
#define maxTitlePixels			    112
#define bottomScreen			      (maxPixels + topScreen)
#define bottomDisplay			      (maxPixels + topScreen - 1)
#define topTile					        (topScreen / charYSize)
#define bottomTitleScreen		    (maxTitlePixels + topScreen)

#define charUpdateYNum			    ((topScreen + maxPixels + charYSize - 1) / charYSize)
#define charUpdate				      (charXNum * charUpdateYNum)

#define charYMax				        27
#define screenYMax				      (charYMax * charYSize)
#define tileYMax				        ((charYMax + 1) * charXNum)
#define tileYMin				        (topTile * charXNum)

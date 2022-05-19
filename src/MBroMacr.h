/*
 * @(#)MBroMacr.h
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
// Maryo Bros - Game Macros
//
// Written by Chip Kerchner
//

//
// Macros
//
#define absSign(var, delta, sign)			\
	if (var > 0) {						        	\
		delta = var;					          	\
		sign = 1;							            \
	} else if (var < 0) {					      \
		delta = -var;				          		\
		sign = -1;					          		\
	} else {							          	  \
		delta = 0;					          		\
		sign = 0;						          	  \
	}

#if 0
#define rectUpdate(x, y, width, height)		\
	if (x < gameVars->updateRect.x1) {	  	\
		gameVars->updateRect.x1 = x;	      	\
	}										                    \
	x += (width - 1);						            \
	if (x > gameVars->updateRect.x2) {		  \
		gameVars->updateRect.x2 = x;		      \
	}										                    \
	if (y < gameVars->updateRect.y1) {		  \
		gameVars->updateRect.y1 = y;		      \
	}										                    \
	y += (height - 1);						          \
	if (y > gameVars->updateRect.y2) {		  \
		gameVars->updateRect.y2 = y;		      \
	}
#else
#define rectUpdate(y, height)				      \
	if (y < gameVars->updateRect.y1) {		  \
		gameVars->updateRect.y1 = y;		      \
	}										                    \
	y += (height - 1);						          \
	if (y > gameVars->updateRect.y2) {		  \
		gameVars->updateRect.y2 = y;		      \
	}
#endif

#define changeSequence(object, seq, siz)	\
	object->draw.sequence = seq;		        \
	object->draw.seqNum   = 0;				      \
	object->size          = siz;

#define adjustAddress(ptr)					  					              \
	if (((UInt8 *)(ptr) >= top) && ((UInt8 *)(ptr) < bottom)) {	\
		(UInt8 *)(ptr) -= adjust;							                    \
  }


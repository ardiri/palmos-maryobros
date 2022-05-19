
/*
 * @(#)palm.inc
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

// variables
typedef struct {

  PreferencesType *prefs;

  Int32           evtTimeOut;
  Int16           timerDiff;
  Int16           ticksPerFrame;
  UInt32          timerPointA;
  UInt32          timerPointB;

} Globals;

// interface
static Boolean mainFormEventHandler(EventType *);
static Boolean gameFormEventHandler(EventType *);
static Boolean infoFormEventHandler(EventType *);
static Boolean dvlpFormEventHandler(EventType *);
static Boolean cfigFormEventHandler(EventType *);
static Boolean grayFormEventHandler(EventType *);
#ifdef PROTECTION_ON
static Boolean regiFormEventHandler(EventType *);
static Boolean rbugFormEventHandler(EventType *);
#endif
static Boolean highFormEventHandler(EventType *);
static Boolean helpFormEventHandler(EventType *);
static Boolean xmemFormEventHandler(EventType *);

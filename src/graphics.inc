
/*
 * @(#)graphics.inc
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

// interface
static void GraphicsClear_asm()                                     __DEVICE__;
static void GraphicsClearLCD_asm()                                  __DEVICE__;
static void GraphicsRepaint_asm()                                   __DEVICE__;
static void GraphicsClear_api()                                     __DEVICE__;
static void GraphicsClearLCD_api()                                  __DEVICE__;
static void GraphicsRepaint_api()                                   __DEVICE__;
static void GraphicsRepaint_handera()                               __DEVICE__;

// global variable structure
typedef struct {

  UInt32    scrDepth;            // the screen depth
  UInt16    scrBits;             // the number of bits needed for depth
  UInt16    scrPixels;           // the number of pixels per byte
  Boolean   scr160x160;          // is screen 160x160 native ASM compatible?

  WinHandle drawWindow;          // the offscreen window
  UInt16    updateY1;            // top update game line
  UInt16    updateY2;            // bottom update game line

  UInt8     scaleTableHoriz1[256];
  UInt8     scaleTableHoriz2[256];
  UInt8     scaleTableHoriz3[256];

  void      (*fnClear)(void);    // function for clearing offscreen window
  void      (*fnClearLCD)(void); // function for clearing LCD offscreen area
  void      (*fnRepaint)(void);  // function for forcing a repaint to LCD

  struct
  {
    Boolean device;              // are we running on a handera device?
    VgaScreenModeType scrMode;   // the current display mode (scale, 1:1 etc)
    VgaRotateModeType scrRotate; // the current display rotation
    BitmapType *offscreenBitmap; // the 160x132 offscreen bitmap (HACK)
  } handera;

} GraphicsGlobals;

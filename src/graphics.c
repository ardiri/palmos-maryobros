
/*
 * @(#)graphics.c
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

#include "palm.h"
#include "graphics.inc"

/**
 * Initialize the Graphics engine.
 *
 * @return true if the graphics initialization was successful, false otherwise
 */
Boolean
GraphicsInitialize()
{
  GraphicsGlobals *globals;
  Boolean result = true;
  UInt16 err;
  UInt32 version;

  // create the globals object, and register it
  globals = (GraphicsGlobals *) MemPtrNew(sizeof(GraphicsGlobals));
  MemSet(globals, sizeof(GraphicsGlobals), 0);
  FtrSet(appCreator, ftrGraphicsGlobals, (UInt32) globals);

  // determine the screen depth
  globals->scrDepth = 1;
  if (DeviceSupportsVersion(romVersion3))
  {
    WinScreenMode(winScreenModeGet, NULL, NULL, &globals->scrDepth, NULL);
  }
  globals->scrPixels = (8 / globals->scrDepth);

  {
    UInt16 count = 0, depth;

    depth = globals->scrPixels;
    while (depth > 1)
    {
      depth >>= 1;
      count++;
    }
    globals->scrBits = count;
  }

  globals->handera.device = 
    (FtrGet(TRGSysFtrID, TRGVgaFtrNum, &version) != ftrErrNoSuchFeature);
  if (globals->handera.device)
  {
    VgaGetScreenMode(&globals->handera.scrMode, &globals->handera.scrRotate);

    // we only care, if in scale-to-fit mode :)
    globals->handera.device =
      ((globals->handera.scrMode == screenModeScaleToFit) &&
       (globals->scrDepth        == 2));

    if (globals->handera.device) {
      register UInt16 i;

      for (i = 0; i < 256; i++) 
      {
                                                                // *INDENT-OFF*
        globals->scaleTableHoriz1[i] =
           (i & 0xc0) |
           ((((i >> 6) + ((i >> 4) & 0x03)) >> 1) << 4) | ((i >> 2) & 0x0f);
        globals->scaleTableHoriz2[i] =
           ((((i >> 6) + ((i >> 4) & 0x03)) >> 1) << 6) | (i & 0x3c) |
           ((((i >> 2) & 0x03) + (i & 0x03)) >> 1);
        globals->scaleTableHoriz3[i] =
           ((i << 2) & 0xf0) |
           (((((i >> 2) & 0x03) + (i & 0x03)) >> 1) << 2) | (i & 0x03);
                                                                // *INDENT-ON*
      }
    }
  }

  // determine screen size (only >= 3.5)
  globals->scr160x160 = true;
  if (DeviceSupportsVersion(romVersion3_5))
  {
    UInt32 width, height;
    UInt32 cpuID;

    // determine the current display mode
    WinScreenMode(winScreenModeGetDefaults, &width, &height, NULL, NULL);
    globals->scr160x160 &= ((width == 160) && (height == 160));

    // lets make sure we are running on a "m68k chip" :)
    FtrGet(sysFtrCreator, sysFtrNumProcessorID, &cpuID);

                                                                // *INDENT-OFF*
    globals->scr160x160 &= ((cpuID == sysFtrNumProcessor328) ||
                            (cpuID == sysFtrNumProcessorEZ) ||
                            (cpuID == sysFtrNumProcessorVZ));
                                                                // *INDENT-ON*
  }

  globals->updateY1 = 0;
  globals->updateY2 = SCREEN_HEIGHT - 1;

  // create the offscreen window
  if (!globals->handera.device)
  {
    globals->drawWindow =
      WinCreateOffscreenWindow(SCREEN_WIDTH, SCREEN_HEIGHT, screenFormat, &err);
  }
  else
  {
    globals->handera.offscreenBitmap =
      BmpCreate(SCREEN_WIDTH, SCREEN_HEIGHT, globals->scrDepth, NULL, &err);
    globals->drawWindow =
      WinCreateBitmapWindow(globals->handera.offscreenBitmap, &err);
  }
  err |= (Boolean)(globals->drawWindow == NULL);

  // something went wrong, display error and exit
  if (err != errNone)
  {
    ApplicationDisplayDialog(xmemForm);
    result = false;
  }

  // no probs, configure pointers
  else
  {
    globals->fnClear = (globals->scr160x160)
      ? (void *)GraphicsClear_asm : (void *)GraphicsClear_api;
    globals->fnClearLCD = (globals->scr160x160)
      ? (void *)GraphicsClearLCD_asm : (void *)GraphicsClearLCD_api;
    globals->fnRepaint = (globals->scr160x160)
      ? (void *)GraphicsRepaint_asm : 
      ((globals->handera.device) 
        ? (void *)GraphicsRepaint_handera : (void *)GraphicsRepaint_api);
  }

  return result;
}

/**
 * Get the draw window pointer (direct access)
 *
 * @return the draw window.
 */
WinHandle
GraphicsGetDrawWindow()
{
  GraphicsGlobals *globals;

  // get a globals reference
  FtrGet(appCreator, ftrGraphicsGlobals, (UInt32 *) & globals);

  return globals->drawWindow;
}

/**
 * Clear the offscreen image.
 */
void
GraphicsClear()
{
  GraphicsGlobals *globals;

  // get a globals reference
  FtrGet(appCreator, ftrGraphicsGlobals, (UInt32 *) & globals);

  // execute the appropriate function
  globals->fnClear();
}

/**
 * Clear the LCD display area (where the graphics area will go)
 */
void
GraphicsClearLCD()
{
  GraphicsGlobals *globals;

  // get a globals reference
  FtrGet(appCreator, ftrGraphicsGlobals, (UInt32 *) & globals);

  // execute the appropriate function
  globals->fnClearLCD();
}

/**
 * Set the top and bottom update lines.
 *
 * @param y1 top update line.
 * @param y2 bottom update line.
 */
void
GraphicsSetUpdate(UInt16 y1,
                  UInt16 y2)
{
  GraphicsGlobals *globals;

  // get a globals reference
  FtrGet(appCreator, ftrGraphicsGlobals, (UInt32 *) & globals);

  globals->updateY1 = y1;
  globals->updateY2 = y2;
}

/**
 * Blit the offscreen image to the screen.
 */
void
GraphicsRepaint()
{
  GraphicsGlobals *globals;

  // get a globals reference
  FtrGet(appCreator, ftrGraphicsGlobals, (UInt32 *) & globals);

  // execute the appropriate function
  globals->fnRepaint();
}

/**
 * Terminate the Graphics engine.
 */
void
GraphicsTerminate()
{
  GraphicsGlobals *globals;

  // get a globals reference
  FtrGet(appCreator, ftrGraphicsGlobals, (UInt32 *)&globals);

  // clean up windows
  if (globals->drawWindow != NULL)
    WinDeleteWindow(globals->drawWindow, false);
  if (globals->handera.device)
    BmpDelete(globals->handera.offscreenBitmap);

  // free globals memory
  MemPtrFree(globals);

  // unregister global data
  FtrUnregister(appCreator, ftrGraphicsGlobals);
}

// 
// 240x240 direct screen display routines [for handera 330 device]
//
// -- Chip Kerchner, 2001
//

/**
 * Blit the offscreen image to the screen, scretching image to handera.
 */
static void
GraphicsRepaint_handera()
{
  GraphicsGlobals *globals;

  // get a globals reference
  FtrGet(appCreator, ftrGraphicsGlobals, (UInt32 *) & globals);

  // blit the buffer to the real screen
  {
#if 0
    UInt8 *ptrLCDScreen, *ptrScreen;
    Int16 loop, y1, y2;

    y1 = globals->updateY1;
    y2 = globals->updateY2;

    ptrLCDScreen = DeviceWindowGetPointer(WinGetDisplayWindow());
    ptrLCDScreen += (60 * ((3 * (SCREEN_START + y1)) >> 1));

    ptrScreen = DeviceWindowGetPointer(globals->drawWindow);
    ptrScreen += ((SCREEN_WIDTH * y1) >> globals->scrBits);

    loop = (y2 - y1 + 1);
                                                                // *INDENT-OFF*
    // push all registers (except a7) on stack
    asm("      movem.l %%d0-%%d7/%%a0-%%a6, -(%%sp)": :);

    // copy inner 160xSCREEN_START from back buffer to screen
    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(ptrLCDScreen));
    asm("      move.l  %0, %%d0": :"g"(loop - 1));
    asm("      move.l  %0, %%d1": :"g"(SCREEN_START + y1));

    asm("      movea.l %%a1, %%a2
               adda.l  #60, %%a2

               btst    #0, %%d1
               bne     ScrLoop2_240x240_odd

         ScrLoop2_240x240:

               moveq   #19, %%d4

         ScrLoop2_240x240_2:

               move.b  (%%a0), %%d1
               move.b  %%d1, %%d2
               lsr.b   #2, %%d1
               and.b   #192, %%d2
               or.b    %%d2, %%d1
               move.b  %%d1, (%%a1)+

               move.w  (%%a0)+, %%d1
               move.b  %%d1, %%d3
               lsr.w   #4, %%d1
               move.b  %%d1, %%d2
               and.b   #252, %%d1
               lsr.b   #2, %%d2
               and.b   #3, %%d2
               or.b    %%d2, %%d1
               move.b  %%d1, (%%a1)+

               move.b  %%d3, %%d2
               lsl.b   #2, %%d3
               and.b   #240, %%d3
               and.b   #15, %%d2
               or.b    %%d2, %%d3
               move.b  %%d3, (%%a1)+

               dbra    %%d4, ScrLoop2_240x240_2

               adda.l  #60, %%a2

               dbra    %%d0, ScrLoop2_240x240_odd
               bra     ScrLoop2_240x240_exit

         ScrLoop2_240x240_odd:

               moveq   #19, %%d4

         ScrLoop2_240x240_2_odd:

               move.b  (%%a0), %%d1
               move.b  %%d1, %%d2
               lsr.b   #2, %%d1
               and.b   #192, %%d2
               or.b    %%d2, %%d1
               move.b  %%d1, (%%a1)+
               move.b  %%d1, (%%a2)+

               move.w  (%%a0)+, %%d1
               move.b  %%d1, %%d3
               lsr.w   #4, %%d1
               move.b  %%d1, %%d2
               and.b   #252, %%d1
               lsr.b   #2, %%d2
               and.b   #3, %%d2
               or.b    %%d2, %%d1
               move.b  %%d1, (%%a1)+
               move.b  %%d1, (%%a2)+

               move.b  %%d3, %%d2
               lsl.b   #2, %%d3
               and.b   #240, %%d3
               and.b   #15, %%d2
               or.b    %%d2, %%d3
               move.b  %%d3, (%%a1)+
               move.b  %%d3, (%%a2)+

               dbra    %%d4, ScrLoop2_240x240_2_odd

               adda.l  #60, %%a1
               adda.l  #60, %%a2

               dbra    %%d0, ScrLoop2_240x240

        ScrLoop2_240x240_exit:                     ": :);
#else
    UInt8 *ptrLCDScreen, *ptrScreen;
    Int16 loop, y1, y2;

    y1 = globals->updateY1 & ~(globals->scrPixels - 1);
    y2 = (((globals->updateY2 >> globals->scrBits) + 1) << globals->scrBits) - 1;

    loop = (y2 - y1 + 1) >> globals->scrBits;

    ptrLCDScreen = DeviceWindowGetPointer(WinGetDisplayWindow());
    ptrLCDScreen += (60 * (((SCREEN_START + y1) * 6) >> globals->scrBits));

    ptrScreen = DeviceWindowGetPointer(globals->drawWindow);
    ptrScreen += ((SCREEN_WIDTH * y1) >> globals->scrBits);
                                                                // *INDENT-OFF*
    // push all registers (except a7) on stack
    asm("      movem.l %%d0-%%d7/%%a0-%%a6, -(%%sp)": :);

    // copy inner 160xSCREEN_START from back buffer to screen
    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(ptrLCDScreen));
    asm("      move.l  %0, %%a3": :"g"(globals->scaleTableHoriz1));
    asm("      move.l  %0, %%a4": :"g"(globals->scaleTableHoriz2));
    asm("      move.l  %0, %%a5": :"g"(globals->scaleTableHoriz3));
    asm("      move.l  %0, %%d0": :"g"(loop - 1));

    asm("      moveq   #0, %%d1
               moveq   #0, %%d2
               moveq   #85, %%d7
               move.w  #170, %%d3

         ScrLoop2_240x240_next0:

               moveq   #19, %%d4

         ScrLoop2_240x240_2_next0:

               move.b  (%%a0), %%d1
               move.b  (%%a3, %%d1.w), %%d1
               move.b  %%d1, (%%a1)+

               move.b  40(%%a0), %%d2
               move.b  (%%a3, %%d2.w), %%d2
               move.b  %%d2, 119(%%a1)

               move.w  %%d1, %%d6
               and.w   %%d2, %%d6
               and.w   %%d7, %%d6
               and.w   %%d3, %%d1
               and.w   %%d3, %%d2
               add.w   %%d1, %%d2
               lsr.w   #1, %%d2
               add.w   %%d6, %%d2
               move.b  %%d2, 59(%%a1)

               move.w  (%%a0), %%d5
               lsr.w   #4, %%d5
               move.b  %%d5, %%d2
               move.b  (%%a4, %%d2.w), %%d1
               move.b  %%d1, (%%a1)+

               move.w  40(%%a0), %%d5
               lsr.w   #4, %%d5
               move.b  %%d5, %%d2
               move.b  (%%a4, %%d2.w), %%d2
               move.b  %%d2, 119(%%a1)

               move.w  %%d1, %%d6
               and.w   %%d2, %%d6
               and.w   %%d7, %%d6
               and.w   %%d3, %%d1
               and.w   %%d3, %%d2
               add.w   %%d1, %%d2
               lsr.w   #1, %%d2
               add.w   %%d6, %%d2
               move.b  %%d2, 59(%%a1)

               move.w  (%%a0)+, %%d5
               move.b  %%d5, %%d1
               move.b  (%%a5, %%d1.w), %%d1
               move.b  %%d1, (%%a1)+

               move.w  38(%%a0), %%d5
               move.b  %%d5, %%d2
               move.b  (%%a5, %%d2.w), %%d2
               move.b  %%d2, 119(%%a1)

               move.w  %%d1, %%d6
               and.w   %%d2, %%d6
               and.w   %%d7, %%d6
               and.w   %%d3, %%d1
               and.w   %%d3, %%d2
               add.w   %%d1, %%d2
               lsr.w   #1, %%d2
               add.w   %%d6, %%d2
               move.b  %%d2, 59(%%a1)

               move.b  78(%%a0), %%d1
               move.b  (%%a3, %%d1.w), %%d1
               move.b  %%d1, 177(%%a1)

               move.b  118(%%a0), %%d2
               move.b  (%%a3, %%d2.w), %%d2
               move.b  %%d2, 297(%%a1)

               move.w  %%d1, %%d6
               and.w   %%d2, %%d6
               and.w   %%d7, %%d6
               and.w   %%d3, %%d1
               and.w   %%d3, %%d2
               add.w   %%d1, %%d2
               lsr.w   #1, %%d2
               add.w   %%d6, %%d2
               move.b  %%d2, 237(%%a1)

               move.w  78(%%a0), %%d5
               lsr.w   #4, %%d5
               move.b  %%d5, %%d2
               move.b  (%%a4, %%d2.w), %%d1
               move.b  %%d1, 178(%%a1)

               move.w  118(%%a0), %%d5
               lsr.w   #4, %%d5
               move.b  %%d5, %%d2
               move.b  (%%a4, %%d2.w), %%d2
               move.b  %%d2, 298(%%a1)

               move.w  %%d1, %%d6
               and.w   %%d2, %%d6
               and.w   %%d7, %%d6
               and.w   %%d3, %%d1
               and.w   %%d3, %%d2
               add.w   %%d1, %%d2
               lsr.w   #1, %%d2
               add.w   %%d6, %%d2
               move.b  %%d2, 238(%%a1)

               move.w  78(%%a0), %%d5
               move.b  %%d5, %%d1
               move.b  (%%a5, %%d1.w), %%d1
               move.b  %%d1, 179(%%a1)

               move.w  118(%%a0), %%d5
               move.b  %%d5, %%d2
               move.b  (%%a5, %%d2.w), %%d2
               move.b  %%d2, 299(%%a1)

               move.w  %%d1, %%d6
               and.w   %%d2, %%d6
               and.w   %%d7, %%d6
               and.w   %%d3, %%d1
               and.w   %%d3, %%d2
               add.w   %%d1, %%d2
               lsr.w   #1, %%d2
               add.w   %%d6, %%d2
               move.b  %%d2, 239(%%a1)

               dbra    %%d4, ScrLoop2_240x240_2_next0

               lea     120(%%a0), %%a0
               lea     300(%%a1), %%a1

               dbra    %%d0, ScrLoop2_240x240_next0

        ScrLoop2_240x240_exit:                     ": :);
#endif

    // pop all registers (except a7) off stack
    asm("      movem.l (%%sp)+, %%d0-%%d7/%%a0-%%a6": :);
                                                                // *INDENT-ON*
  }
}

// 
// 160x160 direct screen display routines [speed on 160x160 devices]
//
// -- Aaron Ardiri, 2000
//

/**
 * Clear the offscreen image.
 */
static void
GraphicsClear_asm()
{
  GraphicsGlobals *globals;

  // get a globals reference
  FtrGet(appCreator, ftrGraphicsGlobals, (UInt32 *) & globals);

  // clear the buffer
  {
    UInt16 ptrSize;

    // how much memory is being used?
    ptrSize = ((SCREEN_WIDTH * SCREEN_HEIGHT) >> globals->scrBits);

    // clear the memory
    MemSet(DeviceWindowGetPointer(globals->drawWindow), ptrSize, 0);
  }
}

/**
 * Clear the LCD display area (where the graphics area will go)
 */
static void
GraphicsClearLCD_asm()
{
  GraphicsGlobals *globals;

  // get a globals reference
  FtrGet(appCreator, ftrGraphicsGlobals, (UInt32 *) & globals);

  // blit the buffer to the real screen
  {
    UInt8 *ptrLCDScreen;
    Int16 loop;

    ptrLCDScreen = DeviceWindowGetPointer(WinGetDisplayWindow());
    ptrLCDScreen += ((SCREEN_WIDTH * SCREEN_START) >> globals->scrBits);
    // 160xSCREEN_START @ depth

    // determine how many "160" byte blits we need :))
    loop = (((SCREEN_WIDTH * SCREEN_HEIGHT) >> globals->scrBits) / 160);

                                                                // *INDENT-OFF*
    // push all registers (except a7) on stack
    asm("      movem.l %%d0-%%d7/%%a0-%%a6, -(%%sp)                ": :);

    // copy inner 160xSCREEN_START from back buffer to screen
    asm("      move.l  %0, %%a0": :"g"(ptrLCDScreen));
    asm("      move.l  %0, %%d0": :"g"(loop - 1));

    asm("      move.l  #0, %%d1
               move.l  %%d1, %%d2
               move.l  %%d1, %%d3
               move.l  %%d1, %%d4
               move.l  %%d1, %%d5
               move.l  %%d1, %%d6
               move.l  %%d1, %%d7
               move.l  %%d1, %%a2
               move.l  %%d1, %%a3
               move.l  %%d1, %%a4
  
         ScrLoop1:

               movem.l %%d1-%%d7/%%a2-%%a4, (%%a0)
               movem.l %%d1-%%d7/%%a2-%%a4, 40(%%a0)
               movem.l %%d1-%%d7/%%a2-%%a4, 80(%%a0)
               movem.l %%d1-%%d7/%%a2-%%a4, 120(%%a0)
  
               adda.l  #160, %%a0                | blit 160 bytes!!
  
               dbra    %%d0, ScrLoop1                              ": :);
  
    // pop all registers (except a7) off stack
    asm("      movem.l (%%sp)+, %%d0-%%d7/%%a0-%%a6": :);
                                                                // *INDENT-ON*
  }
}

/**
 * Blit the offscreen image to the screen.
 */
static void
GraphicsRepaint_asm()
{
  GraphicsGlobals *globals;

  // get a globals reference
  FtrGet(appCreator, ftrGraphicsGlobals, (UInt32 *) & globals);

  // blit the buffer to the real screen
  {
    UInt8 *ptrLCDScreen, *ptrScreen;
    Int16 loop, y1, y2;

    y1 = globals->updateY1;
    y2 = globals->updateY2;

    // determine if blit will runoff image
    loop = ((y2 - y1 + globals->scrPixels) & ~(globals->scrPixels - 1)) + y1;
    if (loop > SCREEN_HEIGHT)
    {
      y1 -= (loop - SCREEN_HEIGHT);
    }

    ptrLCDScreen = DeviceWindowGetPointer(WinGetDisplayWindow());
    ptrLCDScreen +=
      ((SCREEN_WIDTH * (SCREEN_START + y1)) >> globals->scrBits);
    // 160x(SCREEN_START + globals->updateY1) @ depth
    ptrScreen = DeviceWindowGetPointer(globals->drawWindow);
    ptrScreen += ((SCREEN_WIDTH * y1) >> globals->scrBits);

    // determine how many "160" byte blits we need :))
    loop =
      ((((SCREEN_WIDTH * (y2 - y1 + 1)) >> globals->scrBits) + 159) / 160);

                                                                // *INDENT-OFF*
    // push all registers (except a7) on stack
    asm("      movem.l %%d0-%%d7/%%a0-%%a6, -(%%sp)": :);

    // copy inner 160xSCREEN_START from back buffer to screen
    asm("      move.l  %0, %%a0": :"g"(ptrScreen));
    asm("      move.l  %0, %%a1": :"g"(ptrLCDScreen));
    asm("      move.l  %0, %%d0": :"g"(loop - 1));

    asm("ScrLoop2:

               movem.l (%%a0)+, %%d1-%%d7/%%a2-%%a4
               movem.l %%d1-%%d7/%%a2-%%a4, (%%a1)

               movem.l (%%a0)+, %%d1-%%d7/%%a2-%%a4
               movem.l %%d1-%%d7/%%a2-%%a4, 40(%%a1)

               movem.l (%%a0)+, %%d1-%%d7/%%a2-%%a4
               movem.l %%d1-%%d7/%%a2-%%a4, 80(%%a1)

               movem.l (%%a0)+, %%d1-%%d7/%%a2-%%a4
               movem.l %%d1-%%d7/%%a2-%%a4, 120(%%a1)

               adda.l  #160, %%a1                | blit 160 bytes!!

               dbra    %%d0, ScrLoop2                              ": :);

    // pop all registers (except a7) off stack
    asm("      movem.l (%%sp)+, %%d0-%%d7/%%a0-%%a6": :);
                                                                // *INDENT-ON*
  }
}

// 
// 160x160 API call display routines [for future compatability]
//
// -- Aaron Ardiri, 2000
//

/**
 * Clear the offscreen image.
 */
static void
GraphicsClear_api()
{
  const CustomPatternType erase = { 0, 0, 0, 0, 0, 0, 0, 0 };
  const RectangleType rect = { {0, 0}, {SCREEN_WIDTH, SCREEN_HEIGHT} };
  GraphicsGlobals *globals;
  WinHandle currWindow;

  // get a globals reference
  FtrGet(appCreator, ftrGraphicsGlobals, (UInt32 *) & globals);

  currWindow = WinGetDrawWindow();

  // clear the buffer
  WinSetDrawWindow(globals->drawWindow);
  WinSetPattern(&erase);
  WinFillRectangle(&rect, 0);
  WinSetDrawWindow(currWindow);
}

/**
 * Clear the LCD display area (where the graphics area will go)
 */
static void
GraphicsClearLCD_api()
{
  const CustomPatternType erase = { 0, 0, 0, 0, 0, 0, 0, 0 };
  const RectangleType rect =
    { {0, SCREEN_START}, {SCREEN_WIDTH, SCREEN_HEIGHT} };

  // clear the buffer
  WinSetPattern(&erase);
  WinFillRectangle(&rect, 0);
}

/**
 * Blit the offscreen image to the screen.
 */
static void
GraphicsRepaint_api()
{
  RectangleType rect = { {0, 0}, {SCREEN_WIDTH, SCREEN_HEIGHT} };
  GraphicsGlobals *globals;

  // get a globals reference
  FtrGet(appCreator, ftrGraphicsGlobals, (UInt32 *) & globals);

  rect.topLeft.y = globals->updateY1; 
  rect.extent.y  = globals->updateY2 - globals->updateY1; 

  WinCopyRectangle(globals->drawWindow, WinGetDisplayWindow(),
                   &rect, 0, SCREEN_START + rect.topLeft.y, winPaint);
}

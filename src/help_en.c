
/*
 * @(#)help_en.c
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
 *
 * --------------------------------------------------------------------
 *             THIS FILE CONTAINS THE ENGLISH LANGUAGE TEXT
 * --------------------------------------------------------------------
 */

#include "palm.h"
#include "help.inc"

/**
 * Initialize the instructions screen.
 * 
 * @return the height in pixels of the instructions data area.
 */
UInt16
InitInstructions()
{
  const RectangleType rect = { {0, 0}, {142, 284} };
  const CustomPatternType erase = { 0, 0, 0, 0, 0, 0, 0, 0 };
  HelpGlobals *gbls;
  UInt16 err;
  UInt16 result = 0;

  // create the globals object, and register it
  gbls = (HelpGlobals *) MemPtrNew(sizeof(HelpGlobals));
  MemSet(gbls, sizeof(HelpGlobals), 0);
  FtrSet(appCreator, ftrHelpGlobals, (UInt32) gbls);

  // setup the valid keys available at this point in time
  gbls->keyMask = KeySetMask(~(keyBitsAll ^
                               (keyBitPower | keyBitCradle |
                                keyBitPageUp | keyBitPageDown |
                                keyBitAntenna | keyBitContrast)));

  // initialize windows
  gbls->helpWindow =
    WinCreateOffscreenWindow(rect.extent.x, rect.extent.y, screenFormat,
                             &err);
  err |= (gbls->helpWindow == NULL);

  // did something go wrong?
  if (err != errNone)
  {
    result = 0;
    ApplicationDisplayDialog(xmemForm);
  }

  // draw the help
  else
  {
    FontID font;
    WinHandle currWindow;

    currWindow = WinGetDrawWindow();
    font = FntGetFont();

    // draw to help window
    WinSetDrawWindow(gbls->helpWindow);
    WinSetPattern(&erase);
    WinFillRectangle(&rect, 0);

    {
      Char *str, *ptrStr;
      Coord x, y;

      // initialize 
      y = 2;
      str = (Char *) MemPtrNew(256 * sizeof(Char));

      // draw title
      StrCopy(str, "HOW TO PLAY");
      x = (rect.extent.x - FntCharsWidth(str, StrLen(str))) >> 1;

      WinSetUnderlineMode(grayUnderline);
      WinDrawChars(str, StrLen(str), x, y);
      y += FntLineHeight();
      WinSetUnderlineMode(noUnderline);

      // add space (little)
      y += FntLineHeight() >> 1;

      // general text
      StrCopy(str, "Maryo is hopping mad!");
      x = (rect.extent.x - FntCharsWidth(str, StrLen(str))) >> 1;
      WinDrawChars(str, StrLen(str), x, y);
      y += FntLineHeight();

      // add space (little)
      y += FntLineHeight() >> 1;

      // general text
      x = 4;
                                                                // *INDENT-OFF*
      StrCopy(str,
"The water pipes in his house are blocked with crawling creatures. \
If he cannot get rid of them he will never take a bath again!");
                                                                // *INDENT-ON*
      ptrStr = str;
      while (StrLen(ptrStr) != 0)
      {

        UInt8 count = FntWordWrap(ptrStr, rect.extent.x - x);

        x = (rect.extent.x - FntCharsWidth(ptrStr, count)) >> 1;
        WinDrawChars(ptrStr, count, x, y);
        y += FntLineHeight();
        x = 4;

        ptrStr += count;
      }

      // add space (little)
      y += FntLineHeight() >> 1;

      // show the characters
      x = 12;
      {
        MemHandle bitmapHandle = DmGet1Resource('Tbmp', bitmapHelpCharacters);

        WinDrawBitmap((BitmapType *) MemHandleLock(bitmapHandle), x, y);
        MemHandleUnlock(bitmapHandle);
        DmReleaseResource(bitmapHandle);
      }

      // add space (little)
      y += 65 + (FntLineHeight() >> 1);

      // general text
      x = 4;
                                                                // *INDENT-OFF*
      StrCopy(str,
"Help Maryo knock the pipe pests off the floors using the stylus or \
the keys. This is done using a two-part maneouver; first flip the pest \
on its back, then jump up and kick it off the floor.");
                                                                // *INDENT-ON*
      ptrStr = str;
      while (StrLen(ptrStr) != 0)
      {

        UInt8 count = FntWordWrap(ptrStr, rect.extent.x - x);

        x = (rect.extent.x - FntCharsWidth(ptrStr, count)) >> 1;
        WinDrawChars(ptrStr, count, x, y);
        y += FntLineHeight();
        x = 4;

        ptrStr += count;
      }

      // add space (little)
      y += FntLineHeight() >> 1;

      // general text
      x = 4;
                                                                // *INDENT-OFF*
      StrCopy(str,
"As the game progresses, more pests are introduced that require \
different tactics to remove. Watch out for the fireballs!");
                                                                // *INDENT-ON*
      ptrStr = str;
      while (StrLen(ptrStr) != 0)
      {

        UInt8 count = FntWordWrap(ptrStr, rect.extent.x - x);

        x = (rect.extent.x - FntCharsWidth(ptrStr, count)) >> 1;
        WinDrawChars(ptrStr, count, x, y);
        y += FntLineHeight();
        x = 4;

        ptrStr += count;
      }

      // add space (little)
      y += FntLineHeight() >> 1;
      StrCopy(str, "GOOD LUCK!");
      FntSetFont(boldFont);
      x = (rect.extent.x - FntCharsWidth(str, StrLen(str))) >> 1;
      WinDrawChars(str, StrLen(str), x, y);
      y += FntLineHeight();

      // clean up
      MemPtrFree(str);
    }

    FntSetFont(font);
    WinSetDrawWindow(currWindow);

    result = rect.extent.y;
  }

  return result;
}

/**
 * Draw the instructions on the screen.
 * 
 * @param offset the offset height of the window to start copying from.
 */
void
DrawInstructions(UInt16 offset)
{
  const RectangleType helpArea = { {0, offset}, {142, 116} };
  HelpGlobals *gbls;

  // get globals reference
  FtrGet(appCreator, ftrHelpGlobals, (UInt32 *) & gbls);

  // blit the required area
  WinCopyRectangle(gbls->helpWindow,
                   WinGetDrawWindow(), &helpArea, 3, 16, winPaint);
}

/**
 * Terminate the instructions screen.
 */
void
QuitInstructions()
{
  HelpGlobals *gbls;

  // get globals reference
  FtrGet(appCreator, ftrHelpGlobals, (UInt32 *) & gbls);

  // return the state of the key processing
  KeySetMask(gbls->keyMask);

  // clean up memory
  WinDeleteWindow(gbls->helpWindow, false);
  MemPtrFree(gbls);

  // unregister global data
  FtrUnregister(appCreator, ftrHelpGlobals);
}

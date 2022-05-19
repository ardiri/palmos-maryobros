/*
 * @(#)MBroInit.c
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
// Maryo Bros - Initialization and Destruction Code
//
// Written by Chip Kerchner
//

#include "palm.h"

#include "MBroInit.h"
#include "MBroEnum.h"

#include "MBroGame.h"
#include "MBroDraw.h"

//
// Release Data
//
//  Recover a memory handle and release the data associate with it.
//

void
ReleaseData(register MemPtr dataPtr, register MemHandle handle, register Boolean free)
{
  if (dataPtr != (MemPtr) (NULL)) {
    MemPtrUnlock(dataPtr);
    if (free) {
      MemHandleFree(handle);
    } else {
      DmReleaseResource(handle);
    }
  }
}

//
// Convert Data To Color
//
//   Convert a 2BPP grayscale sprite or animation to 4BPP color using the appropiate
//     color lookup table and number of frames.
//
//   Return a pointer to the new graphic data array.
//

static void *
ConvertToColor(MemHandle *theHandle,
               register UInt16 width,
               register UInt16 widthPixels,
               register UInt16 height,
               register UInt16 numSprites)
{
  register void *newPtr, *oldPtr, *dataPtr, *ptr;
  register MemHandle handle;
  register UInt32 size;
  register UInt16 i, j, k, total, color, entry, table;
  register UInt8 *clrTbl, ch, outColor;

  ptr = (void *)(MemHandleLock(*theHandle));
  if (ptr == (void *)(NULL)) {
    return (void *)(NULL);
  }

  if (runVars->useColor != false) {
    size = ((UInt32)((width + screenColorPixels - 1) / screenColorPixels) * height);

    handle = MemHandleNew(size * numSprites);
    if (handle == (MemHandle)(NULL)) {
      return (void *)(NULL);
    }
    oldPtr = newPtr = (void *)(MemHandleLock(handle));
    if (newPtr == (void *)(NULL)) {
      return (void *)(NULL);
    }

    dataPtr = ptr;

    do {
      table = (UInt16) (*((UInt8 *)(dataPtr))++);
      if (table >= ColorTable_Total) {
        oldPtr = (void *)(NULL);
      }
      clrTbl = (UInt8 *) (runVars->colorTablePtr) + (table * screenPixels);

      j = height;
      do {
        i = widthPixels;
        total = 0;
        do {
          ch = *((UInt8 *) (dataPtr))++;
          k = screenPixels;
          outColor = 0;
          do {
            if (++total <= width) {
              entry = (ch >> ((UInt16) (screenPixels - 1) * screenDepth));
              color = (clrTbl[entry] & maskColorValue);
#if gameColors == Color_Swap
              if (table == ColorTable_AlphaNum) {
                if (entry == textFGEntry) {
                  color = textFGColor;
                } else if (entry == textBGEntry) {
                  color = textBGColor;
                }
              }
#endif
              outColor |= color;
              if ((total & (screenColorPixels - 1)) == 0) {
                *((UInt8 *) (newPtr))++ = outColor;
              }
              outColor <<= screenColorDepth;
              ch <<= screenDepth;
            } else {
              if (((total - 1) & (screenColorPixels - 1)) != 0) {
                while ((total++ & (screenColorPixels - 1)) != 0) {
                  outColor <<= screenColorDepth;
                }

                *((UInt8 *) (newPtr))++ = outColor;
              }
              break;
            }
          } while (--k);
        } while (i -= screenPixels);
      } while (--j);
    } while (--numSprites);

    ReleaseData(ptr, *theHandle, false);

    ptr = oldPtr;
    *theHandle = handle;
  }

  return ptr;
}

//
// Expand Mask
//
//   Expand a 1BPP character tile, sprite or animation mask to 2BPP grayscale
//     or 4BPP color compatibility.
//

static void
ExpandMask(register void *maskOutPtr,
           register void *maskInPtr,
           register void *ptr,
           register UInt16 width,
           register UInt16 height,
           register UInt16 color)
{
  register UInt16 j, bits, useSprite;
  register UInt8 chMaskIn, chMaskOut, spriteColor;

  chMaskIn = *((UInt8 *)(maskInPtr))++;
  *((UInt8 *) (maskOutPtr))++ = (chMaskIn >> maskRectBit);
  *((UInt8 *) (maskOutPtr))++ = (chMaskIn & maskRectMask);

  chMaskIn = *((UInt8 *)(maskInPtr))++;
  *((UInt8 *) (maskOutPtr))++ = (chMaskIn >> maskRectBit);
  *((UInt8 *) (maskOutPtr))++ = (chMaskIn & maskRectMask);

  j = 0;

  if (runVars->useColor == false) {
    do {
      bits = 0;
      chMaskOut = 0;
      do {
        if ((j++ & (bitsInByte - 1)) == 0) {
          chMaskIn = *((UInt8 *) (maskInPtr))++;
        }
        if ((chMaskIn & (1 << (bitsInByte - 1))) != 0) {
          chMaskOut |= maskValue;
        }
        chMaskIn <<= 1;
        if ((++bits & (screenPixels - 1)) == 0) {
          *((UInt8 *) (maskOutPtr))++ = chMaskOut;
        }
        chMaskOut <<= screenDepth;
      } while (bits < width);

      if ((bits & (screenPixels - 1)) != 0) {
        while ((++bits & (screenPixels - 1)) != 0) {
          chMaskOut <<= screenDepth;
        }

        *((UInt8 *) (maskOutPtr))++ = chMaskOut;
      }
    } while (--height);
  } else {
    useSprite = (ptr != (void *)(NULL));
#if gameColors == Color_Swap
    spriteColor = 0;
    if (useSprite) {
      for (bits = screenColorPixels; bits--;) {
        spriteColor <<= screenColorDepth;
        spriteColor |= color;
      }
    }
#endif

    do {
      bits = 0;
      chMaskOut = 0;
      do {
        if ((j++ & (bitsInByte - 1)) == 0) {
          chMaskIn = *((UInt8 *) (maskInPtr))++;
        }
        if ((chMaskIn & (1 << (bitsInByte - 1))) != 0) {
          chMaskOut |= maskColorValue;
        }
        chMaskIn <<= 1;
        if ((++bits & (screenColorPixels - 1)) == 0) {
          *((UInt8 *) (maskOutPtr))++ = chMaskOut;
#if gameColors == Color_Swap
          if (useSprite) {
            *((UInt8 *) (ptr))++ =
              (spriteColor & ~chMaskOut) | (*((UInt8 *) (ptr)) & chMaskOut);
          }
#endif
        }
        chMaskOut <<= screenColorDepth;
      } while (bits < width);

#ifndef USE_8BIT_COLOR
      if ((bits & (screenColorPixels - 1)) != 0) {
        while ((++bits & (screenColorPixels - 1)) != 0) {
          chMaskOut <<= screenColorDepth;
        }

        *((UInt8 *) (maskOutPtr))++ = chMaskOut;
#if gameColors == Color_Swap
        if (useSprite) {
          *((UInt8 *) (ptr))++ =
            (spriteColor & ~chMaskOut) | (*((UInt8 *) (ptr)) & chMaskOut);
        }
#endif
      }
#endif
    } while (--height);
  }
}

//
// Initialize Mask
//
//   Initialize a new memory pointer used in masks.  Convert one frame at a time.
//
//   Return a pointer to the new masking array.
//

static void *
InitializeMask(register void *ptr,
               MemHandle *theHandle,
               register DmResType type,
               register DmResID id,
               register UInt16 num,
               register UInt16 width,
               register UInt16 height,
               register UInt32 size,
               register UInt16 compSize)
{
  register void *maskResPtr, *maskRes2Ptr, *maskPtr, *mask2Ptr;
  register MemHandle handle;
  register UInt16 color;

#ifdef USE_8BIT_COLOR
  color = (width == charXSize) ? blackColor : transparentColor;
#else
  color = blackColor;
#endif

  *theHandle = DmGet1Resource(type, id + maskResource);
  if (*theHandle == (MemHandle)(NULL)) {
    return (void *)(NULL);
  }
  maskRes2Ptr = maskResPtr = (void *)(MemHandleLock(*theHandle));
  if (maskResPtr == (void *)(NULL)) {
    return (void *)(NULL);
  }

  handle = MemHandleNew((UInt32) (size) * num);
  if (handle == (MemHandle)(NULL)) {
    return (void *)(NULL);
  }
  mask2Ptr = maskPtr = (void *)(MemHandleLock(handle));
  if (maskPtr == (void *)(NULL)) {
    return (void *)(NULL);
  }

  do {
    ExpandMask(mask2Ptr, maskRes2Ptr, ptr, width, height, color);
    maskRes2Ptr += compSize;
    mask2Ptr += size;
    if (ptr != (void *)(NULL)) {
      ptr += (size - maskRectSize);
    }
  } while (--num);

  ReleaseData((MemPtr) (maskResPtr), *theHandle, false);
  *theHandle = handle;

  return maskPtr;
}

//
// Initialize Characters
//
//   Initialize the character tile array, color lookup table and palette.
//

static Boolean
InitializeCharacters(void)
{
  runVars->colorTablePtr = (void *)(MemHandleLock(runVars->colorTableHandle));
  if (runVars->colorTablePtr == (void *)(NULL)) {
    return false;
  }

  if (runVars->useColor != false) {
    register void *clrTbl;

    clrTbl = (UInt8 *) (runVars->colorTablePtr) + (numColorTables * screenPixels);
    WinPalette(winPaletteSet, startDefinedColor, numDefinedColors, (RGBColorType *) (clrTbl));
  }

  runVars->charPtr = ConvertToColor(&runVars->charHandle, charXSize, charXPixels, charYSize, charCount);
  if (runVars->charPtr == (void *)(NULL)) {
    return false;
  }

  if (runVars->useColor != false){
    runVars->charMaskPtr =
      InitializeMask(runVars->charPtr, &runVars->charMaskHandle, charResourceID, charResourceNum,
                     charCount, charXSize, charYSize,
                     (runVars->useColor == false) ? charMaskSize : charColorMaskSize,
                     charMaskCompactSize);

    if (runVars->charMaskPtr == (void *)(NULL)) {
      return false;
    }
    ReleaseData((MemPtr) (runVars->charMaskPtr), runVars->charMaskHandle, true);
  }
  runVars->charMaskPtr = (void *)(NULL);

  return true;
}

//
// Fix Floor Tiles
//
//   Fix the bottom of the floor tiles in color mode to be the background color.
//

static void
FixFloorTiles(void)
{
  register UInt16 i, j, x, y, mask1, mask2;
  register UInt8 *data, *data2, *data3, ch;

  for (i = floorStart; i <= floorEnd; i++) {
    data = (UInt8 *) (runVars->animatePtr) + (i * (animateColorSrcBytes * animateHeight)) + (animateColorSrcBytes * (animateHeight - 1));
    for (x = animateColorSrcBytes; x--;) {
      data3 = data2 = data++;

      mask1 = 0;
      mask2 = maskColorValue;
      for (j = screenColorPixels; j--;) {
        for (y = animateHeight; y--;) {
          ch = ((*data2 >> mask1) & maskColorValue);
          if (ch != whiteColor) {
            break;
          }
          *data2 = (blackColor << mask1) | (*data2 & ~mask2);
          data2 -= animateColorSrcBytes;
        }
        mask1 += screenColorDepth;
        mask2 <<= screenColorDepth;
        data2 = data3;
      }
    }
  }
}

//
// Initialize Sprites
//
//   Initialize all sprite, mask and animation arrays.
//

static Boolean
InitializeSprites(void)
{
  runVars->piperPtr = ConvertToColor(&runVars->piperHandle, objectWidth, objectXPixels, objectMaxHeight, piperNumSprites);
  if (runVars->piperPtr == (void *)(NULL)) {
    return false;
  }
  runVars->piperMaskPtr =
    InitializeMask(runVars->piperPtr, &runVars->piperMaskHandle, piperResourceID, piperResourceNum,
                   piperNumSprites, objectWidth, objectMaxHeight,
                   (runVars->useColor == false) ? maxMaskSize : maxColorMaskSize,
                   maxMaskCompactSize);
  if (runVars->piperMaskPtr == (void *)(NULL)) {
    return false;
  }

  runVars->turtlePtr = ConvertToColor(&runVars->turtleHandle, objectWidth, objectXPixels, objectHeight, turtleNumSprites);
  if (runVars->turtlePtr == (void *)(NULL)) {
    return false;
  }
  runVars->turtleMaskPtr =
    InitializeMask(runVars->turtlePtr, &runVars->turtleMaskHandle, turtleResourceID, turtleResourceNum,
                   turtleNumSprites, objectWidth, objectHeight,
                   (runVars->useColor == false) ? objectMaskSize : objectColorMaskSize,
                   objectMaskCompactSize);
  if (runVars->turtleMaskPtr == (void *)(NULL)) {
    return false;
  }

  runVars->crabPtr = ConvertToColor(&runVars->crabHandle, objectWidth, objectXPixels, objectHeight, crabNumSprites);
  if (runVars->crabPtr == (void *)(NULL)) {
    return false;
  }
  runVars->crabMaskPtr =
    InitializeMask(runVars->crabPtr, &runVars->crabMaskHandle, crabResourceID, crabResourceNum,
                   crabNumSprites, objectWidth, objectHeight,
                   (runVars->useColor == false) ? objectMaskSize : objectColorMaskSize,
                   objectMaskCompactSize);
  if (runVars->crabMaskPtr == (void *)(NULL)) {
    return false;
  }

  runVars->fighterFlyPtr = ConvertToColor(&runVars->fighterFlyHandle, objectWidth, objectXPixels, objectHeight, fighterFlyNumSprites);
  if (runVars->fighterFlyPtr == (void *)(NULL)) {
    return false;
  }
  runVars->fighterFlyMaskPtr =
    InitializeMask(runVars->fighterFlyPtr, &runVars->fighterFlyMaskHandle, fighterFlyResourceID, fighterFlyResourceNum,
                   fighterFlyNumSprites, objectWidth, objectHeight,
                   (runVars->useColor == false) ? objectMaskSize : objectColorMaskSize,
                   objectMaskCompactSize);
  if (runVars->fighterFlyMaskPtr == (void *)(NULL)) {
    return false;
  }

  runVars->coinPtr = ConvertToColor(&runVars->coinHandle, objectWidth, objectXPixels, objectHeight, coinNumSprites);
  if (runVars->coinPtr == (void *)(NULL)) {
    return false;
  }
  runVars->coinMaskPtr =
    InitializeMask(runVars->coinPtr, &runVars->coinMaskHandle, coinResourceID, coinResourceNum,
                   coinNumSprites, objectWidth, objectHeight,
                   (runVars->useColor == false) ? objectMaskSize : objectColorMaskSize,
                   objectMaskCompactSize);
  if (runVars->coinMaskPtr == (void *)(NULL)) {
    return false;
  }

  runVars->iceMachinePtr = ConvertToColor(&runVars->iceMachineHandle, objectWidth, objectXPixels, objectHeight, iceMachineNumSprites);
  if (runVars->iceMachinePtr == (void *)(NULL)) {
    return false;
  }
  runVars->iceMachineMaskPtr =
    InitializeMask(runVars->iceMachinePtr, &runVars->iceMachineMaskHandle, iceMachineResourceID, iceMachineResourceNum,
                   iceMachineNumSprites, objectWidth, objectHeight,
                   (runVars->useColor == false) ? objectMaskSize : objectColorMaskSize,
                   objectMaskCompactSize);
  if (runVars->iceMachineMaskPtr == (void *)(NULL)) {
    return false;
  }

  runVars->fireBallPtr = ConvertToColor(&runVars->fireBallHandle, objectWidth, objectXPixels, objectHeight, fireBallNumSprites);
  if (runVars->fireBallPtr == (void *)(NULL)) {
    return false;
  }
  runVars->fireBallMaskPtr =
    InitializeMask(runVars->fireBallPtr, &runVars->fireBallMaskHandle, fireBallResourceID, fireBallResourceNum,
                   fireBallNumSprites, objectWidth, objectHeight,
                   (runVars->useColor == false) ? objectMaskSize : objectColorMaskSize,
                   objectMaskCompactSize);
  if (runVars->fireBallMaskPtr == (void *)(NULL)) {
    return false;
  }

  runVars->paintBallPtr = ConvertToColor(&runVars->paintBallHandle, objectWidth, objectXPixels, objectHeight, paintBallNumSprites);
  if (runVars->paintBallPtr == (void *)(NULL)) {
    return false;
  }
  runVars->paintBallMaskPtr =
    InitializeMask(runVars->paintBallPtr, &runVars->paintBallMaskHandle, paintBallResourceID, paintBallResourceNum,
                   paintBallNumSprites, objectWidth, objectHeight,
                   (runVars->useColor == false) ? objectMaskSize : objectColorMaskSize,
                   objectMaskCompactSize);
  if (runVars->paintBallMaskPtr == (void *)(NULL)) {
    return false;
  }

  runVars->iciclePtr = ConvertToColor(&runVars->icicleHandle, objectWidth, objectXPixels, objectHeight, icicleNumSprites);
  if (runVars->iciclePtr == (void *)(NULL)) {
    return false;
  }
  runVars->icicleMaskPtr =
    InitializeMask(runVars->iciclePtr, &runVars->icicleMaskHandle, icicleResourceID, icicleResourceNum,
                   icicleNumSprites, objectWidth, objectHeight,
                   (runVars->useColor == false) ? objectMaskSize : objectColorMaskSize,
                   objectMaskCompactSize);
  if (runVars->icicleMaskPtr == (void *)(NULL)) {
    return false;
  }

  runVars->powPtr = ConvertToColor(&runVars->powHandle, objectWidth, objectXPixels, objectHeight, powNumSprites);
  if (runVars->powPtr == (void *)(NULL)) {
    return false;
  }
  runVars->powMaskPtr =
    InitializeMask(runVars->powPtr, &runVars->powMaskHandle, powResourceID, powResourceNum,
                   powNumSprites, objectWidth, objectHeight,
                   (runVars->useColor == false) ? objectMaskSize : objectColorMaskSize,
                   objectMaskCompactSize);
  if (runVars->powMaskPtr == (void *)(NULL)) {
    return false;
  }

  runVars->flipMaskPtr =
    InitializeMask((void *)(NULL), &runVars->flipMaskHandle, flipResourceID, flipResourceNum,
                   flipNumSprites, objectWidth, flipHeight,
                   (runVars->useColor == false) ? flipMaskSize : flipColorMaskSize,
                   flipMaskCompactSize);
  if (runVars->flipMaskPtr == (void *)(NULL)) {
    return false;
  }

  runVars->animatePtr = ConvertToColor(&runVars->animateHandle, objectWidth, objectXPixels, objectHeight, animateNumSprites);
  if (runVars->animatePtr == (void *)(NULL)) {
    return false;
  }
  runVars->animateMaskPtr =
    InitializeMask(runVars->animatePtr, &runVars->animateMaskHandle, animateResourceID, animateResourceNum,
                   animateNumSprites, animateWidth, animateHeight,
                   (runVars->useColor == false) ? animateMaskSize : animateColorMaskSize,
                   animateMaskCompactSize);
  if (runVars->animateMaskPtr == (void *)(NULL)) {
    return false;
  }

  if (runVars->useColor != false) {
#if gameColors == Color_Swap
    FixFloorTiles();
#endif

#ifdef USE_8BIT_COLOR
    ReleaseData((MemPtr) (runVars->animateMaskPtr), runVars->animateMaskHandle, true);
    runVars->animateMaskPtr = (void *)(NULL);
#endif
  }

  ReleaseData((MemPtr) (runVars->colorTablePtr), runVars->colorTableHandle, false);
  runVars->colorTablePtr = (void *)(NULL);

  return true;
}

//
// Initialize Music
//
//   Initialize the musical and sound effect data.
//

static Boolean
InitializeMusic(void)
{
  runVars->musicPtr = (void *)(MemHandleLock(runVars->musicHandle));
  if (runVars->musicPtr == (void *)(NULL)) {
    return false;
  }

  runVars->music.priority = Priority_Level_None;

  return true;
}

//
// Destroy Characters
//
//   Destroy the character tile array, color lookup tables and palette.
//

static void
DestroyCharacters(void)
{
  ReleaseData((MemPtr) (runVars->charPtr), runVars->charHandle, (runVars->useColor != false));
  ReleaseData((MemPtr) (runVars->charMaskPtr), runVars->charMaskHandle, true);
  ReleaseData((MemPtr) (runVars->colorTablePtr), runVars->colorTableHandle, false);
}

//
// Unlock Sprite
//
//   Unlock sprite and masking data arrays.
//

static void
UnlockSprite(register MemPtr spritePtr,
             register MemHandle spriteHandle,
             register MemPtr maskPtr,
             register MemHandle maskHandle)
{
  ReleaseData(spritePtr, spriteHandle, (runVars->useColor != false));
  ReleaseData(maskPtr, maskHandle, true);
}

//
// Destroy Sprites
//
//   Destroy sprite and masking data arrays.
//

static void
DestroySprites(void)
{
  UnlockSprite((MemPtr) (runVars->piperPtr), runVars->piperHandle,
               (MemPtr) (runVars->piperMaskPtr), runVars->piperMaskHandle);
  UnlockSprite((MemPtr) (runVars->turtlePtr), runVars->turtleHandle,
               (MemPtr) (runVars->turtleMaskPtr), runVars->turtleMaskHandle);
  UnlockSprite((MemPtr) (runVars->crabPtr), runVars->crabHandle,
               (MemPtr) (runVars->crabMaskPtr), runVars->crabMaskHandle);
  UnlockSprite((MemPtr) (runVars->fighterFlyPtr), runVars->fighterFlyHandle,
               (MemPtr) (runVars->fighterFlyMaskPtr), runVars->fighterFlyMaskHandle);
  UnlockSprite((MemPtr) (runVars->coinPtr), runVars->coinHandle,
               (MemPtr) (runVars->coinMaskPtr), runVars->coinMaskHandle);
  UnlockSprite((MemPtr) (runVars->iceMachinePtr), runVars->iceMachineHandle,
               (MemPtr) (runVars->iceMachineMaskPtr), runVars->iceMachineMaskHandle);
  UnlockSprite((MemPtr) (runVars->fireBallPtr), runVars->fireBallHandle,
               (MemPtr) (runVars->fireBallMaskPtr), runVars->fireBallMaskHandle);
  UnlockSprite((MemPtr) (runVars->paintBallPtr), runVars->paintBallHandle,
               (MemPtr) (runVars->paintBallMaskPtr), runVars->paintBallMaskHandle);
  UnlockSprite((MemPtr) (runVars->iciclePtr), runVars->icicleHandle,
               (MemPtr) (runVars->icicleMaskPtr), runVars->icicleMaskHandle);
  UnlockSprite((MemPtr) (runVars->powPtr), runVars->powHandle,
               (MemPtr) (runVars->powMaskPtr), runVars->powMaskHandle);
  UnlockSprite((MemPtr) (NULL), (MemHandle)(NULL),
               (MemPtr) (runVars->flipMaskPtr), runVars->flipMaskHandle);

  UnlockSprite((MemPtr) (runVars->animatePtr), runVars->animateHandle,
               (MemPtr) (runVars->animateMaskPtr), runVars->animateMaskHandle);
}

//
// Destroy Music
//
//   Destroy music data arrays.
//

static void
DestroyMusic(void)
{
  runVars->music.playing = false;
  ReleaseData(runVars->musicPtr, runVars->musicHandle, false);
}

//
// Initial Handles - Characters, Sprites, Tables & Music
//
//   Initialize all handles needs in the game.
//

Boolean InitializeHandles(void)
{
  runVars->colorTableHandle = DmGet1Resource(colorResourceID, colorResourceNum);
  if (runVars->colorTableHandle == (MemHandle)(NULL)) {
    return false;
  }

  runVars->charHandle = DmGet1Resource(charResourceID, charResourceNum);
  if (runVars->charHandle == (MemHandle)(NULL)) {
    return false;
  }

  runVars->piperHandle = DmGet1Resource(piperResourceID, piperResourceNum);
  if (runVars->piperHandle == (MemHandle)(NULL)) {
    return false;
  }

  runVars->turtleHandle = DmGet1Resource(turtleResourceID, turtleResourceNum);
  if (runVars->turtleHandle == (MemHandle)(NULL)) {
    return false;
  }

  runVars->crabHandle = DmGet1Resource(crabResourceID, crabResourceNum);
  if (runVars->crabHandle == (MemHandle)(NULL)) {
    return false;
  }

  runVars->fighterFlyHandle = DmGet1Resource(fighterFlyResourceID, fighterFlyResourceNum);
  if (runVars->fighterFlyHandle == (MemHandle)(NULL)) {
    return false;
  }

  runVars->coinHandle = DmGet1Resource(coinResourceID, coinResourceNum);
  if (runVars->coinHandle == (MemHandle)(NULL)) {
    return false;
  }

  runVars->iceMachineHandle = DmGet1Resource(iceMachineResourceID, iceMachineResourceNum);
  if (runVars->iceMachineHandle == (MemHandle)(NULL)) {
    return false;
  }

  runVars->fireBallHandle = DmGet1Resource(fireBallResourceID, fireBallResourceNum);
  if (runVars->fireBallHandle == (MemHandle)(NULL)) {
    return false;
  }

  runVars->paintBallHandle = DmGet1Resource(paintBallResourceID, paintBallResourceNum);
  if (runVars->paintBallHandle == (MemHandle)(NULL)) {
    return false;
  }

  runVars->icicleHandle = DmGet1Resource(icicleResourceID, icicleResourceNum);
  if (runVars->icicleHandle == (MemHandle)(NULL)) {
    return false;
  }

  runVars->powHandle = DmGet1Resource(powResourceID, powResourceNum);
  if (runVars->powHandle == (MemHandle)(NULL)) {
    return false;
  }

  runVars->animateHandle = DmGet1Resource(animateResourceID, animateResourceNum);
  if (runVars->animateHandle == (MemHandle)(NULL)) {
    return false;
  }

  runVars->musicHandle = DmGet1Resource(musicResourceID, musicResourceNum);
  if (runVars->musicHandle == (MemHandle)(NULL)) {
    return false;
  }

  return true;
}

//
// Initial Graphics - Characters, Sprites & Tables
//
//   Initialize all graphical based on color mode and initialize musical data.
//

Boolean
InitializeGraphics(void)
{
  runVars->useColor = DeviceSupportsColor();

  if (!InitializeHandles()) {
    return false;
  }

  InitializeDraw();
  if (!InitializeCharacters()) {
    return false;
  }

  if (!InitializeSprites()) {
    return false;
  }
  if (!InitializeDrawTables()) {
    return false;
  }
  if (!InitializeMusic()) {
    return false;
  }

  return true;
}

//
// Destory Graphics - Characters, Sprites & Tables
//
//   Destroy all graphical and musical data.
//

void
DestroyGraphics(void)
{
  DestroyCharacters();
  DestroySprites();
  DestroyMusic();
  DestroyDraw();
}

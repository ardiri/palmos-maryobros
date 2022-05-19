/*
 * @(#)MBroDraw.c
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
// Maryo Bros - Drawing, Animation and Intersection Code
//
// Written by Chip Kerchner
//

#include "palm.h"

#include "MBroInit.h"
#include "MBroEnum.h"
#include "MBroMacr.h"

#include "MBroGame.h"
#include "MBroDraw.h"
#include "MBroPSt2.h"

static void  GetMaskRectangle(GameRectanglePtr obj1MaskRect,
                 register ObjectPtr object,
                 register UInt16 noWrap,
                 register UInt16 current) __DEVICE__;
static UInt16 MaskIntersect(register ObjectPtr obj1Ptr,
                            register ObjectPtr obj2Ptr,
                            register UInt16 current) __DEVICE__;
static UInt16 SpriteIntersect(register ObjectPtr obj1Ptr,
                              register ObjectPtr obj2Ptr,
                              register UInt16 current) __DEVICE__;
static UInt16 LevelIntersect(register ObjectPtr obj1Ptr,
                             register ObjectPtr obj2Ptr,
                             register UInt16 current) __DEVICE__;
static void UpdateDirty(register UInt16 x1,
                        register UInt16 y1,
                        register UInt16 width,
                        register UInt16 height) __DEVICE__;

//
// Clear Off-Screen Window
//
//   Clear offscreen buffer with background color.
//

void
ClearScreen(void)
{
  register UInt16 xBytes;
  register UInt8 color;

  if (runVars->useColor == false)
  {
    xBytes = screenXBytes;
    color = 0x00;
  }
  else
  {
    xBytes = screenColorXBytes;
#if gameColors == Color_Swap
    color = blackColor | (blackColor << screenColorDepth);
#else
    color = whiteColor | (whiteColor << screenColorDepth);
#endif
  }

  MemSet(runVars->screenOffPtr, (xBytes * (UInt16) ((maxPixels - 1))), color);
}

//
// Clear Last Line
//
//   Clear last line of offscreen buffer with background color.
//

void
ClearLastLine(void)
{
  if (runVars->useColor == false) {
    MemSet(runVars->screenOffPtr + (UInt32) (((maxPixels - 1) * screenWidth) / screenPixels),
           screenXBytes, 0xff);
  } else {
    MemSet(runVars->screenOffPtr + (UInt32) (((maxPixels - 1) * screenWidth) / screenColorPixels),
           screenColorXBytes, (blackColor | (blackColor << screenColorDepth)));
  }
}

//
// Initialize Draw
//
//   Initialize pointer to offscreen drawing buffer.  Dirty all character array tiles.
//

void
InitializeDraw(void)
{
  register UInt16 i;

  runVars->screenOffPtr = DeviceWindowGetPointer(GraphicsGetDrawWindow());

  ClearScreen();
  ClearLastLine();

  for (i = charNum; i--;) {
    gameVars->charArray[i] |= dirtyChar;
  }
}

//
// Initialize Drawing Tables
//
//   Initialize tables used to drawing sprites (waking colors, reversing byte, and
//     temporary storage sprite arrays).
//

Boolean
InitializeDrawTables(void)
{
  register UInt16 i, j, mask1, mask2, value;

  runVars->wakingColorHandle = MemHandleNew(tableSize);
  if (runVars->wakingColorHandle == (MemHandle)(NULL)) {
    return false;
  }
  runVars->wakingColor = (void *)(MemHandleLock(runVars->wakingColorHandle));
  if (runVars->wakingColor == (void *)(NULL)) {
    return false;
  }

  if (runVars->useColor == false) {
    for (i = tableSize; i--;) {
      mask1 = bitsInByte - screenDepth;
      for (j = screenPixels, value = 0x00; j--;) {
        value |= (colorSwapTable[(i >> mask1) & maskValue] << mask1);
        mask1 -= screenDepth;
      }
      runVars->wakingColor[i] = value;
    }
  } else {
#ifdef USE_8BIT_COLOR
#ifdef USE_VGA_COLORS
    j = numDefinedColors + screenPixels - 1;
#else
    j = numDefinedColors - screenPixels - 1;
#endif
    for (i = numDefinedColors; i--;) {
      runVars->wakingColor[i] = j;
      if (j-- == 0) {
        j = numDefinedColors - 1;
      }
    }
    runVars->wakingColor[numDefinedColors] = numDefinedColors;
#else
    for (i = tableSize; i--;) {
      mask1 = bitsInByte - screenColorDepth;
      for (j = screenColorPixels, value = 0x00; j--;) {
#ifdef USE_VGA_COLORS
        value |= ((((i >> mask1) + screenPixels - 1) & maskColorValue) << mask1);
#else
        value |= ((((i >> mask1) - screenPixels - 1) & maskColorValue) << mask1);
#endif
        mask1 -= screenColorDepth;
      }
      runVars->wakingColor[i] = value;
    }
#endif
  }

  runVars->reverseArrayHandle = MemHandleNew(tableSize);
  if (runVars->reverseArrayHandle == (MemHandle)(NULL)) {
    return false;
  }
  runVars->reverseArray = (void *)(MemHandleLock(runVars->reverseArrayHandle));
  if (runVars->reverseArray == (void *)(NULL)) {
    return false;
  }

  if (runVars->useColor == false) {
    for (i = tableSize; i--;) {
      mask1 = maskValue;
      mask2 = bitsInByte - screenDepth;
      for (j = (screenPixels / 2), value = 0x00; j--;) {
        value |= ((i >> mask2) & mask1) | ((i & mask1) << mask2);
        mask1 <<= screenDepth;
        mask2 -= (screenDepth * 2);
      }
      runVars->reverseArray[i] = value;
    }
  } else {
    for (i = tableSize; i--;) {
      mask1 = maskColorValue;
      mask2 = bitsInByte - screenColorDepth;
      for (j = (screenColorPixels / 2), value = 0x00; j--;) {
        value |= ((i >> mask2) & mask1) | ((i & mask1) << mask2);
        mask1 <<= screenColorDepth;
        mask2 -= (screenColorDepth * 2);
      }
      runVars->reverseArray[i] = value;
    }
  }

  runVars->theSpriteHandle = MemHandleNew((runVars->useColor == false) ? spriteSize : spriteColorSize);
  if (runVars->theSpriteHandle == (MemHandle)(NULL)) {
    return false;
  }
  runVars->theSprite = (void *)(MemHandleLock(runVars->theSpriteHandle));
  if (runVars->theSprite == (void *)(NULL)) {
    return false;
  }

  return true;
}

//
// Destroy Drawing Tables
//
//   Destroy tables used in drawing sprites.
//

void
DestroyDraw(void)
{
  ReleaseData(runVars->wakingColor, runVars->wakingColorHandle, true);
  ReleaseData(runVars->reverseArray, runVars->reverseArrayHandle, true);

  ReleaseData(runVars->theSprite, runVars->theSpriteHandle, true);
}

//
// Initialize Drawers
//
//   Initialize pointers to resources used in drawing tiles, sprites
//     and animations.
//

Boolean
InitializeDrawers(void)
{
  register AnimationPtr theAnimate;
  register ObjElePtr objPtr;
  register ObjectPtr object;
  register UInt16 i;

  for (i = maxNumAnimations; i--;) {
    theAnimate = &gameVars->animateList[i];

    theAnimate->draw.spritePtr = runVars->animatePtr;
    theAnimate->draw.maskPtr = runVars->animateMaskPtr;
  }

  objPtr = gameVars->spriteHead;
  while (objPtr != (ObjElePtr) (NULL)) {
    object = (objPtr->object);
    object->flags |= flagDirty;

    switch (object->type) {
      case Object_Character:
        object->draw.spritePtr = runVars->piperPtr;
        object->draw.maskPtr = runVars->piperMaskPtr;
        break;

      case Object_Flip:
        object->draw.spritePtr = (void *)(NULL);
        object->draw.maskPtr = runVars->flipMaskPtr;
        break;

      case Object_Pow:
        object->draw.spritePtr = runVars->powPtr;
        object->draw.maskPtr = runVars->powMaskPtr;
        break;

      case Object_Turtle:
        object->draw.spritePtr = runVars->turtlePtr;
        object->draw.maskPtr = runVars->turtleMaskPtr;
        break;

      case Object_Crab:
        object->draw.spritePtr = runVars->crabPtr;
        object->draw.maskPtr = runVars->crabMaskPtr;
        break;

      case Object_FighterFly:
        object->draw.spritePtr = runVars->fighterFlyPtr;
        object->draw.maskPtr = runVars->fighterFlyMaskPtr;
        break;

      case Object_IceMachine:
        object->draw.spritePtr = runVars->iceMachinePtr;
        object->draw.maskPtr = runVars->iceMachineMaskPtr;
        break;

      case Object_Coin:
        object->draw.spritePtr = runVars->coinPtr;
        object->draw.maskPtr = runVars->coinMaskPtr;
        break;

      case Object_FireBall:
        object->draw.spritePtr = runVars->fireBallPtr;
        object->draw.maskPtr = runVars->fireBallMaskPtr;
        break;

      case Object_PaintBall:
        object->draw.spritePtr = runVars->paintBallPtr;
        object->draw.maskPtr = runVars->paintBallMaskPtr;
        break;

      case Object_Icicle:
        object->draw.spritePtr = runVars->iciclePtr;
        object->draw.maskPtr = runVars->icicleMaskPtr;
        break;

      default:
        return true;
    }

    objPtr = objPtr->next;
  }

  return false;
}

//
// Adjust Animation
//
//   Adjust pointers to sequence arrays (for animations).
//

Boolean
AdjustAnimation(UInt8 ** animationPtr,
                register Boolean change)
{
  register UInt8 *animation, *newAnimation;
  register UInt16 i;
  register Boolean notFound;

  static UInt8 *newAnimations[numPresetAnimations] = {
    pipeBendSequenceTwo, pipeBendSequenceThree, pipeBendSequenceFour,
    pipeBendSequenceFive, floor1BendSequence,
    floor1EndBendSequence, floor2BendSequence, floor2EndBendSequence,
    floor3BendSequence, floor3EndBendSequence,
    floor4BendSequence, floor4EndBendSequence, floorIceBendSequence,
    floorIceEndBendSequence, floorPlatformSequence,
    splashSequence, turtleTShirtSequence, points800Sequence,
    points1600Sequence, points2400Sequence,
    points3200Sequence, points4000Sequence, points4800Sequence,
    points200Sequence, points500Sequence,
    points1000Sequence, bonusCoin1Sequence, bonusCoin2Sequence,
    waterfall1Sequence, waterfall2Sequence,
    iceMachineMeltSequence
  };

  animation = *animationPtr;
  if (animation != (UInt8 *)(NULL)) {
    notFound = true;
    for (i = numPresetAnimations; i--;) {
      newAnimation = newAnimations[i];

      if (animation == gameVars->preSetAnimation[i]) {
        *animationPtr = newAnimation;
        notFound = false;
      }
      if (change) {
        gameVars->preSetAnimation[i] = newAnimation;
      }
    }
  } else {
    notFound = false;
  }

  return notFound;
}

//
// Initialize Game Globals
//
//   Initialize the pointer to the gametime variables.  Adjust pointers to save variables.
//

Boolean
InitializeGameGlobals(register GameVariablesPtr globals)
{
  register UInt16 i;
  register Boolean reset;
  register UInt32 adjust;
  register UInt8 *top, *bottom;

  gameVars = globals;

  reset = false;

  if (gameVars->gameVarPtr != (void *)(NULL)) {
    adjust = (UInt32)(gameVars->gameVarPtr) - (UInt32)(gameVars);
    top    = (UInt8 *)(gameVars->gameVarPtr);
    bottom = top + sizeof(GameVariables);

    adjustAddress(gameVars->spriteHead);
    adjustAddress(gameVars->animationHead);
    adjustAddress(gameVars->animationTail);

    adjustAddress(gameVars->freezeEvent.tileLoc1);
    adjustAddress(gameVars->freezeEvent.tileLoc2);

    reset |= AdjustKeyList();
    reset |= AdjustKeyDelayList();
    reset |= AdjustInstruction();

    if (gameVars->instructionEvent != (InstructionEventPtr)(NULL)) {
      adjustAddress(gameVars->instructionEvent->text);
    }

    for (i = maxNumTextEvents; i--;) {
      adjustAddress(gameVars->textEvents[i].text);
    }

    for (i = gameVars->eventGameCount; i--;) {
      adjustAddress(gameVars->eventList[i].event);
    }

    for (i = maxNumObjects; i--;) {
      adjustAddress(gameVars->spriteList[i].object);
      adjustAddress(gameVars->spriteList[i].next);

      reset |= AdjustSequence(&gameVars->objectList[i].draw.sequence, (i == 0));
    }

    for (i = maxNumAnimations; i--;) {
      adjustAddress(gameVars->animationList[i].animate);

      if (gameVars->animateList[i].movement != (Int8 *) (NULL)) {
        gameVars->animateList[i].movement = turtleTShirtMovement;
      }

      adjustAddress(gameVars->animationList[i].next);
      adjustAddress(gameVars->animateList[i].object);

      reset |= AdjustAnimation(&gameVars->animateList[i].draw.sequence, (i == 0));
    }
  } else {
    reset = true;
  }

  return reset;
}

//
// Initialize an Animation
//
//   Initialize animation setup values based on type and object.
//

void
InitializeAnimation(register AnimationPtr theAnimate,
                    register UInt16 type,
                    register ObjectPtr object)
{
  register Int16 delta;

  theAnimate->movement = (void *)(NULL);
  theAnimate->moveNum = 0;
  theAnimate->moveSize = 0;

  switch (type)
  {
    case Animation_Pipe_Bend:
      delta = object->deltaX;
      if (delta < 0) {
        delta = -delta;
      }

      if (delta <= speedTwo) {
        theAnimate->draw.sequence = pipeBendSequenceTwo;
        theAnimate->size = sizeof(pipeBendSequenceTwo);
      } else if (delta == speedThree) {
        theAnimate->draw.sequence = pipeBendSequenceThree;
        theAnimate->size = sizeof(pipeBendSequenceThree);
      } else if (delta == speedFour) {
        theAnimate->draw.sequence = pipeBendSequenceFour;
        theAnimate->size = sizeof(pipeBendSequenceFour);
      } else if (delta >= speedFive) {
        theAnimate->draw.sequence = pipeBendSequenceFive;
        theAnimate->size = sizeof(pipeBendSequenceFive);
      }
      break;

    case Animation_Floor1_Bend:
      theAnimate->draw.sequence = floor1BendSequence;
      theAnimate->size = sizeof(floor1BendSequence);
      break;

    case Animation_Floor1_End_Bend:
      theAnimate->draw.sequence = floor1EndBendSequence;
      theAnimate->size = sizeof(floor1EndBendSequence);
      break;

    case Animation_Floor2_Bend:
      theAnimate->draw.sequence = floor2BendSequence;
      theAnimate->size = sizeof(floor2BendSequence);
      break;

    case Animation_Floor2_End_Bend:
      theAnimate->draw.sequence = floor2EndBendSequence;
      theAnimate->size = sizeof(floor2EndBendSequence);
      break;

    case Animation_Floor3_Bend:
      theAnimate->draw.sequence = floor3BendSequence;
      theAnimate->size = sizeof(floor3BendSequence);
      break;

    case Animation_Floor3_End_Bend:
      theAnimate->draw.sequence = floor3EndBendSequence;
      theAnimate->size = sizeof(floor3EndBendSequence);
      break;

    case Animation_Floor4_Bend:
      theAnimate->draw.sequence = floor4BendSequence;
      theAnimate->size = sizeof(floor4BendSequence);
      break;

    case Animation_Floor4_End_Bend:
      theAnimate->draw.sequence = floor4EndBendSequence;
      theAnimate->size = sizeof(floor4EndBendSequence);
      break;

    case Animation_Floor_Ice_Bend:
      theAnimate->draw.sequence = floorIceBendSequence;
      theAnimate->size = sizeof(floorIceBendSequence);
      break;

    case Animation_Floor_Ice_End_Bend:
      theAnimate->draw.sequence = floorIceEndBendSequence;
      theAnimate->size = sizeof(floorIceEndBendSequence);
      break;

    case Animation_Platform:
      theAnimate->draw.sequence = floorPlatformSequence;
      theAnimate->size = sizeof(floorPlatformSequence);
      break;

    case Animation_Splash:
      theAnimate->draw.sequence = splashSequence;
      theAnimate->size = sizeof(splashSequence);
      break;

    case Animation_Turtle_TShirt:
      theAnimate->draw.sequence = turtleTShirtSequence;
      theAnimate->size = sizeof(turtleTShirtSequence);

      theAnimate->movement = turtleTShirtMovement;
      theAnimate->moveSize = sizeof(turtleTShirtMovement);
      break;

    case Animation_Points_800:
      theAnimate->draw.sequence = points800Sequence;
      theAnimate->size = sizeof(points800Sequence);
      break;

    case Animation_Points_1600:
      theAnimate->draw.sequence = points1600Sequence;
      theAnimate->size = sizeof(points1600Sequence);
      break;

    case Animation_Points_2400:
      theAnimate->draw.sequence = points2400Sequence;
      theAnimate->size = sizeof(points2400Sequence);
      break;

    case Animation_Points_3200:
      theAnimate->draw.sequence = points3200Sequence;
      theAnimate->size = sizeof(points3200Sequence);
      break;

    case Animation_Points_4000:
      theAnimate->draw.sequence = points4000Sequence;
      theAnimate->size = sizeof(points4000Sequence);
      break;

    case Animation_Points_4800:
      theAnimate->draw.sequence = points4800Sequence;
      theAnimate->size = sizeof(points4800Sequence);
      break;

    case Animation_Points_200:
      theAnimate->draw.sequence = points200Sequence;
      theAnimate->size = sizeof(points200Sequence);
      break;

    case Animation_Points_500:
      theAnimate->draw.sequence = points500Sequence;
      theAnimate->size = sizeof(points500Sequence);
      break;

    case Animation_Points_1000:
      theAnimate->draw.sequence = points1000Sequence;
      theAnimate->size = sizeof(points1000Sequence);
      break;

    case Animation_Bonus_Coin_1:
      theAnimate->draw.sequence = bonusCoin1Sequence;
      theAnimate->size = sizeof(bonusCoin1Sequence);
      break;

    case Animation_Bonus_Coin_2:
      theAnimate->draw.sequence = bonusCoin2Sequence;
      theAnimate->size = sizeof(bonusCoin2Sequence);
      break;

    case Animation_Waterfall_1:
      theAnimate->draw.sequence = waterfall1Sequence;
      theAnimate->size = sizeof(waterfall1Sequence);
      break;

    case Animation_Waterfall_2:
      theAnimate->draw.sequence = waterfall2Sequence;
      theAnimate->size = sizeof(waterfall2Sequence);
      break;

    case Animation_IceMachine_Melt:
      theAnimate->draw.sequence = iceMachineMeltSequence;
      theAnimate->size = sizeof(iceMachineMeltSequence);
      break;
  }

  theAnimate->type = type;
  theAnimate->object = object;

  theAnimate->draw.height = animateHeight;
  theAnimate->draw.seqNum = 0;

  theAnimate->deltaX = 0;
  theAnimate->deltaY = 0;
  theAnimate->flags = 0x0000;

  if (object != (ObjectPtr) (NULL))
  {
    theAnimate->draw.xStart = object->draw.xStart;
    theAnimate->draw.yStart = object->draw.yStart;
    theAnimate->draw.xSkip = object->draw.xSkip;
    theAnimate->draw.ySkip = object->draw.ySkip;
    theAnimate->draw.xPixels = object->draw.xPixels;
    theAnimate->draw.yPixels = object->draw.yPixels;

    theAnimate->xPos = object->xPos;
    theAnimate->yPos = object->yPos;

    theAnimate->flags |= (object->flags & flagDirectionX);
  }
  else
  {
    theAnimate->draw.xSkip = 0;
    theAnimate->draw.ySkip = 0;
    theAnimate->draw.xPixels = animateWidth;
    theAnimate->draw.yPixels = animateHeight;
  }
}

//
// Add Animation to List
//
//   Add an animation to the list.
//
//   Return a pointer to the new animation.
//

AnimationPtr
AddAnimation(void)
{
  register AnimateElePtr animPtr, retAnimPtr;

  retAnimPtr = animPtr = gameVars->animationTail;

  if (animPtr != (AnimateElePtr) (NULL))
  {
    gameVars->animationTail = gameVars->animationTail->next;

    retAnimPtr->next = (AnimateElePtr) (NULL);

    if (gameVars->animationHead == (AnimateElePtr) (NULL))
    {
      gameVars->animationHead = animPtr;
    }
    else
    {
      register AnimateElePtr curPtr;

      animPtr = gameVars->animationHead;
      do
      {
        animPtr = (curPtr = animPtr)->next;
      } while (animPtr != (AnimateElePtr) (NULL));

      curPtr->next = retAnimPtr;
    }
  }

  return retAnimPtr->animate;
}

//
// Delete Animation From List
//
//   Delete an animation from the list.
//

void
DeleteAnimation(register AnimationPtr theAnimate)
{
  register AnimateElePtr animPtr, curPtr;

  animPtr = gameVars->animationHead;
  if ((animPtr != (AnimateElePtr) (NULL))
      && (theAnimate != (AnimationPtr) (NULL)))
  {
    if (animPtr->animate != theAnimate)
    {
      do
      {
        animPtr = (curPtr = animPtr)->next;
      } while (animPtr->animate != theAnimate);

      curPtr->next = animPtr->next;
    }
    else
    {
      gameVars->animationHead = animPtr->next;
    }

    animPtr->next = gameVars->animationTail;
    gameVars->animationTail = animPtr;
  }
}

//
// Change Animation Position
//
//   Update the clipping variables (both X and Y directions) for an animtion.
//

void
ChangeAnimationPos(register AnimationPtr theAnimate,
                   register Int16 deltaX,
                   register Int16 deltaY)
{
  register UInt16 width, skip, start;
  register Int16 xPos, delta;

  xPos = (theAnimate->xPos + deltaX);

  if (xPos < 0)
  {
    xPos += (UInt16) (screenWidth * speedFactor);
  }
  else if (xPos >= (UInt16) (screenWidth * speedFactor))
  {
    xPos -= (UInt16) (screenWidth * speedFactor);
  }

  theAnimate->xPos = xPos;

  width = (UInt16) (screenWidth) - (start = (xPos / speedFactor));
  skip = 0;

  if (width >= objectWidth)
  {
    width = objectWidth;
  }
  else if (width < (objectWidth / 2))
  {
    start = 0;
    if ((theAnimate->flags & flagAnimDirectionX) == 0)
    {
      skip = width;
    }
    width = objectWidth - width;
  }
  else
  {
    if ((theAnimate->flags & flagAnimDirectionX) != 0)
    {
      skip = objectWidth - width;
    }
  }

  theAnimate->draw.xPixels = width;
  theAnimate->draw.xStart = start;
  theAnimate->draw.xSkip = skip;

  delta = (theAnimate->yPos += deltaY);
  width = theAnimate->draw.height;
  skip = 0;

  if (delta < topScreen)
  {
    skip = topScreen - delta;
    if (width > skip)
    {
      width -= skip;
    }
    else
    {
      width = 0;
    }
    delta = topScreen;
  }

  theAnimate->draw.yPixels = width;
  theAnimate->draw.yStart = delta;
  theAnimate->draw.ySkip = skip;
}

//
// Update Animations
//
//   Update the list of animations.  Remove animation when finished playing
//     unless it loops.
//

void
UpdateAnimations(void)
{
  register UInt16 flags, num;
  register Int16 deltaX, deltaY;
  register AnimationPtr theAnimate;
  register AnimateElePtr animationPtr;

  animationPtr = gameVars->animationHead;
  while (animationPtr != (AnimateElePtr) (NULL))
  {
    theAnimate = animationPtr->animate;
    flags = theAnimate->flags;

    animationPtr = animationPtr->next;

    DrawSprite(&theAnimate->draw, (flags & flagAnimDirectionX));

    if (theAnimate->movement != (void *)(NULL))
    {
      num = theAnimate->moveNum;

      if ((theAnimate->flags & flagAnimDirectionX) != 0)
      {
        deltaX = theAnimate->movement[num];
      }
      else
      {
        deltaX = -theAnimate->movement[num];
      }
      deltaY = theAnimate->movement[num + 1];

      theAnimate->moveNum = (num += animateMovementNum);
      if (num >= theAnimate->moveSize)
      {
        theAnimate->movement = (void *)(NULL);
      }

      ChangeAnimationPos(theAnimate, deltaX, deltaY);
    }
    else
    {
      theAnimate->draw.xStart += theAnimate->deltaX;
      theAnimate->draw.yStart += theAnimate->deltaY;
    }

    if (++theAnimate->draw.seqNum >= theAnimate->size)
    {
      if ((flags & flagAnimLooping) == 0)
      {
        DeleteAnimation(theAnimate);
      }
      else
      {
        theAnimate->draw.seqNum = 0;
      }
    }
  }
}

//
// Create New Animation
//
//   Create a new animation from the list elements.
//
//   Return a pointer to the new animation list element.
//   

AnimationPtr
NewAnimation(void)
{
  register UInt16 animateNum;
  register AnimationPtr theAnimate;

  if ((animateNum = gameVars->animateCount) < maxNumAnimations)
  {
    gameVars->animateCount++;
  }

  theAnimate = &gameVars->animateList[animateNum];

  theAnimate->draw.spritePtr = runVars->animatePtr;
  theAnimate->draw.maskPtr = runVars->animateMaskPtr;

  return theAnimate;
}

//
// Initialize Animations
//
//   Initialize linked list used with animations.
//

void
InitializeAnimations(void)
{
  register UInt16 i;
  register AnimateElePtr animationPtr;

  gameVars->animateCount = 0;
  gameVars->animationHead = (AnimateElePtr) (NULL);
  gameVars->animationTail = &gameVars->animationList[0];

  for (i = 0; i < maxNumAnimations; i++)
  {
    animationPtr = &gameVars->animationList[i];
    animationPtr->animate = NewAnimation();

    if (i != (maxNumAnimations - 1))
    {
      animationPtr->next = &gameVars->animationList[i + 1];
    }
    else
    {
      animationPtr->next = (AnimateElePtr) (NULL);
    }
  }
}

//
// Delete Animation Type From List
//
//   Delete a specified type from the animation list.
//

void
DeleteAnimationType(register UInt16 type)
{
  register AnimateElePtr animPtr;
  register AnimationPtr theAnimate;

  animPtr = gameVars->animationHead;
  while (animPtr != (AnimateElePtr) (NULL))
  {
    theAnimate = animPtr->animate;
    animPtr = animPtr->next;

    if (theAnimate->type == type)
    {
      DeleteAnimation(theAnimate);
    }
  }
}

//
// Delete Animation Object From List
//
//   Delete a specfied object  from the animation list.
//

void
DeleteAnimationObject(register ObjectPtr object)
{
  register AnimateElePtr animPtr;
  register AnimationPtr theAnimate;

  if (object != (ObjectPtr) (NULL))
  {
    animPtr = gameVars->animationHead;
    while (animPtr != (AnimateElePtr) (NULL))
    {
      theAnimate = animPtr->animate;
      animPtr = animPtr->next;

      if (theAnimate->object == object)
      {
        DeleteAnimation(theAnimate);
      }
    }
  }
}

//
// Get Mask Rectangle
//
//  Get the masking rectangle based on an object's sequence number.
//

void
GetMaskRectangle(GameRectanglePtr obj1MaskRect,
                 register ObjectPtr object,
                 register UInt16 noWrap,
                 register UInt16 current)
{
  register UInt16 seqNum, rectX1, rectX2;
  register Int16 x1, y1;
  register void *maskPtr;
  register UInt32 offset;

  if ((object->flags & flagInvisible) != 0)
  {
    seqNum = 0;
  }
  else
  {
    if (current)
    {
      seqNum = object->draw.sequence[object->draw.seqNum];
    }
    else
    {
      seqNum = object->draw.sequence[object->prevSeqNum];
    }
  }

  if (runVars->useColor == false)
  {
    offset =
      (seqNum *
       (((object->draw.height * objectXPixels) / screenPixels) +
        sizeof(BoundingRectangle)));
  }
  else
  {
    offset =
      (seqNum *
       (((object->draw.height * objectColorXPixels) / screenColorPixels) +
        sizeof(BoundingRectangle)));
  }
  maskPtr = object->draw.maskPtr + offset;

  if (current)
  {
    x1 = object->xPos;
    y1 = object->yPos;
  }
  else
  {
    x1 = object->xPrev;
    y1 = object->yPrev;
  }
  x1 /= speedFactor;

  if ((noWrap != 0) && (x1 > ((Int16) (screenWidth - (objectWidth / 2)))))
  {
    x1 -= (Int16) (screenWidth);
  }

  rectX1 = *((UInt8 *) (maskPtr))++;
  obj1MaskRect->y1 = y1 + *((UInt8 *) (maskPtr))++;
  rectX2 = *((UInt8 *) (maskPtr))++;
  obj1MaskRect->y2 = y1 + *((UInt8 *) (maskPtr))++;

  if ((object == &piperObject) && (object->motion == Motion_Hit))
  {
    obj1MaskRect->y1 = ((obj1MaskRect->y1 / charYSize) + 1) * charYSize;
  }

  if ((object->flags & flagDirectionX) != 0)
  {
    y1 = rectX1;
    rectX1 = (objectWidth - 1) - rectX2;
    rectX2 = (objectWidth - 1) - y1;
  }

  obj1MaskRect->x1 = x1 + rectX1;
  obj1MaskRect->x2 = x1 + rectX2;
}

//
// Check if Two Sprite Masks Rectangle Intersect
//
//   Check to see if two masking rectangles intersect one another.
//
//   Return true if they do intersect.
//

UInt16
MaskIntersect(register ObjectPtr obj1Ptr,
              register ObjectPtr obj2Ptr,
              register UInt16 current)
{
  register UInt16 noWrap;
  GameRectangle obj1MaskRect, obj2MaskRect;

  noWrap = 0;
  if ((obj1Ptr->level == Level_Inanimate)
      || (obj2Ptr->level == Level_Inanimate))
  {
    noWrap = 1;
  }

  if ((current == 0) && (obj1Ptr->type == Object_Coin)
      && (obj2Ptr->type == Object_Coin))
  {
    current = 1;
  }

  GetMaskRectangle(&obj1MaskRect, obj1Ptr, noWrap, current);
  GetMaskRectangle(&obj2MaskRect, obj2Ptr, noWrap, current);

  if (noWrap == 0)
  {
    if ((obj1MaskRect.x2 >= (Int16) (screenWidth))
        && (obj2MaskRect.x1 < objectWidth))
    {
      obj2MaskRect.x1 += (Int16) (screenWidth);
      obj2MaskRect.x2 += (Int16) (screenWidth);
    }
    else if ((obj2MaskRect.x2 >= (Int16) (screenWidth))
             && (obj1MaskRect.x1 < objectWidth))
    {
      obj1MaskRect.x1 += (Int16) (screenWidth);
      obj1MaskRect.x2 += (Int16) (screenWidth);
    }
  }

  if (((obj2MaskRect.x1 >= obj1MaskRect.x1)
       && (obj2MaskRect.x1 <= obj1MaskRect.x2))
      || ((obj2MaskRect.x2 >= obj1MaskRect.x1)
          && (obj2MaskRect.x2 <= obj1MaskRect.x2)))
  {
    if (((obj2MaskRect.y1 >= obj1MaskRect.y1)
         && (obj2MaskRect.y1 <= obj1MaskRect.y2))
        || ((obj2MaskRect.y2 >= obj1MaskRect.y1)
            && (obj2MaskRect.y2 <= obj1MaskRect.y2)))
    {
      return 1;
    }
  }
  else
    if (((obj1MaskRect.x1 >= obj2MaskRect.x1)
         && (obj1MaskRect.x1 <= obj2MaskRect.x2))
        || ((obj1MaskRect.x2 >= obj2MaskRect.x1)
            && (obj1MaskRect.x2 <= obj2MaskRect.x2)))
  {
    if (((obj1MaskRect.y1 >= obj2MaskRect.y1)
         && (obj1MaskRect.y1 <= obj2MaskRect.y2))
        || ((obj1MaskRect.y2 >= obj2MaskRect.y1)
            && (obj1MaskRect.y2 <= obj2MaskRect.y2)))
    {
      return 1;
    }
  }

  return 0;
}

//
// Check if Two Sprite Rectangles Intersect
//
//   Check to see if two sprite rectangles intersect one another.
//
//   Return true if they do intersect.
//

UInt16
SpriteIntersect(register ObjectPtr obj1Ptr,
                register ObjectPtr obj2Ptr,
                register UInt16 current)
{
  register UInt16 noWrap;
  register Int16 obj1X1,
    obj1X2, obj1Y1, obj1Y2, obj2X1, obj2X2, obj2Y1, obj2Y2;

  noWrap = 0;
  if ((obj1Ptr->level == Level_Inanimate)
      || (obj2Ptr->level == Level_Inanimate))
  {
    noWrap = 1;
  }

  if (current)
  {
    obj1X1 = obj1Ptr->xPos;
    obj1Y1 = obj1Ptr->yPos;
  }
  else
  {
    obj1X1 = obj1Ptr->xPrev;
    obj1Y1 = obj1Ptr->yPrev;
  }
  obj1X1 /= speedFactor;

  if ((noWrap != 0) && (obj1X1 > ((Int16) (screenWidth - (objectWidth / 2)))))
  {
    obj1X1 -= (Int16) (screenWidth);
  }

  obj1X2 = obj1X1 + objectWidth - 1;

  if (current)
  {
    obj2X1 = obj2Ptr->xPos;
    obj2Y1 = obj2Ptr->yPos;
  }
  else
  {
    obj2X1 = obj2Ptr->xPrev;
    obj2Y1 = obj2Ptr->yPrev;
  }
  obj2X1 /= speedFactor;

  if ((noWrap != 0)
      && (obj2X1 >= ((Int16) (screenWidth - (objectWidth / 2)))))
  {
    obj2X1 -= (Int16) (screenWidth);
  }

  obj2X2 = obj2X1 + objectWidth - 1;

  obj1Y2 = obj1Y1 + obj1Ptr->draw.height - 1;
  obj2Y2 = obj2Y1 + obj2Ptr->draw.height - 1;

  if (noWrap == 0)
  {
    if ((obj1X2 >= (Int16) (screenWidth)) && (obj2X1 < objectWidth))
    {
      obj2X1 += (Int16) (screenWidth);
      obj2X2 += (Int16) (screenWidth);
    }
    else if ((obj2X2 >= (Int16) (screenWidth)) && (obj1X1 < objectWidth))
    {
      obj1X1 += (Int16) (screenWidth);
      obj1X2 += (Int16) (screenWidth);
    }
  }

  if (((obj2X1 >= obj1X1) && (obj2X1 <= obj1X2))
      || ((obj2X2 >= obj1X1) && (obj2X2 <= obj1X2)))
  {
    if (((obj2Y1 >= obj1Y1) && (obj2Y1 <= obj1Y2))
        || ((obj2Y2 >= obj1Y1) && (obj2Y2 <= obj1Y2)))
    {
      return MaskIntersect(obj1Ptr, obj2Ptr, current);
    }
  }

  return 0;
}

//
// Check if Two Sprite Levels or Motions Intersect
//
//   Check to see if two sprites are allowed to intersect one another.
//
//   Return true if they do intersect.
//

static UInt16
LevelIntersect(register ObjectPtr obj1Ptr,
               register ObjectPtr obj2Ptr,
               register UInt16 current)
{
  register UInt16 level1, level2, motion1, motion2;

  level1 = obj1Ptr->level;
  level2 = obj2Ptr->level;

  if (current) {
    if ((((level1 == Level_Inanimate) || (level1 == Level_Misc)) && (level2 != Level_Character) && (level2 != Level_Other)) ||
       (((level2 == Level_Inanimate) || (level2 == Level_Misc)) && (level1 != Level_Character) && (level1 != Level_Other))) {
      return 0;
    }

    motion1 = obj1Ptr->motion;
    motion2 = obj2Ptr->motion;

    if ((level1 != Level_Character) && (level2 != Level_Character) && ((motion1 == Motion_Fall) || (motion1 == Motion_Flip) ||
        (motion1 == Motion_Drop) || (motion2 == Motion_Fall) || (motion2 == Motion_Flip) || (motion2 == Motion_Drop))) {
      return 0;
    }
  } else {
    if (((level1 == Level_Bonus) || (level1 == Level_Enemy)) && ((level2 == Level_Bonus) || (level2 == Level_Enemy))) {
      return 0;
    }
  }

  return 1;

}

//
// Clear Intersection Array
//
//   Clear sector array used to intersection calculations.
//

void
ClearIntersect(void)
{
  register UInt16 i;

  for (i = sectorNum; i--;)
  {
    gameVars->sectorArray[i] = 0;
  }
}

//
// Check if Sector Has an Intersect
//
//   Check each sector to see if there is an intersection of two
//     objects.  If there is set the object's intersection flag.
//

void
SectorIntersect(register UInt16 num)
{
  register ObjectPtr object, object2;
  register UInt32 sec1Num, sec2Num;
  register UInt16 obj1Num, obj2Num;

  sec1Num = gameVars->sectorArray[num];
  obj1Num = 0;

  do {
    while ((sec1Num & 0x1) == 0) {
      obj1Num++;
      sec1Num >>= 1;
    }

    object = &gameVars->objectList[obj1Num];
    obj2Num = obj1Num + 1;
    sec2Num = sec1Num >> 1;
    do {
      if (sec2Num & 0x1) {
        object2 = &gameVars->objectList[obj2Num];
        if (SpriteIntersect(object, object2, 1) && LevelIntersect(object, object2, 1)) {
          if (LevelIntersect(object, object2, 0) || !SpriteIntersect(object, object2, 0)) {
            register UInt16 xDelta1, xDelta2, sign1, sign2;

            absSign(object->deltaX, xDelta1, sign1);
            absSign(object2->deltaX, xDelta2, sign2);

            if ((UInt16)(sign1 + sign2) == 0) {
              if ((object == &piperObject) || (object2 != &piperObject)) {
                if ((object2->flags & flagInvisible) != 0) {
                  ObjectFlipStart(object);
                } else if ((object2->flags & flagExit) == 0) {
                  object->intersect |= ((UInt32)(1) << obj2Num);
                }
              }
              if ((object2 == &piperObject) || (object != &piperObject)) {
                if ((object->flags & flagInvisible) != 0) {
                  ObjectFlipStart(object2);
                } else if ((object->flags & flagExit) == 0) {
                  object2->intersect |= ((UInt32)(1) << obj1Num);
                }
              }
            }else if (xDelta1 > xDelta2) {
              if ((object2->flags & flagInvisible) != 0) {
                ObjectFlipStart(object);
              } else if ((object->flags & flagExit) == 0) {
                if (object2 == &piperObject) {
                  object2->intersect |= ((UInt32)(1) << obj1Num);
                } else {
                  object->intersect |= ((UInt32)(1) << obj2Num);
                }
              }
            } else if (xDelta1 < xDelta2) {
              if ((object->flags & flagInvisible) != 0) {
                ObjectFlipStart(object2);
              } else if ((object2->flags & flagExit) == 0) {
                if (object == &piperObject) {
                  object->intersect |= ((UInt32)(1) << obj2Num);
                } else {
                  object2->intersect |= ((UInt32)(1) << obj1Num);
                }
              }
            } else {
              if (((object == &piperObject) && ((object2->flags & flagExit) == 0)) ||
                  ((object2 == &piperObject) && ((object->flags & flagExit) == 0))) {
                object->intersect  |= ((UInt32)(1) << obj2Num);
                object2->intersect |= ((UInt32)(1) << obj1Num);
              }
            }
          }
        }
      }
      obj2Num++;
      sec2Num >>= 1;
    } while (sec2Num);
    obj1Num++;
    sec1Num >>= 1;
  } while (sec1Num);
}

//
// Update Intersection Array
//
//   Update all sector array elements.
//

void
UpdateIntersect(void)
{
  register UInt16 i;

  for (i = sectorNum; i--;) {
    if (gameVars->sectorArray[i] != 0) {
      SectorIntersect(i);
    }
  }
}

//
// Update Sector List
//
//   Determine which sector an object is in and set bit to indicate
//     object's location.
//

void
UpdateSector(register ObjectPtr object)
{
  register UInt16 x1, y1, x2, y2;
  register UInt16 num;
  register UInt32 objNum;

  x1 = ((x2 = (object->xPos / speedFactor)) / sectorXSize);
  x2 = (x2 + objectWidth - 1) / sectorXSize;

  y1 = (object->draw.yStart / sectorYSize);
  y2 = (object->draw.yStart + object->draw.yPixels - 1) / sectorYSize;

  num = x1 + (y1 * sectorXNum);
  objNum = ((UInt32) (1) << object->num);

  gameVars->sectorArray[num] |= objNum;

  if (x1 != x2) {
    if (x1 != (sectorXNum - 1)) {
      gameVars->sectorArray[num + 1] |= objNum;
    } else {
      gameVars->sectorArray[num - (sectorXNum - 1)] |= objNum;
    }
  }

  if (y1 != y2) {
    num += sectorXNum;
    gameVars->sectorArray[num] |= objNum;

    if (x1 != x2) {
      if (x1 != (sectorXNum - 1)) {
        gameVars->sectorArray[num + 1] |= objNum;
      } else {
        gameVars->sectorArray[num - (sectorXNum - 1)] |= objNum;
      }
    }
  }
}

//
// Update Dirty Characters
//
//   Update the dirty bit of each character tile array elements in
//     specified area.
//

void
UpdateDirty(register UInt16 x1,
            register UInt16 y1,
            register UInt16 width,
            register UInt16 height)
{
  register UInt16 xEle, yEle;
  register Int16 i, j;
  register TilePtr tilePtr, tileBasePtr;

  xEle = x1;
  yEle = y1;
#if 0
  rectUpdate(xEle, yEle, width, height);
#else
  rectUpdate(yEle, height);
#endif

  xEle = (x1 / charXSize);
  width += (x1 % charXSize);

  yEle = (y1 / charYSize);
  height += (y1 % charYSize);

  tileBasePtr = &gameVars->charArray[xEle + (yEle * charXNum)];

  j = height;
  do {
    i = width;
    tilePtr = tileBasePtr;
    do {
      *tilePtr++ |= dirtyChar;
    } while ((i -= charXSize) > 0);
    tileBasePtr += charXNum;
  } while ((j -= charYSize) > 0);
}

//
// Check if Object Dropped On
//
//   Check to see if an object has dropped onto a floor character tile.
//
//   Return object's current Y position
//

Int16
CheckDrop(register ObjectPtr object,
          register Int16 y,
          register Int16 delta,
          register UInt16 dir)
{
  register Tile tile;
  register Int16 i, j;
  register UInt16 base, width, xEle, yPos, k, x;

  width = objectCheckWidth;
  x = (object->xPos / speedFactor);

  if (dir) {
    x += (objectWidth - width);
  }
  if (x >= (UInt16) (screenWidth)) {
    x -= (UInt16) (screenWidth);
  }

  xEle = (x / charXSize);
  width = (x % charXSize) + width;

  base = (UInt16) (screenWidth) - x;
  if (width > base) {
    width = base;
  }

  yPos = ((y + object->draw.height + 1) / charYSize);
  base = xEle + (yPos * charXNum);

  j = delta;
  do {
    i = width;
    k = base;
    do {
      tile = gameVars->charArray[k++] & ~dirtyChar;
      if ((tile < FLOOR_START) || (tile > FLOOR_END2)) {
        break;
      }
    } while ((i -= charXSize) > 0);

    if (i <= 0) {
      tile = gameVars->charArray[k - charXNum - 1] & ~dirtyChar;
      if ((tile < FLOOR_START) || (tile > FLOOR_END2)) {
        object->deltaY = stopFall;

        return ((yPos * charYSize) - object->draw.height - 1);
      }
    }

    base -= charXNum;
    yPos--;
  } while ((j -= charYSize) > 0);

  return y;
}

//
// Check if Object is Touching Any
//
//   Check to see if an object is touch any floor character tiles.
//
//   Return first tile object is touching
//

Tile
CheckAny(register ObjectPtr object,
         register Int16 y,
         register Int16 delta,
         register UInt16 dir)
{
  register Tile tile;
  register Int16 i, j;
  register UInt16 width, xEle, x;
  register TilePtr tilePtr, tileBasePtr;

  x = (object->xPos / speedFactor);

  width = objectWidth;
  if (dir != checkAll) {
    width -= objectCheckWidth;

    if (dir) {
      x += (objectWidth - width);
    }

    if (x >= (UInt16) (screenWidth)) {
      x -= (UInt16) (screenWidth);
    }
  }

  xEle   = (x / charXSize);
  width  = (x % charXSize) + width;

  i = (UInt16) (screenWidth) - x;
  if (width > i) {
    width = i;
  }

  tileBasePtr = &gameVars->charArray[xEle + ((y / charYSize) * charXNum)];

  if (delta < 0) {
    j = -delta;
    delta = charXNum;
  } else {
    j = delta;
    delta = -charXNum;
  }

  do {
    i = width;
    tilePtr = tileBasePtr;
    do {
      tile = *tilePtr++ & ~dirtyChar;
      if ((tile >= FLOOR_START) && (tile <= FLOOR_END2)) {
        return tile;
      }
    } while ((i -= charXSize) > 0);
    tileBasePtr += delta;
  } while ((j -= charYSize) > 0);

  return dirtyChar;
}

//
// Check if Object Dropped On a Floor Tile
//
//   Check to see if an object has dropped onto any floor character tiles in
//     either direction.
//
//   Return object's current Y position
//

Int16
CheckDropOn(register ObjectPtr object,
            register Int16 y,
            register Int16 delta)
{
  register Int16 retY;
  register UInt16 flags;

  if (y >= topScreen) {
    flags = 0;
    if ((object->flags & flagDirectionX) == 0) {
      flags = 1;
    }

    retY = CheckDrop(object, y, delta, flags);
    if (object->deltaY == stopFall) {
      return retY;
    } else if ((CheckAny(object, y + object->draw.height + 1, delta, flags) & dirtyChar) == 0) {
      retY = CheckDrop(object, y, delta, 1 - flags);
      if (object->deltaY == stopFall) {
        return retY;
      }
    }
  }

  return y;
}

//
// Check if Object Dropped Off a Floor Tile
//
//   Check to see if an object has dropped off a floor character tile.
//
//   Return object's current Y position
//

void
CheckDropOff(register ObjectPtr object,
             register Int16 y)
{
  register Tile tile;
  register Int16 i;
  register UInt16 k, x, width, xEle;
  register TilePtr tilePtr;

  width = (objectWidth - objectCheckWidth) + 1;
  x = (object->xPos / speedFactor);

  if ((object->flags & flagDirectionX) == 0) {
    x += (objectWidth - width);
  }
  if (x >= (UInt16) (screenWidth)) {
    x -= (UInt16) (screenWidth);
  }

  xEle = (x / charXSize);
  i = (x % charXSize) + width;

  k = (UInt16) (screenWidth) - x;
  if (i > k) {
    i = k;
  }

  tilePtr = &gameVars->charArray[xEle + (((UInt16)(y + object->draw.height + 1) / charYSize) * charXNum)];

  do {
    tile = *tilePtr++ & ~dirtyChar;
    if ((tile >= FLOOR_START) && (tile <= FLOOR_END2)) {
      return;
    }
  } while ((i -= charXSize) > 0);

  object->deltaY = 0;
  object->motion = Motion_Fall;
}

//
// Check if Object Ricochet Off a Floor Tile
//
//  Check to see if a paintball has ricochet off the top off or bottom of a floor tile.
//
//   Return ricochet direction (either X or Y or none)
//

UInt16
CheckRicochetOff(register ObjectPtr object,
                 register Int16 y,
                 register Int16 delta,
                 register UInt16 dir)
{
  register Tile tile;
  register Int16 i, j;
  register UInt16 x, base, width, xEle, yPos, k, flags;

  if (y >= topScreen) {
    width = objectCheckWidth;
    x = (object->xPos / speedFactor);

    if (dir) {
      x += (objectWidth - width);
    }

    xEle = (x / charXSize);
    width = (x % charXSize) + width;

    base = (UInt16) (screenWidth) - x;
    if (width > base) {
      width = base;
    }

    if (delta < 0) {
      yPos = y;
      j = -delta;
      delta = charXNum;
    } else {
      yPos = y + object->draw.height;
      j = delta;
      delta = -charXNum;
    }

    flags = object->flags;

    base = xEle + ((yPos / charYSize) * charXNum);

    do {
      i = width;
      k = base;
      do {
        tile = gameVars->charArray[k++] & ~dirtyChar;
        if ((tile < FLOOR_START) || (tile > FLOOR_END2)) {
          break;
        }
      } while ((i -= charXSize) > 0);

      if (i <= 0) {
        k = ((k / charXNum) * charYSize);
        if (delta < 0) {
          object->yPos = k - object->draw.height;
        } else {
          object->yPos = k + charYSize;
        }

        return ricochetY;
      }

      base += delta;
    } while ((j -= charYSize) > 0);

    object->flags = flags & ~flagRicochet;

    i = width;
    k = base;
    do {
      tile = gameVars->charArray[k++] & ~dirtyChar;
      if ((tile < FLOOR_START) || (tile > FLOOR_END2)) {
        break;
      }
    } while ((i -= charXSize) > 0);

    if ((flags & flagRicochet) == 0) {
      if (i <= 0) {
        return ricochetX;
      }
    }
  }

  return 0;
}

//
// Check if Object Ricochet
//
//   Check to see if a paintball has ricochet off any floor tiles in either
//     direction.
//
//   Return object's current Y position
//

Int16
CheckRicochet(register ObjectPtr object,
              register Int16 y,
              register Int16 delta)
{
  register UInt16 flags, height, ret;

  if (y >= topScreen) {
    flags = (object->deltaX > 0);

    ret = CheckRicochetOff(object, y, delta, flags);
    if (ret == 0) {
      height = object->draw.height - charYSize;
      if (delta < 0) {
        height = 0;
      }
      if ((CheckAny(object, y + height, delta, flags) & dirtyChar) == 0) {
        ret = CheckRicochetOff(object, y, delta, 1 - flags);
      }
    }

    if (ret == ricochetY) {
      object->flags |= flagRicochet;

      object->deltaY = -object->deltaY;
      y = object->yPos;
    } else if (ret == ricochetX) {
      object->deltaX = -object->deltaX;
    }

    if (ret != 0) {
      PlaySound(Sound_PaintBall_Ricochet, Priority_Level_4);
    }
  }

  return y;
}

//
// Check Sprite's Dirty Tiles
//
//   Check to see if any of the character tile behind an object have changed.
//      If they have, set the dirty flag of that object.
//

void
CheckDirtySprite(register ObjectPtr object)
{
  register UInt16 start, xEle, yEle, width;
  register Int16 i, height;
  register TilePtr tilePtr, tileBasePtr;

  start = object->draw.xStart;
  width = object->draw.xPixels;

  xEle = (start / charXSize);
  width += (start % charXSize);

  start = object->draw.yStart;
  height = object->draw.yPixels;

  yEle = (start / charYSize);
  height += (start % charYSize);

  tileBasePtr = &gameVars->charArray[xEle + (yEle * charXNum)];

  do {
    i = width;
    tilePtr = tileBasePtr;
    do {
      if (*tilePtr++ & dirtyChar) {
        object->flags |= flagDirty;
        return;
      }
    } while ((i -= charXSize) > 0);
    tileBasePtr += charXNum;
  } while ((height -= charYSize) > 0);
}

//
// Draw Character
//
//   Draw a character tile in either 2BPP grayscale or 4BPP color.
//     Should still support 4BPP grayscale and 8BPP color.
//

void
DrawChar(register UInt16 x,
         register Int16 y,
         register Tile which,
         register Int16 height)
{
  register void *srcPtr, *destPtr;
  register UInt16 chPrev, chNew, shiftLeft, shiftRight, maskLeft, maskRight;

  if (runVars->useColor == false) {
    srcPtr = runVars->charPtr + (which * (UInt16) (charYSize * charSrcBytes)) +
      (which * colorTableIndex) + (1 * colorTableIndex);
    if (y < topScreen) {
      srcPtr += ((topScreen - y) * charSrcBytes);
      height -= (topScreen - y);
      y = topScreen;
    } else if (y > (bottomDisplay - charXSize)) {
      height = (bottomDisplay - y);
    }

    if (height <= 0) {
      return;
    }

    destPtr = runVars->screenOffPtr + (UInt32)((x + ((y - topDisplay) * screenWidth)) / screenPixels);
    shiftRight = (x & (UInt16) (screenPixels - 1));

#if 0
    rectUpdate(x, y, charXSize, height);
#else
    rectUpdate(y, height);
#endif

    if (shiftRight == 0) {
      do {
        *((UInt8 *) (destPtr)++) = *((UInt8 *) (srcPtr)++);
        *((UInt8 *) (destPtr)) =
          *((UInt8 *) (srcPtr)++) | (*((UInt8 *) (destPtr)) & charMaskLeft);
        destPtr += (charDestBytes + 1);
      } while (--height);
    } else {
      if (shiftRight == (screenPixels - 1)) {
        do {
          chNew = *((UInt8 *) (srcPtr)++);
          *((UInt8 *) (destPtr)++) =
            (chNew >> (bitsInByte - screenDepth)) | (*((UInt8 *) (destPtr)) & charMaskRight);
          chPrev = chNew << screenDepth;
          chNew = *((UInt8 *) (srcPtr)++);
          *((UInt8 *) (destPtr)) = ((chNew >> (bitsInByte - screenDepth)) | chPrev);
          destPtr += (charDestBytes + 1);
        } while (--height);
      } else {
        shiftRight *= (UInt16) (screenDepth);
        shiftLeft = (bitsInByte - shiftRight);

        maskLeft = ~((1 << shiftLeft) - 1);
        maskRight = ((1 << (((charXPixels - charXSize) * (UInt16) (screenDepth)) - shiftRight)) - 1);

        do {
          chNew = *((UInt8 *) (srcPtr)++);
          *((UInt8 *) (destPtr)++) =
            (chNew >> shiftRight) | (*((UInt8 *) (destPtr)) & maskLeft);
          chPrev = chNew << shiftLeft;
          chNew = *((UInt8 *) (srcPtr)++);
          *((UInt8 *) (destPtr)) =
            (((chNew >> shiftRight) | chPrev) & ~maskRight) |
            (*((UInt8 *) (destPtr)) & maskRight);
          destPtr += (charDestBytes + 1);
        } while (--height);
      }
    }
  } else {
    srcPtr = runVars->charPtr + (which * (UInt16) (charYSize * charColorSrcBytes));
    if (y < topScreen) {
      srcPtr += ((topScreen - y) * charColorSrcBytes);
      height -= (topScreen - y);
      y = topScreen;
    } else if (y > (bottomDisplay - charXSize)) {
      height = (bottomDisplay - y);
    }

    if (height <= 0) {
      return;
    }

    destPtr = runVars->screenOffPtr + (UInt32) ((x + ((y - topDisplay) * screenWidth)) / screenColorPixels);
#ifdef USE_8BIT_COLOR
#if 0
    rectUpdate(x, y, charXSize, height);
#else
    rectUpdate(y, height);
#endif

    do {
      *((UInt8 *) (destPtr)++) = *((UInt8 *) (srcPtr)++);
      *((UInt8 *) (destPtr)++) = *((UInt8 *) (srcPtr)++);
      *((UInt8 *) (destPtr)++) = *((UInt8 *) (srcPtr)++);
      *((UInt8 *) (destPtr)++) = *((UInt8 *) (srcPtr)++);
      *((UInt8 *) (destPtr)) = *((UInt8 *) (srcPtr)++);
      destPtr += (charColorDestBytes + 1);
    } while (--height);
#else
    shiftRight = (x & (UInt16) (screenColorPixels - 1));

#if 0
    rectUpdate(x, y, charXSize, height);
#else
    rectUpdate(y, height);
#endif

    if (shiftRight == 0) {
      do {
        *((UInt8 *) (destPtr)++) = *((UInt8 *) (srcPtr)++);
        *((UInt8 *) (destPtr)++) = *((UInt8 *) (srcPtr)++);
        *((UInt8 *) (destPtr)) =
          (*((UInt8 *) (srcPtr)++) & ~charColorMaskLeft) |
          (*((UInt8 *) (destPtr)) & charColorMaskLeft);
        destPtr += (charColorDestBytes + 1);
      } while (--height);
    } else {
      shiftRight *= (UInt16) (screenColorDepth);
      shiftLeft = (bitsInByte - shiftRight);

      do {
        chNew = *((UInt8 *) (srcPtr)++);
        *((UInt8 *) (destPtr)++) =
          ((chNew >> shiftRight) & charColorMaskLeft) |
          (*((UInt8 *) (destPtr)) & ~charColorMaskLeft);
        chPrev = chNew << shiftLeft;
        chNew = *((UInt8 *) (srcPtr)++);
        *((UInt8 *) (destPtr)++) = (chNew >> shiftRight) | chPrev;
        chPrev = chNew << shiftLeft;
        chNew = *((UInt8 *) (srcPtr)++);
        *((UInt8 *) (destPtr)) = (chNew >> shiftRight) | chPrev;
        destPtr += (charColorDestBytes + 1);
      } while (--height);
    }
#endif
  }
}

//
// Draw Sprite
//
//   Draw a sprite or animation in either 2BPP grayscale or 4BPP color.
//     Should still support 4BPP grayscale and 8BPP color.
//

void
DrawSprite(register DrawerPtr draw,
           register UInt16 flags)
{
  register Int16 destBytes, i;
  register UInt16 j, srcBytes, skipBytes, xSize;
  register UInt16 chNew,
    chPrev,
    maskNew,
    maskPrev, shiftLeft, shiftRight, shiftAlign, shiftSkip, maskAlign;
  register UInt32 offset;
  register void *srcPtr, *destPtr, *maskPtr;
  register UInt8 *reverseArray;

  i = draw->yPixels;
  if (draw->yStart > (bottomDisplay - i))
  {
    i = (bottomDisplay - draw->yStart);
  }

  if (i <= 0)
  {
    return;
  }

  UpdateDirty(draw->xStart, draw->yStart, draw->xPixels, i);

  if (runVars->useColor == false)
  {
    xSize =
      (draw->xPixels + (UInt16) (screenPixels) -
       1) & ~(UInt16) (screenPixels - 1);

    skipBytes = ((objectXPixels - xSize) / screenPixels);
    srcBytes = (xSize / screenPixels);

    j = draw->sequence[draw->seqNum];
    offset =
      (((j * draw->height) + draw->ySkip) * (srcBytes + skipBytes)) +
      (draw->xSkip / screenPixels);
    srcPtr = draw->spritePtr + offset + ((j + 1) * colorTableIndex);
    maskPtr = draw->maskPtr + offset + ((j + 1) * sizeof(BoundingRectangle));

    if ((flags & spriteRemap) != 0)
    {
      destPtr = (void *)(runVars->theSprite);
      j = spriteSize;

      do
      {
        *((UInt8 *) (destPtr))++ =
          runVars->wakingColor[*((UInt8 *) (srcPtr))++];
      } while (--j);

      srcPtr = (void *)(runVars->theSprite);
    }

    offset =
      ((draw->xStart +
        ((draw->yStart -
          topDisplay) * (UInt16) (screenWidth))) / screenPixels);
    destPtr = runVars->screenOffPtr + offset;

    destBytes = screenXBytes - srcBytes;
    if ((flags & spriteX) != 0)
    {
      destPtr += srcBytes;
      destBytes += (2 * srcBytes);
    }
    if ((flags & spriteY) != 0)
    {
      destPtr += ((i - 1) * screenXBytes);
      destBytes -= (2 * screenXBytes);
    }

    if ((flags & spriteX) == 0)
    {
      shiftRight =
        (draw->xStart & (UInt16) (screenPixels - 1)) * (UInt16) (screenDepth);
      xSize =
        (draw->xPixels & (UInt16) (screenPixels - 1)) *
        (UInt16) (screenDepth);
      if (xSize == 0)
      {
        xSize = bitsInByte;
      }

      shiftAlign =
        ((draw->xStart + draw->xPixels) & (UInt16) (screenPixels -
                                                    1)) *
        (UInt16) (screenDepth);
      maskAlign =
        ~((1 << ((bitsInByte - shiftAlign) & (bitsInByte - 1))) - 1);

      if (shiftRight == 0)
      {
        srcBytes--;

        shiftSkip =
          (draw->xSkip & (UInt16) (screenPixels - 1)) *
          (UInt16) (screenDepth);
        if (shiftSkip != 0)
        {
          shiftRight = (bitsInByte - shiftSkip);
          shiftLeft = shiftSkip;
          xSize += shiftLeft;

          do
          {
            maskNew = *((UInt8 *) (maskPtr))++;
            chNew = *((UInt8 *) (srcPtr))++;
            maskPrev = (maskNew << shiftLeft);
            chPrev = (chNew << shiftLeft);
            j = srcBytes;
            if (j)
            {
              do
              {
                maskNew = *((UInt8 *) (maskPtr))++;
                chNew = *((UInt8 *) (srcPtr))++;
                *((UInt8 *) (destPtr))++ =
                  ((chNew >> shiftRight) | chPrev) |
                  (*((UInt8 *) (destPtr)) &
                   ~((maskNew >> shiftRight) | maskPrev));
                maskPrev = (maskNew << shiftLeft);
                chPrev = (chNew << shiftLeft);
              } while (--j);
            }
            maskNew = 0;
            chNew = 0;
            if (xSize >= bitsInByte)
            {
              maskNew = *((UInt8 *) (maskPtr)) >> shiftRight;
              chNew = *((UInt8 *) (srcPtr)) >> shiftRight;
            }
            *((UInt8 *) (destPtr))++ = ((chNew | chPrev) & maskAlign) |
              ((*((UInt8 *) (destPtr))) &
               ~((maskNew | maskPrev) & maskAlign));
            srcPtr += skipBytes;
            maskPtr += skipBytes;
            destPtr += destBytes;
          } while (--i);
        }
        else
        {
          do
          {
            j = srcBytes;
            if (j)
            {
              do
              {
                *((UInt8 *) (destPtr))++ = (*((UInt8 *) (srcPtr))++) |
                  (*((UInt8 *) (destPtr)) & ~(*((UInt8 *) (maskPtr))++));
              } while (--j);
            }
            *((UInt8 *) (destPtr))++ =
              ((*((UInt8 *) (srcPtr))++) & maskAlign) |
              ((*((UInt8 *) (destPtr))) &
               ~((*((UInt8 *) (maskPtr))++) & maskAlign));
            srcPtr += skipBytes;
            maskPtr += skipBytes;
            destPtr += destBytes;
          } while (--i);
        }
      }
      else
      {
        shiftLeft = (bitsInByte - shiftRight);
        shiftAlign = (shiftRight + xSize);

        if (shiftAlign <= bitsInByte)
        {
          srcBytes--;
        }
        else
        {
          xSize += shiftLeft;
        }

        do
        {
          chPrev = 0;
          maskPrev = 0;
          j = srcBytes;
          if (j)
          {
            do
            {
              maskNew = *((UInt8 *) (maskPtr))++;
              chNew = *((UInt8 *) (srcPtr))++;
              *((UInt8 *) (destPtr))++ = ((chNew >> shiftRight) | chPrev) |
                (*((UInt8 *) (destPtr)) &
                 ~((maskNew >> shiftRight) | maskPrev));
              maskPrev = (maskNew << shiftLeft);
              chPrev = (chNew << shiftLeft);
            } while (--j);
          }

          if (shiftAlign <= bitsInByte)
          {
            maskNew = *((UInt8 *) (maskPtr))++ >> shiftRight;
            chNew = *((UInt8 *) (srcPtr))++ >> shiftRight;
            *((UInt8 *) (destPtr))++ = ((chNew | chPrev) & maskAlign) |
              ((*((UInt8 *) (destPtr))) &
               ~((maskNew | maskPrev) & maskAlign));
          }
          else
          {
            maskNew = 0;
            chNew = 0;
            if (xSize > bitsInByte)
            {
              maskNew = *((UInt8 *) (maskPtr)) >> shiftRight;
              chNew = *((UInt8 *) (srcPtr)) >> shiftRight;
            }
            *((UInt8 *) (destPtr)) = ((chNew | chPrev) & maskAlign) |
              ((*((UInt8 *) (destPtr))) &
               ~((maskNew | maskPrev) & maskAlign));
          }
          srcPtr += skipBytes;
          maskPtr += skipBytes;
          destPtr += destBytes;
        } while (--i);
      }
    }
    else
    {
      shiftRight =
        (draw->xStart & (UInt16) (screenPixels - 1)) * (UInt16) (screenDepth);
      xSize =
        (draw->xPixels & (UInt16) (screenPixels - 1)) *
        (UInt16) (screenDepth);
      if (xSize == 0)
      {
        xSize = bitsInByte;
      }

      shiftSkip =
        (draw->xSkip & (UInt16) (screenPixels - 1)) * (UInt16) (screenDepth);

      shiftAlign =
        ((draw->xStart + draw->xPixels) & (UInt16) (screenPixels -
                                                    1)) *
        (UInt16) (screenDepth);
      maskAlign = ((1 << ((bitsInByte - shiftRight) & (bitsInByte - 1))) - 1);

      if (maskAlign == 0)
      {
        maskAlign = ~maskAlign;
      }

      reverseArray = runVars->reverseArray;
      if (shiftAlign == 0)
      {
        srcBytes--;

        if (shiftSkip != 0)
        {
          shiftRight = (bitsInByte - shiftSkip);
          shiftLeft = shiftSkip;
          xSize += shiftLeft;

          do
          {
            maskNew = *((UInt8 *) (maskPtr))++;
            chNew = *((UInt8 *) (srcPtr))++;
            maskPrev = (maskNew << shiftLeft) & 0xff;
            chPrev = (chNew << shiftLeft) & 0xff;
            j = srcBytes;
            if (j)
            {
              do
              {
                maskNew = *((UInt8 *) (maskPtr))++;
                chNew = *((UInt8 *) (srcPtr))++;
                *--((UInt8 *) (destPtr)) =
                  reverseArray[((chNew >> shiftRight) | chPrev)] |
                  (*((UInt8 *) (destPtr)) &
                   ~reverseArray[((maskNew >> shiftRight) | maskPrev)]);
                maskPrev = (maskNew << shiftLeft) & 0xff;
                chPrev = (chNew << shiftLeft) & 0xff;
              } while (--j);
            }
            maskNew = 0;
            chNew = 0;
            if (xSize >= bitsInByte)
            {
              maskNew = *((UInt8 *) (maskPtr)) >> shiftRight;
              chNew = *((UInt8 *) (srcPtr)) >> shiftRight;
            }
            *--((UInt8 *) (destPtr)) =
              (reverseArray[(chNew | chPrev)] & maskAlign) |
              ((*((UInt8 *) (destPtr))) &
               ~(reverseArray[(maskNew | maskPrev)] & maskAlign));
            srcPtr += skipBytes;
            maskPtr += skipBytes;
            destPtr += destBytes;
          } while (--i);
        }
        else
        {
          do
          {
            j = srcBytes;
            if (j)
            {
              do
              {
                *--((UInt8 *) (destPtr)) =
                  reverseArray[(*((UInt8 *) (srcPtr))++)] |
                  (*((UInt8 *) (destPtr)) &
                   ~reverseArray[(*((UInt8 *) (maskPtr))++)]);
              } while (--j);
            }
            *--((UInt8 *) (destPtr)) =
              (reverseArray[(*((UInt8 *) (srcPtr))++)] & maskAlign) |
              ((*((UInt8 *) (destPtr))) &
               ~(reverseArray[(*((UInt8 *) (maskPtr))++)] & maskAlign));
            srcPtr += skipBytes;
            maskPtr += skipBytes;
            destPtr += destBytes;
          } while (--i);
        }
      }
      else
      {
        shiftLeft = (bitsInByte - shiftAlign);

        if (shiftAlign > shiftRight)
        {
          srcBytes--;

          do
          {
            chPrev = 0;
            maskPrev = 0;
            j = srcBytes;
            if (j)
            {
              do
              {
                maskNew = *((UInt8 *) (maskPtr))++;
                chNew = *((UInt8 *) (srcPtr))++;
                *--((UInt8 *) (destPtr)) =
                  reverseArray[((chNew >> shiftLeft) | chPrev)] |
                  (*((UInt8 *) (destPtr)) &
                   ~reverseArray[((maskNew >> shiftLeft) | maskPrev)]);
                maskPrev = (maskNew << shiftAlign) & 0xff;
                chPrev = (chNew << shiftAlign) & 0xff;
              } while (--j);
            }
            maskNew = *((UInt8 *) (maskPtr))++ >> shiftLeft;
            chNew = *((UInt8 *) (srcPtr))++ >> shiftLeft;
            *--((UInt8 *) (destPtr)) =
              (reverseArray[(chNew | chPrev)] & maskAlign) |
              ((*((UInt8 *) (destPtr))) &
               ~(reverseArray[(maskNew | maskPrev)] & maskAlign));
            srcPtr += skipBytes;
            maskPtr += skipBytes;
            destPtr += destBytes;
          } while (--i);
        }
        else
        {
          shiftRight = (shiftLeft + xSize);
          if (shiftRight <= bitsInByte)
          {
            srcBytes--;
          }
          else
          {
            xSize += shiftAlign;
          }

          do
          {
            chPrev = 0;
            maskPrev = 0;
            j = srcBytes;
            if (j)
            {
              do
              {
                maskNew = *((UInt8 *) (maskPtr))++;
                chNew = *((UInt8 *) (srcPtr))++;
                *((UInt8 *) (destPtr))-- =
                  reverseArray[((chNew >> shiftLeft) | chPrev)] |
                  (*((UInt8 *) (destPtr)) &
                   ~reverseArray[((maskNew >> shiftLeft) | maskPrev)]);
                maskPrev = (maskNew << shiftAlign) & 0xff;
                chPrev = (chNew << shiftAlign) & 0xff;
              } while (--j);
            }

            if (shiftRight <= bitsInByte)
            {
              maskNew = *((UInt8 *) (maskPtr))++ >> shiftLeft;
              chNew = *((UInt8 *) (srcPtr))++ >> shiftLeft;
              *((UInt8 *) (destPtr))-- =
                (reverseArray[(chNew | chPrev)] & maskAlign) |
                ((*((UInt8 *) (destPtr))) &
                 ~(reverseArray[(maskNew | maskPrev)] & maskAlign));
            }
            else
            {
              maskNew = 0;
              chNew = 0;
              if (xSize > bitsInByte)
              {
                maskNew = *((UInt8 *) (maskPtr)) >> shiftLeft;
                chNew = *((UInt8 *) (srcPtr)) >> shiftLeft;
              }
              *((UInt8 *) (destPtr)) =
                (reverseArray[(chNew | chPrev)] & maskAlign) |
                ((*((UInt8 *) (destPtr))) &
                 ~(reverseArray[(maskNew | maskPrev)] & maskAlign));
            }
            srcPtr += skipBytes;
            maskPtr += skipBytes;
            destPtr += destBytes;
          } while (--i);
        }
      }
    }
  }
  else
  {
#ifdef USE_8BIT_COLOR
    xSize =
      (draw->xPixels + (UInt16) (screenColorPixels) -
       1) & ~(UInt16) (screenColorPixels - 1);

    skipBytes = ((objectColorXPixels - xSize) / screenColorPixels);
    srcBytes = (xSize / screenColorPixels);

    j = draw->sequence[draw->seqNum];
    offset =
      (((j * draw->height) + draw->ySkip) * (srcBytes + skipBytes)) +
      (draw->xSkip / screenColorPixels);
    srcPtr = draw->spritePtr + offset;

    if ((flags & spriteRemap) != 0)
    {
      destPtr = (void *)(runVars->theSprite);
      j = spriteColorSize;

      do
      {
        *((UInt8 *) (destPtr))++ =
          runVars->wakingColor[*((UInt8 *) (srcPtr))++];
      } while (--j);

      srcPtr = (void *)(runVars->theSprite);
    }

    offset =
      ((draw->xStart +
        ((draw->yStart -
          topDisplay) * (UInt16) (screenWidth))) / screenColorPixels);
    destPtr = runVars->screenOffPtr + offset;

    destBytes = screenColorXBytes - srcBytes;
    if ((flags & spriteX) != 0)
    {
      destPtr += srcBytes;
      destBytes += (2 * srcBytes);
    }
    if ((flags & spriteY) != 0)
    {
      destPtr += ((i - 1) * screenColorXBytes);
      destBytes -= (2 * screenColorXBytes);
    }

    if ((flags & spriteX) == 0)
    {
      do
      {
        j = srcBytes;

        do
        {
          chNew = (*((UInt8 *) (srcPtr))++);
          if (chNew != transparentColor)
          {
            *((UInt8 *) (destPtr)) = chNew;
          }
          destPtr++;
        } while (--j);
        srcPtr += skipBytes;
        destPtr += destBytes;
      } while (--i);
    }
    else
    {
      do
      {
        j = srcBytes;

        do
        {
          chNew = (*((UInt8 *) (srcPtr))++);
          destPtr--;
          if (chNew != transparentColor)
          {
            *((UInt8 *) (destPtr)) = chNew;
          }
        } while (--j);
        srcPtr += skipBytes;
        destPtr += destBytes;
      } while (--i);
    }
#else
    register UInt16 align;

    xSize =
      (draw->xPixels + (UInt16) (screenColorPixels) -
       1) & ~(UInt16) (screenColorPixels - 1);

    align = (xSize - draw->xPixels);
    shiftRight = (draw->xStart & (UInt16) (screenColorPixels - 1));

    skipBytes = ((objectColorXPixels - xSize) / screenColorPixels);
    srcBytes = (xSize / screenColorPixels);

    j = draw->sequence[draw->seqNum];
    offset =
      (((j * draw->height) + draw->ySkip) * (srcBytes + skipBytes)) +
      (draw->xSkip / screenColorPixels);
    srcPtr = draw->spritePtr + offset;
    maskPtr = draw->maskPtr + offset + ((j + 1) * sizeof(BoundingRectangle));

    if ((flags & spriteRemap) != 0)
    {
      destPtr = (void *)(runVars->theSprite);
      j = spriteColorSize;

      do
      {
        *((UInt8 *) (destPtr))++ =
          runVars->wakingColor[*((UInt8 *) (srcPtr))++];
      } while (--j);

      srcPtr = (void *)(runVars->theSprite);
    }

    offset =
      ((draw->xStart +
        ((draw->yStart -
          topDisplay) * (UInt16) (screenWidth))) / screenColorPixels);
    destPtr = runVars->screenOffPtr + offset;

    destBytes = screenColorXBytes - srcBytes;
    if ((flags & spriteX) != 0)
    {
      destPtr += srcBytes;
      destBytes += (2 * srcBytes);
    }
    if ((flags & spriteY) != 0)
    {
      destPtr += ((i - 1) * screenColorXBytes);
      destBytes -= (2 * screenColorXBytes);
    }

    if (shiftRight == 0)
    {                                            // Destination Alignment
      if (align == 0)
      {                                          // Source Alignment
        if ((flags & spriteX) == 0)
        {                                        // Draw Direction
          do
          {
            j = srcBytes;
            do
            {
              maskPrev = *((UInt8 *) (maskPtr))++;
              *((UInt8 *) (destPtr))++ =
                (*((UInt8 *) (srcPtr))++ & maskPrev) |
                (*((UInt8 *) (destPtr)) & ~maskPrev);
            } while (--j);
            srcPtr += skipBytes;
            maskPtr += skipBytes;
            destPtr += destBytes;
          } while (--i);
        }
        else
        {
          reverseArray = runVars->reverseArray;
          do
          {
            j = srcBytes;
            do
            {
              maskPrev = *((UInt8 *) (maskPtr))++;
              *--((UInt8 *) (destPtr)) =
                reverseArray[*((UInt8 *) (srcPtr))++ & maskPrev] |
                (*((UInt8 *) (destPtr)) & ~reverseArray[maskPrev]);
            } while (--j);
            srcPtr += skipBytes;
            maskPtr += skipBytes;
            destPtr += destBytes;
          } while (--i);
        }
      }
      else
      {
        shiftRight = (align * (UInt16) (screenColorDepth));
        shiftLeft = (bitsInByte - shiftRight);

        srcBytes--;
        if ((flags & spriteX) == 0)
        {
          do
          {
            j = srcBytes;
            maskNew = *((UInt8 *) (maskPtr))++;
            chNew = *((UInt8 *) (srcPtr))++;
            if (j)
            {
              do
              {
                maskPrev = (maskNew << shiftLeft) & 0xff;
                chPrev = (chNew << shiftLeft) & 0xff;
                maskNew = *((UInt8 *) (maskPtr))++;
                chNew = *((UInt8 *) (srcPtr))++;
                maskPrev |= (maskNew >> shiftRight);
                *((UInt8 *) (destPtr))++ =
                  (((chNew >> shiftRight) | chPrev) & maskPrev) |
                  (*((UInt8 *) (destPtr)) & ~maskPrev);
              } while (--j);
            }
            maskPrev = (maskNew << shiftLeft) & 0xff;
            chPrev = (chNew << shiftLeft) & 0xff;
            *((UInt8 *) (destPtr))++ =
              (chPrev & maskPrev) | (*((UInt8 *) (destPtr)) & ~maskPrev);
            srcPtr += skipBytes;
            maskPtr += skipBytes;
            destPtr += destBytes;
          } while (--i);
        }
        else
        {
          reverseArray = runVars->reverseArray;
          do
          {
            j = srcBytes;
            maskNew = *((UInt8 *) (maskPtr))++;
            chNew = *((UInt8 *) (srcPtr))++;
            maskPrev = (maskNew >> shiftRight);
            *--((UInt8 *) (destPtr)) =
              reverseArray[(chNew >> shiftRight) & maskPrev] |
              (*((UInt8 *) (destPtr)) & ~reverseArray[maskPrev]);
            if (j)
            {
              do
              {
                maskPrev = (maskNew << shiftLeft) & 0xff;
                chPrev = (chNew << shiftLeft) & 0xff;
                maskNew = *((UInt8 *) (maskPtr))++;
                chNew = *((UInt8 *) (srcPtr))++;
                maskPrev |= (maskNew >> shiftRight);
                *--((UInt8 *) (destPtr)) =
                  reverseArray[((chNew >> shiftRight) | chPrev) & maskPrev]
                  | (*((UInt8 *) (destPtr)) & ~reverseArray[maskPrev]);
              } while (--j);
            }
            srcPtr += skipBytes;
            maskPtr += skipBytes;
            destPtr += destBytes;
          } while (--i);
        }
      }
    }
    else
    {
      shiftRight *= (UInt16) (screenColorDepth);
      shiftLeft = (bitsInByte - shiftRight);

      if ((flags & spriteX) == 0)
      {
        do
        {
          j = srcBytes;
          chPrev = 0;
          maskPrev = 0;
          do
          {
            maskNew = *((UInt8 *) (maskPtr))++;
            chNew = *((UInt8 *) (srcPtr))++;
            maskPrev |= (maskNew >> shiftRight);
            *((UInt8 *) (destPtr))++ =
              (((chNew >> shiftRight) | chPrev) & maskPrev) |
              (*((UInt8 *) (destPtr)) & ~maskPrev);
            maskPrev = maskNew << shiftLeft;
            chPrev = chNew << shiftLeft;
          } while (--j);
          if (align == 0)
          {
            *((UInt8 *) (destPtr)) =
              (chPrev & maskPrev) | (*((UInt8 *) (destPtr)) & ~maskPrev);
          }
          srcPtr += skipBytes;
          maskPtr += skipBytes;
          destPtr += destBytes;
        } while (--i);
      }
      else
      {
        reverseArray = runVars->reverseArray;
        if (align == 0)
        {
          do
          {
            j = srcBytes;
            chPrev = 0;
            maskPrev = 0;
            do
            {
              maskNew = *((UInt8 *) (maskPtr))++;
              chNew = *((UInt8 *) (srcPtr))++;
              maskPrev |= (maskNew >> shiftRight);
              *((UInt8 *) (destPtr))-- =
                reverseArray[((chNew >> shiftRight) | chPrev) & maskPrev] |
                (*((UInt8 *) (destPtr)) & ~reverseArray[maskPrev]);
              maskPrev = (maskNew << shiftLeft) & 0xff;
              chPrev = (chNew << shiftLeft) & 0xff;
            } while (--j);
            *((UInt8 *) (destPtr)) =
              reverseArray[chPrev & maskPrev] | (*((UInt8 *) (destPtr)) &
                                                 ~reverseArray[maskPrev]);
            srcPtr += skipBytes;
            maskPtr += skipBytes;
            destPtr += destBytes;
          } while (--i);
        }
        else
        {
          srcBytes--;
          do
          {
            j = srcBytes;
            maskNew = *((UInt8 *) (maskPtr))++;
            chNew = *((UInt8 *) (srcPtr))++;
            if (j)
            {
              do
              {
                maskPrev = (maskNew << shiftLeft) & 0xff;
                chPrev = (chNew << shiftLeft) & 0xff;
                maskNew = *((UInt8 *) (maskPtr))++;
                chNew = *((UInt8 *) (srcPtr))++;
                maskPrev |= (maskNew >> shiftRight);
                *--((UInt8 *) (destPtr)) =
                  reverseArray[((chNew >> shiftRight) | chPrev) & maskPrev]
                  | (*((UInt8 *) (destPtr)) & ~reverseArray[maskPrev]);
              } while (--j);
            }
            maskPrev = (maskNew << shiftLeft) & 0xff;
            chPrev = (chNew << shiftLeft) & 0xff;
            *--((UInt8 *) (destPtr)) = reverseArray[chPrev & maskPrev] |
              (*((UInt8 *) (destPtr)) & ~reverseArray[maskPrev]);
            srcPtr += skipBytes;
            maskPtr += skipBytes;
            destPtr += destBytes;
          } while (--i);
        }
      }
    }
#endif
  }
}

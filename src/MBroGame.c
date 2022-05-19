/*
 * @(#)MBroGame.c
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
// Maryo Bros - Game Code
//
// Written by Chip Kerchner
//

#include "palm.h"

#include "MBroInit.h"
#include "MBroEnum.h"
#include "MBroMacr.h"

#include "MBroGame.h"
#include "MBroDraw.h"

//
// Compile-Time Definitions
//

#define NOT_ENABLE_CHEATS

// Game Variables
GameVariablesPtr gameVars;
RunVariablesPtr runVars;

// Preset Variables
#include "MBroPSet.h"

void UpdateEvents(void)  __GAME__;
void MoveFlipObject(register ObjectPtr object,
                    register UInt16 yPos,
                    register Tile tile) __GAME__;
Int16 CheckHit(register ObjectPtr object,
               register Int16 y,
               register Int16 delta) __GAME__;
void AddEvent(register UInt16 type,
              register UInt16 time,
              register void *event) __GAME__;
void DeleteEvent(register GameEventPtr theEvent) __GAME__;
void DeleteEventType(register UInt16 type) __GAME__;
void DeleteObjectEvent(register ObjectPtr object) __GAME__;
void InitializePiperObject(register ObjectPtr object) __GAME__;
void InitializeTurtleObject(register ObjectPtr object) __GAME__;
void InitializeCrabObject(register ObjectPtr object) __GAME__;
void InitializeFighterFlyObject(register ObjectPtr object) __GAME__;
void InitializeCoinObject(register ObjectPtr object) __GAME__;
void InitializeIceMachineObject(register ObjectPtr object) __GAME__;
void InitializeFireBallObject(register ObjectPtr object) __GAME__;
void InitializePaintBallObject(register ObjectPtr object) __GAME__;
void InitializePowObject(register ObjectPtr object) __GAME__;
void InitializeIcicleObject(register ObjectPtr object) __GAME__;
void InitializeFlipObject(register ObjectPtr object) __GAME__;
void InitializePreDefaultObject(register ObjectPtr object) __GAME__;
void InitializePostDefaultObject(register ObjectPtr object) __GAME__;
void InitializeObject(register ObjectPtr object,
                      register UInt16 type) __GAME__;
void ObjectIgnite(register ObjectPtr object) __GAME__;
void ObjectExtinquish(register ObjectPtr object,
                      register UInt16 kill) __GAME__;
void ResetReviveTiles(void) __GAME__;
void InitializeGame(void) __GAME__;
void CheckIntersect(void) __GAME__;
void DrawText(register UInt8 * text,
              register UInt16 x,
              register UInt16 y,
              register UInt16 direct) __GAME__;
void EraseText(register UInt16 len,
               register UInt16 x,
               register UInt16 y,
               register UInt16 direct) __GAME__;

//
// Update Rectangle Coordinates
//
//   Update bounding rectangle coordinate values.
//

#if 0
void UpdateRect(register UInt16 x1,
                register UInt16 y1,
                register UInt16 width,
                register UInt16 height)
{
  rectUpdate(x1, y1, width, height);
}
#else
void UpdateRect(register UInt16 y1,
                register UInt16 height)
{
  rectUpdate(y1, height);
}
#endif

//
// Reset Revive Tiles
//
//   Reset tiles used to determine what state the platform is in and the Pow state.
//

void
ResetReviveTiles(void)
{
  register UInt16  i;
  register TilePtr tilePtr;

  if ((gameVars->platformTile >= PChar_PLATFORM1) && (gameVars->platformTile <= (PChar_PLATFORM1 + numPlatforms))) {
    gameVars->platformTile = PChar_SPACE;

    gameVars->charArray[revivePos] = gameVars->platformTile | dirtyChar;
    gameVars->charArray[revivePos + 1] = gameVars->platformTile | dirtyChar;
  }

  if (gameVars->powNum != 0) {
    gameVars->powNum = 0;

    tilePtr = gameVars->charArray + tileYMin;
    i = charUpdate - tileYMin;

    do {
      *tilePtr++ |= dirtyChar;
    } while (--i);
  }
}

//
// Initialize Game
//
//   Initialize variables for a new game.
//

void
InitializeGame(void)
{
  register UInt16 i;

  gameVars->gameScore = 0;
  gameVars->numLives = gameVars->initNumLives;
  if (gameVars->gameDifficulty == Difficulty_Easy) {
    gameVars->numLives++;
  }
  gameVars->gameOver = false;

  gameVars->objectCount = 0;

  gameVars->powNum = 0;
  gameVars->slideNum = 0;
  gameVars->enemyCount = 0;

  gameVars->prevPhaseWidth = 0;
  gameVars->tileFriction = defaultFriction;

  gameVars->lastGameScore = (UInt32)(-1);
  gameVars->lastNumLives = gameVars->lastPhaseType = (UInt8)(-1);

  for (i = numPipes; i--;) {
    gameVars->lastPipeTime[i] = 0;
  }

  ClearLastLine();
}

//
// Play Sound
//
//   Play a sound number if priority is lower than current.
//

void
PlaySound(register UInt16 soundNum,
          register UInt16 priority)
{
#if 0
  register UInt16 i, length, prevTime;
  register UInt16 *data;
#endif

  if (!runVars->music.playing || (priority <= runVars->music.priority)) {
    runVars->music.priority = priority;
    runVars->music.looping  = false;
    runVars->music.playing  = true;

    runVars->music.musicSize = ((UInt16 *)(runVars->musicPtr))[(2 * soundNum) + 0];
    runVars->music.musicData = ((UInt16 *)(runVars->musicPtr)) + ((((UInt16 *) (runVars->musicPtr))[(2 * soundNum) + 1] +
        musicNumSamples) * 2);

    runVars->music.playback.frameIndex     = 0;
    runVars->music.playback.frameCountDown = 0;

#if 0
    length = ((UInt16 *) (runVars->musicPtr))[(2 * soundNum) + 0];
    data   = ((UInt16 *) (runVars->musicPtr)) + ((((UInt16 *) (runVars->musicPtr))[(2 * soundNum) + 1] + musicNumSamples) * 2);

    gameVars->soundCommand.cmd      = sndCmdFreqDurationAmp;
    gameVars->soundCommand.reserved = 0;
    gameVars->soundCommand.param3   = sndMaxAmp;

    prevTime = 0;
    for (i = length; i--;) {
      gameVars->soundCommand.param1 = data[1];   // fequency
      gameVars->soundCommand.param2 = data[0];   // duration

      prevTime = data[0];
      data += 2;

      DevicePlaySound(&gameVars->soundCommand);
    }
#endif
  }
}

//
// Update Screen Rectancle
//
//   Update the bounding top and bottom lines of the offscreen buffer
//

void
UpdateScreen(void)
{
#if 0
  register UInt16 x1, y1;
  RectangleType sourceBounds;

  y1 = gameVars->updateRect.y1;

  if (useColor == false) {
    x1 = gameVars->updateRect.x1 & -(UInt16) (screenPixels);

    sourceBounds.extent.x =
      (gameVars->updateRect.x2 - x1 + screenPad) & -(UInt16) (screenPad);
  } else {
    x1 = gameVars->updateRect.x1 & -(UInt16) (screenColorPixels);

    sourceBounds.extent.x =
      (gameVars->updateRect.x2 - x1 +
       screenColorPad) & -(UInt16) (screenColorPad);
  }

  sourceBounds.topLeft.x = x1;
  sourceBounds.topLeft.y = y1 - topDisplay;
  sourceBounds.extent.y = (gameVars->updateRect.y2 - y1 + 1);

  WinCopyRectangle(screenHandleOff, screenHandle, &sourceBounds,
                   sourceBounds.topLeft.x, sourceBounds.topLeft.y, winPaint);

  gameVars->updateRect.x1 = screenWidth;
  gameVars->updateRect.x2 = 0;
#endif
  if (gameVars->updateRect.y1 < topDisplay) {
    gameVars->updateRect.y1 = topDisplay;
  } else {
    if (gameVars->gameSize) {
      if (gameVars->updateRect.y1 >= bottomTitleScreen) {
        gameVars->updateRect.y1 = (bottomTitleScreen - 1);
      }
    } else {
      if (gameVars->updateRect.y1 >= bottomDisplay) {
        gameVars->updateRect.y1 = (bottomDisplay - 1);
      }
    }
  }

  if (gameVars->updateRect.y2 < topDisplay) {
    gameVars->updateRect.y2 = topDisplay;
  } else {
    if (gameVars->gameSize) {
      if (gameVars->updateRect.y2 >= bottomDisplay) {
        gameVars->updateRect.y2 = (bottomDisplay - 1);
      }
    } else {
      if (gameVars->updateRect.y2 >= bottomTitleScreen) {
        gameVars->updateRect.y2 = (bottomTitleScreen - 1);
      }
    }
  }

  if (gameVars->updateRect.y1 > gameVars->updateRect.y2) {
    gameVars->updateRect.y2 = gameVars->updateRect.y1;
  }

  GraphicsSetUpdate(gameVars->updateRect.y1 - topDisplay,
                    gameVars->updateRect.y2 - topDisplay);

  gameVars->updateRect.y1 = bottomDisplay - 1;
  gameVars->updateRect.y2 = topDisplay;
}

//
// Change Object's Movement Sequence
//
//   Change an object's sprite sequence and motion if it is different.
//

void
ChangeObjectSequence(register ObjectPtr object,
                     register UInt8 * sequence,
                     register UInt16 size,
                     register UInt16 motion)
{
  if (object->draw.sequence != sequence) {
    changeSequence(object, sequence, size);
    object->motion = motion;
  }
}

//
// Move Flip Object
//
//   Move flip object to where Maryo has hit the floor.
//

void
MoveFlipObject(register ObjectPtr object,
               register UInt16 yPos,
               register Tile tile)
{
  register UInt16 start, xEle, newStart, animNum, direction;
  register Int16 width;
  register TilePtr tilePtr;
  register AnimationPtr theAnimate;

  object->motion = Motion_Hit;
  object->deltaY = 0;

  (flipObject.draw).xStart = start = object->draw.xStart;
  (flipObject.draw).yStart = flipObject.yPos = ((yPos - 1) * charYSize);
  (flipObject.draw).xPixels = width = object->draw.xPixels;
  flipObject.flags &= ~flagNotInGame;

  animNum = (((gameVars->flipFloorTile = tile) - PChar_FLOOR1) * animateFloorNum);
  direction = (object->flags & flagDirectionX);

  if (width == objectWidth)
  {
    xEle = (start / charXSize);
    width += (start % charXSize);

    tilePtr = &gameVars->charArray[xEle + (yPos * charXNum)];
    newStart = (xEle * charXSize);

    do
    {
      tile = *tilePtr++ & ~dirtyChar;
      if ((tile >= FLOOR_START) && (tile <= FLOOR_END1))
      {
        break;
      }
      start = (newStart += charXSize);
    } while ((width -= charXSize) > 0);

    if (width >= objectWidth)
    {
      width -= charXSize;
      do
      {
        start = (newStart += charXSize);
        tile = *tilePtr++ & ~dirtyChar;
        if ((tile < FLOOR_START) || (tile > FLOOR_END1))
        {
          animNum++;
          direction = 0;
          break;
        }
      } while ((width -= charXSize) > 0);

      start -= objectWidth;
    }
    else
    {
      animNum++;
      direction = flagAnimDirectionX;
    }
  }

  flipObject.xPos = flipObject.xPrev = (start * speedFactor);

  if ((gameVars->flipFloorTile != PChar_FLOOR_INVIS)
      && (gameVars->flipFloorTile != PChar_FLOOR_SEMI_INVIS))
  {
    theAnimate = AddAnimation();
    InitializeAnimation(theAnimate, animNum + Animation_Floor1_Bend,
                        &flipObject);

    if ((animNum & (animateFloorNum - 1)) != 0)
    {
      theAnimate->draw.xStart = start;
    }

    theAnimate->draw.yStart = (flipObject.draw).yStart;
    theAnimate->draw.xSkip = (piperObject.draw).xSkip;
    theAnimate->draw.ySkip = (piperObject.draw).ySkip;
    theAnimate->draw.yPixels = animateHeight;

    if ((direction & flagAnimDirectionX) != 0)
    {
      theAnimate->flags |= flagDirectionX;
    }
    else
    {
      theAnimate->flags &= ~flagDirectionX;
    }
  }
}

//
// Check if Object Hit a Floor Tile
//
//   Check if Maryo has hit a floor tile.
//
//   Return object's current Y position
//

Int16
CheckHit(register ObjectPtr object,
         register Int16 y,
         register Int16 delta)
{
  register Tile tile;
  register Int16 i, j;
  register UInt16 x, width, xEle, yPos;
  register TilePtr tilePtr, tileBasePtr;

  if (y > topScreen)
  {
    width = (objectWidth - objectCheckWidth);
    x = (object->xPos / speedFactor);

    if ((object->flags & flagDirectionX) == 0) {
      x += (objectWidth - width);
    }
    if (x >= (UInt16) (screenWidth)) {
      x -= (UInt16) (screenWidth);
    }

    xEle = (x / charXSize);
    width = (x % charXSize) + width;

    i = (UInt16) (screenWidth) - x;
    if (width > i) {
      width = i;
    }

    yPos = ((y - 1) / charYSize);
    tileBasePtr = &gameVars->charArray[xEle + (yPos * charXNum)];

    j = -delta + 1;
    do
    {
      i = width;
      tilePtr = tileBasePtr;
      do {
        tile = *tilePtr++ & ~dirtyChar;
        if ((tile >= FLOOR_START) && (tile <= FLOOR_END1)) {
          MoveFlipObject(object, yPos, tile);

          return ((yPos + 1) * charYSize);
        } else if (tile == PChar_POW) {
          object->motion = Motion_Hit;
          object->deltaY = 0;

          gameVars->flipFloorTile = PChar_POW;
          gameVars->powNum = (hitTime / 2) - 1;

          return ((yPos + 1) * charYSize);
        }
      } while ((i -= charXSize) > 0);
      yPos++;
      tileBasePtr += charXNum;
    } while ((j -= charYSize) > 0);
  }

  return y;
}

//
// Initialize Text Event
//
//   Initialize a text event with a pointer to a string, X and Y positions,
//     time until event change, and flags.
//

void
InitializeTextEvent(register TextEventPtr theEvent,
                    register UInt8 * str,
                    register UInt16 x,
                    register UInt16 y,
                    register UInt16 time,
                    register UInt16 flags)
{
  theEvent->text = str;
  theEvent->x = x;
  theEvent->y = y;
  theEvent->time = time;
  theEvent->flags = flags;
}

//
// Initialize Key Event
//
//   Initialize a key list and key delay list event (for automatic keys
//     for Maryo's movement).
//

void
InitializeKeyEvent(register KeyEventPtr theEvent,
                   register UInt8 * keyList,
                   register UInt8 * keyDelayList)
{
  theEvent->keyList = keyList;
  theEvent->keyNum = 0;
  theEvent->keyDelayList = keyDelayList;
  theEvent->keyDelayNum = 0;
}

//
// Initialize Freeze Event
//
//   Initialize a freeze event from ice machine's position
//

void
InitializeFreezeEvent(register FreezeEventPtr theEvent,
                      register ObjectPtr object)
{
  register UInt16 xPos, xEle, yEle, base;

  xPos = (object->xPos / speedFactor);

  xEle = (xPos + (objectWidth / 2)) / charXSize;
  yEle = (object->yPos + object->draw.height + 1) / charYSize;

  base = xEle + (yEle * charXNum);

  theEvent->tileLoc2 = &gameVars->charArray[base];

  if ((xPos % charXSize) == 0) {
    theEvent->tileLoc1 = &gameVars->charArray[base - 1];
  } else {
    theEvent->tileLoc1 = theEvent->tileLoc2;
  }
}

//
// Add Game Event
//
//   Add an event to the list which the specified type and time.
//

void
AddEvent(register UInt16 type,
         register UInt16 time,
         register void *event)
{
  register GameEventPtr theEvent;

  theEvent = (GameEventPtr) (NULL);

  if ((type == Event_EndPhase) || (type == Event_Revive)) {
    register UInt16 i, deltaTime;

    i = gameVars->eventGameCount;
    while (i--) {
      theEvent = &gameVars->eventList[i];
      if (theEvent->type == type) {
        type = Event_Found;
        deltaTime = theEvent->time - gameVars->nextPeriodTime;
        if (deltaTime < time) {
          time = deltaTime;
        }
        break;
      }
    }
  }

  if (gameVars->eventGameCount < maxNumEvents) {
    if (type != Event_Found) {
      theEvent = &gameVars->eventList[gameVars->eventGameCount++];

      theEvent->type = type;
      theEvent->event = event;
    }

    if (time < gameVars->keyRepeatRate) {
      time = gameVars->keyRepeatRate;
    }

    theEvent->time = gameVars->nextPeriodTime + (UInt32) (time);
  }
}

//
// Delete Game Event
//
//   Delete an event from the list.
//

void
DeleteEvent(register GameEventPtr theEvent)
{
  register GameEventPtr lastEvent;

  if ((gameVars->eventGameCount != 0) && (theEvent != (GameEventPtr) (NULL))) {
    lastEvent = &gameVars->eventList[--gameVars->eventGameCount];

    if (theEvent != lastEvent) {
      theEvent->time = lastEvent->time;
      theEvent->type = lastEvent->type;
      theEvent->event = lastEvent->event;
    }
  }
}

//
// Delete Game Event Type
//
//   Delete an event from the list which matches the passed in type.
//

void
DeleteEventType(register UInt16 type)
{
  register UInt16 i;
  register GameEventPtr theEvent;

  i = gameVars->eventGameCount;
  while (i--) {
    theEvent = &gameVars->eventList[i];

    if (theEvent->type == type) {
      DeleteEvent(theEvent);
    }
  }
}

//
// Delete Object Event
//
//   Delete an event from the list which matches the passed in object.
//

void
DeleteObjectEvent(register ObjectPtr object)
{
  register GameEventPtr theEvent;
  register UInt16 i;

  if (object != (ObjectPtr) (NULL)) {
    i = gameVars->eventGameCount;
    while (i--) {
      theEvent = &gameVars->eventList[i];
      if ((ObjectPtr) (theEvent->event) == object) {
        DeleteEvent(theEvent);
      }
    }
  }
}

//
// Initialize a Piper Object
//
//   Initialize Maryo's settings to set values.
//

void
InitializePiperObject(register ObjectPtr object)
{
  object->xPos = (piperStartX * speedFactor);
  object->yPos = piperStartY;
  object->deltaX = 0;
  object->deltaY = 0;
  object->draw.seqNum = piperStartSeqNum;
  object->motion = Motion_Stop;
  object->level = Level_Character;
  object->draw.spritePtr = runVars->piperPtr;
  object->draw.maskPtr = runVars->piperMaskPtr;
  object->draw.sequence = piperStopSequence;
  object->size = sizeof(piperStopSequence);
}

//
// Initialize a Turtle Object
//
//   Initialize a turtle's settings to set values.
//

void
InitializeTurtleObject(register ObjectPtr object)
{
  object->draw.seqNum = turtleStartSeqNum;
  object->motion = Motion_Run;
  object->level = Level_Enemy;
  object->flags = flagCount;
  object->draw.spritePtr = runVars->turtlePtr;
  object->draw.maskPtr = runVars->turtleMaskPtr;
  object->draw.sequence = turtleSequence;
  object->size = sizeof(turtleSequence);
}

// Initialize a Crab Object
//
//   Initialize a crab's settings to set values.
//

void
InitializeCrabObject(register ObjectPtr object)
{
  object->draw.seqNum = crabStartSeqNum;
  object->motion = Motion_Run;
  object->level = Level_Enemy;
  object->flags = flagCount;
  object->draw.spritePtr = runVars->crabPtr;
  object->draw.maskPtr = runVars->crabMaskPtr;
  object->draw.sequence = crabSequence;
  object->size = sizeof(crabSequence);
}

//
// Initialize a Fighter Fly Object
//
//   Initialize a fighter fly's settings to set values.
//

void
InitializeFighterFlyObject(register ObjectPtr object)
{
  object->draw.seqNum = fighterFlyStartSeqNum;
  object->motion = Motion_Bounce;
  object->level = Level_Enemy;
  object->flags = flagCount;
  object->draw.spritePtr = runVars->fighterFlyPtr;
  object->draw.maskPtr = runVars->fighterFlyMaskPtr;
  object->draw.sequence = fighterFlySequence;
  object->size = sizeof(fighterFlySequence);
}

//
// Initialize a Coin Object
//
//   Initialize a coin's settings to set values.
//

void
InitializeCoinObject(register ObjectPtr object)
{
  object->draw.seqNum = coinStartSeqNum;
  object->motion = Motion_Run;
  object->level = Level_Bonus;
  object->draw.spritePtr = runVars->coinPtr;
  object->draw.maskPtr = runVars->coinMaskPtr;
  object->draw.sequence = coinSequence;
  object->size = sizeof(coinSequence);
}

//
// Initialize an Ice Machine Object
//
//   Initialize an ice machine's settings to set values.
//

void
InitializeIceMachineObject(register ObjectPtr object)
{
  object->draw.seqNum = iceMachineStartSeqNum;
  object->motion = Motion_Run;
  object->level = Level_Enemy;
  object->flags = flagOnceThru;
  object->draw.spritePtr = runVars->iceMachinePtr;
  object->draw.maskPtr = runVars->iceMachineMaskPtr;
  object->draw.sequence = iceMachineSequence;
  object->size = sizeof(iceMachineSequence);
}

//
// Initialize a Fire Ball Object
//
//   Initialize a fireball's settings to set values.
//

void
InitializeFireBallObject(register ObjectPtr object)
{
  object->draw.seqNum = fireBallStartSeqNum;
  object->motion = Motion_Wave;
  object->level = Level_Inanimate;
  object->flags = flagRotating;
  object->draw.spritePtr = runVars->fireBallPtr;
  object->draw.maskPtr = runVars->fireBallMaskPtr;
  object->draw.sequence = fireBallSequence;
  object->size = sizeof(fireBallSequence);
}

//
// Initialize a Paint Ball Object
//
//   Initialize a paintball's settings to set values.
//

void
InitializePaintBallObject(register ObjectPtr object)
{
  object->xPos = (paintBallStartX * speedFactor);
  object->yPos = paintBallStartY;
  object->draw.seqNum = paintBallStartSeqNum;
  object->motion = Motion_Ricochet;
  object->level = Level_Inanimate;
  object->flags = flagRotating;
  object->draw.spritePtr = runVars->paintBallPtr;
  object->draw.maskPtr = runVars->paintBallMaskPtr;
  object->draw.sequence = paintBallSequence;
  object->size = sizeof(paintBallSequence);
}

//
// Initialize a Pow Object
//
//   Initialize an icicle's settings to set values.
//

void
InitializePowObject(register ObjectPtr object)
{
  object->xPos = (powStartX * speedFactor);
  object->yPos = powStartY;
  object->draw.seqNum = powStartSeqNum;
  object->motion = Motion_Stop;
  object->level = Level_Misc;
  object->draw.spritePtr = runVars->powPtr;
  object->draw.maskPtr = runVars->powMaskPtr;
  object->draw.sequence = powSequence;
  object->size = sizeof(powSequence);
}

// Initialize an Icicle
void
InitializeIcicleObject(register ObjectPtr object)
{
  register UInt16 num, num2;

  num = (SysRandom(0) % numIcicleStarts);
  if (gameVars->icicleFill[num] != 0)
  {
    for (num2 = numIcicleStarts; num2--;)
    {
      if (++num == numIcicleStarts)
      {
        num = 0;
      }
      if (gameVars->icicleFill[num] == 0)
      {
        break;
      }
    }
  }
  gameVars->icicleFill[num] = 1;

  object->xPos = (icicleStarts[(num * 2)] * speedFactor);
  object->yPos = icicleStarts[(num * 2) + 1];
  object->draw.seqNum = icicleStartSeqNum;
  object->motion = Motion_Spin;
  object->level = Level_Inanimate;
  object->draw.spritePtr = runVars->iciclePtr;
  object->draw.maskPtr = runVars->icicleMaskPtr;
  object->draw.sequence = icicleDripSequence;
  object->size = sizeof(icicleDripSequence);
}

//
// Initialize a Flip Object
//
//   Pre-initialize an object's settings to set values.
//

void
InitializeFlipObject(register ObjectPtr object)
{
  object->xPos = (flipStartX * speedFactor);
  object->yPos = flipStartY;
  object->draw.seqNum = flipStartSeqNum;
  object->motion = Motion_Stop;
  object->level = Level_Other;
  object->flags = (flagInvisible | flagNotInGame);
  object->draw.spritePtr = (void *)(NULL);
  object->draw.maskPtr = runVars->flipMaskPtr;
  object->draw.sequence = (void *)(NULL);
  object->size = 0;
}

// Pre-Initialize Object to Default Values
void
InitializePreDefaultObject(register ObjectPtr object)
{
  object->deltaX = 0;
  object->deltaY = 0;
  object->width = objectWidth;
  object->flags = 0;
}

//
// Post-Initialize Object to Default Values
//
//   Post-initialize an object's settings to set values.
//

void
InitializePostDefaultObject(register ObjectPtr object)
{
  if (object == &piperObject) {
    object->draw.height = piperHeight;
  } else if (object == &flipObject) {
    object->draw.height = flipHeight;
  } else {
    object->draw.height = objectHeight;
    if (object != &powObject) {
      if (gameVars->phaseType == Phase_Bonus) {
        if (object->level == Level_Bonus) {
          gameVars->enemyCount++;
        }
      } else {
        if ((gameVars->phaseType == Phase_Normal) || ((object->type == Object_IceMachine) &&
            (gameVars->phaseType != Phase_DisplayHighScores))) {
          object->flags |= (flagWaitToEnter | flagInvisible | flagNotInGame);
        }
        if ((object->flags & flagCount) != 0) {
          gameVars->enemyCount++;
        }
      }
    }
  }

  if (object->draw.seqNum > 0) {
    object->prevSeqNum = object->draw.seqNum - 1;
  } else if (object->size > 0) {
    object->prevSeqNum = object->size - 1;
  } else {
    object->prevSeqNum = 0;
  }
}

//
// Initialize an Object
//
//   Initialize an object's settings to set values.
//

void
InitializeObject(register ObjectPtr object,
                 register UInt16 type)
{
  object->type = type;
  InitializePreDefaultObject(object);

  switch (type)
  {
    case Object_Character:
      InitializePiperObject(object);
      break;

    case Object_Flip:
      InitializeFlipObject(object);
      break;

    case Object_Pow:
      InitializePowObject(object);
      break;

    case Object_Turtle:
      InitializeTurtleObject(object);
      object->deltaX = turtleSpeedX;
      break;

    case Object_Crab:
      InitializeCrabObject(object);
      object->deltaX = crabSpeedX;
      break;

    case Object_FighterFly:
      InitializeFighterFlyObject(object);
      object->deltaX = fighterFlySpeedX;
      break;

    case Object_IceMachine:
      InitializeIceMachineObject(object);
      object->deltaX = iceMachineSpeedX;
      break;

    case Object_Coin:
      InitializeCoinObject(object);
      break;

    case Object_FireBall:
      InitializeFireBallObject(object);
      break;

    case Object_PaintBall:
      InitializePaintBallObject(object);
      object->deltaX = paintBallSpeedX;
      object->deltaY = paintBallSpeedY;
      break;

    case Object_Icicle:
      InitializeIcicleObject(object);
      break;
  }

  if ((gameVars->phaseType != Phase_Instructions) && (gameVars->phaseType != Phase_DisplayHighScores)) {
    if (object->level == Level_Enemy) {
      if (gameVars->gameDifficulty == Difficulty_Hard) {
        object->deltaX++;
      }
    }
  }

  if ((gameVars->phaseType == Phase_Normal) && (object->level == Level_Enemy)) {
    if ((SysRandom(0) & 0x1) != 0x0) {
      object->deltaX = -object->deltaX;
    }
  }

  InitializePostDefaultObject(object);
}

//
// Remove Object
//
//   Remove an object from the playing field (make it invisible and do not check
//     for intersections).
//

void
ObjectRemove(register ObjectPtr object)
{
  if ((object->flags & flagWaitToEnter) != 0)
  {
    DeleteObjectEvent(object);
  }
  object->yPos = screenYMax;
  object->deltaX = 0;
  object->deltaY = 0;
  object->motion = Motion_Stop;
  object->flags |= (flagInvisible | flagNotInGame);
}

//
// Reincarnate Object
//
//   Handle the reincarnation of an object.  Reintialize the object and 
//     add it to the entrance event list.
//
//   Return object's current Y position
//

Int16
ObjectReincarnate(register ObjectPtr object)
{
  register UInt16 num, type, x, enterTime;

  type = object->type;
  if (type == Object_Icicle) {
    x = (object->xPos / speedFactor);

    for (num = 0; num < numIcicleStarts; num++) {
      if (x == icicleStarts[num * 2]) {
        gameVars->icicleFill[num] = 0;
        break;
      }
    }
  }

  enterTime = entranceInterTime;
  if (gameVars->gameDifficulty == Difficulty_Easy) {
    enterTime += (entranceInterTime / 2);
  } else if (gameVars->gameDifficulty == Difficulty_Hard) {
    enterTime -= (entranceInterTime / 2);
  }
  InitializeObject(object, type);
  AddEvent(Event_Entrance, enterTime, (void *)(object));

  object->flags |= (flagInvisible | flagNotInGame | flagWaitToEnter);

  return object->yPos;
}

//
// Collect Object Bonus
//
//   Handle the collection of bonus points from a coin object.
//
//   Return object's current Y position
//

Int16
ObjectBonus(register ObjectPtr object)
{
  if (gameVars->phaseType == Phase_Bonus) {
    gameVars->totalCoins++;
    if (--gameVars->enemyCount == 0) {
      gameVars->gameState |= gameLast;
    }
  }

  if (object->type == Object_IceMachine) {
    register AnimationPtr theAnimate;

    if (gameVars->powNum == 0) {
      theAnimate = AddAnimation();
      InitializeAnimation(theAnimate, Animation_Points_500, object);

      theAnimate->draw.yPixels = animateHeight;
      theAnimate->flags &= ~flagAnimDirectionX;
    }

    ObjectReincarnate(object);

    object->yPos = screenYMax;
  } else {
    object->flags |= flagNotInGame;
    object->flags &= ~flagDirectionX;

    object->deltaX = 0;
    if (object->motion == Motion_Flip) {
      object->motion = Motion_Spin;
      object->deltaY = 0;
    } else {
      if (gameVars->phaseType != Phase_Bonus) {
        object->motion = Motion_Flip;
      }

      if (gameVars->phaseType == Phase_Normal) {
        changeSequence(object, coinDieSequence, sizeof(coinDieSequence));
        AddEvent(Event_Remove, coinRemoveTime, (void *)(object));
      } else {
        changeSequence(object, coinCollectSequence, sizeof(coinCollectSequence));
        AddEvent(Event_Remove, collectTime, (void *)(object));
      }


      if (gameVars->powNum == 0) {
        PlaySound(Sound_Coin, Priority_Level_2);
      }
    }
  }

  return object->yPos;
}

//
// Ignite FireBall or PaintBall
//
//   Handle the reigniting of a fireball or paintball.  Reintialize the object and
//     add it to the entrance event list.
//

void
ObjectIgnite(register ObjectPtr object)
{
  register Int16 deltaX, deltaY, flags;
  register UInt32 igniteTime;

  deltaX = object->deltaX;
  deltaY = object->deltaY;
  flags = object->flags;

  InitializeObject(object, object->type);

  if (object->type == Object_PaintBall) {
    object->xPos += (((Int16) (3 - (SysRandom(0) % paintBallLocations)) * paintBallDeltaX) * speedFactor);
  }

  igniteTime = reigniteTime;
  if (gameVars->gameDifficulty == Difficulty_Easy) {
    igniteTime *= 2;
  } else if (gameVars->gameDifficulty == Difficulty_Hard) {
    igniteTime /= 2;
  }
  if (gameVars->gameDifficulty != Difficulty_Easy) {
    if (((object->type == Object_FireBall) && (gameVars->gamePhase >= phaseInstructIceMachine)) || ((flags & flagHit) != 0)) {
      igniteTime = 0;
    }
  }
  if (gameVars->enemyCount != 0) {
    AddEvent(Event_Entrance, igniteTime, (void *)(object));
  }

  object->deltaX = deltaX;
  object->deltaY = deltaY;
  object->flags |= (flagInvisible | flagNotInGame | flagWaitToEnter);
}

//
// Extinquish FireBall or PaintBall
//
//   Extinquish a fireball or paintball.  Adjust its speed for retry onto the playing field.
//

void
ObjectExtinquish(register ObjectPtr object,
                 register UInt16 kill)
{
  register UInt16 type;

  type = object->type;
  if (type == Object_PaintBall)
  {
    register Int16 delta;

    delta = object->deltaY;
    if (delta > 0)
    {
      delta = -delta;
    }

    if ((gameVars->powNum == 0) && (kill == 0))
    {
      object->flags |= flagHit;
      if (delta == paintBallSpeedY)
      {
        object->deltaX = paintBallFastSpeedX;
        object->deltaY = paintBallFastSpeedY;
      }
      else if (delta == paintBallFastSpeedY)
      {
        object->deltaX = paintBallFastestSpeedX;
        object->deltaY = paintBallFastestSpeedY;
      }
      else
      {
        object->deltaX = paintBallInsaneSpeedX;
        object->deltaY = paintBallInsaneSpeedY;
      }
    }
    else
    {
      if (delta == paintBallSpeedY)
      {
        object->deltaX = paintBallSpeedX;
        object->deltaY = paintBallSpeedY;
      }
      else if (delta == paintBallFastSpeedY)
      {
        object->deltaX = paintBallFastSpeedX;
        object->deltaY = paintBallFastSpeedY;
      }
      else if (delta == paintBallFastestSpeedY)
      {
        object->deltaX = paintBallFastestSpeedX;
        object->deltaY = paintBallFastestSpeedY;
      }
      else
      {
        object->deltaX = paintBallInsaneSpeedX;
        object->deltaY = paintBallInsaneSpeedY;
      }
    }
  }

  object->motion = Motion_Appear;
  object->flags |= flagNotInGame;

  if (type == Object_FireBall)
  {
    changeSequence(object, fireBallDisappearSequence,
                   sizeof(fireBallDisappearSequence));
  }
  else
  {
    changeSequence(object, paintBallDisappearSequence,
                   sizeof(paintBallDisappearSequence));
  }

  AddEvent(Event_Disappear, appearTime, (void *)(object));
}

//
// Sequelch FireBall or PaintBall
//
//   Handle killing a fireball or paintball.
//

void
ObjectSequelch(register ObjectPtr object)
{
  if (gameVars->phaseType == Phase_Normal) {
    if (gameVars->powNum == 0) {
      register AnimationPtr theAnimate;

      theAnimate = AddAnimation();

      if (object->motion == Motion_Ricochet) {
        gameVars->gameScore += pointsPaintBall;

        InitializeAnimation(theAnimate, Animation_Points_1000, object);
      } else {
        gameVars->gameScore += pointsFireBall;

        InitializeAnimation(theAnimate, Animation_Points_200, object);
      }

      theAnimate->draw.yPixels = animateHeight;
      theAnimate->flags &= ~flagAnimDirectionX;

      ChangeAnimationPos(theAnimate, 0, kickDelta);
    }
  }

  ObjectExtinquish(object, 0);
}

//
// Start Flip Object
//
//   Handle flipping over and back of an object by adjusting its motion and sprite sequence.
//

void
ObjectFlipStart(register ObjectPtr object)
{
  register UInt16 motion, level, flags, type;

  motion = object->motion;
  level = object->level;
  if ((motion == Motion_Run) || (motion == Motion_Bounce) || (motion == Motion_Stop) || (motion == Motion_Ricochet) ||
      (motion == Motion_Wave) || ((motion == Motion_Appear) && (level == Level_Enemy)) ||
      ((motion == Motion_Spin) && (level != Level_Bonus))) {
    if ((level == Level_Bonus) || (level == Level_Enemy)) {
      object->motion = Motion_Flip;
      object->deltaY = 0;

      type = object->type;
      flags = object->flags;
      DeleteObjectEvent(object);
      if ((flags & flagFlipped) == 0) {
        if ((type == Object_Crab) && ((flags & flagHit) == 0)) {
          if (object->deltaX > 0) {
            object->deltaX += (Int8) (crabHitSpeedX - crabSpeedX);
          } else {
            object->deltaX -= (Int8) (crabHitSpeedX - crabSpeedX);
          }
        } else {
          if (type == Object_Crab) {
            if (object->deltaX > 0) {
              object->deltaX = (Int8) (crabSpeedX);
            } else {
              object->deltaX = (Int8) (-crabSpeedX);
            }
          }

          object->deltaX = -object->deltaX;

          if ((gameVars->phaseType == Phase_Normal) && (gameVars->powNum == 0) && ((flags & flagCount) != 0)) {
            gameVars->gameScore += pointsFlip;
          } else if (type == Object_IceMachine) {
            DeleteObjectEvent(object);
            DeleteAnimationObject(object);

            changeSequence(object, iceMachineBreakSequence, sizeof(iceMachineBreakSequence));
          } else if (type == Object_Coin) {
            object->flags |= flagNotInGame;
            object->flags &= ~flagDirectionX;

            changeSequence(object, coinDieSequence, sizeof(coinDieSequence));
            AddEvent(Event_Remove, coinRemoveTime, (void *)(object));

            PlaySound(Sound_Coin, Priority_Level_2);
          }
        }
      } else {
        if (type == Object_Turtle) {
          DeleteAnimationObject(object);
        }

        object->deltaX = -object->flipDeltaX;
        object->flags &= ~flagWaking;
      }

      if (gameVars->powNum == 0) {
        register UInt16 xPos, flipXPos;

        if (object->deltaX > 0) {
          level = object->deltaX;
        } else {
          level = -object->deltaX;
        }

        xPos = object->draw.xStart;
        flipXPos = (flipObject.draw).xStart;

        if (xPos <= (flipXPos - (objectWidth - (flipArea / 3)))) {
          object->flipDeltaX = -(Int8) (level);
        } else if (xPos >= (flipXPos + (objectWidth - (flipArea / 3)))) {
          object->flipDeltaX = (Int8) (level);
        } else {
          object->flipDeltaX = 0;
        }

        PlaySound(Sound_Flip, Priority_Level_3);
      } else {
        object->flipDeltaX = 0;
      }
    } else if (level == Level_Inanimate) {
      if ((motion == Motion_Spin) && (gameVars->powNum != 0)) {
        DeleteObjectEvent(object);

        object->motion = Motion_Drop;
      } else {
        ObjectSequelch(object);
      }
    }
  }
}

//
// Change Enemy Sequence
//
//   Change enemy's sprite sequence based on flip condition and type.
//

void
ChangeEnemySequence(register ObjectPtr object)
{
  register UInt16 type, flag;

  flag = object->flags;
  type = object->type;

  if (type == Object_Turtle) {
    if ((object->flags & flagFlipped) == 0) {
      changeSequence(object, turtleFlipSequence, sizeof(turtleFlipSequence));
    } else {
      changeSequence(object, turtleSequence, sizeof(turtleSequence));
    }
  } else if (type == Object_Crab) {
    if ((object->flags & flagFlipped) == 0) {
      if ((object->flags & flagHit) == 0) {
        changeSequence(object, crabHitSequence, sizeof(crabHitSequence));
      } else {
        changeSequence(object, crabFlipSequence, sizeof(crabFlipSequence));
      }
    } else {
      changeSequence(object, crabSequence, sizeof(crabSequence));
    }
  } else if (type == Object_FighterFly) {
    if ((object->flags & flagFlipped) == 0) {
      changeSequence(object, fighterFlyFlipSequence, sizeof(fighterFlyFlipSequence));
    } else {
      changeSequence(object, fighterFlySequence, sizeof(fighterFlySequence));
    }
  }
}

//
// Flip Back Over Object
//
//   Flip object back over and adjust the enemy's speed and motion based on type.
//

void
ObjectFlipOver(register ObjectPtr object,
               register UInt16 event)
{
  object->deltaX = object->flipDeltaX;

  if (event != 0) {
    register UInt16 fastSpeed;

    if (object->type == Object_Turtle) {
      fastSpeed = turtleFastSpeedX;
    } else if (object->type == Object_Crab) {
      fastSpeed = crabFastSpeedX;
    } else {
      fastSpeed = fighterFlyFastSpeedX;
    }

    if (gameVars->gameDifficulty == Difficulty_Easy) {
      fastSpeed--;
    } else if (gameVars->gameDifficulty == Difficulty_Hard) {
      fastSpeed++;
    }

    if (object->deltaX > 0) {
      if (++object->deltaX > (Int16)(fastSpeed)) {
        object->deltaX = (Int16)(fastSpeed);
      }
    } else {
      if (--object->deltaX < (Int16)(-fastSpeed)) {
        object->deltaX = (Int16)(-fastSpeed);
      }
    }
  }

  object->flags &= ~(flagHit | flagFlipped | flagWaking);

  if (object->type == Object_FighterFly) {
    object->motion = Motion_Bounce;
  } else {
    object->motion = Motion_Run;
  }
}

//
// Stop Flip Object
//
//   Stop object's flip motion and add event to list to wake up enemy.
//

void
ObjectFlipStop(register ObjectPtr object)
{
  register UInt16 timeWake;

  object->deltaY = 0;

  if ((object->flags & flagFlipped) != 0) {
    ObjectFlipOver(object, 0);
  } else {
    if ((object->type == Object_Crab) && ((object->flags & flagHit) == 0)) {
      object->motion = Motion_Run;
      object->flags |= flagHit;
    } else {
      object->motion = Motion_Spin;
      object->flags |= flagFlipped;

      timeWake = wakeTime;
      if (gameVars->gameDifficulty == Difficulty_Easy) {
        timeWake += (wakeTime / 2);
      } else if (gameVars->gameDifficulty == Difficulty_Hard) {
        timeWake -= (wakeTime / 2);
      }
      AddEvent(Event_Wakeup, timeWake, (void *)(object));
    }
  }
}

//
// Fix Last Enemy's Speed
//
//   Speed up the last enemy.
//

void
FixLastEnemy(void)
{
  register ObjElePtr objPtr;
  register ObjectPtr object;
  register UInt16 flags, motion;

  objPtr = gameVars->spriteHead;
  while (objPtr != (ObjElePtr) (NULL))
  {
    object = (objPtr->object);

    motion = object->motion;
    if ((motion != Motion_Stop) && (motion != Motion_Die))
    {
      flags = object->flags;
      if ((flags & flagCount) != 0)
      {
        if (object->type != Object_FighterFly)
        {
          if ((flags & flagFlipped) != 0)
          {
            if (object->flipDeltaX > 0)
            {
              object->flipDeltaX += lastSpeedX;
            }
            else
            {
              object->flipDeltaX -= lastSpeedX;
            }
          }
          else
          {
            if (object->deltaX > 0)
            {
              object->deltaX += lastSpeedX;
            }
            else
            {
              object->deltaX -= lastSpeedX;
            }
          }
        }

        return;
      }
    }

    objPtr = objPtr->next;
  }
}

//
// Game Over
//
//   Display game over text and prepare to end playable game.
//

void
GameOver(void)
{
  register TextEventPtr theEvent;

  PlaySound(Sound_Game_Over, Priority_Level_0);

  theEvent = &gameVars->textEvents[0];

  StrPrintF(gameVars->gameStr[1], gameOverText);
  InitializeTextEvent(theEvent, gameVars->gameStr[1], gameOverStartX, gameOverStartY, gameOverTime, (textNormal | textDraw));

  AddEvent(Event_Text, 0, (void *)(theEvent));
  AddEvent(Event_EndPhase, gameOverTime, (void *)(theEvent));

  piperObject.flags |= (flagInvisible | flagNotInGame);

  gameVars->lastGamePhase = gameVars->gamePhase;
  gameVars->gamePhase = phaseEnterHighScore - 1;
}

//
// Revive Piper
//
//   Revive Maryo or if no more lives, go into game over phase.
//

void
RevivePiper(void)
{
  register AnimationPtr theAnimate;

#ifdef ENABLE_CHEATS
  if (gameVars->numLives)
  {
    gameVars->numLives--;
  }
  {
#else
  if (--gameVars->numLives == 0)
  {
    GameOver();
  }
  else
  {
#endif
    PlaySound(Sound_Revive, Priority_Level_0);

    ChangeObjectSequence(&piperObject, piperStopSequence, sizeof(piperStopSequence), Motion_Stop);

    piperObject.xPos = (piperReviveX * speedFactor);
    piperObject.yPos = piperReviveStartY;
    piperObject.deltaX = 0;
    piperObject.deltaY = 0;
    piperObject.flags = (flagNotInGame | flagDirectionX);
    piperObject.motion = Motion_Revive;

    theAnimate = AddAnimation();
    InitializeAnimation(theAnimate, Animation_Platform, (ObjectPtr) (NULL));

    theAnimate->draw.xStart = piperReviveX;
    theAnimate->draw.yStart = piperReviveStartY + (piperObject.draw).height + 2;

    theAnimate->deltaY = piperReviveDeltaY;
    theAnimate->flags = flagAnimLooping;
  }
}

//
// Kill Object or Change Object Into Coin
//
//   Kill an object and if an animate enemy change into a bonus coin.
//
//   Return object's current Y position
//

Int16
ObjectKill(register ObjectPtr object)
{
  if (object == &piperObject) {
    piperObject.flags = (flagNotInGame | flagInvisible);

    AddEvent(Event_Entrance, reviveTime, (void *)(object));
  } else {
    if ((gameVars->phaseType != Phase_Instructions) && (gameVars->enemyCount > 0)) {
      if ((object->flags & flagCount) != 0) {
        InitializeObject(object, Object_Coin);

        object->flags |= flagOnceThru;
        AddEvent(Event_Entrance, coinTime, (void *)(object));

        return screenYMax;
      }
    }

    ObjectRemove(object);
  }

  return object->yPos;
}

//
// Object Death
//
//  Change an object to the death sequence.
//

void
ObjectDeath(register ObjectPtr object)
{
  register UInt16 type;

  if ((object->flags & flagCount) == 0) {
    DeleteObjectEvent(object);
    DeleteAnimationObject(object);

    object->deltaX = 0;
    object->deltaY = 0;
    object->motion = Motion_Spin;
    object->flags |= flagNotInGame;
  }

  type = object->type;

  if (type == Object_Coin) {
    changeSequence(object, coinDieSequence, sizeof(coinDieSequence));

    AddEvent(Event_Invisible, removeTime, (void *)(object));
  } else if (type == Object_IceMachine) {
    DeleteAnimationObject(object);

    changeSequence(object, iceMachineBreakSequence,
                   sizeof(iceMachineBreakSequence));
    AddEvent(Event_Invisible, disappearTime, (void *)(object));
  } else if (type == Object_FireBall) {
    changeSequence(object, fireBallDisappearSequence, sizeof(fireBallDisappearSequence));

    AddEvent(Event_Disappear, appearTime, (void *)(object));
  } else if (type == Object_PaintBall) {
    changeSequence(object, paintBallDisappearSequence, sizeof(paintBallDisappearSequence));

    AddEvent(Event_Disappear, appearTime, (void *)(object));
  } else if (type == Object_Icicle) {
    object->motion = Motion_Stop;
  }
}

//
// Kill All Objects
//
//   Kill all objects (when finishing a phase).
//

void
KillAllObjects(register ObjectPtr object)
{
  register ObjElePtr objPtr;
  register ObjectPtr object2;

  piperObject.deltaX = piperObject.deltaY = 0;
  piperObject.motion = Motion_Stop;

  gameVars->slideNum = 0;
  gameVars->tileFriction = defaultFriction;
  gameVars->gameState |= gameKill;

  objPtr = gameVars->spriteHead;
  while (objPtr != (ObjElePtr)(NULL)) {
    object2 = objPtr->object;

    if (object2->num >= numSetObjects) {
      if (object == (ObjectPtr)(NULL)) {
        ObjectKill(object2);
      } else if (object2 != object) {
        ObjectDeath(object2);
      }
    }

    objPtr = objPtr->next;
  }
}

//
// Kick Object
//
//   Kick an object and change its sequence.
//

void
ObjectKick(register ObjectPtr object)
{
  register Int16 delta;

  object->motion = Motion_Die;
  object->flags |= (flagNotInGame | flagRotating);
  object->flags &= ~flagWaking;

  delta = piperObject.deltaX;

  DeleteObjectEvent(object);

  if (gameVars->phaseType == Phase_Normal) {
    if (gameVars->nextPeriodTime > gameVars->lastKickTime) {
      gameVars->kickCount = 0;
    }
    gameVars->lastKickTime = gameVars->nextPeriodTime + kickDelayTime;

    gameVars->gameScore += (++gameVars->kickCount * pointsKick);

    if (--gameVars->enemyCount == fixEnemyCount) {
      FixLastEnemy();

      PlaySound(Sound_Kick, Priority_Level_3);
    } else {
      if (gameVars->enemyCount == 0) {
        PlaySound(Sound_Kick_Last, Priority_Level_2);
        KillAllObjects(object);
      } else {
        PlaySound(Sound_Kick, Priority_Level_3);
      }
    }
  } else {
    PlaySound(Sound_Kick, Priority_Level_3);
  }

  if (object->type == Object_Turtle) {
    DeleteAnimationObject(object);

    changeSequence(object, turtleRotationSequence, sizeof(turtleRotationSequence));
  } else if (object->type == Object_Crab) {
    changeSequence(object, crabRotationSequence, sizeof(crabRotationSequence));
  } else if (object->type == Object_FighterFly) {
    changeSequence(object, fighterFlyRotationSequence, sizeof(fighterFlyRotationSequence));
  }

  if (gameVars->phaseType == Phase_Normal) {
    register AnimationPtr theAnimate;
    register Int16 yDelta, yPos;

    theAnimate = AddAnimation();
    InitializeAnimation(theAnimate, Animation_Points_800 + gameVars->kickCount - 1, object);

    theAnimate->draw.yPixels = animateHeight;
    theAnimate->flags &= ~flagAnimDirectionX;

    yDelta = kickDelta + (killAdditional * (gameVars->kickCount - 1));
    yPos = -object->yPos;
    if (yDelta < yPos) {
      yDelta = yPos;
    }

    ChangeAnimationPos(theAnimate, 0, yDelta);
  }

  object->deltaX = delta;
}

//
// Kill Piper Character
//
//   Kill and have Maryo go into dying sequence.
//

void
ObjectDie(register ObjectPtr object,
          register ObjectPtr object2)
{
  register UInt16 type;

  gameVars->slideNum = 0;
  gameVars->tileFriction = defaultFriction;

  flipObject.flags |= flagNotInGame;
  if (gameVars->powNum != 0) {
    register TilePtr tilePtr;

    gameVars->powNum = 0;
    gameVars->flipFloorTile = PChar_SPACE;

    tilePtr = gameVars->charArray + tileYMin;

    type = charUpdate - tileYMin;
    do {
      *tilePtr++ |= dirtyChar;
    } while (--type);
  }

  type = object2->type;
  if ((type == Object_FireBall) || (type == Object_PaintBall)) {
    ChangeObjectSequence(&piperObject, piperSplatSequence, sizeof(piperSplatSequence), Motion_Surprise);
  } else {
    ChangeObjectSequence(&piperObject, piperBiteSequence, sizeof(piperBiteSequence), Motion_Surprise);
  }
  object->deltaX = 0;
  object->deltaY = 0;
  object->flags |= flagNotInGame;
}

//
// Object Enter Piper
//
//   Handle the entering of an object onto the playing field.
//
//   Return true if the object actual enter the field.
//

UInt16
ObjectEnterPipe(register ObjectPtr object)
{
  register UInt16 whichPipe, level;
  register AnimationPtr theAnimate;

  level = object->level;
  if (level != Level_Inanimate) {
    if ((gameVars->phaseType != Phase_Instructions) || (level != Level_Bonus)) {
      if (object->deltaX > 0) {
        whichPipe = leftPipe;
      } else {
        if (gameVars->phaseType == Phase_Instructions) {
          whichPipe = instructPipe;
        } else {
          whichPipe = rightPipe;
        }
      }

      if ((gameVars->enemyCount > 1) || (level == Level_Bonus)) {
        if (gameVars->lastPipeTime[whichPipe] > gameVars->nextPeriodTime) {
          return 0;
        }
      }

      if (gameVars->phaseType == Phase_Instructions) {
        gameVars->lastPipeTime[whichPipe] = gameVars->nextPeriodTime + (UInt32)(delayInstructTime);
        object->yPos = objectEnterY3;
      } else {
        gameVars->lastPipeTime[whichPipe] = gameVars->nextPeriodTime + (UInt32)(delayTime);
        object->yPos = objectEnterY1;
      }

      if (level == Level_Bonus) {
        if ((piperObject.draw).xStart >= (screenWidth / 2)) {
          object->deltaX = (Int8)(coinSpeedX);
        } else {
          object->deltaX = -(Int8)(coinSpeedX);
        }
      }

      if (object->deltaX > 0) {
        object->xPos = ((objectEnterX1 - objectWidth + 1) * speedFactor);
        object->flags &= ~flagDirectionX;
      } else {
        if (gameVars->phaseType == Phase_Instructions) {
          object->xPos = ((objectEnterX3 - 1) * speedFactor);
        } else {
          object->xPos = ((objectEnterX2 - 1) * speedFactor);
        }
        object->flags |= flagDirectionX;
      }

      theAnimate = AddAnimation();
      InitializeAnimation(theAnimate, Animation_Pipe_Bend, object);

      if (gameVars->phaseType == Phase_Normal) {
        if (object->type == Object_Turtle) {
          PlaySound(Sound_Turtle_Appear, Priority_Level_2);
        } else if (object->type == Object_Crab) {
          PlaySound(Sound_Crab_Appear, Priority_Level_2);
        } else if (object->type == Object_FighterFly) {
          PlaySound(Sound_FighterFly_Appear, Priority_Level_2);
        } else if (object->type == Object_IceMachine) {
          PlaySound(Sound_IceMachine_Appear, Priority_Level_2);
        }
      }

      theAnimate->draw.yStart = object->yPos;
      theAnimate->draw.xSkip = 0;
      theAnimate->draw.ySkip = 0;
      theAnimate->draw.xPixels = animateWidth;
      theAnimate->draw.yPixels = animateHeight;

      if (object->deltaX > 0) {
        theAnimate->flags &= ~flagDirectionX;
        theAnimate->draw.xStart = (object->xPos / speedFactor) - 1;
      } else {
        theAnimate->flags |= flagDirectionX;
        theAnimate->draw.xStart = (object->xPos / speedFactor) + 1;
      }
    }
  } else {
    if (object->type == Object_FireBall) {
      if ((piperObject.draw).xStart >= (screenWidth / 2)) {
        object->xPos = (0 * speedFactor);
        object->deltaX = fireBallSpeedX;
        object->flags &= ~flagDirectionX;
      } else {
        object->xPos = ((screenWidth - objectWidth) * speedFactor);
        object->deltaX = -fireBallSpeedX;
        object->flags |= flagDirectionX;
      }

      for (whichPipe = 0; whichPipe < numFireBallStarts; whichPipe++) {
        if ((piperObject.draw).yStart <= fireBallRanges[whichPipe]) {
          object->yPos = fireBallStarts[whichPipe];
          break;
        }
      }
    }
  }

  level = object->type;
  if ((level == Object_FireBall) || (level == Object_PaintBall)) {
    object->motion = Motion_Appear;
    object->flags &= ~(flagWaitToEnter | flagInvisible);

    if (level == Object_FireBall) {
      PlaySound(Sound_FireBall, Priority_Level_2);

      changeSequence(object, fireBallAppearSequence, sizeof(fireBallAppearSequence));
    } else {
      PlaySound(Sound_PaintBall_Appear, Priority_Level_2);

      changeSequence(object, paintBallAppearSequence, sizeof(paintBallAppearSequence));
    }

    AddEvent(Event_Appear, appearTime, (void *)(object));
  } else {
    object->flags &= ~(flagWaitToEnter | flagInvisible | flagNotInGame);
  }
  object->xPrev = object->xPos;

  return 1;
}

//
// Object Exit Pipe
//
//   Handle the exiting of an object from the playing field.  Add it to the entrance
//     list if an enemy exits thru a pipe.
//

void
ObjectExitPipe(register ObjectPtr object)
{
  if ((object->flags & flagOnceThru) != 0) {
    if (object->type == Object_IceMachine) {
      ObjectReincarnate(object);
    } else {
      ObjectRemove(object);
    }
  } else {
    object->deltaX = -object->deltaX;
    object->flags |= (flagInvisible | flagNotInGame | flagWaitToEnter);

    AddEvent(Event_Entrance, pipeTime, (void *)(object));
  }
}

//
// Draw Text
//
//   Translate an ASCII string into the character tile array format and copy
//     the string into the array.
//

void
DrawText(register UInt8 * text,
         register UInt16 x,
         register UInt16 y,
         register UInt16 direct)
{
  register UInt16 ch;
  register UInt16 i;
  register TilePtr tilePtr;

  tilePtr = &gameVars->charArray[x + (y * charXNum)];

  ch = *text++;
  i = 0;

  while (ch != '\0') {
    if ((ch >= 'A') && (ch <= 'Z')) {
      ch = ch - 'A' + PChar_A;
    } else if ((ch >= '0') && (ch <= '9')) {
      ch = ch - '0' + PChar_ZERO;
    } else if (ch == ' ') {
      ch = PChar_SPACE;
    } else if (ch == '-') {
      ch = PChar_DASH;
    } else if (ch == '!') {
      ch = PChar_EXCLAIM;
    } else if (ch == '?') {
      ch = PChar_QUESTION;
    } else if (ch == '\'') {
      ch = PChar_QUOTE;
    } else if (ch == '@') {
      ch = PChar_AT;
    } else if (ch == '(') {
      ch = PChar_LEFT_PAREN;
    } else if (ch == ')') {
      ch = PChar_RIGHT_PAREN;
    } else if (ch == '_') {
      ch = PChar_UNDERSCORE;
    } else {
      ch = PChar_PERIOD;
    }

    if (direct == 0) {
      *tilePtr++ = ch | dirtyChar;
    } else {
      DrawChar((x + i) * charXSize, y * charYSize, ch, charYSize);
    }
    i++;
    ch = *text++;
  }

#if 0
  UpdateRect(x * charXSize, y * charYSize, i * charXSize, charYSize);
#else
  UpdateRect(y * charYSize, charYSize);
#endif
}

//
// Add Sprite to List
//
//   Add a sprite to the sprite list.
//

void
AddSprite(register ObjectPtr object)
{
  if ((gameVars->spriteCount < maxNumObjects) && (object != (ObjectPtr) (NULL))) {
    register ObjElePtr listPtr;

    listPtr = &gameVars->spriteList[gameVars->spriteCount++];
    listPtr->object = object;

    if (gameVars->spriteHead == (ObjElePtr) (NULL)) {
      (gameVars->spriteHead = listPtr)->next = (ObjElePtr) (NULL);
    } else {
      register ObjElePtr objPtr, curPtr;
      register UInt16 level;

      level = (listPtr->object)->level;
      objPtr = gameVars->spriteHead;
      if (level >= (objPtr->object)->level) {
        do {
          objPtr = (curPtr = objPtr)->next;
        } while ((objPtr != (ObjElePtr) (NULL)) && (level >= (objPtr->object)->level));

        listPtr->next = curPtr->next;
        curPtr->next = listPtr;
      } else {
        (gameVars->spriteHead = listPtr)->next = objPtr;
      }
    }
  }
}

//
// Delete Sprite From List
//
//   Remove a sprite from the sprite list.
//
//   Return true if sprite is actually removed.
//

UInt16
DeleteSprite(register ObjectPtr object)
{
  if ((gameVars->spriteCount != 0) && (object != (ObjectPtr) (NULL))) {
    register ObjElePtr objPtr;

#if 0
    UpdateRect(object->draw.xStart, object->draw.yStart,
               object->draw.xPixels, object->draw.yPixels);
#else
    UpdateRect(object->draw.yStart, object->draw.yPixels);
#endif

    gameVars->spriteCount--;

    objPtr = gameVars->spriteHead;
    if (objPtr->object != object) {
      register ObjElePtr curPtr;

      do {
        objPtr = (curPtr = objPtr)->next;
      } while (objPtr->object != object);

      curPtr->next = objPtr->next;
    } else {
      gameVars->spriteHead = objPtr->next;
    }

    return 1;
  }

  return 0;
}

// Destroy Phase
void
DestroyPhase(void)
{
  gameVars->enemyCount = 0;
  if ((gameVars->gameState & gameDestroy) == 0) {
    gameVars->gameState |= gameDestroy;

    if ((gameVars->gameState & gameKill) == 0) {
      KillAllObjects((ObjectPtr) (NULL));
    }
  }
}

//
// Erase Text
//
//   Erase the text from the character tile array.
//

void
EraseText(register UInt16 len,
          register UInt16 x,
          register UInt16 y,
          register UInt16 direct)
{
  register TilePtr pos;

  if (len) {
#if 0
    UpdateRect(x * charXSize, y * charYSize, len * charXSize, charYSize);
#else
    UpdateRect(y * charYSize, charYSize);
#endif

    pos = &gameVars->charArray[x + (y * charXNum)];

    do {
      if (direct == 0) {
        *pos++ = PChar_SPACE | dirtyChar;
        if (gameVars->powNum != 0) {
          *(pos - charXNum - 1) |= dirtyChar;
        }
      } else {
        *pos++ |= dirtyChar;
      }
    } while (--len);
  }
}

//
// Remove Text Event
//
//   Remove a text event of the the event list.
//


void
RemoveTextEvent(register TextEventPtr theTextEvent)
{
  register UInt16 len, i;
  register GameEventPtr theEvent;

  i = gameVars->eventGameCount;
  while (i--) {
    theEvent = &gameVars->eventList[i];

    if (theEvent->event == theTextEvent) {
      len = StrLen((const Char *)(theTextEvent->text));
      EraseText(len, theTextEvent->x, theTextEvent->y, 0);

      DeleteEvent(theEvent);
    }
  }
}

//
// Display Phase
//
//   Update the phase indicator to the new phase and erase the old one.
//

void
DisplayPhase(register UInt16 thePhase)
{
  register TilePtr pos;
  register UInt16 width, theWidth;
  register TextEventPtr theEvent;

  theEvent = &gameVars->textEvents[0];

  StrPrintF(gameVars->gameStr[1], phaseText, thePhase);
  InitializeTextEvent(theEvent, gameVars->gameStr[1], phaseStartX,
                      phaseStartY, textTime, (textNormal | textDraw));

  AddEvent(Event_Text, 0, (void *)(theEvent));

  if (thePhase == 1) {
    theEvent = &gameVars->textEvents[1];

    StrPrintF(gameVars->gameStr[2], initial1Text);
    InitializeTextEvent(theEvent, gameVars->gameStr[2], initialStartX,
                        initialStartY, textTime, (textNormal | textDraw));

    AddEvent(Event_Text, 0, (void *)(theEvent));

    theEvent = &gameVars->textEvents[2];

    StrPrintF(gameVars->gameStr[3], initial2Text);
    InitializeTextEvent(theEvent, gameVars->gameStr[3], initialStartX,
                        initialStartY + initialDeltaY, textTime,
                        (textNormal | textDraw));

    AddEvent(Event_Text, 0, (void *)(theEvent));
  }

  pos = &gameVars->charArray[(phaseX + (phaseY * charXNum))];
  width = 0;

  if (thePhase < phaseFive) {
    *pos = PChar_PHASE1 | dirtyChar;
    *(pos++ + charXNum) = PChar_PHASE3 | dirtyChar;
    *pos = PChar_PHASE2 | dirtyChar;
    *(pos++ + charXNum) = PChar_PHASE4 | dirtyChar;

    width += phaseWidth;
  } else {
    while (thePhase >= phaseFifty) {
      thePhase -= phaseFifty;

      *pos = PChar_PHASE_FIFTY1 | dirtyChar;
      *(pos++ + charXNum) = PChar_PHASE_FIFTY3 | dirtyChar;
      *pos = PChar_PHASE_TEN2 | dirtyChar;
      *(pos++ + charXNum) = PChar_PHASE_TEN4 | dirtyChar;

      width += phaseWidth;
    }

    while (thePhase >= phaseTwenty) {
      thePhase -= phaseTwenty;

      *pos = PChar_PHASE_TWENTY1 | dirtyChar;
      *(pos++ + charXNum) = PChar_PHASE_TWENTY3 | dirtyChar;
      *pos = PChar_PHASE_TEN2 | dirtyChar;
      *(pos++ + charXNum) = PChar_PHASE_TEN4 | dirtyChar;

      width += phaseWidth;
    }

    while (thePhase >= phaseTen) {
      thePhase -= phaseTen;

      *pos = PChar_PHASE_TEN1 | dirtyChar;
      *(pos++ + charXNum) = PChar_PHASE_TEN3 | dirtyChar;
      *pos = PChar_PHASE_TEN2 | dirtyChar;
      *(pos++ + charXNum) = PChar_PHASE_TEN4 | dirtyChar;

      width += phaseWidth;
    }

    while (thePhase >= phaseFive) {
      thePhase -= phaseFive;

      *pos = PChar_PHASE_FIVE1 | dirtyChar;
      *(pos++ + charXNum) = PChar_PHASE_FIVE3 | dirtyChar;
      *pos = PChar_PHASE_FIVE2 | dirtyChar;
      *(pos++ + charXNum) = PChar_PHASE_FIVE4 | dirtyChar;

      width += phaseWidth;
    }
  }

  while (thePhase--) {
    *pos = PChar_PHASE_TRAP1 | dirtyChar;
    *(pos++ + charXNum) = PChar_PHASE_TRAP2 | dirtyChar;

    width++;
  }

  theWidth = width;
  if (width < gameVars->prevPhaseWidth) {
    thePhase = gameVars->prevPhaseWidth - width;
    width += thePhase;

    do {
      *pos = PChar_PIPE_BRICK1 | dirtyChar;
      *(pos++ + charXNum) = PChar_PIPE_BRICK6 | dirtyChar;
    } while (--thePhase);
  }
  gameVars->prevPhaseWidth = theWidth;

#if 0
  UpdateRect(phaseX * charXSize, phaseY * charYSize, width * charXSize,
             phaseHeight * charYSize);
#else
  UpdateRect(phaseY * charYSize, phaseHeight * charYSize);
#endif
}

//
// Draw Tiles
//
//   Fill a specific length of the character tile array with a certain tile.
//

void
DrawTiles(register Tile tile,
          register UInt16 x,
          register UInt16 y,
          register UInt16 len)
{
  register UInt16 i;
  register TilePtr tilePtr;

  tilePtr = &gameVars->charArray[x + (y * charXNum)];
  tile |= dirtyChar;

  i = len;
  do {
    *tilePtr++ = tile;
  } while (--i);
}

//
// Change Floor Tiles
//
//   Change all floor character array tiles to the appropiate new floor type.
//

void
ChangeFloorTiles(register UInt16 thePhase,
                 register Tile overrideTile)
{
  register UInt16 i;
  register Tile tile, tileChange;
  register TilePtr tilePtr1, tilePtr2;

  if ((thePhase >= phaseTurtleStart) && (thePhase <= phaseTurtleEnd)) {
    tileChange = PChar_FLOOR1 | dirtyChar;
  } else if ((thePhase >= phaseCrabStart) && (thePhase <= phaseCrabEnd)) {
    tileChange = PChar_FLOOR2 | dirtyChar;
  } else if ((thePhase >= phaseFighterFlyStart) && (thePhase <= phaseFighterFlyEnd)) {
    tileChange = PChar_FLOOR3 | dirtyChar;
  } else if (overrideTile != PChar_SPACE) {
    tileChange = overrideTile | dirtyChar;
  } else {
    tileChange = PChar_FLOOR4 | dirtyChar;
  }

  tilePtr1 = initialScreen + tileYMin;
  tilePtr2 = gameVars->charArray + tileYMin;

  i = charUpdate - tileYMin;
  do {
    tile = *tilePtr1++;
    if ((tile >= FLOOR_START) && (tile <= FLOOR_END1)) {
      *tilePtr2 = tileChange;
    }
    tilePtr2++;
  } while (--i);
}

//
// Clear All Tiles
//
//   Fill the character tile array with spaces.
//

void
ClearTiles(void)
{
  register UInt16 i;
  register TilePtr tilePtr;

  tilePtr = gameVars->charArray + tileYMin;

  i = charUpdate - tileYMin;
  do {
    *tilePtr++ = PChar_SPACE | dirtyChar;
  } while (--i);
}

//
// Create New Object
//
//   Create a new object from the list.
//
//   Return a pointer to the new object.
//

ObjectPtr
NewObject(void)
{
  register UInt16 objNum;

  if ((objNum = gameVars->totalObjects) < maxNumObjects) {
    gameVars->totalObjects++;
  }

  return &gameVars->objectList[objNum];
}

//
// Instruction Phase
//
//   Update the screen for the current instruction phase.  Initialize automatic handlers of
//     enemies and Maryo.
//

void
InstructionScreens(register UInt16 which)
{
  register UInt16 objectType;
  register ObjectPtr object;
  register UInt8 *theKeyDelay, *theKeyList;
  register Tile floorTile;
  register TextEventPtr theEvent;
  register AnimationPtr theAnimate;

  ClearTiles();

  objectType = Object_None;
  floorTile = PChar_SPACE;
  theKeyDelay = instruction1KeyDelayList;
  theKeyList = instruction1KeyList;

  gameVars->lastInstructionTime = 0;

  piperObject.flags &= ~(flagInvisible | flagNotInGame);

  switch (which) {
    case Instruction_Turtle:
      theEvent = &gameVars->textEvents[0];

      StrPrintF(gameVars->gameStr[1], instructDemoText);
      InitializeTextEvent(theEvent, gameVars->gameStr[1],
                          instructDemoStartX, instructDemoStartY,
                          blinkTime, (textBlink | textDraw));

      AddEvent(Event_Text, 0, (void *)(theEvent));

      gameVars->instructionEvent = &instruction1EventList[0];

      objectType = Object_Turtle;
      floorTile = PChar_FLOOR1;
      theKeyDelay = instruction1KeyDelayList;
      theKeyList = instruction1KeyList;
      PlaySound(Sound_Turtle_Start, Priority_Level_0);

      DrawText(instructTurtleText, instructTurtleStartX, instructTurtleStartY, 0);

      object = NewObject();
      InitializeObject(object, Object_Coin);

      object->flags |= (flagWaitToEnter | flagInvisible | flagNotInGame | flagDirectionX);
      object->xPos = (instruction1CoinStartX * speedFactor);
      object->yPos = instruction1CoinStartY;
      object->deltaX = -(Int8) (coinSpeedX);

      AddEvent(Event_Entrance, instruction1CoinDelay, (void *)(object));
      AddEvent(Event_Animation, instruction1CoinDelay, (void *)(NULL));
      break;

    case Instruction_Crab:
      gameVars->instructionEvent = &instruction2EventList[0];

      objectType = Object_Crab;
      floorTile = PChar_FLOOR2;
      theKeyDelay = instruction2KeyDelayList;
      theKeyList = instruction1KeyList;
      PlaySound(Sound_Crab_Start, Priority_Level_0);

      DrawText(instructCrabText, instructCrabStartX, instructCrabStartY, 0);
      break;

    case Instruction_FighterFly:
      gameVars->instructionEvent = &instruction3EventList[0];

      objectType = Object_FighterFly;
      floorTile = PChar_FLOOR3;
      theKeyDelay = instruction3KeyDelayList;
      theKeyList = instruction1KeyList;
      PlaySound(Sound_FighterFly_Start, Priority_Level_0);

      DrawText(instructFFlyText, instructFFlyStartX, instructFFlyStartY, 0);
      break;

    case Instruction_IceMachine:
      objectType = Object_IceMachine;
      floorTile = PChar_FLOOR4;
      gameVars->enemyCount = 1;
      theKeyDelay = instruction4KeyDelayList;
      theKeyList = instruction2KeyList;
      PlaySound(Sound_FighterFly_Start, Priority_Level_0);

      gameVars->iceMachineFill[numIceMachineStops] = 0;

      DrawText(instructIceText, instructIceStartX, instructIceStartY, 0);
      break;
  }

  object = NewObject();
  InitializeObject(object, objectType);
  object->xPos = (instructObject1X * speedFactor);
  object->yPos = instructObject1Y;
  object->deltaX = 0;
  object->motion = Motion_Spin;

  object = NewObject();
  InitializeObject(object, objectType);
  object->xPos = (instructObject2X * speedFactor);
  object->yPos = instructObject2Y;
  object->motion = Motion_Spin;
  object->flags |= flagDirectionX;
  object->deltaX = 0;
  if (which != Instruction_IceMachine) {
    object->draw.seqNum -= instruct1SeqNumDelta;
  } else {
    object->draw.seqNum -= instruct2SeqNumDelta;
  }
  ChangeEnemySequence(object);

  object = NewObject();
  InitializeObject(object, objectType);

  if (which != Instruction_IceMachine) {
    AddEvent(Event_Instructions, 0, (void *)(gameVars->instructionEvent));

    object->xPos = (instruct1ObjectMoveX * speedFactor);
    object->yPos = instruct1ObjectMoveY;

    DrawTiles(floorTile, instruct1Tile1X, instruct1Tile1Y, instruct1Tile1Len);
    DrawTiles(floorTile, instruct1Tile2X, instruct1Tile2Y, instruct1Tile2Len);

    piperObject.xPos = (instruct1PiperStartX * speedFactor);
    piperObject.yPos = instruct1PiperStartY;
  } else {
    register UInt16 i, j, outLoc, inLoc, base;

    object->xPos = (instruct2ObjectMoveX * speedFactor);
    object->yPos = instruct2ObjectMoveY;
    object->motion = Motion_Fall;

    object->flags |= flagDirectionX;
    object->deltaX = -object->deltaX;

    AddEvent(Event_Entrance, startTime, (void *)(object));

    object = NewObject();
    InitializeObject(object, objectType);

    object->xPos = (instruct2ObjectMoveX * speedFactor);
    object->yPos = instruct2ObjectMoveY;
    object->motion = Motion_Fall;

    AddEvent(Event_Entrance, startTime + delayInstructTime, (void *)(object));

    DrawTiles(floorTile, instruct2Tile1X, instruct2Tile1Y, instruct2Tile1Len);

    base = iceInstructTileStartY * charXNum;
    for (i = outLoc = 0; i < iceInstructTileHeight; i++) {
      inLoc = base;
      for (j = 0; j < iceInstructTileWidth; j++) {
        gameVars->charArray[inLoc] = iceInstruction1[outLoc] | dirtyChar;
        gameVars->charArray[inLoc++ + charXNum - iceInstructTileWidth] =
          iceInstruction2[outLoc++] | dirtyChar;
      }
      base += charXNum;
    }

    for (i = 0; i < instruct2Tile2Height; i++) {
      DrawTiles(PChar_PIPE_BRICK1 + i, instruct2Tile2X,
                instruct2Tile2Y + i, instruct2Tile2Len);
    }
    DrawTiles(PChar_WATER, instruct2Tile2X,
              instruct2Tile2Y + instruct2Tile2Height, instruct2Tile2Len);

    piperObject.xPos = (instruct2PiperStartX * speedFactor);
    piperObject.yPos = instruct2PiperStartY;

    for (i = 0; i < instruct2Anim1Num; i++) {
      theAnimate = AddAnimation();
      InitializeAnimation(theAnimate, Animation_Waterfall_1 + (i & 1),
                          (ObjectPtr) (NULL));

      theAnimate->draw.xStart =
        instruct2Anim1X1 + ((i / 2) * instruct2Anim1DeltaX);
      theAnimate->draw.yStart =
        instruct2Anim1Y1 + ((i & 1) * instruct2Anim1DeltaY);
      theAnimate->flags |= flagAnimLooping;
    }
  }

  object->flags |= flagDirectionX;
  object->deltaX = -object->deltaX;

  gameVars->currentKey = gameVars->currentCount = 0;
  InitializeKeyEvent(&gameVars->keyEvent, theKeyList, theKeyDelay);
  AddEvent(Event_Key, 0, (void *)(&gameVars->keyEvent));
}

//
// Initialize Bonus Coin
//
//   Initialize a new coin for the bonus phase.
//

void
InitializeBonusCoin(register UInt16 x,
                    register UInt16 y)
{
  register ObjectPtr object;

  object = NewObject();
  InitializeObject(object, Object_Coin);

  object->xPos = (x * speedFactor);
  object->yPos = y;
  object->motion = Motion_Spin;
  object->draw.seqNum = gameVars->totalObjects - (numSetObjects + 1);

  if (object->draw.seqNum > 0) {
    object->prevSeqNum = object->draw.seqNum - 1;
  } else if (object->size > 0) {
    object->prevSeqNum = object->size - 1;
  } else {
    object->prevSeqNum = 0;
  }
}

// Display High Score Time
void
DisplayHighScoreTime(register UInt16 timeLeft)
{
  StrPrintF(gameVars->gameStr[3], highScoreTimeText, timeLeft / 100);
  DrawText(gameVars->gameStr[3], highScoreTimeTextX, highScoreTimeTextY, 0);
}

//
// Display Time
//
//  Display timer value (zero padded two integer digits and one tenth digit) and on
//    initialization draw the timer box.
//

void
DisplayTime(register UInt16 timeLeft,
            register UInt16 init)
{
  register TilePtr tilePtr;

  timeLeft /= 10;

  StrPrintF(gameVars->gameStr[3], bonusTimeText, timeLeft / 10, timeLeft % 10);
  DrawText(gameVars->gameStr[3], bonusTimeStartX, bonusTimeStartY, 0);

  if (init != 0) {
    register UInt16 i;

    tilePtr = &gameVars->charArray[bonusTimeStartX + (bonusTimeStartY * charXNum)];
    for (i = 0; i < bonusWidth; i++) {
      *(tilePtr - charXNum) = *(tilePtr + (bonusHeight * charXNum)) = PChar_HORIZONTAL | dirtyChar;
      tilePtr++;
    }

    *tilePtr = *(tilePtr - bonusWidth - 1) = PChar_VERTICAL | dirtyChar;

    *(tilePtr - charXNum - bonusWidth - 1) = PChar_DIAGONAL1 | dirtyChar;
    *(tilePtr + charXNum - bonusWidth - 1) = PChar_DIAGONAL2 | dirtyChar;
    *(tilePtr - charXNum) = PChar_DIAGONAL3 | dirtyChar;
    *(tilePtr + charXNum) = PChar_DIAGONAL4 | dirtyChar;
  }
}

//
// Display Lives
//
//   Copy the number of lives pips into the character tile array.
//

void
DisplayLives(void)
{
  register TilePtr pos;
  register UInt16 i, lives, y;

  if (gameVars->numLives) {
    lives = gameVars->numLives - 1;
    pos = &gameVars->charArray[(lifeX + ((y = lifeY) * charXNum))];

    if (lives) {
      i = 0;
      do {
        *pos = PChar_LIFE_CHAR | dirtyChar;
        pos += lifeDeltaX;

        if (++i == lifeXSize) {
          i = 0;
          pos = &gameVars->charArray[(lifeX + (++y * charXNum))];
        }
      } while (--lives);
    }

    *pos = PChar_SPACE | dirtyChar;
    if (gameVars->powNum != 0) {
      *(pos - charXNum) |= dirtyChar;
    }

#if 0
    UpdateRect(lifeX * charXSize, lifeY * charYSize,
               (lifeXSize * lifeDeltaX) * charXSize,
               (y - lifeY + 1) * charYSize);
#else
    UpdateRect(lifeY * charYSize, (y - lifeY + 1) * charYSize);
#endif
  }
}

//
// Display Score
//
//   Copy and convert the score into the character tile array.
//

void
DisplayScore(register UInt32 score,
             register UInt16 x,
             register UInt16 y,
             register UInt16 minDigits,
             register UInt8 whiteSpace)
{
  register TilePtr pos;
  register Int16   i;
  register UInt32 digit;

  whiteSpace |= dirtyChar;

  pos = &gameVars->charArray[(x + (y * charXNum)) + (i = scoreXSize)];

  do {
    digit = (score / 10);
    *--pos = ((Tile) (score - ((digit << 3) + (digit << 1))) + PChar_ZERO) | dirtyChar;
    score = digit;
    i--;
  } while (((i > minDigits) || (score != 0)) && (i != 0));

  while (i--) {
    *--pos = whiteSpace;
  }

#if 0
  UpdateRect(x * charXSize, y * charYSize, scoreXSize * charXSize, charYSize);
#else
  UpdateRect(y * charYSize, charYSize);
#endif
}

//
// Update Game Score & Top Score
//
//   Update the number of lives indicate and current score.
//

void
UpdateScore(void)
{
  if (gameVars->lastGameScore != gameVars->gameScore) {
    if ((gameVars->gameScore >= gameVars->initExtraLife) && (gameVars->lastGameScore < gameVars->initExtraLife)) {
      PlaySound(Sound_Extra_Life, Priority_Level_2);
      gameVars->numLives++;
    }

    gameVars->lastGameScore = gameVars->gameScore;
//    DisplayScore(gameVars->gameScore, scoreX, scoreY, scoreMinDig, PChar_DOT);
    DisplayScore(gameVars->gameScore, scoreX, scoreY, scoreMinDig, PChar_SPACE);
  }
  if (gameVars->lastNumLives != gameVars->numLives) {
    gameVars->lastNumLives = gameVars->numLives;
    DisplayLives();
  }
}

//
// Total Screens
//
//   Update the screen for the bonus totals (number of coins collected).
//

void
TotalScreens(register UInt16 which)
{
  register UInt16 i, x, y;
  register ObjectPtr object;

  ClearTiles();

  DrawText(maryoBros1Str, maryoBros1StartX, maryoBros1StartY, 0);

  piperObject.xPos = (total1PiperStartX * speedFactor);
  piperObject.yPos = total1PiperStartY;

  x = (totalCoinStartX * speedFactor);
  y = totalCoinStartY;

  for (i = 0; i < gameVars->totalCoins; i++) {
    object = NewObject();
    InitializeObject(object, Object_Coin);

    object->deltaX = 0;
    object->motion = Motion_Spin;
    object->draw.seqNum = totalSeqNum;
    object->flags |= (flagInvisible | flagNotInGame);
    object->xPos = x;
    object->yPos = y;

    x += (totalCoinDeltaX * speedFactor);
    if (i == (totalCountWidth - 1)) {
      x = (totalCoinStartX * speedFactor);
      y += totalCoinDeltaY;
    }
  }

  gameVars->lastCoin = 0;
  AddEvent(Event_Coin, 0, (void *)(NULL));
}

//
// Other Screen
//
//   Update the screen for the other text intermission.
//

void
OtherScreen(void)
{
  ClearTiles();

  piperObject.flags |= (flagInvisible | flagNotInGame);

  DrawText(misc1Str, misc1StartX, misc1StartY, 0);
  DrawText(misc2Str, misc2StartX, misc2StartY, 0);

  AddEvent(Event_EndPhase, miscTime, (void *)(NULL));
}

//
// Draw Line
//
//   Draw a vertical line, with vertical tile characters, used in high score table.
//

void
DrawLine(register UInt16 x,
         register UInt16 y,
         register UInt16 len)
{
  register TilePtr pos;

  if (len) {
#if 0
    UpdateRect(x * charXSize, y * charYSize, len * charXSize, charYSize);
#else
    UpdateRect(y * charYSize, charYSize);
#endif

    pos = &gameVars->charArray[x + (y * charXNum)];

    do {
      *pos++ = PChar_HORIZONTAL | dirtyChar;
    } while (--len);
  }
}

//
// Display High Scores Screen
//
//   Update the screen for the list of high scores.
//

void
DisplayHighScores(register UInt16 scoreNum,
                  register UInt16 init)
{
  register UInt16 i, y;

  if (gameVars->phaseType == Phase_DisplayHighScores) {
    gameVars->gameDifficulty = gameVars->initDifficulty;
  }

  if (init != 0) {
    ClearTiles();
    ClearScreen();

    piperObject.flags = (flagInvisible | flagNotInGame);

    DrawLine(highScoreLineX,
             (gameVars->phaseType !=
              Phase_DisplayHighScores) ? highScoreLineY : (highScoreLineY - displayHighScoreDeltaY),
             highScoreLineLen);

    DrawText(highScoreRankStr, highScoreRankX - 1,
             (gameVars->phaseType !=
              Phase_DisplayHighScores) ? highScoreRankY : (highScoreRankY - displayHighScoreDeltaY),
             0);
    DrawText(highScoreScoreStr, highScoreScoreX,
             (gameVars->phaseType !=
              Phase_DisplayHighScores) ? highScoreScoreY : (highScoreScoreY - displayHighScoreDeltaY),
             0);
    DrawText(highScoreNameStr, highScoreNameX,
             (gameVars->phaseType !=
              Phase_DisplayHighScores) ? highScoreNameY : (highScoreNameY - displayHighScoreDeltaY),
             0);
  }

  y =
    (gameVars->phaseType !=
     Phase_DisplayHighScores) ? highScoreRankY : (highScoreRankY - displayHighScoreDeltaY);
  for (i = 0; i < numHighScore; i++) {
    y += highScoreDeltaLine;

    DrawText(highScoreRankingStr[i], highScoreRankX, y, 0);

    if (scoreNum != i) {
      DrawText(gameVars->highScoreStrTable[gameVars->gameDifficulty][i], highScoreNameX + 1, y, 0);
    } else {
      gameVars->highScoreTable[gameVars->gameDifficulty][i] = gameVars->gameScore;
    }

    DisplayScore(gameVars->highScoreTable[gameVars->gameDifficulty][i], highScoreScoreX, y,
                 scoreXSize, PChar_ZERO);
  }

  if (gameVars->phaseType == Phase_DisplayHighScores) {
    register ObjectPtr object;
    register Tile floorTile;

    floorTile =
      PChar_FLOOR1 + (SysRandom(0) & (PChar_FLOOR_ICE - PChar_FLOOR1));
    DrawTiles(floorTile, highScoreTile1X, highScoreTile1Y, highScoreTile1Len);
    DrawTiles(floorTile, highScoreTile2X, highScoreTile2Y, highScoreTile2Len);
    DrawTiles(floorTile, highScoreTile3X, highScoreTile3Y, highScoreTile3Len);

    i = (SysRandom(0) & (Object_IceMachine - Object_Turtle));

    object = NewObject();
    InitializeObject(object, Object_Turtle + i);
    object->xPos = (highScoreObject1X * speedFactor);
    object->yPos = highScoreObject1Y;

    object = NewObject();
    InitializeObject(object, Object_Turtle + ((i + 1) & (Object_IceMachine - Object_Turtle)));
    object->xPos = (highScoreObject2X * speedFactor);
    object->yPos = highScoreObject2Y;
    object->flags |= flagDirectionX;
    object->deltaX = -object->deltaX;

    object = NewObject();
    InitializeObject(object, Object_Turtle + ((i + 2) & (Object_IceMachine - Object_Turtle)));
    object->xPos = (highScoreObject3X * speedFactor);
    object->yPos = highScoreObject3Y;
    object->flags |= flagDirectionX;
    object->deltaX = -object->deltaX;

    object = NewObject();
    InitializeObject(object, Object_Turtle + ((i + 3) & (Object_IceMachine - Object_Turtle)));
    object->xPos = (highScoreObject4X * speedFactor);
    object->yPos = highScoreObject4Y;

    AddEvent(Event_EndPhase, displayHighScoreTime, (void *)(NULL));
    gameVars->gamePhase = phaseTitle - 1;
  }
}

//
// Update High Score Name
//
//   Copy the new high score entry string into the character tile array.
//

void
UpdateHighScoreName(void)
{
  register UInt16 i;
  register TilePtr tilePtr;

  tilePtr = &gameVars->charArray[highScoreInitialX + (highScoreInitialY * charXNum)];

  for (i = 0; i < lenHighScore; i++) {
    *tilePtr++ = gameVars->highScoreName[gameVars->gameDifficulty][i] | dirtyChar;
  }
}

//
// Update High Score Selector
//
//   Display or erase the box around the current high score entry character.
//

void
UpdateHighScoreSelector(register UInt16 clear)
{
  register UInt16 x, y;
  register TilePtr tilePtr;

  if (gameVars->totalCoins <= lenHighScore) {
    x = ((gameVars->lastCoin % highScoreCharWidth) * highScoreCharDeltaX) + highScoreCharX;
    y = ((gameVars->lastCoin / highScoreCharWidth) * highScoreCharDeltaY) + highScoreCharY;

    tilePtr = &gameVars->charArray[x + (y * charXNum)];

    if (clear == 0) {
      *(tilePtr - 1) = *(tilePtr + 1) = PChar_VERTICAL | dirtyChar;
      *(tilePtr - charXNum) = *(tilePtr + charXNum) =
        PChar_HORIZONTAL | dirtyChar;

      *(tilePtr - charXNum - 1) = PChar_DIAGONAL1 | dirtyChar;
      *(tilePtr + charXNum - 1) = PChar_DIAGONAL2 | dirtyChar;
      *(tilePtr - charXNum + 1) = PChar_DIAGONAL3 | dirtyChar;
      *(tilePtr + charXNum + 1) = PChar_DIAGONAL4 | dirtyChar;
    } else {
      *(tilePtr - 1) = *(tilePtr + 1) = PChar_SPACE | dirtyChar;
      *(tilePtr - charXNum) = *(tilePtr + charXNum) = PChar_SPACE | dirtyChar;

      *(tilePtr - charXNum - 1) = PChar_SPACE | dirtyChar;
      *(tilePtr + charXNum - 1) = PChar_SPACE | dirtyChar;
      *(tilePtr - charXNum + 1) = PChar_SPACE | dirtyChar;
      *(tilePtr + charXNum + 1) = PChar_SPACE | dirtyChar;
    }
  }
}

//
// Register Name
//
//   Convert high score entry screen to storage format and display on screen.
//

void
RegisterName(void)
{
  register UInt16 i;
  register UInt8 initial;

  DeleteEventType(Event_Timer);
  AddEvent(Event_EndPhase, registeredTime, (void *)(NULL));
  gameVars->lastKickTime =
    gameVars->nextPeriodTime + registeredTime + highScoreSelectTime;

  for (i = 0; i < lenHighScore; i++) {
    initial = gameVars->highScoreName[gameVars->gameDifficulty][i];
    if ((initial >= PChar_A) && (initial <= PChar_Z)) {
      initial += 'A' - PChar_A;
    } else if (initial == PChar_PERIOD) {
      initial = '.';
    } else if (initial == PChar_DASH) {
      initial = '-';
    }
    gameVars->highScoreStrTable[gameVars->gameDifficulty][gameVars->whichScore][i] = initial;
  }

  DisplayHighScores(numHighScore, 0);

  DrawText(highScoreRegisteredStr, highScoreRegisterTextX,
           highScoreRegisterTextY, 0);
  gameVars->totalCoins = lenHighScore + 1;
}

//
// Update High Score Char
//
//   Add currently selected character to high score entry string.
//

void
UpdateHighScoreChar(void)
{
  register UInt16 initial;

  if (gameVars->totalCoins <= lenHighScore) {
    initial = highScoreEntryStr[gameVars->lastCoin];

    if (initial == PChar_RUB) {
      if (gameVars->totalCoins != 0) {
        gameVars->highScoreName[gameVars->gameDifficulty][--gameVars->totalCoins] = PChar_SPACE;
      }
    } else if (initial == PChar_END) {
      RegisterName();
    } else {
      if (gameVars->totalCoins == lenHighScore) {
        UpdateHighScoreSelector(1);
        gameVars->lastCoin = highScoreCharLen - 1;
        UpdateHighScoreSelector(0);
      } else {
        gameVars->highScoreName[gameVars->gameDifficulty][gameVars->totalCoins++] = initial;
      }
    }

    UpdateHighScoreName();
  }
}

//
// Update Stylus High Score
//
//   Update your initial entry string for the chosen character for the stylus.
//     Move the box to the current character.
//

void
UpdateStylusHighScore(register Boolean penDown,
                      register Coord x,
                      register Coord y)
{
  register Int16 whichX, whichY;
  register UInt16 pos;

  whichX = ((Int16) (x) - (highScoreCharX * charXSize) + (charXSize / 2)) / charXSize;
  whichY = ((Int16) (y + (topScreen - SCREEN_START)) - (highScoreCharY * charYSize) + (charYSize / 2)) / charYSize;

  if ((whichX >= 0) && (whichX < (highScoreCharDeltaX * highScoreCharWidth)) &&
      (whichY >= 0) && (whichY < (highScoreCharDeltaY * (highScoreCharLen / highScoreCharWidth)))) {
    pos = ((whichY / highScoreCharDeltaY) * highScoreCharWidth) + (whichX / highScoreCharDeltaX);
    if (gameVars->lastCoin != pos) {
      UpdateHighScoreSelector(1);

      gameVars->lastCoin = pos;

      UpdateHighScoreSelector(0);
    }

    if (penDown) {
      UpdateHighScoreChar();
    }

    gameVars->lastKickTime = gameVars->nextPeriodTime + highScoreSelectTime;
  }
}

//
// Update High Score
//
//   Update your initial entry string for the chosen character from the buttons.
//     Move the box to the current character.
//

void
UpdateHighScore(register UInt32 keyState)
{
  if (gameVars->lastKickTime < gameVars->nextPeriodTime) {
    gameVars->lastKickTime = gameVars->nextPeriodTime + highScoreSelectTime;

    if (keyState & (moveLeft | moveRight)) {
      UpdateHighScoreSelector(1);

      if (keyState & moveLeft) {
        if (gameVars->lastCoin-- == 0) {
          gameVars->lastCoin = highScoreCharLen - 1;
        }
      } else {
        if (++gameVars->lastCoin == highScoreCharLen) {
          gameVars->lastCoin = 0;
        }
      }

      UpdateHighScoreSelector(0);
    } else if (keyState & (moveJump1 | moveJump2)) {
      UpdateHighScoreChar();
    }
  }
}

//
// Enter High Scores Screen
//
//   Update the screen for entry of a high score into the table.  Display the high score table,
//     start a timer and determine if the current score is high enough.
//

void
EnterHighScores(void)
{
  register UInt16 i, j;
  register TilePtr tilePtr, tileBasePtr;

  gameVars->gamePhase = phaseTitle - 1;

  for (gameVars->whichScore = 0; gameVars->whichScore < numHighScore; gameVars->whichScore++) {
    if (gameVars->gameScore >= gameVars->highScoreTable[gameVars->gameDifficulty][gameVars->whichScore]) {
      break;
    }
  }

  if ((gameVars->whichScore != numHighScore) && (gameVars->nextTilePhase == (phaseDisplayHighScore - 1))) {
    for (j = numHighScore - 1; j-- > gameVars->whichScore;) {
      gameVars->highScoreTable[gameVars->gameDifficulty][j + 1] = gameVars->highScoreTable[gameVars->gameDifficulty][j];
      for (i = lenHighScore; i--;) {
        gameVars->highScoreStrTable[gameVars->gameDifficulty][j + 1][i] =
          gameVars->highScoreStrTable[gameVars->gameDifficulty][j][i];
      }
    }

    DisplayHighScores(gameVars->whichScore, 1);

    DrawText(highScoreRegisterStr, highScoreRegisterX, highScoreRegisterY, 0);
    DrawText(highScoreNameStr, highScoreNameEnterX, highScoreNameEnterY, 0);
    DrawText(highScoreTimeStr, highScoreTimeX, highScoreTimeY, 0);

    tilePtr = tileBasePtr = &gameVars->charArray[highScoreCharX + (highScoreCharY * charXNum)];

    for (i = j = 0; i < highScoreCharLen; i++) {
      *tilePtr = highScoreEntryStr[i] | dirtyChar;

      tilePtr += highScoreCharDeltaX;

      if (++j == highScoreCharWidth) {
        j = 0;
        tilePtr = tileBasePtr += (highScoreCharDeltaY * charXNum);
      }
    }

    piperObject.xPos = (enterHighScoreX * speedFactor);
    piperObject.yPos = enterHighScoreY;
    piperObject.flags = (flagDirectionX | flagNotInGame);

    gameVars->lastKickTime = 0;
    gameVars->lastCoin = gameVars->totalCoins = 0;

    for (i = lenHighScore; i--;) {
      gameVars->highScoreName[gameVars->gameDifficulty][i] = PChar_DASH;
      gameVars->highScoreStrTable[gameVars->gameDifficulty][gameVars->whichScore][i] = PChar_SPACE;
    }

    UpdateHighScoreSelector(0);
    UpdateHighScoreName();

    gameVars->timeEvent.time = timerTime;
    gameVars->timeEvent.origTime = gameVars->nextPeriodTime + (UInt32)(highScoreTime) +  (UInt32)(startTime);

    AddEvent(Event_Timer, startTime, (void *)(&gameVars->timeEvent));
  } else {
    AddEvent(Event_EndPhase, 0, (void *)(NULL));

    piperObject.flags |= (flagInvisible | flagNotInGame);
  }
}

//
// Title Screen
//
//   Update the screen for the title (i.e. Maryo Bros) and automatic movement of Maryo.
//

void
TitleScreen(register UInt16 init)
{
  register UInt16 i, x, y;
  register TilePtr tilePtr, tileBasePtr;
  register Tile floorTile;

  if (init == 0) {
    ClearTiles();

    tileBasePtr = &gameVars->charArray[titleNameX + (titleNameY * charXNum)];

    for (y = i = 0; y < titleTileY; y++) {
      tilePtr = tileBasePtr;
      for (x = 0; x < titleTileX; x++) {
        *tilePtr++ = titleScreen[i++] | dirtyChar;
      }
      tileBasePtr += charXNum;
    }

    floorTile =
      PChar_FLOOR1 + (SysRandom(0) & (PChar_PIPE_BRICK1 - PChar_FLOOR1));
    DrawTiles(floorTile, titleTile1X, titleTile1Y, titleTile1Len);

    piperObject.flags &= ~flagNotInGame;
    piperObject.xPos = (piperStartTitleX * speedFactor);;
    piperObject.yPos = piperStartTitleY;

    gameVars->enemyCount = 1;

    AddEvent(Event_EndPhase, titleTime, (void *)(NULL));

    gameVars->gamePhase = gameVars->nextTilePhase;
    gameVars->lastPhaseType = (UInt8) (-1);

    gameVars->currentKey = gameVars->currentCount = 0;
    InitializeKeyEvent(&gameVars->keyEvent, titleKeyList, titleKeyDelayList);
    AddEvent(Event_Key, 0, (void *)(&gameVars->keyEvent));
  }
  else
  {
    tilePtr = titleScreen;
    for (y = i = 0; y < titleTileY; y++)
    {
      for (x = 0; x < titleTileX; x++)
      {
        floorTile = *tilePtr++;
        if (floorTile != PChar_SPACE)
        {
          DrawChar((x + titleNameX) * charXSize,
                   (y + titleNameY) * charYSize, floorTile, charYSize);
        }
      }
    }
  }
}

//
// Bonus Phase
//
//   Update the screen for a bonus phase (i.e. bonus coins and timer).
//

void
BonusScreens(register UInt16 which)
{
  register TextEventPtr theEvent;
  register Tile tile;
  register UInt16 period;

  theEvent = &gameVars->textEvents[1];

  StrPrintF(gameVars->gameStr[2], bonusText);
  InitializeTextEvent(theEvent, gameVars->gameStr[2], bonusStartX,
                      bonusStartY, textTime, (textNormal | textDraw));

  AddEvent(Event_Text, 0, (void *)(theEvent));

  for (period = 0; period < (numBonusCoins * 2); period += 2)
  {
    InitializeBonusCoin(bonusCoinStart[period], bonusCoinStart[period + 1]);
  }

  tile = PChar_SPACE;
  if (which == Bonus_Num1) {
    period = bonus1Time;
    tile = PChar_FLOOR4;
  } else {
    (powObject.draw).seqNum = 0;
    powObject.flags |= flagDirty;

    if (which == Bonus_Num2) {
      period = bonus2Time;
      tile = PChar_FLOOR_ICE;
    } else {
      period = bonus3Time;
      tile = PChar_FLOOR_ICE;
    }
  }

  if (gameVars->gameDifficulty == Difficulty_Easy) {
    period += bonusExtraTime;
  } else if (gameVars->gameDifficulty == Difficulty_Hard) {
    if (period != bonus3Time) {
      period -= bonusExtraTime;
    }
  }

  ChangeFloorTiles(gameVars->gamePhase, tile);

  DisplayPhase(gameVars->gamePhase);
  DisplayTime(period, 1);

  gameVars->timeEvent.time = timerTime;
  gameVars->timeEvent.origTime =
    gameVars->nextPeriodTime + (((UInt32) (period) * timerFactor) / 100) + (UInt32) (startTime);
  gameVars->totalCoins = 0;

  AddEvent(Event_Timer, startTime, (void *)(&gameVars->timeEvent));
}

//
// Initialize Characters with Background Characters
//
//  Copy the preset initial screen onto the char tile array.
//

void
InitializeChar(void)
{
  register UInt16 i;
  register TilePtr tilePtr, tileInitPtr;

  tilePtr = gameVars->charArray + tileYMin;
  tileInitPtr = initialScreen + tileYMin;

  i = charUpdate - tileYMin;
  do {
    *tilePtr++ = *tileInitPtr++ | dirtyChar;
  } while (--i);
}

//
// Initialize Objects & Sprites
//
//   Initialize the object and sprite lists.
//

void
InitializeObjects(void)
{
  gameVars->spriteHead = (ObjElePtr) (NULL);
  gameVars->spriteCount = 0;

  gameVars->objectCount = 0;
}

//
// Add Object to Entrance List
//
//   Add an object to the entrance event list.
//

void
AddEnterObject(register UInt16 type,
               register UInt8 overrideTime)
{
  register ObjectPtr object;
  register UInt16 time, pos;

  if ((type == Object_IceMachine) || (type == Object_FireBall) || (type == Object_PaintBall)) {
    for (pos = numSetObjects; pos < gameVars->totalObjects; pos++) {
      if (gameVars->objectList[pos].type == type) {
        return;
      }
    }
  }

  pos = gameVars->totalObjects - numSetObjects;

  object = NewObject();
  InitializeObject(object, type);

  if (overrideTime == 0) {
    time = (entranceInterTime * (pos / 2)) + (entranceIntraTime * (pos & 0x1)) + startTime;
  } else {
    time = ((UInt32) (overrideTime) * 100) + startTime;
  }
  AddEvent(Event_Entrance, time, (void *)(object));
}

//
// Flip Enemies During Pow
//
//  Handle movement from hitting the POW.
//

void
FlipPow(register UInt16 start)
{
  if (start == ((hitTime / 2) - 1)) {
    register ObjElePtr objPtr;
    register ObjectPtr object;

    objPtr = gameVars->spriteHead;
    while (objPtr != (ObjElePtr) (NULL)) {
      object = objPtr->object;
      if ((object->flags & (flagEnter | flagExit | flagNotInGame | flagInvisible | flagWaitToEnter)) == 0) {
        if (object->motion == Motion_Bounce) {
          if ((CheckAny(object, object->yPos + object->draw.height + charYSize + 1, charYSize + (object->yPos % charYSize), checkAll) &
            dirtyChar) == 0) {
            ObjectFlipStart(object);
          }
        } else {
          ObjectFlipStart(object);
        }
      }
      objPtr = objPtr->next;
    }

    powObject.flags |= flagDirty;

    if (++(powObject.draw).seqNum == powObject.size) {
      powObject.flags |= (flagInvisible | flagNotInGame);
      gameVars->charArray[(powX + (powY * charXNum))] = PChar_SPACE | dirtyChar;
      gameVars->charArray[(powX + (powY * charXNum)) + 1] = PChar_SPACE | dirtyChar;
    }
  } else {
    gameVars->powNum++;
  }
}

//
// Update Sprite's Position
//
//   Update sprite's position based on current motion type and calculate the clipping
//     variables (both X and Y directions).
//

void
UpdateSprite(register ObjectPtr object)
{
  register Int16 x, y, delta;
  register UInt16 width, start, skip, level, motion;
  register AnimationPtr theAnimate;

  object->xPrev = (x = object->xPos);
  object->yPrev = (y = object->yPos);
  object->prevSeqNum = object->draw.seqNum;

  object->flags &= ~(flagEnter | flagExit);

  // Update Sprite's X Position
  motion = object->motion;
  if (motion == Motion_Flip) {
    x += (delta = object->flipDeltaX);
  } else {
    if (motion != Motion_Appear) {
      x += (delta = object->deltaX);
    } else {
      delta = 0;
    }
  }

  if (delta < 0) {
    if (x < 0) {
      if (motion == Motion_Ricochet) {
        PlaySound(Sound_PaintBall_Ricochet, Priority_Level_4);
        x = -x;
        object->deltaX = -delta;
      } else {
        x += (Int16) (screenWidth * speedFactor);
      }
    }
  } else if (delta > 0) {
    if (motion == Motion_Ricochet) {
      width = (((UInt16) (screenWidth) - objectWidth) * speedFactor);
      if (x > width) {
        PlaySound(Sound_PaintBall_Ricochet, Priority_Level_4);
        x = (width * 2) - x;
        object->deltaX = -delta;
      }
    } else {
      if (x >= (UInt16) (screenWidth * speedFactor)) {
        x -= (UInt16) (screenWidth * speedFactor);
      }
    }
  }
  object->xPos = x;

  level = object->level;
  if ((motion != Motion_Fall) && ((motion != Motion_Die) || (level != Level_Character)) && (motion != Motion_Flip)) {
    if (delta < 0) {
      if (++object->draw.seqNum >= object->size) {
        object->draw.seqNum = 0;
      }
    } else if (delta > 0) {
      if (object->draw.seqNum-- <= 0) {
        object->draw.seqNum = object->size - 1;
      }
    }
  }

  width = (UInt16) (screenWidth) - (start = (x / speedFactor));
  delta = objectWidth;
  skip = 0;

  if (((level == Level_Bonus) || (level == Level_Enemy)) && ((motion == Motion_Run) || (motion == Motion_Bounce))) {
    register Int16 origX, xEnd;

    origX = (((Int16) (object->xPrev) + object->deltaX) / speedFactor);
    xEnd = origX + delta;

    if (y >= objectExitY) {
      if (gameVars->phaseType == Phase_Normal) {
        if (origX < objectEnterX1) {
          if (xEnd <= objectExitX1) {
            ObjectExitPipe(object);
            motion = object->motion;
            y = screenYMax;

            origX = (objectEnterX1 - objectWidth + 1);
            xEnd = origX + delta;
          } else {
            if (origX < objectExitX1) {
              start = objectExitX1;
              width = delta - (objectExitX1 - origX);
            } else {
              width = delta;
            }
            object->flags |= flagExit;
          }
        } else if (xEnd > objectEnterX2) {
          if (origX >= objectExitX2) {
            ObjectExitPipe(object);
            motion = object->motion;
            y = screenYMax;

            origX = (objectEnterX2 - 1);
            xEnd = origX + delta;
          } else {
            if (xEnd > objectExitX2) {
              width = objectExitX2 - origX;
            } else {
              width = delta;
            }
            object->flags |= flagExit;
          }
        }
      }
    }

    if (y == objectEnterY1)
    {
      if ((origX < objectEnterX1) && (origX >= objectExitX1))
      {
        if (object->deltaX > 0)
        {
          start = objectEnterX1;
          width = delta - (skip = (objectEnterX1 - origX));
          object->flags |= flagEnter;
        }
      }
      else if ((xEnd > objectEnterX2) && (xEnd <= objectExitX2))
      {
        if (object->deltaX < 0)
        {
          start = origX;
          skip = delta - (width = (objectEnterX2 - origX));
          object->flags |= flagEnter;
        }
      }
    }
    else if (gameVars->phaseType == Phase_Instructions)
    {
      if (y == objectEnterY3)
      {
        start = origX;
        skip = delta - (width = (objectEnterX3 - origX));
        object->flags |= flagEnter;
      }
    }
  }

  if ((object->flags & (flagEnter | flagExit)) == 0) {
    if (width >= delta) {
      width = delta;
    } else if (width < (delta / 2)) {
      start = 0;
      if ((object->flags & flagDirectionX) == 0) {
        skip = width;
      }
      width = delta - width;
    } else {
      if ((object->flags & flagDirectionX) != 0) {
        skip = delta - width;
      }
    }
  }

  object->draw.xPixels = width;
  object->draw.xStart = start;
  object->draw.xSkip = skip;

  // Update Sprite's Y Position
  switch (motion)
  {
    case Motion_Jump:
      start = object->deltaY++;

      if (gameVars->flipFloorTile == PChar_POW) {
        gameVars->flipFloorTile = PChar_SPACE;
        gameVars->powNum = 0;
      }

      if (start != stopFall) {
        delta = ((start - (jumpTime / 2)) * 2);

        if ((start == 0) && (gameVars->platformTile != PChar_SPACE)) {
          AddEvent(Event_Revive, 0, (void *)(NULL));
        }

        if (start < (jumpTime / 2)) {
          delta++;
          if (start == 0) {
            delta -= (jumpHeight - ((jumpTime / 2) * (jumpTime / 2)));
          } else if (start == (maxHitTime + 1)) {
            PlaySound(Sound_Jump, Priority_Level_3);
          }
        } else if (start > (jumpTime / 2)) {
          delta--;
          if (start == jumpTime) {
            delta += (jumpHeight - ((jumpTime / 2) * (jumpTime / 2)));
          }
        }

        if (delta > (Int16) (maxAcceleration)) {
          delta = (Int16) (maxAcceleration);
        } else if (delta < (Int16) (-maxAcceleration)) {
          delta = (Int16) (-maxAcceleration);
        }
        y += delta;

        if (start <= maxHitTime) {
          y = CheckHit(object, y, delta);
        } else if (start >= (jumpTime / 2)) {
          y = CheckDropOn(object, y, delta);
        }
      } else {
        ResetReviveTiles();
        object->deltaY = 0;
        if (object->deltaX) {
          if (gameVars->slideNum == 0) {
            ChangeObjectSequence(&piperObject, piperRunSequence, sizeof(piperRunSequence), Motion_Run);
          } else {
            ChangeObjectSequence(&piperObject, piperSlideSequence, sizeof(piperSlideSequence), Motion_Slide);
          }
        } else {
          ChangeObjectSequence(&piperObject, piperStopSequence, sizeof(piperStopSequence), Motion_Stop);
        }
      }
      break;

    case Motion_Fall:
      start = object->deltaY++;

      if (start != stopFall) {
        delta = ((start * 2) + 1);

        if (delta > (Int16) (maxAcceleration)) {
          delta = (Int16) (maxAcceleration);
        } else if (delta < (Int16) (-maxAcceleration)) {
          delta = (Int16) (-maxAcceleration);
        }

        y += delta;

        if ((object == &piperObject) && (start == 0) && (gameVars->platformTile != PChar_SPACE)) {
          AddEvent(Event_Revive, 0, (void *)(NULL));
        }

        if (start >= (fallTime / 2)) {
          y = CheckDropOn(object, y, delta);
        }
      } else {
        if (object->type == Object_Character) {
          ResetReviveTiles();
        }
        object->deltaY = 0;
        object->motion = Motion_Run;
      }
      break;

    case Motion_Hit:
      start = object->deltaY++;

      if (start <= (hitTime - 1)) {
        if (gameVars->flipFloorTile == PChar_POW) {
          if (start == 0) {
            PlaySound(Sound_Pow, Priority_Level_2);
          }

          FlipPow(start);
        } else {
          if (start == 0) {
            PlaySound(Sound_Hit, Priority_Level_3);
          }

          flipObject.yPos = (y - charYSize);
        }

        delta = (start - (hitTime / 2));

        if (start < (hitTime / 2)) {
          delta += ((delta + 1) & 0x1);
        } else if (start >= (hitTime / 2)) {
          delta -= (delta & 0x1);
        }
        y += delta;
      } else {
        object->motion = Motion_Jump;
        object->deltaY = jumpTime;

        flipObject.flags |= flagNotInGame;

        if (gameVars->flipFloorTile == PChar_POW) {
          gameVars->powNum = (2 * charYSize);
        }
      }
      break;

    case Motion_Ricochet:
      y += (delta = object->deltaY);
      if (y < (topScreen - (object->draw.height / 2))) {
        PlaySound(Sound_PaintBall_Ricochet, Priority_Level_4);
        y = ((2 * topScreen) - (object->draw.height + y));
        object->deltaY = -delta;
      } else {
        y = CheckRicochet(object, y, delta);
      }
      break;

    case Motion_Surprise:
      start = object->deltaY++;

      if (start >= surpriseTime) {
        ChangeObjectSequence(&piperObject, piperFlapSequence, sizeof(piperFlapSequence), Motion_Flap);
        object->deltaY = 0;
      }
      break;

    case Motion_Flap:
      start = object->deltaY++;

      if (start <= (flapTime - 1)) {
        delta = (start - (flapTime / 2));

        if (start == 0) {
          delta--;
        } else if (start == (flapTime - 1)) {
          delta++;
        }

        y += delta;
      } else {
        PlaySound(Sound_Die, Priority_Level_1);
        ChangeObjectSequence(&piperObject, piperDieSequence, sizeof(piperDieSequence), Motion_Die);
        object->deltaY = 0;
      }
      break;

    case Motion_Drop:
    case Motion_Die:
      start = object->deltaY++;

      delta = ((start * 2) + 1);
      if (delta > (Int16) (maxAcceleration)) {
        delta = (Int16) (maxAcceleration);
      } else if (delta < (Int16) (-maxAcceleration)) {
        delta = (Int16) (-maxAcceleration);
      }

      y += delta;
      if (y >= screenYMax) {
        if (motion == Motion_Drop) {
          y = ObjectReincarnate(object);
        } else {
          if (gameVars->phaseType != Phase_Instructions) {
            theAnimate = AddAnimation();
            InitializeAnimation(theAnimate, Animation_Splash, object);
#ifdef INCLUDE_SOUND
            PlaySound(Sound_Kill, Priority_Level_3);
#endif

            theAnimate->draw.yStart = screenYMax - charYSize;
            theAnimate->draw.yPixels = animateHeight;
          }

          y = ObjectKill(object);
          if (gameVars->enemyCount == 0) {
            gameVars->gameState |= gameLast;
          }
        }
      }
      break;

    case Motion_Wave:
      x = (((Int16) (object->xPrev) + (delta = object->deltaX)) / speedFactor);

      if (delta > 0) {
        if (object->xPos > ((screenWidth - objectWidth) * speedFactor)) {
          object->xPos = ((screenWidth - objectWidth) * speedFactor);
          ObjectExtinquish(object, 0);
          break;
        } else if (object->xPos >= (objectWidth * speedFactor)) {
          object->flags &= ~flagNotInGame;
        }
      } else {
        if (object->xPos >= ((screenWidth - (objectWidth / 2)) * speedFactor)) {
          object->xPos = 0;
          ObjectExtinquish(object, 0);
          break;
        } else if (object->xPos <= ((screenWidth - (2 * objectWidth)) * speedFactor)) {
          object->flags &= ~flagNotInGame;
        }
      }

      level = (x % (numFireBallSpikes * objectWidth));
      delta /= speedFactor;

      if ((level >= objectWidth) && (level < (objectWidth * 2))) {
        y -= delta;
      }  else if ((level >= (objectWidth * 2)) && (level < (objectWidth * 3))) {

        y += delta;
      }

#ifdef INCLUDE_SOUND
      PlaySound(Sound_FireBall, Priority_Level_3);
#endif
      break;

    case Motion_Bounce:
      if ((object->flags & flagEnter) != 0) {
        if ((object->deltaX == (Int8) (speedFactor)) || (object->deltaX == -(Int8) (speedFactor))) {
          object->deltaY = bounceTime;
        } else {
          object->deltaY = (bounceTime / 2);
        }
      } else if (((object->flags & flagExit) == 0) || (object->deltaY != 0)) {
        start = object->deltaY++;

        if (start != stopFall) {
          if ((object->deltaX == (Int8) (speedFactor)) || (object->deltaX == -(Int8) (speedFactor))) {
            start /= speedFactor;
            delta = (start - (bounceTime / 2));
          } else {
            delta = ((start - (bounceTime / 2)) * 2);
          }

          if (start == ((bounceTime / 2) - 1)) {
            delta++;
          } else if (start == ((bounceTime / 2) + 1)) {
            delta--;
          }

          if (delta > (Int16) (maxAcceleration)) {
            delta = (Int16) (maxAcceleration);
          } else if (delta < (Int16) (-maxAcceleration)) {
            delta = (Int16) (-maxAcceleration);
          }
          y += delta;

          if (start >= (bounceTime / 2)) {
            y = CheckDropOn(object, y, delta);
          }
        } else {
          object->deltaY = 0;
        }
      }
      break;

    case Motion_Flip:
      start = object->deltaY++;

      if (start != stopFall) {
        if ((object->flags & flagCount) == 0) {
          if (++object->draw.seqNum >= object->size) {
            object->draw.seqNum = 0;
          }
        }

        delta = ((start - (flipperTime / 2)) * 2);

        if (delta > (Int16) (maxAcceleration)) {
          delta = (Int16) (maxAcceleration);
        } else if (delta < (Int16) (-maxAcceleration)) {
          delta = (Int16) (-maxAcceleration);
        }
        y += delta;

        if (start == 0) {
          if ((gameVars->phaseType == Phase_Normal) && ((object->flags & flagCount) == 0)) {
            if (object->level == Level_Bonus) {
              gameVars->gameScore += pointsCoin;
            } else if (gameVars->powNum == 0) {
              gameVars->gameScore += pointsIceMachine;
            }
          }
        } else if (start >= (flipperTime / 2)) {
          if (start == (flipperTime / 2)) {
            if ((object->flags & flagCount) == 0) {
              y = ObjectBonus(object);
            } else {
              object->flags ^= flagDirectionX;

              ChangeEnemySequence(object);
            }
          }

          y = CheckDropOn(object, y, delta);
          x = object->flags;
          if ((object->deltaY == stopFall) && ((object->type != Object_Crab) || ((x & flagHit) != 0))) {
            object->flipDeltaX = object->deltaX;
            object->deltaX = 0;
          }
        }
      } else {
        ObjectFlipStop(object);
      }
      break;

    case Motion_Slide:
    case Motion_Run:
      motion = object->flags;
      if ((motion & (flagEnter | flagExit | flagWaitToEnter)) == 0) {
        CheckDropOff(object, y);
      }

      if (object->type == Object_IceMachine) {
        register Int16 x1, x2;

        x1 = object->xPrev;
        x2 = x1 + (delta = object->deltaX);

        if (delta < 0)
        {
          delta = x1;
          x1 = x2;
          x2 = delta;
        }

        delta = ((numIceMachineStops - 1) * 2);
        if (gameVars->phaseType == Phase_Instructions) {
          delta += 2;
        }

        do {
          x = ((Int16) (iceMachineStops[delta]) * speedFactor);
          if ((y == iceMachineStops[delta + 1]) && (x >= x1) && (x <= x2) && (gameVars->iceMachineFill[delta / 2] == 0)) {
            register AnimationPtr theAnimate;

            gameVars->iceMachineFill[delta / 2] = 1;

            object->motion = Motion_Spin;
            object->xPos = x;
            object->deltaX = 0;
            object->deltaY = 0;

            PlaySound(Sound_IceMachine_Freeze, Priority_Level_2);

            AddEvent(Event_Melt, meltTime, (void *)(object));

            theAnimate = AddAnimation();
            InitializeAnimation(theAnimate, Animation_IceMachine_Melt,
                                object);

            theAnimate->draw.yPixels = animateHeight;

            ChangeAnimationPos(theAnimate, 0, objectWidth + 1);
            break;
          }
        } while ((delta -= 2) >= 0);
      }
      break;

    case Motion_Spin:
    case Motion_Appear:
      if (gameVars->phaseType != Phase_Total) {
        if (++object->draw.seqNum >= object->size) {
          object->draw.seqNum = 0;
        }
      }
      break;

    case Motion_Revive:
      if ((y += piperReviveDeltaY) >= piperReviveEndY) {
        y = piperReviveEndY;
        piperObject.motion = Motion_Stop;
        piperObject.flags &= ~flagNotInGame;

        gameVars->platformTile = PChar_PLATFORM1;
        AddEvent(Event_Platform, 0, (void *)(NULL));
      }
      break;

    case Motion_Stop:
      if (((object->flags & (flagFlipped | flagNotInGame)) == 0) && (object->num >= numSetObjects)) {
        CheckDropOff(object, y);
      }
      break;
  }
  object->yPos = y;

  delta = y;
  width = object->draw.height;
  skip = 0;

  if (delta < topScreen) {
    skip = topScreen - delta;
    if (width > skip) {
      width -= skip;
    } else {
      width = 0;
    }
    delta = topScreen;
  }

  object->draw.yPixels = width;
  object->draw.yStart = delta;
  object->draw.ySkip = skip;
}

//
// Display Total Text
//
//   Display bonus total text and add delay event.
//

void
DisplayTotalText(void)
{
  register TextEventPtr theTextEvent;

  gameVars->gameScore += (pointsCoin * gameVars->totalCoins);

  DrawText(totalCoin1Str, totalCoin1StartX, totalCoin1StartY, 0);

  theTextEvent = &gameVars->textEvents[0];
  if (gameVars->totalCoins == numBonusCoins) {
    PlaySound(Sound_Bonus_Perfect, Priority_Level_0);

    if (gameVars->gamePhase == (phaseBonus1 + phaseBonus)) {
      gameVars->bonusPoints = pointsBonus1;
    } else {
      gameVars->bonusPoints = pointsBonus2;
    }

    StrPrintF(gameVars->gameStr[0], totalCoinPerfectStr, gameVars->bonusPoints);
    InitializeTextEvent(theTextEvent, gameVars->gameStr[0], totalCoinPerfectStartX, totalCoinPerfectStartY,
                        totalTime, (textNormal | textDraw));
  } else {
    gameVars->bonusPoints = 0;

    StrPrintF(gameVars->gameStr[0], totalCoinNoBonusStr);
    InitializeTextEvent(theTextEvent, gameVars->gameStr[0], totalCoinNoBonusStartX, totalCoinNoBonusStartY,
                        totalTime, (textNormal | textDraw));
  }

  AddEvent(Event_Text, gameTotalTime, (void *)(theTextEvent));
  AddEvent(Event_EndPhase, totalTime, (void *)(NULL));
}

//
// Delay of Game
//
//   Increase the speed of the enemies when a delay of game event fires off.
//

void
DelayofGame(void)
{
  register Int16 deltaX, deltaY;
  register UInt16 fastSpeed;
  register ObjElePtr objPtr;
  register ObjectPtr object;

  objPtr = gameVars->spriteHead;
  while (objPtr != (ObjElePtr) (NULL)) {
    object = (objPtr->object);

    if ((object->flags & flagCount) != 0) {
      if (object->type == Object_Turtle) {
        fastSpeed = turtleFastSpeedX;
      } else if (object->type == Object_Crab) {
        fastSpeed = crabFastSpeedX;
      } else {
        fastSpeed = fighterFlyFastSpeedX;
      }

      if ((object->flags & flagFlipped) == 0) {
        if (object->deltaX > 0) {
          if (++object->deltaX > (Int16) (fastSpeed)) {
            object->deltaX = (Int16) (fastSpeed);
          }
        } else if (object->deltaX < 0) {
          if (--object->deltaX < (Int16) (-fastSpeed)) {
            object->deltaX = (Int16) (-fastSpeed);
          }
        }
      } else {
        if (object->flipDeltaX > 0) {
          if (++object->flipDeltaX > (Int16) (fastSpeed)) {
            object->flipDeltaX = (Int16) (fastSpeed);
          }
        } else if (object->flipDeltaX < 0) {
          if (--object->flipDeltaX < (Int16) (-fastSpeed)) {
            object->flipDeltaX = (Int16) (-fastSpeed);
          }
        }
      }
    } else if (object->type == Object_PaintBall) {
      deltaY = object->deltaY;
      if (deltaY > 0) {
        deltaY = -deltaY;
      }

      if (deltaY == paintBallSpeedY) {
        deltaX = paintBallFastSpeedX;
        deltaY = paintBallFastSpeedY;
      } else if (deltaY == paintBallFastSpeedY) {
        deltaX = paintBallFastestSpeedX;
        deltaY = paintBallFastestSpeedY;
      } else {
        deltaX = paintBallInsaneSpeedX;
        deltaY = paintBallInsaneSpeedY;
      }

      if (object->deltaX > 0) {
        object->deltaX = deltaX;
      } else {
        object->deltaX = -deltaX;
      }
      if (object->deltaY < 0) {
        object->deltaY = deltaY;
      } else {
        object->deltaY = -deltaY;
      }
    }
    objPtr = objPtr->next;
  }
}

//
// Update Game Events
//
//   Update and handle events - text, entrances, timers, new phases, etc.
//

void
UpdateEvents(void)
{
  register UInt16 i;
  register UInt32 timeLeft;
  register GameEventPtr theEvent;
  register TextEventPtr theTextEvent;
  register ObjectPtr object;
  register AnimationPtr theAnimate;

  i = gameVars->eventGameCount;
  while (i--) {
    theEvent = &gameVars->eventList[i];

    if (theEvent->time <= gameVars->nextPeriodTime) {
      register UInt16 type, theFlags, len;
      register Tile tile1, tile2;
      register void *event;

      type = theEvent->type;
      event = theEvent->event;

      DeleteEvent(theEvent);

      switch (type) {
        case Event_Text:
          theTextEvent = (TextEventPtr) (event);
          theFlags = theTextEvent->flags;

          if ((theFlags & textDraw) != 0) {
            theTextEvent->flags = theFlags & ~textDraw;
            DrawText(theTextEvent->text, theTextEvent->x, theTextEvent->y, 0);
            AddEvent(Event_Text, theTextEvent->time, (void *)(event));
          } else {
            len = StrLen((const Char *)(theTextEvent->text));
            EraseText(len, theTextEvent->x, theTextEvent->y, 0);
            if ((theFlags & textBlink) != 0) {
              theTextEvent->flags = theFlags | textDraw;
              AddEvent(Event_Text, theTextEvent->time, (void *)(event));
            }
          }

          if (gameVars->phaseType == Phase_Total) {
            gameVars->gameScore += gameVars->bonusPoints;
          }
          break;

        case Event_Entrance:
          object = (ObjectPtr) (event);

          if (object == &piperObject) {
            RevivePiper();
          } else {
            if (!ObjectEnterPipe(object)) {
              AddEvent(Event_Entrance, 0, event);
            } else {
              if (object->type == Object_Icicle) {
                AddEvent(Event_Wakeup, dripTime, (void *)(object));
                PlaySound(Sound_IceMachine_Appear, Priority_Level_2);
              }
            }
          }
          break;

        case Event_FlipBack:
          object = (ObjectPtr) (event);

          ChangeEnemySequence(object);
          ObjectFlipOver(object, 1);
          object->flags &= ~flagWaking;
          break;

        case Event_Wakeup:
          object = (ObjectPtr) (event);

          object->flags |= flagWaking;
          if (object->type == Object_Icicle) {
            register UInt16 timeDrop;

            changeSequence(object, icicleSequence, sizeof(icicleSequence));

            timeDrop = dropTime;
            if (gameVars->gameDifficulty == Difficulty_Hard) {
              timeDrop -= (dropTime / 2);
            }
            AddEvent(Event_Drop, timeDrop, event);
          } else {
            AddEvent(Event_FlipBack, flipTime, event);
          }
          break;

        case Event_EndPhase:
          DestroyPhase();
          gameVars->gameState |= gameEnd;
          if (gameVars->phaseType == Phase_EnterHighScores) {
            gameVars->gameOver = true;
          }
          break;

        case Event_Timer:
          if (gameVars->nextPeriodTime >= gameVars->timeEvent.origTime) {
            timeLeft = 0;
            if (gameVars->phaseType == Phase_Bonus) {
              AddEvent(Event_EndPhase, 0, (void *)(NULL));
            } else {
              RegisterName();
              UpdateHighScoreName();
            }
          } else {
            if (gameVars->phaseType == Phase_Bonus) {
              timeLeft = ((gameVars->timeEvent.origTime - gameVars->nextPeriodTime) * 100) / timerFactor;
            } else {
              timeLeft = gameVars->timeEvent.origTime - gameVars->nextPeriodTime;
            }
            if ((gameVars->enemyCount > 0) || (gameVars->phaseType != Phase_Bonus)) {
              AddEvent(Event_Timer, timerTime, (void *)(&gameVars->timeEvent));
            }
          }

          if (gameVars->phaseType == Phase_Bonus) {
            PlaySound(Sound_Timer, Priority_Level_3);

            DisplayTime(timeLeft, 0);
          } else if (timeLeft != 0) {
            DisplayHighScoreTime(timeLeft);
          }
          break;

        case Event_Drop:
          object = (ObjectPtr) (event);

          object->motion = Motion_Drop;
          PlaySound(Sound_Drop, Priority_Level_2);
          break;

        case Event_Key:
          if (gameVars->currentCount-- == 0) {
            len = gameVars->keyEvent.keyList[gameVars->keyEvent.keyNum++];
            gameVars->currentCount = gameVars->keyEvent.keyDelayList[gameVars->keyEvent.keyDelayNum++];

            if (gameVars->currentCount-- == endDelay) {
              AddEvent(Event_EndPhase, 0, (void *)(NULL));
              break;
            } else {
              gameVars->currentKey = len;
            }
          }

          AddEvent(Event_Key, 0, (void *)(&gameVars->keyEvent));
          break;

        case Event_TileChange:
          if ((gameVars->charArray[bonus3StartPos] & ~dirtyChar) != PChar_FLOOR_SEMI_INVIS) {
            ChangeFloorTiles(gameVars->gamePhase, PChar_FLOOR_SEMI_INVIS);
            AddEvent(Event_TileChange, invisibleTime, (void *)(NULL));
          } else {
            ChangeFloorTiles(gameVars->gamePhase, PChar_FLOOR_INVIS);
          }
          break;

        case Event_Melt:
          object = (ObjectPtr) (event);

          InitializeFreezeEvent(&gameVars->freezeEvent, object);
          AddEvent(Event_Freeze, 0, (void *)(&gameVars->freezeEvent));

          DeleteAnimationObject(object);

          ObjectReincarnate(object);
          break;

        case Event_Freeze:
          theFlags = 0;

          tile1 = *(gameVars->freezeEvent.tileLoc1);
          tile2 = *(gameVars->freezeEvent.tileLoc2);

          if ((tile1 >= FLOOR_START) && (tile1 <= FLOOR_END1)) {
            *(gameVars->freezeEvent.tileLoc1)-- = PChar_FLOOR_ICE | dirtyChar;
            theFlags = 1;
          }
          if ((tile2 >= FLOOR_START) && (tile2 <= FLOOR_END1)) {
            *(gameVars->freezeEvent.tileLoc2)++ = PChar_FLOOR_ICE | dirtyChar;
            theFlags = 1;
          }

          if (theFlags != 0) {
            AddEvent(Event_Freeze, 0, (void *)(&gameVars->freezeEvent));
          }
          break;

        case Event_Instructions:
          len = gameVars->instructionEvent->time;
          if (len != endInstruction) {
            register UInt32 time;
            register UInt8 *destStr;

            time = (((UInt32)(len) - gameVars->lastInstructionTime) * gameVars->keyRepeatRate);
            gameVars->lastInstructionTime = len;

            theTextEvent = (TextEventPtr)(&gameVars->textEvents[gameVars->instructionEvent->eventNum]);
            destStr = gameVars->gameStr[gameVars->instructionEvent->strNum];

            RemoveTextEvent(theTextEvent);

            StrPrintF(destStr, gameVars->instructionEvent->text);
            InitializeTextEvent(theTextEvent, destStr, gameVars->instructionEvent->x, gameVars->instructionEvent->y,
                                instructionTime, (textNormal | textDraw));

            AddEvent(Event_Instructions, time, (void *)(gameVars->instructionEvent++));
            AddEvent(Event_Text, time, (void *)(theTextEvent));
          }
          break;

        case Event_Coin:
          if (gameVars->lastCoin < gameVars->totalCoins) {
            PlaySound(Sound_Collect_Bonus, Priority_Level_2);

            gameVars->objectList[numSetObjects + gameVars->lastCoin++].flags &= ~(flagInvisible | flagNotInGame);

            AddEvent(Event_Coin, 0, (void *)(NULL));
          } else {
            DisplayTotalText();
          }
          break;

        case Event_Revive:
          gameVars->charArray[revivePos]     = PChar_SPACE | dirtyChar;
          gameVars->charArray[revivePos + 1] = PChar_SPACE | dirtyChar;

          if ((piperObject.motion != Motion_Fall) && (piperObject.motion != Motion_Jump)) {
            CheckDropOff(&piperObject, piperObject.yPos);
          }

          DeleteEventType(Event_Platform);
          break;

        case Event_Platform:
          if ((gameVars->platformTile - PChar_PLATFORM1) < numPlatforms) {
            if (gameVars->platformTile == PChar_PLATFORM1) {
              DeleteAnimationType(Animation_Platform);
            }

            gameVars->charArray[revivePos] = gameVars->platformTile | dirtyChar;
            gameVars->charArray[revivePos + 1] = gameVars->platformTile | dirtyChar;

            gameVars->platformTile++;

            AddEvent(Event_Platform, platformTime, (void *)(NULL));
          } else {
            AddEvent(Event_Revive, 0, (void *)(NULL));
          }
          break;

        case Event_Invisible:
          object = (ObjectPtr) (event);

          object->flags |= flagInvisible;
          break;

        case Event_Appear:
          object = (ObjectPtr) (event);

          if (object->type == Object_FireBall) {
            object->motion = Motion_Wave;
            changeSequence(object, fireBallSequence, sizeof(fireBallSequence));
          } else {
            object->flags &= ~flagNotInGame;
            object->motion = Motion_Ricochet;
            changeSequence(object, paintBallSequence, sizeof(paintBallSequence));
          }
          break;

        case Event_Disappear:
          object = (ObjectPtr) (event);

          ObjectIgnite(object);
          break;

        case Event_Remove:
          object = (ObjectPtr) (event);

          ObjectRemove(object);

          if (gameVars->phaseType != Phase_Bonus) {
            theAnimate = AddAnimation();
            InitializeAnimation(theAnimate, Animation_Points_800, object);

            theAnimate->draw.yPixels = animateHeight;
            theAnimate->flags &= ~flagAnimDirectionX;
          }
          break;

        case Event_Animation:
          theAnimate = AddAnimation();
          InitializeAnimation(theAnimate, Animation_Bonus_Coin_1, (ObjectPtr) (NULL));

          theAnimate->draw.xStart = instruction1Anim1X;
          theAnimate->draw.yStart = instruction1Anim1Y;

          theAnimate = AddAnimation();
          InitializeAnimation(theAnimate, Animation_Bonus_Coin_2, (ObjectPtr) (NULL));

          theAnimate->draw.xStart = instruction1Anim2X;
          theAnimate->draw.yStart = instruction1Anim2Y;
          break;

        case Event_Turn:
          object = (ObjectPtr) (event);

				  object->flags ^= flagDirectionX;
          object->deltaX = -object->deltaX;
          object->xPos = object->xPrev;

          if (object->type == Object_FighterFly) {
            object->motion = Motion_Bounce;
          } else {
            if (object->deltaY == 0) {
              object->motion = Motion_Run;
            } else {
              object->motion = Motion_Fall;
            }

            if (object->type == Object_Turtle) {
              changeSequence(object, turtleSequence, sizeof(turtleSequence));
            } else if (object->type == Object_Crab) {
              if (((object->flags) & flagHit) == 0) {
                changeSequence(object, crabSequence, sizeof(crabSequence));
              } else {
                changeSequence(object, crabHitSequence, sizeof(crabHitSequence));
              }
            }
          }
          break;

        case Event_DelayGame:
          DelayofGame();

          len = delayGameTime;
          if (gameVars->gameDifficulty == Difficulty_Easy) {
            len += (delayGameTime / 3);
          } else if (gameVars->gameDifficulty == Difficulty_Hard) {
            len -= (delayGameTime / 3);
          }
          AddEvent(Event_DelayGame, len, (void *)(NULL));
          break;
      }
    }
  }
}

//
// Initialize Phase
//
//   Increment the phase number, intialize the new phase and call main phase displayer function.
//     Setup enemy entrance lists and initialize the sprite, animation and object lists.
//

void
InitializePhase(void)
{
  register ObjectPtr object;
  register UInt16 instructNum, bonusNum, storedPhase;

  if (gameVars->gamePhase == 0) {
    InitializeGame();
  }

  gameVars->eventGameCount = 0;
  gameVars->gameState = gameNormal;

  gameVars->lastKickTime = 0;
  gameVars->kickCount = 0;

  ResetReviveTiles();

  instructNum = bonusNum = storedPhase = 0;
  gameVars->phaseType = Phase_Normal;

  gameVars->gamePhase++;

  InitializeObject(&piperObject, Object_Character);
  InitializeObject(&flipObject, Object_Flip);
  if (gameVars->objectCount == 0) {
    InitializeObject(&powObject, Object_Pow);
  }

  gameVars->totalObjects = numSetObjects;

  if (gameVars->gamePhase == (phaseBonus1 + 1)) {
    bonusNum = Bonus_Num1;
    gameVars->phaseType = Phase_Total;
  } else if (gameVars->gamePhase == (phaseBonus2 + 1)) {
    bonusNum = Bonus_Num2;
    gameVars->phaseType = Phase_Total;
  } else if (((gameVars->gamePhase >= (phaseBonus3 + 1)) && (gameVars->gamePhase <= maxPhaseNum)) &&
            (((gameVars->gamePhase - phaseBonus3) % storedPhaseIterations) == 1)) {
    bonusNum = Bonus_Num3;
    gameVars->phaseType = Phase_Total;
  } else if (gameVars->gamePhase == phaseEnterHighScore) {
    gameVars->phaseType = Phase_EnterHighScores;
  } else if (gameVars->gamePhase == phaseDisplayHighScore) {
    gameVars->phaseType = Phase_DisplayHighScores;
  } else if (gameVars->gamePhase == phaseTitle) {
    gameVars->phaseType = Phase_Title;
  } else {
    if (gameVars->gamePhase >= phaseBonus) {
      gameVars->gamePhase -= phaseBonus;
    }

    if (gameVars->gamePhase == phaseInstructTurtle) {
      instructNum = Instruction_Turtle;
      gameVars->phaseType = Phase_Instructions;
    } else if (gameVars->gamePhase == phaseInstructCrab) {
      instructNum = Instruction_Crab;
      gameVars->phaseType = Phase_Instructions;
    } else if (gameVars->gamePhase == phaseInstructFighterFly) {
      instructNum = Instruction_FighterFly;
      gameVars->phaseType = Phase_Instructions;
    } else if (gameVars->gamePhase == phaseInstructIceMachine) {
      gameVars->gamePhase += phaseOther;
      gameVars->phaseType = Phase_Other;
    } else if (gameVars->gamePhase == (phaseInstructIceMachine + phaseOther + 1)) {
      instructNum = Instruction_IceMachine;
      gameVars->gamePhase -= (phaseOther + 1);
      gameVars->phaseType = Phase_Instructions;
    } else {
      if (gameVars->gamePhase > phaseInstruction) {
        gameVars->gamePhase -= (phaseInstruction + 1);
      }

      storedPhase = gameVars->gamePhase - 1;
      while (storedPhase >= maxStoredPhase) {
        storedPhase -= storedPhaseIterations;
      }

      if (storedPhase == (phaseBonus1 - 1)) {
        bonusNum = Bonus_Num1;
        gameVars->phaseType = Phase_Bonus;
      } else if (storedPhase == (phaseBonus2 - 1)) {
        bonusNum = Bonus_Num2;
        gameVars->phaseType = Phase_Bonus;
      } else if (storedPhase == (phaseBonus3 - 1)) {
        bonusNum = Bonus_Num3;
        gameVars->phaseType = Phase_Bonus;
        AddEvent(Event_TileChange, semiInvisTime + startTime, (void *)(NULL));
      }
    }
  }

  InitializeAnimations();

  if (gameVars->phaseType != gameVars->lastPhaseType) {
    gameVars->lastPhaseType = gameVars->phaseType;

    if (gameVars->phaseType == Phase_Normal) {
      InitializeChar();
    } else if (gameVars->phaseType == Phase_Instructions) {
      gameVars->gamePhase += phaseInstruction;
      InstructionScreens(instructNum);
    } else if (gameVars->phaseType == Phase_Bonus) {
      BonusScreens(bonusNum);
    } else if (gameVars->phaseType == Phase_Other) {
      OtherScreen();
    } else if (gameVars->phaseType == Phase_Total) {
      gameVars->gamePhase += (phaseBonus - 1);
      TotalScreens(bonusNum);
    }

    if (gameVars->phaseType == Phase_EnterHighScores) {
      EnterHighScores();
    } else if (gameVars->phaseType == Phase_DisplayHighScores) {
      DisplayHighScores(numHighScore, 1);
    } else if (gameVars->phaseType == Phase_Title) {
      TitleScreen(0);
    } else {
      gameVars->lastGameScore = (UInt32) (-1);
      gameVars->lastNumLives = (UInt8) (-1);
    }

    if ((gameVars->phaseType == Phase_Normal) || (gameVars->phaseType == Phase_Bonus)) {
      if ((powObject.draw).seqNum == powObject.size) {
        powObject.flags |= (flagInvisible | flagNotInGame);

        gameVars->charArray[(powX + (powY * charXNum))] = PChar_SPACE | dirtyChar;
        gameVars->charArray[(powX + (powY * charXNum)) + 1] = PChar_SPACE | dirtyChar;
      } else {
        powObject.flags &= ~(flagInvisible | flagNotInGame);
        powObject.flags |= flagDirty;
      }
    } else {
      powObject.flags |= (flagInvisible | flagNotInGame);
    }
  }

  if (gameVars->gamePhase == gameVars->gameOverPhase) {
    GameOver();
  } else {
    if ((gameVars->phaseType == Phase_Normal) || (gameVars->phaseType == Phase_Bonus)) {
      PlaySound(Sound_Phase_Start, Priority_Level_0);
      gameVars->lastGamePhase = gameVars->gamePhase;
    }

    if (gameVars->phaseType == Phase_Normal) {
      register UInt8 enemyEntry, enemyType;
      register UInt16 enemyPos, ballTime;
      register PhaseScriptPtr script;

      ChangeFloorTiles(gameVars->gamePhase, PChar_SPACE);
      DisplayPhase(gameVars->gamePhase);

      script = &phaseScripts[storedPhase];

      enemyPos = 0;
      enemyEntry = script->list[0];
      enemyType = (enemyEntry >> ((enemyBytes - 1) * enemyBits));

      do {
        if ((gameVars->gameDifficulty != Difficulty_Easy) || (enemyPos != 0)) {
          AddEnterObject(enemyType, 0);
        }

        enemyEntry <<= enemyBits;
        if ((++enemyPos & enemyMask) == 0x0) {
          enemyEntry = script->list[enemyPos / enemyBytes];
        }

        enemyType = (enemyEntry >> ((enemyBytes - 1) * enemyBits));
        if (gameVars->gamePhase >= phaseIceMachineStart) {
          if (enemyType == Object_None) {
            enemyType = Object_IceMachine;
          }
        }
      } while ((enemyType != Object_None) && (enemyPos < maxEnemies));

      if (gameVars->gameDifficulty == Difficulty_Hard) {
        enemyEntry = script->list[0];
        enemyType = (enemyEntry >> ((enemyBytes - 1) * enemyBits));

        AddEnterObject(enemyType, 0);
      }

      if (gameVars->gamePhase >= phaseIceMachineStart) {
        for (enemyPos = 0; enemyPos < numIceMachineStops; enemyPos++) {
          gameVars->iceMachineFill[enemyPos] = 0;
        }

        AddEnterObject(Object_IceMachine, 0);
      }

      if (gameVars->gamePhase >= phaseIcicleStart) {
        for (enemyPos = 0; enemyPos < numIcicleStarts; enemyPos++) {
          gameVars->icicleFill[enemyPos] = 0;
        }

        enemyPos = gameVars->gamePhase - phaseIcicleStart + 1;
        if (enemyPos > numIcicleObjects) {
          enemyPos = numIcicleObjects;
        }
        if (gameVars->gameDifficulty == Difficulty_Easy) {
          enemyPos = ((enemyPos + 1) / 2);
        }

        do {
          enemyPos--;
          AddEnterObject(Object_Icicle, (icicleTime * (enemyPos / 2)) + (icicleIntraTime * (enemyPos & 0x1)));
        } while (enemyPos);
      }

      ballTime = script->paintBallTime;
      if (gameVars->gameDifficulty == Difficulty_Easy) {
        ballTime += (ballTime / 2);
      } else if (gameVars->gameDifficulty == Difficulty_Hard) {
        ballTime -= (ballTime / 2);
      }
      AddEnterObject(Object_PaintBall, ballTime);
      ballTime = script->fireBallTime;
      if (gameVars->gameDifficulty == Difficulty_Easy) {
        ballTime += (ballTime / 2);
      } else if (gameVars->gameDifficulty == Difficulty_Hard) {
        ballTime -= (ballTime / 2);
      }
      AddEnterObject(Object_FireBall, ballTime);

      ballTime = delayGameTime;
      if (gameVars->gameDifficulty == Difficulty_Easy) {
        ballTime += (delayGameTime / 3);
      } else if (gameVars->gameDifficulty == Difficulty_Hard) {
        ballTime -= (delayGameTime / 3);
      }
      AddEvent(Event_DelayGame, ballTime, (void *)(NULL));
    }
  }

  InitializeObjects();

  for (bonusNum = 0; bonusNum < gameVars->totalObjects; bonusNum++) {
    object = &gameVars->objectList[bonusNum];
    object->num = gameVars->objectCount++;

    AddSprite(object);
  }
}

//
// Update Tiles
//
//   Draw all dirty tiles and uses an offset during unqiue periods of time (hitting POW and
//     display high score table)
//

void
UpdateTiles(void)
{
  register UInt16 i, x, y, deltaX, deltaY, delta, special, lineY;
  register Tile tile, tile2;
  register TilePtr tilePtr;

  if ((gameVars->flipFloorTile == PChar_POW)
      || (gameVars->phaseType == Phase_EnterHighScores)
      || (gameVars->phaseType == Phase_DisplayHighScores))
  {
    if (gameVars->flipFloorTile == PChar_POW)
    {
      special = deltaX = deltaY = lineY = 0;
      if (gameVars->powNum <= charYSize)
      {
        delta = gameVars->powNum;
      }
      else
      {
        delta = (2 * charYSize) - gameVars->powNum;
      }
    }
    else
    {
      if (gameVars->phaseType == Phase_DisplayHighScores)
      {
        lineY = (highScoreRankY - displayHighScoreDeltaY) * charYSize;
      }
      else
      {
        lineY = highScoreRankY * charYSize;
      }
      special = 1;
      delta = 0;
      deltaX = highScoreDeltaX1;
      deltaY = highScoreDeltaY1;
    }

    for (i = charUpdate, tilePtr = &gameVars->charArray[charUpdate];
         i-- > tileYMin;)
    {
      tile2 = (tile = *--tilePtr) & ~dirtyChar;
      if ((tile2 > PIPE_END) && (tile2 != PChar_POW))
      {
        *tilePtr = (tile = tile2);

        x = ((i % charXNum) * charXSize);
        y = ((i / charXNum) * charYSize);
        if (special != 0)
        {
          delta = 0;
          if (y >= lineY)
          {
            delta = deltaY;
            if (y == lineY)
            {
              x += deltaX;
            }
          }
        }

        if (delta != 0)
        {
          tile2 = (*(tilePtr + charXNum) & ~dirtyChar);
          if (tile2 <= PIPE_END)
          {
            if (y > topTile)
            {
              DrawChar(x, y, tile2, charYSize);
            }
            else
            {
              DrawChar(x, y, PChar_SPACE, charYSize);
            }
          }
        }

        if (y > topTile)
        {
          tile2 = (*(tilePtr - charXNum) & ~dirtyChar);
          if (tile2 <= PIPE_END)
          {
            *(tilePtr - charXNum) = tile2;
            if (delta != charYSize)
            {
              DrawChar(x, y - charYSize, tile2, charYSize - delta);
            }
          }
        }

        DrawChar(x, y - delta, tile, charYSize);
      }
      else if (tile & dirtyChar)
      {
        *tilePtr = tile2;

        x = ((i % charXNum) * charXSize);
        y = ((i / charXNum) * charYSize);

        DrawChar(x, y, tile2, charYSize);
      }
    }
  }
  else
  {
    for (i = charUpdate, tilePtr = &gameVars->charArray[charUpdate];
         i-- > tileYMin;)
    {
      tile = *--tilePtr;
      if (tile & dirtyChar)
      {
        *tilePtr = (tile &= ~dirtyChar);

        x = (i % charXNum) * charXSize;
        y = (i / charXNum) * charYSize;

        DrawChar(x, y, tile, charYSize);
      }
    }
  }
}

//
// Check Intersections
//
//   Check if an object's intersection flag is set and if so handle it. 
//     Plus draw sprites in new positions.
//

void
CheckIntersect(void)
{
  register UInt16 i, x, y, delta;
  register UInt32 intersect;
  register ObjElePtr objPtr;
  register ObjectPtr object, object2;

  objPtr = gameVars->spriteHead;
  while (objPtr != (ObjElePtr) (NULL)) {
    object = (objPtr->object);

    intersect = object->intersect;
    x = object->motion;
    if (intersect != 0) {
      if (object == &piperObject) {
        y = 0;
        do {
          if (intersect & 0x1) {
            object2 = &gameVars->objectList[y];

            delta = object2->flags;

            if ((delta & flagNotInGame) == 0) {
              i = object2->level;
              if (i == Level_Bonus) {
                ObjectBonus(object2);

                object2->deltaY = 0;
              } else if (((delta & flagFlipped) != 0) || (object2->motion == Motion_Flip)) {
                ObjectKick(object2);
              } else if ((i != Level_Misc) && (i != Level_Other)) {
                if (gameVars->platformTile == PChar_SPACE) {
                  if (i == Level_Inanimate) {
                    if (object2->type == Object_Icicle) {
                      if ((delta & flagWaking) == 0) {
                        gameVars->gameScore += pointsFlip;

                        ObjectReincarnate(object2);
                        break;
                      }
                    } else {
                      ObjectExtinquish(object2, 1);
                    }
                  } else {
                    x = object2->type;
                    object2->xPos = object2->xPrev;
                    object2->motion = Motion_Appear;
                    AddEvent(Event_Turn, biteTime, (void *)(object2));

                    if (x == Object_Turtle) {
                      changeSequence(object2, turtleBiteSequence, sizeof(turtleBiteSequence));
                    } else if (x == Object_Crab) {
                      changeSequence(object2, crabBiteSequence, sizeof(crabBiteSequence));
                    }
                  }

                  if (gameVars->phaseType == Phase_Normal) {
                    if (object->level == Level_Inanimate) {
                      PlaySound(Sound_Splud, Priority_Level_1);
                    } else {
                      PlaySound(Sound_Bite, Priority_Level_1);
                    }
                    ObjectDie(object, object2);
                    x = object->motion;
                  }
                  break;
                }
              }
            }
          }

          y++;
          intersect >>= 1;
        } while (intersect);
      } else {
        if ((x == Motion_Run) || (x == Motion_Bounce)) {
          object->xPos = object->xPrev;

          x = object->type;
          if ((x == Object_Turtle) || (x == Object_Crab)) {
            object->motion = Motion_Appear;
            AddEvent(Event_Turn, turnTime, (void *)(object));

            if (x == Object_Turtle) {
              changeSequence(object, turtleTurnSequence, sizeof(turtleTurnSequence));
            } else {
              changeSequence(object, crabTurnSequence, sizeof(crabTurnSequence));
            }
          } else {
            object->flags ^= flagDirectionX;
            object->deltaX = -object->deltaX;
          }
        }
      }
    }

    y = object->flags;
    if ((x != Motion_Stop) || ((y & flagDirty) != 0)) {
      delta = (y & flagDirectionX);
      if ((y & flagRotating) != 0) {
        delta ^= rotationTable[(y & flagSprite) >> flagSpriteBits];

        if (object->draw.seqNum != object->prevSeqNum) {
          if ((y & flagDirectionX) != 0) {
            y = (y & ~flagSprite) + ((y + flagSpriteDeltaMinus) & flagSprite);
          } else {
            y = (y & ~flagSprite) + ((y + flagSpriteDeltaPlus) & flagSprite);
          }
        }

        if (((delta & spriteY) != 0) && (object->draw.ySkip != 0)) {
          object->draw.yPixels = objectHeight - object->draw.ySkip;
          object->draw.yStart = topScreen;
          object->draw.ySkip = 0;
        }
      }
      object->flags = y & ~flagDirty;

      if ((y & flagWaking) != 0) {
        i = object->type;
        if (i == Object_Turtle) {
          if (object->draw.sequence != turtleShellSequence) {
            register AnimationPtr theAnimate;
            register Int16 deltaX;

            theAnimate = AddAnimation();
            InitializeAnimation(theAnimate, Animation_Turtle_TShirt, object);

            theAnimate->draw.yPixels = animateHeight;

            if ((y & flagDirectionX) != 0) {
              deltaX = (Int16) (-(turtleTShirtDelta * speedFactor));
            } else {
              deltaX = (Int16) (turtleTShirtDelta * speedFactor);
            }

            changeSequence(object, turtleShellSequence, sizeof(turtleShellSequence));

            ChangeAnimationPos(theAnimate, deltaX, 0);
          }
        } else if (i != Object_Icicle) {
          delta |= spriteRemap;
        }
      }

      if (((x == Motion_Run) || (x == Motion_Bounce)) && (object->level == Level_Enemy)) {
        if (object->deltaX > 0) {
          object->flags &= ~flagDirectionX;
        } else {
          object->flags |= flagDirectionX;
        }
      }

      if ((y & flagInvisible) == 0) {
        DrawSprite(&object->draw, delta);
      }
    }

    objPtr = objPtr->next;
  }
}

//
// Fix Time
//
//   Fix all timers to use current time by adding elapsed time
//

void
FixTime(register UInt32 deltaTime)
{
  register UInt16 i;

  if (gameVars->phaseType != Phase_EnterHighScores) {
    gameVars->lastKickTime += deltaTime;
  } else {
    gameVars->lastKickTime = 0;
  }

  for (i = numPipes; i--;) {
    gameVars->lastPipeTime[i] += deltaTime;
  }

  for (i = gameVars->eventGameCount; i--;) {
    gameVars->eventList[i].time += deltaTime;
  }

  gameVars->timeEvent.origTime += deltaTime;
}

//
// Update World
//
//   Update the whole world - events, score, tiles, animations, sprites, intersections, and screen
//
//   Returns true if game is over
//

UInt16
UpdateWorld(void)
{
  register ObjElePtr objPtr;
  register ObjectPtr object;
  register UInt32 lastPeriodTime;

  lastPeriodTime = gameVars->nextPeriodTime;
  gameVars->nextPeriodTime = TimGetTicks() + gameVars->keyRepeatRate;

  if (gameVars->pauseTime == 0) {
    if ((gameVars->nextPeriodTime - lastPeriodTime) >= (2 * gameVars->keyRepeatRate)) {
      FixTime((gameVars->nextPeriodTime - lastPeriodTime) - gameVars->keyRepeatRate);
    }

    if ((gameVars->gameState & gameEnd) != 0) {
      InitializePhase();
    }

    UpdateEvents();

    if (gameVars->phaseType <= Phase_Other) {
      UpdateScore();
    }

    objPtr = gameVars->spriteHead;
    while (objPtr != (ObjElePtr) (NULL))
    {
      object = (objPtr->object);
      if ((object->flags & (flagWaitToEnter | flagInvisible)) == 0) {
        if (object->motion == Motion_Stop) {
          CheckDirtySprite(object);
        } else {
          object->flags |= flagDirty;
        }
      }
      objPtr = objPtr->next;
    }

    UpdateTiles();

    UpdateAnimations();

    if (gameVars->gameState & gameLast) {
      DestroyPhase();
      AddEvent(Event_EndPhase, endTime, (void *)(NULL));
    }

    objPtr = gameVars->spriteHead;
    while (objPtr != (ObjElePtr) (NULL)) {
      object = (objPtr->object);
      object->intersect = 0;
      if ((object->flags & (flagWaitToEnter | flagInvisible)) == 0) {
        UpdateSprite(object);
      }
      objPtr = objPtr->next;
    }

    ClearIntersect();

    objPtr = gameVars->spriteHead;
    while (objPtr != (ObjElePtr) (NULL)) {
      object = (objPtr->object);
      if ((object->flags & (flagEnter | flagNotInGame)) == 0) {
        UpdateSector(object);
      }
      objPtr = objPtr->next;
    }

    UpdateIntersect();

    CheckIntersect();

    UpdateScreen();
  }

  return gameVars->gameOver;
}

//
// Reset Highscores
//
//   Reset high score table back to initialize values
//

void
ResetHighScores(void)
{
  register UInt16 i, j;

  for (j = numDifficulty; j--;) {
    for (i = numHighScore; i--;) {
      StrCopy(&gameVars->highScoreStrTable[j][i][0], initialHighScoreStr[j][i]);
      gameVars->highScoreTable[j][i] = initHighScores[j][i];
    }
  }

  gameVars->internetHighScore = initInternetScore;
  gameVars->internetHighScorePhase = defaultStartPhase;
}

//
// Adjust Key List
//
//   Adjust pointers to key list arrays.
//

Boolean
AdjustKeyList(void)
{
  register UInt8 *keyList, *newKeyList;
  register UInt16 i;
  register Boolean notFound;

  static UInt8 *newKeyLists[numPresetKeyList] = {
    instruction1KeyList, instruction2KeyList, titleKeyList
  };

  keyList = gameVars->keyEvent.keyList;
  if (keyList != (UInt8 *)(NULL)) {
    notFound = true;
    for (i = numPresetKeyList; i--;) {
      newKeyList = newKeyLists[i];

      if (keyList == gameVars->preSetKeyList[i]) {
        gameVars->keyEvent.keyList = newKeyList;
        notFound = false;
      }
      gameVars->preSetKeyList[i] = newKeyList;
    }
  } else {
    notFound = false;
  }

  return notFound;
}

//
// Adjust Key Delay List
//
//   Adjust pointers to key delay arrays.
//

Boolean
AdjustKeyDelayList(void)
{
  register UInt8 *keyDelayList, *newKeyDelayList;
  register UInt16 i;
  register Boolean notFound;

  static UInt8 *newKeyDelayLists[numPresetKeyDelay] = {
    instruction1KeyDelayList, instruction2KeyDelayList,
    instruction3KeyDelayList, instruction4KeyDelayList, titleKeyDelayList
  };

  keyDelayList = gameVars->keyEvent.keyDelayList;
  if (keyDelayList != (UInt8 *)(NULL)) {
    notFound = true;
    for (i = numPresetKeyDelay; i--;) {
      newKeyDelayList = newKeyDelayLists[i];

      if (keyDelayList == gameVars->preSetKeyDelay[i]) {
        gameVars->keyEvent.keyDelayList = newKeyDelayList;
        notFound = false;
      }
      gameVars->preSetKeyDelay[i] = newKeyDelayList;
    }
  } else {
    notFound = false;
  }

  return notFound;
}

//
// Adjust Instruction
//
//   Adjust pointers to instruction events.
//

Boolean
AdjustInstruction(void)
{
  register InstructionEventPtr instruction, newInstruction;
  register UInt16 i;
  register Boolean notFound;

  static InstructionEventPtr newInstructions[numPresetInstruction] = {
    &instruction1EventList[0], &instruction1EventList[1], &instruction1EventList[2], 
    &instruction1EventList[3], &instruction1EventList[4],
    &instruction2EventList[0], &instruction2EventList[1], &instruction2EventList[2],
    &instruction2EventList[3], &instruction2EventList[4], &instruction2EventList[5],
    &instruction3EventList[0], &instruction3EventList[1], &instruction3EventList[2]
  };

  instruction = gameVars->instructionEvent;
  if (instruction != (InstructionEventPtr)(NULL)) {
    notFound = true;
    for (i = numPresetInstruction; i--;) {
      newInstruction = newInstructions[i];

      if (instruction == gameVars->preSetInstruction[i]) {
        gameVars->instructionEvent = newInstruction;
        notFound = false;
      }
      gameVars->preSetInstruction[i] = newInstruction;
    }
  } else {
    notFound = false;
  }

  return notFound;
}

//
// Adjust Sequence
//
//   Adjust pointers to sequence arrays (for sprites).
//

Boolean
AdjustSequence(UInt8 ** sequencePtr,
               register Boolean change)
{
  register UInt8  *sequence, *newSequence;
  register UInt16  i;
  register Boolean notFound;

  static UInt8 *newSequences[numPresetSequences] = {
    piperRunSequence, piperStopSequence, piperJumpSequence,
    piperSlideSequence, piperFlapSequence, piperDieSequence,
    piperBiteSequence, piperSplatSequence, turtleSequence,
    turtleFlipSequence, turtleRotationSequence, turtleShellSequence,
    turtleBiteSequence, turtleTurnSequence, crabSequence,
    crabHitSequence, crabFlipSequence, crabRotationSequence,
    crabBiteSequence, crabTurnSequence, fighterFlySequence,
    fighterFlyFlipSequence, fighterFlyRotationSequence, coinSequence,
    coinCollectSequence, coinDieSequence, iceMachineSequence,
    iceMachineBreakSequence, fireBallSequence, fireBallAppearSequence,
    fireBallDisappearSequence, paintBallSequence, paintBallAppearSequence,
    paintBallDisappearSequence, icicleSequence, icicleDripSequence,
    powSequence
  };

  sequence = *sequencePtr;
  if (sequence != (UInt8 *)(NULL)) {
    notFound = true;
    for (i = numPresetSequences; i--;) {
      newSequence = newSequences[i];

      if (sequence == gameVars->preSetSequence[i]) {
        *sequencePtr = newSequence;
        notFound = false;
      }
      if (change) {
        gameVars->preSetSequence[i] = newSequence;
      }
    }
  } else {
    notFound = false;
  }

  return notFound;
}

//
// Initialize Run Globals
//
//   Initialize the pointer to the runtime variables
//

void
InitializeRunGlobals(register RunVariablesPtr globals)
{
  runVars = globals;
}

//
// Initialize World
//
//   Initialize world during first time game is run or reset.
//

void
InitializeWorld(register PreferencesType *prefs, register UInt16 ticks, register Boolean reset)
{
  register UInt16 i;

  gameVars->gamePhase = phaseTitle - 1;
  gameVars->gameSize = false;

  gameVars->keyRepeatRate = ticks;
  gameVars->pauseTime = 0;

  if (!CHECK_SIGNATURE(prefs)) {
#ifdef ENABLE_CHEATS
    gameVars->gameOverPhase = maxPhaseNum;
#else
    gameVars->gameOverPhase = maxDemoPhaseNum;
#endif
    gameVars->nextTilePhase = phaseTitle - 1;
  } else {
    gameVars->gameOverPhase = maxPhaseNum;
    gameVars->nextTilePhase = phaseDisplayHighScore - 1;
  }

  gameVars->initExtraLife = defaultPointsExtraLife;
  gameVars->initNumLives = defaultNumLives;
  gameVars->initStartPhase = defaultStartPhase;
  gameVars->initDifficulty = gameVars->gameDifficulty = Difficulty_Arcade;

  gameVars->gamePlaying = false;
  gameVars->gamePaused = false;
  gameVars->gameWait = false;
  gameVars->gameAnimationCount = 0;

  if (reset) {
    ResetHighScores();
  }

  gameVars->keyEvent.keyList = (UInt8 *)(NULL);
  gameVars->keyEvent.keyDelayList = (UInt8 *)(NULL);
  gameVars->instructionEvent = (InstructionEventPtr)(NULL);
  for (i = maxNumAnimations; i--;) {
    gameVars->animateList[i].draw.sequence = (UInt8 *)(NULL);
  }
  for (i = maxNumObjects; i--;) {
    gameVars->objectList[i].draw.sequence = (UInt8 *)(NULL);
  }

  InitializeGame();
  InitializePhase();
}

//
// Update Character's Direction
//
//   Update Maryo's direction, sprite and animation (take into account friction, sliding and dropping off)
//

void
UpdateMaryo(register UInt32 state)
{
  register UInt16 motion;
  register Tile tile;

  if (((piperObject.flags & flagNotInGame) != 0) || (gameVars->enemyCount == 0)) {
    return;
  }

  motion = piperObject.motion;
  if (state & (moveLeft | moveRight | moveJump1 | moveJump2)) {
    if ((motion == Motion_Run) || (motion == Motion_Slide) || (motion == Motion_Stop)) {
      if (gameVars->slideNum == 0) {
        if (state & moveLeft) {                                        // Move Left
          if ((motion == Motion_Run) && ((piperObject.flags & flagDirectionX) == 0)) {
            if (piperObject.deltaX == (Int8) (-piperSpeedX)) {
              ChangeObjectSequence(&piperObject, piperSlideSequence, sizeof(piperSlideSequence), Motion_Slide);
              tile = CheckAny(&piperObject, piperObject.yPos + (piperObject.draw).height + 1, 0, checkAll);
              gameVars->tileFriction = defaultFriction;
              if ((tile == PChar_FLOOR_ICE) || (tile == PChar_FLOOR_INVIS) || (tile == PChar_FLOOR_SEMI_INVIS)) {
                gameVars->tileFriction = iceFriction;
              }
              piperObject.deltaX = (Int8) (-slideSpeedX);
            }
            gameVars->slideNum = (slideDistance * gameVars->tileFriction) + slideDeacceleration;
          } else if ((motion != Motion_Slide) && (motion != Motion_Jump)) {
            if (piperObject.deltaX > (Int8) (-piperSpeedX)) {
              piperObject.deltaX -= piperAccelerationX;
              if (piperObject.deltaX < (Int8) (-piperSpeedX)) {
                piperObject.deltaX = (Int8) (-piperSpeedX);
              }
            }
            piperObject.flags |= flagDirectionX;
            ChangeObjectSequence(&piperObject, piperRunSequence, sizeof(piperRunSequence), Motion_Run);
            gameVars->tileFriction = defaultFriction;
          }
        } else if (state & moveRight) {                                        // Move Right
          if ((motion == Motion_Run) && ((piperObject.flags & flagDirectionX) != 0)) {
            if (piperObject.deltaX == (Int8) (piperSpeedX)) {
              ChangeObjectSequence(&piperObject, piperSlideSequence, sizeof(piperSlideSequence), Motion_Slide);
              tile = CheckAny(&piperObject, piperObject.yPos + (piperObject.draw).height + 1, 0, checkAll);
              gameVars->tileFriction = defaultFriction;
              if ((tile == PChar_FLOOR_ICE) || (tile == PChar_FLOOR_INVIS) || (tile == PChar_FLOOR_SEMI_INVIS)) {
                gameVars->tileFriction = iceFriction;
              }
              piperObject.deltaX = (Int8) (slideSpeedX);
            }
            gameVars->slideNum = (slideDistance * gameVars->tileFriction) + slideDeacceleration;
          } else if ((motion != Motion_Slide) && (motion != Motion_Jump)) {
            if (piperObject.deltaX < (Int8) (piperSpeedX)) {
              piperObject.deltaX += piperAccelerationX;
              if (piperObject.deltaX > (Int8) (piperSpeedX)) {
                piperObject.deltaX = (Int8) (piperSpeedX);
              }
            }
            piperObject.flags &= ~flagDirectionX;
            ChangeObjectSequence(&piperObject, piperRunSequence, sizeof(piperRunSequence), Motion_Run);
            gameVars->tileFriction = defaultFriction;
          }
        }
      }

      if (state & (moveJump1 | moveJump2)) {                                          // Jump
        if ((piperObject.flags & flagDirectionX) != 0) {
          if (state & moveRight) {
            if (piperObject.deltaX < 0) {
              piperObject.deltaX += piperAccelerationX;
            }
            if (piperObject.deltaX == 0) {
              piperObject.flags &= ~flagDirectionX;
              gameVars->slideNum = 0;
            }
          }
        } else {
          if (state & moveLeft) {
            if (piperObject.deltaX > 0) {
              piperObject.deltaX -= piperAccelerationX;
            }
            if (piperObject.deltaX == 0) {
              piperObject.flags |= flagDirectionX;
              gameVars->slideNum = 0;
            }
          }
        }

        ChangeObjectSequence(&piperObject, piperJumpSequence, sizeof(piperJumpSequence), Motion_Jump);
        piperObject.deltaY = 0;
      }
    }
  } else if ((gameVars->slideNum == 0) && (motion == Motion_Run)) {
    if ((piperObject.deltaX == (Int8) (piperSpeedX)) || (piperObject.deltaX == (Int8) (-piperSpeedX))) {
      ChangeObjectSequence(&piperObject, piperSlideSequence, sizeof(piperSlideSequence), Motion_Slide);
      tile = CheckAny(&piperObject, piperObject.yPos + (piperObject.draw).height + 1, 0, checkAll);
      gameVars->tileFriction = defaultFriction;
      if ((tile == PChar_FLOOR_ICE) || (tile == PChar_FLOOR_INVIS) || (tile == PChar_FLOOR_SEMI_INVIS)) {
        gameVars->tileFriction = iceFriction;
      }
      if (piperObject.deltaX == (Int8) (piperSpeedX)) {
        piperObject.deltaX = (Int8) (slideSpeedX);
      } else {
        piperObject.deltaX = (Int8) (-slideSpeedX);
      }
    }
    gameVars->slideNum = (slideDistance * gameVars->tileFriction) + slideDeacceleration;
  }

  motion = piperObject.motion;
  if ((gameVars->slideNum != 0) && ((motion == Motion_Run) || (motion == Motion_Slide) || (motion == Motion_Stop))) {
    gameVars->slideNum -= slideDeacceleration;
    if (gameVars->slideNum <= 0) {
      gameVars->slideNum = 0;
      piperObject.deltaX = piperObject.deltaY = 0;
      ChangeObjectSequence(&piperObject, piperStopSequence, sizeof(piperStopSequence), Motion_Stop);
      CheckDropOff(&piperObject, piperObject.yPos);
      gameVars->tileFriction = defaultFriction;
    }
  }
}

//
// Update Game Stylus
//
//   Translate the stylus input to a game key based on Maryo's position
//
//   Return the translate key input.
//

UInt32
UpdateGameStylus(register PreferencesType * prefs,
                 register Coord x,
                 register Coord y)
{
  register UInt32 keys;
  register Int16 xDiff;

  keys = 0;

  xDiff = ((Int16) (x) - ((piperObject.xPos / speedFactor) + (piperWidth / 2)));
  if ((xDiff > (piperWidth / 2)) || (xDiff < -(piperWidth / 2))) {
    if (xDiff < -(screenWidth / 2)) {
      xDiff += screenWidth;
    } else if (xDiff > (screenWidth / 2)) {
      xDiff -= screenWidth;
    }

    if (xDiff < 0) {
      keys |= prefs->config.ctlKeyLeft;
    } else {
      keys |= prefs->config.ctlKeyRight;
    }
  }

  y += (topScreen - SCREEN_START);
  if (y < piperObject.yPos) {
    keys |= prefs->config.ctlKeyJump;
  }

  return keys;
}

//
// Update Movement
//
//   Translate configured keys to game keys.  Get current key from automatic movement
//     arrays (if phase is the correct type).  Update Maryo with these keys or the movement
//     of the box in the high score entry phase.
//

void
UpdateMovement(register PreferencesType * prefs,
               register UInt32 keyStatus)
{
  UInt32 keyState;

  keyState = 0;
  if (keyStatus & prefs->config.ctlKeyJump) {
    keyState |= (moveJump1 | moveJump2);
  }
  if (keyStatus & prefs->config.ctlKeyLeft) {
    keyState |= moveLeft;
  }
  if (keyStatus & prefs->config.ctlKeyRight) {
    keyState |= moveRight;
  }

  if (gameVars->phaseType == Phase_Instructions) {
    if (keyState != 0) {
      AddEvent(Event_EndPhase, 0, (void *)(NULL));
    }
    keyState = gameVars->currentKey;
  } else if (gameVars->phaseType >= Phase_DisplayHighScores) {
    if (gameVars->phaseType == Phase_Title) {
      keyState = gameVars->currentKey;
    }
  }

#ifdef ENABLE_CHEATS
  if ((KeyCurrentState() & keyBitPageUp) != 0) {
    AddEvent(Event_EndPhase, 75, (void *)(NULL));
  }
#endif

  if ((gameVars->phaseType <= Phase_Other) || (gameVars->phaseType == Phase_Title)) {
    UpdateMaryo(keyState);
  } else if (gameVars->phaseType == Phase_EnterHighScores) {
    UpdateHighScore(keyState);
  }
}

//
// Draw Pause
//
//   Draw pause text directly onto offscreen buffer (NOT into tile array)
//

void
DrawPause(void)
{
  DrawText(pauseStr, pauseX, pauseY, 1);
}

//
// Pause or Unpause Game
//
//   Pause or unpause the game, only if not done so already
//

void
PauseGame(register UInt16 pause)
{
	register UInt32 deltaTime;

  if (pause) {
    if (gameVars->pauseTime == 0) {
      gameVars->pauseTime = gameVars->nextPeriodTime;

      DrawPause();

      UpdateScreen();
      GraphicsRepaint();
    }
  } else {
    if (gameVars->pauseTime != 0) {
			deltaTime = gameVars->nextPeriodTime - gameVars->pauseTime;
			FixTime(deltaTime);

      gameVars->pauseTime = 0;

      EraseText(pauseLen, pauseX, pauseY, 1);
    }
  }
}

//
// Start Game
//
//   Add start game event to queue
//

void
StartGame(void)
{
  gameVars->gameSize = true;
  gameVars->gamePhase = gameVars->initStartPhase;
  gameVars->gameState |= gameEnd;
  gameVars->gameDifficulty = gameVars->initDifficulty;

  gameVars->updateRect.y1 = topDisplay;
  gameVars->updateRect.y2 = bottomDisplay - 1;

  AddEvent(Event_EndPhase, 0, (void *)(NULL));
}

//
// Terminate Game
//
//   Add terminate game event to queue
//

void
TerminateGame(void)
{
  gameVars->gameSize = false;
  gameVars->gamePhase = phaseTitle - 1;
  gameVars->gameOver = false;
  gameVars->gameState |= gameEnd;

  runVars->music.playing = false;

  PauseGame(0);

  gameVars->updateRect.y1 = topDisplay;
  gameVars->updateRect.y2 = bottomTitleScreen - 1;

  AddEvent(Event_EndPhase, 0, (void *)(NULL));
}

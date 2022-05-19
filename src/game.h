
/*
 * @(#)game.h
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

#ifndef _GAME_H
#define _GAME_H

#include "palm.h"

enum moveType {
  moveNone = 0,
  moveLeft,
  moveRight,
  moveUp,
  moveDown,
  moveJump,
  moveFire
};

extern void GameInitialize()                                          __GAME__;
extern void GameCheckStartButton(void)                                __GAME__;
extern void GameResetPreferences(PreferencesType *)                   __GAME__;
extern void GameProcessKeyInput(PreferencesType *, UInt32)            __GAME__;
extern void GameProcessStylusInput(PreferencesType *, Boolean, Coord, Coord) __GAME__;
extern void GameMovement(PreferencesType *)                           __GAME__;
extern void GameDraw(PreferencesType *)                               __GAME__;
extern void GameTerminate()                                           __GAME__;
extern void GameMusicPlayback()                                       __GAME__;

#endif

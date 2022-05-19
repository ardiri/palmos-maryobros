/*
 * @(#)MBroDraw.h
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
// Maryo Bros - Drawing, Animation and Intersection Routines
//
// Written by Chip Kerchner
//

//
// Function prototypes
//

// Animation routines
extern void InitializeAnimation(register AnimationPtr theAnimate,
                                register UInt16 type,
                                register ObjectPtr object) __DEVICE__;
extern AnimationPtr AddAnimation(void) __DEVICE__;
extern void DeleteAnimation(register AnimationPtr theAnimate) __DEVICE__;
extern void UpdateAnimations(void) __DEVICE__;
extern AnimationPtr NewAnimation(void) __DEVICE__;
extern void InitializeAnimations(void) __DEVICE__;
extern void DeleteAnimationType(register UInt16 type) __DEVICE__;
extern void DeleteAnimationObject(register ObjectPtr object)  __DEVICE__;
extern void ChangeAnimationPos(register AnimationPtr theAnimate,
                               register Int16 deltaX,
                               register Int16 deltaY) __DEVICE__;
extern Boolean AdjustAnimation(UInt8 ** animationPtr,
                               register Boolean change) __DEVICE__;
extern Boolean InitializeGameGlobals(register GameVariablesPtr globals) __DEVICE__;

// Intersection routines
extern void ClearIntersect(void) __GAME__;
extern void UpdateIntersect(void) __GAME__;
extern void UpdateSector(register ObjectPtr object) __GAME__;
extern void SectorIntersect(register UInt16 num) __GAME__;
extern void ObjectFlipStart(register ObjectPtr object) __GAME__;

// Drawing routines
extern void DrawChar(register UInt16 x,
                     register Int16 y,
                     register Tile which,
                     register Int16 height) __DEVICE__;
extern void DrawSprite(register DrawerPtr draw,
                       register UInt16 flags) __DEVICE__;

// Initialization routines
extern void InitializeDraw(void) __DEVICE__;
extern Boolean InitializeDrawTables(void) __DEVICE__;
extern Boolean InitializeDrawers(void) __DEVICE__;
extern void ClearScreen(void) __DEVICE__;
extern void ClearLastLine(void) __DEVICE__;

// Destruction routines
extern void DestroyDraw(void) __DEVICE__;

// Misc routines
extern void CheckDirtySprite(register ObjectPtr object) __GAME__;
extern Int16 CheckDrop(register ObjectPtr object,
                       register Int16 y,
                       register Int16 delta,
                       register UInt16 dir) __GAME__;
extern Tile CheckAny(register ObjectPtr object,
                     register Int16 y,
                     register Int16 delta,
                     register UInt16 dir) __GAME__;
extern Int16 CheckDropOn(register ObjectPtr object,
                         register Int16 y,
                         register Int16 delta) __GAME__;
extern void CheckDropOff(register ObjectPtr object,
                         register Int16 y) __GAME__;
extern UInt16 CheckRicochetOff(register ObjectPtr object,
                               register Int16 y,
                               register Int16 delta,
                               register UInt16 dir) __GAME__;
extern Int16 CheckRicochet(register ObjectPtr object,
                           register Int16 y,
                           register Int16 delta) __GAME__;

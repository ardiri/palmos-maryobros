
/*
 * @(#)palm.h
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

#ifndef _PALM_H
#define _PALM_H

// system includes
#include <PalmOS.h>
#include <System/DLServer.h>

// resource "include" :P
#include "resource.h"

// special includes (additional hardware etc)
#include "hardware/GPDLib.h"
#include "hardware/HanderaVGA.h"

#include "MBroScrn.h"
#include "MBroType.h"

// application constants and structures
#define appCreator 'MBRO'
#define __REGISTER__ __attribute__ ((section ("register")))     // code0002.bin
#define __DEVICE__   __attribute__ ((section ("device")))       // code0003.bin
#define __GAME__     __attribute__ ((section ("game")))         // code0004.bin
#define __HELP__     __attribute__ ((section ("help")))         // code0005.bin
#define __SAFE0001__ __attribute__ ((section ("safe0001")))     // code0006.bin
#define __SAFE0002__ __attribute__ ((section ("safe0002")))     // code0007.bin

#define ftrGlobals             1000
#define ftrGlobalsCfgActiveVol 1001
#define ftrDeviceGlobals       2000
#define ftrGraphicsGlobals     3000
#define ftrGameGlobals         4000
#define ftrHelpGlobals         5000
#define ftrRegisterGlobals     1005 // <-- must be, as original code0007.bin 
//#define ftrRegisterGlobals   6000

#define GAME_FPS           125              // 12.5 frames per second
#define MUSIC_FPS          GAME_FPS         // 12.5 frames per second
#define MUSIC_TIME         ((1000 * 10) / MUSIC_FPS)    // 80 ms
#define VERSION            0

#define SCREEN_WIDTH       160
#define SCREEN_HEIGHT      132       // 160x132 visible display area (game)
#define SCREEN_START       14        // 1st visible game line

typedef struct {

  struct {

    UInt8       signatureVersion;    // a preference version number
    Char        signature[16];       // a "signature" for decryption
    Char        *hotSyncUsername;    // the HotSync user name of the user

  } system;

  struct {

    UInt16      ctlKeyJump;          // key definition for jump
    UInt16      ctlKeyLeft;          // key definition for move left
    UInt16      ctlKeyRight;         // key definition for move right

    Boolean     sndMute;             // sound is muted?
    UInt16      sndVolume;           // the volume setting for sound

    UInt8       lgray;               // the light gray configuration setting
    UInt8       dgray;               // the dark gray configuration setting

  } config;

  UInt32        stylusKeys;          // keys created from stylus input

  GameVariables gameVariables;       // game variables

} PreferencesType;

// this is our 'double check' for decryption - make sure it worked :P
#define CHECK_SIGNATURE(x) (StrCompare(x->system.signature, "|HaCkMe|") == 0)

// local includes
#include "device.h"
#include "graphics.h"
#include "help.h"
#include "game.h"
#include "register.h"
#include "gccfix.h"

// functions
extern UInt32  PilotMain(UInt16, MemPtr, UInt16);
extern void    InitApplication(void);
extern Boolean ApplicationHandleEvent(EventType *);
extern void    ApplicationDisplayDialog(UInt16);
extern void    EventLoop(void);
extern void    EndApplication(void);

#endif

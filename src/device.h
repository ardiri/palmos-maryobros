
/*
 * @(#)device.h
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

#ifndef _DEVICE_H
#define _DEVICE_H

#include "palm.h"

#define romVersion1   sysMakeROMVersion(1,0,0,sysROMStageRelease,0)
#define romVersion2   sysMakeROMVersion(2,0,0,sysROMStageRelease,0)
#define romVersion3   sysMakeROMVersion(3,0,0,sysROMStageRelease,0)
#define romVersion3_1 sysMakeROMVersion(3,1,0,sysROMStageRelease,0)
#define romVersion3_2 sysMakeROMVersion(3,2,0,sysROMStageRelease,0)
#define romVersion3_5 sysMakeROMVersion(3,5,0,sysROMStageRelease,0)

#define sysFtrNumProcessorVZ 0x00030000

enum {
  graySet = 0,
  grayGet
};

extern Boolean DeviceCheckCompatability()                           __DEVICE__;
extern void    DeviceInitialize()                                   __DEVICE__;
extern Boolean DeviceSupportsGrayscale()                            __DEVICE__;
extern void    DeviceGrayscale(UInt16, UInt8 *, UInt8 *)            __DEVICE__;
extern void    DevicePlaySound(SndCommandType *)                    __DEVICE__;
extern void    DeviceSetVolume(UInt16)                              __DEVICE__;
extern UInt16  DeviceGetVolume()                                    __DEVICE__;
extern void    DeviceSetMute(Boolean)                               __DEVICE__;
extern Boolean DeviceGetMute()                                      __DEVICE__;
extern UInt32  DeviceGetSupportedDepths()                           __DEVICE__;
extern Boolean DeviceSupportsVersion(UInt32)                        __DEVICE__;
extern void    *DeviceWindowGetPointer(WinHandle)                   __DEVICE__;
extern void    DeviceTerminate()                                    __DEVICE__;
extern Boolean DeviceSupportsColor()                                __DEVICE__;
extern void    DeviceSetUpdate(UInt8 y1, UInt8 y2)                  __DEVICE__;

#endif

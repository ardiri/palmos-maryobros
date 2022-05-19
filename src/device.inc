
/*
 * @(#)device.inc
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

// globals variable structure
typedef struct {

  UInt32  romVersion;           // the rom version of the device

  UInt32  depthState;           // the screen depth state (old)
  Boolean colorState;           // the screen color state

  UInt16  deviceVolume;         // the volume of the game on device
  Boolean deviceMuted;          // the volume muting status

} DeviceGlobals;

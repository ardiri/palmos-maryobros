/*
 * @(#)MBroPSt2.h
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
// Maryo Bros - Pre-Initialized Global Variables
//
// Written by Chip Kerchner
//

//
// Preset Variables
//

// Animation Sequence Variables
UInt8 pipeBendSequenceTwo[] = {
  0, 0, 0, 1, 1, 1, 2, 2, 2
};
UInt8 pipeBendSequenceThree[] = {
  0, 0, 1, 1, 2, 2
};
UInt8 pipeBendSequenceFour[] = {
  0, 0, 1, 1, 2
};
UInt8 pipeBendSequenceFive[] = {
  0, 1, 1, 2
};

UInt8 floor1BendSequence[] = {
  3, 4, 5, 5, 4, 3
};
UInt8 floor1EndBendSequence[] = {
  6, 7, 8, 8, 7, 6
};

UInt8 floor2BendSequence[] = {
  9, 10, 11, 11, 10, 9
};
UInt8 floor2EndBendSequence[] = {
  12, 13, 14, 14, 13, 12
};

UInt8 floor3BendSequence[] = {
  15, 16, 17, 17, 16, 15
};
UInt8 floor3EndBendSequence[] = {
  18, 19, 20, 20, 19, 18
};

UInt8 floor4BendSequence[] = {
  21, 22, 23, 23, 22, 21
};
UInt8 floor4EndBendSequence[] = {
  24, 25, 26, 26, 25, 24
};

UInt8 floorIceBendSequence[] = {
  27, 28, 29, 29, 28, 27
};
UInt8 floorIceEndBendSequence[] = {
  30, 31, 32, 32, 31, 30
};

UInt8 floorPlatformSequence[] = {
  33, 33
};

UInt8 splashSequence[] = {
  34, 35, 36, 37, 38
};

UInt8 turtleTShirtSequence[] = {
  39, 39, 39, 39, 39, 39, 39, 39, 39, 40, 40, 40, 41, 41, 41, 42, 42
};

UInt8 points800Sequence[] = {
  43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43, 43
};
UInt8 points1600Sequence[] = {
  44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44
};
UInt8 points2400Sequence[] = {
  45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45
};
UInt8 points3200Sequence[] = {
  46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46
};
UInt8 points4000Sequence[] = {
  47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47, 47
};
UInt8 points4800Sequence[] = {
  48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48
};
UInt8 points200Sequence[] = {
  49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49
};
UInt8 points500Sequence[] = {
  50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50
};
UInt8 points1000Sequence[] = {
  51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51
};

UInt8 bonusCoin1Sequence[] = {
  52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52
};
UInt8 bonusCoin2Sequence[] = {
  53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53
};

UInt8 waterfall1Sequence[] = {
  54, 55
};
UInt8 waterfall2Sequence[] = {
  56, 57
};

UInt8 iceMachineMeltSequence[] = {
  58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58, 58,
  59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59,
  60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60
};

Int8 turtleTShirtMovement[] = {
  0, -1, 2, -1, 2, -1, 2, -1, 2, -1, 2, 1, 2, 1, 2, 1, -2, 1, -2, 1, -2, 0,
  -2, 0, -2, 0, -2, 0, -2, 0, 0, 0, 0, 0
};

// Table to Swap Colors
#ifdef USE_16_COLOR_GRAYSCALE
static UInt8 colorSwapTable[] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x0b, 0x07, 0x08, 0x09, 0x0a, 0x06,
    0x0c, 0x0d, 0x0e, 0x0f
};
#else
static UInt8 colorSwapTable[] = {
  0x00, 0x02, 0x01, 0x03
};
#endif

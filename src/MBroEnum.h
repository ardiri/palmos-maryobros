/*
 * @(#)MBroEnum.h
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
// Maryo Bros - Enumerations
//
// Written by Chip Kerchner
//

//
// Enumerations
//

// Sprite Motion enumerations
enum {
  Motion_Stop = 0,              // deltaX = 0,       deltaY = 0
  Motion_Run,                   // deltaX = +x/-x,   deltaY = 0
  Motion_Jump,                  // deltaX = +x/-x/0, deltaY = Full Parabula
  Motion_Fall,                  // deltaX = +x/-x/0, deltaY = Half Parabula 1
  Motion_Drop,                  // deltaX = 0,       deltaY = Half Parabula 1
  Motion_Slide,                 // deltaX = +x/-x,   deltaY = 0
  Motion_Flip,                  // deltaX = +x/-x/0, deltaY = Full Mini-Parabula 1
  Motion_Bounce,                // deltaX = +x/-x,   deltaY = Full Mini-Parabula 2
  Motion_Hit,                   // deltaX = +x/-x/0, deltaY = Full Mini-Parabula 3
  Motion_Ricochet,              // deltaX = +x/-x,   deltaY = +y/-y
  Motion_Die,                   // deltaX = +x/-x/0, deltaY = Half Parabula 2
  Motion_Wave,                  // deltaX = +x/-x,   deltaY = 0/Full Mini-Parabula 4
  Motion_Spin,                  // deltaX = 0,       deltaY = 0
  Motion_Revive,                // deltaX = 0,       deltaY = +constant
  Motion_Flap,                  // deltaX = 0,       deltaY = Full Mini-Parabula 5
  Motion_Surprise,              // deltaX = 0,       deltaY = 0
  Motion_Appear,                // deltaX = miscX,   deltaY = miscY (Hold at Stop)
};

// Sprite Level enumerations
enum {
  Level_Other = 0,              // Flip
  Level_Misc,                   // Pow
  Level_Character,              // Piper
  Level_Bonus,                  // Coin
  Level_Enemy,                  // Turtle, Crab, Fighter Fly, Ice Machine
  Level_Inanimate,              // Fire Ball, Paint Ball, Icicle
};

// Event enumerations
enum {
  Event_Text = 0,               // Text
  Event_Entrance,               // Entrance
  Event_FlipBack,               // Flip Back
  Event_Wakeup,                 // Wake Up
  Event_EndPhase,               // End Phase
  Event_Timer,                  // Timer
  Event_Drop,                   // Drop
  Event_Key,                    // Key
  Event_TileChange,             // Tile Change
  Event_Melt,                   // Melt
  Event_Freeze,                 // Freeze
  Event_Instructions,           // Instructions
  Event_Coin,                   // Coin
  Event_Revive,                 // Revive
  Event_Platform,               // Platform
  Event_Invisible,              // Invisible
  Event_Appear,                 // Appear
  Event_Disappear,              // Disappear
  Event_Remove,                 // Remove
  Event_Animation,              // Animation
  Event_Turn,                   // Turn
  Event_DelayGame,              // Delay of Game
  Event_Found,                  // Found
};

// Object Type enumerations
enum {
  Object_None = 0,              // None
  Object_Turtle,                // Turtle
  Object_Crab,                  // Crab
  Object_FighterFly,            // Fighter Fly
  Object_IceMachine,            // Ice Machine
  Object_FireBall,              // Fire Ball
  Object_PaintBall,             // Paint Ball
  Object_Icicle,                // Icicle
  Object_Coin,                  // Coin
  Object_Character,             // Piper
  Object_Flip,                  // Flip
  Object_Pow,                   // Pow
};

// Animation enumerations
enum {
  Animation_Pipe_Bend = 0,      // Pipe Entrance Bend
  Animation_Floor1_Bend,        // Floor1 Bend
  Animation_Floor1_End_Bend,    // Floor1 End Bend
  Animation_Floor2_Bend,        // Floor2 Bend
  Animation_Floor2_End_Bend,    // Floor2 End Bend
  Animation_Floor3_Bend,        // Floor3 Bend
  Animation_Floor3_End_Bend,    // Floor3 End Bend
  Animation_Floor4_Bend,        // Floor4 Bend
  Animation_Floor4_End_Bend,    // Floor4 End Bend
  Animation_Floor_Ice_Bend,     // Floor Ice Bend
  Animation_Floor_Ice_End_Bend, // Floor1 Ice End Bend
  Animation_Platform,           // Platform Descending
  Animation_Splash,             // Splash
  Animation_Turtle_TShirt,      // Turtle T-Shirt
  Animation_Points_800,         // Points  800
  Animation_Points_1600,        // Points 1600
  Animation_Points_2400,        // Points 2400
  Animation_Points_3200,        // Points 3200
  Animation_Points_4000,        // Points 4000
  Animation_Points_4800,        // Points 4800
  Animation_Points_200,         // Points  200
  Animation_Points_500,         // Points  500
  Animation_Points_1000,        // Points 1000
  Animation_Bonus_Coin_1,       // Bonus Coin 1
  Animation_Bonus_Coin_2,       // Bonus Coin 1
  Animation_Waterfall_1,        // Waterfall 1
  Animation_Waterfall_2,        // Waterfall 2
  Animation_IceMachine_Melt,    // Ice Machine Melt
};

// Phase enumerations
enum {
  Phase_Normal = 0,             // Normal
  Phase_Bonus,                  // Bonus
  Phase_Instructions,           // Instructions
  Phase_Total,                  // Total
  Phase_Other,                  // Other
  Phase_EnterHighScores,        // Enter High Scores
  Phase_DisplayHighScores,      // Display High Scores
  Phase_Title,                  // Title
};

// Instruction enumerations
enum {
  Instruction_Turtle = 0,       // Turtle
  Instruction_Crab,             // Crab
  Instruction_FighterFly,       // Fighter Fly
  Instruction_IceMachine,       // Ice Machine
};

// Color Mode enumerations
enum {
  Color_Normal = 0,             // Normal
  Color_Swap,                   // Swap Background
};

// Bonus Phase enumerations
enum {
  Bonus_Num1 = 0,
  Bonus_Num2,
  Bonus_Num3,
};

// Color Table enumerations
enum {
  ColorTable_AlphaNum = 0,
  ColorTable_Brick,
  ColorTable_Pipe,
  ColorTable_Floor1,
  ColorTable_Floor2,
  ColorTable_Floor3,
  ColorTable_Ice,
  ColorTable_Water,
  ColorTable_Turtle,
  ColorTable_Bonus,
  ColorTable_FireBall,
  ColorTable_PaintBall,
  ColorTable_Coin,
  ColorTable_IceMachine,
  ColorTable_Crab,
  ColorTable_FighterFly,
  ColorTable_Maryo,
  ColorTable_Total
};

// Sound enumerations
enum {
  Sound_Hit = 0,
  Sound_Coin,
  Sound_Jump,
  Sound_Kick,
  Sound_Flip,
  Sound_Pow,
  Sound_Bite,
  Sound_Splud,
  Sound_Timer,
  Sound_Kick_Last,
  Sound_Die,
  Sound_Kill,
  Sound_Revive,
  Sound_Drop,
  Sound_Phase_Start,
  Sound_Collect_Bonus,
  Sound_Bonus_Perfect,
  Sound_Game_Over,
  Sound_Turtle_Start,
  Sound_Crab_Start,
  Sound_FighterFly_Start,
  Sound_Turtle_Appear,
  Sound_Crab_Appear,
  Sound_FighterFly_Appear,
  Sound_PaintBall_Appear,
  Sound_IceMachine_Appear,
  Sound_FireBall,
  Sound_IceMachine_Freeze,
  Sound_PaintBall_Ricochet,
  Sound_Extra_Life,
};

// Sound Priority enumerations
enum {
  Priority_Level_None = 0,      // Lowest Priority
  Priority_Level_0,             // Lowest Priority
  Priority_Level_1,
  Priority_Level_2,
  Priority_Level_3,
  Priority_Level_4,             // Highest Priority
};

enum {
  Difficulty_Easy = 0,
  Difficulty_Arcade,
  Difficulty_Hard,
};

// Tile enumerations
enum {
  PChar_SPACE = 0,
  PChar_PIPE_UP00,
  PChar_PIPE_UP01,
  PChar_PIPE_UP02,
  PChar_PIPE_UP04,
  PChar_PIPE_UP05,
  PChar_PIPE_UP06,
  PChar_PIPE_UP07,
  PChar_PIPE_UP08,
  PChar_PIPE_UP09,
  PChar_PIPE_UP10,
  PChar_PIPE_UP11,
  PChar_PIPE_UP12,
  PChar_PIPE_UP13,
  PChar_PIPE_UP14,
  PChar_PIPE_UP15,
  PChar_PIPE_UP16,
  PChar_PIPE_UP20,
  PChar_PIPE_UP21,
  PChar_PIPE_UP24,
  PChar_PIPE_UP25,
  PChar_PIPE_UP27,
  PChar_PIPE_UP28,
  PChar_PIPE_UP29,
  PChar_PIPE_UP30,
  PChar_PIPE_UP31,
  PChar_PIPE_UP32,
  PChar_PIPE_UP33,
  PChar_PIPE_UP34,
  PChar_PIPE_UP35,
  PChar_PIPE_MID03,
  PChar_PIPE_MID04,
  PChar_PIPE_MID13,
  PChar_PIPE_MID14,
  PChar_FLOOR_INVIS,
  PChar_FLOOR_SEMI_INVIS,
  PChar_FLOOR1,
  PChar_FLOOR2,
  PChar_FLOOR3,
  PChar_FLOOR4,
  PChar_FLOOR_ICE,
  PChar_PIPE_BRICK1,
  PChar_PIPE_BRICK2,
  PChar_PIPE_BRICK3,
  PChar_PIPE_BRICK4,
  PChar_PIPE_BRICK5,
  PChar_PIPE_BRICK6,
  PChar_PHASE1,
  PChar_PHASE2,
  PChar_PHASE3,
  PChar_PHASE4,
  PChar_PHASE_TRAP1,
  PChar_PHASE_TRAP2,
  PChar_PHASE_FIVE1,
  PChar_PHASE_FIVE2,
  PChar_PHASE_FIVE3,
  PChar_PHASE_FIVE4,
  PChar_PHASE_TEN1,
  PChar_PHASE_TEN2,
  PChar_PHASE_TEN3,
  PChar_PHASE_TEN4,
  PChar_PHASE_TWENTY1,
  PChar_PHASE_TWENTY3,
  PChar_PHASE_FIFTY1,
  PChar_PHASE_FIFTY3,
  PChar_POW,
  PChar_ZERO,
  PChar_ONE,
  PChar_TWO,
  PChar_THREE,
  PChar_FOUR,
  PChar_FIVE,
  PChar_SIX,
  PChar_SEVEN,
  PChar_EIGHT,
  PChar_NINE,
  PChar_DOT,
  PChar_EXCLAIM,
  PChar_PERIOD,
  PChar_QUOTE,
  PChar_QUESTION,
  PChar_AT,
  PChar_DASH,
  PChar_LEFT_PAREN,
  PChar_RIGHT_PAREN,
  PChar_UNDERSCORE,
  PChar_A,
  PChar_B,
  PChar_C,
  PChar_D,
  PChar_E,
  PChar_F,
  PChar_G,
  PChar_H,
  PChar_I,
  PChar_J,
  PChar_K,
  PChar_L,
  PChar_M,
  PChar_N,
  PChar_O,
  PChar_P,
  PChar_Q,
  PChar_R,
  PChar_S,
  PChar_T,
  PChar_U,
  PChar_V,
  PChar_W,
  PChar_X,
  PChar_Y,
  PChar_Z,
  PChar_PLATFORM1,
  PChar_PLATFORM2,
  PChar_PLATFORM3,
  PChar_TOP1,
  PChar_TOP2,
  PChar_TOP3,
  PChar_LIFE_CHAR,
  PChar_HORIZONTAL,
  PChar_VERTICAL,
  PChar_DIAGONAL1,
  PChar_DIAGONAL2,
  PChar_DIAGONAL3,
  PChar_DIAGONAL4,
  PChar_WATER,
  PChar_END,
  PChar_RUB,
};

#define	PChar_PIPE_UP03		PChar_PIPE_UP02
#define	PChar_PIPE_UP22		PChar_PIPE_UP03
#define	PChar_PIPE_UP23		PChar_PIPE_UP02
#define	PChar_PIPE_UP26		PChar_PIPE_UP07
#define	PChar_PIPE_UP36		PChar_PIPE_UP15
#define	PChar_PIPE_DN01		PChar_SPACE
#define	PChar_PIPE_DN02		PChar_PIPE_UP00
#define	PChar_PIPE_DN03		PChar_PIPE_UP03
#define	PChar_PIPE_DN04		PChar_PIPE_UP04
#define	PChar_PIPE_DN05		PChar_PIPE_UP07
#define	PChar_PIPE_DN06		PChar_PIPE_UP08
#define	PChar_PIPE_DN11		PChar_PIPE_UP20
#define	PChar_PIPE_DN12		PChar_SPACE
#define	PChar_PIPE_DN13		PChar_PIPE_UP21
#define	PChar_PIPE_DN14		PChar_PIPE_UP22
#define	PChar_PIPE_DN15		PChar_PIPE_UP25
#define	PChar_PIPE_DN16		PChar_PIPE_UP26
#define PChar_PIPE_MID01	PChar_PIPE_UP11
#define PChar_PIPE_MID02	PChar_PIPE_UP12
#define PChar_PIPE_MID05	PChar_PIPE_MID03
#define PChar_PIPE_MID06	PChar_PIPE_MID04
#define PChar_PIPE_MID07	PChar_PIPE_UP11
#define PChar_PIPE_MID08	PChar_PIPE_UP12
#define PChar_PIPE_MID11	PChar_PIPE_UP31
#define PChar_PIPE_MID12	PChar_PIPE_UP32
#define PChar_PIPE_MID15	PChar_PIPE_MID13
#define PChar_PIPE_MID16	PChar_PIPE_MID14
#define PChar_PIPE_MID17	PChar_PIPE_UP31
#define PChar_PIPE_MID18	PChar_PIPE_UP32

#define FLOOR_START			PChar_FLOOR_INVIS
#define FLOOR_END1			PChar_FLOOR_ICE
#define FLOOR_END2			PChar_POW

#define PIPE_START			PChar_SPACE
#define PIPE_END			PChar_FLOOR_INVIS

#define INIT_CHAR_SIZE		(PChar_PIPE_MID14 - PChar_SPACE + 1)

/*
 * @(#)MBroType.h
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
// Maryo Bros - Type Definitions
//
// Written by Chip Kerchner
//

//
// Typedefs
//
typedef struct {
  UInt8 xStart;                 // X Start
  UInt8 yStart;                 // Y Start
  UInt8 xSkip;                  // X Skip
  UInt8 ySkip;                  // Y Skip
  UInt8 xPixels;                // X Pixels
  UInt8 yPixels;                // Y Pixels
  UInt8 height;                 // Height
  UInt8 seqNum;                 // Current Sprite Sequence Number
  void *spritePtr;              // Pointer to Array of Graphical Sprites
  void *maskPtr;                // Pointer to Array of Graphical Masks
  UInt8 *sequence;              // Pointer to Sprite Sequence Array
} Drawer,
 *DrawerPtr;

typedef struct {
  UInt16 xPos;                  // X Position
  Int16 yPos;                   // Y Position
  UInt16 xPrev;                 // X Previous
  Int16 yPrev;                  // Y Previous
  UInt8 width;                  // Width
  Int8 deltaX;                  // Delta X
  Int8 deltaY;                  // Delta Y
  Int8 flipDeltaX;              // Flip Delta X
  UInt8 prevSeqNum;             // Previous Sprite Sequence Number
  UInt8 size;                   // Size of Sprite Sequence Array
  UInt8 motion;                 // Sprite Motion Type
  UInt8 level;                  // Level
  UInt8 num;                    // Number
  UInt8 type;                   // Type
  UInt16 flags;                 // Flags
  UInt32 intersect;             // Intersections
  Drawer draw;                  // Draw Structure
} Object,
 *ObjectPtr;

typedef struct objele {
  ObjectPtr object;             // Pointer to Object
  struct objele *next;          // Pointer to Next Sprite
} ObjEle,
 *ObjElePtr;

typedef struct {
  UInt16 xPos;                  // X Position
  Int16 yPos;                   // Y Position
  Int8 deltaX;                  // Delta X
  Int8 deltaY;                  // Delta Y
  UInt8 size;                   // Size of Sprite Sequence Array
  UInt8 type;                   // Type
  UInt16 flags;                 // Flags
  ObjectPtr object;             // Pointer to Object
  UInt8 moveSize;               // Size of Movement Array
  UInt8 moveNum;                // Current Movement Number
  Int8 *movement;               // Pointer to Movement Array
  Drawer draw;                  // Draw Structure
} Animation,
 *AnimationPtr;

typedef struct animele {
  AnimationPtr animate;         // Pointer to Animation
  struct animele *next;         // Pointer to Next Animation
} AnimateEle,
 *AnimateElePtr;

typedef struct {
  Int16 x1;                     // X1 Position
  Int16 y1;                     // Y1 Position
  Int16 x2;                     // X2 Position
  Int16 y2;                     // Y2 Position
} GameRectangle,
 *GameRectanglePtr;

typedef struct {
  Int8 x1;                      // X1 Position
  Int8 y1;                      // Y1 Position
  Int8 x2;                      // X2 Position
  Int8 y2;                      // Y2 Position
} BoundingRectangle,
 *BoundingRectanglePtr;

typedef struct {
  UInt16 type;                  // Event Type
  UInt32 time;                  // Event Time
  void *event;                  // Pointer to Event Data
} GameEvent,
 *GameEventPtr;

typedef struct {
  UInt8 *text;                  // Pointer to Text
  UInt8 x;                      // X Position
  UInt8 y;                      // Y Position
  UInt16 time;                  // Time
  UInt16 flags;                 // Flags
} TextEvent,
 *TextEventPtr;

typedef struct {
  UInt16 time;                  // Time
  UInt32 origTime;              // Original Time
} TimeEvent,
 *TimeEventPtr;

typedef struct {
  UInt8 *keyList;               // Pointer to Key List
  UInt8  keyNum;                // Key Num
  UInt8 *keyDelayList;          // Pointer to Key Delay List
  UInt8  keyDelayNum;           // Key Delay Num
} KeyEvent,
 *KeyEventPtr;

typedef struct {
  UInt8 eventNum;               // Text Event Number
  UInt8 strNum;                 // String Number
  UInt8 *text;                  // Pointer to Text
  UInt8 x;                      // X Position
  UInt8 y;                      // Y Position
  UInt16 time;                  // Time
} InstructionEvent,
 *InstructionEventPtr;

typedef UInt8 Tile,
 *TilePtr;

typedef struct {
  TilePtr tileLoc1;             // Tile Location 1
  TilePtr tileLoc2;             // Tile Location 2
} FreezeEvent,
 *FreezeEventPtr;

#define maxEnemies				    6
#define enemyBits				      2

#define enemyBytes				    (bitsInByte / enemyBits)
#define enemyMask				      ((1 << enemyBits) - 1)
#define enemySize				      ((maxEnemies + enemyBytes - 1)  / enemyBytes)

typedef struct {
  UInt8 paintBallTime;          // PaintBall Entrance Time
  UInt8 fireBallTime;           // FireBall Entrance Time
  UInt8 list[enemySize];        // Enemy Entrance List
} PhaseScript,
 *PhaseScriptPtr;

#define maxNumObjects			    18
#define maxNumEvents			    20
#define maxNumTextEvents		  4
#define maxNumAnimations		  12
#define maxStrLen				      33
#define numPipes				      3
#define numIceMachineStops		3
#define numIcicleStarts			  14
#define lenHighScore			    3
#define numHighScore			    5
#define numDifficulty         3
#define numPresetSequences		37
#define numPresetAnimations		31
#define numPresetKeyList		  3
#define numPresetKeyDelay		  5
#define numPresetInstruction	14

// Sector definitions
#define sectorXSize				    20
#define sectorYSize				    20

#define sectorXNum				    (screenWidth / sectorXSize)
#define sectorYNum				    (screenHeight / sectorYSize)
#define sectorNum				      (sectorXNum * sectorYNum)

typedef struct {
  // Address Variables
  void *gameVarPtr;

  UInt16 versionNum;

  // Status Variables
  Boolean gamePlaying;          // is there a game in play?
  Boolean gamePaused;           // is the game currently paused?
  Boolean gameWait;             // are we waiting for user? 
  UInt16 gameAnimationCount;    // a ticking counter

  // Timing Variables
  UInt16 keyRepeatRate;
  UInt32 nextPeriodTime;

  // Sound variables
  SndCommandType soundCommand;

  // Update Rectangle variables
  GameRectangle updateRect;

  // Key Variables
  UInt32 currentKey;
  UInt8 currentCount;

  // Object, Sprite & Tile Variables
  UInt8 objectCount;
  Object objectList[maxNumObjects];
  UInt8 totalObjects;
  Tile flipFloorTile;
  Tile platformTile;
  ObjEle spriteList[maxNumObjects];
  ObjElePtr spriteHead;
  UInt8 spriteCount;
  Tile charArray[charNum];

  // Event Variables
  GameEvent eventList[maxNumEvents];
  UInt8 eventGameCount;
  TextEvent textEvents[maxNumTextEvents];
  TimeEvent timeEvent;
  KeyEvent keyEvent;
  FreezeEvent freezeEvent;
  InstructionEventPtr instructionEvent;
  UInt16 lastInstructionTime;
  UInt8 gameStr[maxNumTextEvents + 1][maxStrLen];

  // Game Variables
  UInt32 gameScore;
  UInt32 lastGameScore;
  UInt8 numLives;
  UInt8 lastNumLives;
  UInt8 gamePhase;
  UInt8 nextTilePhase;
  UInt8 prevPhaseWidth;
  UInt8 gameState;
  UInt8 gameDifficulty;
  UInt8 enemyCount;
  Int8 slideNum;
  UInt8 powNum;
  UInt8 tileFriction;
  UInt8 phaseType;
  UInt8 lastPhaseType;
  UInt8 lastGamePhase;
  UInt8 totalCoins;
  UInt8 lastCoin;
  UInt16 bonusPoints;
  UInt8 whichScore;
  UInt32 lastKickTime;
  UInt8 kickCount;
  UInt32 lastPipeTime[numPipes];
  UInt8 icicleFill[numIcicleStarts];
  UInt8 iceMachineFill[numIceMachineStops + 1];
  UInt8 highScoreName[numDifficulty][lenHighScore];
  UInt32 pauseTime;
  Boolean gameOver;
  Boolean gameSize;
  UInt8 highScoreStrTable[numDifficulty][numHighScore][lenHighScore + 1];
  UInt32 highScoreTable[numDifficulty][numHighScore];

  // Internet Game Variables
  UInt32 internetHighScore;
  UInt8 internetHighScorePhase;

  // Option Controlled Variables
  UInt32 initExtraLife;
  UInt8 initNumLives;
  UInt8 gameOverPhase;
  UInt8 initStartPhase;
  UInt8 initDifficulty;

  // Animation Variables
  UInt8 animateCount;
  Animation animateList[maxNumAnimations];
  AnimateEle animationList[maxNumAnimations];
  AnimateElePtr animationHead;
  AnimateElePtr animationTail;

  // Collision Sector Variables
  UInt32 sectorArray[sectorNum];

  // Preset Variables
  UInt8 *preSetSequence[numPresetSequences];
  UInt8 *preSetAnimation[numPresetAnimations];
  UInt8 *preSetKeyList[numPresetKeyList];
  UInt8 *preSetKeyDelay[numPresetKeyDelay];
  InstructionEventPtr preSetInstruction[numPresetInstruction];
} GameVariables,
 *GameVariablesPtr;

typedef struct {
  // Off-Screen Buffer Pointers
  void *screenOffPtr;

  // Color Remapping Sprite Variables
  void *theSprite;
  MemHandle theSpriteHandle;

  // Table variables
  UInt8 *wakingColor;
  MemHandle wakingColorHandle;
  UInt8 *reverseArray;
  MemHandle reverseArrayHandle;

  // Maryo Bros variables
  void *piperPtr;
  MemHandle piperHandle;
  void *piperMaskPtr;
  MemHandle piperMaskHandle;

  // Turtle variables
  void *turtlePtr;
  MemHandle turtleHandle;
  void *turtleMaskPtr;
  MemHandle turtleMaskHandle;

  // Crab variables
  void *crabPtr;
  MemHandle crabHandle;
  void *crabMaskPtr;
  MemHandle crabMaskHandle;

  // Fighter Fly variables
  void *fighterFlyPtr;
  MemHandle fighterFlyHandle;
  void *fighterFlyMaskPtr;
  MemHandle fighterFlyMaskHandle;

  // Coin variables
  void *coinPtr;
  MemHandle coinHandle;
  void *coinMaskPtr;
  MemHandle coinMaskHandle;

  // Ice Machine variables
  void *iceMachinePtr;
  MemHandle iceMachineHandle;
  void *iceMachineMaskPtr;
  MemHandle iceMachineMaskHandle;

  // Fire Ball variables
  void *fireBallPtr;
  MemHandle fireBallHandle;
  void *fireBallMaskPtr;
  MemHandle fireBallMaskHandle;

  // Paint Ball variables
  void *paintBallPtr;
  MemHandle paintBallHandle;
  void *paintBallMaskPtr;
  MemHandle paintBallMaskHandle;

  // Icicle variables
  void *iciclePtr;
  MemHandle icicleHandle;
  void *icicleMaskPtr;
  MemHandle icicleMaskHandle;

  // Pow variables
  void *powPtr;
  MemHandle powHandle;
  void *powMaskPtr;
  MemHandle powMaskHandle;

  // Flip variables
  void *flipMaskPtr;
  MemHandle flipMaskHandle;

  // Animation variables
  void *animatePtr;
  MemHandle animateHandle;
  void *animateMaskPtr;
  MemHandle animateMaskHandle;

  // Tile variables
  void *charPtr;
  MemHandle charHandle;
  void *charMaskPtr;
  MemHandle charMaskHandle;

  // Color Palette variables
  void *colorTablePtr;
  MemHandle colorTableHandle;

  // Music variables
  void *musicPtr;
  MemHandle musicHandle;

  // Color variables
  Boolean useColor;

  struct {
    Boolean playing;            // is the music playing?
    Boolean looping;            // is the music looping?
    UInt16 musicSize;           // the size of the music
    UInt16 *musicData;          // the music data
    UInt16 priority;            // current sound priority

    struct {
      Boolean musicInterrupted; // was the music interupted?
      UInt16 frameCountDown;    // countdown until next note
      UInt16 frameIndex;        // the note to play next
    } playback;
  } music;

  struct {
    Boolean gamePadPresent;     // is the gamepad driver present
    UInt16 gamePadLibRef;       // library reference for gamepad
  } hardware;
} RunVariables,
 *RunVariablesPtr;

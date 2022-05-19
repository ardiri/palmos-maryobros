/*
 * @(#)MBroGame.h
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
// Maryo Bros - Game Routines
//
// Written by Chip Kerchner
//

//
// Function prototypes
//

// Main Game Update Routine
extern UInt16 UpdateWorld(void)  __GAME__;

// Initialization routines
extern void InitializeWorld(register PreferencesType *prefs,
                            register UInt16 ticks,
                            register Boolean reset) __GAME__;
extern void InitializeRunGlobals(register RunVariablesPtr globals)  __GAME__;

// Event routines
extern void AddEvent(register UInt16 type,
                     register UInt16 time,
                     register void *event) __GAME__;

// Update routines
extern void UpdateHighScore(register UInt32 keyState) __GAME__;
extern void UpdateMaryo(register UInt32 state) __GAME__;
extern void UpdateMovement(register PreferencesType * prefs,
                           register UInt32 keyStatus) __GAME__;
extern void UpdateScreen(void) __GAME__;
extern void UpdateTiles(void) __GAME__;
extern UInt32 UpdateGameStylus(register PreferencesType * prefs,
                               register Coord x,
                               register Coord y) __GAME__;
extern void UpdateStylusHighScore(register Boolean penDown,
                                  register Coord x,
                                  register Coord y) __GAME__;

// Adjustment routines
extern Boolean AdjustKeyList(void) __GAME__;
extern Boolean AdjustKeyDelayList(void) __GAME__;
extern Boolean AdjustInstruction(void) __GAME__;
extern Boolean AdjustSequence(UInt8 ** sequencePtr,
                              register Boolean change) __GAME__;

// Sound routines
extern void PlaySound(register UInt16 soundNum,
                      register UInt16 priority) __GAME__;

// Misc routines
extern void PauseGame(register UInt16 pause) __GAME__;
extern void StartGame(void) __GAME__;
extern void ResetHighScores(void) __GAME__;
extern void TerminateGame(void) __GAME__;
extern void TitleScreen(register UInt16 init) __GAME__;
extern void DrawPause(void) __GAME__;
extern void GameOver(void) __GAME__;

//
// Definitions
//

// Sprite flags
#define spriteNormal	    		  0x0000  // Draw Normal Sprite
#define spriteX				    	    0x0001  // Draw X Direction
#define spriteY				    	    0x0002  // Draw Y Direction
#define spriteRemap			    	  0x0004  // Draw Remap Colors

// Object flags           
#define flagDirectionX	    	  0x0001  // Drawing X Direction
#define flagDirty				        0x0002  // Object Dirty
#define flagEnter				        0x0004  // Entering Playing Field
#define flagExit				        0x0008  // Exiting Playing Field
#define flagNotInGame		  	    0x0010  // Not in Collision Calculations
#define flagFlipped			  	    0x0020  // Flipped Over
#define flagInvisible		    	  0x0040  // Do Not Draw
#define flagHit					        0x0080  // Object Hit
#define flagCount				        0x0100  // Countable Object
#define flagOnceThru		    	  0x0200  // Once Thru Playing Field
#define flagWaitToEnter	    	  0x0400  // Waiting to Enter Playing Field
#define flagWaking			    	  0x0800  // Enemy Waking
#define flagRotating		    	  0x1000  // Enemy Rotating
#define flagRotationX		    	  0x2000  // Rotation X Direction
#define flagRotationY		    	  0x4000  // Rotation Y Direction
#define flagRicochet		    	  0x8000  // Ricochet

#define flagSprite				      (flagRotationX | flagRotationY)
#define flagSpriteDeltaPlus	  	flagRotationX
#define flagSpriteDeltaMinus  	flagSprite
#define flagSpriteBits		      13

#define flagAnimDirectionX  	  0x0001  // Drawing X Direction
#define flagAnimLooping		  	  0x0002  // Looping

// Key definitions
#define moveLeft				        keyBitHard1
#define moveRight				        keyBitHard2
#define moveJump1				        keyBitHard3
#define moveJump2				        keyBitHard4
#define moveStop				        0x00000000

#define flipFrames				      2
#define rotationFrames		  	  (flipFrames * 3)

// Object definitions       
#define piperStartSeqNum	  	  0

#define piperStartX				      30
#define piperStartY				      116

#define turtleStartSeqNum	  	  0

#define turtleStartX			      24
#define turtleStartY			      29

#define turtleSpeedX			      2
#define turtleFastSpeedX	  	  4

#define crabStartSeqNum		  	  0

#define crabStartX				      40
#define crabStartY				      59

#define crabSpeedX				      3
#define crabHitSpeedX			      4
#define crabFastSpeedX		  	  5

#define fighterFlyStartSeqNum	  0

#define fighterFlyStartX		    56
#define fighterFlyStartY		    89

#define fighterFlySpeedX		    2
#define fighterFlyFastSpeedX	  4

#define coinStartSeqNum			    0

#define coinStartX				      48
#define coinStartY				      29

#define coinSpeedX				      4

#define iceMachineStartSeqNum	  0

#define iceMachineStartX		    64
#define iceMachineStartY		    59

#define iceMachineSpeedX		    3

#define fireBallStartSeqNum		  0

#define fireBallStartX			    80
#define fireBallStartY			    89

#define fireBallSpeedX			    5

#define paintBallStartSeqNum	  0

#define paintBallStartX			    83
#define paintBallStartY			    16
#define paintBallDeltaX			    objectWidth
#define paintBallLocations		  7

#define paintBallSpeedX			    4
#define paintBallSpeedY			    -2
#define paintBallFastSpeedX	  	6
#define paintBallFastSpeedY		  -3
#define paintBallFastestSpeedX	6
#define paintBallFastestSpeedY	-6
#define paintBallInsaneSpeedX	  6
#define paintBallInsaneSpeedY	  -12
#define icicleStartSeqNum		    0

#define icicleStartX			      40
#define icicleStartY			      74
#define powStartSeqNum			    0

#define powX					          15
#define powY					          20

#define powStartX				        (powX * charXSize)
#define powStartY				        (powY * charYSize)

#define flipStartSeqNum			    0

#define flipStartX				      0
#define flipStartY				      150

#define flipArea				        ((2 * objectWidth) - 1)

#define animateFloorNum			    2
#define animateMovementNum		  2

#define turtleTShirtDelta		    -2
#define kickDelta				        -10
#define killAdditional			    -6

// Misc denifitions
#define reviveX					        14
#define reviveY					        5
#define revivePos				        (reviveX + (reviveY * charXNum))
#define numPlatforms			      3

#define maxAcceleration			    13
#define speedTwo				        2
#define speedThree				      3
#define speedFour				        4
#define speedFive				        5

#define piperReviveX			      70
#define piperReviveStartY		    (-12 + topScreen)
#define piperReviveEndY			    11
#define piperReviveDeltaY		    1

#define piperSpeedX				      5
#define piperAccelerationX		  1

#define speedFactor				      2

#define lastSpeedX				      2

#define slideSpeedX				      2
#define slideDistance			      3
#define slideDeacceleration     2

#define scoreX					        20
#define scoreY					        2

#define scoreXSize				      6
#define scoreMinDig				      3

#define lifeX					          7
#define lifeY					          2
#define lifeDeltaX				      2

#define lifeXSize				        3

#define leftPipe				        0
#define rightPipe				        1
#define instructPipe			      2

#define phaseX			        		3
#define phaseY			        		26
#define phaseWidth	        		2
#define phaseHeight	        		2
#define phaseFive		        		5
#define phaseTen		        		10
#define phaseTwenty	        		20
#define phaseFifty	        		50

#define maxPhaseNum				      100
#define maxStoredPhase			    15
#define storedPhaseIterations	  7
#define maxDemoPhaseNum			    4

#define phaseInstruction		    ((maxPhaseNum / 4) * 4)
#define phaseOther				      ((maxPhaseNum / 4) * 5)
#define phaseBonus				      ((maxPhaseNum / 4) * 6)

#define	phaseEnterHighScore		  (maxPhaseNum + ((maxPhaseNum / 4) * 6) + 1)
#define	phaseDisplayHighScore	  (maxPhaseNum + ((maxPhaseNum / 4) * 6) + 2)
#define phaseTitle				      (maxPhaseNum + ((maxPhaseNum / 4) * 6) + 3)
#define phaseOption				      (maxPhaseNum + ((maxPhaseNum / 4) * 6) + 4)

#define phaseTurtleStart		    1
#define phaseTurtleEnd			    2
#define phaseCrabStart			    4
#define phaseCrabEnd			      5
#define phaseFighterFlyStart	  6
#define phaseFighterFlyEnd		  7
#define phaseIceMachineStart	  9
#define phaseIcicleStart		    16

#define phaseInstructTurtle		  1
#define phaseInstructCrab		    4
#define phaseInstructFighterFly	6
#define phaseInstructIceMachine	9

#define phaseStartX				      13
#define phaseStartY				      17

#define instructDemoStartX		  10
#define instructDemoStartY		  11

#define instructObject1X		    35
#define instructObject1Y		    35
#define instructObject2X		    115
#define instructObject2Y		    35

#define instruct1ObjectMoveX	  125
#define instruct1ObjectMoveY	  79

#define instruct2ObjectMoveX	  131
#define instruct2ObjectMoveY	  49

#define instructTurtleStartX	  10
#define instructTurtleStartY	  8

#define instruct1PiperStartX	  101
#define instruct1PiperStartY	  111

#define instruct1Tile1X		    	0
#define instruct1Tile1Y		    	25
#define instruct1Tile1Len	    	charXNum
#define instruct1Tile2X		    	9
#define instruct1Tile2Y		    	18
#define instruct1Tile2Len	    	23
#define instruct1SeqNumDelta	  2

#define instruct2PiperStartX	  32
#define instruct2PiperStartY	  96

#define instruct2Tile1X		    	9
#define instruct2Tile1Y		    	16
#define instruct2Tile1Len	    	16
#define instruct2Tile2X		    	0
#define instruct2Tile2Y		    	22
#define instruct2Tile2Len	    	charXNum
#define instruct2Tile2Height	  5
#define instruct2SeqNumDelta	  3

#define instruct2Anim1X1		    40
#define instruct2Anim1Y1		    120
#define instruct2Anim1DeltaX	  70
#define instruct2Anim1DeltaY	  animateHeight
#define instruct2Anim1Num		    4

#define numBonusCoins			      10

#define iceInstructTileWidth	  6
#define iceInstructTileHeight	  13
#define iceInstructTileNum		  (iceInstructTileWidth * iceInstructTileHeight)
#define iceInstructTileStartY	  9

#define instructCrabStartX		  12
#define instructCrabStartY		  8
#define instructFFlyStartX		  12
#define instructFFlyStartY		  8
#define instructIceStartX		    11
#define instructIceStartY		    8

#define phaseBonus1				      3
#define phaseBonus2				      8
#define phaseBonus3				      15

#define bonusStartX				      8
#define bonusStartY				      11

#define bonusTimeStartX			    14
#define bonusTimeStartY			    4
#define bonusWidth				      4
#define bonusHeight				      1

#define numBonusCoins			      10

#define bonusCoin1StartX		    20
#define bonusCoin1StartY		    9
#define bonusCoin2StartX	    	130
#define bonusCoin2StartY	    	9
#define bonusCoin3StartX	    	10
#define bonusCoin3StartY	    	45
#define bonusCoin4StartX	    	25
#define bonusCoin4StartY	    	45
#define bonusCoin5StartX	    	125
#define bonusCoin5StartY	    	45
#define bonusCoin6StartX	    	140
#define bonusCoin6StartY	    	45
#define bonusCoin7StartX	    	45
#define bonusCoin7StartY	    	75
#define bonusCoin8StartX	    	105
#define bonusCoin8StartY	    	75
#define bonusCoin9StartX	    	20
#define bonusCoin9StartY	    	105
#define bonusCoin10StartX	    	130
#define bonusCoin10StartY	    	105

#define iceMachineX1		      	75
#define iceMachineY1		      	59
#define iceMachineX2		      	22
#define iceMachineY2		      	89
#define iceMachineX3		      	128
#define iceMachineY3		      	89

#define iceMachineX4		      	87
#define iceMachineY4		      	69

#define gameOverStartX		    	12 
#define gameOverStartY			    11

#define initialStartX			      8
#define initialStartY			      10
#define initialDeltaY			      2

#define maryoBros1StartX		    6
#define maryoBros1StartY		    9

#define total1PiperStartX		    15
#define total1PiperStartY		    41

#define totalCoinStartX		    	60
#define totalCoinStartY		    	40
#define totalCoinDeltaX		    	10
#define totalCoinDeltaY		    	10
#define totalCountWidth		    	5
#define totalCoin1StartX	    	24
#define totalCoin1StartY	    	9

#define totalSeqNum				      2

#define totalCoinPerfectStartX	8
#define totalCoinPerfectStartY	18
#define totalCoinNoBonusStartX	12
#define totalCoinNoBonusStartY	18

#define misc1StartX				      9
#define misc1StartY				      10
#define misc2StartX			      	6
#define misc2StartY			      	12

#define instruct1Str1X		    	7
#define instruct1Str1Y		    	20    
#define instruct1Str1Time	    	54
#define instruct1Str2X		    	2
#define instruct1Str2Y		    	19
#define instruct1Str2Time	    	98
#define instruct1Str3X		    	12
#define instruct1Str3Y		    	20
#define instruct1Str3Time	    	116
#define instruct1Str4X		    	8
#define instruct1Str4Y		    	22
#define instruct1Str4Time	    	116

#define instruct2Str1X		    	18
#define instruct2Str1Y		    	20
#define instruct2Str1Time	    	16
#define instruct2Str2X		    	6
#define instruct2Str2Y		    	15
#define instruct2Str2Time	    	24
#define instruct2Str3X		    	3
#define instruct2Str3Y		    	20
#define instruct2Str3Time	    	53
#define instruct2Str4Time	    	98
#define instruct2Str5Time	    	98

#define instruct3Str1X		    	7
#define instruct3Str1Y		    	20
#define instruct3Str1Time	    	43
#define instruct3Str2X		    	5
#define instruct3Str2Y		    	13
#define instruct3Str2Time	    	44

#define initHighScore			      12000
#define defaultNumLives		    	3
#define defaultStartPhase	    	0
#define initInternetScore	    	0

#define initHighScore1	    		initHighScore
#define initHighScore2	    		9800
#define initHighScore3	    		7600
#define initHighScore4	    		5400
#define initHighScore5	    		3200

#define highScoreDeltaX1	    	3
#define highScoreDeltaY1	    	2

#define highScoreLineX	    		7
#define highScoreLineY	    		17
#define highScoreLineLen    		18
#define highScoreRankX	    		8
#define highScoreRankY	    		16
#define highScoreScoreX	    		13
#define highScoreScoreY	    		16
#define highScoreNameX	    		20
#define highScoreNameY	    		16
#define highScoreRegisterX	  	7
#define highScoreRegisterY	  	2
#define highScoreNameEnterX	  	13
#define highScoreNameEnterY	  	4
#define highScoreInitialX		    18
#define highScoreInitialY	    	4
#define highScoreTimeX			    11
#define highScoreTimeY			    13
#define highScoreTimeTextX		  18
#define highScoreTimeTextY		  13
#define highScoreRegisterTextX	5
#define highScoreRegisterTextY	13

#define highScoreDeltaY			    3
#define highScoreDeltaLine		  2
#define lenHighRank				      3

#define highScoreCharX			    6
#define highScoreCharY			    7
#define highScoreCharDeltaX		  2
#define highScoreCharDeltaY		  2
#define highScoreCharWidth		  10
#define highScoreCharLen		    30

#define highScoreTile1X		    	0
#define highScoreTile1Y		    	5
#define highScoreTile1Len	    	14
#define highScoreTile2X		    	18
#define highScoreTile2Y		    	highScoreTile1Y
#define highScoreTile2Len	    	14
#define highScoreTile3X		    	0
#define highScoreTile3Y		    	11
#define highScoreTile3Len	    	charXNum

#define highScoreObject1X	    	20
#define highScoreObject1Y	    	14
#define highScoreObject2X	    	110
#define highScoreObject2Y	    	44
#define highScoreObject3X	    	140
#define highScoreObject3Y	    	14
#define highScoreObject4X	    	50
#define highScoreObject4Y	    	44

#define enterHighScoreX		    	50
#define enterHighScoreY		    	17

#define displayHighScoreDeltaY	3

#define icicleStartX1		      	10
#define icicleStartY1		      	16
#define icicleStartX2		      	140
#define icicleStartY2		      	16
#define icicleStartX3		      	0
#define icicleStartY3		      	44
#define icicleStartX4		      	20
#define icicleStartY4		      	44
#define icicleStartX5		      	30
#define icicleStartY5		      	44
#define icicleStartX6		      	40
#define icicleStartY6		      	44
#define icicleStartX7		      	50
#define icicleStartY7		      	44
#define icicleStartX8		      	60
#define icicleStartY8		      	44
#define icicleStartX9		      	90
#define icicleStartY9		      	44
#define icicleStartX10	    		100
#define icicleStartY10	    		44
#define icicleStartX11	    		110
#define icicleStartY11	    		44
#define icicleStartX12	    		120
#define icicleStartY12	    		44
#define icicleStartX13	    		130
#define icicleStartY13	    		44
#define icicleStartX14	    		150
#define icicleStartY14	    		44

#define numFireBallStarts	    	4
#define numFireBallSpikes	    	4

#define fireBallStartY1	    		27
#define fireBallStartY2	    		57
#define fireBallStartY3	    		87
#define fireBallStartY4	    		117

#define fireBallRangeY1		    	45
#define fireBallRangeY2	    		80
#define fireBallRangeY3		    	105
#define fireBallRangeY4		    	159

#define titleNameX				      0
#define titleNameY				      1

#define titleTileX		      		32
#define titleTileY		      		18
#define titleTileNum	      		(titleTileX * titleTileY)
#define titleTile1X		      		0
#define titleTile1Y		      		23
#define titleTile1Len	      		charXNum

#define piperStartTitleX	    	30
#define piperStartTitleY	    	101

#define numSetObjects		      	3
#define numOtherObjects			    (3 + maxEnemies)
#define numIcicleObjects		    (maxNumObjects - (numSetObjects + numOtherObjects))

#define jumpHeight				      37
#define jumpTime				        12

#define hitHeight				        5
#define hitTime					        6

#define flipperHeight		      	12
#define flipperTime				      6

#define bounceHeight			      11
#define bounceTime				      6

#define flapHeight				      10
#define flapTime				        6

#define surpriseTime	      		4
#define icicleTime		      		5
#define icicleIntraTime		    	1

#define fallTime			        	6

#define stopFall			        	0x7e

#define maxHitTime			      	1

#define defaultFriction		    	1
#define	iceFriction			      	5

#define objectEnterX1	      		20
#define objectEnterX2	      		140
#define objectEnterY1	      		9

#define objectEnterX3	      		130
#define objectEnterY3	      		49

#define objectExitX1	      		10
#define objectExitX2	      		150
#define objectExitY		      		105

// Scoring definitions
#define pointsFlip			       	10
#define pointsCoin			      	800
#define pointsKick			      	800
#define pointsPaintBall	    		1000
#define pointsFireBall	    		200
#define pointsIceMachine    		500
#define pointsBonus1		      	5000
#define pointsBonus2		      	8000
#define defaultPointsExtraLife	20000

#define checkAll				        2

#define ricochetX				        1
#define ricochetY				        2

// Timing definitions
#define textTime				        500     // Time For Text Draw
#define entranceIntraTime		    100     // Time Between Enemy Intra Start Entrances
#define entranceInterTime		    800     // Time Between Enemy Inter Start Entrances
#define coinTime		        		100     // Time Before Bonus Coin Enters
#define wakeTime		        		650     // Time Before Enemy Wakes Up
#define flipTime		        		150     // Time Before Enemy Flips Over
#define pipeTime		        		100     // Time Between Exit & Enter Pipes
#define delayTime		        		200     // Time Between Enemy Entrances
#define delayInstructTime	    	100     // Time Between Enemy Instruction Entrances
#define instructionTime		    	200     // Time For Instruction Text
#define dropTime		        		800     // Time Before Icicle Drop
#define endTime			        		150     // Time At End of Phase
#define startTime		        		100     // Time At Beginning of Phase
#define blinkTime		        		100     // Time For Blinking Text
#define meltTime		        		200     // Time For Melt
#define semiInvisTime	      		100     // Time For Change Floor Tiles to Semi Invisible
#define invisibleTime	      		180     // Time For Change Floor Tiles to Invisible
#define kickDelayTime	      		100     // Time For Multiple Kick Points
#define coinDelayTime	      		100     // Time For Multiple Coin Points
#define totalTime			        	300     // Time For Total Screen
#define miscTime			        	200     // Time For Misc Screen
#define gameTotalTime	      		100     // Time For Grand Total Text
#define platformTime	      		600     // Time For Platform
#define highScoreTime	      		3000    // Time For High Score
#define registeredTime			    400     // Time For Registered Text
#define displayHighScoreTime	  600     // Time For Display High Scores
#define titleTime			        	1000    // Time For Title
#define reviveTime		      		100     // Time For Revive
#define disappearTime	      		25      // Time For Disappear
#define dripTime				        150     // Time For Icicle Drip
#define appearTime			      	75      // Time For Appear
#define removeTime				      50      // Time For Remove
#define biteTime				        25      // Time For Bite
#define reigniteTime	      		300     // Time for Reignite
#define delayGameTime			      12000   // Time for Delay of Game
#define collectTime				      100     // Time For Collect
#define coinRemoveTime			    60      // Time For Coin Remove
#define gameOverTime            700     // Time For Game Over

#define timerTime				        16      // Time Between Bonus Iterations
#define highScoreSelectTime		  16      // Time Before High Score Select
#define turnTime				        48      // Time For Turn

#define bonus1Time		      		2000
#define bonus2Time		      		2000
#define bonus3Time		      		1500
#define bonusExtraTime          500

#define textNormal		      		0x0000
#define textBlink			        	0x0001
#define textDraw		        		0x0002

#define bonus3StartX			      0
#define bonus3StartY			      8
#define bonus3StartPos			    (bonus3StartX + (bonus3StartY * charXNum))

#define gameNormal			  	    0x00
#define gameDestroy			      	0x01
#define gameEnd		        			0x02
#define gameLast	        			0x04
#define gameKill	        			0x08

#define maxNumKeys			      	16
#define maxNumInstructions	  	6

#define fixEnemyCount			      1

#define timerFactor				      125

#define endInstruction		    	255
#define endDelay				        255

#define instruction1Delay1	  	3
#define instruction1Delay2	  	10
#define instruction1Delay3	  	8
#define instruction1Delay4	  	19
#define instruction1Delay5	  	10
#define instruction1Delay6	  	32
#define instruction1Delay7	  	7
#define instruction1Delay8	  	6
#define instruction1Delay9	  	10
#define instruction1Delay10	  	37
#define instruction1Delay11	  	endDelay

#define instruction1CoinDelay	  (125 * 8)
#define instruction1CoinStartX	140
#define instruction1CoinStartY	60
#define instruction1Anim1X	  	135
#define instruction1Anim1Y	  	70
#define instruction1Anim2X	  	145
#define instruction1Anim2Y	  	70

#define instruction2Delay1	  	6
#define instruction2Delay2	  	10
#define instruction2Delay3	  	22
#define instruction2Delay4	  	8
#define instruction2Delay5	  	10
#define instruction2Delay6	  	19
#define instruction2Delay7	  	7
#define instruction2Delay8	  	6
#define instruction2Delay9	  	10
#define instruction2Delay10	  	37
#define instruction2Delay11	  	endDelay

#define instruction3Delay1	  	3
#define instruction3Delay2	  	10
#define instruction3Delay3	  	8
#define instruction3Delay4	  	15
#define instruction3Delay5	  	10
#define instruction3Delay6	  	23
#define instruction3Delay7	  	endDelay

#define instruction4Delay1  		5
#define instruction4Delay2  		31
#define instruction4Delay3  		10
#define instruction4Delay4  		5
#define instruction4Delay5  		3
#define instruction4Delay6  		19
#define instruction4Delay7  		endDelay

#define pauseX				        	11 
#define pauseY				        	15
#define pauseLen			        	11

#define titleDelay1		      		44
#define titleDelay2		      		15
#define titleDelay3		      		10
#define titleDelay4		      		71
#define titleDelay5		      		endDelay

#define dirtyChar				        0x80

// Preset object list defintions

#define piperObject				      gameVars->objectList[0]
#define powObject				        gameVars->objectList[1]
#define flipObject				      gameVars->objectList[2]

//
// Global varaibles
//

extern GameVariablesPtr         gameVars;
extern RunVariablesPtr          runVars;

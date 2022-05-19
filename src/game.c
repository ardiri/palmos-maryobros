
/*
 * @(#)game.c
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

#include "palm.h"

#include "MBroInit.h"
#include "MBroEnum.h"
#include "MBroGame.h"
#include "MBroDraw.h"

// interface

/**
 * Initialize the Game.
 */
void
GameInitialize()
{
  RunVariablesPtr gbls;
  Err err;

  // create the globals object, and register it
  gbls = (RunVariablesPtr) MemPtrNew(sizeof(RunVariables));
  MemSet(gbls, sizeof(RunVariables), 0);
  FtrSet(appCreator, ftrGameGlobals, (UInt32) gbls);

  err = errNone;

  // load images.
  {
    WinHandle currWindow;
    UInt16 numFail;

    currWindow = WinGetDrawWindow();

    // Initialize Graphics
    InitializeRunGlobals(gbls);

    numFail = 0;
    while (!InitializeGraphics()) {
      DestroyGraphics();
      MemSet(gbls, sizeof(RunVariables), 0);

      if (numFail++ != 0) {
        err = errNone + 1;
        break;
      }
    }

    if (err == errNone)
      ClearScreen();

    WinSetDrawWindow(currWindow);
  }

  // load the gamepad driver if available
  {
    Err err;

    // attempt to load the library
    err = SysLibFind(GPD_LIB_NAME, &gbls->hardware.gamePadLibRef);
    if (err == sysErrLibNotFound)
      err =
        SysLibLoad('libr', GPD_LIB_CREATOR, &gbls->hardware.gamePadLibRef);

    // lets determine if it is available
    gbls->hardware.gamePadPresent = (err == errNone);

    // open the library if available
    if (gbls->hardware.gamePadPresent)
      GPDOpen(gbls->hardware.gamePadLibRef);
  }

  if (err != errNone)
  {
    ApplicationDisplayDialog(xmemForm);
  }
}

/**
 * Check if start button on GamePad was pressed
 */
void
GameCheckStartButton(void)
{
  if (WinGetActiveWindow() == (WinHandle) FrmGetActiveForm())
  {
    FormType *form;

    form = FrmGetActiveForm();
    if (form == FrmGetFormPtr(mainForm))
    {
      RunVariablesPtr gbls;

      // get a globals reference
      FtrGet(appCreator, ftrGameGlobals, (UInt32 *) & gbls);

      // standard palm button checks here
      if ((KeyCurrentState() &
          (keyBitHard1 | keyBitHard2 | keyBitHard3 | keyBitHard4 | 
           keyBitPageUp | keyBitPageDown)) != 0) 
      {
        EventType event;

        MemSet(&event, sizeof(EventType), 0);
        event.eType = menuEvent;
        event.data.menu.itemID = mainMenuItemPlay;
        EvtAddEventToQueue(&event);
      }

      // gamepad checks here
      if (gbls->hardware.gamePadPresent)
      {
        UInt8 gamePadKeyStatus;
        Err err;

        err = GPDReadInstant(gbls->hardware.gamePadLibRef, &gamePadKeyStatus);
        if (err == errNone)
        {
          if ((gamePadKeyStatus & GAMEPAD_START) != 0)
          {
            EventType event;

            MemSet(&event, sizeof(EventType), 0);
            event.eType = menuEvent;
            event.data.menu.itemID = mainMenuItemPlay;
            EvtAddEventToQueue(&event);
          }
        }
      }
    }
  }
}

/**
 * Reset the Game preferences.
 * 
 * @param prefs the global preference data.
 */
void
GameResetPreferences(PreferencesType * prefs)
{
  RunVariablesPtr gbls;

  // get a globals reference
  FtrGet(appCreator, ftrGameGlobals, (UInt32 *) & gbls);

  // now we are playing
  prefs->gameVariables.gamePlaying = true;
  prefs->gameVariables.gamePaused = false;
  prefs->gameVariables.gameWait = true;
  prefs->gameVariables.gameAnimationCount = 0;

  // start the game
  StartGame();
}

/**
 * Process key input from the user.
 * 
 * @param prefs the global preference data.
 * @param keyStatus the current key state.
 */
void
GameProcessKeyInput(PreferencesType * prefs,
                    UInt32 keyStatus)
{
  RunVariablesPtr gbls;

  // get a globals reference
  FtrGet(appCreator, ftrGameGlobals, (UInt32 *) & gbls);

  keyStatus &= (prefs->config.ctlKeyJump |
                prefs->config.ctlKeyLeft | prefs->config.ctlKeyRight);

  // additonal checks here
  if (gbls->hardware.gamePadPresent)
  {
    UInt8 gamePadKeyStatus;
    Err err;

    // read the state of the gamepad
    err = GPDReadInstant(gbls->hardware.gamePadLibRef, &gamePadKeyStatus);
    if (err == errNone)
    {
      // process
      if (((gamePadKeyStatus & GAMEPAD_LEFTFIRE) != 0) ||
          ((gamePadKeyStatus & GAMEPAD_RIGHTFIRE) != 0))
        keyStatus |= prefs->config.ctlKeyJump;
      if ((gamePadKeyStatus & GAMEPAD_LEFT) != 0)
        keyStatus |= prefs->config.ctlKeyLeft;
      if ((gamePadKeyStatus & GAMEPAD_RIGHT) != 0)
        keyStatus |= prefs->config.ctlKeyRight;

      // special purpose :)
      if ((gamePadKeyStatus & GAMEPAD_SELECT) != 0)
      {
        // wait until they let it go :) 
        do
        {
          GPDReadInstant(gbls->hardware.gamePadLibRef, &gamePadKeyStatus);
        }
        while ((gamePadKeyStatus & GAMEPAD_SELECT) != 0);

        keyStatus = 0;
        prefs->gameVariables.gamePaused = !prefs->gameVariables.gamePaused;
        PauseGame(prefs->gameVariables.gamePaused);
      }
    }
  }

  // did they press at least one of the game keys?
  if (keyStatus != 0)
  {

    // if they were waiting, we should reset the game animation count
    if (prefs->gameVariables.gameWait)
    {
      prefs->gameVariables.gameAnimationCount = 0;
      prefs->gameVariables.gameWait = false;
    }

    // great! they wanna play
    prefs->gameVariables.gamePaused = false;
    PauseGame(prefs->gameVariables.gamePaused);
  }

  UpdateMovement(prefs, keyStatus);
}

/**
 * Process stylus input from the user.
 * 
 * @param prefs the global preference data.
 * @param x the x co-ordinate of the stylus event.
 * @param y the y co-ordinate of the stylus event.
 */
void
GameProcessStylusInput(PreferencesType * prefs,
                       Boolean penDown,
                       Coord x,
                       Coord y)
{
  if (prefs->gameVariables.phaseType != Phase_EnterHighScores)
  {
    prefs->stylusKeys = UpdateGameStylus(prefs, x, y);
  }
  else
  if (CHECK_SIGNATURE(prefs)) {
    UpdateStylusHighScore(penDown, x, y);
  }

  if (prefs->gameVariables.gamePaused)
  {
    prefs->gameVariables.gamePaused = false;
    PauseGame(prefs->gameVariables.gamePaused);
  }
}

void
GameMusicPlayback()
{
  RunVariablesPtr gbls;

  // get a globals reference
  FtrGet(appCreator, ftrGameGlobals, (UInt32 *) & gbls);

  // is the music actually playing and music exists?
  if ((gbls->music.playing) && (gbls->music.musicData != NULL)) {
    SndCommandType musicNote = { sndCmdFrqOn, 0, 0, 0, sndMaxAmp };
    UInt16 index;

    index = gbls->music.playback.frameIndex * 2;

    // do we need to play a new note?
    if (gbls->music.playback.frameCountDown == 0) {
      if (gbls->music.playback.frameIndex++ >= gbls->music.musicSize) {
        if (gbls->music.looping) {
            index = gbls->music.playback.frameIndex = 0;
        } else {
          gbls->music.playing = false;
          return;
        }
      }

      // prepare for the next frame
      gbls->music.playback.frameCountDown = gbls->music.musicData[index];

      // must trigger the playback :)
      gbls->music.playback.musicInterrupted = true;
    }

    // not ready yet, count down..
    else {
      gbls->music.playback.frameCountDown--;
    }

    // if music was interrupted, we need to restart the note
    if (gbls->music.playback.musicInterrupted) {
      // what freq/duration to play?
      musicNote.param1 = gbls->music.musicData[index + 1];
      musicNote.param2 = gbls->music.playback.frameCountDown;

      gbls->music.playback.frameCountDown = ((gbls->music.playback.frameCountDown + MUSIC_TIME - 1) / MUSIC_TIME);
      gbls->music.playback.musicInterrupted = false;

      // play the sound
      DevicePlaySound(&musicNote);
    }
  }
}

/**
 * Process the object movement in the game.
 * 
 * @param prefs the global preference data.
 */
void
GameMovement(PreferencesType * prefs)
{
}

/**
 * Draw the game on the screen.
 * 
 * @param prefs the global preference data.
 */
void
GameDraw(PreferencesType * prefs)
{
  if (UpdateWorld())
  {
    // "please register" dialog :)
    if (!CHECK_SIGNATURE(prefs)) 
      ApplicationDisplayDialog(rbugForm);

    // show highscore page if possible
    else if (gameVars->gameDifficulty == Difficulty_Arcade)
    {
      GameAdjustmentType adjustType;

      // define the adjustment
      adjustType.adjustMode = gameHighScore;
      adjustType.data.highScore.gameScore = 
        prefs->gameVariables.gameScore;
      adjustType.data.highScore.gamePhase = 
        prefs->gameVariables.lastGamePhase;
      adjustType.data.highScore.highScore = 
        (UInt32 *)&prefs->gameVariables.internetHighScore;
      adjustType.data.highScore.highPhase = 
        (UInt8 *)&prefs->gameVariables.internetHighScorePhase;

      if (RegisterAdjustGame(prefs, &adjustType))
      {
        ApplicationDisplayDialog(highForm);
      }
    }

    // GAME OVER - return to the main screen
    {
      EventType event;

      MemSet(&event, sizeof(EventType), 0);
      event.eType = menuEvent;
      event.data.menu.itemID = gameMenuItemExit;
      EvtAddEventToQueue(&event);
    }
  }

  else
  {
    GraphicsRepaint();
  }
}

/**
 * Terminate the game.
 */
void
GameTerminate()
{
  RunVariablesPtr gbls;

  // get a globals reference
  FtrGet(appCreator, ftrGameGlobals, (UInt32 *) & gbls);

  // unlock the gamepad driver (if available)
  if (gbls->hardware.gamePadPresent)
  {
    Err err;
    UInt32 gamePadUserCount;

    err = GPDClose(gbls->hardware.gamePadLibRef, &gamePadUserCount);
    if (gamePadUserCount == 0)
      SysLibRemove(gbls->hardware.gamePadLibRef);
  }

  // Destroy Graphics
  DestroyGraphics();

  // clean up windows/memory
  MemPtrFree(gbls);

  // unregister global data
  FtrUnregister(appCreator, ftrGameGlobals);
}

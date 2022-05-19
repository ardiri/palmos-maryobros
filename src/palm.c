/*
 * @(#)palm.c
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

#include "MBroEnum.h"
#include "MBroGame.h"
#include "MBroDraw.h"

#include "palm.inc"

#include "HostControl.h"

/**
 * The Form:mainForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
mainFormEventHandler(EventType * event)
{
  Globals *globals;
  Boolean processed = false;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *) & globals);

  switch (event->eType)
  {
    case frmOpenEvent:
      {
        FormType *frm = FrmGetActiveForm();

        // dynamically adjust the menu :)
        FrmSetMenu(frm,
                   DeviceSupportsGrayscale()? mainMenu_gray :
                   mainMenu_nogray);
        FrmDrawForm(frm);

        {
          EventType event;

          MemSet(&event, sizeof(EventType), 0);
          event.eType = frmUpdateEvent;
          event.data.frmClose.formID = FrmGetActiveFormID();
          EvtAddEventToQueue(&event);
        }
      }
      processed = true;
      break;

    case frmUpdateEvent:
      {
        FormType *frm = FrmGetActiveForm();

        // draw the form
        FrmDrawForm(frm);

        // draw seperators (start @ 50,.. to prevent overwriting of y)
        WinDrawLine(50, SCREEN_START - 2, 159, SCREEN_START - 2);
        WinDrawLine(0, SCREEN_START + SCREEN_HEIGHT, 
                    159, SCREEN_START + SCREEN_HEIGHT);

        // repaint the offscreen window
        GraphicsSetUpdate(0, maxTitlePixels - 1);
        GraphicsRepaint();

        RegisterShowMessage(globals->prefs);
      }
      processed = true;
      break;

    case penDownEvent:
    case penMoveEvent:

      // if the player has tapped this area, they wants to play
      if ((event->screenX > 0) && (event->screenX < 160) &&
          (event->screenY > SCREEN_START) &&
          (event->screenY < (160 - SCREEN_START)))
      {
        Boolean penDown;
        Coord penX, penY;

        // we are going to wait until they lift the pen
        penDown = true;
        while (penDown)
          EvtGetPen(&penX, &penY, &penDown);

        // regenerate a menu event
        MemSet(event, sizeof(EventType), 0);
        event->eType = menuEvent;
        event->data.menu.itemID = mainMenuItemPlay;
        EvtAddEventToQueue(event);

        SndPlaySystemSound(sndClick);
        processed = true;
      }
      break;

    case menuEvent:

      // what menu?
      switch (event->data.menu.itemID)
      {
        case mainMenuItemPlay:
          GameResetPreferences(globals->prefs);
          FrmGotoForm(gameForm);

          processed = true;
          break;

        case mainMenuItemShowHigh:

          if (CHECK_SIGNATURE(globals->prefs))
          {
            ApplicationDisplayDialog(highForm);
          }
          else
          {
            SndPlaySystemSound(sndError);
          }

          processed = true;
          break;

        case mainMenuItemResetHigh:

          // lets make sure!!
          if (FrmAlert(resetHighScoresAlert) == 0)
          {
            ResetHighScores();
          }
          processed = true;
          break;

        default:
          break;
      }
      break;

    case nilEvent:

      // make sure the active window is the form 
      if (WinGetActiveWindow() == (WinHandle) FrmGetActiveForm())
      {
        GameMusicPlayback();

        // draw the game!
        GameDraw(globals->prefs);

        // play the game!
        GameProcessKeyInput(globals->prefs,
                            (KeyCurrentState() | globals->prefs->stylusKeys));

        globals->prefs->stylusKeys = 0;
      }
      processed = true;
      break;

    default:
      break;
  }

  return processed;
}

/**
 * The Form:gameForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
gameFormEventHandler(EventType * event)
{
  Globals *globals;
  Boolean processed = false;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *) & globals);

  switch (event->eType)
  {
    case frmOpenEvent:
      {
        FormType *frm = FrmGetActiveForm();

        // dynamically adjust the menu :)
        FrmSetMenu(frm,
                   DeviceSupportsGrayscale()? gameMenu_gray :
                   gameMenu_nogray);
        FrmDrawForm(frm);

        {
          EventType event;

          MemSet(&event, sizeof(EventType), 0);
          event.eType = frmUpdateEvent;
          event.data.frmClose.formID = FrmGetActiveFormID();
          EvtAddEventToQueue(&event);
        }
      }
      processed = true;
      break;

    case frmUpdateEvent:
      {
        FormType *frm = FrmGetActiveForm();

        // draw the form
        FrmDrawForm(frm);

        // draw seperators (start @ 50,.. to prevent overwriting of y)
        WinDrawLine(50, SCREEN_START - 2, 159, SCREEN_START - 2);
        WinDrawLine(0, SCREEN_START + SCREEN_HEIGHT, 
                    159, SCREEN_START + SCREEN_HEIGHT);

        // repaint the offscreen window
        GraphicsSetUpdate(0, maxPixels - 1);
        GraphicsRepaint();

        RegisterShowMessage(globals->prefs);
      }
      processed = true;
      break;

    case penDownEvent:
    case penMoveEvent:

      // within the game play area?
      if ((globals->prefs->gameVariables.gamePlaying) &&
          (event->screenX > 0) && (event->screenX < 160) &&
          (event->screenY > SCREEN_START)
          && (event->screenY < (160 - SCREEN_START)))
      {
        // if the player has tapped this area, they wants to move
        GameProcessStylusInput(globals->prefs,
                               (event->eType == penDownEvent), event->screenX,
                               event->screenY);

        // we have handled this event, lets continue
        processed = true;
      }
      break;

    case menuEvent:

      // what menu?
      switch (event->data.menu.itemID)
      {
        case gameMenuItemPause:
          if (globals->prefs->gameVariables.phaseType < Phase_EnterHighScores)
          {
            globals->prefs->gameVariables.gamePaused =
              !globals->prefs->gameVariables.gamePaused;

            PauseGame(globals->prefs->gameVariables.gamePaused);
          }

          processed = true;
          break;

        case gameMenuItemExit:

          // terminate the game
          GraphicsClear();
          TerminateGame();

          // stop the game in its tracks
          globals->prefs->gameVariables.gamePlaying = false;
          globals->prefs->gameVariables.gamePaused = false;

          // goto the main form
          FrmGotoForm(mainForm);

          processed = true;
          break;

        default:
          break;
      }
      break;

    case ctlSelectEvent:

      switch (event->data.ctlSelect.controlID)
      {
        case gameFormSoundButton:

          // regenerate menu event
          MemSet(event, sizeof(EventType), 0);
          event->eType = menuEvent;
          event->data.menu.itemID = menuItemConfig;
          EvtAddEventToQueue(event);

          processed = true;
          break;

        case gameFormPauseButton:

          // regenerate menu event
          MemSet(event, sizeof(EventType), 0);
          event->eType = menuEvent;
          event->data.menu.itemID = gameMenuItemPause;
          EvtAddEventToQueue(event);

          processed = true;
          break;

        default:
          break;
      }
      break;

    case nilEvent:

      // make sure the active window is the form 
      if (WinGetActiveWindow() == (WinHandle) FrmGetActiveForm())
      {
        GameMusicPlayback();

        // draw the game!
        GameDraw(globals->prefs);

        // play the game!
        GameProcessKeyInput(globals->prefs,
                            (KeyCurrentState() | globals->prefs->stylusKeys));

        globals->prefs->stylusKeys = 0;

        // is the pen being held down? if so, lets post event
        {
          Coord x, y;
          Boolean penDown;

          EvtGetPen(&x, &y, &penDown);

          if (penDown)
          {
            EventType event;

            MemSet(&event, sizeof(EventType), 0);
            event.eType = penMoveEvent;
            event.penDown = true;
            event.screenX = x;
            event.screenY = y;
            EvtAddEventToQueue(&event);
          }
        }
      }
      processed = true;
      break;

    default:
      break;
  }

  return processed;
}

/**
 * The Form:cfigForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
cfigFormEventHandler(EventType * event)
{
  Globals *globals;
  Boolean processed = false;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *) & globals);

  switch (event->eType)
  {
    case frmOpenEvent:
      FrmDrawForm(FrmGetActiveForm());
      {
        FormType *frm;
        UInt16 index;
        ControlType *muteCheckbox, *volumeControl;
        ListType *lstHard1, *lstHard2, *lstHard3, *lstHard4;
        ListType *lstUp, *lstDown, *lstDifficulty;
        ControlType *ctlHard1, *ctlHard2, *ctlHard3, *ctlHard4;
        ControlType *ctlUp, *ctlDown, *ctlDifficulty;
        UInt16 *choices[] = {
          &(globals->prefs->config.ctlKeyLeft),
          &(globals->prefs->config.ctlKeyRight),
          &(globals->prefs->config.ctlKeyJump)
        };

        frm = FrmGetActiveForm();
                                                                // *INDENT-OFF*
        ctlHard1 =
          (ControlType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, cfigFormHardKey1Trigger));
        ctlHard2 =
          (ControlType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, cfigFormHardKey2Trigger));
        ctlHard3 =
          (ControlType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, cfigFormHardKey3Trigger));
        ctlHard4 =
          (ControlType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, cfigFormHardKey4Trigger));
        ctlUp =
          (ControlType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, cfigFormPageUpTrigger));
        ctlDown =
          (ControlType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, cfigFormPageDownTrigger));
        ctlDifficulty =
          (ControlType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, cfigFormDifficultyTrigger));

        lstHard1 =
          (ListType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, cfigFormHardKey1List));
        lstHard2 =
          (ListType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, cfigFormHardKey2List));
        lstHard3 =
          (ListType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, cfigFormHardKey3List));
        lstHard4 =
          (ListType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, cfigFormHardKey4List));
        lstUp =
          (ListType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, cfigFormPageUpList));
        lstDown =
          (ListType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, cfigFormPageDownList));
        lstDifficulty =
          (ListType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, cfigFormDifficultyList));

                                                                // *INDENT-ON*
        LstSetSelection(lstHard1, 0);
        CtlSetLabel(ctlHard1, LstGetSelectionText(lstHard1, 0));
        LstSetSelection(lstHard2, 0);
        CtlSetLabel(ctlHard2, LstGetSelectionText(lstHard2, 0));
        LstSetSelection(lstHard3, 0);
        CtlSetLabel(ctlHard3, LstGetSelectionText(lstHard3, 0));
        LstSetSelection(lstHard4, 0);
        CtlSetLabel(ctlHard4, LstGetSelectionText(lstHard4, 0));
        LstSetSelection(lstDown, 0);
        CtlSetLabel(ctlDown, LstGetSelectionText(lstDown, 0));
        LstSetSelection(lstUp, 0);
        CtlSetLabel(ctlUp, LstGetSelectionText(lstUp, 0));
        LstSetSelection(lstDifficulty, gameVars->initDifficulty);
        CtlSetLabel(ctlDifficulty, LstGetSelectionText(lstDifficulty, gameVars->initDifficulty));

        // show the "current" settings
        for (index = 0; index < 3; index++)
        {
          if ((*(choices[index]) & keyBitHard1) != 0)
          {
            LstSetSelection(lstHard1, index + 1);
            CtlSetLabel(ctlHard1, LstGetSelectionText(lstHard1, index + 1));
          }

          if ((*(choices[index]) & keyBitHard2) != 0)
          {
            LstSetSelection(lstHard2, index + 1);
            CtlSetLabel(ctlHard2, LstGetSelectionText(lstHard2, index + 1));
          }

          if ((*(choices[index]) & keyBitHard3) != 0)
          {
            LstSetSelection(lstHard3, index + 1);
            CtlSetLabel(ctlHard3, LstGetSelectionText(lstHard3, index + 1));
          }

          if ((*(choices[index]) & keyBitHard4) != 0)
          {
            LstSetSelection(lstHard4, index + 1);
            CtlSetLabel(ctlHard4, LstGetSelectionText(lstHard4, index + 1));
          }

          if ((*(choices[index]) & keyBitPageUp) != 0)
          {
            LstSetSelection(lstUp, index + 1);
            CtlSetLabel(ctlUp, LstGetSelectionText(lstUp, index + 1));
          }

          if ((*(choices[index]) & keyBitPageDown) != 0)
          {
            LstSetSelection(lstDown, index + 1);
            CtlSetLabel(ctlDown, LstGetSelectionText(lstDown, index + 1));
          }
        }

                                                                // *INDENT-OFF*
        muteCheckbox =
          (ControlType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, cfigFormMuteCheckbox));
        volumeControl =
          (ControlType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, cfigFormSound0Button + DeviceGetVolume()));
                                                                // *INDENT-ON*

        CtlSetValue(muteCheckbox, DeviceGetMute()? 1 : 0);
        CtlSetValue(volumeControl, 1);

        // save this
        FtrSet(appCreator, ftrGlobalsCfgActiveVol, (UInt32)volumeControl);
      }
      processed = true;
      break;

    case popSelectEvent:

      switch (event->data.popSelect.listID)
      {
        case cfigFormDifficultyList:
          {
            Boolean allow;

            // RULE #1 - must be registered in order to select others
            allow = CHECK_SIGNATURE(globals->prefs);
            if (!allow) FrmAlert(unregisteredAlert);

            // RULE #2 - cannot be playing a game :)
            if ((globals->prefs->gameVariables.gamePlaying) && (allow)) 
            {
              FrmAlert(gameActiveAlert);
              allow = false;
            }

            // revert to the old selection if needed :)
            if (!allow) 
            {
              LstSetSelection(event->data.popSelect.listP, event->data.popSelect.priorSelection);
              CtlSetLabel(event->data.popSelect.controlP, LstGetSelectionText(event->data.popSelect.listP, event->data.popSelect.priorSelection));

              processed = true;
            }
            else
              processed = false;
          }
          break;

        default:
             break;
      }
      break;

    case ctlEnterEvent:

      switch (event->data.ctlEnter.controlID)
      {
        case cfigFormSound0Button:
        case cfigFormSound1Button:
        case cfigFormSound2Button:
        case cfigFormSound3Button:
        case cfigFormSound4Button:
        case cfigFormSound5Button:
        case cfigFormSound6Button:
        case cfigFormSound7Button:
          {
            ControlType *newCtl, *oldCtl;

            newCtl = event->data.ctlEnter.pControl;

            // we dont want an audible beep from the system
            FtrGet(appCreator, ftrGlobalsCfgActiveVol, (UInt32 *)&oldCtl);
            CtlSetValue(oldCtl, 0);
            CtlSetValue(newCtl, 1);
            FtrSet(appCreator, ftrGlobalsCfgActiveVol, (UInt32)newCtl);

            // act as we needed :)
            DeviceSetVolume((event->data.ctlEnter.controlID -
                             cfigFormSound0Button));
            {
              SndCommandType testBeep =
                { sndCmdFreqDurationAmp, 0, 512, 32, sndMaxAmp };
              DevicePlaySound(&testBeep);
            }
          }
          processed = true;
          break;

        default:
          break;
      }
      break;

    case ctlSelectEvent:

      switch (event->data.ctlSelect.controlID)
      {
        case cfigFormMuteCheckbox:
          DeviceSetMute(CtlGetValue(event->data.ctlEnter.pControl) != 0);
          processed = true;
          break;

        case cfigFormOkButton:
          {
            ListType *lstHard1, *lstHard2, *lstHard3, *lstHard4;
            ListType *lstUp, *lstDown, *lstDifficulty;
            UInt16 index;
            FormType *frm;
            UInt16 keySignature;
            UInt16 *choices[] = {
              &(globals->prefs->config.ctlKeyLeft),
              &(globals->prefs->config.ctlKeyRight),
              &(globals->prefs->config.ctlKeyJump)
            };

            frm = FrmGetActiveForm();
                                                                // *INDENT-OFF*
            lstHard1 =
              (ListType *) FrmGetObjectPtr(frm,
                 FrmGetObjectIndex(frm, cfigFormHardKey1List));
            lstHard2 =
              (ListType *) FrmGetObjectPtr(frm,
                 FrmGetObjectIndex(frm, cfigFormHardKey2List));
            lstHard3 =
              (ListType *) FrmGetObjectPtr(frm,
                 FrmGetObjectIndex(frm, cfigFormHardKey3List));
            lstHard4 =
              (ListType *) FrmGetObjectPtr(frm,
                 FrmGetObjectIndex(frm, cfigFormHardKey4List));
            lstUp =
              (ListType *) FrmGetObjectPtr(frm,
                 FrmGetObjectIndex(frm, cfigFormPageUpList));
            lstDown =
              (ListType *) FrmGetObjectPtr(frm,
                 FrmGetObjectIndex(frm, cfigFormPageDownList));
            lstDifficulty =
              (ListType *) FrmGetObjectPtr(frm,
                 FrmGetObjectIndex(frm, cfigFormDifficultyList));
                                                                // *INDENT-ON*

            keySignature =
              ((0x01 << LstGetSelection(lstHard1)) |
               (0x01 << LstGetSelection(lstHard2)) |
               (0x01 << LstGetSelection(lstHard3)) |
               (0x01 << LstGetSelection(lstHard4)) |
               (0x01 << LstGetSelection(lstUp)) |
               (0x01 << LstGetSelection(lstDown)));
            keySignature = keySignature >> 1;    // ignore the '------'

            // only process if good setting is selected.
            if (keySignature == 0x07)
            {

              // sounds preferences
              globals->prefs->config.sndMute = DeviceGetMute();
              globals->prefs->config.sndVolume = DeviceGetVolume();

              // key preferences
              for (index = 0; index < 6; index++)
              {
                *(choices[index]) = 0;
              }
              if (LstGetSelection(lstHard1) != 0)
                *(choices[LstGetSelection(lstHard1) - 1]) |= keyBitHard1;
              if (LstGetSelection(lstHard2) != 0)
                *(choices[LstGetSelection(lstHard2) - 1]) |= keyBitHard2;
              if (LstGetSelection(lstHard3) != 0)
                *(choices[LstGetSelection(lstHard3) - 1]) |= keyBitHard3;
              if (LstGetSelection(lstHard4) != 0)
                *(choices[LstGetSelection(lstHard4) - 1]) |= keyBitHard4;
              if (LstGetSelection(lstUp) != 0)
                *(choices[LstGetSelection(lstUp) - 1]) |= keyBitPageUp;
              if (LstGetSelection(lstDown) != 0)
                *(choices[LstGetSelection(lstDown) - 1]) |= keyBitPageDown;
              gameVars->initDifficulty = LstGetSelection(lstDifficulty);

              // send a close event
              MemSet(event, sizeof(EventType), 0);
              event->eType = frmCloseEvent;
              event->data.frmClose.formID = FrmGetActiveFormID();
              EvtAddEventToQueue(event);
            }
            else
            {
              SndPlaySystemSound(sndError);
            }
          }
          processed = true;
          break;

        case cfigFormCancelButton:

          // revert changes
          DeviceSetMute(globals->prefs->config.sndMute);
          DeviceSetVolume(globals->prefs->config.sndVolume);

          // send a close event
          MemSet(event, sizeof(EventType), 0);
          event->eType = frmCloseEvent;
          event->data.frmClose.formID = FrmGetActiveFormID();
          EvtAddEventToQueue(event);

          processed = true;
          break;

        default:
          break;
      }
      break;

    case frmCloseEvent:

      // clean up
      FtrUnregister(appCreator, ftrGlobalsCfgActiveVol);

      break;

    default:
      break;
  }

  return processed;
}

/**
 * The Form:grayForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
grayFormEventHandler(EventType * event)
{
  Globals *globals;
  Boolean processed = false;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *) & globals);

  switch (event->eType)
  {
    case frmOpenEvent:
      FrmDrawForm(FrmGetActiveForm());
      {
        FormType *frm;
        ControlType *ctlWhite, *ctlLGray, *ctlDGray, *ctlBlack;

        frm = FrmGetActiveForm();
                                                                // *INDENT-OFF*
        ctlWhite =
          (ControlType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, grayWhite1Button));
        ctlLGray =
          (ControlType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, grayLightGray1Button + 
                                    globals->prefs->config.lgray));
        ctlDGray =
          (ControlType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, grayDarkGray1Button +
                                    globals->prefs->config.dgray));
        ctlBlack =
          (ControlType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, grayBlack7Button));
                                                                // *INDENT-ON*

        CtlSetValue(ctlWhite, 1);
        CtlDrawControl(ctlWhite);
        CtlSetValue(ctlLGray, 1);
        CtlDrawControl(ctlLGray);
        CtlSetValue(ctlDGray, 1);
        CtlDrawControl(ctlDGray);
        CtlSetValue(ctlBlack, 1);
        CtlDrawControl(ctlBlack);
      }

      // pre 3.5 - we must 'redraw' form to actually display PUSHBUTTONS
      if (!DeviceSupportsVersion(romVersion3_5))
      {
        FrmDrawForm(FrmGetActiveForm());
      }

      processed = true;
      break;

    case ctlEnterEvent:

      switch (event->data.ctlEnter.controlID)
      {
        case grayLightGray1Button:
        case grayLightGray7Button:
        case grayDarkGray1Button:
        case grayDarkGray7Button:

          // stupid user, they must select one of the other options
          SndPlaySystemSound(sndError);
          processed = true;
      }
      break;

    case ctlSelectEvent:

      switch (event->data.ctlSelect.controlID)
      {
        case grayLightGray2Button:
        case grayLightGray3Button:
        case grayLightGray4Button:
        case grayLightGray5Button:
        case grayLightGray6Button:

          globals->prefs->config.lgray = event->data.ctlEnter.controlID -
            grayLightGray1Button;
          DeviceGrayscale(graySet,
                          &globals->prefs->config.lgray,
                          &globals->prefs->config.dgray);
          processed = true;
          break;

        case grayDarkGray2Button:
        case grayDarkGray3Button:
        case grayDarkGray4Button:
        case grayDarkGray5Button:
        case grayDarkGray6Button:

          globals->prefs->config.dgray = event->data.ctlEnter.controlID -
            grayDarkGray1Button;
          DeviceGrayscale(graySet,
                          &globals->prefs->config.lgray,
                          &globals->prefs->config.dgray);
          processed = true;
          break;

        case grayFormOkButton:
          MemSet(event, sizeof(EventType), 0);
          event->eType = frmCloseEvent;
          event->data.frmClose.formID = FrmGetActiveFormID();
          EvtAddEventToQueue(event);

          processed = true;
          break;

        default:
          break;
      }
      break;

    default:
      break;
  }

  return processed;
}

/**
 * The Form:infoForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
infoFormEventHandler(EventType * event)
{
  Globals *globals;
  Boolean processed = false;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *) & globals);

  switch (event->eType)
  {
    case frmOpenEvent:
      FrmDrawForm(FrmGetActiveForm());
      processed = true;
      break;

    case ctlSelectEvent:

      switch (event->data.ctlSelect.controlID)
      {
        case infoFormOkButton:

          // send a close event
          MemSet(event, sizeof(EventType), 0);
          event->eType = frmCloseEvent;
          event->data.frmClose.formID = FrmGetActiveFormID();
          EvtAddEventToQueue(event);

          processed = true;
          break;

        default:
          break;
      }
      break;

    default:
      break;
  }

  return processed;
}

/**
 * The Form:dvlpForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
dvlpFormEventHandler(EventType * event)
{
  Globals *globals;
  Boolean processed = false;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *) & globals);

  switch (event->eType)
  {
    case frmOpenEvent:
      FrmDrawForm(FrmGetActiveForm());
      processed = true;
      break;

    case ctlSelectEvent:

      switch (event->data.ctlSelect.controlID)
      {
        case dvlpFormOkButton:

          // send a close event
          MemSet(event, sizeof(EventType), 0);
          event->eType = frmCloseEvent;
          event->data.frmClose.formID = FrmGetActiveFormID();
          EvtAddEventToQueue(event);

          processed = true;
          break;

        default:
          break;
      }
      break;

    default:
      break;
  }

  return processed;
}

#ifdef PROTECTION_ON

/**
 * The Form:regiForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
regiFormEventHandler(EventType * event)
{
  Globals *globals;
  Boolean processed = false;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *) & globals);

  switch (event->eType)
  {
    case frmOpenEvent:
      FrmDrawForm(FrmGetActiveForm());

      if (CHECK_SIGNATURE(globals->prefs))
        FrmSetTitle(FrmGetActiveForm(), "Registered - Thank you!");

      // display the HotSync username on the screen (in HEX)
      {
        Char ID[40];
        Char tmp[10], num[3];
        Coord x;
        UInt8 i, checksum;

        FontID font = FntGetFont();

        // initialize
        StrCopy(ID, "");

        // generate strings
        checksum = 0;
        for (i = 0; i < MAX_IDLENGTH; i++)
        {

          checksum ^= (UInt8) globals->prefs->system.hotSyncUsername[i];
          StrIToH(tmp, (UInt8) globals->prefs->system.hotSyncUsername[i]);
          StrNCopy(num, &tmp[StrLen(tmp) - 2], 2);
          num[2] = '\0';
          StrCat(ID, num);
          StrCat(ID, ":");
        }
        StrIToH(tmp, checksum);
        StrNCopy(num, &tmp[StrLen(tmp) - 2], 2);
        num[2] = '\0';
        StrCat(ID, num);

        FntSetFont(boldFont);
        x = (160 - FntCharsWidth(ID, StrLen(ID))) >> 1;
        WinDrawChars(ID, StrLen(ID), x, 68);
        FntSetFont(font);
      }
      processed = true;
      break;

    case ctlSelectEvent:

      switch (event->data.ctlSelect.controlID)
      {
        case regiFormOkButton:

          // send a close event
          MemSet(event, sizeof(EventType), 0);
          event->eType = frmCloseEvent;
          event->data.frmClose.formID = FrmGetActiveFormID();
          EvtAddEventToQueue(event);

          processed = true;
          break;

        default:
          break;
      }
      break;

    default:
      break;
  }

  return processed;
}

/**
 * The Form:rbugForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
rbugFormEventHandler(EventType * event)
{
  Globals *globals;
  Boolean processed = false;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *) & globals);

  switch (event->eType)
  {
    case frmOpenEvent:
      FrmDrawForm(FrmGetActiveForm());

      processed = true;
      break;

    case ctlSelectEvent:

      switch (event->data.ctlSelect.controlID)
      {
        case rbugFormOkButton:

          // send a close event
          MemSet(event, sizeof(EventType), 0);
          event->eType = frmCloseEvent;
          event->data.frmClose.formID = FrmGetActiveFormID();
          EvtAddEventToQueue(event);

          processed = true;
          break;

        default:
          break;
      }
      break;

    default:
      break;
  }

  return processed;
}
#endif

/**
 * The Form:highForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
highFormEventHandler(EventType * event)
{
  Globals *globals;
  Boolean processed = false;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *) & globals);

  switch (event->eType)
  {
    case frmOpenEvent:
      FrmDrawForm(FrmGetActiveForm());
      {
        FormType *frm;
        FieldType *fldScore, *fldPlaying, *fldCode;
        MemHandle memHandle;
        Char scoreStr[10];

        frm = FrmGetActiveForm();

                                                                // *INDENT-OFF*
        fldScore =
          (FieldType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, highFormScoreField));
        fldPlaying =
          (FieldType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, highFormPlayingField));
        fldCode =
          (FieldType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, highFormCodeField));
                                                                // *INDENT-ON*

        // score
        memHandle = MemHandleNew(16);
        if (globals->prefs->gameVariables.internetHighScore != 0)
          StrIToA(MemHandleLock(memHandle),
                  globals->prefs->gameVariables.internetHighScore);
        else
          StrCopy(MemHandleLock(memHandle), "N/A");
        MemHandleUnlock(memHandle);
        FldSetTextHandle(fldScore, memHandle);
        FldDrawField(fldScore);

        // playing 
        memHandle = MemHandleNew(16);
        if (globals->prefs->gameVariables.internetHighScorePhase == maxPhaseNum)
          StrPrintF(MemHandleLock(memHandle), "ALL");
        else
        if (globals->prefs->gameVariables.internetHighScorePhase == 0)
          StrPrintF(MemHandleLock(memHandle), "N/A");
        else
          StrPrintF(MemHandleLock(memHandle), "Phase %2d",
                    globals->prefs->gameVariables.internetHighScorePhase);
        MemHandleUnlock(memHandle);
        FldSetTextHandle(fldPlaying, memHandle);
        FldDrawField(fldPlaying);

        // code 
        memHandle = MemHandleNew(16);
        if ((globals->prefs->gameVariables.internetHighScore != 0) &&
            (globals->prefs->gameVariables.internetHighScorePhase != 0)) 
        {
          GameAdjustmentType adjustType;

#if 0
          StrIToA(scoreStr,
                  globals->prefs->gameVariables.internetHighScore +
                  (globals->prefs->gameVariables.internetHighScorePhase * 10) + 0xcafe);
#else
          StrIToA(scoreStr,
                  globals->prefs->gameVariables.internetHighScore + 0xcafe);
#endif

          // define the adjustment
          adjustType.adjustMode = gameScoreCode;
          adjustType.data.scoreCode.key = scoreStr;
          RegisterAdjustGame(globals->prefs, &adjustType);

          StrIToA(MemHandleLock(memHandle), adjustType.data.scoreCode.code);
        }
        else
          StrCopy(MemHandleLock(memHandle), "N/A");

        MemHandleUnlock(memHandle);
        FldSetTextHandle(fldCode, memHandle);
        FldDrawField(fldCode);
      }
      processed = true;
      break;

    case ctlSelectEvent:

      switch (event->data.ctlSelect.controlID)
      {
        case highFormOkButton:

          // send a close event
          MemSet(event, sizeof(EventType), 0);
          event->eType = frmCloseEvent;
          event->data.frmClose.formID = FrmGetActiveFormID();
          EvtAddEventToQueue(event);

          processed = true;
          break;

        default:
          break;
      }
      break;

    case frmCloseEvent:
      {
        FormType *frm;
        FieldType *fldScore, *fldPlaying, *fldCode;
        MemHandle memHandle;

        frm = FrmGetActiveForm();

                                                                // *INDENT-OFF*
        fldScore =
          (FieldType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, highFormScoreField));
        fldPlaying =
          (FieldType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, highFormPlayingField));
        fldCode =
          (FieldType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, highFormCodeField));
                                                                // *INDENT-ON*

        // score
        memHandle = FldGetTextHandle(fldScore);
        FldSetTextHandle(fldScore, NULL);
        MemHandleFree(memHandle);

        // playing 
        memHandle = FldGetTextHandle(fldPlaying);
        FldSetTextHandle(fldPlaying, NULL);
        MemHandleFree(memHandle);

        // code 
        memHandle = FldGetTextHandle(fldCode);
        FldSetTextHandle(fldCode, NULL);
        MemHandleFree(memHandle);
      }
      break;

    default:
      break;
  }

  return processed;
}

/**
 * The Form:helpForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
helpFormEventHandler(EventType * event)
{
  Globals *globals;
  Boolean processed = false;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *) & globals);

  switch (event->eType)
  {
    case frmOpenEvent:
      {
        UInt16 helpHeight;

        helpHeight = InitInstructions();

        // help exists
        if (helpHeight != 0)
        {
          FormType *frm;
          ScrollBarType *sclBar;
          UInt16 val, min, max, pge;

          frm = FrmGetActiveForm();
          FrmDrawForm(frm);
                                                                // *INDENT-OFF*
          sclBar =
            (ScrollBarType *) FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, helpFormScrollBar));
                                                                // *INDENT-ON*

          SclGetScrollBar(sclBar, &val, &min, &max, &pge);
          val = helpHeight;
          max = (val > pge) ? (val - (pge + 16)) : 0;
          SclSetScrollBar(sclBar, 0, min, max, pge);

          DrawInstructions(0);
        }

        // no help, close form
        else
        {
          EventType newEvent;
          
          MemSet(&newEvent, sizeof(EventType), 0);
          newEvent.eType = frmCloseEvent;
          newEvent.data.frmClose.formID = FrmGetActiveFormID();
          EvtAddEventToQueue(&newEvent);
        }
      }
      processed = true;
      break;

    case ctlSelectEvent:

      switch (event->data.ctlSelect.controlID)
      {
        case helpFormOkButton:

          // send a close event
          MemSet(event, sizeof(EventType), 0);
          event->eType = frmCloseEvent;
          event->data.frmClose.formID = FrmGetActiveFormID();
          EvtAddEventToQueue(event);

          processed = true;
          break;

        default:
          break;
      }
      break;

    case sclRepeatEvent:
      {
        FormType *frm;
        ScrollBarType *sclBar;
        UInt16 val, min, max, pge;

        frm = FrmGetActiveForm();
                                                                // *INDENT-OFF*
        sclBar =
          (ScrollBarType *) FrmGetObjectPtr(frm,
             FrmGetObjectIndex(frm, helpFormScrollBar));
                                                                // *INDENT-ON*

        SclGetScrollBar(sclBar, &val, &min, &max, &pge);
        DrawInstructions(val);
      }
      break;

    case keyDownEvent:

      switch (event->data.keyDown.chr)
      {
        case pageUpChr:
          {
            FormType *frm;
            ScrollBarType *sclBar;
            UInt16 val, min, max, pge;

            frm = FrmGetActiveForm();
                                                                // *INDENT-OFF*
            sclBar =
              (ScrollBarType *) FrmGetObjectPtr(frm,
                 FrmGetObjectIndex(frm, helpFormScrollBar));
                                                                // *INDENT-ON*

            SclGetScrollBar(sclBar, &val, &min, &max, &pge);
            val = (pge > val) ? 0 : (val - pge);
            SclSetScrollBar(sclBar, val, min, max, pge);
            DrawInstructions(val);
          }
          processed = true;
          break;

        case pageDownChr:
          {
            FormType *frm;
            ScrollBarType *sclBar;
            UInt16 val, min, max, pge;

            frm = FrmGetActiveForm();
                                                                // *INDENT-OFF*
            sclBar =
              (ScrollBarType *) FrmGetObjectPtr(frm,
                 FrmGetObjectIndex(frm, helpFormScrollBar));
                                                                // *INDENT-ON*

            SclGetScrollBar(sclBar, &val, &min, &max, &pge);
            val = (max < (val + pge)) ? max : (val + pge);
            SclSetScrollBar(sclBar, val, min, max, pge);
            DrawInstructions(val);
          }
          processed = true;
          break;

        default:
          break;
      }
      break;

    case frmCloseEvent:

      // clean up
      QuitInstructions();
      break;

    default:
      break;
  }

  return processed;
}

/**
 * The Form:xmemForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
xmemFormEventHandler(EventType * event)
{
  Globals *globals;
  Boolean processed = false;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *) & globals);

  switch (event->eType)
  {
    case frmOpenEvent:
      FrmDrawForm(FrmGetActiveForm());
      processed = true;
      break;

    case ctlSelectEvent:

      switch (event->data.ctlSelect.controlID)
      {
        case xmemFormOkButton:

          // send a close event
          {
            EventType event;

            MemSet(&event, sizeof(EventType), 0);
            event.eType = frmCloseEvent;
            event.data.frmClose.formID = FrmGetActiveFormID();
            EvtAddEventToQueue(&event);
          }
          processed = true;
          break;

        default:
          break;
      }
      break;

    default:
      break;
  }

  return processed;
}

/**
 * The Palm Computing Platform initialization routine.
 */
void
InitApplication()
{
  Globals *globals;
  Boolean reset, bad;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *) & globals);

  // load preferences
  {
    UInt16 prefSize;
    Int16 flag;

    // allocate memory for preferences
    globals->prefs = (PreferencesType *) MemPtrNew(sizeof(PreferencesType));
    reset = true;

    // lets see how large the preference is (if it is there)
    prefSize = 0;
    flag = PrefGetAppPreferences(appCreator, 0, NULL, &prefSize, true);

    // we have some preferences, maybe a match :)
    if ((flag != noPreferenceFound) && (prefSize == sizeof(PreferencesType)))
    {
      // extract all the bytes
      PrefGetAppPreferences(appCreator, 0, globals->prefs, &prefSize, true);

      // decrypt the memory chunk (based on the @message)
      RegisterDecryptChunk((UInt8 *) globals->prefs, prefSize, 0x0007, 0x01);

      // decryption screw up? reset
      reset = !CHECK_SIGNATURE(globals->prefs) ||
        (globals->prefs->system.signatureVersion != VERSION);
    }

    // do we need to reset the preferences?
    if (reset)
    {
      // set default values
      prefSize = sizeof(PreferencesType);
      MemSet(globals->prefs, prefSize, 0);

      globals->prefs->system.signatureVersion = VERSION;
      StrCopy(globals->prefs->system.signature, "|HaCkMe|");

      globals->prefs->config.ctlKeyJump = 
        keyBitHard1 | keyBitHard4 | keyBitPageUp;
      globals->prefs->config.ctlKeyLeft  = keyBitHard2;
      globals->prefs->config.ctlKeyRight = keyBitHard3;

      globals->prefs->config.sndMute = false;
      globals->prefs->config.sndVolume = 7;

      globals->prefs->gameVariables.gamePlaying = false;
      globals->prefs->gameVariables.gameVarPtr = (void *)(NULL);

      if (DeviceSupportsGrayscale())
      {
        DeviceGrayscale(grayGet,
                        &globals->prefs->config.lgray,
                        &globals->prefs->config.dgray);
      }
    }
  }

  // get the HotSync user name
  globals->prefs->system.hotSyncUsername =
    (Char *) MemPtrNew(dlkUserNameBufSize * sizeof(Char));
  MemSet(globals->prefs->system.hotSyncUsername, dlkUserNameBufSize, 0);
  DlkGetSyncInfo(NULL, NULL, NULL,
                 globals->prefs->system.hotSyncUsername, NULL, NULL);
  {
    Char *ptrStr;

    ptrStr = StrChr(globals->prefs->system.hotSyncUsername, spaceChr);
    if (ptrStr != NULL)
    {
      // erase everything after the FIRST space
      UInt8 index = ((UInt32) ptrStr -
                     (UInt32) globals->prefs->system.hotSyncUsername);
      MemSet(ptrStr, dlkUserNameBufSize - index, 0);
    }

    ptrStr = StrChr(globals->prefs->system.hotSyncUsername, '\0');
    if (ptrStr != NULL)
    {
      // erase everything after the FIRST null char
      UInt8 index = ((UInt32) ptrStr -
                     (UInt32) globals->prefs->system.hotSyncUsername);
      MemSet(ptrStr, dlkUserNameBufSize - index, 0);
    }
  }

  // configure grayscale registers
  if (DeviceSupportsGrayscale())
  {
    DeviceGrayscale(graySet,
                    &globals->prefs->config.lgray,
                    &globals->prefs->config.dgray);
  }

  // setup sound config
  DeviceSetMute(globals->prefs->config.sndMute);
  DeviceSetVolume(globals->prefs->config.sndVolume);

  // setup the valid keys available while the game is running
  KeySetMask(~(keyBitsAll ^ (keyBitPower | keyBitCradle |
                             keyBitAntenna | keyBitContrast)));

  // initialize the game environemnt
  RegisterInitialize(globals->prefs);

  // initialize the game
  bad = InitializeGameGlobals(&globals->prefs->gameVariables);
  GameInitialize();

  if (CHECK_SIGNATURE(globals->prefs)) {
    if (gameVars->nextTilePhase == (phaseTitle - 1)) {
      bad = true;
    }
  }

  globals->evtTimeOut    = evtWaitForever;
  globals->ticksPerFrame = ((SysTicksPerSecond() * 10) / GAME_FPS);

  if (!reset && !bad) {
    bad = InitializeDrawers();
  }

  if (reset || bad) {
    InitializeWorld(globals->prefs, globals->ticksPerFrame, reset);
  } else {
    if (globals->prefs->gameVariables.gamePaused) {
      gameVars->pauseTime = 0;
      UpdateWorld();
      gameVars->pauseTime = gameVars->nextPeriodTime;
      DrawPause();
    }
  }

  // goto the appropriate form
  if (globals->prefs->gameVariables.gamePlaying)
  {
    FrmGotoForm(gameForm);
  }
  else
  {
    FrmGotoForm(mainForm);
  }
}

/**
 * The Palm Computing Platform entry routine (mainline).
 *
 * @param cmd         a word value specifying the launch code.
 * @param cmdPBP      pointer to a structure associated with the launch code.
 * @param launchFlags additional launch flags.
 * @return zero if launch successful, non zero otherwise.
 */
UInt32
PilotMain(UInt16 cmd,
          MemPtr cmdPBP,
          UInt16 launchFlags)
{
  UInt32 result = 0;

  // what type of launch was this?
  switch (cmd)
  {
    case sysAppLaunchCmdNormalLaunch:
      {
        // is this device compatable?
        if (DeviceCheckCompatability())
        {
          Globals *globals;
          Boolean ok;

          // initialize the device
          DeviceInitialize();

          // create a globals object, and register it
          globals = (Globals *) MemPtrNew(sizeof(Globals));
          MemSet(globals, sizeof(Globals), 0);
          FtrSet(appCreator, ftrGlobals, (UInt32) globals);

          ok = GraphicsInitialize();
          if (ok)
          {
            InitApplication();
            EventLoop();
            EndApplication();
          }
          GraphicsTerminate();

          // must tell use no memory left :(
          if (!ok)
            ApplicationDisplayDialog(xmemForm);

          // unregister the feature
          MemPtrFree(globals);
          FtrUnregister(appCreator, ftrGlobals);

          // restore device state
          DeviceTerminate();
        }
      }
      break;

    default:
      break;
  }

  return result;
}

/**
 * The application event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
Boolean
ApplicationHandleEvent(EventType * event)
{
  Globals *globals;
  Boolean processed = false;

  // get a globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *) & globals);

  switch (event->eType)
  {
    case frmLoadEvent:
      {
        UInt16 formID = event->data.frmLoad.formID;
        FormType *frm = FrmInitForm(formID);

        FrmSetActiveForm(frm);
        switch (formID)
        {
          case mainForm:
            FrmSetEventHandler(frm,
                               (FormEventHandlerPtr) mainFormEventHandler);

            processed = true;
            break;

          case gameForm:
            FrmSetEventHandler(frm,
                               (FormEventHandlerPtr) gameFormEventHandler);

            processed = true;
            break;

          case infoForm:
            FrmSetEventHandler(frm,
                               (FormEventHandlerPtr) infoFormEventHandler);

            processed = true;
            break;

          case dvlpForm:
            FrmSetEventHandler(frm,
                               (FormEventHandlerPtr) dvlpFormEventHandler);

            processed = true;
            break;

          case cfigForm:
            FrmSetEventHandler(frm,
                               (FormEventHandlerPtr) cfigFormEventHandler);

            processed = true;
            break;

          case grayForm:
            FrmSetEventHandler(frm,
                               (FormEventHandlerPtr) grayFormEventHandler);

            processed = true;
            break;

#ifdef PROTECTION_ON
          case regiForm:
            FrmSetEventHandler(frm,
                               (FormEventHandlerPtr) regiFormEventHandler);

            processed = true;
            break;

          case rbugForm:
            FrmSetEventHandler(frm,
                               (FormEventHandlerPtr) rbugFormEventHandler);

            processed = true;
            break;
#endif
          case highForm:
            FrmSetEventHandler(frm,
                               (FormEventHandlerPtr) highFormEventHandler);

            processed = true;
            break;

          case helpForm:
            FrmSetEventHandler(frm,
                               (FormEventHandlerPtr) helpFormEventHandler);

            processed = true;
            break;

          case xmemForm:
            FrmSetEventHandler(frm,
                               (FormEventHandlerPtr) xmemFormEventHandler);

            processed = true;
            break;

          default:
            break;
        }
      }
      break;

    case winEnterEvent:
      {
        if ((event->data.winEnter.enterWindow ==
             (WinHandle) FrmGetFormPtr(gameForm))
            || (event->data.winEnter.enterWindow ==
                (WinHandle) FrmGetFormPtr(mainForm)))
        {

          // when game screen is active, animate
          globals->evtTimeOut = 1;
          processed = true;
        }
      }
      break;

    case winExitEvent:
      {
        if ((event->data.winExit.exitWindow ==
             (WinHandle) FrmGetFormPtr(gameForm))
            || (event->data.winExit.exitWindow ==
                (WinHandle) FrmGetFormPtr(mainForm)))
        {

          // when game screen is not active, stop animation
          globals->evtTimeOut = evtWaitForever;
          processed = true;
        }
      }
      break;

    case menuEvent:

      switch (event->data.menu.itemID)
      {
        case menuItemGrayscale:
          ApplicationDisplayDialog(grayForm);
          processed = true;
          break;

        case menuItemConfig:
          ApplicationDisplayDialog(cfigForm);
          processed = true;
          break;

#ifdef PROTECTION_ON
        case menuItemRegister:
          ApplicationDisplayDialog(regiForm);
          RegisterShowMessage(globals->prefs);
          processed = true;
          break;
#endif
        case menuItemHelp:
          ApplicationDisplayDialog(helpForm);
          processed = true;
          break;

        case menuItemDeveloper:
          ApplicationDisplayDialog(dvlpForm);
          processed = true;
          break;

        case menuItemAbout:
          ApplicationDisplayDialog(infoForm);
          processed = true;
          break;

        default:
          break;
      }
      break;

    case ctlSelectEvent:

      switch (event->data.ctlSelect.controlID)
      {
        case globalFormHelpButton:

          // regenerate menu event
          MemSet(event, sizeof(EventType), 0);
          event->eType = menuEvent;
          event->data.menu.itemID = menuItemHelp;
          EvtAddEventToQueue(event);

          processed = true;
          break;

        case globalFormAboutButton:

          // regenerate menu event
          MemSet(event, sizeof(EventType), 0);
          event->eType = menuEvent;
          event->data.menu.itemID = menuItemAbout;
          EvtAddEventToQueue(event);

          processed = true;
          break;

        default:
          break;
      }
      break;

    default:
      break;
  }

  return processed;
}

/**
 * Display a MODAL dialog to the user (this is a modified FrmDoDialog)
 *
 * @param formID the ID of the form to display.
 */
void
ApplicationDisplayDialog(UInt16 formID)
{
  const CustomPatternType erase = { 0, 0, 0, 0, 0, 0, 0, 0 };
  const RectangleType rect = { {0, 0}, {160, 160} };

  Globals *globals;
  FormActiveStateType frmCurrState;
  FormType *frmActive = NULL;
  WinHandle winDrawWindow = NULL;
  WinHandle winActiveWindow = NULL;

  RGBColorType currPalette[16];
  RGBColorType palette[] = {
                                                              // *INDENT-OFF*
                                       { 0, 0xff, 0xff, 0xff},
                                       { 1, 0x80, 0x80, 0x80},
                                       { 2, 0x80, 0x00, 0x00},
                                       { 3, 0x80, 0x80, 0x00},
                                       { 4, 0x00, 0x80, 0x00},
                                       { 5, 0x00, 0x80, 0x80},
                                       { 6, 0x00, 0x00, 0x80},
                                       { 7, 0x80, 0x00, 0x80},
                                       { 8, 0xff, 0x00, 0xff},
                                       { 9, 0xc0, 0xc0, 0xc0},
                                       {10, 0xff, 0x00, 0x00},
                                       {11, 0xff, 0xff, 0x00},
                                       {12, 0x00, 0xff, 0x00},
                                       {13, 0x00, 0xff, 0xff},
                                       {14, 0x00, 0x00, 0xff},
                                       {15, 0x00, 0x00, 0x00}
                                                              // *INDENT-ON*
  };

  // get a globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *) & globals);

  // save the active form/window
  if (DeviceSupportsVersion(romVersion3))
  {
    FrmSaveActiveState(&frmCurrState);
  }
  else
  {
    frmActive = FrmGetActiveForm();
    winDrawWindow = WinGetDrawWindow();
    winActiveWindow = WinGetActiveWindow();      // < palmos3.0, manual work
  }

  // and tweak a VGA 16 color palette
  if (DeviceSupportsVersion(romVersion3_5) && DeviceSupportsColor())
  {
    // save the current palette
    WinPalette(winPaletteGet, 0, 16, currPalette);

    // clear the LCD screen (dont want palette flash)
    WinSetPattern(&erase);
    WinFillRectangle(&rect, 0);

    // change to standard 16color palette
    WinPalette(winPaletteSet, 0, 16, palette);
  }

  {
    EventType event;
    UInt16 err;
    Boolean keepFormOpen;

    MemSet(&event, sizeof(EventType), 0);

    // send a load form event
    event.eType = frmLoadEvent;
    event.data.frmLoad.formID = formID;
    EvtAddEventToQueue(&event);

    // send a open form event
    event.eType = frmOpenEvent;
    event.data.frmLoad.formID = formID;
    EvtAddEventToQueue(&event);

    // handle all events here (trap them before the OS does) :)
    keepFormOpen = true;
    while (keepFormOpen)
    {
      EvtGetEvent(&event, globals->evtTimeOut);

      // this is our exit condition! :)
      keepFormOpen = (event.eType != frmCloseEvent);
      if (!keepFormOpen) {

        // restore the old palette
        if (DeviceSupportsVersion(romVersion3_5) && DeviceSupportsColor())
        {
          // clear the LCD screen (dont want palette flash)
          WinSetPattern(&erase);
          WinFillRectangle(&rect, 0);

          // restore the original palette
          WinPalette(winPaletteSet, 0, 16, currPalette);
        }
      }

      if (!SysHandleEvent(&event))
      {
        if (!MenuHandleEvent(0, &event, &err))
        {
          if (!ApplicationHandleEvent(&event))
          {
            FrmDispatchEvent(&event);
          }
        }
      }

      if (event.eType == appStopEvent)
      {
        keepFormOpen = false;
        EvtAddEventToQueue(&event);              // tap "applications", exit
      }
    }
  }

  // restore the active form/window
  if (DeviceSupportsVersion(romVersion3))
  {
    FrmRestoreActiveState(&frmCurrState);
  }
  else
  {
    FrmSetActiveForm(frmActive);
    WinSetDrawWindow(winDrawWindow);
    WinSetActiveWindow(winActiveWindow);         // < palmos3.0, manual work
  }

  // post an "update" event for the currently active form
  {
    EventType event;

    MemSet(&event, sizeof(EventType), 0);
    event.eType = frmUpdateEvent;
    event.data.frmClose.formID = FrmGetActiveFormID();
    EvtAddEventToQueue(&event);
  }
}

/**
 * The Palm Computing Platform event processing loop.
 */
void
EventLoop()
{
  Globals *globals;
  EventType event;
  FormType *frm;
  UInt16 err;

  // get a globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *) & globals);

  // reset the timer (just in case)
  globals->timerPointA = TimGetTicks();

  do
  {
    EvtGetEvent(&event, globals->evtTimeOut);
    frm = FrmGetActiveForm();

    //
    // ANIMATION HANDLING:
    //

    // nil event occured, register time state
    if (
        (WinGetActiveWindow() == (WinHandle)frm) && 
        (
         (frm == FrmGetFormPtr(mainForm)) ||
         (frm == FrmGetFormPtr(gameForm))
        ) &&
        (event.eType == nilEvent)
       )
    {
      globals->timerPointA = TimGetTicks();
    }

    //
    // EVENT HANDLING:
    //

    if (!SysHandleEvent(&event))
    {
      if (!MenuHandleEvent(0, &event, &err))
      {
        if (!ApplicationHandleEvent(&event))
        {
          FrmDispatchEvent(&event);
        }
      }

      GameCheckStartButton();
    }

    //
    // ANIMATION HANDLING:
    //

    // on a form that requires animations, calc time since last nilEvent
    if (
        (WinGetActiveWindow() == (WinHandle)frm) && 
        (
         (frm == FrmGetFormPtr(mainForm)) ||
         (frm == FrmGetFormPtr(gameForm))
        ) 
       )
    {
      globals->timerPointB = TimGetTicks();

      // calculate the delay required
      globals->timerDiff = (globals->timerPointB - globals->timerPointA);
      globals->evtTimeOut = (globals->timerDiff > globals->ticksPerFrame) ?
        1 : (globals->ticksPerFrame - globals->timerDiff);

      // manually add nilEvent if needed (only when pen held down)
      if ((globals->evtTimeOut <= 1) && (event.eType == penMoveEvent))
      {
        EventType event;

        MemSet(&event, sizeof(EventType), 0);
        event.eType = nilEvent;
        EvtAddEventToQueue(&event);
      }
    }
  } while (event.eType != appStopEvent);
}

/**
 * The Palm Computing Platform termination routine.
 */
void
EndApplication()
{
  Globals *globals;
  UInt16 prefSize;

  // get a globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *)&globals);

  // restore the key state
  KeySetMask(keyBitsAll);

  // checksum game
  gameVars->gameVarPtr = gameVars;
  gameVars->versionNum = VERSION;
  InitializeGameGlobals(gameVars);

  // save preferences
  MemPtrFree(globals->prefs->system.hotSyncUsername);
  globals->prefs->system.hotSyncUsername = NULL;

  prefSize = sizeof(PreferencesType);
  StrCopy(globals->prefs->system.signature, "|HaCkMe|");
  RegisterDecryptChunk((UInt8 *) globals->prefs, prefSize, 0x0007, 0x01);
  PrefSetAppPreferences(appCreator, 0, 1, globals->prefs, prefSize, true);

  // terminate the game environemnt
  RegisterTerminate();

  // terminate the game
  GameTerminate();

  // ensure all forms are closed
  FrmCloseAllForms();

  // clean up
  MemPtrFree(globals->prefs);
}

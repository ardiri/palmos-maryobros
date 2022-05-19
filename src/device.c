
/*
 * @(#)device.c
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

#include "device.inc"

/**
 * Check the compatability status of the device we are working with.
 *
 * @return true if the device is supported, false otherwise.
 */
Boolean
DeviceCheckCompatability()
{
  Boolean result = true;

                                                                // *INDENT-OFF*
  // the device is only compatable if rom 3.0 or higher is available
  result = (DeviceSupportsVersion(romVersion3) &&
            ((DeviceGetSupportedDepths() & 0x02) != 0));
                                                                // *INDENT-ON*

  // not compatable :(
  if (!result)
  {
    FormType *form;

    // display a incompatability dialog
    form = FrmInitForm(deviForm);
    FrmDoDialog(form);
    FrmDeleteForm(form);

    // lets exit the application "gracefully" :>
    if (!DeviceSupportsVersion(romVersion2))
    {
      AppLaunchWithCommand(sysFileCDefaultApp,
                           sysAppLaunchCmdNormalLaunch, NULL);
    }
  }

  return result;
}

/**
 * Initialize the device.
 */
void
DeviceInitialize()
{
  DeviceGlobals *gbls;

  // create the globals objects, and register it
  gbls = (DeviceGlobals *) MemPtrNew(sizeof(DeviceGlobals));
  MemSet(gbls, sizeof(DeviceGlobals), 0);
  FtrSet(appCreator, ftrDeviceGlobals, (UInt32) gbls);

  // get the rom version and ram size for this device
  FtrGet(sysFtrCreator, sysFtrNumROMVersion, &gbls->romVersion);

  // only OS 3.0 and above have > 1bpp display via API's
  if (DeviceSupportsVersion(romVersion3))
  {
    // save the current display state
    WinScreenMode(winScreenModeGet, NULL, NULL, &gbls->depthState, NULL);

    // for handera

    // change into the "highest" possible mode :P
    {
      UInt32 depthsToTry[] = { 8, 2 };
      UInt32 *depthPtr = &depthsToTry[0];

      // loop until a valid mode is found
      while (WinScreenMode(winScreenModeSet, NULL, NULL, depthPtr, NULL))
      {
        // try the next depth
        depthPtr++;
      }

      gbls->colorState = (*depthPtr == 8);

      // did we make it into 8bpp mode? = color
      if (gbls->colorState)
      {
        // lets change back to 4bpp grayscale
        *depthPtr = 4;
        WinScreenMode(winScreenModeSet, NULL, NULL, depthPtr, NULL);

        // and tweak a VGA 16 color palette
        // -- done later :P
      }
    }
  }
}

/**
 * Determine if the "DeviceGrayscale" routine is supported on the device.
 *
 * @return true if supported, false otherwise.
 */
Boolean
DeviceSupportsGrayscale()
{
  Boolean result = false;

  // only OS 3.0 and above have > 1bpp display via API's
  if (DeviceSupportsVersion(romVersion3))
  {
    UInt32 cpuID;
    UInt32 scrDepth;

    // get the processor ID
    FtrGet(sysFtrCreator, sysFtrNumProcessorID, &cpuID);
    cpuID = cpuID & sysFtrNumProcessorMask;

    // get the current "display" depth
    WinScreenMode(winScreenModeGet, NULL, NULL, &scrDepth, NULL);

                                                                // *INDENT-OFF*
    // the "rules" for grayscale support
    result = ((cpuID == sysFtrNumProcessor328) || 
              (cpuID == sysFtrNumProcessorEZ) || 
              (cpuID == sysFtrNumProcessorVZ)
             ) && (scrDepth == 2);                // must be in 2bpp grayscale!
                                                                // *INDENT-ON*
  }

  return result;
}

/**
 * Grayscale routine/settings for the device.
 *
 * @param mode the desired mode of operation.
 * @param lgray the lGray index (0..6) in intensity
 * @param dgray the dGray index (0..6) in intensity
 */
void
DeviceGrayscale(UInt16 mode,
                UInt8 * lgray,
                UInt8 * dgray)
{
  UInt32 cpuID;

#define LGPMR1 ((unsigned char *)0xFFFFFA32)
#define LGPMR2 ((unsigned char *)0xFFFFFA33)

  // get the processor ID
  FtrGet(sysFtrCreator, sysFtrNumProcessorID, &cpuID);
  switch (cpuID & sysFtrNumProcessorMask)
  {
    case sysFtrNumProcessor328:
      {
        UInt8 gray[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x07 };

        switch (mode)
        {
          case grayGet:
            {
              UInt8 data;

              // light gray
              data = (UInt8) (((*LGPMR1) & 0xF0) >> 4);
              *lgray = 0;
              while (gray[*lgray] < data)
              {
                (*lgray)++;
              }

              // dark gray
              data = (UInt8) ((*LGPMR2) & 0x0F);
              *dgray = 0;
              while (gray[*dgray] < data)
              {
                (*dgray)++;
              }
            }
            break;

          case graySet:
            *LGPMR1 = (gray[*lgray] << 4);
            *LGPMR2 = (0x70 | gray[*dgray]);
            break;

          default:
            break;
        }
      }
      break;

    case sysFtrNumProcessorEZ:
    case sysFtrNumProcessorVZ:
      {
        UInt8 gray[] = { 0x00, 0x03, 0x04, 0x07, 0x0A, 0x0C, 0x0F };

        switch (mode)
        {
          case grayGet:
            {
              UInt8 data;

              // light gray
              data = (UInt8) ((*LGPMR2) & 0x0F);
              *lgray = 0;
              while (gray[*lgray] < data)
              {
                (*lgray)++;
              }

              // dark gray
              data = (UInt8) (((*LGPMR2) & 0xF0) >> 4);
              *dgray = 0;
              while (gray[*dgray] < data)
              {
                (*dgray)++;
              }
            }
            break;

          case graySet:
            *LGPMR2 = ((gray[*dgray] << 4) | gray[*lgray]);
            break;

          default:
            break;
        }
      }
      break;

    default:
      break;
  }
}

/**
 * Play a sound on the device.
 *
 * @param sndCmd the sound information.
 */
void
DevicePlaySound(SndCommandType * sndCmd)
{
  DeviceGlobals *gbls;

  // get a globals reference
  FtrGet(appCreator, ftrDeviceGlobals, (UInt32 *) & gbls);

  // are we muted?
  if (!gbls->deviceMuted)
  {
    UInt16 volumes[] = { 0x00, 0x04, 0x08, 0x10, 0x18, 0x20, 0x30, 0x40 };

    // adjust to the right volume
    sndCmd->param3 = volumes[gbls->deviceVolume];

    // do it!
    SndDoCmd(0, sndCmd, 0);
  }
}

/**
 * Set the volume on the device.
 *
 * @param volume the volume level for the device.
 */
void
DeviceSetVolume(UInt16 volume)
{
  DeviceGlobals *gbls;

  // get a globals reference
  FtrGet(appCreator, ftrDeviceGlobals, (UInt32 *) & gbls);

  // save the volume state
  gbls->deviceVolume = volume;
}

/**
 * Get the volume on the device.
 *
 * @return the volume level for the device.
 */
UInt16
DeviceGetVolume()
{
  DeviceGlobals *gbls;

  // get a globals reference
  FtrGet(appCreator, ftrDeviceGlobals, (UInt32 *) & gbls);

  // return the volume state
  return gbls->deviceVolume;
}

/**
 * Set the mute status of the sound on the device.
 *
 * @param mute true if no sound wanted, false otherwise.
 */
void
DeviceSetMute(Boolean mute)
{
  DeviceGlobals *gbls;

  // get a globals reference
  FtrGet(appCreator, ftrDeviceGlobals, (UInt32 *) & gbls);

  // save the mute state
  gbls->deviceMuted = mute;
}

/**
 * Get the mute status of the sound on the device.
 *
 * @return true if no sound wanted, false otherwise.
 */
Boolean
DeviceGetMute()
{
  DeviceGlobals *gbls;

  // get a globals reference
  FtrGet(appCreator, ftrDeviceGlobals, (UInt32 *) & gbls);

  // return the mute state
  return gbls->deviceMuted;
}

/**
 * Reset the device to its original state.
 */
void
DeviceTerminate()
{
  DeviceGlobals *gbls;

  // get a globals reference
  FtrGet(appCreator, ftrDeviceGlobals, (UInt32 *) & gbls);

  // restore the current display state
  if (DeviceSupportsVersion(romVersion3))
  {
    WinScreenMode(winScreenModeSet, NULL, NULL, &gbls->depthState, NULL);
  }

  // clean up memory
  MemPtrFree(gbls);

  // unregister global data
  FtrUnregister(appCreator, ftrDeviceGlobals);
}

/**
 * Get the supported depths the device can handle. 
 *
 * @return the depths supported (1011b = 2^3 | 2^1 | 2^0 = 4,2,1 bpp).
 */
UInt32
DeviceGetSupportedDepths()
{
  UInt32 result = 0x00000001;

  // only OS 3.0 and above have > 1bpp display via API's
  if (DeviceSupportsVersion(romVersion3))
  {
    WinScreenMode(winScreenModeGetSupportedDepths, NULL, NULL, &result, NULL);
  }

  return result;
}

/**
 * Determine the pointer to the bitmap data chunk for a specific window.
 *
 * @param win the window.
 * @return a pointer to the bitmap data chunk.
 */
void *
DeviceWindowGetPointer(WinHandle win)
{
  void *result = NULL;

  // palmos 3.5        - use BmpGetBits()
  if (DeviceSupportsVersion(romVersion3_5))
  {
    result = BmpGetBits(WinGetBitmap(win));
  }

  // palmos pre 3.5    - use standard technique
  else
    result = (void *)win->displayAddrV20;

  return result;
}

/**
 * Check if the device is compatable with a particular ROM version.
 *
 * @param version the ROM version to compare against.
 * @return true if it is compatable, false otherwise.
 */
Boolean
DeviceSupportsVersion(UInt32 version)
{
  UInt32 romVersion;

  // get the rom version and ram size for this device
  FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);

  return (romVersion >= version);
}

/**
 * Check if the device is color capable.
 *
 * @return true if it is capable, false otherwise..
 */
Boolean
DeviceSupportsColor()
{
  DeviceGlobals *gbls;

  // get a globals reference
  FtrGet(appCreator, ftrDeviceGlobals, (UInt32 *) & gbls);

  // return the color state
  return gbls->colorState;
}

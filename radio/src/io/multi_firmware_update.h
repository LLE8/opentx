/*
 * Copyright (C) OpenTX
 *
 * Based on code named
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef OPENTX_MULTI_FIRMWARE_H
#define OPENTX_MULTI_FIRMWARE_H

#include "ff.h"

/* Signature format is multi-[board type]-[bootloader support][check for bootloader][multi telemetry type][telemetry inversion][debug]-[firmware version]
   Where:
   [board type] is avr, stm, or orx
   [bootloader support] b for optiboot (AVR) / USB (STM) or u (unsupported)
   [check for bootloader] is c (CHECK_FOR_BOOTLOADER) or u (undefined)
   [telemetry type] is t (MULTI_TELEMETRY), s (MULTI_STATUS), or u (undefined) for neither
   [telemetry inversion] is i (INVERT_TELEMETRY) or u (undefined)
   [firmware version] is the version padded to two bytes per segment, without seperators e.g. 01020176

   For example: REM multi-stm-bcsid-01020176
*/

class MultiFirmwareInformation {
  public:
    enum MultiFirmwareBoardType {
      FIRMWARE_MULTI_AVR = 0,
      FIRMWARE_MULTI_STM,
      FIRMWARE_MULTI_ORX,
    };

    enum MultiFirmwareTelemetryType {
      FIRMWARE_MULTI_TELEM_NONE = 0,
      FIRMWARE_MULTI_TELEM_MULTI_STATUS,    // erSkyTX
      FIRMWARE_MULTI_TELEM_MULTI_TELEMETRY, // OpenTX
    };

    bool isMultiStmFirmware() const
    {
      return boardType == FIRMWARE_MULTI_STM;
    }

    bool isMultiAvrFirmware() const
    {
      return boardType == FIRMWARE_MULTI_AVR;
    }

    bool isMultiOrxFirmware() const
    {
      return boardType == FIRMWARE_MULTI_ORX;
    }

    bool isMultiWithBootloaderFirmware() const
    {
      return optibootSupport;
    }

    bool isMultiInternalFirmware() const
    {
      return (boardType == FIRMWARE_MULTI_STM && telemetryInversion == false && optibootSupport == true && telemetryType == FIRMWARE_MULTI_TELEM_MULTI_TELEMETRY);
    }

    bool isMultiExternalFirmware() const
    {
      return (boardType == FIRMWARE_MULTI_STM && telemetryInversion == true && optibootSupport == true && telemetryType == FIRMWARE_MULTI_TELEM_MULTI_TELEMETRY);
    }

    const char * readMultiFirmwareInformation(const char * filename);
    const char * readMultiFirmwareInformation(FIL * file);

  private:
    uint8_t boardType;
    uint8_t optibootSupport;
    uint8_t bootloaderCheck;
    uint8_t telemetryType;
    uint8_t telemetryInversion;
    uint8_t firmwareVersionMajor;
    uint8_t firmwareVersionMinor;
    uint8_t firmwareVersionRevision;
    uint8_t firmwareVersionSubRevision;

    const char * readV1Signature(const char * buffer);
    const char * readV2Signature(const char * buffer);
};

bool multiFlashFirmware(uint8_t module, const char * filename);

#endif //OPENTX_MULTI_FIRMWARE_H

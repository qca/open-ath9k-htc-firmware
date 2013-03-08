#ifndef USB_EXTR_H
#define USB_EXTR_H

#if 0
#include "dt_defs.h"
#include "usb_type.h"

/* Variable for USB EP0 pipe (USB.c) */
uint16_t       *pu8DescriptorEX;
uint16_t       u16TxRxCounter;
uint16_t       *u8ConfigDescriptorEX;
//extern BOOLEAN     bUsbEP0HaltSt;
Action      eUsbCxFinishAction;
CommandType eUsbCxCommand;
BOOLEAN     UsbChirpFinish;
uint16_t       u8UsbConfigValue;
uint16_t       u8UsbInterfaceValue;
uint16_t       u8UsbInterfaceAlternateSetting;
uint16_t       u16FirmwareComplete;

uint16_t *UsbDeviceDescriptor;
uint16_t *String00Descriptor;
uint16_t *String10Descriptor;
uint16_t *String20Descriptor;
uint16_t *String30Descriptor;
uint16_t *u8DeviceQualifierDescriptorEX;
uint16_t *u8OtherSpeedConfigDescriptorEX;
uint16_t *u8UsbDeviceDescriptor;
uint16_t *u8String00Descriptor;
uint16_t *u8String10Descriptor;
uint16_t *u8String20Descriptor;
uint16_t *u8String30Descriptor;
#endif
#endif

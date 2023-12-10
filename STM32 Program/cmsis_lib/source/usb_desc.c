/**
  ******************************************************************************
  * @file    usb_desc.c
  * @author  MCD Application Team
  * @version V4.1.0
  * @date    26-May-2017
  * @brief   Descriptors for Joystick Mouse Demo
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "usb_desc.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
const uint8_t Joystick_DeviceDescriptor[JOYSTICK_SIZ_DEVICE_DESC] =
  {
    0x12,                       /*bLength */
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
    0x00,                       /*bcdUSB */
    0x02,
    0x00,                       /*bDeviceClass*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    0x40,                       /*bMaxPacketSize 64*/
    0x47,                       /*idVendor (0x0483)  0x2047*/
    0x20,
    0x01,                       /*idProduct = 0x5710  0x0401*/
    0x04,
    0x00,                       /*bcdDevice rel. 2.00*/
    0x02,
    1,                          /*Index of string descriptor describing
                                                  manufacturer */
    2,                          /*Index of string descriptor describing
                                                 product*/
    3,                          /*Index of string descriptor describing the
                                                 device serial number */
    0x01                        /*bNumConfigurations*/
  }
  ; /* Joystick_DeviceDescriptor */


/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const uint8_t Joystick_ConfigDescriptor[JOYSTICK_SIZ_CONFIG_DESC] =
  {
    0x09, /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
    JOYSTICK_SIZ_CONFIG_DESC,
    /* wTotalLength: Bytes returned */
    0x00,
    0x01,         /*bNumInterfaces: 1 interface*/
    0x01,         /*bConfigurationValue: Configuration value*/
    0x00,         /*iConfiguration: Index of string descriptor describing
                                     the configuration*/
    0xE0,         /*bmAttributes: Self powered */
    0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/

    /************** Descriptor of Joystick Mouse interface ****************/
    /* 09 */
    0x09,         /*bLength: Interface Descriptor size*/
    USB_INTERFACE_DESCRIPTOR_TYPE,/*bDescriptorType: Interface descriptor type*/
    0x00,         /*bInterfaceNumber: Number of Interface*/
    0x00,         /*bAlternateSetting: Alternate setting*/
    0x01,         /*bNumEndpoints*/
    0x03,         /*bInterfaceClass: HID*/
    0x01,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x01,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,            /*iInterface: Index of string descriptor*/
    /******************** Descriptor of Joystick Mouse HID ********************/
    /* 18 */
    0x09,         /*bLength: HID Descriptor size*/
    HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
    0x00,         /*bcdHID: HID Class Spec release number*/
    0x01,
    0x00,         /*bCountryCode: Hardware target country*/
    0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
    0x22,         /*bDescriptorType*/
    JOYSTICK_SIZ_REPORT_DESC,/*wItemLength: Total length of Report descriptor*/
    0x00,
    /******************** Descriptor of Joystick Mouse endpoint ********************/
    /* 27 */
    0x07,          /*bLength: Endpoint Descriptor size*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/

    0x81,          /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,          /*bmAttributes: Interrupt endpoint*/
    0x08,          /*wMaxPacketSize: 8 Byte max */
    0x00,
    0x20,          /*bInterval: Polling Interval (32 ms)*/
    /* 34 */
  }
  ; /* MOUSE_ConfigDescriptor */


const uint8_t GamePad_ConfigDescriptor[GAMEPAD_SIZ_CONFIG_DESC] =
  {
    0x09, /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
    JOYSTICK_SIZ_CONFIG_DESC,
    /* wTotalLength: Bytes returned */
    0x00,
    0x01,         /*bNumInterfaces: 1 interface*/
    0x01,         /*bConfigurationValue: Configuration value*/
    0x00,         /*iConfiguration: Index of string descriptor describing
                                     the configuration*/
    0xE0,         /*bmAttributes: Self powered */
    0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/

    /************** Descriptor of Joystick Mouse interface ****************/
    /* 09 */
    0x09,         /*bLength: Interface Descriptor size*/
    USB_INTERFACE_DESCRIPTOR_TYPE,/*bDescriptorType: Interface descriptor type*/
    0x00,         /*bInterfaceNumber: Number of Interface*/
    0x00,         /*bAlternateSetting: Alternate setting*/
    0x01,         /*bNumEndpoints*/
    0x03,         /*bInterfaceClass: HID*/
    0x01,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x00,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,            /*iInterface: Index of string descriptor*/
    /******************** Descriptor of Joystick Mouse HID ********************/
    /* 18 */
    0x09,         /*bLength: HID Descriptor size*/
    HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
    0x00,         /*bcdHID: HID Class Spec release number*/
    0x01,
    0x00,         /*bCountryCode: Hardware target country*/
    0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
    0x22,         /*bDescriptorType*/
    GAMEPAD_SIZ_REPORT_DESC,/*wItemLength: Total length of Report descriptor*/
    0x00,
    /******************** Descriptor of Joystick Mouse endpoint ********************/
    /* 27 */
    0x07,          /*bLength: Endpoint Descriptor size*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/

    0x81,          /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,          /*bmAttributes: Interrupt endpoint*/
    0x08,          /*wMaxPacketSize: 8 Byte max */
    0x00,
    0x20,          /*bInterval: Polling Interval (32 ms)*/
    /* 34 */
  }
  ; /* MOUSE_ConfigDescriptor */


const uint8_t Joystick_ReportDescriptor[JOYSTICK_SIZ_REPORT_DESC] =
  {

		  0x05, 0x01, // USAGE_PAGE (Generic Desktop)
		  0x09, 0x06, // USAGE (Keyboard)
		  0xa1, 0x01, // COLLECTION (Application)
		  0x05, 0x07, // USAGE_PAGE (Keyboard)
		  0x19, 0xe0, // USAGE_MINIMUM (Keyboard LeftControl)
		  0x29, 0xe7, // USAGE_MAXIMUM (Keyboard Right GUI)
		  0x15, 0x00, // LOGICAL_MINIMUM (0)
		  0x25, 0x01, // LOGICAL_MAXIMUM (1)
		  0x75, 0x01, // REPORT_SIZE (1)
		  0x95, 0x08, // REPORT_COUNT (8)
		  0x81, 0x02, // INPUT (Data,Var,Abs) //1 byte

		  0x95, 0x01, // REPORT_COUNT (1)
		  0x75, 0x08, // REPORT_SIZE (8)
		  0x81, 0x03, // INPUT (Cnst,Var,Abs) //1 byte

		  0x95, 0x06, // REPORT_COUNT (6)
		  0x75, 0x08, // REPORT_SIZE (8)
		  0x15, 0x00, // LOGICAL_MINIMUM (0)
		  0x25, 0x65, // LOGICAL_MAXIMUM (101)
		  0x05, 0x07, // USAGE_PAGE (Keyboard)
		  0x19, 0x00, // USAGE_MINIMUM (Reserved (no event indicated))
		  0x29, 0x65, // USAGE_MAXIMUM (Keyboard Application)
		  0x81, 0x00, // INPUT (Data,Ary,Abs) //6 bytes

		  0xc0 // END_COLLECTION

  }; /* Joystick_ReportDescriptor */

const uint8_t GamePad_ReportDescriptor[GAMEPAD_SIZ_REPORT_DESC] =
  {
		  /*
		  0x05, 0x01, // USAGE_PAGE (Generic Desktop)
		  0x15, 0x00, // LOGICAL_MINIMUM (0)
		  0x09, 0x04, // USAGE (Joystick)
		  0xa1, 0x01, // COLLECTION (Application)
		  0x05, 0x02, // USAGE_PAGE (Simulation Controls)
		  0x09, 0xbb, // USAGE (Throttle)
		  0x15, 0x00, // LOGICAL_MINIMUM (0)
		  0x26, 0xff, 0x00, // LOGICAL_MAXIMUM (255)
		  0x75, 0x08, // REPORT_SIZE (8)
		  0x95, 0x01, // REPORT_COUNT (1)
		  0x81, 0x02, // INPUT (Data,Var,Abs)
		  0x05, 0x09, // USAGE_PAGE (Button)
		  0x19, 0x01, // USAGE_MINIMUM (Button 1)
		  0x29, 0x02, // USAGE_MAXIMUM (Button 2)
		  0x15, 0x00, // LOGICAL_MINIMUM (0)
		  0x25, 0x01, // LOGICAL_MAXIMUM (1)
		  0x75, 0x01, // REPORT_SIZE (1)
		  0x95, 0x08, // REPORT_COUNT (8)
		  0x55, 0x00, // UNIT_EXPONENT (0)
		  0x65, 0x00, // UNIT (None)
		  0x81, 0x02, // INPUT (Data,Var,Abs)
		  0xc0 // END_COLLECTION
		  */

		  0x05, 0x01, //USAGE_PAGE (Generic Desktop)
		  0x15, 0x00,  //LOGICAL_MINIMUM (0)
		  0x09, 0x04, //USAGE (Joystick)
		  0xA1, 0x01,  //COLLECTION (Application)
		  0x05, 0x02, //USAGE_PAGE (Simulation Controls)
		  0x09, 0xBB, //USAGE (Throttle)
		  0x15, 0x81, //LOGICAL_MINIMUM (-127)
		  0x25, 0x7F, //LOGICAL_MAXIMUM (127)
		  0x75, 0x08, //REPORT_SIZE (8)
		  0x95, 0x01, //REPORT_COUNT (1)
		  0x81, 0x02, //INPUT (Data,Var,Abs)
		  0x05, 0x01, //USAGE_PAGE (Generic Desktop)
		  0x09, 0x01, //USAGE (Pointer)
		  0xA1, 0x00, //COLLECTION (Physical)
		  0x09, 0x30, //USAGE (X)
		  0x09, 0x31, //USAGE (Y)
		  0x95, 0x02,  //REPORT_COUNT (2)
		  0x81, 0x02, //INPUT (Data,Var,Abs)
		  0xC0, //END_COLLECTION
		  /*
		  0x09, 0x39, //USAGE (Hat switch)
		  0x15, 0x00,  //LOGICAL_MINIMUM (0)
		  0x25, 0x03,  //LOGICAL_MAXIMUM (3)
		  0x35, 0x00, //PHYSICAL_MINIMUM (0)
		  0x46, 0x0E, 0x01, //PHYSICAL_MAXIMUM (270)
		  0x65, 0x14, //UNIT (Eng Rot:Angular Pos)
		  0x75, 0x04, //REPORT_SIZE (4)
		  0x95, 0x01, //REPORT_COUNT (1)
		  0x81, 0x02, //INPUT (Data,Var,Abs)
		  */
		  0x05, 0x09, //USAGE_PAGE (Button)
		  0x19, 0x01, //USAGE_MINIMUM (Button 1)
		  0x29, 0x01, //USAGE_MAXIMUM (Button 1)
		  0x15, 0x00, //LOGICAL_MINIMUM (0)
		  0x25, 0x01, //LOGICAL_MAXIMUM (1)
		  0x75, 0x08, //REPORT_SIZE (8)
		  0x95, 0x01, //REPORT_COUNT (1)
		 // 0x55, 0x00, //UNIT_EXPONENT (0)
		 // 0x65, 0x00, //UNIT (None)
		  0x81, 0x02, //INPUT (Data,Var,Abs)
		  0xC0 //END_COLLECTION

  }; /* Joystick_ReportDescriptor */




/* USB String Descriptors (optional) */
const uint8_t Joystick_StringLangID[JOYSTICK_SIZ_STRING_LANGID] =
  {
    JOYSTICK_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
  }
  ; /* LangID = 0x0409: U.S. English */

const uint8_t Joystick_StringVendor[JOYSTICK_SIZ_STRING_VENDOR] =
  {
    JOYSTICK_SIZ_STRING_VENDOR, /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
    /* Manufacturer: "STMicroelectronics" */
    'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
    'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
    'c', 0, 's', 0
  };

const uint8_t Joystick_StringProduct[JOYSTICK_SIZ_STRING_PRODUCT] =
  {
    JOYSTICK_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, ' ', 0, 'J', 0,
    'o', 0, 'y', 0, 's', 0, 't', 0, 'i', 0, 'c', 0, 'k', 0
  };
uint8_t Joystick_StringSerial[JOYSTICK_SIZ_STRING_SERIAL] =
  {
    JOYSTICK_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0
  };

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


/*******************************************************************************

Copyright Datapath Ltd. 2007, 2009.

File:    X4.H

Purpose: Definitions for a simple X4 interface.


History:

*******************************************************************************/

#ifndef _X4_H_
#define _X4_H_

/******************************************************************************/

#pragma pack(push)
#pragma pack(1)

/******************************************************************************/

/* The calling convention of the X4 SDK callback functions. */

#define X4CBKAPI __stdcall

/******************************************************************************/

typedef ULONG X4ERROR;
typedef ULONG_PTR HX4DLL, *PHX4DLL;
typedef ULONG_PTR HX4DEVICE, *PHX4DEVICE;

/******************************************************************************/

typedef struct _X4SYSTEMINFO
{
   unsigned long  Size;
   unsigned char  HardwareVersion;
   unsigned char  FPGAVersion;
   unsigned short FirmwareVersion;
}  X4SYSTEMINFO, *PX4SYSTEMINFO;

typedef struct _X4SYSTEMHEALTH
{
   unsigned long  Size;
   unsigned short Temperature;
   unsigned short Volts50;
   unsigned short Volts33;
   unsigned short Volts25;
   unsigned short Volts18;
   unsigned short Volts12;
} X4SYSTEMHEALTH, *PX4SYSTEMHEALTH;

/******************************************************************************/

typedef struct _X4OUTPUT
{
   unsigned long  Size;
   unsigned long  top;
   unsigned long  left;
   unsigned long  right;
   unsigned long  bottom;
   unsigned long  transform;
}  X4OUTPUT, *PX4OUTPUT;

/******************************************************************************/

typedef enum _X4TIMINGSOURCE
{
   MONITOR,
   DEFAULT,
}  X4TIMINGSOURCE, *PX4TIMINGSOURCE;

/******************************************************************************/

typedef enum _X4TRANSFORM
{
   ROTATION_NONE = 0,
   ROTATION_90,
   ROTATION_180,
   ROTATION_270,
   FLIP_HORZ,
   FLIP_VERT,
}  X4TRANSFORM, *PX4TRANSFORM;

/******************************************************************************/

typedef enum _X4DVILINKOVERRIDE
{
   X4FORCESINGLELINK = 0,
   X4FORCEDUALLINK = 1,
   X4DEFAULTLINK = 2,
} X4DVILINKOVERRIDE;

/******************************************************************************/

typedef enum _X4GENLOCK
{
   X4GENLOCK_NONE = 0,
   X4GENLOCK_MONITOR_MASTER,
   X4GENLOCK_INPUT_MASTER,
   X4GENLOCK_UNKNOWN
}  X4GENLOCK, *PX4GENLOCK;

/******************************************************************************/

typedef enum _X4EQUALISATION
{
   X4EQUALISATION_LEVEL0 = 0,
   X4EQUALISATION_LEVEL1,
   X4EQUALISATION_LEVEL2,
   X4EQUALISATION_LEVEL3,
   X4EQUALISATION_LEVEL4,
   X4EQUALISATION_LEVEL5,
   X4EQUALISATION_LEVEL6,
   X4EQUALISATION_LEVEL7,
}  X4EQUALISATION, *PX4EQUALISATION;

/******************************************************************************/

typedef enum _X4SMOOTHNESS {
   X4SMOOTHNESS_LEVEL0 = 0,
   X4SMOOTHNESS_LEVEL1,
   X4SMOOTHNESS_LEVEL2,
   X4SMOOTHNESS_LEVEL3,
   X4SMOOTHNESS_LEVEL4,
} X4SMOOTHNESS, *PX4SMOOTHNESS;

/******************************************************************************/

typedef enum _X4IMAGESOURCE {
   X4IMGSRC_CROPPING,
   X4IMGSRC_TSTPAT_GREYS,
   X4IMGSRC_TSTPAT_COLOUR,
   X4IMGSRC_TSTPAT_GREYMOVE,
   X4IMGSRC_TSTPAT_COLOURMOVE,
} X4IMAGESOURCE, *PX4IMAGESOURCE;

#define IMGSRC_NUM_ENTRIES 4

/******************************************************************************/

#pragma pack(pop)

/******************************************************************************/

#endif //_X4_H_

/******************************************************************************/

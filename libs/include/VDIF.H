/*******************************************************************************

File:    VDIF.h

Purpose: Declarations for interface to Video Display Information Format.

History:
         03 APR 98    JH   Created.

*******************************************************************************/

#ifndef DATAPATH_VDIF_DEFINED
#define DATAPATH_VDIF_DEFINED

#include "vdifdefs.h"

/* This structure is based on the VESA Monitor Timing Specifications. Where
 * information is duplicated, the relevant fields have been left out. For
 * example, blank time can be calculated by adding front porch, sync time and
 * back porch. */
typedef struct tagVDIFA
{
   unsigned long  HorFrequency;     /* Line rate in Hz. */
   unsigned long  VerFrequency;     /* Refresh rate in Hz*1000. */
   unsigned long  PixelClock;       /* Dot clock in Hz. */

   unsigned short Flags;            /* Bitwise OR of VDIF_FLAG_.*. */

   /* The following values are in pixels. */
   unsigned long  HorAddrTime;      /* Amount of active video (resolution). */
   unsigned long  HorRightBorder;
   unsigned long  HorFrontPorch;
   unsigned long  HorSyncTime;
   unsigned long  HorBackPorch;
   unsigned long  HorLeftBorder;

   /* The following values are in lines. */
   unsigned long  VerAddrTime;      /* Amount of active video (resolution). */
   unsigned long  VerBottomBorder;
   unsigned long  VerFrontPorch;
   unsigned long  VerSyncTime;
   unsigned long  VerBackPorch;
   unsigned long  VerTopBorder;

   /* TODO: Could make this [1] or a pointer. */
   char           Description[128];
}  VDIFA, *PVDIFA, *LPVDIFA;

typedef struct tagVDIFW
{
   unsigned long  HorFrequency;     /* Line rate in Hz. */
   unsigned long  VerFrequency;     /* Refresh rate in Hz*1000. */
   unsigned long  PixelClock;       /* Dot clock in Hz. */

   unsigned short Flags;            /* Bitwise OR of VDIF_FLAG_.*. */

   /* The following values are in pixels. */
   unsigned long  HorAddrTime;      /* Amount of active video (resolution). */
   unsigned long  HorRightBorder;
   unsigned long  HorFrontPorch;
   unsigned long  HorSyncTime;
   unsigned long  HorBackPorch;
   unsigned long  HorLeftBorder;

   /* The following values are in lines. */
   unsigned long  VerAddrTime;      /* Amount of active video (resolution). */
   unsigned long  VerBottomBorder;
   unsigned long  VerFrontPorch;
   unsigned long  VerSyncTime;
   unsigned long  VerBackPorch;
   unsigned long  VerTopBorder;

   /* TODO: Could make this [1] or a pointer. */
   wchar_t        Description[128];
}  VDIFW, *PVDIFW, *LPVDIFW;

#ifdef UNICODE
   typedef VDIFW VDIF;
   typedef PVDIFW PVDIF;
   typedef LPVDIFW LPVDIF;
   typedef struct tagVDIFW tagVDIF;
#else
   typedef VDIFA VDIF;
   typedef PVDIFA PVDIF;
   typedef LPVDIFA LPVDIF;
   typedef struct tagVDIFA tagVDIF;
#endif

#ifdef __cplusplus
extern "C" {
#endif

unsigned long
VDIFReadMode (
   const char  *lpModeName,
   VDIF        *pVDIF,
   const char  *lpFileName );

typedef int ( VDIFENUMFN )(
   VDIF  *pVDIF,
   void  *pVoid );

int
VDIFEnumModes (
   const char  *lpFileName,
   VDIFENUMFN  pEnumFn,
   void        *pVoid );

unsigned long
VDIFWriteMode (
   const char  *lpTableName,
   const char  *lpModeName,
   VDIF        *pVDIF,
   const char  *lpFileName );

#ifdef __cplusplus
}
#endif

/******************************************************************************/

#endif

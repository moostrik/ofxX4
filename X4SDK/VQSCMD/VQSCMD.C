/******************************************************************************/
#if ( defined WIN32 )
#include <windows.h>
#elif ( defined OSX )
#include <errno.h>
#include <wchar.h>
#include <datatypes.h>
#include <privprof.h>
#else
// TODO
#endif

/******************************************************************************/

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/******************************************************************************/

#include <api.h>
#include <cmdline.h>
#include <vdif.h>
#include <X4.H>
#include <X4ERROR.H>
#include <X4EASY.H>

/******************************************************************************/

static TCHAR
_Quiet[]          = TEXT("Quiet"),
_InputEdid[]      = TEXT("InputEdid"),
_Equalisation[]   = TEXT("Equalisation"),
_OutputMode1[]    = TEXT("OutputMode1"),
_OutputMode2[]    = TEXT("OutputMode2"),
_OutputMode3[]    = TEXT("OutputMode3"),
_OutputMode4[]    = TEXT("OutputMode4"),
_OutputMode[]     = TEXT("OutputMode"),
_OutputSource1[]  = TEXT("OutputSource1"),
_OutputSource2[]  = TEXT("OutputSource2"),
_OutputSource3[]  = TEXT("OutputSource3"),
_OutputSource4[]  = TEXT("OutputSource4"),
_OutputSource[]   = TEXT("OutputSource"),
_TestPattern[]    = TEXT("TestPattern"),
_TestPattern1[]   = TEXT("TestPattern1"),
_TestPattern2[]   = TEXT("TestPattern2"),
_TestPattern3[]   = TEXT("TestPattern3"),
_TestPattern4[]   = TEXT("TestPattern4"),
_CaptureRegion1[] = TEXT("CaptureRegion1"),
_CaptureRegion2[] = TEXT("CaptureRegion2"),
_CaptureRegion3[] = TEXT("CaptureRegion3"),
_CaptureRegion4[] = TEXT("CaptureRegion4"),
_CaptureRegion[]  = TEXT("CaptureRegion"),
_CaptureRegionDefault[]  = TEXT("CaptureRegionDefault"),
_Commit[]         = TEXT("Commit");

static TCHAR
_CheckInputMode[]    = TEXT("CheckInputMode"),
_CheckOutputMode1[]  = TEXT("CheckOutputMode1"),
_CheckOutputMode2[]  = TEXT("CheckOutputMode2"),
_CheckOutputMode3[]  = TEXT("CheckOutputMode3"),
_CheckOutputMode4[]  = TEXT("CheckOutputMode4"),
_CheckOutputMode[]   = TEXT("CheckOutputMode"),
_CheckGenlock[]      = TEXT("CheckGenlock");

static TCHAR
_Device[]			= TEXT("DeviceNumber"),
_ConfigFile[]		= TEXT("ConfigFile"),
_FriendlyName[]   = TEXT("DeviceName");

unsigned long Quiet = 0;
unsigned long Commit = 0;
unsigned long Error = 0;

long m_EqualisationSupported;

/******************************************************************************/

unsigned long TimingTable[] =
{
   CVT_TIMING_MODE ,
   CVTRB_TIMING_MODE ,
   GTF_TIMING_MODE ,
   SMPTE_TIMING_MODE ,
};

static const int NumberOfTimings = sizeof ( TimingTable ) / sizeof (TimingTable[0]);

/******************************************************************************/

unsigned long FormatTable[] =
{
   0 ,
   VDIF_FLAG_DVI ,
};

static const int NumberOfFormats = sizeof(FormatTable) / sizeof(FormatTable[0]);

/******************************************************************************/

unsigned long SourceTable[] =
{
   MONITOR ,
   DEFAULT ,
};

static const int NumberOfSource = sizeof(SourceTable) / sizeof(SourceTable[0]);

/******************************************************************************/

unsigned long GenlockTable[] =
{
   X4GENLOCK_NONE ,
   X4GENLOCK_MONITOR_MASTER ,
   X4GENLOCK_INPUT_MASTER ,
   X4GENLOCK_UNKNOWN ,
};

static const int NumberOfGenlock = sizeof(GenlockTable) / sizeof(GenlockTable[0]);

/******************************************************************************/

typedef struct ScaledCropping
{
   unsigned long Top;
   unsigned long Left;
   unsigned long Right;
   unsigned long Bottom;
}  ScaledCropping;

typedef struct InputCropping
{
   unsigned long Top;
   unsigned long Left;
   unsigned long Width;
   unsigned long Height;
}  InputCropping;

/******************************************************************************/

// Flags have been expanded to 64 bits to accomodate test pattern flags.
#define COMMAND_FLAG_INPREF            0x00000001
#define COMMAND_FLAG_INEQU             0x00000002
#define COMMAND_FLAG_OUTDEF1           0x00000004
#define COMMAND_FLAG_OUTDEF2           0x00000008
#define COMMAND_FLAG_OUTDEF3           0x00000010
#define COMMAND_FLAG_OUTDEF4           0x00000020
#define COMMAND_FLAG_OUTDEFALL         0x00000040
#define COMMAND_FLAG_OUTSRC1           0x00000080
#define COMMAND_FLAG_OUTSRC2           0x00000100
#define COMMAND_FLAG_OUTSRC3           0x00000200
#define COMMAND_FLAG_OUTSRC4           0x00000400
#define COMMAND_FLAG_OUTSRCALL         0x00000800
#define COMMAND_FLAG_REGION1           0x00001000
#define COMMAND_FLAG_REGION2           0x00002000
#define COMMAND_FLAG_REGION3           0x00004000
#define COMMAND_FLAG_REGION4           0x00008000
#define COMMAND_FLAG_REGIONALL         0x00010000
#define COMMAND_FLAG_REGIONDEF         0x00020000
#define COMMAND_FLAG_INCUR             0x00040000
#define COMMAND_FLAG_OUTCUR1           0x00080000
#define COMMAND_FLAG_OUTCUR2           0x00100000
#define COMMAND_FLAG_OUTCUR3           0x00200000
#define COMMAND_FLAG_OUTCUR4           0x00400000
#define COMMAND_FLAG_OUTCURALL         0x00800000
#define COMMAND_FLAG_GENLOCK           0x01000000
#define COMMAND_FLAG_CONFIGFILE        0x02000000
#define COMMAND_FLAG_DEVICE            0x04000000
#define COMMAND_FLAG_FRIENDLYNAME      0x08000000
#define COMMAND_FLAG_TSTPAT1           0x10000000
#define COMMAND_FLAG_TSTPAT2           0x20000000
#define COMMAND_FLAG_TSTPAT3           0x40000000
#define COMMAND_FLAG_TSTPAT4           0x80000000
#define COMMAND_FLAG_TSTPATALL         0x0000000100000000

typedef struct
{
   int Timing;
   int Width;
   int Height;
   int Refresh;
}  INPREF, *PINPREF;

typedef struct
{
   int Timing;
   int Width;
   int Height;
   int Refresh;
   int Tolerance;
}  INCUR, *PINCUR;

typedef struct
{
   int Timing;
   int Width;
   int Height;
   int Refresh;
   int Format;
}  OUTDEF, *POUTDEF;

typedef struct
{
   int Timing;
   int Width;
   int Height;
   int Refresh;
   int Format;
   int Tolerance;
}  OUTCUR, *POUTCUR;

typedef struct
{
   int Top;
   int Left;
   int Width;
   int Height;
   X4TRANSFORM Transform;
}  REGION, *PREGION;

typedef struct
{
   int      Help;
   INPREF   InPref;
   int      InEqu;
   OUTDEF   OutDef1;
   OUTDEF   OutDef2;
   OUTDEF   OutDef3;
   OUTDEF   OutDef4;
   OUTDEF   OutDefAll;
   int      OutSrc1;
   int      OutSrc2;
   int      OutSrc3;
   int      OutSrc4;
   int      OutSrcAll;
   X4IMAGESOURCE TstPat1;
   X4IMAGESOURCE TstPat2;
   X4IMAGESOURCE TstPat3;
   X4IMAGESOURCE TstPat4;
   X4IMAGESOURCE TstPatAll;
   
   REGION   Region1;
   REGION   Region2;
   REGION   Region3;
   REGION   Region4;
   REGION   RegionAll;
   int      RegionDef;
   
   INCUR    InCur;
   OUTCUR   OutCur1;
   OUTCUR   OutCur2;
   OUTCUR   OutCur3;
   OUTCUR   OutCur4;
   OUTCUR   OutCurAll;
   int      Genlock;
   
   int		Device;
   TCHAR	ConfigFile[255];
   TCHAR FriendlyName[255];
   unsigned long long SpecifiedFlags;
   unsigned long long ValidFlags;
}  COMMANDLINE, *PCOMMANDLINE;

/******************************************************************************/

CMDLNERRFN  CommandLineErrorFn;

unsigned long
CommandLineErrorFn (
                    unsigned long  Error,
                    LPCTSTR        pKeyword,
                    int            nKeywordChars,
                    LPCTSTR        pValue,
                    int            nValueChars,
                    int            nParameter,
                    PKEYWORDTODATA pKeywordToData,
                    PVOID          pVoid )
{
   return 1;
}

/******************************************************************************/

#define VDIF_FLAG_CVT_TIMING        0x0100
#define VDIF_FLAG_CVTRB_TIMING      0x0400
#define VDIF_FLAG_GTF_TIMING        0x0800
#define VDIF_FLAG_SMPTE_TIMING      0x2000

#define IDS_CFG_FILE_ERROR              132
#define IDS_CFG_FILE_ERROR_FORMAT       133

/******************************************************************************/

BOOL
ReadVdifFromFile(
                 TCHAR *Section,
                 VDIF* pVdif,
                 unsigned long *pTiming,
                 TCHAR *file )
{
   TCHAR ValueName[255];
   int     value;
   
   _sntprintf(ValueName, 100, TEXT("HorFrequency"));
   value = GetPrivateProfileInt(Section, ValueName, -1, file);
   
   if (value == -1 )
   {
      return FALSE;
   }
   pVdif->HorFrequency = (unsigned long)value;
   
   _sntprintf(ValueName, 255, TEXT("VerFrequency"));
   value = GetPrivateProfileInt(Section, ValueName, -1, file);
   
   if (value == -1 )
   {
      return FALSE;
   }
   pVdif->VerFrequency = (unsigned long)value;
   
   _sntprintf(ValueName, 255, TEXT("PixelClock"));
   value = GetPrivateProfileInt(Section, ValueName, -1, file);
   
   if (value == -1 )
   {
      return FALSE;
   }
   pVdif->PixelClock = (unsigned long)value;
   
   
   _sntprintf(ValueName, 255, TEXT("Flags"));
   value = GetPrivateProfileInt(Section, ValueName, -1, file);
   
   if (value == -1 )
   {
      return FALSE;
   }
   pVdif->Flags = (unsigned short)value;
   
   _sntprintf(ValueName, 255, TEXT("Timing"));
   value = GetPrivateProfileInt(Section, ValueName, -1, file);
   
   if (value == -1 )
   {
      if ( pVdif->Flags & VDIF_FLAG_CVT_TIMING )
         *pTiming = CVT_TIMING_MODE;
      else if ( *pTiming & VDIF_FLAG_CVTRB_TIMING )
         *pTiming = CVTRB_TIMING_MODE;
      else if ( *pTiming & VDIF_FLAG_GTF_TIMING )
         *pTiming = GTF_TIMING_MODE;
      else if ( *pTiming & VDIF_FLAG_SMPTE_TIMING )
         *pTiming = SMPTE_TIMING_MODE;
      else
         *pTiming = CUSTOM_TIMING_MODE;
   }
   else
      *pTiming = (unsigned short)value;
   
   _sntprintf(ValueName, 255, TEXT("HorAddrTime"));
   value = GetPrivateProfileInt(Section, ValueName, -1, file);
   
   if (value == -1 )
   {
      return FALSE;
   }
   pVdif->HorAddrTime = (unsigned long)value;
   
   _sntprintf(ValueName, 255, TEXT("HorRightBorder"));
   value = GetPrivateProfileInt(Section, ValueName, -1, file);
   
   if (value == -1 )
   {
      return FALSE;
   }
   pVdif->HorRightBorder = (unsigned long)value;
   
   _sntprintf(ValueName, 255, TEXT("HorFrontPorch"));
   value = GetPrivateProfileInt(Section, ValueName, -1, file);
   
   if (value == -1 )
   {
      return FALSE;
   }
   pVdif->HorFrontPorch = (unsigned long)value;
   
   _sntprintf(ValueName, 255, TEXT("HorSyncTime"));
   value = GetPrivateProfileInt(Section, ValueName, -1, file);
   
   if (value == -1 )
   {
      return FALSE;
   }
   pVdif->HorSyncTime = (unsigned long)value;
   
   _sntprintf(ValueName, 255, TEXT("HorBackPorch"));
   value = GetPrivateProfileInt(Section, ValueName, -1, file);
   
   if (value == -1 )
   {
      return FALSE;
   }
   pVdif->HorBackPorch = (unsigned long)value;
   
   _sntprintf(ValueName, 255, TEXT("HorLeftBorder"));
   value = GetPrivateProfileInt(Section, ValueName, -1, file);
   
   if (value == -1 )
   {
      return FALSE;
   }
   pVdif->HorLeftBorder = (unsigned long)value;
   
   _sntprintf(ValueName, 255, TEXT("VerAddrTime"));
   value = GetPrivateProfileInt(Section, ValueName, -1, file);
   
   if (value == -1 )
   {
      return FALSE;
   }
   pVdif->VerAddrTime = (unsigned long)value;
   
   _sntprintf(ValueName, 255, TEXT("VerBottomBorder"));
   value = GetPrivateProfileInt(Section, ValueName, -1, file);
   
   if (value == -1 )
   {
      return FALSE;
   }
   pVdif->VerBottomBorder = (unsigned long)value;
   
   _sntprintf(ValueName, 255, TEXT("VerFrontPorch"));
   value = GetPrivateProfileInt(Section, ValueName, -1, file);
   
   if (value == -1 )
   {
      return FALSE;
   }
   pVdif->VerFrontPorch = (unsigned long)value;
   
   _sntprintf(ValueName, 255, TEXT("VerSyncTime"));
   value = GetPrivateProfileInt(Section, ValueName, -1, file);
   
   if (value == -1 )
   {
      return FALSE;
   }
   pVdif->VerSyncTime = (unsigned long)value;
   
   _sntprintf(ValueName, 255, TEXT("VerBackPorch"));
   value = GetPrivateProfileInt(Section, ValueName, -1, file);
   
   if (value == -1 )
   {
      return FALSE;
   }
   pVdif->VerBackPorch = (unsigned long)value;
   
   _sntprintf(ValueName, 255, TEXT("VerTopBorder"));
   value = GetPrivateProfileInt(Section, ValueName, -1, file);
   
   if (value == -1 )
   {
      return FALSE;
   }
   pVdif->VerTopBorder = (unsigned long)value;
   
   return TRUE;
}

/******************************************************************************/

unsigned long
ConfigurationLoad(
                  HX4DEVICE hDevice,
                  TCHAR * Filename )
{
   TCHAR  Section[255];
   TCHAR  ValueName[255];
   //TCHAR  Value[255];
   VDIF     InputPreferred;
   int      i, ivalue;
   unsigned long  inTiming;
   
   VDIF           OutputDefault[4];
   unsigned long  outTiming[4];
   X4TIMINGSOURCE source[4];
   X4TRANSFORM    transform[4];
   ScaledCropping cropping[4];
   int            equ = -1;
   int            smooth = -1;
#if (DVILINKCONTROLS == 1)
   int            dlm = -1;
#endif
   
   
   //// [Machine 1 Window 1]
   //// [Machine 1 Window 1 Hardware Resources]
   //// Don't give a shit about these for now.
   
   
   //// [Machine 1 Window 1 Input Preferred Timings 1]
   _sntprintf(Section, 255, TEXT("Machine 1 Window 1 Input Preferred Timings 1"));
   if (ReadVdifFromFile(Section, &InputPreferred, &inTiming, Filename) == FALSE)
   {
      //AfxMessageBox(IDS_CFG_FILE_ERROR, MB_OK | MB_ICONERROR );
      printf(TEXT("%d\n"),IDS_CFG_FILE_ERROR);
      return FALSE;
   }
   
   //// [Machine 1 Window 1 Input Equalisation 1]
   _sntprintf(Section, 255, TEXT("Machine 1 Window 1 Input Equalisation Timings 1"));
   _sntprintf(ValueName, 255, TEXT("Equalisation"));
   equ = GetPrivateProfileInt(Section, ValueName, -1, Filename);
   
   
#if (DVILINKCONTROLS == 1)
   //// [Machine 1 Window 1 Input LinkOverride 1]
   printf(Section, TEXT("Machine 1 Window 1 Input LinkOverride Timings 1"));
   _sntprintf(ValueName, 255, TEXT("LinkOverride"));
   dlm = GetPrivateProfileInt(Section, ValueName, -1, Filename);
#endif
   
   //// [Machine 1 Window 1 Input Smoothness 1]
   _sntprintf(Section, 255, TEXT("Machine 1 Window 1 Input Smoothness 1"));
   _sntprintf(ValueName, 255, TEXT("Smoothness"));
   smooth = GetPrivateProfileInt(Section, ValueName, -1, Filename);
   
   
   for (i=0; i<4; i++)
   {
      //// [Machine 1 Window 1 Output Settings 1]
      _sntprintf(Section, 255, TEXT("Machine 1 Window 1 Output Settings %d"), i);
      
      _sntprintf(ValueName, 255, TEXT("TimingSource"));
      ivalue = GetPrivateProfileInt(Section, ValueName, -1, Filename);
      
      if (ivalue == -1 ) break;
      source[i] = (X4TIMINGSOURCE)ivalue;
      
      // This setting is not currently supported
      _sntprintf(ValueName, 255, TEXT("Enable"));
      ivalue = GetPrivateProfileInt(Section, ValueName, -1, Filename);
      
      if (ivalue == -1 ) break;
      
      //// [Machine 1 Window 1 Output Default Timings 1]
      _sntprintf(Section, 255, TEXT("Machine 1 Window 1 Output Default Timings %d"), i);
      
      if (ReadVdifFromFile(Section, &OutputDefault[i], &outTiming[i], Filename) == FALSE)
      {
         ivalue = -1;
         break;
      }
      
      //// [Machine 1 Window 1 Output Image 1]
      _sntprintf(Section, 255, TEXT("Machine 1 Window 1 Output Image %d"), i);
      
      _sntprintf(ValueName, 255, TEXT("CroppingTop"));
      ivalue = GetPrivateProfileInt(Section, ValueName, -1, Filename);
      
      if (ivalue == -1 ) break;
      cropping[i].Top = (unsigned long)ivalue;
      _sntprintf(ValueName, 255,  TEXT("CroppingLeft"));
      ivalue = GetPrivateProfileInt(Section, ValueName, -1, Filename);
      
      if (ivalue == -1 ) break;
      cropping[i].Left = (unsigned long)ivalue;
      _sntprintf(ValueName, 255, TEXT("CroppingWidth"));
      ivalue = GetPrivateProfileInt(Section, ValueName, -1, Filename);
      
      if (ivalue == -1 ) break;
      cropping[i].Right = (unsigned long)ivalue;
      _sntprintf(ValueName, 255, TEXT("CroppingHeight"));
      ivalue = GetPrivateProfileInt(Section, ValueName, -1, Filename);
      
      if (ivalue == -1 ) break;
      cropping[i].Bottom = (unsigned long)ivalue;
      
      _sntprintf(ValueName, 255, TEXT("Transform"));
      ivalue = GetPrivateProfileInt(Section, ValueName, -1, Filename);
      
      if (ivalue == -1 ) break;
      transform[i] = (X4TRANSFORM)ivalue;
   }
   
   
   if (ivalue == -1)
   {
      TCHAR error[10], format[10];
      
      _sntprintf(format, 10, TEXT("%d"),IDS_CFG_FILE_ERROR_FORMAT);
      _sntprintf(error, 10, TEXT("%s %s"), format, Section);
      //AfxMessageBox(error, MB_OK | MB_ICONERROR );
      return FALSE;
   }
   
   X4EasyInputSetPreferredTimings(hDevice, &InputPreferred, inTiming);
   X4EasyInputIsEqualisationSupported(hDevice, &m_EqualisationSupported);
   if (m_EqualisationSupported)
   {
      if (equ != -1)
      {
         X4EasyInputSetEqualisation(hDevice, (X4EQUALISATION)equ);
      }
#if (DVILINKCONTROLS == 1)
      if (dlm != -1)
      {
         InputSetDualLinkMethod((X4DVILINKOVERRIDE)dlm);
      }
#endif
   }
   if (smooth != -1)
   {
      X4EasyDeviceSetSmoothness(hDevice, (X4SMOOTHNESS)smooth);
   }
   
   for (i=0; i<4; i++)
   {
      X4EasyOutputSetDefaultTimings(hDevice, i, &OutputDefault[i], outTiming[i]);
      X4EasyOutputSetTimingSource(hDevice, i, source[i], i);
      X4EasyOutputSetCropping(hDevice, i, cropping[i].Top,
                              cropping[i].Left, cropping[i].Right, cropping[i].Bottom );
      X4EasyOutputSetTransformation(hDevice, i, transform[i]);
   }
   X4EasyDeviceUpdateFlash(hDevice);
   return TRUE;
}

/******************************************************************************/

static unsigned long
ExtractMode (
             LPCTSTR pValue,
             int     nChars,
             PVOID   pData,
             PVOID   pVoid )
{
   PCOMMANDLINE   pCommandLine;
   unsigned long long *pSpecifiedFlagGroup;
   unsigned long long *pValidFlagGroup;
   unsigned long long  flag;
   unsigned long       Error;
   unsigned long  args;
   
   pCommandLine = (PCOMMANDLINE)pData;
   
   switch ( (unsigned long)pVoid - (unsigned long)pData )
   {
      case offsetof ( COMMANDLINE, InPref ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = sizeof(INPREF) / sizeof(int);
         flag = COMMAND_FLAG_INPREF;
         break;
      case offsetof ( COMMANDLINE, InEqu ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = 1;
         flag = COMMAND_FLAG_INEQU;
         break;
      case offsetof ( COMMANDLINE, OutDef1 ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = sizeof(OUTDEF) / sizeof(int);
         flag = COMMAND_FLAG_OUTDEF1;
         break;
      case offsetof ( COMMANDLINE, OutDef2 ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = sizeof(OUTDEF) / sizeof(int);
         flag = COMMAND_FLAG_OUTDEF2;
         break;
      case offsetof ( COMMANDLINE, OutDef3 ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = sizeof(OUTDEF) / sizeof(int);
         flag = COMMAND_FLAG_OUTDEF3;
         break;
      case offsetof ( COMMANDLINE, OutDef4 ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = sizeof(OUTDEF) / sizeof(int);
         flag = COMMAND_FLAG_OUTDEF4;
         break;
      case offsetof ( COMMANDLINE, OutDefAll ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = sizeof(OUTDEF) / sizeof(int);
         flag = COMMAND_FLAG_OUTDEFALL;
         break;
      case offsetof ( COMMANDLINE, OutSrc1 ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = 1;
         flag = COMMAND_FLAG_OUTSRC1;
         break;
      case offsetof ( COMMANDLINE, OutSrc2 ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = 1;
         flag = COMMAND_FLAG_OUTSRC2;
         break;
      case offsetof ( COMMANDLINE, OutSrc3 ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = 1;
         flag = COMMAND_FLAG_OUTSRC3;
         break;
      case offsetof ( COMMANDLINE, OutSrc4 ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = 1;
         flag = COMMAND_FLAG_OUTSRC4;
         break;
      case offsetof ( COMMANDLINE, OutSrcAll ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = 1;
         flag = COMMAND_FLAG_OUTSRCALL;
         break;
         
      case offsetof ( COMMANDLINE, TstPat1 ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = 1;
         flag = COMMAND_FLAG_TSTPAT1;
         break;
      case offsetof ( COMMANDLINE, TstPat2 ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = 1;
         flag = COMMAND_FLAG_TSTPAT2;
         break;
      case offsetof ( COMMANDLINE, TstPat3 ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = 1;
         flag = COMMAND_FLAG_TSTPAT3;
         break;
      case offsetof ( COMMANDLINE, TstPat4 ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = 1;
         flag = COMMAND_FLAG_TSTPAT4;
         break;
      case offsetof ( COMMANDLINE, TstPatAll ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = 1;
         flag = COMMAND_FLAG_TSTPATALL;
         break;
         
         
      case offsetof ( COMMANDLINE, Region1 ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = sizeof(REGION) / sizeof(int);
         flag = COMMAND_FLAG_REGION1;
         break;
      case offsetof ( COMMANDLINE, Region2 ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = sizeof(REGION) / sizeof(int);
         flag = COMMAND_FLAG_REGION2;
         break;
      case offsetof ( COMMANDLINE, Region3 ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = sizeof(REGION) / sizeof(int);
         flag = COMMAND_FLAG_REGION3;
         break;
      case offsetof ( COMMANDLINE, Region4 ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = sizeof(REGION) / sizeof(int);
         flag = COMMAND_FLAG_REGION4;
         break;
      case offsetof ( COMMANDLINE, RegionAll ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = sizeof(REGION) / sizeof(int);
         flag = COMMAND_FLAG_REGIONALL;
         break;
      case offsetof ( COMMANDLINE, RegionDef ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = 1;
         flag = COMMAND_FLAG_REGIONDEF;
         break;
         
      case offsetof ( COMMANDLINE, InCur ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = sizeof(INCUR) / sizeof(int);
         flag = COMMAND_FLAG_INCUR;
         break;
         
      case offsetof ( COMMANDLINE, OutCur1 ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = sizeof(OUTCUR) / sizeof(int);
         flag = COMMAND_FLAG_OUTCUR1;
         break;
         
      case offsetof ( COMMANDLINE, OutCur2 ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = sizeof(OUTCUR) / sizeof(int);
         flag = COMMAND_FLAG_OUTCUR2;
         break;
         
      case offsetof ( COMMANDLINE, OutCur3 ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = sizeof(OUTCUR) / sizeof(int);
         flag = COMMAND_FLAG_OUTCUR3;
         break;
         
      case offsetof ( COMMANDLINE, OutCur4 ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = sizeof(OUTCUR) / sizeof(int);
         flag = COMMAND_FLAG_OUTCUR4;
         break;
         
      case offsetof ( COMMANDLINE, OutCurAll ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = sizeof(OUTCUR) / sizeof(int);
         flag = COMMAND_FLAG_OUTCURALL;
         break;
         
      case offsetof ( COMMANDLINE, Genlock ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = 1;
         flag = COMMAND_FLAG_GENLOCK;
         break;
         
      case offsetof ( COMMANDLINE, Device ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = 1;
         flag = COMMAND_FLAG_DEVICE;
         break;
         
      case offsetof ( COMMANDLINE, ConfigFile ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = 255;
         flag = COMMAND_FLAG_CONFIGFILE;
         break;
         
      case offsetof (COMMANDLINE, FriendlyName ):
         pSpecifiedFlagGroup = &pCommandLine->SpecifiedFlags;
         pValidFlagGroup = &pCommandLine->ValidFlags;
         args = 255;
         flag = COMMAND_FLAG_FRIENDLYNAME;
         break;
   }
   
   if (( *pSpecifiedFlagGroup & flag ) == 0 )
   {
      *pSpecifiedFlagGroup |= flag;
      
      //Changed so a string is read for COMMAND_FLAG_CONFIGFILE and COMMAND_FLAG_FRIENDLYNAME
      switch(flag)
      {
         default:
            Error = ExtractTLWH ( pValue, nChars, (PVOID)args, pVoid );
            break;
         case COMMAND_FLAG_FRIENDLYNAME:
         case COMMAND_FLAG_CONFIGFILE:
            Error = ExtractString ( pValue, nChars, (PVOID)args, pVoid );
            break;
      }
      
      if ( Error == 0 )
      {
         *pValidFlagGroup |= flag;
      }
   }
   else
   {
      Error = CMDLN_KEYWORD_DUPLICATED;
   }
   
   return Error;
}

/******************************************************************************/

unsigned long
ExtractQuiet (
              LPCTSTR pValue,
              int     nValueChars,
              PVOID   pData,
              PVOID   pVoid )
{
   Quiet = 1;
   return 0;
}

/******************************************************************************/

unsigned long
ExtractCommit (
               LPCTSTR pValue,
               int     nValueChars,
               PVOID   pData,
               PVOID   pVoid )
{
   Commit = 1;
   return 0;
}

/******************************************************************************/

unsigned long
ExtractHelp (
             LPCTSTR pValue,
             int     nValueChars,
             PVOID   pData,
             PVOID   pVoid )
{
   PCOMMANDLINE pCommandLine;
   
   pCommandLine = (PCOMMANDLINE)pVoid;
   pCommandLine->Help = 1;
   return 0;
}

/******************************************************************************/

unsigned long
ProcessCommandLine (
#if (defined OSX)
                    int argc,
                    char *argv[],
#endif
                    PCOMMANDLINE   pCommandLine )
{
#if ( defined WIN32 )
   TCHAR          *pBuffer, searchChar;
   unsigned long error = 0;
#else
	int i;
#endif
   
   KEYWORDTODATA  keywordToData[] =
   {
      { _Quiet,   ExtractQuiet, 0, pCommandLine },
      { _Commit,  ExtractCommit, 0, pCommandLine },
      { _InputEdid,  ExtractMode, offsetof(COMMANDLINE, InPref),
         pCommandLine },
      { _Equalisation,   ExtractMode, offsetof(COMMANDLINE, InEqu),
         pCommandLine },
      { _OutputMode1, ExtractMode, offsetof(COMMANDLINE, OutDef1),
         pCommandLine },
      { _OutputMode2, ExtractMode, offsetof(COMMANDLINE, OutDef2),
         pCommandLine },
      { _OutputMode3, ExtractMode, offsetof(COMMANDLINE, OutDef3),
         pCommandLine },
      { _OutputMode4, ExtractMode, offsetof(COMMANDLINE, OutDef4),
         pCommandLine },
      { _OutputMode, ExtractMode, offsetof(COMMANDLINE, OutDefAll),
         pCommandLine },
      { _TestPattern1, ExtractMode, offsetof(COMMANDLINE, TstPat1),
         pCommandLine },
      { _TestPattern2, ExtractMode, offsetof(COMMANDLINE, TstPat2),
         pCommandLine },
      { _TestPattern3, ExtractMode, offsetof(COMMANDLINE, TstPat3),
         pCommandLine },
      { _TestPattern4, ExtractMode, offsetof(COMMANDLINE, TstPat4),
         pCommandLine },
      { _TestPattern, ExtractMode, offsetof(COMMANDLINE, TstPatAll),
         pCommandLine },
      { _OutputSource1, ExtractMode, offsetof(COMMANDLINE, OutSrc1),
         pCommandLine },
      { _OutputSource2, ExtractMode, offsetof(COMMANDLINE, OutSrc2),
         pCommandLine },
      { _OutputSource3, ExtractMode, offsetof(COMMANDLINE, OutSrc3),
         pCommandLine },
      { _OutputSource4, ExtractMode, offsetof(COMMANDLINE, OutSrc4),
         pCommandLine },
      { _OutputSource, ExtractMode, offsetof(COMMANDLINE, OutSrcAll),
         pCommandLine },
      { _CaptureRegion1, ExtractMode, offsetof(COMMANDLINE, Region1),
         pCommandLine },
      { _CaptureRegion2, ExtractMode, offsetof(COMMANDLINE, Region2),
         pCommandLine },
      { _CaptureRegion3, ExtractMode, offsetof(COMMANDLINE, Region3),
         pCommandLine },
      { _CaptureRegion4, ExtractMode, offsetof(COMMANDLINE, Region4),
         pCommandLine },
      { _CaptureRegion, ExtractMode, offsetof(COMMANDLINE, RegionAll),
         pCommandLine },
      { _CaptureRegionDefault, ExtractMode, offsetof(COMMANDLINE, RegionDef),
         pCommandLine },
      { _CheckInputMode,   ExtractMode, offsetof(COMMANDLINE, InCur),
         pCommandLine },
      { _CheckOutputMode1,   ExtractMode, offsetof(COMMANDLINE, OutCur1),
         pCommandLine },
      { _CheckOutputMode2,   ExtractMode, offsetof(COMMANDLINE, OutCur2),
         pCommandLine },
      { _CheckOutputMode3,   ExtractMode, offsetof(COMMANDLINE, OutCur3),
         pCommandLine },
      { _CheckOutputMode4,   ExtractMode, offsetof(COMMANDLINE, OutCur4),
         pCommandLine },
      { _CheckOutputMode,   ExtractMode, offsetof(COMMANDLINE, OutCurAll),
         pCommandLine },
      { _CheckGenlock,   ExtractMode, offsetof(COMMANDLINE, Genlock),
         pCommandLine },
	   { _Device,   ExtractMode, offsetof(COMMANDLINE, Device),
			pCommandLine },
	   { _ConfigFile,   ExtractMode, offsetof(COMMANDLINE, ConfigFile),
			pCommandLine },
      { _FriendlyName, ExtractMode, offsetof(COMMANDLINE, FriendlyName),
         pCommandLine },
      { NULL, NULL, 0, NULL },
   };
   
#if ( defined WIN32 )
   pBuffer = GetCommandLine ( );
   if ( *pBuffer == '"' )
   {
      searchChar = '"';
   }
   else
   {
      searchChar = ' ';
   }
   pBuffer++;
   while ( *pBuffer )
   {
      if ( *pBuffer == searchChar )
      {
         pBuffer++;
         break;
      }
      pBuffer++;
   }
   
   return ParseCommandLine ( pBuffer, keywordToData, pCommandLine,
                            CommandLineErrorFn );
#elif (defined OSX)
   for (i=1; i<argc; i++)
   {
	   if (!ParseCommandLine(argv[i], keywordToData, pCommandLine, CommandLineErrorFn))
         return FALSE;
   }
   return TRUE;
#elif ( defined linux )
   // TODO
#endif
}

/******************************************************************************/

void
InputUnitsToScaled (
                    unsigned long hPixels,
                    unsigned long vLines,
                    InputCropping  *pPixels,
                    ScaledCropping *pScaled )
{
   pScaled->Top = (( pPixels->Top << 16 ) + vLines - 1 ) / vLines;
   pScaled->Left = (( pPixels->Left << 16 ) + hPixels - 1 ) / hPixels;
   pScaled->Right =
   ((( pPixels->Left + pPixels->Width - 1 ) << 16 ) + hPixels - 1 ) / hPixels;
   pScaled->Bottom =
   ((( pPixels->Top + pPixels->Height - 1 ) << 16 ) + vLines - 1 ) / vLines;
}

/******************************************************************************/

#define E_INVCMDLINE 1
#define E_API        2
#define E_X4         3
#define E_GETIPCUR   4
#define E_VERIPCUR   5
#define E_INVIPPREF  6
#define E_SETIPPREF  7
#define E_GETIPPREF  8
#define E_VERIPPREF  9
#define E_INVINEQU   10
#define E_SETINEQU   11
#define E_GETINEQU   12
#define E_VERINEQU   13
#define E_INVOUTDEF  14
#define E_SETOUTDEF  15
#define E_GETOUTDEF  16
#define E_VEROUTDEF  17
#define E_INVOPSRC   18
#define E_SETOPSRC   19
#define E_GETOPSRC   20
#define E_VEROPSRC   21
#define E_INVREGION  22
#define E_SETCROP    23
#define E_GETCROP    24
#define E_VERCROP    25
#define E_SETTRAN    26
#define E_GETTRAN    27
#define E_VERTRAN    28
#define E_INVOPCUR   29
#define E_GETOPCUR   30
#define E_VEROPCUR   31
#define E_INVGENL    32
#define E_GETGENL    33
#define E_VERGENL    34
#define E_DEVICE     35
#define E_CONFIGFILE 36
#define E_MANYOPTS	37
#define E_INVTSTPAT  38
#define E_SETTSTPAT  39
#define E_GETTSTPAT  40
#define E_VERTSTPAT  41
/******************************************************************************/

unsigned long
HTotal (
        PVDIF v1 )
{
   return v1->HorAddrTime + v1->HorRightBorder + v1->HorFrontPorch +
   v1->HorSyncTime + v1->HorBackPorch  + v1->HorLeftBorder;
}

unsigned long
VTotal (
        PVDIF v1 )
{
   return v1->VerAddrTime + v1->VerBottomBorder + v1->VerFrontPorch +
   v1->VerSyncTime + v1->VerBackPorch + v1->VerTopBorder;
}

BOOL
CompareTimings(
               PVDIF v1,
               PVDIF v2,
               unsigned long timing1,
               unsigned long timing2)
{
   return ((v1->PixelClock == v2->PixelClock ) &&
           (timing1 == timing2 ) &&
           (v1->HorAddrTime == v2->HorAddrTime ) &&
           (v1->HorRightBorder == v2->HorRightBorder ) &&
           (v1->HorFrontPorch == v2->HorFrontPorch ) &&
           (v1->HorSyncTime == v2->HorSyncTime ) &&
           (v1->HorBackPorch == v2->HorBackPorch ) &&
           (v1->HorLeftBorder == v2->HorLeftBorder ) &&
           (v1->VerAddrTime == v2->VerAddrTime ) &&
           (v1->VerBottomBorder == v2->VerBottomBorder ) &&
           (v1->VerFrontPorch == v2->VerFrontPorch ) &&
           (v1->VerSyncTime == v2->VerSyncTime ) &&
           (v1->VerBackPorch == v2->VerBackPorch ) &&
           (v1->VerTopBorder == v2->VerTopBorder ));
}

/******************************************************************************/

unsigned long
CheckInCur(
           HX4DEVICE hDevice,
           PINCUR pInCur)
{
   VDIF vdif, vdif2;
   unsigned long timing, timing2;
   
   Error = 0;
   
   memset(&vdif, 0, sizeof(vdif));
   vdif.HorAddrTime = pInCur->Width;
   vdif.VerAddrTime = pInCur->Height;
   vdif.VerFrequency = pInCur->Refresh;
   timing = TimingTable[pInCur->Timing];
   X4EasyCalculateVideoTimings(&vdif, timing);
   
   memset(&vdif2, 0, sizeof(vdif2));
   
   if ((Error = X4EasyInputGetCurrentTimings(hDevice, &vdif2, &timing2)) > 0)
   {
      if (!Quiet)
      {
         _tprintf(TEXT("X4InputGetCurrentTimings: 0x%08lx.\n"), Error);
      }
      Error = E_GETIPCUR;
   }
   else
   {
      unsigned long tol = (vdif.VerFrequency * pInCur->Tolerance ) / 100;
      
      if (( HTotal(&vdif) != HTotal(&vdif2) ) ||
          ( VTotal(&vdif) != VTotal(&vdif2) ) ||
          ( vdif2.VerFrequency < vdif2.VerFrequency - tol ) ||
          ( vdif2.VerFrequency > vdif2.VerFrequency + tol ))
      {
         if (!Quiet)
         {
            _tprintf(TEXT("X4InputGetCurrentTimings: Verify Failed.\n"));
         }
         Error = E_VERIPCUR;
      }
   }
   return Error;
}

/******************************************************************************/

unsigned long
CheckOutCur(
            HX4DEVICE hDevice,
            unsigned long i,
            POUTCUR pOutCur )
{
   VDIF vdif, vdif2;
   unsigned long timing, timing2;
   
   Error = 0;
   
   memset(&vdif, 0, sizeof(vdif));
   vdif.HorAddrTime = pOutCur->Width;
   vdif.VerAddrTime = pOutCur->Height;
   vdif.VerFrequency = pOutCur->Refresh;
   timing = TimingTable[pOutCur->Timing];
   X4EasyCalculateVideoTimings(&vdif, timing);
   
   memset(&vdif2, 0, sizeof(vdif2));
   
   if ((Error = X4EasyOutputGetCurrentTimings(hDevice, i, &vdif2, &timing2)) > 0)
   {
      if (!Quiet)
      {
         _tprintf(TEXT("X4EasyOutputGetCurrentTimings[%ld]: 0x%08lx.\n"), i, Error);
      }
      Error = E_GETOPCUR;
   }
   else
   {
      unsigned long tol = (vdif.VerFrequency * pOutCur->Tolerance ) / 100;
      
      if (( HTotal(&vdif) != HTotal(&vdif2) ) ||
          ( VTotal(&vdif) != VTotal(&vdif2) ) ||
          ( vdif2.VerFrequency < vdif2.VerFrequency - tol ) ||
          ( vdif2.VerFrequency > vdif2.VerFrequency + tol ))
      {
         if (!Quiet)
         {
            _tprintf(TEXT("X4OutputGetCurrentTimings[%ld]: Verify Failed.\n"), i);
         }
         Error = E_VEROPCUR;
      }
   }
   return Error;
}

/******************************************************************************/

unsigned long
CheckGenlock(
             HX4DEVICE hDevice,
             unsigned long gen )
{
   X4GENLOCK gen2;
   
   Error = 0;
   
   if ((Error = X4EasyDeviceGetGenlockStatus(hDevice, &gen2)) > 0)
   {
      if (Error)
      {
         _tprintf(TEXT("X4EasyDeviceGetGenlockStatus: 0x%08lx.\n"), Error);
      }
      Error = E_GETGENL;
   }
   else
   {
      if (gen != (unsigned long)gen2)
      {
         if (!Quiet)
         {
            _tprintf(TEXT("X4EasyDeviceGetGenlockStatus: Verify Failed.\n"));
         }
         Error = E_VERIPPREF;
      }
   }
   return Error;
}

/******************************************************************************/

unsigned long
SetInPref(
          HX4DEVICE hDevice,
          PINPREF pInPref )
{
   VDIF vdif;
   unsigned long timing;
   
   Error = 0;
   
   memset(&vdif, 0, sizeof(vdif));
   vdif.HorAddrTime = pInPref->Width;
   vdif.VerAddrTime = pInPref->Height;
   vdif.VerFrequency = pInPref->Refresh;
   timing = TimingTable[pInPref->Timing];
   X4EasyCalculateVideoTimings(&vdif, timing);
   if ((Error = X4EasyInputSetPreferredTimings(hDevice, &vdif, timing)) > 0)
   {
      if (!Quiet)
      {
         _tprintf(TEXT("X4EasyInputSetPreferredTimings: 0x%08lx.\n"), Error);
      }
      Error = E_SETIPPREF;
   }
   else
   {
      VDIF vdif2;
      unsigned long timing2;
      
      if ((Error = X4EasyInputGetPreferredTimings(hDevice, &vdif2, &timing2)) > 0)
      {
         if (!Quiet)
         {
            _tprintf(TEXT("X4EasyInputGetPreferredTimings: 0x%08lx.\n"), Error);
         }
         Error = E_GETIPPREF;
      }
      else
      {
         if (CompareTimings(&vdif, &vdif2, timing, timing2) == FALSE)
         {
            if (!Quiet)
            {
               _tprintf(TEXT("X4EasyInputGetPreferredTimings: Verify Failed.\n"));
            }
            Error = E_VERIPPREF;
         }
      }
   }
   return Error;
}

/******************************************************************************/

unsigned long
SetInEqu(
         HX4DEVICE hDevice,
         unsigned long equ )
{
   Error = 0;
   
   if ((Error = X4EasyInputSetEqualisation(hDevice, (X4EQUALISATION)equ)) > 0)
   {
      if (Error)
      {
         _tprintf(TEXT("X4EasyInputSetEqualisation: 0x%08lx.\n"), Error);
      }
      Error = E_SETINEQU;
   }
   else
   {
      X4EQUALISATION equ2;
      
      if ((Error = X4EasyInputGetEqualisation(hDevice, &equ2)) > 0)
      {
         if (Error)
         {
            _tprintf(TEXT("X4EasyInputGetEqualisation: 0x%08lx.\n"), Error);
         }
         Error = E_GETINEQU;
      }
      else
      {
         if (equ != (unsigned long)equ2)
         {
            if (!Quiet)
            {
               _tprintf(TEXT("X4EasyInputGetEqualisation: Verify Failed.\n"));
            }
            Error = E_VERIPPREF;
         }
      }
   }
   return Error;
}

/******************************************************************************/

unsigned long
SetOutDef(
          HX4DEVICE hDevice,
          unsigned long output,
          POUTDEF pOutDef )
{
   VDIF vdif;
   unsigned long timing;
   
   Error = 0;
   
   memset(&vdif, 0, sizeof(vdif));
   vdif.HorAddrTime = pOutDef->Width;
   vdif.VerAddrTime = pOutDef->Height;
   vdif.VerFrequency = pOutDef->Refresh;
   timing = TimingTable[pOutDef->Timing];
   vdif.Flags |= FormatTable[pOutDef->Format];
   X4EasyCalculateVideoTimings(&vdif, timing);
   
   if ((Error = X4EasyOutputSetDefaultTimings(hDevice, output, &vdif, timing)) > 0)
   {
      if (!Quiet)
      {
         _tprintf(TEXT("X4EasyOutputSetDefaultTimings: OutDef%ld : 0x%08lx.\n"),
                  output+1, Error);
      }
      Error = E_SETOUTDEF;
   }
   else
   {
      VDIF vdif2;
      unsigned long timing2;
      
      if ((Error = X4EasyOutputGetDefaultTimings(hDevice, output, &vdif2,
                                                 &timing2)) > 0)
      {
         if (!Quiet)
         {
            _tprintf(TEXT("X4EasyOutputGetDefaultTimings: OutDef%ld : 0x%08lx.\n"),
                     output+1, Error);
         }
         Error = E_GETOUTDEF;
      }
      else
      {
         if (CompareTimings(&vdif, &vdif2, timing, timing2) == FALSE )
         {
            if (!Quiet)
            {
               _tprintf(TEXT("X4EasyInputGetPreferredTimings: : Verify Failed.\n"));
            }
            Error = E_VEROUTDEF;
         }
      }
   }
   return Error;
}

/******************************************************************************/

unsigned long
SetTestPattern(
               HX4DEVICE hDevice,
               unsigned long output,
               X4IMAGESOURCE TestPat )
{
   X4IMAGESOURCE Verify;
   Error = 0;
   
   if ((Error = X4EasySetImageSource(hDevice, output, TestPat)) > 0)
   {
      if (!Quiet)
      {
         _tprintf(TEXT("X4EasySetImageSource: TestPattern%ld : 0x%08lx.\n"),
                  output+1, Error);
      }
      Error = E_SETTSTPAT;
   }
   else
   {
      
      if ((Error = X4EasyGetImageSource(hDevice, output, &Verify)) > 0)
      {
         if (!Quiet)
         {
            _tprintf(TEXT("X4OutputGetImageSource: TestPattern%ld : 0x%08lx.\n"),
                     output+1, Error);
         }
         Error = E_GETTSTPAT;
      }
      else
      {
         if (TestPat != Verify)
         {
            if (!Quiet)
            {
               _tprintf(TEXT("X4OutputSetImageSource: : Verify Failed.\n"));
            }
            Error = E_VERTSTPAT;
         }
      }
   }
   return Error;
}

/******************************************************************************/

unsigned long
SetOutSrc(
          HX4DEVICE hDevice,
          unsigned long output,
          unsigned long source )
{
   Error = 0;
   
   if ((Error = X4EasyOutputSetTimingSource(hDevice, output,
                                            (X4TIMINGSOURCE)SourceTable[source], output)) > 0)
   {
      if (!Quiet)
      {
         _tprintf(TEXT("X4EasyOutputSetTimingSource: OutSrc%ld : 0x%08lx.\n"),
                  output+1, Error);
      }
      Error = E_SETOPSRC;
   }
   else
   {
      X4TIMINGSOURCE source2;
      unsigned long refOutput2;
      
      if ((Error = X4EasyOutputGetTimingSource(hDevice, output, &source2, &refOutput2)) > 0)
      {
         if (!Quiet)
         {
            _tprintf(TEXT("X4EasyOutputGetTimingSource: OutSrc%ld : 0x%08lx.\n"),
                     output+1, Error);
         }
         Error = E_GETOPSRC;
      }
      else
      {
         if (SourceTable[source] != source2)
         {
            if (!Quiet)
            {
               _tprintf(TEXT("X4EasyOutputSetTimingSource: : Verify Failed.\n"));
            }
            Error = E_VEROPSRC;
         }
      }
      
   }
   return Error;
}

/******************************************************************************/

unsigned long
SetRegion(
          HX4DEVICE hDevice,
          unsigned long output,
          ScaledCropping *pCropScaled,
          X4TRANSFORM transform )
{
   if ((Error = X4EasyOutputSetCropping(hDevice, output, pCropScaled->Top,
                                        pCropScaled->Left, pCropScaled->Right, pCropScaled->Bottom )) > 0)
   {
      if (!Quiet)
      {
         _tprintf(TEXT("X4OutputSetCropping: Region1 : 0x%08lx.\n"), Error);
      }
      Error = E_SETCROP;
   }
   else
   {
      ScaledCropping cropScaled2;
      
      if ((Error = X4EasyOutputGetCropping(hDevice, output, &cropScaled2.Top,
                                           &cropScaled2.Left, &cropScaled2.Right, &cropScaled2.Bottom )) > 0)
      {
         if (!Quiet)
         {
            _tprintf(TEXT("X4OutputGetCropping: Region1 : 0x%08lx.\n"), Error);
         }
         Error = E_GETCROP;
      }
      else
      {
         if (memcmp(pCropScaled, &cropScaled2, sizeof(ScaledCropping)) != 0)
         {
            if (!Quiet)
            {
               _tprintf(TEXT("X4InputGetPreferredTimings: : Verify Failed.\n"));
            }
            Error = E_VERCROP;
         }
      }
   }
   
   if ( Error ) return Error;
   
   if ((Error = X4EasyOutputSetTransformation(hDevice, output, transform)) > 0)
   {
      if (!Quiet)
      {
         _tprintf(TEXT("X4OutputSetTransformation: Region1 : 0x%08lx.\n"), Error);
      }
      Error = E_SETTRAN;
   }
   else
   {
      X4TRANSFORM transform2;
      
      if ((Error = X4EasyOutputGetTransformation(hDevice, output, &transform2)) > 0)
      {
         if (!Quiet)
         {
            _tprintf(TEXT("X4OutputGetTransformation: Region1 : 0x%08lx.\n"),
                     Error);
         }
         Error = E_GETTRAN;
      }
      else
      {
         if (transform != transform2)
         {
            if (!Quiet)
            {
               _tprintf(TEXT("X4InputGetPreferredTimings: : Verify Failed.\n"));
            }
            Error = E_VERTRAN;
         }
      }
   }
   return Error;
}

/******************************************************************************/

#if ( defined WIN32 ) && (( defined _UNICODE ) || ( defined UNICODE ))
int wmain (int argc, wchar_t *argv[])
#else
int main(int argc, char *argv[])
#endif
{
#if ( defined WIN32 )
   HINSTANCE      hInstance = NULL;
#endif
   HX4DLL         hDLL = 0;
   HX4DEVICE      hDevice = 0;
   COMMANDLINE    commandLine;
   
   VDIF           currInput;
   unsigned long  currTiming;
   int load_file_flag = 0;
   
   memset(&commandLine, 0, sizeof(COMMANDLINE));
#if ( defined WIN32 )
   if ((Error = ProcessCommandLine(&commandLine)) == FALSE)
#else
      if ((Error = ProcessCommandLine(argc, argv, &commandLine)) == FALSE)
#endif
      {
         if (!Quiet)
         {
            _tprintf(TEXT("Invalid Command Line: 0x%08lx.\n"), Error);
         }
         Error = E_INVCMDLINE;
         goto cleanup;
      }
   if ((commandLine.SpecifiedFlags == 0) && (Commit == 0))
   {
      _tprintf(
               TEXT("VQSCMD.EXE\n")
               TEXT("Options:\n")
               TEXT("  -InputEdid=<timing>,<width>,<height>,<refresh>\n")
               TEXT("     Sets the input preferred mode\n")
               TEXT("\n")
               TEXT("  -Equalisation=<0,1,2,3,4,5,6,7>\n")
               TEXT("     Sets the input equalisation level\n")
               TEXT("\n")
               TEXT("  -OutputMode[1,2,3,4]=<timing>,<width>,<height>,<refresh>,<format>\n")
               TEXT("     Sets the default mode of output 1, 2, 3 or 4\n")
               TEXT("\n")
               TEXT("  -OutputMode=<timing>,<width>,<height>,<refresh>,<format>\n")
               TEXT("     Sets the default mode of all four outputs\n")
               TEXT("\n")
               TEXT("  -OutputSource[1,2,3,4]=<source>\n")
               TEXT("     Sets mode source of output 1, 2, 3 or 4\n")
               TEXT("\n")
               TEXT("  -OutputSource=<source>\n")
               TEXT("     Sets mode source of all outputs\n")
               TEXT("\n")
               TEXT("  -TestPattern[1,2,3,4]=<source>\n")
               TEXT("     Sets pattern generation for output 1, 2, 3 or 4\n")
               TEXT("     <source> is one of:\n")
               TEXT("         0 - No test pattern (display crop region)\n")
               TEXT("         1 - Static greyscale bars\n")
               TEXT("         2 - Moving greyscale bars\n")
               TEXT("         3 - Static colour bars\n")
               TEXT("         4 - Moving colour bars\n")
               TEXT("\n")
               TEXT("  -TestPattern=<source>\n")
               TEXT("     Sets test pattern generation for all outputs\n")
               TEXT("     See -TestPattern[0,1,2,3] for explanation of <source>\n")
               TEXT("\n")
               TEXT("  -CaptureRegion[1,2,3,4]=<top>,<left>,<width>,<height>,<transform>\n")
               TEXT("     Sets the input capture region of output 1, 2, 3 or 4\n")
               TEXT("\n")
               TEXT("  -CaptureRegion=<top>,<left>,<width>,<height>,<transform>\n")
               TEXT("     Sets the input capture region of all four outputs\n")
               TEXT("\n")
               TEXT("  -CaptureRegionDefault=<0,1>\n")
               TEXT("     Sets a predfined set of input capture regions to all four outputs\n")
               TEXT("\n")
               TEXT("  -Quiet\n")
               TEXT("     Supresses error messages\n")
               TEXT("\n")
               TEXT("  -Commit\n")
               TEXT("     Commit the parameters in effect into the non-volatile memory\n")
               TEXT("\n")
               TEXT("  -CheckInputMode=<timing>,<width>,<height>,<refresh>,<tolerance>\n")
               TEXT("     Checks the current input against the supplied video mode\n")
               TEXT("\n")
               TEXT("  -CheckOutputMode[1,2,3,4]=<timing>,<width>,<height>,<refresh>,<format>,<tolerance>\n")
               TEXT("     Checks the current output mode of output 1, 2, 3 or 4 against the supplied video mode\n")
               TEXT("\n")
               TEXT("  -CheckOutputMode=<timing>,<width>,<height>,<refresh>,<format>,<tolerance>\n")
               TEXT("     Checks all four output modes against the supplied video mode\n")
               TEXT("\n")
               TEXT("  -CheckGenlock=<genlock>\n")
               TEXT("     Checks the genlock status of the device.\n")
               TEXT("\n")
               TEXT("   <timing> - video timing formula where\n")
               TEXT("      0 - CVT\n")
               TEXT("      1 - CVR-RB\n")
               TEXT("      2 - GTF\n")
               TEXT("      3 - SMPTE *** Valid SMPTE modes only e.g. 1920,1080,59940\n")
               TEXT("\n")
               TEXT("   <width> - number of active pixels on a line\n")
               TEXT("\n")
               TEXT("   <height> - number of active lines\n")
               TEXT("\n")
               TEXT("   <refresh> - vertical refresh in milliherts eg 59940\n")
               TEXT("\n")
               TEXT("   <tolerance> - percentage +/- tolerance in vertical refresh\n")
               TEXT("\n")
               TEXT("   <format> - output video format where\n")
               TEXT("      0 - Analog\n")
               TEXT("      1 - DVI\n")
               TEXT("\n")
               TEXT("   <source> - source of output video mode where\n")
               TEXT("      0 - Monitor Preferred mode\n")
               TEXT("      1 - Default mode\n")
               TEXT("\n")
               TEXT("   <transform> - capture region transformation where\n")
               TEXT("      0 - No Rotation\n")
               TEXT("      1 - Rotate 90 degrees\n")
               TEXT("      2 - Rotate 180 degrees\n")
               TEXT("      3 - Rotate 270 degrees\n")
               TEXT("      4 - Mirror Horizontaly\n")
               TEXT("      5 - Flip Vertically\n")
               TEXT("\n")
               TEXT("   <CaptureRegionDefault> - predfined set of input capture regions where\n")
               TEXT("      0 - Quartered\n")
               TEXT("      1 - Replicated\n")
               TEXT("\n")
               TEXT("   <genlock> - genlock status where\n")
               TEXT("      0 - Not genlocked\n")
               TEXT("      1 - Output genlocked\n")
               TEXT("      2 - Input genlocked\n")
               TEXT("\n")
               TEXT("  -ConfigFile=<config file>\n")
               TEXT("     Specifies the configuration file to load and apply settings from\n")
               TEXT("\n")
               TEXT("  -DeviceNumber=<0,1,2,...>\n")
               TEXT("     Specifies the device number to open.  Note that this is in USB\n")
               TEXT("     enumeration order, not the order in which the devices were connected\n")
               TEXT("\n")
               TEXT("  -DeviceName=<name>\n")
               TEXT("     Specifies the device to open by name.  This is the same name as\n")
               TEXT("     is set using the control application's \"Friendly Name\" setting.\n")
               TEXT("\n"));
      
      Error = 0;
      goto cleanup;
   }
   
	if ( (commandLine.SpecifiedFlags & COMMAND_FLAG_DEVICE) == 0 &&
       (commandLine.SpecifiedFlags & COMMAND_FLAG_FRIENDLYNAME) == 0 )
	{
		if (!Quiet)
      {
         _tprintf(TEXT("Device not specified by either number or friendly name.\n"));
      }
		Error = E_DEVICE;
		goto cleanup;
	}
   
	if (commandLine.SpecifiedFlags & COMMAND_FLAG_CONFIGFILE)
	{
		if ((commandLine.SpecifiedFlags & COMMAND_FLAG_INPREF) ||
          (commandLine.SpecifiedFlags & COMMAND_FLAG_INEQU) ||
          ( commandLine.SpecifiedFlags &
           ( COMMAND_FLAG_OUTDEF1 | COMMAND_FLAG_OUTDEF2 |
				COMMAND_FLAG_OUTDEF3 | COMMAND_FLAG_OUTDEF4 )) ||
          (commandLine.SpecifiedFlags & COMMAND_FLAG_OUTDEFALL) ||
          ( commandLine.SpecifiedFlags &
           ( COMMAND_FLAG_OUTSRC1 | COMMAND_FLAG_OUTSRC2 |
            COMMAND_FLAG_OUTSRC3 | COMMAND_FLAG_OUTSRC4 )) ||
          ( commandLine.SpecifiedFlags &
           ( COMMAND_FLAG_REGION1 | COMMAND_FLAG_REGION2 |
				COMMAND_FLAG_REGION3 | COMMAND_FLAG_REGION4 )) ||
          ( commandLine.SpecifiedFlags & COMMAND_FLAG_OUTCURALL ) ||
          ( commandLine.SpecifiedFlags &
           ( COMMAND_FLAG_OUTCUR1 | COMMAND_FLAG_OUTCUR2 |
				COMMAND_FLAG_OUTCUR2 | COMMAND_FLAG_OUTCUR4 )) ||
          ( commandLine.SpecifiedFlags & COMMAND_FLAG_GENLOCK ))
		{
			if (!Quiet)
			{
				_tprintf(TEXT("Only define DEVICE and VQS options when using a configuration file. Commit remains optional.\n"));
			}
			Error = E_MANYOPTS;
			goto cleanup;
		} else {
         
			load_file_flag=1;
         
		}
	}
   
   if (commandLine.SpecifiedFlags & COMMAND_FLAG_INPREF)
   {
      if (( commandLine.InPref.Width == 0 ) ||
          ( commandLine.InPref.Height == 0 ) ||
          ( commandLine.InPref.Refresh == 0 ) ||
          ( commandLine.InPref.Timing > NumberOfTimings -1 ))
      {
         if (!Quiet)
         {
            _tprintf(TEXT("Invalid InputEdid Arguments.\n"));
         }
         Error = E_INVIPPREF;
         goto cleanup;
      }
   }
   if (commandLine.SpecifiedFlags & COMMAND_FLAG_INEQU)
   {
      if (( commandLine.InEqu < X4EQUALISATION_LEVEL0 ) ||
          ( commandLine.InEqu > X4EQUALISATION_LEVEL7 ))
      {
         if (!Quiet)
         {
            _tprintf(TEXT("Invalid Equalisation Arguments.\n"));
         }
         Error = E_INVINEQU;
         goto cleanup;
      }
   }
   
   /* Output Default Timings. */
   {
      if ( commandLine.SpecifiedFlags &
          ( COMMAND_FLAG_OUTDEF1 | COMMAND_FLAG_OUTDEF2 |
           COMMAND_FLAG_OUTDEF3 | COMMAND_FLAG_OUTDEF4 ))
      {
         int i;
         for ( i = 0; i< 4; i++ )
         {
            if ( commandLine.SpecifiedFlags & ( COMMAND_FLAG_OUTDEF1 << i ))
            {
               POUTDEF pOutDef = (&commandLine.OutDef1) + i;
               
               if (( pOutDef->Width == 0 ) ||
                   ( pOutDef->Height == 0 ) ||
                   ( pOutDef->Refresh == 0 ) ||
                   ( pOutDef->Timing > NumberOfTimings -1 ) ||
                   ( pOutDef->Format > NumberOfFormats -1 ))
               {
                  if (!Quiet)
                  {
                     _tprintf(TEXT("Invalid OutputMode%d Arguments.\n"), i+1);
                  }
                  Error = E_INVOUTDEF;
                  goto cleanup;
               }
            }
         }
         if (commandLine.SpecifiedFlags & COMMAND_FLAG_OUTDEFALL)
         {
            _tprintf(TEXT("Cannot specify OutputMode[1,2,3,4] with OutputMode.\n"));
            Error = E_INVOUTDEF;
            goto cleanup;
         }
      }
      if (commandLine.SpecifiedFlags & COMMAND_FLAG_OUTDEFALL)
      {
         POUTDEF pOutDef = &commandLine.OutDefAll;
         
         if (( pOutDef->Width == 0 ) ||
             ( pOutDef->Height == 0 ) ||
             ( pOutDef->Refresh == 0 ) ||
             ( pOutDef->Timing > NumberOfTimings -1 ) ||
             ( pOutDef->Format > NumberOfFormats -1 ))
         {
            if (!Quiet)
            {
               _tprintf(TEXT("Invalid OutputMode Arguments.\n"));
            }
            Error = E_INVOUTDEF;
            goto cleanup;
         }
         if ( commandLine.SpecifiedFlags &
             ( COMMAND_FLAG_OUTDEF1 | COMMAND_FLAG_OUTDEF2 |
              COMMAND_FLAG_OUTDEF3 | COMMAND_FLAG_OUTDEF4 ))
         {
            {
               _tprintf(TEXT("Cannot specify OutputMode[1,2,3,4] with OutputMode.\n"));
            }
            Error = E_INVOUTDEF;
            goto cleanup;
         }
      }
   }
   
   /* Output Test Pattern generation. */
   {
      if ( commandLine.SpecifiedFlags &
          ( COMMAND_FLAG_TSTPAT1 | COMMAND_FLAG_TSTPAT2 |
           COMMAND_FLAG_TSTPAT3 | COMMAND_FLAG_TSTPAT4 ))
      {
         int i;
         for ( i = 0; i< 4; i++ )
         {
            if ( commandLine.SpecifiedFlags & ( COMMAND_FLAG_TSTPAT1 << i ))
            {
               PX4IMAGESOURCE pTstPat = (&commandLine.TstPat1) + i;
               
               if (( *pTstPat < X4IMGSRC_CROPPING ) ||
                   ( *pTstPat > X4IMGSRC_TSTPAT_COLOURMOVE ))
               {
                  if (!Quiet)
                  {
                     _tprintf(TEXT("Invalid TestPattern%d Argument.\n"), i+1);
                  }
                  Error = E_INVTSTPAT;
                  goto cleanup;
               }
            }
         }
         if (commandLine.SpecifiedFlags & COMMAND_FLAG_TSTPATALL)
         {
            _tprintf(TEXT("Cannot specify TestPattern[1,2,3,4] with TestPattern.\n"));
            Error = E_INVTSTPAT;
            goto cleanup;
         }
      }
      if (commandLine.SpecifiedFlags & COMMAND_FLAG_TSTPATALL)
      {
         PX4IMAGESOURCE pTstPat = &commandLine.TstPatAll;
         
         if (( *pTstPat < X4IMGSRC_CROPPING ) ||
             ( *pTstPat > X4IMGSRC_TSTPAT_COLOURMOVE ))
         {
            if (!Quiet)
            {
               _tprintf(TEXT("Invalid TestPattern Argument.\n"));
            }
            Error = E_INVTSTPAT;
            goto cleanup;
         }
         if ( commandLine.SpecifiedFlags &
             ( COMMAND_FLAG_TSTPAT1 | COMMAND_FLAG_TSTPAT2 |
              COMMAND_FLAG_TSTPAT3 | COMMAND_FLAG_TSTPAT4 ))
         {
            {
               _tprintf(TEXT("Cannot specify TestPattern[1,2,3,4] with TestPattern.\n"));
            }
            Error = E_INVTSTPAT;
            goto cleanup;
         }
      }
   }
   
   /* Output Timings source */
   {
      if ( commandLine.SpecifiedFlags &
          ( COMMAND_FLAG_OUTSRC1 | COMMAND_FLAG_OUTSRC2 |
           COMMAND_FLAG_OUTSRC3 | COMMAND_FLAG_OUTSRC4 ))
      {
         int i;
         for ( i = 0; i< 4; i++ )
         {
            if ( commandLine.SpecifiedFlags & ( COMMAND_FLAG_OUTSRC1 << i ))
            {
               int source = *((&commandLine.OutSrc1) + i);
               if (( source < MONITOR ) ||
                   ( source > DEFAULT ))
               {
                  if (!Quiet)
                  {
                     _tprintf(TEXT("Invalid OutputSource%d Arguments.\n"), i);
                  }
                  Error = E_INVOPSRC;
                  goto cleanup;
               }
            }
         }
         if ( commandLine.SpecifiedFlags & COMMAND_FLAG_OUTSRCALL )
         {
            if (!Quiet)
            {
               _tprintf(TEXT("Cannot specify OutputSource[1,2,3,4] with OutputSource.\n"));
            }
            Error = E_INVOPSRC;
            goto cleanup;
         }
      }
      if ( commandLine.SpecifiedFlags & COMMAND_FLAG_OUTSRCALL )
      {
         int source = commandLine.OutSrcAll;
         if (( source < MONITOR ) ||
             ( source > DEFAULT ))
         {
            if (!Quiet)
            {
               _tprintf(TEXT("Invalid OutputSource Arguments.\n"));
            }
            Error = E_INVOPSRC;
            goto cleanup;
         }
         
         if ( commandLine.SpecifiedFlags &
             ( COMMAND_FLAG_OUTSRC1 | COMMAND_FLAG_OUTSRC2 |
              COMMAND_FLAG_OUTSRC3 | COMMAND_FLAG_OUTSRC4 ))
         {
            {
               _tprintf(TEXT("Cannot specify OutputSource[1,2,3,4] with OutputSource.\n"));
            }
            Error = E_INVOPSRC;
            goto cleanup;
         }
      }
   }
   
   /* Input Region*/
   {
      if ( commandLine.SpecifiedFlags &
          ( COMMAND_FLAG_REGION1 | COMMAND_FLAG_REGION2 |
           COMMAND_FLAG_REGION3 | COMMAND_FLAG_REGION4 ))
      {
         int i;
         for ( i = 0; i< 4; i++ )
         {
            if ( commandLine.SpecifiedFlags & ( COMMAND_FLAG_REGION1 << i ))
            {
               PREGION pRegion = (&commandLine.Region1) + i;
               if (( pRegion->Top < 0 ) ||
                   ( pRegion->Left < 0 ) ||
                   ( pRegion->Width <= 0 ) ||
                   ( pRegion->Height <= 0 ) ||
                   ( pRegion->Transform < ROTATION_NONE ) ||
                   ( pRegion->Transform > FLIP_VERT ))
               {
                  if (!Quiet)
                  {
                     _tprintf(TEXT("Invalid CaptureRegion%d Arguments.\n"), i);
                  }
                  Error = E_INVREGION;
                  goto cleanup;
               }
            }
         }
         if ( commandLine.SpecifiedFlags &
             ( COMMAND_FLAG_REGIONALL | COMMAND_FLAG_REGIONDEF ))
         {
            if (!Quiet)
            {
               _tprintf(TEXT("Cannot specify CaptureRegion[1,2,3,4] with CaptureRegionDefault or CaptureRegion.\n"));
            }
            Error = E_INVREGION;
            goto cleanup;
         }
      }
      if (commandLine.SpecifiedFlags & COMMAND_FLAG_REGIONALL)
      {
         PREGION pRegion = &commandLine.RegionAll;
         if (( pRegion->Top < 0 ) || ( pRegion->Left < 0 ) ||
             ( pRegion->Width <= 0 ) || ( pRegion->Height <= 0 ) ||
             ( pRegion->Transform < ROTATION_NONE ) ||
             ( pRegion->Transform> FLIP_VERT ))
         {
            if (!Quiet)
            {
               _tprintf(TEXT("Invalid CaptureRegion Arguments.\n"));
            }
            Error = E_INVREGION;
            goto cleanup;
         }
         if ( commandLine.SpecifiedFlags &
             ( COMMAND_FLAG_REGION1 | COMMAND_FLAG_REGION2 |
              COMMAND_FLAG_REGION3 | COMMAND_FLAG_REGION4 |
              COMMAND_FLAG_REGIONDEF ))
         {
            if (!Quiet)
            {
               _tprintf(TEXT("Cannot specify CaptureRegion with CaptureRegion[1,2,3,4] or CaptureRegionDefault.\n"));
            }
            Error = E_INVREGION;
            goto cleanup;
         }
      }
      if (commandLine.SpecifiedFlags & COMMAND_FLAG_REGIONDEF)
      {
         if ( ( commandLine.RegionDef < 0 ) || ( commandLine.RegionDef > 1 ))
         {
            if (!Quiet)
            {
               _tprintf(TEXT("Invalid CaptureRegionDefault Arguments.\n"));
            }
            Error = E_INVREGION;
            goto cleanup;
         }
         if ( commandLine.SpecifiedFlags &
             ( COMMAND_FLAG_REGION1 | COMMAND_FLAG_REGION2 |
              COMMAND_FLAG_REGION3 | COMMAND_FLAG_REGION4 |
              COMMAND_FLAG_REGIONALL ))
         {
            if (!Quiet)
            {
               _tprintf(TEXT("Cannot specify CaptureRegionDefault with CaptureRegion[1,2,3,4] or CaptureRegion.\n"));
            }
            Error = E_INVREGION;
            goto cleanup;
         }
      }
   }
   
   if ( commandLine.SpecifiedFlags & COMMAND_FLAG_OUTCURALL )
   {
      if ( commandLine.SpecifiedFlags &
          ( COMMAND_FLAG_OUTCUR1 | COMMAND_FLAG_OUTCUR2 |
           COMMAND_FLAG_OUTCUR2 | COMMAND_FLAG_OUTCUR4 ))
      {
         if (!Quiet)
         {
            _tprintf(TEXT("Cannot specify CheckOutputMode with CheckOutputMode[1,2,3,4].\n"));
         }
         Error = E_INVOPCUR;
         goto cleanup;
      }
   }
   
   if ( commandLine.SpecifiedFlags &
       ( COMMAND_FLAG_OUTCUR1 | COMMAND_FLAG_OUTCUR2 |
        COMMAND_FLAG_OUTCUR2 | COMMAND_FLAG_OUTCUR4 ))
   {
      if ( commandLine.SpecifiedFlags & COMMAND_FLAG_OUTCURALL )
      {
         if (!Quiet)
         {
            _tprintf(TEXT("Cannot specify CheckOutputMode[1,2,3,4] with CheckOutputMode.\n"));
         }
         Error = E_INVOPCUR;
         goto cleanup;
      }
   }
   
   if ( commandLine.SpecifiedFlags & COMMAND_FLAG_GENLOCK )
   {
      if (( commandLine.Genlock < X4GENLOCK_NONE ) ||
          ( commandLine.Genlock > X4GENLOCK_UNKNOWN ))
      {
         if (!Quiet)
         {
            _tprintf(TEXT("Invalid CheckGenlock Arguments.\n"));
         }
         Error = E_INVOPCUR;
         goto cleanup;
      }
   }
   
#if (defined WIN32)
   if ((Error = APILoadLibrary( TEXT("X4.DLL"), &hInstance)) > 0)
   {
      if (!Quiet)
      {
         _tprintf(TEXT("APILoadLibrary: 0x%08x.\n"), Error);
      }
      Error = E_API;
      goto cleanup;
   }
   if (!APILoadFunctions(hInstance, fnList, NULL))
   {
      if (!Quiet)
      {
         _tprintf(TEXT("Incompatible X4.DLL\n"));
      }
      Error = E_API;
      goto cleanup;
   }
#endif
   if ((Error = X4EasyLoad(&hDLL)) > 0)
   {
      if (!Quiet)
      {
         _tprintf(TEXT("X4Load: 0x%08lx.\n"), Error);
      }
      Error = E_X4;
      goto cleanup;
   }
   
   if (commandLine.SpecifiedFlags & COMMAND_FLAG_FRIENDLYNAME)
   {
      TCHAR szThisDevice[255];
      unsigned long  i = 0, iNumX4Devices;
      HX4DEVICE hTest = (HX4DEVICE)-1;
      X4EasyDeviceGetCount(hDLL, &iNumX4Devices);
	   
      do {
         if (hTest != (HX4DEVICE)-1)
         {
            X4EasyDeviceClose(hTest);
         }
         if ((Error = X4EasyDeviceOpen(hDLL, i, &hTest)) != X4ERROR_NO_ERROR)
         {
            if (!Quiet)
            {
               _tprintf(TEXT("X4DeviceOpen: 0x%08lx.\n"), Error);
            }
            Error = E_X4;
            goto cleanup;
         }
         
         if ((Error = X4EasyDeviceGetFriendlyName(hTest, szThisDevice)))
         {
            if (!Quiet)
            {
               _tprintf(TEXT("X4DeviceGetFriendlyName: 0x%08lx.\n"), Error);
            }
            Error = E_X4;
            goto cleanup;
         }
         i++;
      } while (_tcscmp(szThisDevice, commandLine.FriendlyName));
      if (i <= iNumX4Devices)
      {
         // Found a device with the correct friendly name, keep the handle open
         // and store in the handle that the rest of the program uses.
         hDevice = hTest;
      } else {
         X4EasyDeviceClose(hTest);
      }
   }
   else
   {
      if ((Error = X4EasyDeviceOpen(hDLL,commandLine.Device,&hDevice)) > 0)
      {
         if (!Quiet)
         {
            _tprintf(TEXT("X4DeviceOpen: 0x%08lx.\n"), Error);
         }
         Error = E_X4;
         goto cleanup;
      }
   }

   if (load_file_flag == 1)
   {
      
	   if (ConfigurationLoad(hDevice, commandLine.ConfigFile)==FALSE)
	   {
		   _tprintf(TEXT("Error loading configuration file!\n"));
		   goto cleanup;
	   }
      
   } else {
      
	   if ((Error = X4EasyInputGetCurrentTimings(hDevice, &currInput, &currTiming)) > 0)
	   {
         if (!Quiet)
         {
            _tprintf(TEXT("X4InputGetCurrentTimings: 0x%08lx.\n"), Error);
         }
         Error = E_GETIPCUR;
         goto cleanup;
	   }
	   else
	   {
         if (currInput.Flags &
             (VDIF_FLAG_NOSIGNAL | VDIF_FLAG_OUTOFRANGE | VDIF_FLAG_UNRECOGNISABLE))
         {
            if ( commandLine.SpecifiedFlags & COMMAND_FLAG_INPREF )
            {
               memset(&currInput, 0, sizeof(currInput));
               currInput.HorAddrTime = commandLine.InPref.Width;
               currInput.VerAddrTime = commandLine.InPref.Height;
               currInput.VerFrequency = commandLine.InPref.Refresh;
               currTiming = TimingTable[commandLine.InPref.Timing];
               X4EasyCalculateVideoTimings(&currInput, currTiming);
            }
            else
            {
               if ((Error = X4EasyInputGetPreferredTimings(hDevice, &currInput,
                                                           &currTiming)) > 0)
               {
                  if (!Quiet)
                  {
                     _tprintf(TEXT("X4InputGetPreferredTimings: 0x%08lx.\n"), Error);
                  }
                  Error = E_GETIPPREF;
                  goto cleanup;
               }
            }
         }
	   }
      
	   /* Input Region*/
	   {
         if ( commandLine.SpecifiedFlags &
             ( COMMAND_FLAG_REGION1 | COMMAND_FLAG_REGION2 |
              COMMAND_FLAG_REGION3 | COMMAND_FLAG_REGION4 ))
         {
            int i;
            for ( i = 0; i< 4; i++ )
            {
               if ( commandLine.SpecifiedFlags & ( COMMAND_FLAG_REGION1 << i ))
               {
                  PREGION pRegion = (&commandLine.Region1) + i;
                  if (( pRegion->Left + pRegion->Width ) >
                      (int)currInput.HorAddrTime )
                  {
                     if (!Quiet)
                     {
                        _tprintf(TEXT("Region%d cropping width too large\n"), i+1);
                     }
                     Error = 1;
                     goto cleanup;
                  }
                  if (( pRegion->Top + pRegion->Height ) >
                      (int)currInput.VerAddrTime )
                  {
                     if (!Quiet)
                     {
                        _tprintf(TEXT("Region%d cropping height too large\n"), i+1);
                     }
                     Error = 1;
                     goto cleanup;
                  }
               }
            }
         }
         if (commandLine.SpecifiedFlags & COMMAND_FLAG_REGIONALL)
         {
            PREGION pRegion = &commandLine.RegionAll;
            if (( pRegion->Left + pRegion->Width ) > (int)currInput.HorAddrTime )
            {
               if (!Quiet)
               {
                  _tprintf(TEXT("RegionAll cropping width too large\n"));
               }
               Error = 1;
               goto cleanup;
            }
            if (( pRegion->Top + pRegion->Height ) > (int)currInput.VerAddrTime )
            {
               if (!Quiet)
               {
                  _tprintf(TEXT("RegionAll cropping height too large\n"));
               }
               Error = 1;
               goto cleanup;
            }
         }
	   }
      
      /*****************************************************************************/
      
	   if ( commandLine.SpecifiedFlags & COMMAND_FLAG_INPREF )
	   {
         Error = SetInPref(hDevice, &commandLine.InPref);
         if (Error)
         {
            goto cleanup;
         }
	   }
      
	   if ( commandLine.SpecifiedFlags & COMMAND_FLAG_INEQU )
	   {
         Error = SetInEqu(hDevice, commandLine.InEqu);
         if (Error)
         {
            goto cleanup;
         }
	   }
      
	   /* Output Default Timings. */
	   {
         if ( commandLine.SpecifiedFlags &
             ( COMMAND_FLAG_OUTDEF1 | COMMAND_FLAG_OUTDEF2 |
              COMMAND_FLAG_OUTDEF3 | COMMAND_FLAG_OUTDEF4 ))
         {
            int i;
            for ( i = 0; i< 4; i++ )
            {
               if ( commandLine.SpecifiedFlags & ( COMMAND_FLAG_OUTDEF1 << i ))
               {
                  POUTDEF pOutDef = (&commandLine.OutDef1) + i;
                  Error = SetOutDef(hDevice, i, pOutDef);
                  if (Error)
                  {
                     goto cleanup;
                  }
                  
               }
            }
         }
         if ( commandLine.SpecifiedFlags & COMMAND_FLAG_OUTDEFALL )
         {
            int i;
            for (i=0;i<4;i++)
            {
               POUTDEF pOutDef = &commandLine.OutDefAll;
               Error = SetOutDef(hDevice, i, pOutDef);
               if (Error)
               {
                  goto cleanup;
               }
            }
         }
	   }
      
	   /* Output Pattern generation. */
	   {
         if ( commandLine.SpecifiedFlags &
             ( COMMAND_FLAG_TSTPAT1 | COMMAND_FLAG_TSTPAT2 |
              COMMAND_FLAG_TSTPAT3 | COMMAND_FLAG_TSTPAT4 ))
         {
            int i;
            for ( i = 0; i< 4; i++ )
            {
               if ( commandLine.SpecifiedFlags & ( COMMAND_FLAG_TSTPAT1 << i ))
               {
                  PX4IMAGESOURCE pTstPat= (&commandLine.TstPat1) + i;
                  Error = SetTestPattern(hDevice, i, *pTstPat);
                  if (Error)
                  {
                     goto cleanup;
                  }
                  
               }
            }
         }
         if ( commandLine.SpecifiedFlags & COMMAND_FLAG_TSTPATALL )
         {
            int i;
            for (i=0;i<4;i++)
            {
               PX4IMAGESOURCE pTstPat = &commandLine.TstPatAll;
               Error = SetTestPattern(hDevice, i, *pTstPat);
               if (Error)
               {
                  goto cleanup;
               }
            }
         }
	   }
      
	   /* Output Timings source */
	   {
         if ( commandLine.SpecifiedFlags &
             ( COMMAND_FLAG_OUTSRC1 | COMMAND_FLAG_OUTSRC2 |
              COMMAND_FLAG_OUTSRC3 | COMMAND_FLAG_OUTSRC4 ))
         {
            int i;
            for ( i = 0; i< 4; i++ )
            {
               if ( commandLine.SpecifiedFlags & ( COMMAND_FLAG_OUTSRC1 << i ))
               {
                  int source = *((&commandLine.OutSrc1) + i);
                  Error = SetOutSrc(hDevice, i, SourceTable[source]);
                  if (Error)
                  {
                     goto cleanup;
                  }
               }
            }
         }
         if ( commandLine.SpecifiedFlags & COMMAND_FLAG_OUTSRCALL )
         {
            int i;
            for ( i = 0; i< 4; i++ )
            {
               int source = commandLine.OutSrcAll;
               Error = SetOutSrc(hDevice, i, SourceTable[source]);
               if (Error)
               {
                  goto cleanup;
               }
            }
         }
	   }
      
	   /* Input Region*/
	   {
         if ( commandLine.SpecifiedFlags &
             ( COMMAND_FLAG_REGION1 | COMMAND_FLAG_REGION2 |
              COMMAND_FLAG_REGION3 | COMMAND_FLAG_REGION4 ))
         {
            int i;
            for ( i = 0; i< 4; i++ )
            {
               if ( commandLine.SpecifiedFlags & ( COMMAND_FLAG_REGION1 << i ))
               {
                  PREGION pRegion = (&commandLine.Region1) + i;
                  
                  InputCropping crop;
                  ScaledCropping cropScaled;
                  
                  crop.Top = pRegion->Top;
                  crop.Left = pRegion->Left;
                  crop.Width = pRegion->Width;
                  crop.Height = pRegion->Height;
                  
                  InputUnitsToScaled(currInput.HorAddrTime, currInput.VerAddrTime,
                                     &crop, &cropScaled);
                  
                  Error = SetRegion(hDevice, i, &cropScaled, pRegion->Transform );
                  if (Error)
                  {
                     goto cleanup;
                  }
               }
            }
         }
         if (commandLine.SpecifiedFlags & COMMAND_FLAG_REGIONALL)
         {
            int i;
            for ( i = 0; i< 4; i++ )
            {
               PREGION pRegion = &commandLine.RegionAll;
               
               InputCropping crop;
               ScaledCropping cropScaled;
               
               crop.Top = pRegion->Top;
               crop.Left = pRegion->Left;
               crop.Width = pRegion->Width;
               crop.Height = pRegion->Height;
               
               InputUnitsToScaled(currInput.HorAddrTime, currInput.VerAddrTime,
                                  &crop, &cropScaled);
               
               Error = SetRegion(hDevice, i, &cropScaled, pRegion->Transform );
               if (Error)
               {
                  goto cleanup;
               }
            }
         }
         if (commandLine.SpecifiedFlags & COMMAND_FLAG_REGIONDEF)
         {
            int i;
            ScaledCropping Quarter[4] =
            {
               {0x0000,0x0000,0x7FFF,0x7FFF},
               {0x0000,0x8000,0xFFFF,0x7FFF},
               {0x8000,0x0000,0x7FFF,0xFFFF},
               {0x8000,0x8000,0xFFFF,0xFFFF},
            };
            ScaledCropping Replicate[4] =
            {
               {0x0000,0x0000,0xFFFF,0xFFFF},
               {0x0000,0x0000,0xFFFF,0xFFFF},
               {0x0000,0x0000,0xFFFF,0xFFFF},
               {0x0000,0x0000,0xFFFF,0xFFFF},
            };
            ScaledCropping *pCropping;
            
            if (commandLine.RegionDef == 0)
            {
               pCropping = Quarter;
            }
            else
            {
               pCropping = Replicate;
            }
            
            for ( i = 0; i< 4; i++ )
            {
               Error = SetRegion(hDevice, i, pCropping, ROTATION_NONE );
               if (Error)
               {
                  goto cleanup;
               }
               pCropping++;
            }
         }
	   }
   }
   
   
   if (Commit == 1)
   {
      // Probably should really be beyond the scope of a test program, but we may want to test
      // that the non-volatile parameter saves work at some point in the future.
      X4EasyDeviceUpdateFlash(hDevice);
   }
   
   if ( commandLine.SpecifiedFlags & COMMAND_FLAG_INCUR )
   {
      Error = CheckInCur(hDevice, &commandLine.InCur);
      if (Error)
      {
         goto cleanup;
      }
   }
   
   {
      if ( commandLine.SpecifiedFlags & 
          ( COMMAND_FLAG_OUTCUR1 | COMMAND_FLAG_OUTCUR2 |
           COMMAND_FLAG_OUTCUR3 | COMMAND_FLAG_OUTCUR4 ))
      {
         int i;
         for ( i = 0; i< 4; i++ )
         {
            if ( commandLine.SpecifiedFlags & ( COMMAND_FLAG_OUTCUR1 << i ))
            {
               POUTCUR pOutCur = (&commandLine.OutCur1) + i;
               
               Error = CheckOutCur(hDevice, i, pOutCur);
               if (Error)
               {
                  goto cleanup;
               }
            }
         }
      }
      if ( commandLine.SpecifiedFlags & COMMAND_FLAG_OUTCURALL )
      {
         int i;
         POUTCUR pOutCur = &commandLine.OutCurAll;
         
         for ( i = 0; i< 4; i++ )
         {
            Error = CheckOutCur(hDevice, i, pOutCur);
            if (Error)
            {
               goto cleanup;
            }
         }
      }
   }
   
   if ( commandLine.SpecifiedFlags & COMMAND_FLAG_GENLOCK )
   {
      Error = CheckGenlock(hDevice, commandLine.Genlock);
      if (Error)
      {
         goto cleanup;
      }
   }
   
   
cleanup:
   if (hDevice)
   {
      X4EasyDeviceClose(hDevice);
   }
   
   if (hDLL)
   {
      X4EasyFree(hDLL);
   }
#if ( defined WIN32 )
   if (hInstance)
   {
      APIFreeLibrary(hInstance);
   }
#endif
   return (int)Error;
}

/******************************************************************************/


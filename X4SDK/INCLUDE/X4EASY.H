/*******************************************************************************

Copyright Datapath Ltd. 2009, 2012.

Purpose: Function declarations for a simple X4 control interface.

History:
         24 OCT 12    DMJ   Created.

*******************************************************************************/

#define X4API __stdcall

#ifndef API
   #ifdef __cplusplus
      #define API(type,modifier,name,args) \
      extern "C" type modifier name args;
   #else
      #define API(type,modifier,name,args) \
         type modifier name args;
   #endif
#endif

API ( X4ERROR, X4API,
X4EasyLoad, (
   PHX4DLL pHX4DLL ) )


API ( X4ERROR, X4API,
X4EasyFree, (
   HX4DLL hX4DLL ) )


API ( X4ERROR, X4API,
X4EasyDeviceGetCount, (
   HX4DLL         hX4DLL,
   unsigned long  *pCount ) )


API ( X4ERROR, X4API,
X4EasyDeviceGetSerialNumberA, (
   HX4DLL         hX4DLL,
   unsigned long  index,
   char           *pSerialNumber ) )


API ( X4ERROR, X4API,
X4EasyDeviceGetSerialNumberW, (
   HX4DLL         hX4DLL,
   unsigned long  index,
   wchar_t        *pSerialNumber ) )

#if ( defined ( _UNICODE ) || defined ( UNICODE ))
#define X4EasyDeviceGetSerialNumber X4EasyDeviceGetSerialNumberW
#else
#define X4EasyDeviceGetSerialNumber X4EasyDeviceGetSerialNumberA
#endif

API ( X4ERROR, X4API,
X4EasyDeviceGetFriendlyNameA, (
   HX4DEVICE   hDevice,
   char        *pName ) )


API ( X4ERROR, X4API,
X4EasyDeviceGetFriendlyNameW, (
   HX4DEVICE   hDevice,
   wchar_t     *pName ) )

#if ( defined ( _UNICODE ) || defined ( UNICODE ))
#define X4EasyDeviceGetFriendlyName X4EasyDeviceGetFriendlyNameW
#else
#define X4EasyDeviceGetFriendlyName X4EasyDeviceGetFriendlyNameA
#endif

API ( X4ERROR, X4API,
X4EasyDeviceSetFriendlyNameA, (
   HX4DEVICE   hDevice,
   char        *pName ) )


API ( X4ERROR, X4API,
X4EasyDeviceSetFriendlyNameW, (
   HX4DEVICE   hDevice,
   wchar_t     *pName ) )

#if ( defined ( _UNICODE ) || defined ( UNICODE ))
#define X4EasyDeviceSetFriendlyName X4EasyDeviceSetFriendlyNameW
#else
#define X4EasyDeviceSetFriendlyName X4EasyDeviceSetFriendlyNameA
#endif

API ( X4ERROR, X4API,
X4EasyDeviceGetCapabilities, (
   HX4DEVICE      hDevice,
   unsigned long  *pCapabilities ) )


API ( X4ERROR, X4API,
X4EasyDeviceSetNewSlicingMethod, (
   HX4DEVICE hDevice ) )


API ( X4ERROR, X4API,
X4EasyDeviceOpen, (
   HX4DLL hX4DLL,
   unsigned long  device,
   PHX4DEVICE     phDevice ) )


API ( X4ERROR, X4API,
X4EasyDeviceClose, (
   HX4DEVICE hDevice ) )


API ( X4ERROR, X4API,
X4EasyDeviceGetSystemInfo, (
   HX4DEVICE      hDevice,
   PX4SYSTEMINFO  pSystemInfo ) )


API ( X4ERROR, X4API,
X4EasyDeviceGetSystemHealth, (
   HX4DEVICE         hDevice,
   PX4SYSTEMHEALTH   pSystemHealth ) )


API ( X4ERROR, X4API,
X4EasyDeviceUpdateFlash, (
   HX4DEVICE hDevice ) )


API ( X4ERROR, X4API,
X4EasyDeviceGetGenlockStatus, (
   HX4DEVICE   hDevice,
   PX4GENLOCK  pGenlock ) )


API ( X4ERROR, X4API,
X4EasyDeviceSetSmoothness, (
   HX4DEVICE      hDevice,
   X4SMOOTHNESS   smoothness ) )


API ( X4ERROR, X4API,
X4EasyDeviceGetSmoothness, (
   HX4DEVICE      hDevice,
   PX4SMOOTHNESS  pSmoothness ) )


API ( X4ERROR, X4API,
X4EasyInputGetPreferredTimingsA, (
   HX4DEVICE      hDevice,
   PVDIFA         pVDIF,
   unsigned long  *pTiming ) )


API ( X4ERROR, X4API,
X4EasyInputGetPreferredTimingsW, (
   HX4DEVICE      hDevice,
   PVDIFW         pVDIF,
   unsigned long  *pTiming ) )

#if ( defined ( _UNICODE ) || defined ( UNICODE ))
#define X4EasyInputGetPreferredTimings X4EasyInputGetPreferredTimingsW
#else
#define X4EasyInputGetPreferredTimings X4EasyInputGetPreferredTimingsA
#endif

API ( X4ERROR, X4API,
X4EasyInputSetPreferredTimingsA, (
   HX4DEVICE      hDevice,
   PVDIFA         pVDIF,
   unsigned long  timing ) )


API ( X4ERROR, X4API,
X4EasyInputSetPreferredTimingsW, (
   HX4DEVICE      hDevice,
   PVDIFW         pVDIF,
   unsigned long  timing ) )

#if ( defined ( _UNICODE ) || defined ( UNICODE ))
#define X4EasyInputSetPreferredTimings X4EasyInputSetPreferredTimingsW
#else
#define X4EasyInputSetPreferredTimings X4EasyInputSetPreferredTimingsA
#endif

API ( X4ERROR, X4API,
X4EasyInputIsEqualisationSupported, (
   HX4DEVICE   hDevice,
   signed long *pbSupported ) )


API ( X4ERROR, X4API,
X4EasyInputSetDVILinkState, (
   HX4DEVICE         hDevice,
   X4DVILINKOVERRIDE override ) )


API ( X4ERROR, X4API,
X4EasyInputGetEqualisation, (
   HX4DEVICE       hDevice,
   PX4EQUALISATION pEqualisation ) )


API ( X4ERROR, X4API,
X4EasyInputSetEqualisation, (
   HX4DEVICE      hDevice,
   X4EQUALISATION equalisation ) )


API ( X4ERROR, X4API,
X4EasyInputGetEdid, (
   HX4DEVICE   hDevice,
   char        *pEdid ) )


API ( X4ERROR, X4API,
X4EasyInputSetEdid, (
   HX4DEVICE   hDevice,
   char        *pEdid ) )


API ( X4ERROR, X4API,
X4EasyInputGetCurrentTimingsA, (
   HX4DEVICE      hDevice,
   PVDIFA         pVDIF,
   unsigned long  *pTiming ) )


API ( X4ERROR, X4API,
X4EasyInputGetCurrentTimingsW, (
   HX4DEVICE      hDevice,
   PVDIFW         pVDIF,
   unsigned long  *pTiming ) )

#if ( defined ( _UNICODE ) || defined ( UNICODE ))
#define X4EasyInputGetCurrentTimings X4EasyInputGetCurrentTimingsW
#else
#define X4EasyInputGetCurrentTimings X4EasyInputGetCurrentTimingsA
#endif

API ( X4ERROR, X4API,
X4EasyInputGetDualLinkStatus, (
   HX4DEVICE         hDevice,
   int               *bDualLink,
   X4DVILINKOVERRIDE *pReason ) )


API ( X4ERROR, X4API,
X4EasyOutputGetCount, (
   HX4DEVICE      hDevice,
   unsigned long  *pCount ) )


API ( X4ERROR, X4API,
X4EasyOutputGetPreferredTimingsA, (
   HX4DEVICE      hDevice,
   unsigned long  output,
   PVDIFA         pVDIF,
   unsigned long  *pTiming ) )


API ( X4ERROR, X4API,
X4EasyOutputGetPreferredTimingsW, (
   HX4DEVICE hDevice,
   unsigned long  output,
   PVDIFW         pVDIF,
   unsigned long  *pTiming ) )

#if ( defined ( _UNICODE ) || defined ( UNICODE ))
#define X4EasyOutputGetPreferredTimings X4EasyOutputGetPreferredTimingsW
#else
#define X4EasyOutputGetPreferredTimings X4EasyOutputGetPreferredTimingsA
#endif

API ( X4ERROR, X4API,
X4EasyOutputGetCurrentTimingsA, (
   HX4DEVICE hDevice,
   unsigned long  output,
   PVDIFA         pVDIF,
   unsigned long  *pTiming ) )


API ( X4ERROR, X4API,
X4EasyOutputGetCurrentTimingsW, (
   HX4DEVICE hDevice,
   unsigned long  output,
   PVDIFW         pVDIF,
   unsigned long  *pTiming ) )

#if ( defined ( _UNICODE ) || defined ( UNICODE ))
#define X4EasyOutputGetCurrentTimings X4EasyOutputGetCurrentTimingsW
#else
#define X4EasyOutputGetCurrentTimings X4EasyOutputGetCurrentTimingsA
#endif

API ( X4ERROR, X4API,
X4EasyOutputGetDefaultTimingsA, (
   HX4DEVICE hDevice,
   unsigned long  output,
   PVDIFA         pVDIF,
   unsigned long  *pTiming ) )


API ( X4ERROR, X4API,
X4EasyOutputGetDefaultTimingsW, (
   HX4DEVICE      hDevice,
   unsigned long  output,
   PVDIFW         pVDIF,
   unsigned long  *pTiming ) )

#if ( defined ( _UNICODE ) || defined ( UNICODE ))
#define X4EasyOutputGetDefaultTimings X4EasyOutputGetDefaultTimingsW
#else
#define X4EasyOutputGetDefaultTimings X4EasyOutputGetDefaultTimingsA
#endif

API ( X4ERROR, X4API,
X4EasyOutputSetDefaultTimingsA, (
   HX4DEVICE      hDevice,
   unsigned long  output,
   PVDIFA         pVDIF,
   unsigned long  timing ) )


API ( X4ERROR, X4API,
X4EasyOutputSetDefaultTimingsW, (
   HX4DEVICE      hDevice,
   unsigned long  output,
   PVDIFW         pVDIF,
   unsigned long  timing ) )

#if ( defined ( _UNICODE ) || defined ( UNICODE ))
#define X4EasyOutputSetDefaultTimings X4EasyOutputSetDefaultTimingsW
#else
#define X4EasyOutputSetDefaultTimings X4EasyOutputSetDefaultTimingsA
#endif

API ( X4ERROR, X4API,
X4EasyOutputSetTimingSource, (
   HX4DEVICE      hDevice,
   unsigned long  output,
   X4TIMINGSOURCE source,
   unsigned long  refOutput ) )


API ( X4ERROR, X4API,
X4EasyOutputGetTimingSource, (
   HX4DEVICE         hDevice,
   unsigned long     output,
   PX4TIMINGSOURCE   pSource,
   unsigned long     *refOutput ) )


API ( X4ERROR, X4API,
X4EasyOutputSetCropping, (
   HX4DEVICE hDevice,
   unsigned long output,
   unsigned long top,
   unsigned long left,
   unsigned long right,
   unsigned long bottom ) )


API ( X4ERROR, X4API,
X4EasyOutputGetCropping, (
   HX4DEVICE hDevice,
   unsigned long output,
   unsigned long *pTop,
   unsigned long *pLeft,
   unsigned long *pRight,
   unsigned long *pBottom ) )


API ( X4ERROR, X4API,
X4EasyOutputSetTransformation, (
   HX4DEVICE hDevice,
   unsigned long  output,
   X4TRANSFORM    transform ) )


API ( X4ERROR, X4API,
X4EasyOutputGetTransformation, (
   HX4DEVICE      hDevice,
   unsigned long  output,
   PX4TRANSFORM   pTransform ) )


API ( X4ERROR, X4API,
X4EasyOutputEnable, (
   HX4DEVICE      hDevice,
   unsigned long  output,
   unsigned long  bEnable ) )


API ( X4ERROR, X4API,
X4EasyOutputIsEnabled, (
   HX4DEVICE      hDevice,
   unsigned long  output,
   unsigned long  *pbEnable ) )


API ( X4ERROR, X4API,
X4EasyCalculateVideoTimingsA, (
   PVDIFA pVDIF,
   unsigned long timing ) )


API ( X4ERROR, X4API,
X4EasyCalculateVideoTimingsW, (
   PVDIFW pVDIF,
   unsigned long timing ) )

#if ( defined ( _UNICODE ) || defined ( UNICODE ))
#define X4EasyCalculateVideoTimings X4EasyCalculateVideoTimingsW
#else
#define X4EasyCalculateVideoTimings X4EasyCalculateVideoTimingsA
#endif

API ( X4ERROR, X4API,
X4EasyOutputSetSlicing, (
   HX4DEVICE hDevice,
   PX4OUTPUT pOutput ) )


API ( X4ERROR, X4API,
X4EasyOutputGetSlicing, (
   HX4DEVICE hDevice,
   PX4OUTPUT pOutput ) )


API ( X4ERROR, X4API,
X4EasyCoordinateConvertMIToInput, (
   HX4DEVICE hDevice,
   PX4OUTPUT pMI,
   PX4OUTPUT pInput ) )


API ( X4ERROR, X4API,
X4EasyCoordinateConvertInputToMI, (
   HX4DEVICE hDevice,
   PX4OUTPUT pInput,
   PX4OUTPUT pMI ) )


API ( X4ERROR, X4API,
X4EasySetImageSource, (
   HX4DEVICE      hDevice,
   unsigned long  output,
   X4IMAGESOURCE  src ) )


API ( X4ERROR, X4API,
X4EasyGetImageSource, (
   HX4DEVICE      hDevice,
   unsigned long  output,
   X4IMAGESOURCE  *src ) )


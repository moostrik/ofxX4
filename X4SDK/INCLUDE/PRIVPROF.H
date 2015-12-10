/*******************************************************************************

Copyright Datapath Ltd. 2012.

File:    GetPrivateProfileInt.h

Purpose: Definitions for a GetPrivateProfileInt implementation function.


History:
         21 MAR 12    OM   Created.

*******************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif
   
int
GetPrivateProfileInt(
   TCHAR *lpAppName,
   TCHAR *lpKeyName,
   int nDefault,
   TCHAR *lpFileName );
/*
 * Summary: Retrieves an integer associated with a key in the specified section
 * of configuration file.
 *
 * Purpose:
 *          lpAppName
 *             The name of the section in the initialization file.
 *
 *          lpKeyName
 *             The name of the key whose value is to be retrieved.
 *
 *          nDefault
 *             The default value to return if the key name cannot be found in 
 *             the initialization file.
 *
 *          lpFileName
 *             The name of the configuration file.
 *
 * Return:  Returns the integer value of the specified key. If key is not found
 *          returns the default value.
 */

#ifdef __cplusplus
}
#endif
/*******************************************************************************

Copyright Datapath Ltd. 1997, 2006.

File:    CmdLine.c

Purpose: Implementation of functions for processing command line arguments.

History:
         12 DEC 97   KML   Created.
         20 JAN 98   KML   Document all functions.
                           Return value included in all functions.
                           Changed the ExtractTLWH routine to leave
                           any default values as is.
         02 APR 98   KML   Modify extract functions to support more than two
                           Possible return values.
                           Update documentation.
         25 JUN 98   KML   Modify the command line functions to provide an
                           interface for validation/error handling.
                           This interface uses the error codes to call an
                           application specific function to process and where
                           possible, amend the parameter at fault.
         16 FEB 99    JH   Reimplemented OldExtractTLWH.
         10 SEP 03    JH   Reimplemented ParseCommandLine.
         08 OCT 03    RL   Reimplemented ExtractTLWH.
         11 FEB 04    RL   Added ExtractBool.
         25 MAY 04    RL   Added ExtractLong.
         27 MAY 04    JH   Temporarily replace the character after a value with
                           '\0' while the extract function is called.
         07 JUN 04    JH   Tidied up!
         18 JUN 04    JH   ExtractFloat, ExtractUnsignedInt, ExtractLong and
                           ExtractBool now return an error if no value is
                           specified.
         22 JUN 04    JH   Now pass correct pVoid to error function when called
                           due to error in extract function.
         04 MAY 05    RL   Added ExtractColour.
         13 MAY 05    RL   Restricted ExtractColour to specify three values.
         29 JUN 05    RL   Added range check to ExtractFloat.
         04 AUG 05    RL   Added ExtractUnsignedShort.
         22 MAY 06    JH   Ported to Unicode.
         23 JUN 11    DJ   Conditional compile around _stscanf to use _stscanf_s
                           on compilers that have deprecated _stscanf.

*******************************************************************************/

#if ( defined WIN32 )
#include <windows.h>
#include <tchar.h>
#elif ( defined OSX )
#include <ctype.h>
#include <errno.h>
#include <wchar.h>
#include <datatypes.h>
#else
//TODO
#endif

#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#include "cmdline.h"

/******************************************************************************/

unsigned long
ExtractFloat (
   LPCTSTR  pValue,
   int      nValueChars,
   PVOID    pData,
   PVOID    pVoid )
/*
 * See cmdline.h for a description of this function.
 */
{
   float *value = (float *)pVoid;
   float localValue;
   int   i;
   int   bNoPreviousDot = TRUE;

   if ( nValueChars == 0 )
   {
      return CMDLN_VALUE_NOT_SPECIFIED;
   }

   for ( i = 0; i < nValueChars; i++ )
   {
      TCHAR ch = pValue[i];

      /* Ascii character test */
      if ( ch == '.' )
      {
         bNoPreviousDot = !bNoPreviousDot;
      }

      if ( !(( ch >= '0' ) && ( ch <='9' )) )
      {
         if (( ch != '.' ) || ( bNoPreviousDot ))
         {
            return ( CMDLN_VALUE_INVALID_TYPE );
         }
      }
   }

#ifdef WIN32
#if _MSC_VER < 1500
   // For compiler versions < 1500 (i.e. less than VC2008), we want to use the old call.
   _stscanf( pValue, TEXT("%f"), &localValue );
#else
   // But for 2008 and above, fix the compiler warning for new projects.
   _stscanf_s ( pValue, TEXT("%f"), nValueChars, &localValue );
#endif
#else
   _stscanf(pValue, TEXT("%f"), &localValue);
#endif

   /* User specific range test */
   if ( pData )
   {
      PFLLIMIT pflLimit = (PFLLIMIT)pData;

      if ( !(( pflLimit->flLower <= localValue ) && 
             ( pflLimit->flUpper >= localValue )) )
      {
         return CMDLN_VALUE_OUTOF_RANGE;
      }
   }

   *value = localValue;

   return 0;
}

/******************************************************************************/

unsigned long
ExtractString (
   LPCTSTR  pValue,
   int      nValueChars,
   PVOID    pData,
   PVOID    pVoid )
/*
 * See cmdline.h for a description of this function.
 */
{
   LPTSTR   lpstr = (LPTSTR)pVoid;
   int      maxChars = (int)(ULONG_PTR)pData;

   int      nCount = 0;
   int      iStart = 0;
   int      iEnd = nValueChars;

   /* Strip double-quotes at either end */
   if ( pValue[0] == '"' )
   {
      iStart++;
   }
   if ( pValue[nValueChars - 1] == '"' )
   {
      iEnd--;
   }

   /* Update buffer with string */
   while ( iStart < iEnd )
   {
      /* Get rid of all special characters */
      if (( pValue[iStart] != '\\' ) || ( pValue[iStart + 1] != '"' ))
      {
         lpstr[nCount] = pValue[iStart];
         nCount++;
      }
      if ( nCount >= maxChars )
      {
         return ( CMDLN_VALUE_EXCEEDS_LENGTH );
      }
      iStart++;
   }
   lpstr[nCount] = '\0';
   return 0;
}

/******************************************************************************/

unsigned long
ExtractUnsignedInt (
   LPCTSTR  pValue,
   int      nValueChars,
   PVOID    pData,
   PVOID    pVoid )
/*
 * See cmdline.h for a description of this function.
 */
{
   unsigned int   total, digit;
   int            i;

   if ( nValueChars == 0 )
   {
      return CMDLN_VALUE_NOT_SPECIFIED;
   }

   total = 0;
   for ( i = 0; i < nValueChars; i++ )
   {
      /* ASCII character test */
      if ( !_istdigit ( pValue[i] ) )
      {
         return CMDLN_VALUE_INVALID_TYPE;
      }

      digit = (unsigned int)pValue[i] - TEXT('0');

      /* The maximum unsigned int (in 32-bit) is 4294967295. In order to
       * avoid overflow, we check the total before it is multiplied by
       * 10 and we check the least significant digit before it is added in. */
      if ( i > 8  )
      {
         if ( total == 429496729 )
         {
            if ( digit > 5 )
            {
               return CMDLN_VALUE_OUTOF_RANGE;
            }
         }
         else if ( total > 429496729 )
         {
            return CMDLN_VALUE_OUTOF_RANGE;
         }
      }

      total *= 10;
      total += digit;
   }

   /* User specific range test */
   if ( pData )
   {
      PLIMIT pLimit = (PLIMIT)pData;

      if ( !(( pLimit->ulLower <= total ) && ( pLimit->ulUpper >= total )) )
      {
         return CMDLN_VALUE_OUTOF_RANGE;
      }
   }

   *(unsigned int *)pVoid = total;
   return 0;
}

/******************************************************************************/

unsigned long
ExtractUnsignedShort (
   LPCTSTR  pValue,
   int      nValueChars,
   PVOID    pData,
   PVOID    pVoid )
{
   unsigned long  uLong, error;

   error = ExtractUnsignedInt ( pValue, nValueChars, 0, &uLong );
   if ( error == 0 )
   {
      if ( uLong <= USHRT_MAX )
      {
         /* User specific range. */
         if ( pData )
         {
            PLIMIT pLimit = (PLIMIT)pData;

            if ( !(( pLimit->ulLower <= uLong ) && ( pLimit->ulUpper >= uLong )))
            {
               return CMDLN_VALUE_OUTOF_RANGE;
            }
         }
         *(unsigned short *)pVoid = (unsigned short)uLong;
         return 0;
      }
      return CMDLN_VALUE_OUTOF_RANGE;
   }
   return error;
}

/******************************************************************************/

unsigned long
ExtractLong (
   LPCTSTR  pValue,
   int      nValueChars,
   PVOID    pData,
   PVOID    pVoid )
/*
 * See cmdline.h for a description of this function.
 */
{
   BOOL  bIsNegative;
   int   i;
   long  total, digit;

   if ( nValueChars == 0 )
   {
      return CMDLN_VALUE_NOT_SPECIFIED;
   }

   if ( pValue[0] == TEXT('-') )
   {
      bIsNegative = TRUE;
      i = 1;
   }
   else
   {
      bIsNegative = FALSE;
      i = 0;
   }

   total = 0;
   for ( i; i < nValueChars; i++ )
   {
      /* ASCII character test */
      if ( !_istdigit ( pValue[i] ) )
      {
         return CMDLN_VALUE_INVALID_TYPE;
      }

      digit = (long)pValue[i] - TEXT('0');

      /* Signed / unsigned long type size test */
      if ( bIsNegative )
      {
         if ( i > 9  )
         {
            /* The maximum our total can be at this stage is 214748364 and the
             * last digit less than nine. */

            /* A minimum signed long is -2147483648. In order to avoid overflow,
             * we check the total before it is multiplied by 10 and we check the
             * least significant digit before it is added in. */
            if ( total == 214748364 )
            {
               if ( digit > 8 )
               {
                  return CMDLN_VALUE_OUTOF_RANGE;
               }
            }
            else if ( total > 214748364 )
            {
               return CMDLN_VALUE_OUTOF_RANGE;
            }
         }
      }
      else
      {
         if ( i > 8  )
         {
            /* The maximum our total can be at this stage is 214748364 and the
             * last digit less than eight. */

            /* A maximum signed long is 2147483647. In order to avoid overflow,
             * we check the total before it is multiplied by 10 and we check the
             * least significant digit before it is added in. */
            if ( total == 214748364 )
            {
               if ( digit > 7 )
               {
                  return CMDLN_VALUE_OUTOF_RANGE;
               }
            }
            else if ( total > 214748364 )
            {
               return CMDLN_VALUE_OUTOF_RANGE;
            }
         }
      }

      total *= 10;
      total += digit;
   }

   if ( bIsNegative )
   {
      if ( total != 0 )
      {
         total = -total;
      }
      else
      {
         return CMDLN_VALUE_INVALID_TYPE;
      }
   }

   /* User specific range test */
   if ( pData )
   {
      PLIMIT pLimit = (PLIMIT)pData;

      if ( !(( (long)pLimit->ulLower <= total ) &&
             ( (long)pLimit->ulUpper >= total )) )
      {
         return CMDLN_VALUE_OUTOF_RANGE;
      }
   }

   *(long *)pVoid = total;
   return 0;
}

/******************************************************************************/

unsigned long
ExtractBool (
   LPCTSTR  pValue,
   int      nValueChars,
   PVOID    pData,
   PVOID    pVoid )
/*
 * See cmdline.h for a description of this function.
 */
{
   BOOL  *pBool;

   pBool = (BOOL*)pVoid;

   switch ( nValueChars )
   {
      case 0:
         return CMDLN_VALUE_NOT_SPECIFIED;
      case 1:
         if ( _tcsncicmp ( pValue, TEXT("0"), 1 ) == 0 )
         {
            *pBool = 0;
            return 0;
         }
         else if ( _tcsncicmp ( pValue, TEXT("1"), 1 ) == 0 )
         {
            *pBool = 1;
            return 0;
         }
         break;
      case 2:
         if ( _tcsncicmp ( pValue, TEXT("No"), 2 ) == 0 )
         {
            *pBool = 0;
            return 0;
         }
         else if ( _tcsncicmp ( pValue, TEXT("On"), 2 ) == 0 )
         {
            *pBool = 1;
            return 0;
         }
         break;
      case 3:
         if ( _tcsncicmp ( pValue, TEXT("Off"), 3 ) == 0 )
         {
            *pBool = 0;
            return 0;
         }
         else if ( _tcsncicmp ( pValue, TEXT("Yes"), 3 ) == 0 )
         {
            *pBool = 1;
            return 0;
         }
         break;
      case 4:
         if ( _tcsncicmp ( pValue, TEXT("True"), 4 ) == 0 )
         {
            *pBool = 1;
            return 0;
         }
         break;
      case 5:
         if ( _tcsncicmp ( pValue, TEXT("False"), 5 ) == 0 )
         {
            *pBool = 0;
            return 0;
         }
   }

   return CMDLN_VALUE_INVALID_TYPE;
}

/******************************************************************************/

unsigned long
ExtractTLWH (
   LPCTSTR  pValue,
   int      nValueChars,
   PVOID    pData,
   PVOID    pVoid )
/*
 * See cmdline.h for a description of this function.
 */
{
   int      *pDimensions;
   LPCTSTR  pEnd;
   int      nValues;

   pDimensions = (int*)pVoid;
   nValues = 0;

   pEnd = pValue + nValueChars;
   while ( pValue < pEnd )
   {
      BOOL  bNegative, bDefault;
      int   value;

      /* Is the number negative? */
      if ( *pValue == _TEXT ('-') )
      {
         bNegative = TRUE;
         pValue++;
      }
      else
      {
         bNegative = FALSE;
      }

      bDefault = TRUE;
      value = 0;
      while ( pValue < pEnd  )
      {
         if ( _istdigit ( *pValue ) )
         {
            value *= 10;
            value += *pValue - _TEXT ('0');
            bDefault = FALSE;
            pValue++;
         }
         else if ( *pValue == _TEXT (',') )
         {
            if ( nValues < (int)(ULONG)pData-1 )
            {
               pValue++;
               break;
            }
            return CMDLN_VALUE_TOOMANY_PARAMS;
         }
         else
         {
            /* Data pointed to is niether numeric or comma. */
            return CMDLN_VALUE_INVALID_TYPE;
         }
      }

      if ( !bDefault )
      {
         if ( !bNegative )
         {
            pDimensions[nValues] = value;
         }
         else
         {
            pDimensions[nValues] = -value;
         }
      }

      nValues++;
   }

   return 0;
}

/******************************************************************************/

unsigned long
ExtractColour ( 
   LPCTSTR  pValue,
   int      nValueChars,
   PVOID    pData,
   PVOID    pVoid )
/*
 * See cmdline.h for a description of this function.
 */
{
   unsigned long  error;
   unsigned long  cFound, dFound, nFound;
   byte           colour[3];
   LPCTSTR        pDigit;

   error = cFound = dFound = nFound = 0;
   pDigit = pValue;
   
   while ( ( *pDigit ) && ( error == 0 ) )
   {
      byte  value = 0;
      
      while ( *pDigit )
      {
         if ( _istdigit ( *pDigit ) )
         {
            /* Check that we are not going to overflow. */
            if ( dFound == 2 )
            {
               if ( ( value > 25 ) || ( ( value == 25 ) && ( *pDigit > '5' ) ) )
               {
                  error = CMDLN_VALUE_INVALID_TYPE;
                  break;
               }
            }
            else if ( dFound > 2 )
            {
               error = CMDLN_VALUE_INVALID_TYPE;
               break;
            }

            value *= 10;
            value += *pDigit - _TEXT ('0');
            pDigit++;
            dFound++;
         }
         else if ( *pDigit == _TEXT (',') )
         {
            if ( ( cFound < 3 ) && ( dFound > 0 ) )
            {
               cFound++;
               pDigit++;
               break;
            }   
            error = CMDLN_VALUE_INVALID_TYPE;
            break;
         }
         else
         {
            /* Data pointed to is niether numeric or comma. */
            error = CMDLN_VALUE_INVALID_TYPE;
            break;
         }

         colour[cFound] = value;
      }

      dFound = 0;
      nFound++;
   }

   if ( ( nFound == 3 ) && ( cFound == 2 ) )
   {
      *( ( COLORREF* ) pVoid ) = RGB ( colour[0], colour[1], colour[2] );
   }
   else
      error = CMDLN_VALUE_INVALID_TYPE;

   return error;
}

/******************************************************************************/

static unsigned long
ValidateParameter (
   LPTSTR         pKeyword,
   int            nKeywordChars,
   LPTSTR         pValue,
   int            nValueChars,
   int            nParameter,
   PKEYWORDTODATA pMapping,
   PVOID          pVoid,
   CMDLNERRFN     cmdLnErrFn )
/*
 * Summary: Validates and Processes the command line parameter.
 *
 * Purpose: Compares the keyword to the list of mapped keywords without regard
 *          to case. If a match is found the relevant extract function is
 *          called to process the value.
 *
 *          pKeyword is a pointer to the start of a keyword.
 *          nKeywordChars is an integer containing the length of the string
 *          pointed by pKeyword.
 *          pValue is a pointer to the start of an argument.
 *          nValueChars is an integer containing the length of the string
 *          pointed by pArgument.
 *          nParameter is the index number of keyword/argument at fault.
 *          pMapping is a pointer to a structure containing the list of
 *          keywords and associated processing information.
 *          pVoid is a pointer to a buffer for storing the exracted value.
 *          cmdLnErrFn is a pointer to a application specific error handling
 *          function for processing the error.
 *
 * Return:  TRUE, if the keyword and argument are processed successfully;
 *          otherwise FALSE is returned.
 */
{
   while ( pMapping->Keyword != NULL )
   {
      if ( nKeywordChars == (int)_tcslen ( pMapping->Keyword ))
      {
         if ( _tcsnicmp ( pMapping->Keyword, pKeyword, nKeywordChars ) == 0 )
         {
            TCHAR ch;
            unsigned long   error;

            /* TODO: Should really make a copy of the command line or the value
             * so we can put in NULL terminators after the value. Have not
             * noticed any problems with using the data passed to WinMain. */

            /* Temporarily replace the character after the value with a null
             * terminator. */
            ch = pValue[nValueChars];
            pValue[nValueChars] = '\0';

            error = ( pMapping->PExtractFn ( pValue, nValueChars,
                  pMapping->PVoid, ((char *)pVoid + pMapping->Offset ) ));

            pValue[nValueChars] = ch;

            if ( error )
            {
               return ( cmdLnErrFn ( error, pKeyword, nKeywordChars,
                     pValue, nValueChars, nParameter, pMapping, pVoid ));
            }
            return ( TRUE );
         }
      }
      pMapping++;
   }
   return ( cmdLnErrFn ( CMDLN_KEYWORD_NO_MATCH, pKeyword, nKeywordChars,
         pValue, nValueChars, nParameter, pMapping, pVoid ));
}

/******************************************************************************/

unsigned long
ParseCommandLine (
   LPTSTR         lpszCmdLine,
   PKEYWORDTODATA pMapping,
   PVOID          pVoid,
   PCMDLNERRFN    pCmdLnErrFn )
/*
 * Summary: Parses keyword(s) and argument(s) on the command line.
 *
 * Purpose: lpszCmdLine points to a string buffer containing a number of
 *          parameters. Each parameter consists of a keyword and a value.
 *
 *          A keyword is identifed by a hyphen at the beginning of the
 *          keyword and ending with an equal sign.
 *          A value is identifed by an equal sign (end of a keyword format)
 *          and ending with a space or terminating NULL character.
 *          For multiple parameters, a space is used to separate parameters.
 *          Double-quotes must be used on arguments that contain spaces.
 *          Double-quotes used within double-quotes would be entered as \"
 *
 *          Usage: -keyword=argument -keyword="Argument with space"
 *          Example: -station="Channel 5" -Channel=37   or,
 *                   -station="\"-station=BBC 1\""
 *
 *          pMapping is a pointer to a structure that describes the keywords
 *          and associated information for each as follows:
 *
 *           a) The keywords to be supported.
 *           b) The relevant functions for processing the arguments of each
 *              keyword.
 *           c) The location of a buffer, where the extracted value is to
 *                be store.
 *           d) (PVOID) spare parameter for extended use.
 *
 *          Example, where STARTUP is a user define structure for receiving data.
 *          KEYWORDTODATA mapping[] = {
 *            { "input", ExtractUnsignedInt, offsetof( STARTUP, Input ), NULL },
 *            { "windows", ExtractTLWH, offsetof( STARTUP, Dimensions ), NULL },
 *          }
 *
 *          pVoid is a pointer to a structure that will receive the extracted
 *            value.
 *            cmdLnErrFn is a pointer to an application specific error handling
 *          function for processing the keyword/argument at fault.
 *
 * Method:  The routine sets pointers to the beginning of each keyword/argument
 *          and counts the number of characters in each to determine the ends
 *          respectively.
 *          Validation is carried out on the format of each keyword/argument.
 *          If the keyword/argument does not conform to the command line format,
 *          the appropriate error code is given. The function pointer cmdLnErrFn
 *          is called and appropriate measures are taken.
 *
 * Return:  TRUE, If all keywords and arguments are processed successfully;
 *          otherwise FALSE is returned.
 */
{
   TCHAR nullString, *pKeyword, *pValue;
   int   nKeywordChars, nValueChars, nParameters = 0;

   nullString = '\0';
   nParameters = 1;

   /* Step over spaces at the beginning of the command line. */
   while ( *lpszCmdLine == ' ' )
   {
      lpszCmdLine++;
   }

   while ( *lpszCmdLine != '\0' )
   {
      if ( *lpszCmdLine == '-' )
      {
         BOOL  bQuotedString = FALSE;

         lpszCmdLine++;

         /* Find the key word. */
         pKeyword = lpszCmdLine;
         while ( 1 )
         {
            if (  ( *lpszCmdLine == '\0' ) ||
                  ( *lpszCmdLine == ' ' ) ||
                  ( *lpszCmdLine == '=' ))
            {
               break;
            }
            lpszCmdLine++;
         }
         nKeywordChars = (int)(lpszCmdLine - pKeyword);

         /* If the keyword is followed by an equals sign, find the parameter. */
         if ( *lpszCmdLine == '=' )
         {
            lpszCmdLine++;

            /* If the parameter starts with a double quote, we are looking
             * for a quoted string. */
            if ( *lpszCmdLine == '"' )
            {
               lpszCmdLine++;
               bQuotedString = TRUE;
            }
            else
            {
               bQuotedString = FALSE;
            }

            pValue = lpszCmdLine;

            while ( 1 )
            {
               if ( *lpszCmdLine == '\0' )
               {
                  if ( bQuotedString )
                  {
                     /* Reached the end of the command line without matching a
                      * double quote. */
                     if ( !pCmdLnErrFn ( CMDLN_UNMATCHED_DBLEQUOTES,
                           pKeyword, nKeywordChars, pValue, nValueChars,
                           nParameters, pMapping, pVoid ))
                     {
                        return (FALSE);
                     }
                  }
                  break;
               }
               else if ( *lpszCmdLine == ' ' )
               {
                  if ( !bQuotedString )
                  {
                     break;
                  }
               }
               else if ( *lpszCmdLine == '"' )
               {
                  if ( bQuotedString && ( *(lpszCmdLine - 1) != '\\' ))
                  {
                     /* A double quote without an escape indicates the end of
                      * a quoted string. Breaking at this point will cause the
                      * calculation of nValueChars to not include the last
                      * double quote (good) but will mean that the pointer
                      * to the command line lpszCmdLine will still be pointing
                      * at the double quote (bad).*/
                     break;
                  }
               }
               lpszCmdLine++;
            }
            nValueChars = (int)(lpszCmdLine - pValue);
         }
         else
         {
            /* Make pValue point to a string of zero length. If we made pValue
             * a NULL pointer, extract functions would potentially crash if the
             * command line was wrong. */
            pValue = &nullString;
            nValueChars = 0;
         }

         if ( nKeywordChars == 0 )
         {
            /* No keyword specified. */
            if ( !pCmdLnErrFn ( CMDLN_KEYWORD_NOT_SPECIFIED,
                  pKeyword, nKeywordChars, NULL, 0, nParameters,
                  pMapping, pVoid ))
            {
               return FALSE;
            }
         }
         else
         {
            if ( !ValidateParameter ( pKeyword, nKeywordChars, pValue,
                  nValueChars, nParameters, pMapping, pVoid, pCmdLnErrFn ))
            {
               return FALSE;
            }
         }

         /* If we have just processed a quoted string and are still pointing
          * at a double quote, advance the command line pointer. */
         if (( bQuotedString ) && ( *lpszCmdLine == '"' ))
         {
            lpszCmdLine++;
         }
      }
      else
      {
         /* Find a synchronisation point.
          * If we didn't find a synchronisation point, we would call the error
          * function for each character until we found acceptable data. I'm
          * sure that this would wind us up more than it would wind up our
          * customers! */
         pKeyword = lpszCmdLine;
         while (( *lpszCmdLine != '\0' ) && ( *lpszCmdLine != '-' ))
         {
            lpszCmdLine++;
         }
         nKeywordChars = (int)(lpszCmdLine - pKeyword);

         /* There is something on the command line that does not start with a
          * minus sign. */
         if ( !pCmdLnErrFn ( CMDLN_MISSING_HYPHEN,
               pKeyword, nKeywordChars, NULL, 0, nParameters,
               pMapping, pVoid ))
         {
            return FALSE;
         }
      }

      /* Count number of keywords/arguments on the command line. */
      nParameters++;

      /* Step over spaces between parameters. */
      while ( *lpszCmdLine == ' ' )
      {
         lpszCmdLine++;
      }
   }
   return ( TRUE );
}

/******************************************************************************/

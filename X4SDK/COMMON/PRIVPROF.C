/*******************************************************************************

Copyright Datapath Ltd. 2012.

File:    GetPrivateProfileInt.h

Purpose: Definitions for a GetPrivateProfileInt implementation function.


History:
         21 MAR 12    OM   Created.

*******************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <datatypes.h>
#include <privprof.h>

/******************************************************************************/

int
readstr(
   FILE *f,
   TCHAR *string )
{
   do
   {
      if (_fgetts(string, 255, f) == NULL)
      {
         return 1;
      }
   }
   
   while ((string[0] == TEXT('/')) || (string[0] == TEXT('\n')));
   return 0;
}

/******************************************************************************/

void
upstr(
   TCHAR *s )
{
  char  *p;

  for (p = s; *p != TEXT('\0'); p++)
     *p = toupper(*p);
}

/******************************************************************************/

int
GetPrivateProfileInt(
   TCHAR *lpAppName,
   TCHAR *lpKeyName,
   int nDefault,
   TCHAR *lpFileName )
/*
 * See GetPrivateProfileInt.h for a description of this function.
 */
{

   FILE *filein;
   TCHAR oneline[255];
   TCHAR appName[255];
   TCHAR keyName[255];
   TCHAR *pch;
   int appFlag;
   int keyFlag = 0;
   int stopFlag;
   int retval;
   int i;

   filein = (FILE*)_tfopen(lpFileName, TEXT("rt"));

   if (filein==NULL)
   {
      _tprintf(TEXT("Couldn't open the file: %s"),lpFileName);
      return 0;
   }

   /* AppName to upper case */
   i=0;
   while (lpAppName[i]!=TEXT('\0'))
   {
      appName[i]=(char)lpAppName[i];
      i++;
   }
   appName[i]=TEXT('\0');	
   upstr(appName);

   /* KeyName to upper case */
   i=0;
   while (lpKeyName[i]!=TEXT('\0'))
   {
      keyName[i]=(char)lpKeyName[i];
      i++;
   }
   keyName[i]=TEXT('\0');
   upstr(keyName);

   /* Search for AppName */
   stopFlag=0;
   appFlag=0;
   while (stopFlag==0 && appFlag==0)
   {
      /* Read next line */
      retval=readstr(filein, oneline);
      if (retval==1)
      {
         /* Reached end of file */
         stopFlag=1;
      }
      else
      {
         /* Is a section? */
         if (oneline[0]==TEXT('['))
         {
            upstr(oneline);
            if (_tcsstr(oneline,appName)!=NULL)
            {
               /* Found AppName */
               appFlag=1;
            }
         }
      }

   }

   /* if found AppName */
   if (appFlag==1)
   {
      /* Search for KeyName */
      stopFlag=0;
      keyFlag=0;
      while (keyFlag==0 && stopFlag==0)
      {
         /* Read next line */
         retval=readstr(filein, oneline);
         if (retval==1)
         {
            stopFlag=1;
         }
         else
         {
            /* Is not a Section? */
            if (oneline[0]!=TEXT('['))
            {
               pch = _tcstok (oneline, TEXT("="));
               /* To upper case */
               upstr(pch);
               if (_tcscmp(keyName,pch)==0)
               {
                  /* Found KeyName */
                  keyFlag=1;
               }
            }
            else
            {
               /* Didn't find KeyName */
               stopFlag=1;
            }
         }
      }
   }

   /* if found KeyName */
   if (keyFlag==1)
   {
      /* Get value */
      pch = _tcstok (NULL, TEXT("\n"));

      /* If value is a number */
      if (_istdigit(pch[0])!=0)
      {
         /* Return value as integer */
         return _ttoi(pch);
      }
   }

   return nDefault;
}

/******************************************************************************/

/*******************************************************************************

Copyright Datapath Ltd. 1997, 2010.

File:    CmdLine.h

Purpose: Declarations of functions etc. for processing command line arguments.

History:
         12 DEC 97   KML   Created.
         11 FEB 04    RL   Added ExtractBool.
         25 MAY 04    RL   Added ExtractLong.
         07 JUN 04    JH   Tidied up!
         18 JUN 04    JH   Added CMDLN_VALUE_NOT_SPECIFIED.
         04 MAY 05    RL   Added ExtractColour.
         29 JUN 05    RL   Added FLLIMIT structure.
         04 AUG 05    RL   Added ExtractUnsignedShort.
         30 MAY 06    JH   Ported to Unicode.
         28 APR 10    TM   Added CMDLN_INVALID_LOCATION.

*******************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************/

typedef unsigned long
(EXTRACTFN) (
   LPCTSTR  pValue,
   int      nValueChars,
   PVOID    pData,
   PVOID    pVoid );
typedef EXTRACTFN *PEXTRACTFN;

typedef struct
{
   LPCTSTR     Keyword;
   PEXTRACTFN  PExtractFn;
   int         Offset;
   PVOID       PVoid;
} KEYWORDTODATA, *PKEYWORDTODATA;

typedef unsigned long (CMDLNERRFN) (
   unsigned long  error,
   LPCTSTR        pKeyword,
   int            nKeywordChars,
   LPCTSTR        pValue,
   int            nValueChars,
   int            nParameter,
   PKEYWORDTODATA pMapping,
   PVOID          pVoid );
typedef CMDLNERRFN *PCMDLNERRFN;

extern unsigned long
ParseCommandLine (
   LPTSTR         lpszCmdLine,
   PKEYWORDTODATA pMapping,
   PVOID          pVoid,
   PCMDLNERRFN    pCmdLnErrFn );
/*
 * Summary: Parses a command line using a table describing parameters.
 *
 * Purpose:
 *          lpszCmdLine
 *             Points to the command line.
 *
 *          pMapping
 *             Points to the table that describes the command line parameters
 *             and how to extract the values.
 *
 *          pVoid
 *             Pointer used as a base. The offset value specified in the
 *             KEYWORDTODATA structure will be added to this pointer before
 *             it is passed to the extract function as the pVoid parameter.
 *
 *          pCmdLnErrFn
 *             Pointer to a function used to report errors on the command line.
 *
 * Return:  If successful, TRUE is returned; otherwise FALSE is returned.
 */

/******************************************************************************/

typedef struct
{
   ULONG  ulUpper;
   ULONG  ulLower;
}  LIMIT, *PLIMIT;

typedef struct
{
   float  flUpper;
   float  flLower;
}  FLLIMIT, *PFLLIMIT;

extern unsigned long
ExtractFloat (
   LPCTSTR  pValue,
   int      nValueChars,
   PVOID    pData,
   PVOID    pVoid );
/*
 * Summary: Extracts a float value from a string.
 *
 * Purpose:
 *          pValue
 *             Points to a string containing the value to extract.
 *
 *          nValueChars
 *             The number of characters in the string.
 *
 *          pData
 *             Points to a FLLIMIT structure containing two float values
 *             specifing the permitted range of values.
 *             If pData points to NULL, there is no range limitation.
 *
 *          pVoid
 *             Points to the location where the extracted value is stored.
 *
 * Return:  If the value is extracted successfully, 0 is returned; otherwise, an
 *          appropriate CMDLN_ error is returned.
 */

extern unsigned long
ExtractString (
   LPCTSTR  pValue,
   int      nValueChars,
   PVOID    pData,
   PVOID    pVoid );
/*
 * Summary: Copies a string.
 *
 * Purpose:
 *          pValue
 *             Points to a string.
 *
 *          nValueChars
 *             The number of characters in the string.
 *
 *          pData
 *             pData is an integer value specifing the size of the buffer into
 *             which the string is to be copied.
 *
 *          pVoid
 *             Points to the location where the extracted value is stored.
 *
 *          If the string is too large to fit in the supplied buffer, the string
 *          will be truncated.
 *
 *          If the string was quoted, the quotes will be removed.
 *
 * Return:  If the value is extracted successfully, 0 is returned; otherwise, an
 *          appropriate CMDLN_ error is returned.
 */

extern unsigned long
ExtractUnsignedInt (
   LPCTSTR  pValue,
   int      nValueChars,
   PVOID    pData,
   PVOID    pVoid );
/*
 * Summary: Extracts an unsigned integer value from a string.
 *
 * Purpose:
 *          pValue
 *             Points to a string containing the value to extract.
 *
 *          nValueChars
 *             The number of characters in the string.
 *
 *          pData
 *             Points to a LIMIT structure containing two unsigned long values
 *             specifing the permitted range of values.
 *             If pData points to NULL, there is no range limitation.
 *
 *          pVoid
 *             Points to the location where the extracted value is stored.
 *
 * Return:  If the value is extracted successfully, 0 is returned; otherwise, an
 *          appropriate CMDLN_ error is returned.
 */

extern unsigned long
ExtractUnsignedShort (
   LPCTSTR  pValue,
   int      nValueChars,
   PVOID    pData,
   PVOID    pVoid );
/*
 * Summary: Extracts an unsigned short value from a string.
 *
 * Purpose:
 *          pValue
 *             Points to a string containing the value to extract.
 *
 *          nValueChars
 *             The number of characters in the string.
 *
 *          pData
 *             Points to a LIMIT structure containing two unsigned long values
 *             specifing the permitted range of values.
 *             If pData points to NULL, there is no range limitation.
 *
 *          pVoid
 *             Points to the location where the extracted value is stored.
 *
 * Return:  If the value is extracted successfully, 0 is returned; otherwise, an
 *          appropriate CMDLN_ error is returned.
 */

extern unsigned long
ExtractLong (
   LPCTSTR  pValue,
   int      nValueChars,
   PVOID    pData,
   PVOID    pVoid );
/*
 * Summary: Extracts a signed long value from a string.
 *
 * Purpose:
 *          pValue
 *             Points to a string containing the value to extract.
 *
 *          nValueChars
 *             The number of characters in the string.
 *
 *          pData
 *             Points to a LIMIT structure containing two unsigned long values
 *             specifing the permitted range of values.
 *             If pData points to NULL, there is no range limitation.
 *
 *          pVoid
 *             Points to the location where the extracted value is stored.
 *
 * Return:  If the value is extracted successfully, 0 is returned; otherwise, an
 *          appropriate CMDLN_ error is returned.
 */

extern unsigned long
ExtractBool (
   LPCTSTR  pValue,
   int      nValueChars,
   PVOID    pData,
   PVOID    pVoid );
/*
 * Summary: Extracts a boolean value from a string.
 *
 * Purpose:
 *          pValue
 *             Points to a string containing the value to extract.
 *             A boolean value can be represented by any of the following
 *             strings: 0, 1, No, Yes, Off, On, False, True.
 *
 *          nValueChars
 *             The number of characters in the string.
 *
 *          pData
 *             pData is not used and should be set to NULL.
 *
 *          pVoid
 *             Points to the location where the extracted value is stored.
 *
 * Return:  If the value is extracted successfully, 0 is returned; otherwise, an
 *          appropriate CMDLN_ error is returned.
 */

enum
{
   CMDLINE_TLWH_TOP,
   CMDLINE_TLWH_LEFT,
   CMDLINE_TLWH_WIDTH,
   CMDLINE_TLWH_HEIGHT,
};

extern unsigned long
ExtractTLWH (
   LPCTSTR  pValue,
   int      nValueChars,
   PVOID    pData,
   PVOID    pVoid );
/*
 * Summary: Extracts integer values from a comma separated string.
 *
 * Purpose:
 *          pValue
 *             Points to a string containing the comma separated values to
 *             extract.
 *
 *          nValueChars
 *             The number of characters in the string.
 *
 *          pData
 *             An integer value specifing the number of values to obtain from
 *             the string.
 *
 *          pVoid
 *             Points to the location where the extracted value is stored.
 *
 * Return:  If the value is extracted successfully, 0 is returned; otherwise, an
 *          appropriate CMDLN_ error is returned.
 */

/******************************************************************************/

extern unsigned long
ExtractColour ( 
   LPCTSTR  pValue,
   int      nValueChars,
   PVOID    pData,
   PVOID    pVoid );

/*
 * Summary: Extracts rgb values from a comma separated string.
 *
 * Purpose:
 *          pValue
 *             Points to a string containing the comma separated values to
 *             extract.
 *
 *          nValueChars
 *             The number of characters in the string.
 *
 *          pData
 *             pData is not used and should be set to NULL.
 *
 *          pVoid
 *             Points to a COLORREF struct location where the extracted values 
 *             are stored.
 *
 * Return:  If the value is extracted successfully, 0 is returned; otherwise, an
 *          appropriate CMDLN_ error is returned.
 */

/******************************************************************************/

#define   CMDLN_BASEERROR                0x01150000
#define   CMDLN_VALUE_OUTOF_RANGE        ( CMDLN_BASEERROR + 0x0001 )
#define   CMDLN_VALUE_EXCEEDS_LENGTH     ( CMDLN_BASEERROR + 0x0002 )
#define   CMDLN_VALUE_TOOMANY_PARAMS     ( CMDLN_BASEERROR + 0x0003 )
#define   CMDLN_VALUE_INVALID_TYPE       ( CMDLN_BASEERROR + 0x0004 )
#define   CMDLN_KEYWORD_NOT_SPECIFIED    ( CMDLN_BASEERROR + 0x0006 )
#define   CMDLN_MISSING_HYPHEN           ( CMDLN_BASEERROR + 0x0007 )
#define   CMDLN_KEYWORD_NO_MATCH         ( CMDLN_BASEERROR + 0x0009 )
#define   CMDLN_UNMATCHED_DBLEQUOTES     ( CMDLN_BASEERROR + 0x000B )
#define   CMDLN_KEYWORD_DUPLICATED       ( CMDLN_BASEERROR + 0x000D )
#define   CMDLN_VALUE_NOT_SPECIFIED      ( CMDLN_BASEERROR + 0x000E )
#define   CMDLN_INVALID_LOCATION         ( CMDLN_BASEERROR + 0x000F )

/* Misspelt symbolic constant left in for posterity! */
#define   CMDLN_KEYWORD_MISSING_HYPEN    ( CMDLN_BASEERROR + 7 )

/* Old error values that are no longer used. */
#define   CMDLN_VALUE_DBLEQUOTES_REQ     ( CMDLN_BASEERROR + 5 )
#define   CMDLN_KEYWORD_MISSING_EQUAL    ( CMDLN_BASEERROR + 8 )
#define   CMDLN_NOVALUE_KEYWRD_SPACE     ( CMDLN_BASEERROR + 10 )
#define   CMDLN_UNKNOWN_PARAMETER        ( CMDLN_BASEERROR + 12 )

/******************************************************************************/

#ifdef __cplusplus
}
#endif

/******************************************************************************/

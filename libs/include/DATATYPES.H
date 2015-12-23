#ifndef __DATATYPES_H__
#define __DATATYPES_H__

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

// Windows Types
typedef int             BOOL;
typedef unsigned char   byte;
typedef unsigned char   BYTE;
typedef BYTE            *PBYTE;
typedef BYTE            *LPBYTE;
typedef unsigned short  USHORT;
typedef unsigned short  WORD;
typedef unsigned long   ULONG;
typedef unsigned long   DWORD;
typedef DWORD           *LPDWORD;
typedef DWORD           COLORREF;
typedef void            VOID;
typedef VOID            *PVOID;
typedef VOID            *LPVOID;
typedef void            *HINSTANCE;
typedef void            *FARPROC;
typedef void            *HANDLE;
typedef void            *OVERLAPPED;
typedef char            *LPSTR;
typedef const char      *LPCSTR;
typedef char            TCHAR;
typedef LPCSTR          LPCTSTR;
typedef LPSTR           LPTSTR;

// 64bit types
typedef unsigned long long ULONGLONG;
#ifdef __LP64__
   typedef unsigned long long ULONG_PTR;
#else
   typedef unsigned long      ULONG_PTR;
#endif

// Strings....
#define TEXT(quote)     quote
#define _TEXT(quote)    quote
#define __TEXT(quote)   quote
#define _fgetts         fgets
#define _istdigit       isdigit
#define _sntprintf      snprintf
#define _stscanf        sscanf
#define _tcscmp         strcmp
#define _tcscpy         strcpy
#define _tcslen         strlen
#define _tcsncicmp      strncasecmp
#define _tcsnicmp       strncasecmp
#define _tcsstr         strstr
#define _tcstok         strtok
#define _tfopen         fopen
#define _totupper       toupper
#define _tprintf        printf
#define _ttoi           atoi


// Calling conventions
#define __stdcall
#define __declspec(a)
#define WINAPI

// Misc defines
#define RGB(r,g,b) \
   ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

#endif
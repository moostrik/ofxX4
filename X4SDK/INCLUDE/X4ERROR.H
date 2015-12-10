/*******************************************************************************

Copyright Datapath Ltd. 2008.

File:    X4ERROR.H

Purpose: Definitions of X4 errors.


History:

*******************************************************************************/

#define X4ERROR_BASE                               0x01170000

#define X4USBERROR_BASE                            X4ERROR_BASE + 0x100

#define X4ERROR_NO_ERROR                                                 0
#define X4ERROR_OUT_OF_MEMORY                      ( X4ERROR_BASE + 0x0000 )
#define X4ERROR_BUFFER_TOO_SMALL                   ( X4ERROR_BASE + 0x0001 )
#define X4ERROR_INVALID_HANDLE                     ( X4ERROR_BASE + 0x0002 )
#define X4ERROR_INVALID_DEVICE                     ( X4ERROR_BASE + 0x0003 )
#define X4ERROR_INVALID_INPUT                      ( X4ERROR_BASE + 0x0004 )
#define X4ERROR_INVALID_OUTPUT                     ( X4ERROR_BASE + 0x0005 )
#define X4ERROR_INVALID_METHOD                     ( X4ERROR_BASE + 0x0006 )
#define X4ERROR_MUTEX_FAIL                         ( X4ERROR_BASE + 0x0007 )
#define X4ERROR_EDID_READ_FAIL                     ( X4ERROR_BASE + 0x0008 )
#define X4ERROR_EDID_WRITE_FAIL                    ( X4ERROR_BASE + 0x0009 )
#define X4ERROR_INVALID_EDID                       ( X4ERROR_BASE + 0x000A )
#define X4ERROR_UNSUPPORTED                        ( X4ERROR_BASE + 0x000B )

#define X4USBERROR_INVALID_HANDLE                  ( X4USBERROR_BASE + 0x01 )
#define X4USBERROR_READ_ERROR                      ( X4USBERROR_BASE + 0x02 )
#define X4USBERROR_RX_QUEUE_NOT_READY              ( X4USBERROR_BASE + 0x03 )
#define X4USBERROR_WRITE_ERROR                     ( X4USBERROR_BASE + 0x04 )
#define X4USBERROR_RESET_ERROR                     ( X4USBERROR_BASE + 0x05 )
#define X4USBERROR_INVALID_PARAMETER               ( X4USBERROR_BASE + 0x06 )
#define X4USBERROR_INVALID_REQUEST_LENGTH          ( X4USBERROR_BASE + 0x07 )
#define X4USBERROR_DEVICE_IO_FAILED                ( X4USBERROR_BASE + 0x08 )
#define X4USBERROR_INVALID_BAUDRATE                ( X4USBERROR_BASE + 0x09 )
#define X4USBERROR_FUNCTION_NOT_SUPPORTED          ( X4USBERROR_BASE + 0x0A )
#define X4USBERROR_GLOBAL_DATA_ERROR               ( X4USBERROR_BASE + 0x0B )
#define X4USBERROR_SYSTEM_ERROR_CODE               ( X4USBERROR_BASE + 0x0C )
#define X4USBERROR_READ_TIMED_OUT                  ( X4USBERROR_BASE + 0x0D )
#define X4USBERROR_WRITE_TIMED_OUT                 ( X4USBERROR_BASE + 0x0E )
#define X4USBERROR_IO_PENDING                      ( X4USBERROR_BASE + 0x0F )
#define X4USBERROR_DEVICE_NOT_FOUND                ( X4USBERROR_BASE + 0xFF )
#define X4USBERROR_NACK                            ( X4USBERROR_BASE + 0x100 )



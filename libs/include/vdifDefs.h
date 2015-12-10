#ifndef _DATPATH_VDIFDEFS_H_INCLUDED
#define _DATPATH_VDIFDEFS_H_INCLUDED

#define VDIF_FLAG_INTERLACED        0x0001
#define VDIF_FLAG_HSYNC_POSITIVE    0x0002
#define VDIF_FLAG_VSYNC_POSITIVE    0x0004
#define VDIF_FLAG_READ_ONLY         0x0008
#define VDIF_FLAG_NOSIGNAL          0x0010
#define VDIF_FLAG_OUTOFRANGE        0x0020
#define VDIF_FLAG_UNRECOGNISABLE    0x0040
#define VDIF_FLAG_NONVESA           0x0080
#define VDIF_FLAG_SDI               0x0100
#define VDIF_FLAG_DVI               0x0200
#define VDIF_FLAG_DVI_DUAL_LINK     0x0400
#define VDIF_FLAG_VIDEO             0x1000
#define VDIF_FLAG_HD_MODE           0x4000   /* Internal Flags. Do not use. */
#define VDIF_FLAG_YPRPB_CANDIDATE   0x8000   /* Internal Flags. Do not use. */

#define CUSTOM_TIMING_MODE          0
#define CVT_TIMING_MODE             1
#define CVTRB_TIMING_MODE           2
#define GTF_TIMING_MODE             3
#define SMPTE_TIMING_MODE           4

#endif

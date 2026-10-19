/* The launcher's Language Export: the PlayStation version's .DAV (texture pack), decoded into the PC's VDX7 layout. See
 * langexport_psxdav.cpp for the format. */
#ifndef SDW_LAUNCHER_LANGEXPORT_PSXDAV_H
#define SDW_LAUNCHER_LANGEXPORT_PSXDAV_H

#include "langexport.h"

/* the u32 at offset 4 of a decoded image ("PSXC"; a PC .DAV has "CHEK" there) */
#define PSXDAV_TAG 0x43585350u

/* whether data (size bytes: the whole file, or at least its first 0x44) is a PlayStation .DAV */
int ExportPsxDav_Is(const void *data, u32 size);

/* the VDX7 image of a PlayStation .DAV, in a new[]'d buffer of *outSize bytes; 0 (and *outSize 0) when the file is
 * malformed. The last conversion is kept: the same file decoded again costs nothing. */
char *ExportPsxDav_ToVdx7(const void *data, u32 size, u32 *outSize);

#endif

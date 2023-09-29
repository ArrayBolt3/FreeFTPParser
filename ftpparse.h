#ifndef FTPPARSE_H
#define FTPPARSE_H

/*
 * FreeFTPParser - A parser for FTP LIST responces
 * Copyright 2023 Aaron Rainbolt. Licensed under the 0BSD license.
 * No warranty.
 */

#define FTPPARSE_SIZE_UNKNOWN 0
#define FTPPARSE_SIZE_BINARY 1
#define FTPPARSE_SIZE_ASCII 1
#define FTPPARSE_MTIME_UNKNOWN 0
#define FTPPARSE_MTIME_LOCAL 1
#define FTPPARSE_MTIME_REMOTEMINUTE 2
#define FTPPARSE_MTIME_REMOTEDAY 3
#define FTPPARSE_ID_UNKNOWN 0
#define FTPPARSE_ID_FULL 1

#include <time.h>

struct ftpparse {
	char *name;      // The **non-zero-terminated** name of the file.

	int namelen;     // The length of the filename in `name`.

	int flagtrycwd;  // 1 if the file is a directory or link, else 0. Just
			 // because this field is set to 1 does NOT
			 // necessarily mean an attempt to change to this
			 // directory will succeed.

	int flagtryretr; // 1 if the file may be able to be downloaded, else
			 // 0. Just because this field is set to 1 does NOT
			 // necessarily mean an attempt to download this file
			 // will succeed.

	int sizetype;    // Specifies whether the length of the file is in an
			 // unknown unit size, ASCII characters, or bytes.

	int size;        // The length of the file, in `sizetype` units.

	int mtimetype;   // Specifies how accurate the modification time field
			 // is. FTPPARSE_MTIME_LOCAL = the server's timezone
			 // matches local time. FTPPARSE_MTIME_REMOTEMINUTE =
			 // the timezone may not match but day and time
			 // information are present. FTPPARSE_MTIME_REMOTEDAY
			 // = the timezone may not match and only day
			 // information is present.

	time_t mtime;    // The modification time of the file.

	int idtype;      // Whether the FTP server supports unique IDs for
			 // each file on the server. FTPPARSE_ID_UNKNOWN =
			 // unsupported, FTPPARSE_ID_FULL = supported.

	char *id;        // The **non-zero-terminated** ID of the file.

	int idlen;       // The length of the ID in `id`.
};

/*
 * !!! WARNING - If you feed a malformed string into ftpparse, the output
 * struct will contain incomplete data (or possibly no meaningful data at
 * all). 1 is returned if all parsing went well, 0 is returned if something
 * went wrong. If ftpparse returns 0, you MUST be prepared for the info in the
 * output struct to be incomplete, wrong, or blank.
 */
extern int ftpparse(struct ftpparse *, char *, int);

#endif

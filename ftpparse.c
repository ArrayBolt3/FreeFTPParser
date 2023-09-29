/*
 * FreeFTPParser - A parser for FTP LIST responces
 * Copyright 2023 Aaron Rainbolt. Licensed under the 0BSD license.
 * No warranty.
 */

#include <ftpparse.h>

#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

const int FTPPARSE_LIST_UNKNOWN = 0;
const int FTPPARSE_LIST_UNIX = 1;
const char UNIX_PERM_CHARS[8] = {
	'd', 'r', 'w', 'x', 's', 'S', 'l', '-'
};
const char *UNIX_MONTH_NAMES[12] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct",
	"Nov", "Dec"
};

char **tokenize(char *input, int inputlen, size_t *outputlen, char separator, int seplen) {
	size_t tokens  = 0;
	size_t tkidx   = 0;
	size_t i       = 0;
	size_t j       = 0;
	int sepsfound  = 0;
	size_t cpstart = 0;
	size_t cplen   = 0;
	bool mktoken   = false;
	char **output  = NULL;

	for(i = 0;i < inputlen;i++) {
		if(input[i] != separator) {
			mktoken = true;
			if (i == (inputlen - 1)) {
				tokens++;
			}
		} else {
			if (mktoken == true) {
				sepsfound++;
				if (sepsfound == seplen) {
					tokens++;
					sepsfound = 0;
					mktoken = false;
				}
			}
		}
	}

	output = calloc(tokens, sizeof(char *));
	mktoken = false;

	for(i = 0;i < inputlen;i++) {
		if(input[i] != separator) {
			if(cplen == 0) {
				cpstart = i;
				cplen = 1;
			} else {
				cplen++;
			}

			if(i == (inputlen - 1)) {
				output[tkidx] = calloc(cplen + 1, sizeof(char));
				memmove(output[tkidx], input + cpstart, cplen);
				output[tkidx][cplen] = '\0';
			}
		} else {
			if(cplen != 0) {
				sepsfound++;
				cplen++;
				if (sepsfound == seplen) {
					cplen -= seplen;
					output[tkidx] = calloc(cplen + 1, sizeof(char));
					memmove(output[tkidx], input + cpstart, cplen);
					output[tkidx][cplen] = '\0';
					tkidx++;
					cplen = 0;
					sepsfound = 0;
				}
			}
		}
	}

	*outputlen = tokens;
	return output;
}

size_t indexofc(const char *arr, const size_t arrlen, const char element) {
	size_t i = 0;
	for(i = 0;i < arrlen;i++) {
		if (arr[i] == element) {
			return i;
		}
	}
	return -1;
}

size_t indexofs(const char **arr, const size_t arrlen, const char *element) {
	size_t i = 0;
	for(i = 0;i < arrlen;i++) {
		if (strcmp(arr[i], element) == 0) {
			return i;
		}
	}
	return -1;
}

int ftpparse(struct ftpparse *output, char *input, int inputlen) {
	int listtype = FTPPARSE_LIST_UNIX;
	size_t i = 0;
	char **tokens;
	size_t numtokens;
	struct tm timeinfo;
	int monthnum;
	
	if(inputlen < 11) { // ten chars indicating permissions, plus a null terminator
		return -0;
	}

	for(i = 0;i < 10;i++) {
		if(indexofc(UNIX_PERM_CHARS, 8, input[i]) == -1) {
			listtype = FTPPARSE_LIST_UNKNOWN;
			break;
		}
	}
	if(listtype == FTPPARSE_LIST_UNKNOWN) {
		return 0;
	} else if (listtype == FTPPARSE_LIST_UNIX) {
		tokens = tokenize(input, inputlen, &numtokens, ' ', 1);
		if(numtokens < 8) {
			return 0;
		}

		/*
		 * Field 0 = permissions and type info, from this we can
		 * deduce flagtrycwd and flagtryretr.
		 * 
		 * Fields 1 through 3 = don't care
		 *
		 * Field 4 = file size in don't know units (we can't tell if
		 * the user is in ASCII or Binary mode)
		 *
		 * Field 5 = modified month
		 *
		 * Field 6 = modified day
		 *
		 * Field 7 = EITHER modified time OR modified year
		 *
		 * THERE MAY BE EXTRA FIELDS AFTER FIELD 7!!!
		 *
		 * Field len-1 = file/directory name IF the file is not a link
		 *
		 * Field len-3 = file/directory name IF the file is a link
		 */

		if(tokens[0][0] != 'l') {
			output->name = calloc(strlen(tokens[numtokens - 1]), sizeof(char));
			memmove(output->name, tokens[numtokens - 1], strlen(tokens[numtokens - 1]));
			output->namelen = strlen(tokens[numtokens - 1]);
		} else {
			output->name = calloc(strlen(tokens[numtokens - 3]), sizeof(char));
			memmove(output->name, tokens[numtokens - 3], strlen(tokens[numtokens - 3]));
			output->namelen = strlen(tokens[numtokens - 3]);
		}

		if(tokens[0][0] != '-') {
			output->flagtrycwd = 1;
		} else {
			output->flagtrycwd = 0;
		}

		if(indexofc(tokens[0], 10, 'r') != -1 && tokens[0][0] != 'd') {
			output->flagtryretr = 1;
		} else {
			output->flagtryretr = 0;
		}

		output->sizetype = FTPPARSE_SIZE_UNKNOWN;
		output->size = atol(tokens[4]);

		monthnum = indexofs(UNIX_MONTH_NAMES, 12, tokens[5]);
		if (monthnum == -1) {
			return 0;
		} else {
			timeinfo.tm_mon = monthnum;
		}

		timeinfo.tm_mday = atoi(tokens[6]);

		if(indexofc(tokens[7], strlen(tokens[7]), ':') != -1) {
			size_t timepartslen;
			char **timeparts = tokenize(tokens[7], 5, &timepartslen, ':', 1);
			if (timepartslen != 2) {
				return 0;
			}
			timeinfo.tm_hour = atoi(timeparts[0]);
			timeinfo.tm_min = atoi(timeparts[1]);
			output->mtimetype = FTPPARSE_MTIME_REMOTEMINUTE;
		} else {
			timeinfo.tm_year = atoi(tokens[7]) - 1900;
			output->mtimetype = FTPPARSE_MTIME_REMOTEDAY;
		}

		output->mtime = mktime(&timeinfo);

		// There are no unique identifiers in UNIX LIST output
		output->idtype = FTPPARSE_ID_UNKNOWN;
		output->id = NULL;
		output->idlen = 0;
	}

	return 1;
}

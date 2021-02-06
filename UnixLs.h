#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>
#include <libgen.h>
#include "infodemo.h"

void printData(struct stat buf);
void lsRecursive(_Bool lslong, _Bool lsnode, const char* directory);
void lsBasic(_Bool lslong, _Bool lsnode, _Bool lsrec, const char* directory);
void lsmultiple(_Bool lslong, _Bool lsnode, _Bool lsrec, char** directories, int limit);
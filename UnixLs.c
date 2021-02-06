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
#include <libgen.h>
#include <limits.h>
#include <stdint.h>
#include <inttypes.h>
#include "UnixLs.h"
#include "infodemo.h"

void printData(struct stat buf) {
	// First handle type of file and permissions
	// For now print d for directory and '-' for anything else
	if (S_ISDIR(buf.st_mode) != 0) {
       printf("d");
   	}
   	else if ((buf.st_mode & S_IFMT) == S_IFLNK) {
		printf("l");
	}
   	else {
   		printf("-");
   	}

   	// Owner permissions
   	if (buf.st_mode & S_IRUSR) {
       printf("r");
   	}
   	else {
   		printf("-");
   	}
   	if (buf.st_mode & S_IWUSR) {
       printf("w");
   	}
   	else {
   		printf("-");
   	}
   	if (buf.st_mode & S_IXUSR) {
       printf("x");
   	}
   	else {
   		printf("-");
   	}

   	// Group permissions
   	if (buf.st_mode & S_IRGRP) {
       printf("r");
   	}
   	else {
   		printf("-");
   	}
   	if (buf.st_mode & S_IWGRP) {
       printf("w");
   	}
   	else {
   		printf("-");
   	}
   	if (buf.st_mode & S_IXGRP) {
       printf("x");
   	}
   	else {
   		printf("-");
   	}

   	// Other permissions
   	if (buf.st_mode & S_IROTH) {
       printf("r");
   	}
   	else {
   		printf("-");
   	}
   	if (buf.st_mode & S_IWOTH) {
       printf("w");
   	}
   	else {
   		printf("-");
   	}
   	if (buf.st_mode & S_IXOTH) {
       printf("x");
   	}
   	else {
   		printf("-");
   	}
   	printf(" %2ld ", buf.st_nlink);

	getAndPrintUserName(buf.st_uid);
	printf(" ");
	getAndPrintGroup(buf.st_gid);
	printf(" %5ld ", buf.st_size);

	struct tm date = *(localtime(&buf.st_mtime));
	switch(date.tm_mon) {
		case 0:
			printf("Jan ");
			break;
		case 1:
			printf("Feb ");
			break;
		case 2:
			printf("Mar ");
			break;
		case 3:
			printf("Apr ");
			break;
		case 4:
			printf("May ");
			break;
		case 5:
			printf("Jun ");
			break;
		case 6:
			printf("Jul ");
			break;
		case 7:
			printf("Aug ");
			break;
		case 8:
			printf("Sep ");
			break;
		case 9:
			printf("Oct ");
			break;
		case 10:
			printf("Nov ");
			break;
		case 11:
			printf("Dec ");
			break;
		default:
			printf("??? ");
			break;
	}
	printf("%2d %d ", date.tm_mday, date.tm_year + 1900);
	if (date.tm_hour < 10) {
		printf("0");
	}
	printf("%d:", date.tm_hour);
	if (date.tm_min < 10) {
		printf("0");
	}
	printf("%d ", date.tm_min);
	return;
}

_Bool isProgram(struct stat buf) {
	if (!S_ISDIR(buf.st_mode)) {
		// If the file is a symbolic link don't treat it like a program
		if ((buf.st_mode & S_IFMT) == S_IFLNK) {
			return false;
		}
		if (buf.st_mode & S_IXUSR || buf.st_mode & S_IXGRP || buf.st_mode & S_IXOTH) {
			return true;
		}
	}
	return false;
}

void lsRecursive(_Bool lslong, _Bool lsnode, const char* directory) {
	if (directory == NULL) {
		directory = ".";
	}
	printf("%s:\n", directory);
	// Printing contents of the directory
	lsBasic(lslong, lsnode, false, directory);

	// Open a stream to loop through contents of directory and find any other
	// directories
	struct stat buf;
	struct dirent* d;
	DIR* dirStream = opendir(directory);
	if (dirStream == NULL) {
		// Attempt to see if a file was input
		if (lstat(directory, &buf) != 0) {
			printf("UnixLs REC: cannot access '%s': No such file or directory\n", directory);
			return;
		} 
		// If we've reached this point then we're either recursing on a text file
		// or have tried to opdendir a restricted directory
		else {
			if (S_ISDIR(buf.st_mode)) {
				printf("UnixLs: cannot open directory '%s': Permission denied\n", directory);
				return;
			}
			// Is a file, don't recurse into it or print it, it has already been printed
			return;
		}
	}
	while ((d = readdir(dirStream)) != NULL) {
		// Making the full directory path 10KB just in case path is huge
		char concatDir[10240];
		memset(&concatDir, 0, 10240);
		strcpy(concatDir, directory);
		if (strcmp(directory, "/") != 0) {
			strcat(concatDir, "/");
		}
		strcat(concatDir, (char* restrict)d->d_name);
		if (lstat(concatDir, &buf) == 0) {
			// If the file is a directory call lsRecursive on it
			if (S_ISDIR(buf.st_mode)) {
				// First print a space
				if (d->d_name[0] != '.') {
					printf("\n");
					lsRecursive(lslong, lsnode, concatDir);
				}
			}
		}
	}
	closedir(dirStream);

	return;
}

void lsBasic(_Bool lslong, _Bool lsnode, _Bool lsrec, const char* directory) {
	struct dirent* d;
	struct stat buf;
	if (directory == NULL) {
		directory = "."; // Current directory path
	}

	if (lsrec) {
		lsRecursive(lslong, lsnode, directory);
		return;
	}

	DIR* dirStream = opendir(directory);
	if (dirStream == NULL) {
		// Attempt to see if a file was input
		if (lstat(directory, &buf) != 0) {
			printf("UnixLs: cannot access '%s': No such file or directory\n", directory);
			return;
		} else {
			// If we couldn't open a directory stream for a file but it can
			// fill a stat then it is a restricted directory
			if (S_ISDIR(buf.st_mode)) {
				printf("UnixLs: cannot open directory '%s': Permission denied\n", directory);
				return;
			}
			if (lsnode) {
				printf("%ju ", (uintmax_t)buf.st_ino);
			}
			if (lslong) {
				printData(buf);
			}
			printf("%s", directory);
			if (lslong && (buf.st_mode & S_IFMT) == S_IFLNK) {
				memset(&buf, 0, sizeof(struct stat));
				stat(directory, &buf);
				char linkPath[3024];
				memset(&linkPath, 0, 3024);
				readlink(directory, linkPath, 3024);
				printf(" -> %s", linkPath);
				// Adding file specifications for the linked-to file
			}
			printf("\n");
			return;
		}
	}

	while ((d = readdir(dirStream)) != NULL) {
		if (d->d_name[0] != '.') {
			// Making the full directory path 10KB just in case path is huge
			char concatDir[10240];
			memset(&concatDir, 0, 10240);
			strcpy(concatDir, directory);
			if (strcmp(directory, "/") != 0) {
				strcat(concatDir, "/");
			}
			strcat(concatDir, (char* restrict)d->d_name);
			//printf("The full path\n");
			//puts(concatDir);
			if (lstat(concatDir, &buf) == 0) {
				if (lsnode) {
					printf("%ju ", (uintmax_t)buf.st_ino);
				}
				if (lslong) {
					printData(buf);
				}
				printf("%s", d->d_name);
				if (lslong && (buf.st_mode & S_IFMT) == S_IFLNK) {
					memset(&buf, 0, sizeof(struct stat));
					stat(concatDir, &buf);
					char linkPath[3024];
					memset(&linkPath, 0, 3024);
					readlink(concatDir, linkPath, 3024);
					printf(" -> %s", linkPath);
				}
				printf("\n");
			}
			else {
				printf("lstat didn't work for %s\n", d->d_name);
			}
		}
	}
	closedir(dirStream);
	return;
}

void lsmultiple(_Bool lslong, _Bool lsnode, _Bool lsrec, char** directories, int limit) {
	char** currentDir = directories;
	struct stat checkBuf;
	for (int i = 0; i < limit; i++) {
		if (*(*currentDir) == '-') {
			printf("UnixLs: options must be put before directory or file\n");
			exit(1);
		}
		else {
			if (lstat(*currentDir, &checkBuf) != 0) {
				printf("UnixLs: cannot access '%s': No such file or directory\n", *currentDir);
			}
			else {
				if (!lsrec) {
					printf("%s:\n", *currentDir);
				}
				lsBasic(lslong, lsnode, lsrec, *currentDir);
				if (i != limit - 1) {
					printf("\n");
				}
			}
		}
		currentDir++;
	}
}
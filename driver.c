#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "UnixLs.h"
#include "infodemo.h"

int main(int argc, char* argv[]) {	
		// -F and -a are assumed always true
		bool lslong = false; // -l, long info option (lowercase "L")
		bool lsnode = false; // -i, node option
		bool lsrec = false; // -R, recursive option (from indicated directory to all leaves)
		const char* directory = NULL;

		// Part 1: Decoding input, if any
		if (argc > 1) {
			// Going through all inputs
			for (int i = 1; i < argc; i++) {
				// Every flag string should start with a dash
				if (*(argv[i]) == '-') {
					// If a flag is put after a directory print an error message and exit
					if (directory != NULL) {
						printf("UnixLs: options must be put before directory or file\n");
						exit(1);
					}
					// Following ls standards any characters in the flag
					// are considered, in this case only i, l, and R are valid
					// inputs. So ./UnixLs -lRhotdog would not ignore "hotdog"
					// and the program would exit, unforunately
					for (int j = 1; j < strlen(argv[i]); j++) {
						if (*(argv[i] + j) == 'l') {
							lslong = true;
						}
						else if (*(argv[i] + j) == 'i') {
							lsnode = true;
						}
						else if (*(argv[i] + j) == 'R') {
							lsrec = true;
						}
						else {
							printf("UnixLs: invalid option -- '%c'\n", *(argv[i] + j));
							printf("Try reading the assignment description for more information.\n");
							exit(1);
						}
						//printf("CharL %c\n", *(argv[i] + j));
					}
				}
				// Option doesn't begin with a '-' therefore it will be treated as a
				// directory/file
				else {
					directory = argv[i];
					// If the first character of the string argv entry after a directory
					// does not start with '-' treat it as a directory and run
					// lsmultiple
					if ((i + 1) < argc && *(argv[i + 1]) != '-') {
						lsmultiple(lslong, lsnode, lsrec, argv + i, argc - i);
						return 0;
					} else {
						lsBasic(lslong, lsnode, lsrec, directory);
					}
				}
			}
		}
		
		// If flags but no directory was passed in or no flags or
		// directory was passed in then this will be called
		if (directory == NULL) {
			// Executing the user inputs
			lsBasic(lslong, lsnode, lsrec, directory);
		}
	return 0;
}
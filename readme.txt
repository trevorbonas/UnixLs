Extra information about UnixLs

Since ls allows for directories to come before options deciding what to do when an option comes after a directory was up to me. I decided that as long as input is good the execution will occur until the execution is found to be invalid. For example: 
	./UnixLs -R -i /baseball_cards/big_hitters -l ~/Documents 
the directory (or file) /baseball_cards/big_hitters will be recursively printed to stdout with i node numbers, after that the program will identify -l as an option and simply output an error message and shutdown. ~/Documents will not be printed to stdout.

Like ls if a non-valid or not-permitted directory is passed in an error message will be printed, but execution will not stop.
For example:
	./UnixLs notexist ~/Documents
The program would first put out an error message for notexist directory then print the contents of ~/Documents.
This is exactly what ls does.

As mentioned in the assignment description all entries are printed out in one column, entries are not in alphabetical order, and option -l omits the "total" line.

The maximum character length of a given directory supported by UnixLs is 10,240 characters, just in case some absurd test are run on it.

As per an answer given on Piazza the formatting/alignment of -l is not the same as ls. We are
supposed to "make a reasonable guess for field lengths," so I did just that. For the link column
I chose a right alignment of 2, since I find there is always at least one file in a directory
with > 10 links and most files don't have more than 99 links. For the size column that was
kind of tricky, in my Projects folder, which holds all my CMPT or personal projects files tended
to be between 1 and 4 thousand bytes. But in my Downloads folder movies were completely massive,
billions of bytes maybe. I'm going to assume that UnixLs will be used in a CMPT context where files
aren't massive, so I chose a formatting length for size of 5. Most dates are more than two characters
so I set 2 for the days.
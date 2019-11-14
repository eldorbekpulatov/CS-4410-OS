/* Author: Robbert van Renesse 2018
 *
 * The interface is as follows:
 *	reader_t reader_create(int fd);
 *		Create a reader that reads characters from the given file descriptor.
 *
 *	char reader_next(reader_t reader):
 *		Return the next character or -1 upon EOF (or error...)
 *
 *	void reader_free(reader_t reader):
 *		Release any memory allocated.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "shall.h"

struct reader {
	int fd;
	char buffer[512];
	int index; // 0-511 
	int size;  // 512
	// fully read: if index == size
};

reader_t reader_create(int fd){
	reader_t reader = (reader_t) calloc(1, sizeof(*reader));
	reader->fd = fd;
	return reader;
}

char reader_next(reader_t reader){
	if (reader->index == reader->size){ // if buffer is fully read
		int n = read(reader->fd, &(reader->buffer), 512);  // read next 512
		if (n == 0 ){ return EOF; } // if nothing is read, return EOF
		reader->index = 0; // put the index back at the beginning
		reader->size = n; // size of the bufffer
	}
	char nextchar = reader->buffer[reader->index]; // get the nextchar
	reader->index += 1; // increment the index of the followup
	return nextchar; // return nextchar
}

void reader_free(reader_t reader){
	free(reader);
}

#ifndef TACA_H
#define TACA_H

#include <linux/types.h>

struct taca_video {
	ssize_t lyrics_len;
	char *lyrics;
	ssize_t data_len;
	char data[];
};

#endif /* TACA_H */

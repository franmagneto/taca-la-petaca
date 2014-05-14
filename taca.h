#ifndef TACA_H
#define TACA_H

#include <linux/types.h>

struct taca_video {
	char *lyrics;
	ssize_t length;
	char data[];
};

#endif /* TACA_H */

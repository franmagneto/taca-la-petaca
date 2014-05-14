#include "taca.h"

#define VIDEO_LENGTH 5986482

struct taca_video video = {
	.len = VIDEO_LENGTH,
	.data = {
#include "video_data.h"
	}
};

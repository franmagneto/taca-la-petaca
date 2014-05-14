#include "taca.h"
#include "video_lyrics.h"

#define VIDEO_LENGTH 5986482

struct taca_video video = {
	.lyrics = VIDEO_LYRICS,
	.length = VIDEO_LENGTH,
	.data = {
#include "video_data.h"
	}
};

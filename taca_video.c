#include "taca.h"
#include "video_lyrics.h"

#define VIDEO_LEN 5986482
#define LYRICS_LEN 844

/**
 * Inicialização dos dados da letra e do vídeo
 */
struct taca_video video = {
	.lyrics_len = LYRICS_LEN,
	.lyrics = VIDEO_LYRICS,
	.data_len = VIDEO_LEN,
	.data = {
#include "video_data.h"
	}
};

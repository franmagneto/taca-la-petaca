/*
 * Taca la Petaca - Módulo de kernel
 * =================================
 *
 * Autor: Francisco Machado Magalhães Neto
 *
 * Maio de 2014
 *
 * Módulo para o kernel do Linux que cria o dispositivo `/dev/taca`, que
 * contém o vídeo *Chapolin - Taca La Petaca*, e a entrada `/proc/taca`,
 * com a letra da música.
 *
 * O código foi baseado no álbum *Cycles Per Instruction*, da banda
 * [netcat](http://www.netcat.co/), lançado como módulo de kernel.
 *
 * Também foi usado como referência o livro *Linux Device Drivers*.
 */

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

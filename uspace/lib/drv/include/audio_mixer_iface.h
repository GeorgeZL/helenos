/*
 * Copyright (c) 2011 Jan Vesely
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/** @addtogroup libdrv
 * @addtogroup usb
 * @{
 */
/** @file
 * @brief USB host controller interface definition.
 */

#ifndef LIBDRV_AUDIO_MIXER_IFACE_H_
#define LIBDRV_AUDIO_MIXER_IFACE_H_

#include <async.h>
#include <bool.h>

#include "ddf/driver.h"

int audio_mixer_get_info(async_exch_t *, const char **, unsigned *);
int audio_mixer_get_item_info(async_exch_t *, unsigned,
    const char **, unsigned *);
int audio_mixer_get_channel_info(async_exch_t *, unsigned, unsigned,
    const char **, unsigned *);
int audio_mixer_channel_mute_set(async_exch_t *, unsigned, unsigned, bool);
int audio_mixer_channel_mute_get(async_exch_t *, unsigned, unsigned, bool *);
int audio_mixer_channel_volume_set(async_exch_t *, unsigned, unsigned,
    unsigned);
int audio_mixer_channel_volume_get(async_exch_t *, unsigned, unsigned,
    unsigned *, unsigned *);


/** Audio mixer communication interface. */
typedef struct {
	int (*get_info)(ddf_fun_t *, const char **, unsigned *);
	int (*get_item_info)(ddf_fun_t *, unsigned, const char **, unsigned *);
	int (*get_channel_info)(ddf_fun_t *, unsigned, unsigned,
	    const char **, unsigned *);
	int (*channel_mute_set)(ddf_fun_t *, unsigned, unsigned, bool);
	int (*channel_mute_get)(ddf_fun_t *, unsigned, unsigned, bool *);
	int (*channel_volume_set)(ddf_fun_t *, unsigned, unsigned, unsigned);
	int (*channel_volume_get)(ddf_fun_t *, unsigned, unsigned,
	    unsigned *, unsigned *);
} audio_mixer_iface_t;

#endif
/**
 * @}
 */


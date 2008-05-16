/*
 * Copyright 2008 Arsen Chaloyan
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __MPF_CODEC_DESCRIPTOR_H__
#define __MPF_CODEC_DESCRIPTOR_H__

/**
 * @file mpf_codec_descriptor.h
 * @brief MPF Codec Descriptor
 */ 

#include "apt_string.h"
#include "mpf.h"

APT_BEGIN_EXTERN_C

/** Codec frame time base in msec */
#define CODEC_FRAME_TIME_BASE 10
/** Bytes per sample for linear pcm */
#define BYTES_PER_SAMPLE 2
/** Bits per sample for linear pcm */
#define BITS_PER_SAMPLE 16

/** Codec descriptor declaration */
typedef struct mpf_codec_descriptor_t mpf_codec_descriptor_t;
/** Codec list declaration */
typedef struct mpf_codec_list_t mpf_codec_list_t;
/** Codec frame declaration */
typedef struct mpf_codec_frame_t mpf_codec_frame_t;
/** Codec attributes declaration */
typedef struct mpf_codec_attribs_t mpf_codec_attribs_t;


/** Codec descriptor */
struct mpf_codec_descriptor_t {
	/** Payload type used in RTP packet */
	apr_byte_t   payload_type;
	/** Codec name */
	apt_str_t    name;
	/** Sampling rate */
	apr_uint16_t sampling_rate;
	/** Channel count */
	apr_byte_t   channel_count;
	/** Codec dependent additional format */
	const char  *format;
};

/** List of codec descriptors */
struct mpf_codec_list_t {
	/** Dynamically allocated array of codec descriptors */
	mpf_codec_descriptor_t *codecs;
	/** Bounds of allocated array */
	apr_size_t              max_count;
	/** Number of descriptors already appended to the list */
	apr_size_t              count;
};

/** Codec frame */
struct mpf_codec_frame_t {
	/** Raw buffer, which may contain encoded or decoded data */
	void      *buffer;
	/** Buffer size */
	apr_size_t size;
};

/** Codec attributes */
struct mpf_codec_attribs_t {
	/** Bits per sample */
	apr_byte_t   bits_per_samples;
};


/** Initialize codec descriptor */
static APR_INLINE void mpf_codec_descriptor_init(mpf_codec_descriptor_t *descriptor)
{
	descriptor->payload_type = 0;
	apt_string_reset(&descriptor->name);
	descriptor->sampling_rate = 0;
	descriptor->channel_count = 0;
	descriptor->format = NULL;
}

/** Calculate encoded frame size in bytes */
static APR_INLINE apr_size_t mpf_codec_frame_size_calculate(const mpf_codec_descriptor_t *descriptor, const mpf_codec_attribs_t *attribs)
{
	return descriptor->channel_count * attribs->bits_per_samples * CODEC_FRAME_TIME_BASE * 
			descriptor->sampling_rate / 1000 / 8; /* 1000 - msec per sec, 8 - bits per byte */
}

/** Calculate samples of the frame (ts) */
static APR_INLINE apr_size_t mpf_codec_frame_samples_calculate(const mpf_codec_descriptor_t *descriptor)
{
	return descriptor->channel_count * CODEC_FRAME_TIME_BASE * descriptor->sampling_rate / 1000;
}

/** Calculate linear frame size in bytes */
static APR_INLINE apr_size_t mpf_codec_linear_frame_size_calculate(apr_uint16_t sampling_rate, apr_byte_t channel_count)
{
	return channel_count * BYTES_PER_SAMPLE * CODEC_FRAME_TIME_BASE * sampling_rate / 1000;
}

/** Match two codec descriptors */
static APR_INLINE apr_size_t mpf_codec_descriptor_match(const mpf_codec_descriptor_t *descriptor1, const mpf_codec_descriptor_t *descriptor2)
{
	apt_bool_t match = FALSE;
	if(descriptor1->payload_type < 96 && descriptor2->payload_type < 96) {
		if(descriptor1->payload_type == descriptor2->payload_type) {
			match = TRUE;
		}
	}
	else {
		if(apt_string_compare(&descriptor1->name,&descriptor2->name) == TRUE) {
			if(descriptor1->sampling_rate == descriptor2->sampling_rate && 
				descriptor1->channel_count == descriptor2->channel_count) {
				match = TRUE;
			}
		}
	}
	return match;
}


/** Reset list of codec descriptors */
static APR_INLINE void mpf_codec_list_reset(mpf_codec_list_t *codec_list)
{
	codec_list->codecs = NULL;
	codec_list->max_count = 0;
	codec_list->count = 0;
}

/** Initialize list of codec descriptors */
static APR_INLINE void mpf_codec_list_init(mpf_codec_list_t *codec_list, apr_size_t max_count, apr_pool_t *pool)
{
	codec_list->codecs = apr_palloc(pool,sizeof(mpf_codec_descriptor_t)*max_count);
	codec_list->max_count = max_count;
	codec_list->count = 0;
}

/** Increment number of codec descriptors in the list and return the descriptor to fill */
static APR_INLINE mpf_codec_descriptor_t* mpf_codec_list_add(mpf_codec_list_t *codec_list)
{
	mpf_codec_descriptor_t *descriptor;
	if(codec_list->count >= codec_list->max_count) {
		return NULL;
	}
	descriptor = &codec_list->codecs[codec_list->count++];
	mpf_codec_descriptor_init(descriptor);
	return descriptor;
}

APT_END_EXTERN_C

#endif /*__MPF_CODEC_DESCRIPTOR_H__*/
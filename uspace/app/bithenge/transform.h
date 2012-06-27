/*
 * Copyright (c) 2012 Sean Bartell
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

/** @addtogroup bithenge
 * @{
 */
/**
 * @file
 * Transforms.
 */

#ifndef BITHENGE_TRANSFORM_H_
#define BITHENGE_TRANSFORM_H_

#include "blob.h"
#include "tree.h"

/** A transform that creates a new tree from an old tree. */
typedef struct {
	/** @privatesection */
	const struct bithenge_transform_ops *ops;
	unsigned int refs;
} bithenge_transform_t;

/** Operations that may be provided by a transform. */
typedef struct bithenge_transform_ops {
	/** @copydoc bithenge_transform_t::bithenge_transform_apply */
	int (*apply)(bithenge_transform_t *self, bithenge_node_t *in, bithenge_node_t **out);
	/** @copydoc bithenge_transform_t::bithenge_transform_prefix_length */
	int (*prefix_length)(bithenge_transform_t *self, bithenge_blob_t *blob, aoff64_t *out);
	/** Destroy the transform.
	 * @param self The transform. */
	void (*destroy)(bithenge_transform_t *self);
} bithenge_transform_ops_t;

/** Apply a transform.
 * @memberof bithenge_transform_t
 * @param self The transform.
 * @param in The input tree.
 * @param[out] out Where the output tree will be stored.
 * @return EOK on success or an error code from errno.h. */
static inline int bithenge_transform_apply(bithenge_transform_t *self,
    bithenge_node_t *in, bithenge_node_t **out)
{
	assert(self);
	assert(self->ops);
	return self->ops->apply(self, in, out);
}

/** Find the length of the prefix of a blob this transform can use as input. In
 * other words, figure out how many bytes this transform will use up.  This
 * method is optional and can return an error, but it must succeed for struct
 * subtransforms.
 * @memberof bithenge_transform_t
 * @param self The transform.
 * @param blob The blob.
 * @param[out] out Where the prefix length will be stored.
 * @return EOK on success, ENOTSUP if not supported, or another error code from
 * errno.h. */
static inline int bithenge_transform_prefix_length(bithenge_transform_t *self,
    bithenge_blob_t *blob, aoff64_t *out)
{
	assert(self);
	assert(self->ops);
	if (!self->ops->prefix_length)
		return ENOTSUP;
	return self->ops->prefix_length(self, blob, out);
}

/** Increment a transform's reference count.
 * @param self The transform to reference. */
static inline void bithenge_transform_inc_ref(bithenge_transform_t *self)
{
	assert(self);
	self->refs++;
}

/** Decrement a transform's reference count and free it if appropriate.
 * @param self The transform to dereference, or NULL. */
static inline void bithenge_transform_dec_ref(bithenge_transform_t *self)
{
	if (!self)
		return;
	assert(self->ops);
	if (--self->refs == 0)
		self->ops->destroy(self);
}

/** A transform with a name. */
typedef struct {
	const char *name;
	bithenge_transform_t *transform;
} bithenge_named_transform_t;

extern bithenge_transform_t bithenge_uint32le_transform;
extern bithenge_transform_t bithenge_uint32be_transform;
extern bithenge_named_transform_t *bithenge_primitive_transforms;

int bithenge_init_transform(bithenge_transform_t *self,
    const bithenge_transform_ops_t *ops);

int bithenge_new_struct(bithenge_transform_t **out,
    bithenge_named_transform_t *subtransforms);

#endif

/** @}
 */

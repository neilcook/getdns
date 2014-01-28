/**
 *
 * /file
 * /brief getdns context management functions
 *
 * Originally taken from the getdns API description pseudo implementation.
 *
 */

/*
 * Copyright (c) 2013, Versign, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * * Neither the name of the <organization> nor the
 *   names of its contributors may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Verisign, Inc. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _GETDNS_CONTEXT_H_
#define _GETDNS_CONTEXT_H_

#include <getdns/getdns.h>
#include "types-internal.h"

struct event_base;
struct getdns_dns_req;
struct ldns_rbtree_t;
struct ub_ctx;

/** function pointer typedefs */
typedef void (*getdns_update_callback) (struct getdns_context *, uint16_t);

struct getdns_context {

	/* Context values */
	uint16_t resolution_type;
	uint16_t *namespaces;
	uint16_t timeout;
	uint16_t follow_redirects;
	struct getdns_list *dns_root_servers;
	uint16_t append_name;
	struct getdns_list *suffix;
	struct getdns_list *dnssec_trust_anchors;
	struct getdns_list *upstream_list;

	uint8_t edns_extended_rcode;
	uint8_t edns_version;
	uint8_t edns_do_bit;

	getdns_update_callback update_callback;

	struct mem_funcs mf;
	struct mem_funcs my_mf;

	/* Event loop for sync requests */
	struct event_base *event_base_sync;
	/* Event loop for async requests */
	struct event_base *event_base_async;

	/* The underlying unbound contexts that do
	 * the real work */
	struct ub_ctx *unbound_sync;
	struct ub_ctx *unbound_async;

	/* which resolution type the contexts are configured for
	 * 0 means nothing set
	 */
	uint8_t resolution_type_set;

	/*
	 * outbound requests -> transaction to getdns_dns_req
	 */
	struct ldns_rbtree_t *outbound_requests;
};

/** internal functions **/
/**
 * Sets up the unbound contexts with stub or recursive behavior
 * if needed.
 */
getdns_return_t getdns_context_prepare_for_resolution(struct getdns_context *context);

/* track an outbound request */
getdns_return_t getdns_context_track_outbound_request(struct getdns_dns_req
    *req);
/* clear the outbound request from being tracked - does not cancel it */
getdns_return_t getdns_context_clear_outbound_request(struct getdns_dns_req
    *req);
/* cancel callback internal - flag to indicate if req should be freed and callback fired */
getdns_return_t getdns_context_cancel_request(struct getdns_context *context,
    getdns_transaction_t transaction_id, int fire_callback);

char *getdns_strdup(const struct mem_funcs *mfs, const char *str);

struct getdns_bindata *getdns_bindata_copy(
    struct mem_funcs *mfs,
    const struct getdns_bindata *src);

void getdns_bindata_destroy(
    struct mem_funcs *mfs,
    struct getdns_bindata *bindata);

#endif /* _GETDNS_CONTEXT_H_ */

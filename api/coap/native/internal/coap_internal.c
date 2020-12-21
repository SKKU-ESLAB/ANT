/* Copyright (c) 2017-2020 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <coap2/coap.h>

#include "coap_internal.h"

bool gIsInitialized = false;

int resolve_address(const char *host, const char *service,
                    coap_address_t *dst) {
  struct addrinfo *res, *ainfo;
  struct addrinfo hints;
  int error, len = -1;

  memset(&hints, 0, sizeof(hints));
  memset(dst, 0, sizeof(*dst));
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_family = AF_UNSPEC;

  error = getaddrinfo(host, service, &hints, &res);

  if (error != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
    return error;
  }

  for (ainfo = res; ainfo != NULL; ainfo = ainfo->ai_next) {
    switch (ainfo->ai_family) {
    case AF_INET6:
    case AF_INET:
      len = dst->size = ainfo->ai_addrlen;
      memcpy(&dst->addr.sin6, ainfo->ai_addr, dst->size);
      goto finish;
    default:
      break;
    }
  }

finish:
  freeaddrinfo(res);
  return len;
}

void coap_request_end(void) {
  coap_context_t *ctx = nullptr;
  coap_session_t *session = nullptr;
  coap_address_t dst;
  coap_pdu_t *pdu = nullptr;
  int result = EXIT_FAILURE;

  coap_startup();

  /* resolve destination address where server should be sent */
  if (resolve_address("coap.me", "5683", &dst) < 0) {
    coap_log(LOG_CRIT, "failed to resolve address\n");
    goto finish;
  }

  /* create CoAP context and a client session */
  ctx = coap_new_context(nullptr);

  if (!ctx || !(session = coap_new_client_session(ctx, nullptr, &dst,
                                                  COAP_PROTO_UDP))) {
    coap_log(LOG_EMERG, "cannot create client session\n");
    goto finish;
  }

  /* coap_register_response_handler(ctx, response_handler); */
  coap_register_response_handler(
      ctx, [](auto, auto, auto, coap_pdu_t *received, auto) {
        coap_show_pdu(LOG_INFO, received);
      });
  /* construct CoAP message */
  pdu = coap_pdu_init(COAP_MESSAGE_CON, COAP_REQUEST_GET, 0 /* message id */,
                      coap_session_max_pdu_size(session));
  if (!pdu) {
    coap_log(LOG_EMERG, "cannot create PDU\n");
    goto finish;
  }

  /* add a Uri-Path option */
  coap_add_option(pdu, COAP_OPTION_URI_PATH, 5,
                  reinterpret_cast<const uint8_t *>("hello"));

  /* and send the PDU */
  coap_send(session, pdu);

  coap_run_once(ctx, 0);

  result = EXIT_SUCCESS;
finish:
  coap_session_release(session);
  coap_free_context(ctx);
  coap_cleanup();
}

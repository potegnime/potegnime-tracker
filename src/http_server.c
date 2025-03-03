#include "http_server.h"

#include "logger.h"


#include <string.h>
#include <signal.h>

#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/http.h>


static void request_handler(struct evhttp_request *req, void *ctx);

static void
signal_cb(evutil_socket_t fd, short event, void *arg)
{
        LOG_INFO("%s signal received\n", strsignal(fd));
        event_base_loopbreak(arg);
}



void http_server_init() {
    ev_uint16_t http_port = 80;
    const char *http_addr = "0.0.0.0";
    struct event_base* base;
    struct evhttp* http_server;
    struct event* sig_int;

    base = event_base_new();

    http_server = evhttp_new(base);
    evhttp_bind_socket(http_server, http_addr, http_port);
    evhttp_set_gencb(http_server, request_handler, NULL);

    sig_int = evsignal_new(base, SIGINT, signal_cb, base);
    event_add(sig_int, NULL);

    LOG_INFO("Listening on http://%s:%d", http_addr, http_port);

    event_base_dispatch(base);
    evhttp_free(http_server);
    event_base_free(base);

}   


static void request_handler(struct evhttp_request *req, void *ctx) {

    struct evbuffer *reply = NULL;
    struct evhttp_uri *decoded = NULL;

    enum evhttp_cmd_type cmd = evhttp_request_get_command(req);
    if (cmd != EVHTTP_REQ_GET)
        return;

    decoded = evhttp_uri_parse(evhttp_request_get_uri(req));
    if (!decoded) {
        evhttp_send_error(req, HTTP_BADREQUEST, NULL);
        return;
    }


    const char *path = evhttp_uri_get_path(decoded);
    
    reply = evbuffer_new();
    
    evbuffer_add_printf(reply, path);
    evhttp_send_reply(req, HTTP_OK, NULL, reply);
    evbuffer_free(reply);
}


#include <af_common.h>

static fishsoup_client_t* client = NULL;

static void net_return(MwWidget handle, void* user, void* client) {
	scene_change(AF_SCENE_MAIN);
}

void net_connect(const char* username, const char* hostname, int port) {
	scene_change(AF_SCENE_CONNECTING);
	client = fishsoup_client(hostname, port == 0 ? AF_DEFAULT_PORT : port);
	if(client == NULL) {
		MwAddUserHandler(ui_message("Failed to connect"), MwNactivateHandler, net_return, NULL);
		return;
	}
}

void net_poll(void) {
	int ret;
	if(client == NULL) return;

	while((ret = fishsoup_client_poll(client)) > 0);
	if(ret < 0) {
		MwAddUserHandler(ui_message("Connection destroyed"), MwNactivateHandler, net_return, NULL);
		fishsoup_client_destroy(client);
		client = NULL;
		return;
	}
}

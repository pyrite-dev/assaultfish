#include <af_common.h>

int main(int argc, char** argv) {
	fishsoup_server_t* server;
	int		   port = 0;

	server = fishsoup_server(port == 0 ? AF_DEFAULT_PORT : port);

	fishsoup_server_loop(server);
}

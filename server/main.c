#include <af_common.h>

#include <signal.h>

int main(int argc, char** argv) {
	int port = 0;

	signal(SIGPIPE, SIG_IGN);

	net_start(port);

	net_loop();
}

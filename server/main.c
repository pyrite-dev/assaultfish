#include <af_common.h>

int main(int argc, char** argv) {
	int port = 0;

	net_start(port);

	net_loop();
}

#include <GearBox/Foundation.h>

int main(int argc, char** argv){
	GBEngineParam param;
	GBEngine engine;

	param.server = 0;
	param.client = 1;

	GBInit();

	engine = GBEngineCreate(&param);
	GBEngineLoop(engine);
}

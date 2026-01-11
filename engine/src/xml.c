#include <GearSrc/XML.h>

#include <GearSrc/File.h>

typedef struct arg {
	GSEngine    engine;
	const char* path;
} arg_t;

static int driver_open(xemil_t* handle) {
	arg_t* arg = handle->drv_arg;

	if((handle->drv_opaque = GSFileOpen(arg->engine, arg->path)) == NULL) return 0;

	return 1;
}

static int driver_read(xemil_t* handle, void* data, int size) {
	return GSFileRead(handle->drv_opaque, data, size);
}

static void driver_close(xemil_t* handle) {
	GSFileClose(handle->drv_opaque);
}

static xl_driver_t driver_rec = {
    driver_open,
    driver_read,
    driver_close};
static xl_driver_t* driver = &driver_rec;

GSXML GSXMLOpen(GSEngine engine, const char* path) {
	arg_t	 arg;
	xemil_t* xl;

	arg.engine = engine;
	arg.path   = path;

	xl = xl_open(driver, &arg);
	if(xl == NULL) return NULL;
	if(!xl_parse(xl)) {
		xl_close(xl);

		return NULL;
	}

	return xl;
}

void GSXMLClose(GSXML xml) {
	xl_close(xml);
}

#include <GearBox/Resource.h>

#include <GearBox/File.h>

GBResource GBResourceOpen(GBEngine engine, const char* path) {
	GBResource resource = malloc(sizeof(*resource));

	memset(resource, 0, sizeof(*resource));

	if((resource->file = GBFileOpen(engine, path)) == NULL) {
		GBResourceClose(resource);
		return NULL;
	}

	return resource;
}

void GBResourceClose(GBResource resource) {
	if(resource->file != NULL) GBFileClose(resource->file);

	free(resource);
}

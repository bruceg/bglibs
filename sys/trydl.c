#include <dlfcn.h>

int main(void) {
	void* handle;
	handle = dlopen("filename", 0);
	dlsym(handle, "symbol");
	dlclose(handle);
	dlerror();
	return 0;
}

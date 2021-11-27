
/* I use this test program to test that the dylib can be loaded
   because I do not own a mac, only a VMware.
   and I can't test GM stuff there sadly :(
 
 */

#include <dlfcn.h>
#include <stdio.h>

typedef double(*np_initdll_t)(void);
np_initdll_t np_initdll;

int main() {
	void* lib;
	double retv;
	retv = 0.0;

	lib = dlopen("libnekopresenceMac.dylib", RTLD_GLOBAL | RTLD_NOW);
	if (!lib) {
		printf("Load library fail.\n");
		return 1;
	}
	
	np_initdll = (np_initdll_t)dlsym(lib, "np_initdll");
	if (!np_initdll) {
		printf("Resolve lib fail.\n");
		return 1;
	}

	retv = np_initdll();
	printf("retv = %f\n", retv);	
	
	dlclose(lib);
	lib = NULL;

	printf("bye\n");
	return 0;
}

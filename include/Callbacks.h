#include <cstdio>

static void glfw_Error_Callback(int error, const char* description) {

	fprintf(stderr, "Error is \n", description);
	throw("glfw_Error");
}
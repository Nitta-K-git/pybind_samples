#define PY_SSIZE_T_CLEAN
#include <Python.h> // compile only release mode

// https://docs.python.org/ja/3/extending/embedding.html#very-high-level-embedding

int main(int argc, char *argv[])
{
	(void)argc;
	wchar_t *program = Py_DecodeLocale(argv[0], nullptr);
	if (program == nullptr) {
		fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
		exit(1);
	}
	Py_SetProgramName(program);  /* optional but recommended */
	Py_Initialize();
	PyRun_SimpleString("from time import time,ctime\n"
					   "print('Today is', ctime(time()))\n");
	Py_Finalize(); // fix
	PyMem_RawFree(program);
	return 0;
}

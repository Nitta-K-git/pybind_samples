// https://qiita.com/benikabocha/items/5ec6b471de0d10aaf2df

#include <pybind11/eval.h>
#include <pybind11/pybind11.h>

#include <iostream>

namespace py = pybind11;

int main(int argc, char** argv) {
	wchar_t* program = Py_DecodeLocale(argv[0], nullptr);
	if (program == nullptr) {
		fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
		exit(1);
	}
	Py_SetProgramName(program);
	Py_Initialize();
	
	try {
		auto global = py::dict(py::module::import("__main__").attr("__dict__"));
		auto local = py::dict();
		// ここにコードを記述していく
	} catch (py::error_already_set& e) {
		std::cout << "Python error.\n" << e.what() << "\n";
	}
	
	Py_Finalize();
	PyMem_RawFree(program);
	
	return 0;
}

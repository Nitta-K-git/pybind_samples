// https://pybind11.readthedocs.io/en/stable/advanced/pycpp/utilities.html#evaluating-python-expressions-from-strings-and-files

#include <pybind11/eval.h>
#include <pybind11/pybind11.h>

#include <iostream>

namespace py = pybind11;

static int g_cnt;

int add(int a, int b){
	g_cnt++;
	return a+b;
}

int get_cnt(){
	return g_cnt;
}

// モジュールとしてpythonに関数を公開
PyMODINIT_FUNC PyInit_sample() {
	py::module m("sample", "pybind11 module sample."); // define module
	m.def("add", &add, "");
	m.def("get", &get_cnt, ""); // pythonからC++の変数を操作できる
	
	return m.ptr();
}


int main(int argc, char** argv) {
	(void)argc;
	wchar_t* program = Py_DecodeLocale(argv[0], nullptr);
	if (program == nullptr) {
		fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
		exit(1);
	}
	PyImport_AppendInittab("sample", PyInit_sample); // append module before Py_Initialize
	Py_SetProgramName(program);
	Py_Initialize();
	
	py::module::import("sample");
	try {
		auto global = py::dict(py::module::import("__main__").attr("__dict__"));
		auto local = py::dict();
		// ここにコードを記述していく

		// Evaluate in scope of main module
//		py::object scope = py::module::import("__main__").attr("__dict__");
		
		// Evaluate an isolated expression
		global["my_variable"] = 3;
		int result = py::eval("my_variable + 10", global).cast<int>();
		
		// Evaluate a sequence of statements
		py::exec(
			"print('Hello')\n"
			"print('world!');",
			global);
		
		// Evaluate the statements in an separate Python file on disk
		py::eval_file("script.py", global);	
	
	} catch (py::error_already_set& e) {
		std::cout << "Python error.\n" << e.what() << "\n";
	}
	
	Py_Finalize();
	PyMem_RawFree(program);
	
	return 0;
}




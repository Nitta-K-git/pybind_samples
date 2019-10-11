// https://qiita.com/benikabocha/items/5ec6b471de0d10aaf2df

// cppからpythonへEigenの配列を参照渡しするサンプル

/*
#include <pybind11/embed.h>

#include <pybind11/eval.h>
#include <pybind11/pybind11.h>
//#include <pybind11/numpy.h>
//#include <pybind11/eigen.h>

//#include <Eigen/Dense>

#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
PYBIND11_MAKE_OPAQUE(std::vector<int>);

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
//	py::scoped_interpreter guard{};
	
	try {
		auto global = py::dict(py::module::import("__main__").attr("__dict__"));
		auto local = py::dict();
		// ここにコードを記述していく
		py::exec(
					"import numpy as np\n"
					"arr = np.array([1,2,])\n"
					"print(arr)\n",
					global);
		py::exec(
					"def tes(a):\n"
					"	print(1)\n",
					global);
		auto func_tes = global["tes"];
		std::vector<int> a;
		a.push_back(1);
		a.push_back(3);
//		func_tes(py::cast(&a));
		
	} catch (py::error_already_set& e) {
		std::cout << "Python error.\n" << e.what() << "\n";
	}
	
	Py_Finalize();
	PyMem_RawFree(program);

	return 0;
}

*/

// https://pybind11.readthedocs.io/en/stable/advanced/embedding.html
// https://github.com/pybind/pybind11/issues/1508

#include <pybind11/embed.h> // everything needed for embedding
namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(std::vector<int>);

int main() {
	py::scoped_interpreter guard{}; // start the interpreter and keep it alive
	
	py::print("Hello, World!"); // use the Python API
	
	py::exec(R"(
        kwargs = dict(name="World", number=42)
        message = "Hello, {name}! The answer is {number}".format(**kwargs)
        print(message)
    )");
	py::exec(R"(
			 import numpy as np
			 arr = np.array([1,2])
			 print(arr)
			 )");
	py::module m;
	m = py::module::import("stl");
	auto addFunc = m.attr("add");
	
	std::vector<int> v;
	v.push_back(3);
	v.push_back(6);
	
	addFunc(&v);
	
}

// https://qiita.com/benikabocha/items/5ec6b471de0d10aaf2df

// cppからpythonへEigenの配列を参照渡しするサンプル

#include <pybind11/eval.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/eigen.h>

#include <Eigen/Dense>

#include <iostream>

namespace py = pybind11;

template <typename T>
using RMatrix = Eigen::Matrix<T, -1, -1, Eigen::RowMajor>;

int main(int argc, char** argv) {
	RMatrix<double> m(2,2);
	m << 2,3,4,6;
	
//	py::EigenDRef<Eigen::MatrixXd> k();
	
	
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
		Eigen::Ref<RMatrix<double>> mm = m;
		global["val"] = mm;
		py::eval_file("script.py", global);	
		
	} catch (py::error_already_set& e) {
		std::cout << "Python error.\n" << e.what() << "\n";
	}
	
	Py_Finalize();
	PyMem_RawFree(program);

	std::cout << m << std::endl;
	
	return 0;
}

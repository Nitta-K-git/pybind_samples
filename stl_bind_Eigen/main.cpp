#include <pybind11/embed.h> // everything needed for embedding
#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/eigen.h>

#include <Eigen/Eigen>
#include <Eigen/Cholesky>

#include <iostream>
namespace py = pybind11;

using MatrixXdR = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;


PYBIND11_MAKE_OPAQUE(std::vector<int>);
PYBIND11_MAKE_OPAQUE(std::vector<MatrixXdR>);
PYBIND11_MAKE_OPAQUE(std::vector<Eigen::Vector3d>);

PYBIND11_EMBEDDED_MODULE(_cc11binds, m) {
	m.doc() = "C++ type bindings created by py11bind";
	py::bind_vector<std::vector<int>>(m, "IntVector", py::module_local(false));
}

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
	py::module::import("_cc11binds");
	m = py::module::import("stl");
	auto addFunc = m.attr("add");
	
	std::vector<int> v;
	v.push_back(3);
	v.push_back(6);
	
	std::cout << "before" << std::endl;
	for(auto &&val:v){
		std::cout << val << std::endl;
	}
	
	addFunc(&v);
	
	std::cout << "after" << std::endl;
	for(auto &&val:v){
		std::cout << val << std::endl;
	}
	
}

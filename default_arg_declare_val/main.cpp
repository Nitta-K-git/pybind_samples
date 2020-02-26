#include<pybind11/pybind11.h>
namespace py = pybind11;
int add(int i, int j) {
	return i + j;
}
int add_def(int i = 1, int j = 2) {
	return i + j;
}
PYBIND11_MODULE(default_arg_declare_val, m) {
	m.doc() = "pybind11 example plugin";
	m.def("add", &add, "A function which adds tow numbers");
	m.def("add1", &add, "A function which adds two numbers", py::arg("iii"), py::arg("j")); // i=0,j=1のような形で指定する
	using namespace pybind11::literals;
	m.def("add2", &add, "i"_a, "j"_a); // 上の表記の簡略化版
	m.def("add_def", &add_def, "A function which adds two numbers", py::arg("i") = 1, py::arg("j") = 2); // デフォルト引数
	m.def("add_def1", &add_def, py::arg("i") = 1, py::arg("j") = 2); // regular notation
	m.def("add_def2", &add_def, "i"_a = 1, "j"_a = 2); // shorthand
	
	//変数の公開
	m.attr("the_answer") = 42;
	py::object world = py::cast("World");
	m.attr("what") = world;
	
	
}

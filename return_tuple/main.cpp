#include<pybind11/pybind11.h>
#include<pybind11/stl.h> // vector用
#include<pybind11/numpy.h>
#include<algorithm>
#include <iostream>
namespace py = pybind11;

PYBIND11_MODULE(QT_TARGET, m) {
	// ラムダで記述
	m.def("get_tuple", []() { //
		std::vector<ssize_t> shape{ 2,3 }; //2行3列の形状
		py::array_t<double> y{shape};
		*y.mutable_data(0, 1) = 8.5; //0行1列目だけ設定してみる。初期値はゴミデータらしい
		std::vector<int> l{ 3, 5, 10, 20 };
		std::set<std::string> s{ "A","OK","123" };
		std::tuple<py::array_t<double>, std::vector<int>, std::set<std::string>> t = std::make_tuple(y, l, s);
		return t;
	});
}

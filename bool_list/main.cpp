#include<pybind11/pybind11.h>
#include<pybind11/stl.h> // vector用
#include<pybind11/numpy.h>
#include<algorithm>
#include <iostream>
#include<deque>
namespace py = pybind11;

PYBIND11_MODULE(QT_TARGET, m) {
	m.def("vector_int", [](std::vector<int> arr) { // bool値をvector<int>で拾う→1,0になる(charは不可)
		std::stringstream ss;
		for (auto a : arr) {
			ss << a << " ";
		}
		py::print(ss.str());
		std::vector<int> v{ true, false, true, true, false };
		return v;
	});
	m.def("deque_bool", [](std::deque<bool> arr) { // これはエラーになる
		std::stringstream ss;
		for (auto a : arr) {
			ss << a << " ";
		}
		py::print(ss.str());
		std::deque<bool> v{ true, false, true, true, false };
		return v;
	});
	m.def("vector_bool", [](std::vector<bool> arr) { // 
		std::stringstream ss;
		for (auto a : arr) {
			ss << a << " ";
		}
		py::print(ss.str());
		std::vector<bool> v{ true, false, true, true, false };
		v[1] = true;
		return v;
	});
}

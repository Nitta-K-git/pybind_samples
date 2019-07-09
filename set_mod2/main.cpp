#include<pybind11/pybind11.h>
#include<pybind11/stl.h> // vector用
#include<algorithm>
#include <iostream>
namespace py = pybind11;

PYBIND11_MODULE(QT_TARGET, m) {
    m.def("get_std_set", []() {
        std::set<int> s{ 1,5,6,1 };
        return s;
    });
}

#include<pybind11/pybind11.h>
#include<pybind11/stl.h> // vector用
#include<algorithm>
#include <iostream>
namespace py = pybind11;

PYBIND11_MODULE(QT_TARGET, m) {
    // ラムダで記述
    m.def("get_set", []() { // pythonのset型を作成して返す
        py::set set;
        set.add(py::str("key1"));
        set.add("key2");
        set.add(std::string("key3"));
        return set;
    });
    m.def("print_set", [](py::set set) { // setデータをprint
        for (auto item : set)
            py::print("key:", item);
    });
    m.def("cast_set", []() { return std::set<std::string>{"key1", "key2"}; });
    m.def("load_set", [](const std::set<std::string> &set) {
        return set.count("key1") && set.count("key2") && set.count("key3");
    });
}

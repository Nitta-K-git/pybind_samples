#include <pybind11/pybind11.h>
namespace py = pybind11;

// int add(int i=1, int j=2) {
//     return i + j*100;
// }
// PYBIND11_MODULE(MY_MODULE_NAME, m) {
//     m.doc() = "pybind11 example plugin"; // optional module docstring
//     m.def("add", &add, py::arg("ii")=10, py::arg("jj")=5, "A function which adds two numbers");
// }

// int add(int i, int j, int k) {
//     return i + j*100 + k*200;
// }
// PYBIND11_MODULE(MY_MODULE_NAME, m) {
//     m.doc() = "pybind11 example plugin"; // optional module docstring
//     m.def("add", &add, "A function which adds two numbers", py::arg("aa"), py::arg("aa"), py::arg("kk")=1);
// }


struct Pet {
    Pet(const std::string &name) : name(name) { }
    void setName(const std::string &name_) { name = name_; }
    const std::string &getName() const { return name; }

    std::string name;
};

PYBIND11_MODULE(MY_MODULE_NAME, m) {
    py::class_<Pet>(m, "Pet")
        .def(py::init<const std::string &>())
        .def("setName", &Pet::setName, py::arg("name")="default")
        .def("getName", &Pet::getName)
        .def("test", [](Pet &a, int b, int c){
            return 10*b+100*c;
        },py::arg("aa"), py::arg("bb")=5)
        ;
}
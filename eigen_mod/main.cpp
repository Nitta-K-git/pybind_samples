#include <iostream>
#include <pybind11/eigen.h>
#include <Eigen/Dense>
namespace py = pybind11;

template <typename T>
using RMatrix = Eigen::Matrix<T, -1, -1, Eigen::RowMajor>;

template <typename T>
void print_array(RMatrix<T> m){
    py::print(m);
}
template <typename T>
RMatrix<T> modify_array(RMatrix<T> m, T a){
    return m * a;
}
template <typename T>
void modify_array_inplace(Eigen::Ref<RMatrix<T>> m, T a){ //中身参照
    m = m * a;
}
PYBIND11_MODULE(QT_TARGET, m){
    m.def("print_array", &print_array<double>, "");
    m.def("modify_array", &modify_array<double>, "");
    m.def("modify_array_inplace", &modify_array_inplace<double>, "");
}

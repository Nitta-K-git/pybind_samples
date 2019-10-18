// https://qiita.com/lucidfrontier45/items/5048ef74fbf32eeb9f08

#include <iostream>
#include <pybind11/eigen.h>
#include <Eigen/Core>
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
template<typename T>
void modify_new_array(Eigen::Ref<RMatrix<T>> m){
    m = RMatrix<T>::Ones(3,10); // not work
	
}
template<typename T>
RMatrix<T> get_new_array(){
    RMatrix<T> m = RMatrix<T>::Ones(10,3);
	for(int i=0; i<m.rows(); ++i){
		for(int j=0; j<m.cols(); ++j){
			std::cout << m(i,j) << std::endl;
		}
	}
	RMatrix<T> mm(1,3);
	mm << 100,200,300;
	m.block(0,0,1,3) = mm;
	return m;
}

PYBIND11_MODULE(MY_MODULE_NAME, m){
    m.def("print_array", &print_array<double>, "");
    m.def("modify_array", &modify_array<double>, "");
    m.def("modify_array_inplace", &modify_array_inplace<double>, "");
	m.def("modify_new_array", &modify_new_array<double>, "");
	m.def("get_new_array", &get_new_array<double>, "");
}

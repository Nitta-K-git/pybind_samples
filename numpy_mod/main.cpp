#include<pybind11/pybind11.h>
#include<pybind11/numpy.h>
#include<algorithm>
#include <iostream>
namespace py = pybind11;

//numpyの配列を参照するだけの関数(2次元配列限定)
template <typename T>
void print_array(py::array_t<T> x){
    const auto &buff_info = x.request(); //requestメソッドで以下の構造体を得られる
    /*
        struct buffer_info {
            void *ptr;
            ssize_t itemsize;
            std::string format;
            ssize_t ndim;
            std::vector<ssize_t> shape;
            std::vector<ssize_t> strides;
        };
    */
    const auto &shape = buff_info.shape; // numpyのshapeと同じ
    for (auto i = 0; i < shape[0]; i++){ // 普通に成分の数だけ回す
        for (auto j = 0; j < shape[1]; j++){
            auto v = *x.data(i, j); // x.dataで読み取り専用のconstポインタが得られる。
            //std::cout << "i:" << i << "j:" << j << "v:" << v << std::endl;
            py::print(v); // jupyterとかでprintする場合はこっち
        }
    }
}
//numpyの配列を書き換える関数
template <typename T>
void modify_array_inplace(py::array_t<T> x, T a){
    const auto &buff_info = x.request(); // numpyのデータ構造体取得
    const auto &shape = buff_info.shape;
    for (auto i = 0; i < shape[0]; i++){
        for (auto j = 0; j < shape[1]; j++){
            *x.mutable_data(i, j) += a; // x.mutable_dataでconstでない書き換え可能なポインタが得られる
        }
    }
}
// numpyの配列を新しく作って返す
template <typename T>
auto modify_array(py::array_t<T> x, T a){
    const auto &buff_info = x.request();
    const auto &shape = buff_info.shape;
    py::array_t<T> y{shape}; // 新しい配列作成。shapeをコンストラクタに渡す方法。同じ形になる
    for (auto i = 0; i < shape[0]; i++){
        for (auto j = 0; j < shape[1]; j++){
            *y.mutable_data(i, j) = *x.data(i, j) + a;
        }
    }
    return y;
}
PYBIND11_MODULE(numpy_mod, m) {
    m.def("print_array", &print_array<int32_t>, ""); // templateで作成した関数は型指定をして登録する
    m.def("print_array", &print_array<double>, "");
    m.def("modify_array_inplace", &modify_array_inplace<int32_t>, "");
    m.def("modify_array_inplace", &modify_array_inplace<double>, "");
    m.def("modify_array", &modify_array<int32_t>, "");
    m.def("modify_array", &modify_array<double>, "");
}

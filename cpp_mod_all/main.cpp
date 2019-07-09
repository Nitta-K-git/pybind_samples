#include<pybind11/pybind11.h>
#include<pybind11/stl.h> // vector用
#include<pybind11/numpy.h>
#include<algorithm>
#include <iostream>
namespace py = pybind11;

int add(int a, int b){
	return a + b;
}

std::vector<int> vec_double(std::vector<int> &vec) { // リストを引数に取る。vector型で受け取れる
	for (auto &v : vec){
		v *= 2;
	}
	vec[1] = 1000; // 変数vecはコピーされたものなので、参照先は変わらないらしい
	return vec;
}
// 2次元リストの各行について、自身以前の総和を計算したものを入れる処理(各行の要素数は違ってよい)
std::vector<std::vector<int>> vec_add(std::vector<std::vector<int>> &vec) { // 2次元リストはベクタのベクタで受け取れる
	std::vector<std::vector<int>> result(vec.size(), std::vector<int>()); // 変数を新規作成 リストで返す
	for (int i = 0; i < vec.size(); i++){ // 全ての行について
		int tmp = 0;
		for (auto &t : vec[i]){ // ある行の全ての値について
			tmp += t;
			result[i].push_back(tmp); // 新規変数にpush
		}
	}
	return result;
}


PYBIND11_MODULE(minimum_sample, m) { // must same target file name
	m.doc() = "Pybind Document Sample";
	m.def("add", &add);

	m.def("vec_double", &vec_double);
	m.def("vec_add", &vec_add);

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



#include <pybind11/eigen.h>
#include <Eigen/Dense>
namespace py = pybind11;

template <typename T>
using RMatrix = Eigen::Matrix<T, -1, -1, Eigen::RowMajor>;

template <typename T>
void block_test(Eigen::Ref<RMatrix<T>> m, T a) //中身参照
{
	//プロパティ取得
	py::print(m.rows(), m.cols());
	//blockで4x4取り出して処理する
	py::print(m.block(0, 0, 4, 4));
	py::print(m.block(4, 0, 4, 4));
	
	Eigen::MatrixXd A = m.block(4, 0, 4, 4); // blockで取り出した行列はコピー
	py::print(A.rows(), A.cols());
	A = A * a; // ブロック元には影響しない
	py::print(A);
	m.block<4, 4>(2, 0) = Eigen::Matrix4d::Identity(); // blockに直接代入すると書き換えられる <>はサイズ固定する場合に使用
}
PYBIND11_MODULE(QT_TARGET, m)
{
	m.def("block_test", &block_test<double>, "");
}


int add(int i, int j) {
	return i + j;
}
int add_def(int i = 1, int j = 2) {
	return i + j;
}
PYBIND11_MODULE(default_arg_declare_val, m) {
	m.doc() = "pybind11 example plugin";
	m.def("add", &add, "A function which adds tow numbers");
	m.def("add1", &add, "A function which adds two numbers", py::arg("i"), py::arg("j")); // i=0,j=1のような形で指定する
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


PYBIND11_MODULE(QT_TARGET, m) {
    m.def("get_std_set", []() {
        std::set<int> s{ 1,5,6,1 };
        return s;
    });
}

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

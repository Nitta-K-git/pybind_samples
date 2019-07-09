#include<pybind11/pybind11.h>
#include<pybind11/numpy.h>
#include<pybind11/stl.h> // vector用
#include<algorithm>
#include <iostream>
namespace py = pybind11;

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
PYBIND11_MODULE(list_mod, m){
	m.def("vec_double", &vec_double);
	m.def("vec_add", &vec_add);
}

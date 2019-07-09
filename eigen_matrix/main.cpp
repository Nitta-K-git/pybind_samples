#include <iostream>
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

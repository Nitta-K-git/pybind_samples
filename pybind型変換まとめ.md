# pybind型変換まとめ

```cpp
/* C++ */
int add(int a, int b){
	return a + b;
}
```

```python
# python
a = 1
b = 2
c = m.add(a,b)
assert(c == a+b)
```

```cmake
QT -= core gui
CONFIG -= qt
QMAKE_EXTENSION_SHLIB = pyd # 拡張子をdllからpydに変更

# dll setting
TEMPLATE = lib
CONFIG += plugin

# pybind11のヘッダファイルとライブラリのパスを指定
EIGENDIR = C:/eigen-eigen-323c052e1731
PYBINDDIR = C:/Users/nitta/AppData/Local/Programs/Python/Python35/include
PYBINDLIB = C:/Users/nitta/AppData/Local/Programs/Python/Python35/libs/python35.lib

INCLUDEPATH  += $$EIGENDIR $$PYBINDDIR
LIBS += $$PYBINDLIB

win32{
    win32-msvc{
        QMAKE_CXXFLAGS += /utf-8 # visual studioはUTF-8用の指定が必要
    }
}
DEFINES += QT_TARGET=$$TARGET # モジュール名をdefineしとくとcppファイルでもそのまま使える

# ここはインポートするヘッダとソースを指定
# HEADERS +=
SOURCES += main.cpp

```

```cpp
#include <pybind11/pybind11.h>

int add(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("add", &add, "A function which adds two numbers");
}
```

```cpp
m.def("add", &add, "A function which adds two numbers", py::arg("i"), py::arg("j"));
```

```python
>>> import example
>>> example.add(i=1, j=2)
3
```

```cpp
// regular notation
m.def("add1", &add, py::arg("i"), py::arg("j"));
// shorthand
using namespace pybind11::literals;
m.def("add2", &add, "i"_a, "j"_a);
```

```cpp
m.def("add", &add, "A function which adds two numbers", py::arg("i") = 1, py::arg("j") = 2);
// regular notation
m.def("add1", &add, py::arg("i") = 1, py::arg("j") = 2);
// shorthand
using namespace pybind11::literals;
m.def("add2", &add, "i"_a=1, "j"_a=2);
```

```cpp
m.def("print_list", [](py::list list) {
    int index = 0;
    for (auto item : list)
        py::print("list item {}: {}"_s.format(index++, item));
});
```

```python
>>> m.print_list([1,3,4])
list item 0: 1
list item 1: 3
list item 2: 4
>>> m.print_list([[1,3,4],[2,3]])
list item 0: [1, 3, 4]
list item 1: [2, 3]
```

型コンストラクタ

```cpp
// test_constructors
m.def("default_constructors", []() {
    return py::dict(
        "str"_a=py::str(),
        "bool"_a=py::bool_(),
        "int"_a=py::int_(),
        "float"_a=py::float_(),
        "tuple"_a=py::tuple(),
        "list"_a=py::list(),
        "dict"_a=py::dict(),
        "set"_a=py::set()
    );
});
```

```python
>>> m.default_constructors()
{'dict': {}, 'tuple': (), 'list': [], 'set': set(), 'int': 0, 'str': '', 'float': 0.0, 'bool': False}
```

std型の変数

```cpp
m.def("cast_vector", []() { return std::vector<int>{1}; });
m.def("load_vector", [](const std::vector<int> &v) { return v.at(0) == 1 && v.at(1) == 2; });
```

```python
>>> m.cast_vector()
[1]
>>> m.load_vector([1,2])
True
>>> m.load_vector([1,3])
False
```

```cpp
m.def("vint2", [](std::vector<std::vector<int>> list){
    for(auto l:list){
        for(auto val:l){
            py::print(val);
        }
    }
});
```

```python
>>> m.vint2([[1,3,4], [2,4]])
1
3
4
2
4
```

```cpp
m.attr("the_answer") = 42;
py::object world = py::cast("World");
m.attr("what") = world;
std::vector<int> l{1,5,8};
m.attr("num_list") = py::cast(l);
py::module m_sub = m.def_submodule("subsubmodule");
m_sub.def("submodule_func", []() { return "submodule_func()"; });
```

```python
>>> m.the_answer
42
>>> m.what
'World'
>>> m.num_list
[1, 5, 8]
>>> m.subsubmodule
<module 'cpp_mod_all.subsubmodule'>
>>> m.subsubmodule.submodule_func()
'submodule_func()'
```

numpyとの連携

```cpp
#include<pybind11/pybind11.h>
#include<pybind11/numpy.h>
#include<algorithm>
#include <iostream>
namespace py = pybind11;

//numpyの配列を参照するだけの関数
template <typename T>
void print_array(py::array_t<T> x) {
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
	for (auto i = 0; i < shape[0]; i++) { // 普通に成分の数だけ回す
		for (auto j = 0; j < shape[1]; j++) {
			auto v = *x.data(i, j); // x.dataで読み取り専用のconstポインタが得られる。
			//std::cout << "i:" << i << "j:" << j << "v:" << v << std::endl;
			py::print(v);
		}
	}
}
//numpyの配列を書き換える関数
template <typename T>
void modify_array_inplace(py::array_t<T> x, T a) {
	const auto &buff_info = x.request(); // numpyのデータ構造体取得
	const auto &shape = buff_info.shape;
	for (auto i = 0; i < shape[0]; i++) {
		for (auto j = 0; j < shape[1]; j++) {
			*x.mutable_data(i, j) += a; // x.mutable_dataでconstでない書き換え可能なポインタが得られる
		}
	}
}
// numpyの配列を新しく作って返す
template <typename T>
auto modify_array(py::array_t<T> x, T a) {
	const auto &buff_info = x.request();
	const auto &shape = buff_info.shape;
	py::array_t<T> y{ shape }; // 新しい配列作成。shapeをコンストラクタに渡す方法。同じ形になる
	for (auto i = 0; i < shape[0]; i++) {
		for (auto j = 0; j < shape[1]; j++) {
			*y.mutable_data(i, j) = *x.data(i, j) + a;
		}
	}
	return y;
}
PYBIND11_MODULE(test006, m) {
	m.def("print_array", &print_array<int32_t>, ""); // templateで作成した関数は型指定をして登録する
	m.def("print_array", &print_array<double>, "");
	m.def("modify_array_inplace", &modify_array_inplace<int32_t>, "");
	m.def("modify_array_inplace", &modify_array_inplace<double>, "");
	m.def("modify_array", &modify_array<int32_t>, "");
	m.def("modify_array", &modify_array<double>, "");
}
```

```cpp
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
```
```cpp
m.def("modify_array_inplace", &modify_array_inplace<double>, "");
m.def("modify_array_inplace", &modify_array_inplace<int32_t>, "");
```

```python
>>> a = np.array([1,3,5,6]).reshape(2,2)
>>> print(a)
[[1 3]
 [5 6]]
>>> m.modify_array_inplace(a,4)
>>> print(a)
[[ 5  7]
 [ 9 10]]
```





基本的な使い方

```cpp
// チュートリアルに従ってやってみる https://pybind11.readthedocs.io/en/stable/basics.html

#include <pybind11/pybind11.h>
namespace py = pybind11;

int add(int i = 1, int j = 2) { //デフォルト引数も使えるが、pybind側でも同様の定義が必要(2回同じ定義をしないといけない)
    return i + j;
}

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    // m.def("add", &add, "A function which adds two numbers");
    m.def("add", &add, "A function which adds two numbers", py::arg("i"), py::arg("j")); // .add(i=1, j=2) の形で使える
    // regular notation
    m.def("add1", &add, py::arg("i"), py::arg("j"));
    // shorthand
    using namespace pybind11::literals;
    m.def("add2", &add, "i"_a, "j"_a); //省略記法
    m.def("add", &add, "A function which adds two numbers", py::arg("i") = 1, py::arg("j") = 2); //デフォルト引数の定義はC++側と両方で必要
    // regular notation
    m.def("add1", &add, py::arg("i") = 1, py::arg("j") = 2);
    // shorthand
    m.def("add2", &add, "i"_a=1, "j"_a=2); //省略記法が同様に使える
}

PYBIND11_MODULE(example, m) {
    m.attr("the_answer") = 42; // 変数の公開
    py::object world = py::cast("World"); // 型変換をしてからattrに設定する
    m.attr("what") = world;
}
//オブジェクト指向 ------------------------------------------------------------------------------------------------------------------------------------//
struct Pet {
    Pet(const std::string &name) : name(name) { } // コンストラクタ
    void setName(const std::string &name_) { name = name_; }
    const std::string &getName() const { return name; }

    std::string name;
};
PYBIND11_MODULE(example, m) {
    py::class_<Pet>(m, "Pet")
        .def(py::init<const std::string &>())
        .def("setName", &Pet::setName)
        .def("getName", &Pet::getName);
    py::class_::def_static();    // 静的メンバー関数の場合
}
py::class_<Pet>(m, "Pet")
    .def(py::init<const std::string &>())
    .def("setName", &Pet::setName)
    .def("getName", &Pet::getName)
    .def("__repr__",    //repr関数をラムダで実装する
        [](const Pet &a) {
            return "<example.Pet named '" + a.name + "'>";
        }
    );

py::class_<Pet>(m, "Pet")
    .def(py::init<const std::string &>())
    .def_readwrite("name", &Pet::name) // 読み書きできる変数として公開
    // ... remainder ...
py::class_<Pet>(m, "Pet")
    .def(py::init<const std::string &>())
    .def_property("name", &Pet::getName, &Pet::setName) // nameがprivateの場合、setterとgetterをこのように定義可能
    // ... remainder ...

// Dynamic attributes ------------------------------------------------------------------------------------------------------------------------------------//
py::class_<Pet>(m, "Pet", py::dynamic_attr()) //dynamic_attrを追加指定するとC++の公開したクラスにも新しいメンバを動的に追加できる(__dict__が追加される)
    .def(py::init<>())
    .def_readwrite("name", &Pet::name);

//継承とアップキャスト ------------------------------------------------------------------------------------------------------------------------------------//
struct Pet {
    Pet(const std::string &name) : name(name) { }
    std::string name;
};
struct Dog : Pet { // 継承で作成
    Dog(const std::string &name) : Pet(name) { }
    std::string bark() const { return "woof!"; }
};
py::class_<Pet>(m, "Pet")
   .def(py::init<const std::string &>())
   .def_readwrite("name", &Pet::name);
// Method 1: template parameter:
py::class_<Dog, Pet /* <- specify C++ parent type */>(m, "Dog") // Dogを定義するときclass_にPetを追加する
    .def(py::init<const std::string &>())
    .def("bark", &Dog::bark);
// Method 2: pass parent class_ object:
py::class_<Dog>(m, "Dog", pet /* <- specify Python parent type */) // 引数の中に追加。どちらの方法でも同等
    .def(py::init<const std::string &>())
    .def("bark", &Dog::bark);
// Return a base pointer to a derived instance
m.def("pet_store", []() { return std::unique_ptr<Pet>(new Dog("Molly")); }); // 親クラスにキャストして返す→Dog固有の関数は使えなくなる

// -------
struct PolymorphicPet {
    virtual ~PolymorphicPet() = default; //仮想関数定義
};
struct PolymorphicDog : PolymorphicPet {
    std::string bark() const { return "woof!"; } //継承
};
// Same binding code
py::class_<PolymorphicPet>(m, "PolymorphicPet");
py::class_<PolymorphicDog, PolymorphicPet>(m, "PolymorphicDog")
    .def(py::init<>())
    .def("bark", &PolymorphicDog::bark);
// Again, return a base pointer to a derived instance
m.def("pet_store2", []() { return std::unique_ptr<PolymorphicPet>(new PolymorphicDog); }); //親クラスが仮想関数を持つクラスにキャストをしても自動的に具象クラスに再キャストされるので注意

// オーバーロード ------------------------------------------------------------------------------------------------------------------------------------//
struct Pet {
    Pet(const std::string &name, int age) : name(name), age(age) { }

    void set(int age_) { age = age_; } // オーバーロード関数
    void set(const std::string &name_) { name = name_; }

    std::string name;
    int age;
};
py::class_<Pet>(m, "Pet")
   .def(py::init<const std::string &, int>())
   .def("set", (void (Pet::*)(int)) &Pet::set, "Set the pet's age") // オーバーロード関数はキャストをして定義しないとエラーになる
   .def("set", (void (Pet::*)(const std::string &)) &Pet::set, "Set the pet's name");
py::class_<Pet>(m, "Pet")
    .def("set", py::overload_cast<int>(&Pet::set), "Set the pet's age") // C++14対応のコンパイラならこっちの記法が使える
    .def("set", py::overload_cast<const std::string &>(&Pet::set), "Set the pet's name");

struct Widget {
    int foo(int x, float y);
    int foo(int x, float y) const;
};
py::class_<Widget>(m, "Widget")
   .def("foo_mutable", py::overload_cast<int, float>(&Widget::foo))
   .def("foo_const",   py::overload_cast<int, float>(&Widget::foo, py::const_)); // constの有無でオーバーロード分かれる場合は、このように指定

// enum ------------------------------------------------------------------------------------------------------------------------------------//
struct Pet {
    enum Kind {
        Dog = 0,
        Cat
    };
    Pet(const std::string &name, Kind type) : name(name), type(type) { }
    std::string name;
    Kind type;
};

py::class_<Pet> pet(m, "Pet");
pet.def(py::init<const std::string &, Pet::Kind>())
    .def_readwrite("name", &Pet::name)
    .def_readwrite("type", &Pet::type);
py::enum_<Pet::Kind>(pet, "Kind") // enumを使う場合は別に定義が必要
    .value("Dog", Pet::Kind::Dog)
    .value("Cat", Pet::Kind::Cat)
    .export_values();

// 返り値のポリシー設定 ------------------------------------------------------------------------------------------------------------------------------------//
/* Function declaration */
Data *get_data() { return _data; /* (pointer to a static data structure) */ }
/* Binding code */
m.def("get_data", &get_data); // <-- KABOOM, will cause crash when called from Python
    // デフォルトでは return_value_policy::automatic のポリシー → データの所有権をpython側も持つ
    // ガベージコレクタがpython側で動いたときに変数deleteしてしまい、エラー発生になる
m.def("get_data", &get_data, return_value_policy::reference); // 参照だけにしたいものは明示的に指定する

// ポリシー一覧 スマートポインタを使用するとC++とpythonどちらからでも参照状態が分かるため、ポリシーの設定が不要になるので推奨
return_value_policy::take_ownership // コピーを作成せずに所有権を取得。python側で不要になった時点でデストラクタ呼ぶ or delete してしまう
return_value_policy::copy // 新しいコピーを作成する
return_value_policy::move // 戻り値の内容をpython側に代入する(コピーとほぼ一緒？)。std::moveが使われる
return_value_policy::reference // python側は参照だけ。リソースの削除はC++側で行わないといけない
return_value_policy::reference_internal	 // クラスのメソッドで使用。オブジェクト自体が破棄されるまで有効
return_value_policy::automatic // デフォルト。ポインターならtake_ownership。値ならcopy or moveが適用される
return_value_policy::automatic_reference // C++からpythonの関数呼ぶときに使う？普通は使わないらしい

class_<MyClass>(m, "MyClass")
    .def_property("data", &MyClass::getData, &MyClass::setData, py::return_value_policy::copy); // Return value policies can also be applied to properties

class_<MyClass>(m, "MyClass")
    .def_property("data"
        py::cpp_function(&MyClass::getData, py::return_value_policy::copy), // ポリシーの対象となる引数はcpp_functionで指定できる
        py::cpp_function(&MyClass::setData)
    );

// Keep alive
py::class_<List>(m, "List")
    .def("append", &List::append, py::keep_alive<1, 2>()); // よく分からない

// pythonのデータ型を使う
void print_dict(py::dict dict) { // dict型を使う
    /* Easily interact with Python types */
    for (auto item : dict)
        std::cout << "key=" << std::string(py::str(item.first)) << ", "
                  << "value=" << std::string(py::str(item.second)) << std::endl;
}

//argsとkwargsを使用
def generic(*args, **kwargs):
    // ...  # do something with args and kwargs
void generic(py::args args, py::kwargs kwargs) { // 表記違い
    /// .. do something with args
    if (kwargs)
        /// .. do something with kwargs
}
/// Binding code
m.def("generic", &generic);
```

Eigen

```cpp
template <typename T>
using RMatrix = Eigen::Matrix<T, -1, -1, Eigen::RowMajor>;
template <typename T>
void modify_array_inplaceE(Eigen::Ref<RMatrix<T>> m, T a){
	m = m * a;
}
```

```cpp
m.def("modify_array_inplaceE", &modify_array_inplaceE<double>, "");
```

```python
>>> x = np.arange(12).reshape(3, 4).astype(np.float64)
>>> print(x)
[[ 0.  1.  2.  3.]
 [ 4.  5.  6.  7.]
 [ 8.  9. 10. 11.]]
>>> m.modify_array_inplaceE(x, 3.0)
>>> print(x)
[[ 0.  3.  6.  9.]
 [12. 15. 18. 21.]
 [24. 27. 30. 33.]]
```

Qt C++ call python

```cmake
QT -= core gui
TEMPLATE = app

# pybind11のヘッダファイルとライブラリのパスを指定
EIGENDIR = C:/eigen-eigen-323c052e1731
PYBINDDIR = C:/Users/nitta/AppData/Local/Programs/Python/Python35/include
PYBINDLIB = C:/Users/nitta/AppData/Local/Programs/Python/Python35/libs/python35.lib

INCLUDEPATH  += $$EIGENDIR $$PYBINDDIR
LIBS += $$PYBINDLIB

win32{
    win32-msvc{
        QMAKE_CXXFLAGS += /utf-8 # visual studioはUTF-8用の指定が必要
    }
}
SOURCES += main.cpp

```

```cpp
#include <pybind11/eval.h>
#include <pybind11/pybind11.h>
#include <iostream>

namespace py = pybind11;
int main(int argc, char** argv) {
	wchar_t* program = Py_DecodeLocale(argv[0], nullptr);
	if (program == nullptr) {
		fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
		exit(1);
	}
	Py_SetProgramName(program);
	Py_Initialize();
	
	try {
		auto global = py::dict(py::module::import("__main__").attr("__dict__"));
		auto local = py::dict();
		// ここにコードを記述していく
	} catch (py::error_already_set& e) {
		std::cout << "Python error.\n" << e.what() << "\n";
	}
	
	Py_Finalize();
	PyMem_RawFree(program);
	return 0;
}
```

```cpp
// 単一文の実行
py::eval<py::eval_single_statement>("from time import time,ctime", global, local);
py::eval<py::eval_single_statement>("print('Today is', ctime(time()))", global, local);
py::eval<py::eval_single_statement>("aaa = 100+200", global, local);
// 変数を定義する
local["x"] = 100; // localに新しく追加
global["g_x"] = 1000; // globalに追加
py::eval("print(x, g_x)", global, local);
py::eval<py::eval_single_statement>("y = 200", global, local); // pythonコードで定義、参照先のlocalに追加される
py::print(local["y"]);
py::eval<py::eval_single_statement>("yy = 250", global); // pythonコードで定義、global変数として追加される
py::print(global["yy"]);
py::eval("print('x + y =', x + y)", global, local);
// get variable
py::eval<py::eval_single_statement>("z = x + y", global, local);
auto z = local["z"].cast<int>(); // pythonのオブジェクトはcastしてC++の型に変換する
std::cout << "cpp: z = " << z << "\n"; // cpp: z = 300
// define and call function
py::eval<py::eval_statements>(
    "def func_01():\n"
    "   print('func_01: call')\n",
    global, local); // pythonコードで関数定義
auto func_01 = local["func_01"]; //pythonオブジェクトで取得
func_01(); // func_01: call そのまま()で呼び出せる
// pass value to func
py::eval<py::eval_statements>(
    "def func_02(a, b):\n"
    "   print('func_02: {} + {} = {}'.format(a, b, a + b))\n",
    global, local);
auto func_02 = local["func_02"];
func_02(123, 456);     // func_02: 123 + 456 = 579		()に値もそのまま渡せる
func_02("abc", "efg"); // func_02: abc + efg = abcdefg
// get func return value
py::eval<py::eval_statements>(
    "def func_03(a, b):\n"
    "   return a + b\n",
    global, local);
auto func_03 = local["func_03"];
auto result = func_03(123, 456); // 戻り値もそのまま取得できる
std::cout << "cpp: func_03 result "
    << py::str(result).cast<std::string>() << "\n"; // cpp: func_03 result 579		//pythonオブジェクトなのでcastして取得
		
py::object Decimal = py::module::import("decimal").attr("Decimal"); // C++で特定のライブラリ使いたいときは.attrで取得する
// Try to import scipy
py::object scipy = py::module::import("scipy");
py::object scipy_ver = scipy.attr("__version__");
py::print(py::str(scipy_ver));
// Construct a Python object of class Decimal
py::object pi = Decimal("3.14159"); // pi = decimal.Decimal("3.14159")
// Use Python to make our directories
py::object os = py::module::import("os");
py::object makedirs = os.attr("makedirs");
makedirs("./tmp/path/to/somewhere");
// Calculate e^π in decimal
py::object exp_pi = pi.attr("exp")(); // exp_pi = pi.exp()
py::print(py::str(exp_pi));
```

```cpp
// Evaluate a sequence of statements
py::exec(
    "print('Hello')\n"
    "print('world!');",
    global);
// Evaluate the statements in an separate Python file on disk
py::eval_file("script.py", global);	
```

```cpp
int add(int a, int b){
	g_cnt++;
	return a+b;
}
int get_cnt(){
	return g_cnt;
}
// モジュールとしてpythonに関数を公開
PyMODINIT_FUNC PyInit_sample() {
	py::module m("sample", "pybind11 module sample."); // define module
	m.def("add", &add, "");
	m.def("get", &get_cnt, ""); // pythonからC++の変数を操作できる
	return m.ptr();
}
int main(int argc, char** argv) {
	(void)argc;
	wchar_t* program = Py_DecodeLocale(argv[0], nullptr);
	if (program == nullptr) {
		fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
		exit(1);
	}
	PyImport_AppendInittab("sample", PyInit_sample); // append module before Py_Initialize
	Py_SetProgramName(program);
	Py_Initialize();
	
	py::module::import("sample");
	try {
		auto global = py::dict(py::module::import("__main__").attr("__dict__"));
		auto local = py::dict();
		py::eval_file("script.py", global);	
	} catch (py::error_already_set& e) {
		std::cout << "Python error.\n" << e.what() << "\n";
	}
	
	Py_Finalize();
	PyMem_RawFree(program);
	
	return 0;
}

```

```python
import sample
print("cnt",sample.get())
print(sample.add(3,5))
print("cnt",sample.get())
print(sample.add(103,5))
print("cnt",sample.get())
```

```python
ext_modules = [
    Extension(
        'mesh_regist_y', # library name
        ['src/main.cpp'], # reference file
        include_dirs=[
            # Path to pybind11 headers
            get_pybind_include(),
            get_pybind_include(user=True),
            find_eigen(['third_party/eigen'])
        ],
        language='c++'
    ),
    Extension(
        'my3d002', # library name
        ['src/my3d002.cpp'], # reference file
        include_dirs=[
            # Path to pybind11 headers
            get_pybind_include(),
            get_pybind_include(user=True),
            find_eigen(['third_party/eigen'])
        ],
        language='c++'
    ),

]
setup(
    name='mesh_regist_y', # library name
    version=__version__,
    author='Nitta',
    author_email='foo@gmail.com',
    url='https://github.com/foo',
    description='A test project using pybind11',
    long_description='',
    ext_modules=ext_modules,
    install_requires=['pybind11>=2.3'],
    setup_requires=['pybind11>=2.3'],
    cmdclass={'build_ext': BuildExt},
    zip_safe=False,
)

```






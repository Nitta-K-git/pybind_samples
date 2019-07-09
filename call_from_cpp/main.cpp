// https://qiita.com/benikabocha/items/5ec6b471de0d10aaf2df

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
		auto global = py::dict(py::module::import("__main__").attr("__dict__")); // globalの
		auto local = py::dict();
		// write python code here
		py::eval<py::eval_single_statement>("from time import time,ctime", global, local);
		// 以下は同じ結果
		py::eval<py::eval_single_statement>("print('Today is', ctime(time()))", global, local);
		py::eval<py::eval_expr>("print('Today is', ctime(time()))", global, local);
		py::eval("print('Today is', ctime(time()))", global, local);
		
//		py::eval<py::eval_expr>("aaa = 100+200", global, local); // error
		py::eval<py::eval_single_statement>("aaa = 100+200", global, local);
		// 変数を定義する
		local["x"] = 100; // localに新しく追加
		py::eval<py::eval_single_statement>("y = 200", global, local); // pythonコードで定義、参照先のlocalに追加される
		py::print(local["y"]);
		py::eval<py::eval_single_statement>("yy = 250", global); // pythonコードで定義、global変数として追加される
		py::print(global["yy"]);
		py::eval("print('x + y =', x + y)", global, local); // 
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
		
		// サンプル実行 https://pybind11.readthedocs.io/en/stable/advanced/pycpp/object.html
		// Equivalent to "from decimal import Decimal"
		py::object Decimal = py::module::import("decimal").attr("Decimal"); // C++で特定のライブラリ使いたいときは.attrで取得する
		// Try to import scipy
		py::object scipy = py::module::import("scipy");
		py::object scipy_ver = scipy.attr("__version__");
		py::print(py::str(scipy_ver));
		// Construct a Python object of class Decimal
		py::object pi = Decimal("3.14159");
		// Use Python to make our directories
		py::object os = py::module::import("os");
		py::object makedirs = os.attr("makedirs");
//		makedirs("./tmp/path/to/somewhere");
		// Calculate e^π in decimal
		py::object exp_pi = pi.attr("exp")();
		py::print(py::str(exp_pi));
	} catch (py::error_already_set& e) {
		std::cout << "Python error.\n" << e.what() << "\n";
	}
	
	Py_Finalize();
	PyMem_RawFree(program);
	
	return 0;
}

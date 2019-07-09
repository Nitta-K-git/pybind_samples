#define PY_SSIZE_T_CLEAN
#include <Python.h> // compile only release mode
#include <iostream>
#include <QString>

// https://docs.python.org/ja/3/extending/embedding.html#extending-embedded-python
// define module by C++ and load from python file

/*
 * pythonのファイルを読み込んで実行する
 * python内でprintしてもコマンドプロンプトの標準出力には出てこない(Qtのアプリケーション出力には出てくる) → パイプでmoreに送ると出せる
*/

static int numargs=0;

/* Return the number of arguments of the application command line */
static PyObject *emb_numargs(PyObject *self, PyObject *args){
	(void)self;
    if(!PyArg_ParseTuple(args, ":numargs"))
        return nullptr;
    return PyLong_FromLong(numargs);
}

static PyMethodDef EmbMethods[] = {
    {"numargs", emb_numargs, METH_VARARGS, "Return the number of arguments received by the process."},
    {nullptr, nullptr, 0, nullptr}
};

static PyModuleDef EmbModule = {
    PyModuleDef_HEAD_INIT, "emb", nullptr, -1, EmbMethods, nullptr, nullptr, nullptr, nullptr
};

static PyObject *PyInit_emb(void){
    return PyModule_Create(&EmbModule);
}

int main(int argc, char *argv[]){
	QString fname;
	if (argc < 2) {
//		fprintf(stderr,"Usage: call pythonfile\n");
//		std::cout << "Usage: call pythonfile funcname [args]!!\n" << std::endl;
//		return 1;
		fname.append("py_org_read_py_file_test.py");
	}else{
		fname = argv[1];
	}
	
	// load emb before Py_Initialize
	numargs = argc;
	PyImport_AppendInittab("emb", &PyInit_emb);
	
	Py_Initialize();
//	FILE *fp = fopen(argv[1], "rb");
	FILE *fp = fopen(qPrintable(fname), "rb");
	if(fp != nullptr){
//		PyRun_SimpleFile(fp, argv[1]); // exec python file
		PyRun_SimpleFile(fp, qPrintable(fname)); // exec python file
	}else{
		std::cout << "read error!!\n" << std::endl;
	}
	
	Py_Finalize(); // fix
	return 0;
}

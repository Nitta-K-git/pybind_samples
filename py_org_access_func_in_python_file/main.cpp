#define PY_SSIZE_T_CLEAN
#include <Python.h> // compile only release mode
#include <iostream>

// https://docs.python.org/ja/3/extending/embedding.html#pure-embedding
// access to func in python file

int main(int argc, char *argv[]){
	PyObject *pName, *pModule, *pFunc;
	PyObject *pArgs, *pValue;
	int i;
	
	if (argc < 3) {
		fprintf(stderr,"Usage: call pythonfile funcname [args]\n");
		std::cout << "Usage: call pythonfile funcname [args]!!\n" << std::endl;
		return 1;
	}
	
	Py_Initialize();
	pName = PyUnicode_DecodeFSDefault(argv[1]); // load python file
	/* Error checking of pName left out */
	
	pModule = PyImport_Import(pName);
	Py_DECREF(pName);
	
	if (pModule != nullptr) {
		pFunc = PyObject_GetAttrString(pModule, argv[2]); // load func
		/* pFunc is a new reference */
		
		if (pFunc && PyCallable_Check(pFunc)) {
			pArgs = PyTuple_New(argc - 3);
			for (i = 0; i < argc - 3; ++i) {
				pValue = PyLong_FromLong(atoi(argv[i + 3]));
				if (!pValue) {
					Py_DECREF(pArgs);
					Py_DECREF(pModule);
					fprintf(stderr, "Cannot convert argument\n");
					return 1;
				}
				/* pValue reference stolen here: */
				PyTuple_SetItem(pArgs, i, pValue);
			}
			pValue = PyObject_CallObject(pFunc, pArgs); // call python func object
			Py_DECREF(pArgs);
			if (pValue != nullptr) {
				printf("Result of call: %ld\n", PyLong_AsLong(pValue));
				Py_DECREF(pValue);
			}
			else {
				Py_DECREF(pFunc);
				Py_DECREF(pModule);
				PyErr_Print();
				fprintf(stderr,"Call failed\n");
				return 1;
			}
		}
		else {
			if (PyErr_Occurred())
				PyErr_Print();
			fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
		}
		Py_XDECREF(pFunc);
		Py_DECREF(pModule);
	}
	else {
		PyErr_Print();
		fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
		return 1;
	}
	Py_Finalize(); // fix
	return 0;
}

#include <pybind11/pybind11.h>

int add(int a, int b){
	return a + b;
}

PYBIND11_MODULE(minimum_sample, m) { // must same target file name
	m.doc() = "Pybind Document Sample";
	m.def("add", &add);
}

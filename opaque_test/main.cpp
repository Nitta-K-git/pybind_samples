#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(std::vector<int, std::allocator<int>>);

PYBIND11_MAKE_OPAQUE(std::vector<std::string, std::allocator<std::string>>);
using StringList = std::vector<std::string, std::allocator<std::string>>;

PYBIND11_MODULE(QT_TARGET, m)
{
	m.def("o_list", [](std::vector<int, std::allocator<int>> &l){
		l.push_back(20);
	}, "");
	
	m.def("get_unique_ptr", []() -> std::unique_ptr<StringList> {
			  StringList *result = new StringList();
			  result->push_back("some value");
			  return std::unique_ptr<StringList>(result);
		  });
	m.def("push_back", [](void (StringList::*)(const std::string &s)){
		s.push
	});
}

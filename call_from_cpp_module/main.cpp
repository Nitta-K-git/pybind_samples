// https://qiita.com/benikabocha/items/5ec6b471de0d10aaf2df
// define C++ class and set as python module

/*
 * C++側でモジュールを定義、モジュールにはC++純正のクラスを指定
 * main関数でtryする前にC++側でモジュールを登録しておく(モジュール名とポインタだけ)
 * pythonコードでimportする
*/


#include <pybind11/eval.h>
#include <pybind11/pybind11.h>

#include <iostream>

namespace py = pybind11;

class Job {
public:
	std::string GetName() const { return m_name; }
	void SetName(const std::string& name) { m_name = name; }
	
private:
	std::string m_name;
};

class Person {
public:
	std::string GetName() const { return m_name; }
	void SetName(const std::string& name) { m_name = name; }
	
	std::shared_ptr<Job> GetJob() const { return m_job; }
	void SetJob(const std::shared_ptr<Job>& job) { m_job = job; }
	
private:
	std::string m_name;
	std::shared_ptr<Job> m_job;
};

namespace py = pybind11;

PyMODINIT_FUNC PyInit_sample() {
	py::module m("sample", "pybind11 module sample."); // define module
	
	py::class_<Job, std::shared_ptr<Job>> job(m, "Job"); // add member object in defined module
	job.def(py::init<>()).def_property("name", &Job::GetName, &Job::SetName); // set member and method
	
	py::class_<Person, std::shared_ptr<Person>> person(m, "Person");
	person.def(py::init<>())
			.def_property("name", &Person::GetName, &Person::SetName) // ?
			.def_property("job", &Person::GetJob, &Person::SetJob);
	
	return m.ptr();
}

int main(int argc, char** argv) {
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
		
		// create function in Python (Name=Hoge, Job=Teacher)
		py::eval<py::eval_statements>(
					"import sample\n"
					"def initialize_person(p):\n" // argument is Person object
					"   job = sample.Job()\n" // create Job object
					"   job.name = 'Teacher'\n" // set attribute value
					"   p.name = 'Hoge'\n" // set attribute
					"   p.job = job\n",
					global); // グローバル関数として定義
		{
			auto person = std::make_shared<Person>(); // create Person object by C++
			global["initialize_person"](person); // call python function
			std::cout << "Name : " << person->GetName() << "\n";           // Name : Hoge 
			std::cout << "Job  : " << person->GetJob()->GetName() << "\n"; // Job  : Teacher
		}
		
		// overwrite function in Python (Name=Foo, Job=Programmer)
		py::eval<py::eval_statements>(
					"import sample\n"
					"def initialize_person(p):\n"
					"   job = sample.Job()\n"
					"   job.name = 'Programmer'\n"
					"   p.name = 'Foo'\n"
					"   p.job = job\n",
					global);
		{
			auto person = std::make_shared<Person>();
			global["initialize_person"](person); // C++で作成した変数で関数呼び出し→C++の変数のメンバがpython側のコードで変更される
			std::cout << "Name : " << person->GetName() << "\n";           // Name : Foo
			std::cout << "Job  : " << person->GetJob()->GetName() << "\n"; // Job  : Programmer
		}
	} catch (py::error_already_set& e) {
		std::cout << "Python error.\n" << e.what() << "\n";
	}
}

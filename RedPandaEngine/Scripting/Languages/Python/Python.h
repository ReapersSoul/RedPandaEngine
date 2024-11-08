#pragma once
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "VirtualMachineLanguage.h"


class Python :public VirtualMachineLanguage {
	PyObject* main_module, * main_dict;
	PyObject* sys_module, * sys_dict;
	PyObject* version_obj;
	char* version_string;
public:

	Python() {
		Name = "Python";
		PrimaryColor = glm::vec3(21, 234, 128);
		//maroon
		SecondaryColor = glm::vec3(128, 21, 21);
	}

	bool Init() {
		Py_Initialize();
		/* Setup the __main__ module for us to use */
		main_module = PyImport_ImportModule("__main__");
		main_dict = PyModule_GetDict(main_module);

		/* Fetch the sys module */
		sys_module = PyImport_ImportModule("sys");
		sys_dict = PyModule_GetDict(sys_module);

		/* Attach the sys module into the __main__ namespace */
		PyDict_SetItemString(main_dict, "sys", sys_module);

		/* Retrieve the Python version from sys and print it out */
		version_obj = PyMapping_GetItemString(sys_dict, "version");
		version_string = (char*)_PyUnicode_AsString(version_obj);
		printf("%s\n\n", version_string);
		Py_XDECREF(version_obj);
		return true;
	}

	bool Deconstruct() {
		Py_Finalize();
		return true;
	}

	bool LoadScript(std::string path) {
		RunScript(path);
		return true;
	};
	bool LoadString(std::string str) {
		RunString(str);
		return true;
	};

	bool RunScript(std::string path) {
		PyObject* obj = Py_BuildValue("s", path.c_str());
		FILE* file = _Py_fopen_obj(obj, "r+");
		if (file != NULL) {
			PyRun_SimpleFile(file, path.c_str());
		}
		return true;
	};

	bool RunString(std::string str) {
		PyRun_SimpleString(str.c_str());
		return true;
	};

	//TODO::
	bool RegisterFunction(std::string Name, std::function<double(std::vector<Var>*)>* f)
	{
	//	static PyMethodDef SpamMethods[] = {
	//{"system",  		[](PyObject* self, PyObject* args)-> PyObject*
	//	{
	//		const char* command;
	//		int sts;

	//		if (!PyArg_ParseTuple(args, "s", &command))
	//			return NULL;
	//		sts = system(command);
	//		return PyLong_FromLong(sts);
	//	}, METH_VARARGS,
	// "Execute a shell command."},
	//{NULL, NULL, 0, NULL}        /* Sentinel */
	//	};

	//	struct PyModuleDef spammodule = {
	//PyModuleDef_HEAD_INIT,
	//"__main__",   /* name of module */
	//NULL, /* module documentation, may be NULL */
	//-1,       /* size of per-interpreter state of the module,
	//			 or -1 if the module keeps state in global variables. */
	//SpamMethods
	//	};

	//	PyMODINIT_FUNC PyInit_spam(void)
	//	{
	//		return PyModule_Create(&spammodule);
	//	}

		return false;
	}
	//bool RegisterFunction(std::string Name, std::function<int(std::vector<Var>*)>* f)
	//{

	//	return false;
	//}
	//bool RegisterFunction(std::string Name, std::function<bool(std::vector<Var>*)>* f)
	//{

	//	return false;
	//}
	//bool RegisterFunction(std::string Name, std::function<std::string(std::vector<Var>*)>* f)
	//{

	//	return false;
	//}

	bool SetVar(std::string Name, bool value) {
		PyObject* i_obj;
		i_obj = Py_BuildValue("b", value);
		PyDict_SetItemString(main_dict, Name.c_str(), i_obj);
		Py_XDECREF(i_obj);
		return true;
	};
	bool SetVar(std::string Name, double x) {
		PyObject* value = PyLong_FromDouble(x);
		PyDict_SetItemString(main_dict, Name.c_str(), value);
		Py_DECREF(value);
		return true;
	};
	bool SetVar(std::string Name, int x) {
		PyObject* value = PyLong_FromDouble(x);
		PyDict_SetItemString(main_dict, Name.c_str(), value);
		Py_DECREF(value);
		return true;
	};
	bool SetVar(std::string Name, float x) {
		PyObject* value = PyLong_FromDouble(x);
		PyDict_SetItemString(main_dict, Name.c_str(), value);
		Py_DECREF(value);
		return true;
	};
	bool SetVar(std::string Name, char* value) {
		PyObject* i_obj;
		i_obj = Py_BuildValue(Name.c_str(), value);
		PyDict_SetItemString(main_dict, Name.c_str(), i_obj);
		Py_XDECREF(i_obj);
		return true;
	};
	bool SetVar(std::string Name, const char* value) {
		PyObject* i_obj;
		i_obj = Py_BuildValue(Name.c_str(), value);
		PyDict_SetItemString(main_dict, Name.c_str(), i_obj);
		Py_XDECREF(i_obj);
		return true;
	};
	bool SetVar(std::string Name, std::string value) {
		PyObject* i_obj;
		i_obj = Py_BuildValue(Name.c_str(), value);
		PyDict_SetItemString(main_dict, Name.c_str(), i_obj);
		Py_XDECREF(i_obj);
		return true;
	};
	//bool SetVar(std::string Name, Table value) {
	//	PyObject* i_obj;
	//	i_obj = Py_BuildValue(Name.c_str(), value);
	//	PyDict_SetItemString(main_dict, Name.c_str(), i_obj);
	//	Py_XDECREF(i_obj);
	//	return true;
	//};

	//TODO::
	//bool RegisterLinkedVar(std::string Name, T* value);

	bool GetVarAsBool(std::string Name)
	{
		PyObject* i_obj;
		i_obj = PyDict_GetItemString(main_dict, Name.c_str());
		int i = PyLong_AsLong(i_obj);
		return i;
	}

	std::string GetVarAsString(std::string Name)
	{
		PyObject* v = PyDict_GetItemString(main_dict, Name.c_str());
		std::string ret = _PyUnicode_AsString(v);
		Py_DECREF(v);
		return ret;
	}

	int GetVarAsInt(std::string Name)
	{
		/* Extract the resultant variable, k */
		PyObject* k_obj = PyDict_GetItemString(main_dict, Name.c_str());
		int k = _PyLong_AsInt(k_obj);
		Py_XDECREF(k_obj);
		return k;
	}

	double GetVarAsDouble(std::string Name)
	{
		PyObject* v = PyDict_GetItemString(main_dict, Name.c_str());
		int ret = PyLong_AsDouble(v);
		Py_DECREF(v);
		return ret;
	}


	template<typename T, int numArgs>
	T CallFunction(std::string Name, ...) {};

	template<int numArgs>
	int CallFunction(std::string Name, ...)
	{
		
	}
};
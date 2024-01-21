The Scripting Languages availible in The Redpanda Engine are [[Lua]], [[Javascript]], [[C Sharp|C#]], [[Native|Native C++]], [[Python]], and [[Java]] inherrit from a base class called Scripting_Language this class allows functions and variables to be easily defined in all languages at once using a Scripting_Language_Managerv and allows you to call functions from the language as well as get the value of the variables.

```c++
//Example
//Lua class is child of Scripting_Language
Lua lua;
Int GlobalVar=60;

//loads functions and variables into lua
lua.LoadScript("ExampleScript.lua");
lua.LoadString("function Double(x) return x*2 end");
//runs any code in file or string
lua.RunScript("ExampleScript.lua");
lua.RunString("print(\"hello world\")");

//registering function inside of lua language
lua.RegisterFunction("HalfInt",std::function<int(int)>([](int x){ return x/2;}));
//registering variable inside of lua same as set var in most implementations
lua.RegisterVar<Int>("VariableTest",10);
//registering linked global any change in the variable on the lua side will be reflected in c++ immediatly
lua.RegisterGlobalVar<Int>("VariableTest2",&GlobalVar);

//fetches the variable "VariableTest" from lua calls the function "HalfInt" from lua passing VariableTest as an argument then assignes that value back to "VariableTest"
lua.SetVar<Int>("VariableTest",lua.CallFunction<int>("HalfInt",lua.GetVar<int>("VariableTest")));

//gets the variable from lua and prints it to the console
std::cout<<lua.GetVar<Int>("VariableTest")<<std::endl;

//Javascript class is child of Scripting_Language
Javascript JS:

//used to manage languages can be used just like how you use a Scripting language however you may need to define which language you want to use for certain functions like call function, load script, run string and the like
Scripting_Language_Manager SLM;
//registering all languages
SLM.RegisterLanguage(&lua);
SLM.RegisterLanguage(&JS);
//registers function in javascript and lua
SLM.RegisterFunction("Quad",std::function<int(int)>([](int x){ return x*4;}));

int value = SLM.CallFunction<int>("Lua","Quad",10);
```

#ScriptingLanguage
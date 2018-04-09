# Easyargs

A simple C++ command line arguments parser

See [main.cpp](./main.cpp) for tests and usage examples

## Building

See the [Makefile](./Makefile)

```bash
$ make      # to build and run tests
$ make lib  # to build a dynamic lib (.so)
```

## Usage

1. Create an **instance**, passing argc and argv directly, like this: 

	```c++
	EasyArgs *ez = new EasyArgs(argc, argv);
	```

	or use the other available constructors:

	```c++
	EasyArgs(int argc, std::string argv[]);
	EasyArgs(std::vector<std::string> argv);
	```

2. Specify **Version/Description** (optionaly) for the usage message:

	```c++
	ez->Version("0.1")
	  ->Description("Example");
	```

3. Add **Flags**:

	```c++
	ez->Flag("-b", "--bacon", "bacon help message")
	  ->Flag("-v", "--verbose", "be verbose");
	```

	Flags are **optional** arguments given in the form of "-a" (short form) or "--argument" (long form). Multiple flags can also be given in the form of "-abc".

	Flags are considered _set_ (meaning the have a boolean value of `true`) when they are given in the program's cmdline arguments.
	
	To get the boolean value use `ez->IsSet("--long")` or `ez->IsSet("-s")` (long or short form respectively).

4. Add **Values**:

	```c++
	ez->Value("-f", "--foo", "foo help message", true)
	  ->Value("-p", "--pizza", "everybody likes pizza", false);
	```

	Values are arguments given in the form of "-a=value" (short form) (short form) or "--argument=value" (long form).

	The 4th argument of the function ez->Value() is a boolean value specifying if they are required of not. 

	If required, but not given, an exception is thrown.

	You can get a value from the given args with `ez->GetValueFor("--long")` or `ez->GetValueFor("-s")` (long or short form respectively).

	`ez->GetValueFor(...)` will return an empty string `""` if the argument is not given or specified.

5. Add **Positional** arguments:

	```c++
	ez->Positional("name", "your name")
	  ->Positional("nom", "om nom");
	```

	Positionals args are **required**. If not given, an exception is thrown.

	To get the value of the positional, use `ez->GetPositional("name_of_the_positional")`.

## Full Example

For the example let's suppose that the program is being run like this:

```bash
$ ./my_program -pba -m=12 --foo=bar --verbose pos1 -Sw pos2
```

```C++
int main(int argc, char *argv[])
{
	/* EasyArgs has a contructor for passing directly the cmdline args like this: */
	EasyArgs *ez = new EasyArgs(argc, argv);

	ez->Version("0.1")              // specify version
	  ->Description("Example")      // add a description

	  ->Flag("-b", "--bacon", "bacon help message")        // add some flags
	  ->Flag("-v", "--verbose", "be verbose")
	  
	  ->Value("-f", "--foo", "foo help message", true)     // add expected values (true = required)
	  ->Value("-p", "--pizza", "everybody likes pizza", false)
	  
	  ->Positional("name", "your name")                    // add expected positional
	  ->Positional("nom", "om nom");
	
	//ez->PrintUsage();             // print usage info if you want


	ez->IsSet("-w");                // false because such argument doesn't exist
	ez->IsSet("-S");                // false, for the same reason
	
	ez->IsSet("-b");                // true, because 'b' is set (in the group of "-pba")
	ez->IsSet("--bacon");           // true, --bacon is long form for -b
	
	ez->IsSet("-v");                // true, same with above
	ez->IsSet("--verbose");         // true, same
	
	ez->IsSet("--foo");             // false, --foo is a value arg, not a flag
	ez->IsSet("-f");                // false, same reason

	ez->GetValueFor("--foo");       // "bar"
	ez->GetValueFor("-f");          // "bar"
	
	ez->GetValueFor("-m");          // "", because arg is not specified (even if given in cmdline)
	ez->GetValueFor("-p");          // "", because "-p" is given, but as flag
	ez->GetValueFor("--pizza");     // "", same

	ez->GetPositional("name");      // "pos1", positionals maintain their order, so "pos1"
	ez->GetPositional("nom");       // "pos2", same

	delete ez;

	return 0;
}
```

With `ez->PrintUsage()` you can print usage info:

```
version: 0.1
description: Example
usage: ./my_program [OPTIONS] name nom 

OPTION       REQUIRED   DESCRIPTION
===========================================
-b, --bacon	no	bacon help message
-v, --verbose	no	be verbose
-f=, --foo=	yes	foo help message
-p=, --pizza=	no	everybody likes pizza


POSITIONAL      DESCRIPTION
===========================================
name		your name
nom		om nom
```

#include "src/easyargs.h"

// TODO: usage
// TODO: write tests
// TODO: make lib
// TODO: add a callback on error
// TODO: add methods: getAllFlags, getAllValues, getAllPositional
// TODO: add another constructor for vector, or array
int main(int argc, char *argv[])
{
	char *raw[] = {"exec", "-pba", "-m=12", "--foo=bar", "--verbose", "pos1", "-Sw", "pos2"};
	int raw_len = 8;

	EasyArgs *ez = new EasyArgs(raw_len, raw);

	ez->Version("0.1")
		->Description("Example")
		->Flag("-b", "--bacon", "bacon help message")
		->Flag("-v", "--verbose", "be verbose")
		->Value("-f", "--foo", "foo help message", true)
		->Value("-p", "--pizza", "everybody likes pizza", false)
		->Positional("name", "your name")
		->Positional("nom", "om nom");
//		->PrintUsage();

	if (ez->IsSet("-w"))
		std::cout << "True" << std::endl;

	std::cout << "Value for optional: " << ez->GetValueFor("--foo") << std::endl;
	std::cout << "Value for positional: " << ez->GetPositional("nom");

	delete ez;

	return 0;
}
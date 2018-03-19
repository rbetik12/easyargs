#include <iostream>
#include <string>
#include <vector>

class Argument {
public:
private:
};

class EasyArgs {
public:
	EasyArgs(char argc, char *argv[]) {
		for (int i = 0; i < argc; i++) {
			this->args.push_back(std::string(argv[i]));
		}
		/* for (std::string s : this->args) {
			std::cout << s << std::endl;
		} */ 
	};

	~EasyArgs() {};

	void AddFlag(std::string shrt, std::string lng, std::string help) {};
	void AddValue(std::string shrt, std::string lng, std::string help, std::string req) {};
	void AddPositional(std::string name) {};
	void Parse() {};
	bool IsSet(std::string argname) {return false; };
	std::string GetValueFor(std::string argname) {return "a";};
	std::string GetPositional(std::string argname) {return "f";};

private:
	std::vector<std::string> args;

	void Explode() {};
};

int main(int argc, char *argv[]) {

	char *raw[] = {"-pa", "-f", "12", "--verbose", "true", "pos1", "-S"};
	int raw_len = 7;

	EasyArgs *ez = new EasyArgs(raw_len, raw);

	ez->AddFlag("-b", "--bacon", "bacon help message");
	ez->AddValue("-f", "--foo", "foo help message", "required");
	ez->AddValue("-p", "--pizza", "everybody likes pizza", "optional");
	ez->AddPositional("name");

	ez->Parse();

	if (ez->IsSet("b")) {
		std::cout << "True";
	}
	std::cout << "Value for p: " << ez->GetValueFor("p") << std::endl;
	std::cout << "Value for positional: " << ez->GetPositional("name");

	return 0;
}

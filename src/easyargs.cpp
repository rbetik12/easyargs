#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

class EasyArgs
{
public:
	// TODO: make cleaner the valueDelimiter (enum?)
	EasyArgs(int argc, char *argv[])
	{
		for (int i = 0; i < argc; i++)
			this->args.push_back(std::string(argv[i]));

		this->valueDelimiter = '=';

//		for (std::string s : this->args)
//			std::cout << s << ", ";

		this->posArgCount = 0;
	}

	~EasyArgs() {}

	EasyArgs *Version(std::string version)
	{
		this->version = version;
		return this;
	}

	EasyArgs *Description(std::string desc)
	{
		this->description = desc;
		return this;
	}

	EasyArgs *Flag(std::string shrt, std::string lng, std::string helptext)
	{
		// if shrt in args or lng in args, push both in flagsSet;
		if (this->FlagIsSet(shrt, lng))
		{
			this->flagsSet.push_back(shrt);
			this->flagsSet.push_back(lng);
		}
		return this;
	}

	EasyArgs *Value(std::string shrt, std::string lng, std::string helptext, bool required)
	{
		// search for shrt. if not found, search for lng. store value in map (with entries
		// both the shrt and long name). if required and not given, ->error.
		std::string val = this->GetValueFromArgs(shrt, lng);

		if (val.empty() && required)
			this->Error("argument " + shrt + "," + lng + " is required");
		else
		{
			this->valuesGiven.insert(std::pair<std::string, std::string>(shrt, val));
			this->valuesGiven.insert(std::pair<std::string, std::string>(lng, val));
		}

		return this;
	}

	EasyArgs *Positional(std::string name, std::string helptext)
	{
		// in constructor -> form posvector with given positionals. after that,
		// in this function, count the times it is called, and store to the 
		// posit. map <name, posvector[times called]> or throw error if not given enough
		// in vector.
		// search for positional, store in appropriate map with key
		this->posArgCount++;
		std::string pos = this->FetchPositional();
		if (pos.empty())
			this->Error("positional argument " + name + " is required");
			
		this->posGiven.insert(std::pair<std::string, std::string>(name, pos));

		return this;
	}

	bool IsSet(std::string arg)
	{
//		for (std::string flag : this->flagsSet)
//			std::cout << flag << ", ";
		return std::find(this->flagsSet.begin(), this->flagsSet.end(), arg)
			!= this->flagsSet.end();
	}

	std::string GetValueFor(std::string arg)
	{
//		valuesSet.insert(std::pair<std::string, std::string>())
		return valuesGiven[arg];
	}

	std::string GetPositional(std::string name)
	{
		return this->posGiven[name];
	}

private:
	std::vector<std::string> args;
	enum ArgType {flagShrt, flagLng, val, pos};

	std::vector<std::string> flagsSet;
	std::map<std::string, std::string> valuesGiven;
	std::map<std::string, std::string> posGiven;
	int posArgCount;

	std::string version;
	std::string description;
	std::string valueDelimiter;

	bool FlagIsSet(std::string shrt, std::string lng)
	{
		for (std::string rawArg : this->args)
		{
			ArgType typ = this->FindArgType(rawArg);
//			std::cout << rawArg + " has type: " << typ << std::endl;
			
			if (typ == ArgType::flagLng 
				&& rawArg == lng)
				return true;

			if (typ == ArgType::flagShrt 
				&& rawArg.find(shrt[1]) != std::string::npos)
				return true;
		}
		return false;
	}

	// instead of looking up each time, I should maybe find the types once and be done with it.
	ArgType FindArgType(std::string arg)
	{
		if (arg[0] != '-')
			return ArgType::pos;

		size_t pos = 0;
		if ((pos = arg.find(this->valueDelimiter)) != std::string::npos)
			return ArgType::val;

		return arg.size() > 2 && arg[1] == '-' ? ArgType::flagLng : ArgType::flagShrt;
	}

	std::string GetValueFromArgs(std::string shrt, std::string lng)
	{
		// I use the same code above, abstract pls.
		for (std::string rawArg : this->args)
		{
			ArgType typ = this->FindArgType(rawArg);

			std::string key = this->ExtractKey(rawArg);

			if (typ == ArgType::val
				&& (key == lng || key == shrt))
				return this->ExtractValue(rawArg);
		}

		return "";
	}

	std::string ExtractKey(std::string arg)
	{
		return arg.substr(0, arg.find(this->valueDelimiter));
	}

	std::string ExtractValue(std::string arg)
	{
		size_t pos = arg.find(this->valueDelimiter);
		return arg.substr(pos + 1, arg.length() - pos -1);
	}

	std::string FetchPositional()
	{
		int i = 0;
		for (std::string rawArg : this->args)
		{
			ArgType typ = this->FindArgType(rawArg);
			if (typ == ArgType::pos)
				i++;
			if (i == this->posArgCount)
				return rawArg;				
		}
		return "";
	}

	void Error(std::string err)
	{
		std::cerr << "arguments error: " + err << std::endl;
		//this->PrintUsage();
		//throw exception;
	}
};

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
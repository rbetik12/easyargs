#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

#define DEBUG

class Argument {
public:
	std::string Short;
	std::string Long;
	std::string Helptext;
	bool Required;

	Argument(std::string shrt, std::string lng, std::string helptext, bool required) {
		this->Short = shrt;
		this->Long = lng;
		this->Helptext = helptext;
		this->Required = required;
	}
};

class EasyArgs {
public:
	EasyArgs(char argc, char *argv[]) 
	{
		for (int i = 0; i < argc; i++)
			this->args.push_back(std::string(argv[i]));
		
		this->InitUsage();
		this->ExplodeFlags();

		for (std::string s : this->args)
			std::cout << s + ' ';
		std::cout << std::endl;
	};

	// should I free the Arguments?
	~EasyArgs() {}

	EasyArgs* Version(std::string version) 
	{
		this->version = version;
		return this;
	}

	EasyArgs* Description(std::string desc) 
	{
		this->description = desc;
		return this;
	}

	EasyArgs* Flag(std::string shrt, std::string lng, std::string help) 
	{
		this->AddUsageOption(shrt, lng, help, false);
		this->expectedArgs.push_back(new Argument(shrt, lng, help, false));
		return this;
	}

	EasyArgs* Value(std::string shrt, std::string lng, std::string help, bool required) 
	{
		this->AddUsageOption(shrt, lng, help, required);
		this->expectedArgs.push_back(new Argument(shrt, lng, help, required));
		return this;
	}

	EasyArgs *Positional(std::string argname, std::string help)
	{
		this->usagePos << argname << ' ';
		this->expectedArgs.push_back(new Argument(argname, argname, help, true));
		return this;
	};

	bool IsSet(std::string argname) 
	{
		Argument* ret = GetArgByName(argname);

		if (!ret)
			return false;

		return std::find(this->args.begin(), this->args.end(), '-' + ret->Short) != this->args.end() 
			|| std::find(this->args.begin(), this->args.end(), "--" + ret->Long) != this->args.end();
	}
	
	std::string GetValueFor(std::string argname) 
	{
		auto it = std::find(this->args.begin(), this->args.end(), '-' + argname);
		if (it == this->args.end() || it == this->args.end() - 1) 
		{
			it = std::find(this->args.begin(), this->args.end(), "--" + argname);
			if (it == this->args.end() || it == this->args.end() - 1) 
			{
				std::ostringstream err;
				err << '"' << argname << '"' << " not specified";
				this->Error(err.str());
			}
		}
		std::string ret = *(it + 1);
		if (ret[0] != '-') 
		{
			return ret;
		}
		return "penis";
	}

	std::string GetPositional(std::string name) 
	{
		return name; // FIXME
	}
	
	void PrintUsage()
	{
		std::cout << this->usage.str();
		std::cout << this->usagePos.str() << std::endl << std::endl;
		std::cout << "version: " << this->version << std::endl;
		std::cout << "description: " << this->description << std::endl << std::endl;
		std::cout << this->usageOpt.str() << std::endl;
	}

private:
	std::vector<std::string> args;
	std::vector<Argument*> expectedArgs;
	std::map<std::string, std::string> optValues;
	std::map<std::string, std::string> posValues;

	std::stringstream usage;
	std::stringstream usageOpt;
	std::stringstream usagePos;

	std::string version;
	std::string description;

	void InitUsage() 
	{
		this->version = "0.0.1";
		this->description = "-";
		this->usage << "usage: " << this->args[0] << " [OPTIONS] ";
		this->usageOpt << "OPTION       REQUIRED   DESCRIPTION\n-----------------------------------\n";
	}

	void ExplodeFlags() 
	{
		/* if s is argument of type '-abc', make it '-a -b -c' */
		int argc = this->args.size();

		for (int i = 0; i < argc; i++) 
		{
			std::string s = this->args[i];

			/* push back the exploded args */
			if (this->IsMultipleFlagsArg(s))
			{
				for (unsigned int i = 1; i < s.length(); i++) 
				{
					std::string ret = "-";
					ret += s[i];
					this->args.push_back(ret);
				}
			}
		}
		/* delete the leftovers */
		for (auto it = this->args.begin(); it != this->args.end(); ) 
		{
			if (this->IsMultipleFlagsArg(*it)) 
				it = this->args.erase(it);
			else
				it++;
		}
	};

	bool IsMultipleFlagsArg(std::string s) 
	{
		/* I didn't want to make <regex> a dependency, because it is too heavy
		 * and I have already used enough */
		return s.length() > 2 
			&& s[0] == '-' 
			&& s[1] != '-'
			&& s.find("=") == std::string::npos;
	}

	Argument* GetArgByName(std::string name) 
	{
		Argument* ret = NULL;

		for (Argument *arg : this->expectedArgs)
		{
			if (arg->Short == name || arg->Long == name)
				ret = arg;
		}

		return ret;
	}

	void AddUsageOption(std::string shrt, std::string lng, std::string help, bool required) 
	{
		//lng = lng.size() < 8 ? lng + '\t' : lng;
		std::string req = required ? "yes" : "no";
		this->usageOpt << shrt + ", " << lng + '\t' << req + '\t' << help << std::endl;
	}

	void Error(std::string err) 
	{
		std::cout << "wrong arguments: " << err << std::endl;
		this->PrintUsage();
	}
};

// TODO: subcommands
// TODO: get as specific type
int main(int argc, char *argv[])
{
	char *raw[] = {"exec", "-pba", "-f=12", "--foo=bar", "--verbose", "pos1", "-Sw"};
	int raw_len = 7;

	EasyArgs *ez = new EasyArgs(raw_len, raw);

	ez->Version("0.1")
		->Description("Example")
		->Flag("b", "bacon", "bacon help message")
		->Flag("v", "verbose", "be verbose")
		->Value("f", "foo", "foo help message", true)
		->Value("p", "pizza", "everybody likes pizza", false)
		->Positional("name", "your name")
		->Positional("nom",  "om nom")
		->PrintUsage();

	if (ez->IsSet("pizza"))
		std::cout << "True" << std::endl;
	
	std::cout << "Value for optional: " << ez->GetValueFor("f") << std::endl;
	std::cout << "Value for positional: " << ez->GetPositional("name");

	delete ez;

	return 0;
}

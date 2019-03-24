#include <assert.h>
#include "easyargs.h"

#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[00m"

#define NUM_TESTS 4

/* Generic test */
void test0() {
    std::string input[] = {"exec", "-pba", "-m=12", "--foo=bar", "--verbose", "pos1", "-Sw", "pos2", "-"};
    int inputLen = 8;

    EasyArgs *ez = new EasyArgs(inputLen, input);

    ez->Version("0.1")
        ->Description("Example")
        ->Flag("-b", "--bacon", "bacon help message")
        ->Flag("-v", "--verbose", "be verbose")
        ->Value("-f", "--foo", "foo help message", true)
        ->Value("-p", "--pizza", "everybody likes pizza", false)
        ->Positional("name", "your name")
        ->Positional("nom", "om nom");

    // ez->PrintUsage();

    assert(!ez->IsSet("-w"));
    assert(!ez->IsSet("-S"));
    assert(ez->IsSet("-b"));
    assert(ez->IsSet("--bacon"));
    assert(ez->IsSet("-v"));
    assert(ez->IsSet("--verbose"));
    assert(!ez->IsSet("--foo"));
    assert(!ez->IsSet("-f"));

    assert(ez->GetValueFor("--foo") == "bar");
    assert(ez->GetValueFor("-f") == "bar");
    assert(ez->GetValueFor("-m") == "");
    assert(ez->GetValueFor("-p") == "");
    assert(ez->GetValueFor("--pizza") == "");

    assert(ez->GetPositional("name") == "pos1");
    assert(ez->GetPositional("nom") == "pos2");

    delete ez;
}

void test1() {
    std::string input[] = {"exec"};
    int inputLen = 1;

    EasyArgs *ez = new EasyArgs(inputLen, input);

    assert(ez->GetValueFor("--pizza") == "");
    assert(ez->GetPositional("test") == "");

    delete ez;
}

/* Random Order */
void test2() {
    std::string input[] = {"exec", "--pickles=green", "-apg", "-d", "trains", "-w=qwerty"};
    int inputLen = 6;

    EasyArgs *ez = new EasyArgs(inputLen, input);

    ez->Positional("trains", "are very important");
    ez->Version("6.0");
    ez->Flag("-e", "--enable", "Cheese");
    ez->Description("dunno");
    ez->Value("-p", "--pickles", "just pickles", true);

    try {
        /* argument does not exist, and is required so an exception is thrown */
        /* uncomment the following line */

        //ez->Value("-t", "--try", "I tried", true);
    } catch (const std::exception &ex) {
        std::cerr << ex.what() << std::endl;
        ez->PrintUsage();
    }

    assert(!ez->IsSet("-c"));
    assert(ez->GetValueFor("--pickles") == "green");
    assert(ez->GetPositional("trains") == "trains");
    assert(ez->GetValueFor("-w") == "");

    delete ez;
}

void test3() {
    std::vector<std::string> argv = {"execname", "foo"};

    EasyArgs *ez = new EasyArgs(argv);
    ez->Flag("-b", "--bar", "foobar");
    assert(!ez->IsSet("-b"));

    delete ez;
}

typedef void (*test)();

test tests[NUM_TESTS] = {test0, test1, test2, test3};

int main(int argc, char *argv[]) {
    for (int i = 0; i < NUM_TESTS; i++) {
        std::cout << "Executing test" << i << "..." << std::endl;
        tests[i]();
    }

    std::cout << GREEN << "All tests passed." << RESET << std::endl;
    return 0;
}

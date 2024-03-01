#include <stdio.h>
#include <iostream>
using std::string;
int main() {
    try {
        string str1("Test");
        string str2("ing");
        str1.append(str2, 4, 2);
        throw "no error in the code";
    }

    catch (std::exception &e) {
        std::cout << "caught exception";
    }

    catch (std::out_of_range &e) {
        std::cout << "caught out of range";
    }

    catch (const char* c) {
        std::cout << c;
    }

    return 0;
}

#include "string.h"

using namespace std;
using namespace growtopia;

string helpers::get_upper(string txt) {
    string ret;
    for (char c : txt) ret += char(toupper(c));
    return ret;
}

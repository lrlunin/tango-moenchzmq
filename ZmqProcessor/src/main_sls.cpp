#include <iostream>
#include <sls/ZmqSocket.h>
#include <rapidjson/document.h>
#include "archiver.h"
using namespace std;

struct Student {
    std::string name;
    unsigned age;
    double height;
    bool canSwim;
};

template <typename Archiver>
Archiver& operator&(Archiver& ar, Student& s) {
    ar.StartObject();
    ar.Member("name") & s.name;
    ar.Member("age") & s.age;
    ar.Member("height") & s.height;
    ar.Member("canSwim") & s.canSwim;
    return ar.EndObject();
};

std::ostream& operator<<(std::ostream& os, const Student& s) {
    return os << s.name << " " << s.age << " " << s.height << " " << s.canSwim;
}

int main(){
    cout << "Hello world, zmqheader size: " << sizeof(sls::zmqHeader) << endl;

    const char* json_str = strdup("{ \"name\": \"John Doe\", \"age\": 14, \"height\": 22.2, \"canSwim\": true}");
    Student s = { "Lua", 9, 150.5, true };
    JsonReader reader(json_str);
    reader & s;
    cout << s << endl;
    return 0;
}
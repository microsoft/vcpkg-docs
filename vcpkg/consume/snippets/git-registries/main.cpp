#include <iostream>
#include <beison.h>

int main()
{
    beison::Object obj;
    obj.insert("name", beison::Value::string("demo"));
    std::cout << beison::stringify(obj) << std::endl;
    return 0;
}

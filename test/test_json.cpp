#include "json.h"
#include "str.h"

using namespace rapidjson;

constexpr Str hello_world = "{\"hello\": \"world\", \"a\": [1, 2, 3, 4]}";

Task task = {};

int main()
{
    Document document;
    document.Parse(hello_world.buf);

    assert(document.HasMember("hello"));
    assert(document["hello"].IsString());
    printf("hello = %s\n", document["hello"].GetString());

    const Value& a = document["a"];
    assert(a.IsArray());
    for (auto& v : a.GetArray()) printf("%d ", v.GetInt());
    printf("\n");

    return 0;
}

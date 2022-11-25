#include "Tools/JSonUtils.h"


#include <rapidjson/document.h>
#include <iostream>


namespace libapp {

void printJSon(const rapidjson::Value::Array& a, const std::string spaceIterator) 
{
    size_t idx{};

    for (auto& itsub : a) {
        if (itsub.IsString()) {
            std::cout << fmt::format("printJSon | {}   - [{}] ==>   value string: \"{}\"", spaceIterator, idx++, itsub.GetString()) << std::endl;
        } else if (itsub.IsInt()) {
            std::cout << fmt::format("printJSon | {}   - [{}] ==>   value int: {}", spaceIterator, idx++, itsub.GetInt()) << std::endl;
        } else if (itsub.IsNumber()) {
            std::cout << fmt::format("printJSon | {}   - [{}] ==>   value double: {}", spaceIterator, idx++, itsub.GetDouble()) << std::endl;
        } else if (itsub.IsBool()) {
            std::cout << fmt::format("printJSon | {}   - [{}] ==>   value bool: {}", spaceIterator, idx++, itsub.GetBool()) << std::endl;
        } else if (itsub.IsArray()) {
            printJSon( itsub.GetArray(), fmt::format("{}  [{}]", spaceIterator, idx++) );
        }
    }
}

void printJSon(const rapidjson::Value::Object& config, const std::string spaceIterator) 
{
    for (auto& cfgContext : config) {
        if (cfgContext.value.IsString()) {
            std::cout << fmt::format("printJSon | {} - item \"{}\", value string: \"{}\"", spaceIterator, cfgContext.name.GetString(), cfgContext.value.GetString()) << std::endl;
        } else if (cfgContext.value.IsInt()) {
            std::cout << fmt::format("printJSon | {} - item \"{}\", value int: {}", spaceIterator, cfgContext.name.GetString(), cfgContext.value.GetInt()) << std::endl;
        } else if (cfgContext.value.IsNumber()) {
            std::cout << fmt::format("printJSon | {} - item \"{}\", value double: {}", spaceIterator, cfgContext.name.GetString(), cfgContext.value.GetDouble()) << std::endl;
        } else if (cfgContext.value.IsBool()) {
            std::cout << fmt::format("printJSon | {} - item \"{}\", value bool: {}", spaceIterator, cfgContext.name.GetString(), cfgContext.value.GetBool()) << std::endl;
        } else if (cfgContext.value.IsArray()) {
            std::cout << fmt::format("printJSon | {} - array name: \"{}\" ", spaceIterator, cfgContext.name.GetString()) << std::endl;
            printJSon( cfgContext.value.GetArray() );
        } else if (cfgContext.value.IsObject()) {
            std::cout << fmt::format("printJSon | {} - object name: \"{}\" ", spaceIterator, cfgContext.name.GetString()) << std::endl;
            printJSon(cfgContext.value.GetObject(), spaceIterator + "  ");
        } else {
            const size_t size = (cfgContext.value.IsNull() ? 0 : cfgContext.value.MemberCount());
            std::cout << fmt::format("printJSon | {}  ==> \"{}\" : type \"{}\", size: {}", spaceIterator, cfgContext.name.GetString(), cfgContext.value.GetType(), size) << std::endl;
        }
    }
}

}



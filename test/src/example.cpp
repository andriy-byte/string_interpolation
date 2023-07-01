#include <iostream>
#include "../../include/string_interpolation.hpp"
#include <chrono>
#include <ctime>

template<istr::CharTypes T>
void print_test(const istr::BasicStringInterpolation<T> &stringInterpolation, const std::string &type)
{
    std::cout << "---------------------test-start------------------------------" << std::endl;
    std::cout << "test " << type << " : " << std::endl;
    std::cout << "pattern is : \"" << stringInterpolation.getPattern() << "\"" << std::endl;
    std::cout << "open delimiter : " << stringInterpolation.getOpenDelimiter() << std::endl;
    std::cout << "close delimiter : " << stringInterpolation.getCloseDelimiter() << std::endl;
    std::cout << "pattern arguments : " << std::endl;

    for (auto[key, value]: stringInterpolation.getPatternArguments())
    {
        std::cout << key << " : " << value << std::endl;
    }

    std::cout << "substituted string is : " << stringInterpolation.getSubstituted() << std::endl;

    std::cout << "---------------------test-end--------------------------------" << std::endl;
}

int main(){



    istr::StringInterpolation stringInterpolation("name is  --name-- ", "--", "--");

    stringInterpolation.putPatternArgument("name", "Harry");
    // or
    // stringInterpolation.putPatternArgument("--name--","Harry");

    auto now =[]()
    {
        auto end = std::chrono::system_clock::now();
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        return std::string(std::ctime(&end_time));
    };

    // in this case only pattern is immutable
    const istr::StringInterpolation stringInterpolation1(" current time is  {{time}}");
    //by default is using as open delimiter "{{" and  as close delimiter "}}"

    stringInterpolation1.putPatternArgument("time",now());
    // or
    // stringInterpolation.putPatternArgument("{{time}}",now());

    istr::StringInterpolation stringInterpolation2 = istr::StringInterpolation("--1)) --2)) --3))");

    stringInterpolation2.setOpenDelimiter("--");
    stringInterpolation2.setCloseDelimiter("))");

    stringInterpolation2.putPatternArguments({{"1", "one" },{"2", "two" },{"3", "three" } });

    /*
     *or
     *    StringInterpolation2.putPatternArguments({
                                         {"--1))","one"},
                                         {"--2))","two"},
                                         {"--3))","three"}});
     */

    print_test(stringInterpolation,"StringInterpolation");
    print_test(stringInterpolation1,"StringInterpolation");
    print_test(stringInterpolation2,"StringInterpolation");

    //    istr::StringInterpolation stringInterpolation3 = istr::StringInterpolation("test for optimization {{tests}}");
    //
    //    std::cout<<stringInterpolation3.getSubstituted()<<std::endl;

    // in this case you will recive error
    //
    //  " has no arguments to replace in pattern, cannot do substitution string"
    // because you don't add arguments for replace them in pattern and create substition
    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    // but you can use " Modes::WITHOUT_REPLACEMENT_VALIDATION " for diabled substition validation,
    // in this case, the responsibility rests with you, you must follow which pattern you inserted and which arguments you added

    istr::StringInterpolation stringInterpolation4 = istr::StringInterpolation("test for optimization {{tests}}",
                                                                               istr::SubstitutionPreparationModes::WITHOUT_REPLACEMENT_VALIDATION);
    std::cout << stringInterpolation4.getSubstituted()<<std::endl;

    istr::StringInterpolation stringInterpolation5("test for prepare substition : {{test}}");
    stringInterpolation5.putPatternArgument("test", "test passed");
    stringInterpolation5.prepareSubstitution();

    std::cout << stringInterpolation5.getSubstituted()<<std::endl;

    return 0;
}

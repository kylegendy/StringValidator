//
// Created by Kyle Gendreau on 7/11/21.
//

#ifndef STRINGVALIDATOR_STRINGVALIDATOR_H
#define STRINGVALIDATOR_STRINGVALIDATOR_H

#include <map>
#include <list>
#include <string>


struct CharValidator {

    // operator that checks validity and makes any edits necessary
    void operator()(char32_t& c) {
        auto it(map.find(c));
        if (it != map.end()) {
            c = it->second;
        }
        else
            throw std::logic_error("no matching mapped character\n");
    }

    // map holds keys of readable chars, and values that it should equal
    std::map<char32_t, char32_t> map;

};

struct StrValidator {

    // operator looks for ALL substrings to replace in order
    void operator()(std::string& s) {
        bool altered(false);
        int32_t index(-1);
        for (auto it(list.begin()); it != list.end(); ++it) {
            index = s.find(it->first);
            if (index != -1) {
                altered = true;
                s.replace(index,it->first.size(),it->second);
            }
        }
        if (altered)
            operator()(s); // if altered, make recursive call to do it all again
    }

    // ordered list of all substrings to search for and replace
    std::list<std::pair<std::string,std::string>> list;

};


template <typename C = CharValidator, typename S = StrValidator>
class StringValidator {
public:

    explicit StringValidator();

    explicit StringValidator(const StringValidator& rhs);
    explicit StringValidator(const StringValidator&& rhs);

    ~StringValidator() = default;

    StringValidator& operator=(const StringValidator& rhs);
    StringValidator& operator=(const StringValidator&& rhs);

//////////////////////////////////////////////////////
//// FUNCTOR

    std::string operator()(std::string str);

//////////////////////////////////////////////////////
//// ACCESSORS

    C& getCharValidator();

    S& getSubstringValidator();

private:

    // takes a char and ensures its valid, else replaces it
    C charValidator;

    // ensures that all substrings are valid
    S substringValidator;

};

#include "../src/StringValidator.cpp"

#endif //STRINGVALIDATOR_STRINGVALIDATOR_H

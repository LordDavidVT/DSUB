//filter.hpp
#ifndef _FILTER_H_
#define _FILTER_H_

#include <algorithm>
#include <vector>
#include <string>
#include <set>
#include <sstream> 


class filter {

protected:


public:
   

    filter();

    ~filter();

    bool check_message(std::string message_content);


private:
    std::vector<std::string> banned_words;
    bool advanced_message_checker(std::string message_content);



};

#endif // _FILTER_H_
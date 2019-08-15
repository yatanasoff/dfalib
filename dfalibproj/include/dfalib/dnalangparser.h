#ifndef DNA_LANG_PARSER_H
#define DNA_LANG_PARSER_H

#include <fstream>
#include <list>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <regex>
#include <set>
#include <stack>
#include "dfalib/stringutils.h"

using namespace std;

class DNALangParser
{
public:
    map<string, set<string>> parseFile(const std::string& file_path);
    map<string, set<string>> parseString(const std::string& grammar);
    map<string, set<string>> parse(const list<std::string> chains);

};

#endif

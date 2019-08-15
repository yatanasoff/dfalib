#ifndef GRAMMARGENERATOR_H
#define GRAMMARGENERATOR_H


#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

class GrammarGenerator{

public:
    string generate_gquadruplex(int count = 1);
    string generate_imotiv(int count_1 = 20, int count_2 = 20);
    vector<string> cartesian_product(vector<string> data);
    vector<string> generate_hairpins_from_set(vector<string> hrps);
    vector<string> get_triplex_set(int kind = 1);
    std::string create_grammar(
            bool find_GQD = 0,
            bool find_IMT = 0,
            bool find_TPR = 0,
            bool find_HRP = 0,
            vector<string> hrps = {},
            int lenght=0);
};
#endif // GRAMMARGENERATOR_H

#include <iostream>
#include <set>
#include <queue>
#include <map>
#include <cassert>
#include <list>
#include <algorithm>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "dfalib/algorithms.h"
#include "ProgressBar.hpp"

using std::string;
using std::map;
using namespace dfa;

int main(int argc, char* argv[]) {

    string tmpdir = "/home/christmas/Desktop/asd2";
    string grammarpath = tmpdir + "/grammar.txt";
    string regpath = tmpdir + "/parsed_grammar.txt";
    string resultgraphpath = tmpdir + "/result_graph.txt";
    string minstringpath = tmpdir + "/next_result.txt";
    string outputpath = tmpdir +"/output.txt";

    vector<vector<int>> values_from_best_sequences;

    bool find_GQD = 0, find_IMT= 1, find_TRP=0, find_HRP=0; int size = 0;


    std::map<std::string, std::shared_ptr<Automata>> processed_items;
    DNALangParser grammarParser;
    GrammarGenerator grammarGenerator;
    Parser parser;
    string grammar_string = grammarGenerator.create_grammar(find_GQD, find_IMT, find_TRP, find_HRP,{}, size);
    cout << grammar_string << endl << "-----------------------------------------" << endl;
    std::map<string, set<string> > grammar = grammarParser.parseString(grammar_string);
    assert(grammar.count("result") != 0);
    for (auto it = grammar.begin(); it != grammar.end(); ++it) {
        if (it->first == "result") {
            continue;
        }
        cout << "reading rule " << it->first << std::endl;

        std::shared_ptr<Automata> result;
        int ind = 0;
        for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
            cout << "\treading subrule " << ind++ << std::endl;


            string expr = *jt;
            std::string::iterator end_pos = std::remove(expr.begin(), expr.end(), ' ');
            expr.erase(end_pos, expr.end());
            RegEx re;
            re.Compile(expr);
            stringstream f1;
           re.Dump2Stream(f1);
            auto temp = find_min_automata(Automata::read_from_stream(f1));

            // generate_automata_visualization_script(temp, "E:/projects/projects-git/dfalib/img/fsm-my.gv");

            if (jt == it->second.begin()) {
                result = temp;
            } else {
                result = find_min_automata(sum_automata(result, temp));
            }
            cout << "\tCount of states in subrule: " << result->state_count() << std::endl;
        }
        processed_items[it->first] = result;

    }


    std::string resultexpr = *grammar["result"].begin();
    std::shared_ptr<GrammarExprTree> calculations_graph = parser.parse_request(resultexpr);


    std::set<std::string> items;
    std::queue<std::shared_ptr<GrammarExprTree>> nodes;
    nodes.push(calculations_graph);
    while (nodes.empty() == false) {
        auto next = nodes.front();
        nodes.pop();
        for (int i = 0; i < next->childs.size(); ++i) {
            nodes.push(next->childs[i]);
        }
        if (next->childs.empty()) {
            items.insert(next->name);
        }
    }

    cout << "Answer calculation..." << std::endl;
    std::shared_ptr<Automata> big = generate_big_automata(calculations_graph, processed_items);
    cout << "\tCount of states in result: " << big->state_count() << std::endl;
    std::vector<string> min_strings;
    map<string, vector<int>> best_sequences = {};

    find_all_min_strings(big, min_strings);

    cout << "total: " << min_strings.size() <<  endl;
    if (min_strings.empty()) {
        cout << "NO ANSWER" << std::endl;
    } else {
        const int limit = min_strings.size();
        cout << limit << endl;
        ProgressBar progressBar(limit, 50);



//          min_strings  = {"gaaagaaagaaag"};

        for (auto &nextstring : min_strings) {
            best_sequences.insert({nextstring, analyze_string(nextstring,find_GQD,find_IMT,find_TRP,find_HRP)}) ;
            ++progressBar;
            progressBar.display();

        }
        progressBar.done();
    }




    ofstream output(outputpath);
    assert(output.is_open()==true);
    int i,im;
    for( auto k : best_sequences){
            string coefs = "(";
            im = k.second.size();
            for(i=0;i<im;i++){
                coefs += to_string(k.second[i])+(i==im-1?"":", ");
            }
            coefs += ")";
        output << k.first <<": "<< coefs << endl;

    }

    return 0;
}

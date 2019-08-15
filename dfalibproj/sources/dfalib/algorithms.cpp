#ifndef ALG_DFA
#define ALG_DFA

#include <string>
#include <fstream>
#include <vector>
#include <list>
#include <queue>
#include <bits/stdc++.h>
#include <set>

#include "dfalib/dfa.h"
#include "dfalib/parserequest.h"
#include "dfalib/grammargenerator.h"
#include "dfalib/dnalangparser.h"
#include "dfalib/stringutils.h"


using namespace std;
namespace dfa {


static map<char, char> nsymb2symb = { { 'a', 'a' },{ 'c', 'c' },{ 'b', 'g' },{ 'd', 't' } };

struct Config{
    string path_grammar;
    string path_dir_tmp;
    string path_file_output;
    string path_file_outputh_graph;
};


static void find_all_min_strings(shared_ptr<Automata>& big, vector<int>& min_paths, int currentNode, list<string>& result) {
    result.clear();
    if (min_paths[currentNode] == 0) {
        result.push_back("");
        return;
    }
    for (int prevNode = 0; prevNode < big->state_count(); ++prevNode) {
        if (min_paths[prevNode] != min_paths[currentNode] - 1) {
            continue;
        }

        std::list<int> symbols;
        for (int i = 0; i < 4; ++i) {
            if (big->get_to_state(prevNode, i) == currentNode) {
                symbols.push_back(i);
            }
        }

        list<string> subresult;
        if (symbols.empty() == true) {
            continue;
        }

        find_all_min_strings(big, min_paths, prevNode, subresult);
        for (auto symb : symbols) {
            for (auto res : subresult) {
                result.push_back(res.append(1, nsymb2symb['a' + symb]));
            }
        }
    }
}

static void find_all_min_strings(shared_ptr<Automata>& big, list<string>& min_strings) {
    min_strings.clear();
    std::vector<int> min_paths(big->state_count(), std::numeric_limits<int>::max());
    min_paths[0] = 0;

    std::queue<int> states;
    std::set<int> viewed_states;
    states.push(0);
    viewed_states.insert(0);
    min_paths[0] = 0;
    while (states.empty() == false) {
        auto from = states.front();
        states.pop();
        for (int i = 0; i < 4; ++i) {
            int to = big->get_to_state(from, i);
            if (viewed_states.count(to) > 0)
                continue;
            min_paths[to] = min_paths[from] + 1;
            states.push(to);
            viewed_states.insert(to);
        }
    }

    // assert(big->terminal_states.size() == 1);
    for (int i = 0; i < big->state_count(); ++i) {
        if (big->is_terminal(i) == false)
            continue;

        list<string> subresult;
        find_all_min_strings(big, min_paths, i, subresult);
        min_strings.insert(min_strings.end(), subresult.begin(), subresult.end());
    }
}

static shared_ptr<Automata> generate_big_automata(std::shared_ptr<GrammarExprTree> root, std::map<std::string, std::shared_ptr<Automata>>& processed_items) {
    if (root->childs.empty()) {
        return processed_items.at(root->name);
    }
    std::shared_ptr<Automata> result = find_min_automata(generate_big_automata(root->childs[0], processed_items));
    for (int i = 1; i < root->childs.size(); ++i) {
        auto next = find_min_automata(generate_big_automata(root->childs[i], processed_items));
        if (root->op == '*') {
            result = find_min_automata(intesect_automata(result, next));
            continue;
        }
        if (root->op == '|') {
            result = find_min_automata(sum_automata(result, next));
            continue;
        }
    }
    return result;
}

static std::map<std::string, std::shared_ptr<Automata>> find_minimal_strings(std::map<string, set<string> > grammar){
    std::map<std::string, std::shared_ptr<Automata>> processed_items;
    for (auto it = grammar.begin(); it != grammar.end(); ++it) {
        if (it->first == "result") {
            continue;
        }
        cout << "reading rule " << it->first << std::endl;

        std::shared_ptr<Automata> result;
        int ind = 0;
        for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
            cout << "\treading subrule " << ind++ << std::endl;

            string temppath ="/tmp/temp.txt";
            string expr = *jt;
            std::string::iterator end_pos = std::remove(expr.begin(), expr.end(), ' ');
            expr.erase(end_pos, expr.end());
            RegEx re;
            re.Compile(expr);
            std::ofstream f1(temppath);
            re.Dump2Stream(f1);

            ifstream f2(temppath);
//            assert(f2.is_open() == true);


            auto temp = find_min_automata(Automata::read_from_stream(f2));

            // generate_automata_visualization_script(temp, "E:/projects/projects-git/dfalib/img/fsm-my.gv");

            if (jt == it->second.begin()) {
//                result = temp;
            } else {
//                result = find_min_automata(sum_automata(result, temp));
            }
            //cout << "\tCount of states in subrule: " << result->state_count() << std::endl;
        }
        processed_items[it->first] = result;

    }
    return processed_items;
}

static bool is_less(vector<int> v1, vector<int> v2 ){
    assert(v1.size()==v2.size());
    int i, im = v1.size();
    for(i=0; i< im; i++)
        if(v1[i]>v2[i])
            return false;
    return true;
}

static bool comp(int a, int b){
    return (a<b);
}

static int traverse_gquadruplex(string str, char ch, int last_pos, int last_group_id, unordered_map<int,int> groups){
    int MinSBetweenGroups = 3;
    int new_pos  = str.find(ch, last_pos+1);
    if(new_pos<0){
        int count[] = {0,0,0,0};
        for(auto m : groups){
            count[m.second]++;
        }
        return *min_element(count, count+4);
    }
    int result1 = traverse_gquadruplex(str,ch,new_pos,last_group_id, groups);
    int current_group_id =  last_group_id;
    groups.insert({new_pos,current_group_id});
    int result2 =  traverse_gquadruplex(str,ch,new_pos,current_group_id,groups);
    groups.erase(groups.begin());
    result1 =  max(result1,result2);
    int valcmp = groups.begin()->second;
    if(last_group_id<3 &&  new_pos-valcmp>MinSBetweenGroups){
        current_group_id = last_group_id+1;
        groups.insert({new_pos,current_group_id});
        result2 = traverse_gquadruplex(str,ch, new_pos, current_group_id,groups);
        groups.erase(groups.begin());
        result1 =  max(result1,result2);
    }
    return result1;
}

static int max_gquadruplex_strength(string str, char ch){
    int current_pos = str.find(ch);
    if(current_pos<0)
        return -1;

    int last_group_id = 0;
    unordered_map<int,int> groups = {{current_pos,last_group_id}};
    return traverse_gquadruplex(str,ch,current_pos,last_group_id,groups);
}

static int hairpin_strength(string hairpin, int tail_left){
    int strength = 0,
        tail_center_right = tail_left+5,
        tail_size = min(tail_left+1,(int)hairpin.length()-tail_center_right),
        i;
    for(i=0;i<tail_size;i++){
        if(hairpin[tail_left-i]=='a' && hairpin[tail_center_right+i]== 't')
            strength++;
        else if(hairpin[tail_left-i]=='c' && hairpin[tail_center_right+i]== 'g')
            strength++;
        else if(hairpin[tail_left-i]=='g' && hairpin[tail_center_right+i]== 'c')
            strength++;
        else return strength;
    }

    return strength;
}

static int max_hairpin_strength(string str){
    int cw = 0, cw_new, tail_index = 0, i, im = str.length();
    for(i=0; i< im; i++){
        cw_new = hairpin_strength(str,i);
        if(cw_new>cw){
            cw = cw_new;
            tail_index = i;
        }
    }
    return cw;
}
static bool is_triplex(string str, int  first_position, int second_position, int third_position, bool triplex_examples){
    if (third_position-second_position< 4) return false;
    if (second_position - first_position < 4) return false;
    if (!(first_position < second_position < third_position))return false;
    if ((first_position < 0) || (second_position < 0) || (third_position < 0)) return false;
    if ((first_position > str.length() - 1) ||
        (second_position > str.length() - 1) ||
        (third_position > str.length() - 1)) return false;
    string new_triplex = "" + str[first_position]+str[second_position]+ str[third_position];
    vector<string> tmpval = get_triplex_set(1);
    set<string> trps_set(tmpval.begin(),tmpval.end());
    return trps_set.find(new_triplex)==trps_set.end();
}

static int find_next_triplex(
        string str,
        int first_triplex_first_pos,
        int first_triplex_second_pos,
        int first_triplex_third_pos,
        bool triplex_examples){
    if(first_triplex_second_pos - first_triplex_first_pos < 4){return 0;}
    int max_strength = 0,
    strength = 0, l,
            another_triplex_first_position,
            another_triplex_second_position,
            another_triplex_third_positon;
    for(l=1; l < first_triplex_second_pos - first_triplex_first_pos; l++){
        another_triplex_first_position = first_triplex_first_pos + l;
        another_triplex_second_position = first_triplex_second_pos - l;
        another_triplex_third_positon = first_triplex_third_pos + l;
        if(is_triplex(
                str,
                another_triplex_first_position,
                another_triplex_second_position,
                another_triplex_third_positon,
                triplex_examples)){
            strength = 1 + find_next_triplex(
                    str,
                    another_triplex_first_position,
                    another_triplex_second_position,
                    another_triplex_third_positon,
                    triplex_examples);
            max_strength = max(max_strength,strength);
        }
    }
    return max_strength;
}

static int max_triplex_strength(string str, bool triplex_examples){
    int max_strength =0,strength,i,k,j,l=str.length();
    for(i=0; i< l; i++){
        for(j=0; j<l; j++){
            for(k=0 ;k<l; k++){
                vector<int> triplex_candidate = {i,j,k};
                sort(triplex_candidate.begin(),triplex_candidate.end());
                if(is_triplex(
                        str,
                        triplex_candidate[0],
                        triplex_candidate[1],
                        triplex_candidate[2],
                        triplex_examples )){
                    strength = 1;
                    strength +=   find_next_triplex(str, triplex_candidate[0], triplex_candidate[1],
                                                    triplex_candidate[2], triplex_examples);
                    max_strength = max(strength,max_strength);
                    return max_strength;
                }
            }
        }
    }
    return max_strength;
}

static vector<int> analyze_string(
        string string,
        bool find_GQD ,
        bool find_IMT ,
        bool find_TPR ,
        bool find_HRP ){

    vector<int> result = {};

    if(find_GQD){
        result.push_back(max_gquadruplex_strength(string,'g'));
    }else{
        result.push_back(-1);
    }

    if(find_IMT){
        result.push_back(max_gquadruplex_strength(string,'c'));
    }else{
        result.push_back(-1);
    }


    if(find_HRP){
        result.push_back(max_hairpin_strength(string));
    }else {
        result.push_back(-1);
    }

    if(find_TPR){
        result.push_back(max_triplex_strength(string,find_TPR));
    } else{
        result.push_back(-1);
    }

    return result;
}
}
#endif

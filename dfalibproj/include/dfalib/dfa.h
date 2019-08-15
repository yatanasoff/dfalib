#ifndef DFA_H
#define DFA_H

#include <vector>
#include <set>
#include <memory>
#include <fstream>
#include <bits/stdc++.h>
#include "regex/regex.h"

struct Automata {
    int n_value; // state count
    int l_value; // alphabet size

    std::set<int> terminal_states;


    Automata() {

    }

    int terminal_states_count() {
        return terminal_states.size();
    }

	int state_count() const {
        return n_value;
	}

    void init() {
        transitions.resize(l_value * n_value);
    }

    void set_transition(int from, int symb, int to) {
        transitions[from * l_value + symb] = to;
    }

    bool is_terminal(int state) const {
        return terminal_states.find(state) != terminal_states.end();
    }

    int get_to_state(int from, int symb) const {
        return transitions[from * l_value + symb];
    }

    static std::shared_ptr<Automata> read_from_stream(std::istream& in);

    void dump_to_stream(std::ofstream& out);
    void dump_to_string(std::string& result);

private:
    std::vector<int> transitions;
};


std::shared_ptr<Automata> sum_automata(const std::shared_ptr<Automata>& first_automata, std::shared_ptr<Automata>& second_automata);
std::shared_ptr<Automata> intesect_automata(const std::shared_ptr<Automata>& first_automata, std::shared_ptr<Automata>& second_automata);
std::shared_ptr<Automata> find_min_automata(const std::shared_ptr<Automata>& automata);
bool check_eq(const std::shared_ptr<Automata>& automata_first_, const std::shared_ptr<Automata>& automata_second_);

//void generate_automata_visualization_script(const std::shared_ptr<Automata>& automata, std::string filepath);

#endif

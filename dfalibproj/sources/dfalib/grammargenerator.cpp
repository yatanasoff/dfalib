#ifndef GRAMMARGENERATOR_CPP
#define GRAMMARGENERATOR_CPP

#include "dfalib/grammargenerator.h"
#include <string>
#include <vector>

using namespace std;





string GrammarGenerator::generate_gquadruplex(int count){
    return "X*  g{m}  X{3}X*  g{m}  X{3}X*  g{m}  X{3}X*  g{m}   X*,   m=[1:"+to_string(count)+"]";
}

string GrammarGenerator::generate_imotiv(int count_1, int count_2){
    return "X*  c{a}  X{3}X*  c{b}  X{3}X*  c{a}  X{3}X*  c{b}   X*,   a=[1:"+to_string(count_1)+"], b=[1:"+to_string(count_2)+"]";
}

vector<string> GrammarGenerator::cartesian_product(vector<string> data){
    vector<string> result = {};
    bool stop = data.size()==1;
    string string_val = data[0];
    if(stop){
        for(char char_val : string_val){
            result.insert(result.begin(),string(1,char_val));
        }
    }else{
        data.erase(data.begin());
        vector<string> partial_result = cartesian_product(data);
        for(char char_val_base : string_val){
            for(string partial_result_string : partial_result){
                result.insert(result.begin(),string(1,char_val_base)+partial_result_string);
            }
        }
    }

    return result;
};

vector<string> GrammarGenerator::generate_hairpins_from_set(vector<string> hrps){
    vector<string> result = {};
    unordered_map<char,char> opposite_chars = {};
    opposite_chars.emplace('a','t');
    opposite_chars.emplace('t','a');
    opposite_chars.emplace('c','g');
    opposite_chars.emplace('g','c');
    vector<string> cp = cartesian_product(hrps);
    int i,j, im=cp.size(), jm; string left_part, left_part_formatted, righ_part;
    for (i=0;i<im;i++) {
        left_part = cp[i];
        righ_part = "";
        left_part_formatted = "(";
        jm =left_part.size();
        for(j=0;j<jm;j++){
            char left_part_char= left_part[j];
            left_part_formatted += "'"+string(1,left_part_char)+"'"+(j==jm-1?"":", ");
            righ_part += opposite_chars[left_part_char];
        }

        left_part_formatted += ")";
        result.push_back("HRP_"+to_string(i+1)+" = X*  "+left_part_formatted+" X{3}X* "+righ_part+" X*\n");
    }

    return result;
}

vector<string> GrammarGenerator::get_triplex_set(int kind){
    if(kind)
        return {"tac","taa","tag","cgg","atg","cgt","cga","cgc","tat"};
    return {"cat","agc","cgc","gat","ggc","tgc","tat","gta","aat"};
}

std::string GrammarGenerator::create_grammar(
        bool find_GQD,
        bool find_IMT,
        bool find_TPR,
        bool find_HRP,
        vector<string> hrps,
        int min_size){

    string result = "";
    string names = {};


    if(find_TPR){
        vector<string> trps_list = get_triplex_set(1);
        int i, im = trps_list.size();
        names += "(";
        for (i=0;i<im;i++) {
            result += "TRP_"+ to_string(i+1) +
                    " = X* "+ trps_list[i][0] +
                    " X{3}X* "+ trps_list[i][1] +
                    " X{3}X* "+ trps_list[i][2] +
                    " X*\n";
            names += "TRP_"+to_string(i+1)+(i==im-1?")":" | ");
        }
        result += "\n";
        names+="\\\n\t";
    }

    if(find_GQD){
        result += "GQD = "+generate_gquadruplex(20)+"\n\n";
        names += "(GQD)\\\n\t";
    }

    if(find_IMT){
        result += "IMT = "+generate_imotiv(20,20)+"\n\n";
        names += "(IMT)\\\n\t";
    }

    if(find_HRP){
        vector<string> hairpins = generate_hairpins_from_set(hrps);
        names += "(";
        int i,im = hairpins.size(); string hairpin;
        for(i=0;i<im;i++){
            hairpin = hairpins[i];
            result += hairpin;
            names += hairpin.substr(0,hairpin.find('=')-1)+(i==im-1?")":" | ");
        }
        result+="\n";
        names+="\\\n\t";

    }
    names += "LENGTH";
    result += "LENGTH = ";
    int i;
    for(i=0;i<min_size;i++){
        result+="X";
    }

    result += "\n";
    result += "X = (a|c|g|t)\n";
    result += "result = " + names;
    return result;
}

#endif // GRAMMARGENERATOR_CPP

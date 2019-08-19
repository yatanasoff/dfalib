#ifndef GRAMMARGENERATOR_CPP
#define GRAMMARGENERATOR_CPP

#include "dfalib/grammargenerator.h"
#include <string>
#include <vector>
#include <bits/stdc++.h>

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

vector<string> GrammarGenerator::generate_hairpins(){
    vector<string> hrps_data = {"a", "c", "g", "t"}, result, perms;
    unordered_map<char,string> opposite_chars = {{'a',"t"},{'t',"a"},{'c',"g"},{'g',"c"}};
    do{
        perms.push_back(hrps_data[0]+hrps_data[1]+hrps_data[2]+hrps_data[3]);
    } while(next_permutation(hrps_data.begin(),hrps_data.end()));
    int index = 0;
    for(string hrp : perms){
        string s1= "";
        string s2 = "";
        for(char ch : hrp){
            s1 += string(1,ch) + "{"+ string(1,ch) +"}";
            s2 = opposite_chars[ch]+"{"+string(1,ch)+"}";
        }
        result.push_back("TRP"+to_string(index++)+" = X* "+s1+" X{4}X* "+s2+" X*, a=[1:1] c=[1:1] g=[1:1] t=[1:1]\n");
    }
    return result;
}

vector<string> GrammarGenerator::get_triplex_set(int kind){
    if(kind)
        return {"tac","taa","tag","cgg","atg","cgt","cga","cgc","tat"};
    return {"cat","agc","cgc","gat","ggc","tgc","tat","gta","aat"};
}


string GrammarGenerator::create_grammar(
        bool find_GQD,
        bool find_IMT,
        bool find_TPR,
        bool find_HRP,
        vector<string> hrps,
        int lenght)
{
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
        result += "IMT = "+generate_imotiv(10,10)+"\n\n";
        names += "(IMT)\\\n\t";
    }

    if(find_HRP){
        vector<string> hairpins;
        if(hrps.size()>0){
            hairpins = generate_hairpins_from_set(hrps);
        }else{
            hairpins = generate_hairpins();
        }
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

    if(lenght>0){
        names += "LENGTH";
        result += "LENGTH = ";
        int i;
        for(i=0;i<lenght;i++){
            result+="X";
        }
        result += "\n";
    }

    result += "X = (a|c|g|t)\n";
    result += "result = " + names;
    return result;
}

#endif // GRAMMARGENERATOR_CPP

#include "runner.h"

Runner::Runner(QObject *parent) : QObject(parent)
{

}

void Runner::doDFA(bool gqd, bool imt, bool trp, bool hrp, int min_len)
{

    DNALangParser grammarParser;
    GrammarGenerator grammarGenerator;
    Parser parser;

    std::string  grammar_str = grammarGenerator.create_grammar(gqd,imt,trp,0,{},min_len);
    emit sendText(QString::fromStdString(grammar_str));

    std::map<std::string, std::shared_ptr<Automata>> processed_items;
    std::map<string, set<string> > grammar = grammarParser.parseString(grammar_str);


    for (auto it = grammar.begin(); it != grammar.end(); ++it) {
        if (it->first == "result") {
            continue;
        }

        std::shared_ptr<Automata> result;
        int ind = 0;
        for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {

            string expr = *jt;
            std::string::iterator end_pos = std::remove(expr.begin(), expr.end(), ' ');
            expr.erase(end_pos, expr.end());
            RegEx re;
            re.Compile(expr);
            stringstream f1;
            re.Dump2Stream(f1);
            auto temp = find_min_automata(Automata::read_from_stream(f1));
            if (jt == it->second.begin()) {
                result = temp;
            } else {
                result = find_min_automata(sum_automata(result, temp));
            }
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

    std::shared_ptr<Automata> big = dfa::generate_big_automata(calculations_graph, processed_items);
    std::vector<string> min_strings;



    map<string, vector<int>> best_sequences = {};
    dfa::find_all_min_strings(big, min_strings);



    if (!min_strings.empty()) {
        const int limit = min_strings.size();
        emit sendText("total: "+QString::number(limit));
        string tmp;
        int i,j;
        for (i=0;i<limit; i++) {
qDebug()<<i<<endl;
            std::vector<int>  res = dfa::analyze_string( min_strings[i],gqd,imt,trp,hrp);
            best_sequences.insert({ min_strings[i], res});

            tmp += to_string(i)+min_strings[i]+":";
            for(j=0; j<4;j++){
                tmp+=to_string(res[j])+(j==3?"":",");
            }
            tmp +="\n";

            if(i%100==0){
                emit sendProgress(i,limit);
            }
        }
        emit sendResults(QString::fromStdString(tmp));
        emit sendText("done");
    }else{
        //show no data
    }
}

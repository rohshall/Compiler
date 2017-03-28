//
// Created by sabra on 26/03/17.
//

#include "bits/stdc++.h"
#include "Parser.h"
#include "Node.h"
#include "NFA.h"
#include "DFA.h"
using namespace std;


vector<string> lexical_rules;
map<string, string> def_map;
vector<string> keywords, punctuations;
map<string, string> exp_map;

string add_concatination_to_string(string a) {
    string ans;
    ans =+ a[0];
    for(int i = 1; i < a.size(); i++) {
        if(a[i] == '|' || a[i-1] == '|' || a[i-1] == '\\'){
            ans += a[i];
            continue;
        }
        if(a[i-1] == '('){
            ans += a[i];
            continue;
        }
        if(a[i] == ')' || a[i] == '@'){
            ans += a[i];
            continue;
        }
        if(a[i] == '-'|| a[i] == '+' || a[i] == '*'){
            ans += a[i];
            continue;
        }
        ans += '@';
        ans += a[i];
    }
    return ans;
}

void add_concatination_def(){
    map<string, string>substituted;
    for(auto const& x : def_map) {
        substituted[x.first] = add_concatination_to_string(x.second);
    }
    def_map = substituted;
}

void add_concatination_expression(){
    map<string, string>substituted;
    for(auto const& x : exp_map) {
        substituted[x.first] = add_concatination_to_string(x.second);
    }
    exp_map = substituted;
}



int strstr(string haystack, string needle) {
    int f[needle.size()];
    for(int i = 1 , k = 0 ; i < needle.size() ; i++){
        while(k > 0 && needle[i] != needle[k])
            k = f[k-1];
        if(needle[i] == needle[k])
            k++;
        f[i] = k;
    }
    for(int i = 0 , k = 0 ; i < haystack.length() ; i++){
        while(k>0 && haystack[i] != needle[k])
            k = f[k-1];
        if(haystack[i] == needle[k]){
            k++;
        }
        if(k == needle.length()){
            return i - k + 1;
        }
    }
    return -1;
}

string remove_spaces(string s){
    string s_without_space;
    size_t pos = 0, found;
    while((found = s.find_first_of(' ', pos)) != string::npos){
        s_without_space += s.substr(pos, found - pos);
        pos = found + 1;
    }
    s_without_space += s.substr(pos);
    return s_without_space;
}


void replaceToken(string &in, string &out){

}

void epsilon_closure(Node* node, set<int> &vis, vector<Node*> &inner_nodes, bool &isAccepting, int &type) {
    if(vis.find(node->id) != vis.end()) {
        return;
    }
    vis.insert(node->id);
    inner_nodes.push_back(node);
    isAccepting |= node->isAccepting;
    type = min(type, node->type);
    for(int i = 0; i < node->adjacent['\0'].size(); i++) {
        epsilon_closure(node->adjacent['\0'][i], vis, inner_nodes, isAccepting, type);
    }
}

DFA* convert_NFA_to_DFA(NFA* nfa) {
    map<set<int>, DFA*> visited_dfa;
    //set<int>types;
    set<int> vis;
    vector<Node*> inner_nodes;
    int type = 20;
    bool isAccepting = false;
    epsilon_closure(nfa->start, vis, inner_nodes, isAccepting, type);

    DFA* start_dfa = new DFA();
    start_dfa->inner_nodes = inner_nodes;
    start_dfa->type = type;
    start_dfa->isAccepting = isAccepting;
    visited_dfa[vis] = start_dfa;

    stack<DFA*> dfa_stack;
    dfa_stack.push(start_dfa);

    vis.clear();
    inner_nodes.clear();
    while(!dfa_stack.empty()) {
        DFA* dfa = dfa_stack.top();
        dfa_stack.pop();
        //if(dfa->isAccepting)types.insert(dfa->type);
        set<char> valid_char;
        for(int i = 0; i < dfa->inner_nodes.size(); i++) {
            for(auto x : dfa->inner_nodes[i]->adjacent) {
                if(x.first != '\0') {
                    valid_char.insert(x.first);
                }
            }
        }
        for(auto x : valid_char) {
            vis.clear();
            inner_nodes.clear();
            type = 20;
            isAccepting = false;
            for(int i = 0; i < dfa->inner_nodes.size(); i++) {
                if(dfa->inner_nodes[i]->adjacent.find(x) != dfa->inner_nodes[i]->adjacent.end()) {
                    for(auto y : dfa->inner_nodes[i]->adjacent[x]) {
                        epsilon_closure(y, vis, inner_nodes, isAccepting, type);
                    }
                }
            }
            if(visited_dfa.find(vis) != visited_dfa.end()) {
                dfa->adjacent[x] = visited_dfa[vis];
            } else {
                DFA* new_dfa = new DFA();
                new_dfa->inner_nodes = inner_nodes;
                new_dfa->isAccepting = isAccepting;
                new_dfa->type = type;
                dfa->adjacent[x] = new_dfa;
                visited_dfa[vis] = new_dfa;
                dfa_stack.push(new_dfa);
            }
        }
    }
    cout << visited_dfa.size() << endl;
    //cout << types.size() << endl;
    return start_dfa;
}

int match_dfa(DFA* dfa, string input) {
    for(int i = 0; i < input.size(); i++) {
        if(dfa->adjacent.find(input[i]) != dfa->adjacent.end()) {
            dfa = dfa->adjacent[input[i]];
        } else {
            cout << "5555555\n";
        }
    }
    if(dfa->isAccepting) {
        return dfa->type;
    }
    return -1;
}

void printDFA(DFA* dfa) {
    stack<DFA*> s;
    s.push(dfa);
    set<int>vis;
    ///if you want to print it in a file enter file_name in freopen(file_name, "w", stdout);
    int aaaaaa = 0;
    vector<char>hamada;
    while(!s.empty()) {
        DFA* aux = s.top();
        s.pop();
        if(vis.find(aux->id) != vis.end())continue;
        vis.insert(aux->id);
        for(auto x : aux->adjacent) {
            if(aux->id == x.second->id)aaaaaa++, hamada.push_back(x.first);
            cout << aux->id << " " << x.second->id << " " << x.first << endl;
            s.push(x.second);
        }
    }
    cout << aaaaaa << endl;
    for(auto x : hamada) {
        cout << x << endl;
    }
}

int main() {
    /*const char * filename = "home/sabra/ClionProjects/Compiler/Rules.txt";
    ifstream myFile;
    myFile.open(filename, ios::in);
    string line;
    if(myFile.is_open()){
        while(getline(myFile,line)){
            lexical_rules.push_back(line);
        }
    }else{
        cout << "Can not open the file" << endl;
    }
    myFile.close();*/

    lexical_rules = {"letter = a-z | A-Z",
                     "digit = 0-9",
                     "id: letter (letter | digit)*",
                     "digits = digit+",
                     "{boolean int float}",
                     "num: digit+ | digit+ . digits ( \\L | E digits)",
                     "relop: \\=\\= | !\\= | > | >\\= | < | <\\=",
                     "assign: =",
                     "{ if else while }",
                     "[; , \\( \\) { }]",
                     "addop: \\+ | -",
                     "mulop: \\* | /"

    };
    // parse
    def_map = Parser::parse_out_definitions(lexical_rules);
    exp_map = Parser::parse_out_expressions(lexical_rules);
    keywords = Parser::parse_out_keywords(lexical_rules);
    punctuations = Parser::parse_out_punctuations(lexical_rules);


    // substitute
    vector<pair<int, string>> list;
    for(auto const& entry : def_map) {
        string key = entry.first;
        list.push_back(make_pair(key.size(), key));
    }
    sort(list.begin(), list.end(), greater<pair<int, string>>());

    for(auto const& x : list){
        string key = x.second;
        for(auto const& y : def_map){
            string value = y.second;
            int found = strstr(value, key);
            while(found != -1){
                string str;
                str += value.substr(0,found);
                str += def_map[key];
                str += value.substr(found + key.size(),value.size());
                def_map[y.first] = str;
                found = strstr(str,key);
                value = y.second;
            }
        }
    }

    for(auto const& x : list) {
        string key = x.second;
        for (auto const& y : exp_map) {
            string value = y.second;
            int found = strstr(value, key);
            while (found != -1) {
                string str;
                str += value.substr(0, found);
                str += def_map[key];
                str += value.substr(found + key.size(), value.size());
                exp_map[y.first] = str;
                found = strstr(str, key);
                value = y.second;
            }
        }
    }
    //////////////////////

    // remove any space after substitute
    for(auto const& entry : exp_map){
        exp_map[entry.first] = remove_spaces(entry.second);
    }

    add_concatination_def();
    add_concatination_expression();
    for(int i = 0; i < keywords.size(); i++) {
        keywords[i] = add_concatination_to_string(remove_spaces(keywords[i]));
        if(keywords[i][0] == '\0') {
            continue;
        }
        cout << keywords[i] << endl;
    }

    // print to test here
    cout << "def map" << endl;
    for (auto const& x : def_map)
    {
        cout << x.first << " : " << x.second << endl ;
    }

    cout << "exp map" << endl;
    for (auto const& x : exp_map)
    {
        cout << x.first << " : " << x.second << endl ;
    }

    //Testing evaluating expression
    NFA* final_nfa = NULL;
    NFA* dummy = new NFA(new Node(0), new Node(0));
    int expression_num = 2;

    // maps between expression_num and the corresponding label
    map<int, string> mp;
    for(auto x : exp_map) {
        cout << x.second << endl;
        mp[expression_num] = x.first;
        NFA* aux = dummy->evaluate_expression(x.second, expression_num++);
        cout << endl;
        if(final_nfa == NULL) {
            final_nfa = aux;
        } else {
            final_nfa = dummy->NFA_or(final_nfa, aux, 20);
        }
    }

    for(auto x : punctuations) {
        NFA* aux = dummy->evaluate_expression(x, 1);
        final_nfa = dummy->NFA_or(final_nfa, aux, 20);
    }
    for(auto x : keywords) {
        if(x[0] == '\0') {
            continue;
        }
        NFA* aux = dummy->evaluate_expression(x, 0);
        final_nfa = dummy->NFA_or(final_nfa, aux, 20);
    }

    // transfer NFA to DFA

    DFA* dfa_not_minimized = convert_NFA_to_DFA(final_nfa);

    //printDFA(dfa_not_minimized);
    // minimize DFA
    // enter code
    string in = "while";
    int t = match_dfa(dfa_not_minimized, in);
    if(t == 0 || t == 1) {
        cout << in;
    } else {
        cout << mp[t];
    }
    return 0;
}

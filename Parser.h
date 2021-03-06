//
// Created by sabra on 26/03/17.
//

#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H


using namespace std;

class Parser {
public:
    static map<string, string> parse_out_definitions(vector<string> &);

    static map<string, string> parse_out_expressions(vector<string> &);

    static vector<string> parse_out_keywords(vector<string> &);

    static vector<string> parse_out_punctuations(vector<string> &);

    static inline string left_trim(string &);

    static inline string right_trim(string &);

    static inline string trim(string &);

    static inline string extract(string);


};


#endif //COMPILER_PARSER_H

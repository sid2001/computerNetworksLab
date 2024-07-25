/*
 * =====================================================================================
 *
 *       Filename:  lexicalAnalyser.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/18/24 16:30:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <iostream>
#include <unordered_set>
#include <regex>
#include <string>
using namespace std;

#define MAX_TOKEN_COUNT 100

unordered_set<string> keywords = {
    "int", "float","double","char","byte","short","string","void","long","for","while","if","main","printf","cout"
};
unordered_set<string> special_symbols = {
    "(",")","{","}",";","[","]"
};

unordered_set<string> operators = {"=","==","<",">","<=",">="};

unordered_set<string> whitespaces = {"\t","\n"," "};

vector<string> kw;
vector<string> constants;
vector<string> literals;
vector<string> special_symbols;
vector<string> opt;
vector<string> indentifier;
vector<string> unknown;

int isDelimiter(char ch){
    if(ch== ' ' || ch == '\n')
        return 1;
    else 
        return 0;
}

void display(string arr[]){
    for(int i = 0;i<MAX_TOKEN_COUNT;i++){
        std::cout << arr[i]<<"\n";
    }

    return;
}

string tokenType(char w){
    
}

void tokenParser(FILE *fp,string tokens[]){
    int indx = 0;
    char c; 
    string token = "";
    int tcount = 0;
    while((c = getc(fp)) != EOF){
        if(whitespaces.find(c)!=whitespaces.end()) {
            if(token != "")
                unknown.push_back(token);
            token = ;
            continue;
        }
        if(special_symbols.find(to_string(token))!=special_symbols.end()){
            special_symbol.push_back(token);
            token = "";
            tcount ++;
        } else if(operators.find(to_string(token))!=operators.end()){
            opt.push_back(token);
            token = "";
            tcount++;
        } else if(keywords.find(to_string(token))!=keywords.end()){
            kw.push_back(token);
            token = "";
            tcount++;
        } else if(whitespaces.find(c)!=whitespaces.end()) {
            if(token!="")
                unknown.push_back(token);
            token = "";
            continue;
        }
    return;
}

int main(int argc, char *argv[]){
    if(argc<=1){
        printf("No input file provided.");
        printf("Type --help for more details");
        return 0;
    }
    FILE *fp;
    while(--argc>0){
        if((fp = fopen(*++argv,"r"))==NULL){
            printf("can't open %s\n",*argv);
            return 1;
        } else {
            string tokens[MAX_TOKEN_COUNT];
            tokenParser(fp,tokens);
            display(tokens);
        }
    }
        
    return 0;
}


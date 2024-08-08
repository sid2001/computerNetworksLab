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
    "auto","const","Struct","unsigned","break","else","continue","switch","signed","case","enum","register","default","typedef","goto","sizeof","volatile","extern","return","do","union","static","int", "float","double","char","byte","short","string","void","long","for","while","if","main","printf","cout"
};
unordered_set<char> special_symbols = {
    '(',')','{','}',';',','
};

unordered_set<char> operators = {'=','+','-'};

//undordered_set<string> lit = {"%d"};

unordered_set<string> whitespaces = {"\t","\n"," "};

vector<string> kw;
vector<string> constants;
vector<string> lit;
vector<string> ss;
vector<string> opt;
vector<string> identifiers;
vector<string> unknown;


int isDelimiter(char ch){
    if(ch== ' ' || ch == '\n')
        return 1;
    else 
        return 0;
}

void display(vector<string> arr){
    for(int i = 0;i<arr.size();i++){
        std::cout << arr[i]<<",";
    }
    cout<<"\n";
    return;
}

void tokenParser(FILE *fp,string tokens[]){
    int indx = 0;
    char c; 
    string token = "";
    int tcount = 0;
    bool lf = false;
    cout<<*keywords.find("main")<<"\n";
    while((c = getc(fp)) != EOF){
        //out<<<<"\n";
        //cout<<token<<"\n";
        if(lf==true){
            token += to_string(c);
            continue;
        }
        if(to_string(c)==to_string('"')){
            if(lf){
                lit.push_back(token);
                token = "";
                tcount++;
                lf = false;
            }else
                lf = true;

            continue;
        }
        if(special_symbols.find(c)!=special_symbols.end()){
            if(token != ""){
                if(keywords.find(token) != keywords.end()){
                    kw.push_back(token);
                    token += c;
                    token = "";
                }else identifiers.push_back(token);
                tcount++;
            }
            ss.push_back(to_string(c));
            tcount++;
            token = "";
        } else if(c == ' ') {
            if(token != ""){
                if(keywords.find(token) != keywords.end()){
                    kw.push_back(token);
                    token += c;
                    token = "";
                }else identifiers.push_back(token);
                tcount++;
            }
            tcount++;
            token = "";
            continue;
        } else if(operators.find(c)!=operators.end()){
            if(token != ""){
                if(keywords.find(token) != keywords.end()){
                    kw.push_back(token);
                    token += c;
                    token = "";
                }else 
                    identifiers.push_back(to_string(c));
                tcount++;
            }
            tcount++;
            opt.push_back(to_string(c));
            token = "";
        } else if(keywords.find(token) != keywords.end()){
            kw.push_back(token);
            token += c;
            token = "";
            tcount++;
         } 
        //else if(c=='"') {
        //    if(lf){
        //        lit.push_back(token);
        //        token = "";
         //       tcount++;
         //       lf = false;
         //   }else
         //       lf = true;
        //} 
        else {
            token += c;
        }
       // if(whitespaces.find(c)!=whitespaces.end()) {
       //     if(token != "")
       //         unknown.push_back(token);
       //     token = ;
       //     continue;
       // }
       // if(special_symbols.find(to_string(token))!=special_symbols.end()){
       //     special_symbol.push_back(token);
       //     token = "";
       //     tcount ++;
       // } else if(operators.find(to_string(token))!=operators.end()){
       //     opt.push_back(token);
       //     token = "";
       //     tcount++;
       // } else if(keywords.find(to_string(token))!=keywords.end()){
       //     kw.push_back(token);
       //     token = "";
       //     tcount++;
       // } else if(whitespaces.find(c)!=whitespaces.end()) {
       //     if(token!="")
       //         unknown.push_back(token);
       //     token = "";
       //     continue;
       // }
    }
    cout<<"keywords: ";
    display(kw);
    cout<<"special symbols: ";
    display(ss);
    cout<<"lit: ";
    display(lit);
    cout<<"identifiers: ";
    display(identifiers);
    cout<<"operators: ";
    display(opt);
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
        }
    }
        
    return 0;
}


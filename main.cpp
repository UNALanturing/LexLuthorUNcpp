#include<bits/stdc++.h>
#include"AFD.h"
using namespace std;

int main(){
	freopen("input.l", "r", stdin);
	freopen("output.cpp", "w", stdout);
	map <string, string> exp;
	vector<pair<string, string>> rules;
	string current_line;
	bool inDeclarations = true;
	bool inDefinitions = false;
	bool inRules = false;
	cout << "#include\"AFD.h\"" << endl;
	while(getline(cin, current_line)){
		if(inDeclarations){//Reading declarations section.
			if(current_line != "%{" && current_line != "%}") cout << current_line << endl;
			if(current_line == "%}"){ 
				inDeclarations = false;
				inDefinitions = true;
			}
		}else if(inDefinitions){//Reading definitions section.
			if(current_line == "%%"){
				inDefinitions = false;
				inRules = true;
			}
			else{
				string ti = "", ri = ""; //Ti: Lexeme, and Ri: Regular expresion
				int n = current_line.size(), flag = 0;
				for (int i = 0; i < n; ++i) {
					if (!flag) {
						if (current_line[i] == ' ') flag = 1;
						else ti += current_line[i];
					} else if (flag == 1){
						if (current_line[i] == ' ') {
							if (!ri.empty()) {
								flag = 2;
								exp[ti] = ri;
								break;
							}
						} else ri += current_line[i];
					}
				}
				if (!exp.count(ti) && ti != "") exp[ti] = ri; //Saves the current lexeme and its regular expression	
			}
		}else if(inRules){//Reading rules section.
			//cout << "CL: " << current_line <<'\n';
			if(current_line == "%%"){
				inRules = 0;
				//cout << "freopen(\"input.txt\", \"r\", stdin);\n";
				//cout << "freopen(\"token_list.txt\", \"w\", stdout);\n";
				cout << "string yytext;\n";
				cout << "int yylval;\n";
				cout << "int yylength;\n";
				cout << "//Vector de automatas:\n";
				cout << "int num_automatas = " << rules.size() << ";\n";
				cout << "vector<AFD> automatas;\n";
				cout << "//Accion ingresada para cada regla:\n";
				int j = 0;
				for(auto x : rules){
					if(x.second.find("return") != string::npos){
						cout << "int action" << j++ <<"(){\n";
						cout << '\t' << x.second.substr(1,x.second.size()-2) << '\n';
						cout << "}\n\n";
					}else{
						cout << "void action" << j++ <<"(){\n";
						cout << '\t' << x.second.substr(1,x.second.size()-2) << '\n';
						cout << "}\n\n";
					}
				}
				cout << "//Analizador lexico:\n";
				cout << "vector<pair<int,string>> yylex(){\n";
				for(auto x : rules){
					cout << "\tautomatas.push_back( buildAFD( \"" << x.first << "\" ) );\n";
				}
				cout << "\tstring lexema;\n";
				cout << "\tvector<pair<int,string>> tokens;\n";
				cout << "\twhile(cin >> lexema){\n";
				cout << "\t\tyytext = lexema;\n";
				cout << "\t\tyylength = lexema.size();\n";
				j=0;
				for(auto x : rules){
					cout << "\t\t"<< (j ? "else ": "") << "if(automatas["<< j <<"].procesarString(lexema)){\n";
					if(x.second.find("return") == string::npos){
						cout << "\t\t\taction"<< j <<"();\n";
					}else{
						cout << "\t\t\ttokens.push_back({action"<< j <<"(), lexema});\n";
					}
					cout << "\t\t}\n";
					j++;
				}
				cout << "\t}\n";
				cout << "\treturn tokens;\n";
				cout << "}\n";
			}else{
				string pattern = "", action = "", def;//Token pattern and action.
				int n = current_line.size(), flag = 0, id = 0, stri = 0;
				for(int i=0; i<n; i++){
					if(!flag){
						if(stri){
							if(current_line[i] == '"') stri = 0;
							else pattern += current_line[i];
						}
						else if(current_line[i] == ' ') flag = 1;
						else if(current_line[i] == '"') stri = 1;
						else if(id){
							if(current_line[i] == '}'){
								id = 0;
								try{
									if(exp.count(def)) pattern += exp[def];
									else throw 1;	
								}catch(int e){
									cout << def << "is an invalid definition" << '\n';
								}
								
							}else{
								def += current_line[i];
							}
						}else{
							if(current_line[i] == '{'){
								id = 1;
								def = "";
							}else{
								
								pattern += current_line[i];
							}
						}
					}else{
						action += current_line[i];
					}
				}
				trim(action);
				//cout << "P: " << pattern << " A: " << action << '\n';
				if(pattern != "") rules.push_back(make_pair(pattern, action)); //Saves the current lexeme and its regular expression
			}
			
		}else{//Reading last section
			cout << current_line << endl;
		}
	}
	/*
	cout << "Definitions" << '\n';
	for(auto x : exp) cout << x.first << " " << x.second << '\n';
	cout << "Rules" << '\n';
	for(auto x : rules) cout << x.first << " " << x.second << '\n';
	*/
	return 0;
}
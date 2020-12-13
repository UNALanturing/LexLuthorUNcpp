#include <bits/stdc++.h>
#include "functions.h"
using namespace std;

void imp() {cout << "You've made a mistake sir / ma'am" << endl; exit(0);}
bool change(int i, int n, string &s) {return i + 1 < n && (s[i + 1] == '?' || s[i + 1] == '$'); }
bool isOperator(char c){
	return (c == '?' or c == '+' or c == '-' or c == '*' or c == '/' or c=='=' or c=='&' or c=='^' or c=='|');
}
string solve(pair<string,string> li){
	string ans = "", s = li.second, last = "";
	int n = s.size();
	int inStr = 0;
	vector < pair<string, int> > st;
	for (int i = 0; i < n; ++i) {
		//New expression, list or string
		if (s[i] == '(') {
			ans += "(";
			st.push_back({"", 1});
		} else if (s[i] == '[') {
			ans += "(";
			st.push_back({"", 2});
		} else if (s[i] == '"') {
			if (st.size() && st.back().second == 3) {
				if (!change(i, n, s)) {
					ans += st.back().first;
					st.pop_back();
				}
			} else {
				ans += "(";
				st.push_back({"", 3});
				inStr = 1;
			}
		} else {
			// Ending of an expression or list
			if(inStr && isOperator(s[i])){
				string aux = "";
				aux.push_back('\');
				aux.push_back(s[i]);
				ans += aux;
				continue;
			}
			if(s[i] == '+'){
				char aux = ans.back();
				if(aux != ')'){
					ans += aux + "*";
				}else{
					int cnt = 1;
					int inx = 0;
					if(i-2 < 0) imp();
					for(int j=i-2; j>=0; j--){
						if(ans[j] == ')')cnt++;
						else if(ans[j] == '(')cnt--;
						if(cnt == 0){
							inx = j;
							break;
						}
					}
					string t = ans.substr(inx,i-1-inx+1);
					ans += t+"*";
				}
				continue;
			}
			if (s[i] == ')' || s[i] == ']') {
				if (st.empty()) imp();
				if (!change(i, n, s)) {
					ans += st.back().first + ")";
					st.pop_back();
				}
			} else {
				//Current element in an operator
				if (s[i] == '?') {
					if (!i || ans.empty() || st.empty()) imp();
					if (st.size()) {
						ans += st.back().first + "|" + LAMBDA+ ")";
						st.pop_back();
					} else {
						char c = ans.back();
						ans += "(" ;
						ans += LAMBDA + "|";
						ans += c + ")";
					}
				} else if (s[i] == '$') {
					if (!i || ans.empty() || st.empty()) imp();
					if (st.size()) {
						last = st.back().first;
						st.pop_back();
					} else last = ans.back();
				} else if (s[i] == '-') {
					if (i == 0 || i + 1 >= n) imp();
					if (st.size()) {
						for (char c = s[i - 1] + 1; c <= s[i + 1]; ++c) {
							st.back().first += '|';
							st.back().first += c;
						}
					} else {
						for (char c = s[i - 1] + 1; c <= s[i + 1]; ++c) {
							ans += '|';
							ans += c;
						}
					}
					i++;
				} else {
					if (st.size()) {
						if (st.back().second == 2) {
							if (i != 0 && s[i - 1] != '[')
								st.back().first += '|';
							st.back().first += s[i];
						} else st.back().first += s[i];
					} else ans += s[i];
				}
			}
		}
	}
	cout << li.first << ' ' << ans << endl;
	li.second = ans;
	return ans;
}
int main() {
	freopen("input.txt", "r", stdin);
	freopen("parteC.txt", "w", stdout);
	map <string, string> exp;
	int token = 0;
	string s;
	//Extracting Lexemes and Regular extentions for further processing
	while (getline(cin, s)) {
		int n = s.size();
		if (!token) {
			cout << s << endl; //Reads the current line
			for (int i = 0; i + 1 < n; ++i) {
				if (token != 1) {
					if (s[i] == '%' && s[i + 1] == '}') { //End of first section
						token = 1; 
						fclose(stdout);
						freopen("funcionesC.txt.", "w", stdout);
						break; 
					}
				} 
			}
		} else {
			string ti = "", ri = ""; //Ti: Lexeme, and Ri: Regular expresion
			int n = s.size(), flag = 0;
			for (int i = 0; i < n; ++i) {
				if (i + 1 < n && s[i] == s[i + 1] && s[i] == '%') {
					token = 0;
					break;
				}
				if (!flag) {
					if (s[i] == ' ') flag = 1;
					else ti += s[i];
				} else if (flag == 1){
					if (s[i] == ' ') {
						if (!ri.empty()) {
							flag = 2;
							exp[ti] = ri;
							break;
						}
					} else ri += s[i];
				}
			}
			if (!exp.count(ti)) exp[ti] = ri; //Saves the current lexeme and its regular expression
		}
	}
	//fclose(stdout);
	//Converts Regular extension to regular expension, in order to further process using automatons
	//cout << "IN" << endl;
	for (auto &li : exp) {//For each Lexeme saved
		/* Cur states
		 * 0: just a char
		 * 1: in a expression ()
		 * 2: in a list []
		 * 3: in a string ""
		 */
		string res = solve(li);
		li.second = res;
	}

	//Arbol sintactico del ejemplo 3.56 del libro de Aho
	//Hojas
	Node h1('a',1);
	Node h2('b',2);
	Node h3('a',3);
	Node h4('b',4);
	Node h5('b',5);
	Node h6('#',6);
	//Nodos internos
	Node n7('|',&h1,&h2);
	Node n8('*',&n7);
	Node n9(CONCAT,&n8, &h3);
	Node n10(CONCAT,&n9,&h4);
	Node n11(CONCAT,&n10,&h5);
	Node root(CONCAT,&n11,&h6);
	printInOrder(&root);
	cout << '\n';
	
	AFD automata(root);
	automata.info();
	automata = minAFD(automata);
	automata.info();
	//cout << automata.procesarString("aab#") << '\n';
	//Node* root1 = parseRegex("(a|b)*abb#");
	//Node* root = new Node('|', new Node('a',1), new Node('b',2));
	//cout << p -> right -> simbol << '\n';
	//printInOrder(root1);
	cout << '\n';
	return 0;
}


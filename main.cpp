#include <bits/stdc++.h>
using namespace std;

const string lambda = "~";
void imp() {cout << "You've made a mistake sir / ma'am" << endl; exit(0);}
bool change(int i, int n, string &s) {return i + 1 < n && (s[i + 1] == '?' || s[i + 1] == '$'); }
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
		string ans = "", s = li.second, last = "";
		int n = s.size();
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
				}
			} else {
				// Ending of an expression or list
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
							ans += st.back().first + "|" + lambda + ")";
							st.pop_back();
						} else {
							char c = ans.back();
							ans += "(" + lambda + "|" + c + ")";
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
	}
	return 0;
}


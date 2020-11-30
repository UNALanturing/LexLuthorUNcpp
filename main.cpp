#include <bits/stdc++.h>
using namespace std;

int main() {
	freopen("input.txt", "r", stdin);
	freopen("parteC.txt", "w", stdout);
	map <string, string> exp;
	int token = 0;
	string s;
	while (getline(cin, s)) {
		int n = s.size();
		if (!token) {
			cout << s << endl;
			for (int i = 0; i + 1 < n; ++i) {
				if (token != 1) {
					if (s[i] == '%' && s[i + 1] == '}') {
						token = 1; 
						fclose(stdout);
						freopen("funcionesC.txt.", "w", stdout);
						break; 
					}
				} 
			}
		} else {
			string ti = "", ri = "";
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
			if (!exp.count(ti)) exp[ti] = ri;
		}
	}
	for (auto u: exp) cout << u.first << ' ' << u.second << endl;
	return 0;
}


#include "AFD.h"
#include <bits/stdc++.h>

AFD minAFD(AFD aut) {
	int n = aut.getQ(); //number of nodes in the original automaton
	vector <int> father(n, -1); //Representative node of each partition
	map <vector <int>, vector <int> > pi; //Current partitions (1 acceptance and 0 non acceptance)
	for (int i = 0; i < n; ++i) {
		vector <int> aux = {aut.f[i]};
		pi[aux].push_back(i);
		father[i] = pi[aux][0];
	}
	while (1) { //While there are new partitions
		map <vector<int>, vector <int>> newPar; //Every new partition formed
		vector < vector <int> > tran (n, vector <int> (aut.alp)); //transition to representative node
		for (auto g: pi) { //for each group G in PI (current paritition)
			for (int s : g.second) { // for every node in group G
				for (int i = 0; i < alp.size() - 1; ++i) { //check transition with every char in alphabet
					tran[s][i] = father[aut.table[s][i]]; 
				}
				newPar[tran[s]].push_back(s);
				father[s] = newPar[tran[s]][0];
			}
		}
		if (newpi == pi) break;
		pi = newpi;
	}
	//PI contains all groups of equivalent states
	int s = father[aut.s]; //New initial state is the representative node of initial state's group
	int q = pi.size(); //Size of PI equals number of minimum states
	vector <int> f(q, 0);//New vector of acceptance states
	for (int i = 0; i < q; ++i) f[i] = aut.f[father[i]]; //Acceptance states remin as accepted states
	vector < vector <int> > table(q, vector <int> (aut.alph.size())); //New table of transitions
	int idx = 0; //current state's index
	for (auto s : pi) { //
		for (int i = 0; i < alp.size() - 1; ++i) { //Adding new transitions;
			table[idx][i] = father[aut.table[s.second[0]][i]];
		}
		idx++;
	}
	return AFD(s, q, aut.alp, f, table); //Return new minimum states automaton
}

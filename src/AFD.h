#include <bits/stdc++.h>

using namespace std;
class AFD{
	public:
		int s; //Start state -- > int
		int q;//States --> int
		vector<char> alp;//Alphabet <-- is ordered
		vector<int> f;//Accepted states
		vector<vector<int>> table;//Graph --> vv
		AFD(int S, int Q, vector<char> A, vector<int> F, vector<vector<int>> T){//Build function
			s = S;
			q = Q;
			alp = A;
			f = F;
			table = T;
		}
		//Getters and Setters
		int getS(){
			return s;
		}
		void setS(Node S){
			s = S;
		}
		int getQ(){
			return q;
		}
		void setQ(int Q){
			q = Q;
		}
		vector<char> getA(){
			return alp;
		}
		void setA(vector<char> A){
			alp = A;
		}
		vector<int> getF(){
			return f;
		}
		void setF(vector<int> F){
			f = F;
		}
		vector<vector<int>> getT(){
			return table;
		}
		void setT(vector<vector<int>> T){
			table = T;
		}
		bool isAFD(string word){
			int u = s;
			int i = 0;
			while(true){
				if(s.size() == i) return f[u];
				int inx = lower_bound(alp.begin(),alp.end(),s[i++])-alp.begin();
				u = table[u][inx];
				if(u == -1) return 0;
		}
};

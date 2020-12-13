#include <bits/stdc++.h>

using namespace std;
class AFD{
	public:
		int s; //Start state -- > int
		int q;//States --> int
		vector<char> alp;//Alphabet
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
		bool isAFD(string word){//Have to be changed, but not today XD, srry Scrum Master :(
			Node u = s;//First node
			int i=0;
			while(1){
				if(i == word.size()){//Finish the string, analize if the node is Accepted
					return (f[u.getU()]);
				}
				vector<int> tran = u.getE();
				int inx = lower_bound(alp.begin(),alp.end(),s[i])-alp.begin();
				if(tran[inx]==-1)// -1 -> null -> There is no node
					return 0;
				u = table[tran[inx]];//node u goes to node tran[inx] with character 'inx' in the alphabet
				i++;
			}
		}
};

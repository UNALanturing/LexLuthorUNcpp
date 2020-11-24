#include "node.h"
class AFD{
	public:
		Node s; //Start state
		vector<int> q;//States
		vector<char> alp;//Alphabet
		vector<int> f;//Accepted states
		vector<Node> table;//Graph
		void build(Node S, vector<int> Q, vector<char> A, vector<int> F, vector<Node> T){//Build function
			s = S;
			q = Q;
			alp = A;
			f = F;
			table = T;
		}
		//Getters and Setters
		Node getS(){
			return s;
		}
		void setS(Node S){
			s = S;
		}
		vector<int> getQ(){
			return q;
		}
		void setQ(vector<int> Q){
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
		vector<Node> getT(){
			return table;
		}
		void setT(vector<Node> T){
			table = T;
		}
		bool isAFD(string word){
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

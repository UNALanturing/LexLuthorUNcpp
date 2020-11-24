#include <node.h>
class AFD{
	public:
		node s; //Start state
		vector<int> q;//States
		vector<char> alp;//Alphabet
		vector<int> f;//Accepted states
		vector<node> table;//Graph
		void build(node S, vector<int> Q, vector<char> A, vector<int> F, vector<node> T){//Build function
			s = S;
			q = Q;
			alp = A;
			f = F;
			table = T;
		}
		//Getters and Setters
		node getS(){
			return s;
		}
		void setS(node S){
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
		vector<node> getT(){
			return table;
		}
		void setT(vector<node> T){
			table = T;
		}
		bool isAFD(string word){
			node u = s;//First node
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

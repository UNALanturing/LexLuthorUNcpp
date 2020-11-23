class Node{
	public:
		int u;//Id node
		vector<int> edges;
		void build(int U,vector<int> E){
			edges = E;
			u = U;
		}
		vector<int> getE(){
			return edges;
		}
		void setE(vector<int> E){
			e = E;
		}
		int getU(){
			return u;
		}
		void setU(int U){
			u=U;
		}
};
		

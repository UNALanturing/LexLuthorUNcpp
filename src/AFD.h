class AFD{
	public:
		int s; //Start state
		vector<int> q;//States
		vector<char> alp;//Alphabet
		vector<int> f;//Accepted states
		map<pair<int,int>,char> table;//Graph
		void build(int S, vector<int> Q, vector<char> A, vector<int> F, map<pair<int,int>,char> T){//Build function
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
		void setS(int S){
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
		map<pair<int,int>,int> getT(){
			return table;
		}
		void setT(map<pair<int,int>,char> T){
			table = T;
		}
};

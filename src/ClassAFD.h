class ClassAFD{
	public:
		int whatAFD(string s, vector<AFD> A){//string s, vector of AFDs
			for(int i=0; i<A.size(); i++){
				bool f = A[i].isAFD(s);
				if(f){
					return i;
				}
			}
		}
};

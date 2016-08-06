#include "searchServer.h"

//#define DEBUGCLU

extern vector<struct INodeType> docVec[DOCNUM];

float dot(int d1, int d2){
	int i = 0, j = 0;
	float simv = 0;
	while(i < docVec[d1].size() && j < docVec[d2].size()){
		if(docVec[d1][i].termID == docVec[d2][j].termID)
			simv += docVec[d1][i++].vn * docVec[d2][j++].vn;
		else if(docVec[d1][i].termID < docVec[d2][j].termID) i++;
		else j++; 
	}
	return simv;
}

float vec[TERMNUM];

float Sim(vector<int> c1, vector<int> c2){

	float sumV; 

	for(int i = 0; i < TERMNUM; i++)
		vec[i] = 0.0;

	int docID;
	for(int i = 0; i < c1.size(); i++){
		docID = c1[i];
		for(int j = 0; j < docVec[docID].size(); j++)
			vec[docVec[docID][j].termID] += docVec[docID][j].vn;
	}
	for(int i = 0; i < c2.size(); i++){
		docID = c2[i];
		for(int j = 0; j < docVec[docID].size(); j++)
			vec[docVec[docID][j].termID] += docVec[docID][j].vn;
	}
/*
	float vlen = 0;
	for(int i = 0; i < TERMNUM; i++)
		vlen += vec[i] * vec[i];
*/
	sumV = 0;
	for(int i = 0; i < TERMNUM; i++)
		sumV += vec[i] * vec[i];
	
	float ret = (1.0/((c1.size()+c2.size())*(c1.size()+c2.size()-1)))*(sumV-(c1.size()+c2.size()));
	
	return ret;
}

vector<vector<int> > GAAClustering(vector<struct SearchResultNode> docs, int topN, float simtr, int cN){
	float C[300][300];
	bool act[300];
	vector<int> clusters[300];
//	priority_queue<struct CMatrixType> que[500];
	for(int i = 0; i < topN; i++){
		for(int j = i+1; j < topN; j++){
				C[i][j] = dot(docs[i].docID, docs[j].docID);
				C[j][i] = C[i][j];
			}
		act[i] = true;
		clusters[i].push_back(docs[i].docID);
	}
	
	int clusterN = topN;
	int k1,k2;
	for(int i = 0; i < topN - 1; i++){
		
	//	if(clusterN <= cN) break;

		float maxsim = 0, minsim = 99999.99;
		for(int j = 0; j < topN; j++)
			if(act[j]) 
				for(int k = 0; k < topN; k++)
					if(act[k]){
						if(C[j][k] > maxsim){
							maxsim = C[j][k];
							k1 = j;
							k2 = k;
						}
						if(C[j][k] < minsim)
							minsim = C[j][k];
					}	
			
#ifdef DEBUGCLU
		printf("Cluster round %d: maxsim %f, minsim %f\n", i, maxsim, minsim);
#endif

		if(maxsim < simtr && clusterN <= cN) break;

		for(int j = 0; j < clusters[k2].size(); j++)
			clusters[k1].push_back(clusters[k2][j]);

		act[k2] = false;
		clusters[k2].clear();

		for(int j = 0; j < topN; j++)
			if(j != k1 && act[j] == true){
				C[k1][j] = Sim(clusters[k1], clusters[j]);
				C[j][k1] = C[k1][j];
			}

		clusterN--;
	}

	vector<vector<int> > retVec;
	for(int i = 0; i < topN; i++)
		if(act[i])
			retVec.push_back(clusters[i]);

#ifdef DEBUGCLU
	for(int i = 0; i < retVec.size(); i++){
		printf("Cluster_%d: ", i);
		for(int j = 0; j < retVec[i].size(); j++)
			printf("%d ", retVec[i][j]);
		printf("\n");
	}
#endif
	
	return retVec;
}

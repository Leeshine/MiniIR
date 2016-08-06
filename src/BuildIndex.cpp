#include "searchServer.h"

#define DEBUGINDEX

extern RIHeadType dict[TERMNUM];
extern vector<struct INodeType> docVec[DOCNUM];
extern int termN;
extern int fileN;

void BuildIndex(){
	printf("Building index from reverse index...\n");

	for(int ti = 0; dict[ti].nodes.size() > 0; ti++)
		for(int j = 0; j < dict[ti].nodes.size(); j++){
			struct INodeType tmp;
			tmp.termID = ti;
			tmp.tf = dict[ti].nodes[j].tf;
			docVec[dict[ti].nodes[j].docID].push_back(tmp);
		}

	printf("Build index successfully!\n");

	printf("Document vector normalizing...\n");
	
	float vlen;
	for(int i = 0; i < fileN; i++){
		vlen = 0;
		for(int j = 0; j < docVec[i].size(); j++)
			vlen += docVec[i][j].tf * docVec[i][j].tf;
		vlen = sqrt(vlen);
		for(int j = 0; j < docVec[i].size(); j++)
			docVec[i][j].vn = (float)docVec[i][j].tf / vlen;
	}
	
	printf("Normalized successfully!\n");

#ifdef DEBUGINDEX
	
#endif

}

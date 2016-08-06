#include "searchServer.h"

//extern struct RIHeadType dict[TERMNUM];
extern struct FileInfoType fileInfo[DOCNUM];
extern int termN;

bool cmpTfidf(struct SearchResultNode a, struct SearchResultNode b){
	return a.tfidf_sum > b.tfidf_sum;
}

bool cmpTime(struct SearchResultNode a, struct SearchResultNode b){
	if(a.year > b.year) return true;
	else if(a.year < b.year) return false;
	else{
		unsigned int av = 0, bv = 0;
		av = a.month*100000000 + a.day*1000000 + a.hour*10000 + a.min*100 + a.sec; 
		bv = b.month*100000000 + b.day*1000000 + b.hour*10000 + b.min*100 + b.sec;
		return av > bv;
	}
}

bool cmpCmtN(struct SearchResultNode a, struct SearchResultNode b){
	return a.cmtN > b.cmtN;
}

bool cmpHot(struct SearchResultNode a, struct SearchResultNode b){
	return a.hot_value > b.hot_value;
}

void setDate(struct SearchResultNode *srn){
	int id = srn->docID;
	srn->year = fileInfo[id].year;
	srn->month = fileInfo[id].month;
	srn->day = fileInfo[id].day;
	srn->hour = fileInfo[id].hour;
	srn->min = fileInfo[id].min;
	srn->sec = fileInfo[id].sec;
	srn->cmtN = fileInfo[id].cmtN;
}

struct SearchResultType KeyWordSearch(struct RIHeadType dict[], vector<string> vkeywords, int ord){
	char *keyword = new char[128];
	//int keyWordsLen = strlen(keywords);
	int len;
	int i = 0;
	
	vector<struct SearchResultNode> searchResult[2];
	int R = 1;

	while(i < vkeywords.size()){
		len = 0;	
		//while(i < keyWordsLen && keywords[i] != ' ')
		int j = 0;
		while(j < vkeywords[i].length())
			keyword[len++] = vkeywords[i][j++];
		keyword[len] = '\0';
		i++;
		
		for(int j = 0; j < termN; j++)
			if(strcmp(keyword, dict[j].term) == 0){
				if(R == 0) R = 1;
				else R = 0;			

				int i1 = 0, i2 = 0;
				while(i1 < dict[j].nodes.size() && i2 < searchResult[R].size()){
					if(dict[j].nodes[i1].docID == searchResult[R][i2].docID){
						struct SearchResultNode tmp;
						tmp.docID = dict[j].nodes[i1].docID;
						tmp.tfidf_sum = searchResult[R][i2].tfidf_sum + (1.0 / dict[j].df) * dict[j].nodes[i1].tf;
						setDate(&tmp);
						searchResult[1-R].push_back(tmp);
//add time and hot value of searchResult[1-R] here						
						i1++;
						i2++;
					}else if(dict[j].nodes[i1].docID < searchResult[R][i2].docID){
						struct SearchResultNode tmp;
						tmp.docID = dict[j].nodes[i1].docID;
						tmp.tfidf_sum = 1.0 / dict[j].df * dict[j].nodes[i1].tf;
						setDate(&tmp);
						searchResult[1-R].push_back(tmp);
						i1++;
					}else{
						struct SearchResultNode tmp;
						tmp.docID = searchResult[R][i2].docID;
						tmp.tfidf_sum = searchResult[R][i2].tfidf_sum;
						setDate(&tmp);
						searchResult[1-R].push_back(tmp);
						i2++;
					}
				}

				while(i1 < dict[j].nodes.size()){
					struct SearchResultNode tmp;
					tmp.docID = dict[j].nodes[i1].docID;
					tmp.tfidf_sum = 1.0 / dict[j].df * dict[j].nodes[i1].tf;
					setDate(&tmp);
					searchResult[1-R].push_back(tmp);
					i1++;
				}
				while(i2 < searchResult[R].size()){
					struct SearchResultNode tmp;
					tmp.docID = searchResult[R][i2].docID;
					tmp.tfidf_sum = searchResult[R][i2].tfidf_sum;
					setDate(&tmp);
					searchResult[1-R].push_back(tmp);
					i2++;					
				}
				
				searchResult[R].clear();
				break;
			}
	}

	R = 1 - R;
	if(ord == TFIDF_ORD)
		sort(searchResult[R].begin(), searchResult[R].end(), cmpTfidf);
	else if(ord == TIME_ORD)
		sort(searchResult[R].begin(), searchResult[R].end(), cmpTime);
	else if(ord == HOT_ORD){
		float sigma = 0.3;
		sort(searchResult[R].begin(), searchResult[R].end(), cmpTime);
		for(int i = 0; i < searchResult[R].size(); i++)
			searchResult[R][i].hot_value = sigma * (searchResult[R].size() - i) / searchResult[R].size();
		sort(searchResult[R].begin(), searchResult[R].end(), cmpCmtN);
		for(int i = 0; i < searchResult[R].size(); i++)
			searchResult[R][i].hot_value += (1-sigma) * (searchResult[R].size() - i) / searchResult[R].size();
		
		sort(searchResult[R].begin(), searchResult[R].end(), cmpHot);
	}

	struct SearchResultType ret;
	ret.docs = searchResult[R];
	
	int topN = 20;
	if(searchResult[R].size() < topN) topN = searchResult[R].size();
	ret.clusters = GAAClustering(searchResult[R], topN, 0.6f, 5);
	
	return ret;	
}

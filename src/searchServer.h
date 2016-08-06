#include<cstdlib>
#include<map>
#include<fstream>

#include<vector>
#include<iostream>
#include<cstdio>
#include<vector>
#include<dirent.h>
#include<unistd.h>
#include<string.h>
#include<algorithm>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/time.h>
#include<queue>
#include<math.h>

using namespace std;

#define BUFFLEN 100000
#define DOCNUM 120000
#define TERMNUM 160000

#define TFIDF_ORD 1
#define TIME_ORD 2
#define HOT_ORD 3

struct RINodeType{
	int tf;
	int docID;
};

struct RIHeadType{
	int df;
	char *term;
	vector<struct RINodeType> nodes;
	void setTermHead(char *str, int len, int dftmp){
		df = dftmp;
		term = new char[len];
		for(int i = 0; i < len; i ++)
			term[i] = str[i];
	}
};

struct INodeType{
	int termID;
	int tf;
	float vn;
	INodeType(){
		vn = 0;
	}
};

struct SearchResultNode{
	int docID;
	double tfidf_sum;
	int year,month,day,hour,min,sec;
	int cmtN;
	float hot_value;
	SearchResultNode(){
		tfidf_sum = 0;
		year = 0;
		month = 0;
		day = 0;
		hour = 0;
		min = 0;
		sec = 0;
		cmtN = 0;
		hot_value = 0;
	}
};

struct SearchResultType{
	vector<struct SearchResultNode> docs;
	vector<vector<int> > clusters;
};

struct FileInfoType{
	char filename[16];
	int year,month,day,hour,min,sec;
	int cmtN;
	char path[60];

	FileInfoType(){
		year = month = day = hour = min = sec = 0;
		cmtN = 0;
	}
	void setDateTime(int datetime[]){
		year = datetime[0];
		month = datetime[1];
		day = datetime[2];
		hour = datetime[3];
		min = datetime[4];
		sec = datetime[5];		
	}
};

struct SearchResultType KeyWordSearch(struct RIHeadType dict[], vector<string> keywords, int ord);

vector<string> WordsProcess(char p[]);

vector<vector<int> > GAAClustering(vector<struct SearchResultNode> docs, int topN, float simtr, int cN);

void WordsInit();
void BuildIndex();

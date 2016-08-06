
#include "searchServer.h"
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define DEBUG
//#define DEBUGINFO

map<string, int> stop;
map<string, int> dic;

int stop_num = 0;
int dic_num = 0;

struct FileInfoType fileInfo[DOCNUM];
struct RIHeadType dict[TERMNUM];
vector<struct INodeType> docVec[DOCNUM];
int fileN;
int termN;


void GetFileInfo(){
	printf("Mapping index to filename...\n");
	
	FILE *findex = fopen("IndexResult/fileindex","r");
	fileN = 0;
	int i,ni;
	int tmp;
	char line[100];
	int datetime[6];
	while(fgets(line, 100, findex)){
		i = 0;
		ni = 0;
		while(line[i] != '#')
			fileInfo[fileN].filename[ni++] = line[i++];
		fileInfo[fileN].filename[ni] = '\0';
		i++;
		
		memset(datetime, 0, sizeof(datetime));
		int k = 0;
		if(line[i] != 'N'){
			while(line[i] != '#'){
				tmp = 0;
				while(line[i] >= '0' && line[i] <= '9')
					tmp = tmp * 10 + line[i++] - '0';
				while((line[i] < '0' || line[i] > '9') && line[i] != '#')
					i++;
				datetime[k++] = tmp;
			}
		}else while(line[i] != '#') i++;
		fileInfo[fileN].setDateTime(datetime);
		i++;
		if(line[i] > '9' || line[i] < '0')
			fileInfo[fileN].cmtN = 0;
		else{
			tmp = 0;
			while(line[i] != '#'){
				tmp = tmp * 10 + line[i++] - '0';
			}
			fileInfo[fileN].cmtN = tmp;
		}

		fileN++;
	}

	fclose(findex);
	
	FILE *fpath = fopen("filepath/PathFile", "r");
	i = 0;
	int j,l;
	while(fgets(line, 100, fpath)){
		j = 0;
		while(line[j] != ' ') j++;
		j++;
		l = 0;
		while(line[j] != '\0') 	
			fileInfo[i].path[l++] = line[j++];
		if(fileInfo[i].path[l-1] == '\n') l--;
		fileInfo[i].path[l++] = '\0';
		i++;
	}

	fclose(fpath);
#ifdef DEBUGINFO
	for(int  i = 0; i < 200; i++){
		printf("%s %d %d %d %d %d %d %d %s\n%", fileInfo[i].filename, fileInfo[i].year, fileInfo[i].month, fileInfo[i].day, fileInfo[i].hour, fileInfo[i].min, fileInfo[i].sec, fileInfo[i].cmtN, fileInfo[i].path);
	}
#endif
	
	printf("Mapping OK!\n");
}

void ReverseIndexToMemory(FILE *fin){
	char *buff = new char[BUFFLEN];
	char *term = new char[1000];
	
	while(fgets(buff, BUFFLEN, fin)){
		int i = 0;
		int len = strlen(buff);
		int termLen;
		while(i < len && buff[i] != ' ')
			term[i] = buff[i++];
		term[i] = '\0';
		termLen = i + 1;
		i++;
		int dftmp = 0;
		while(i < len && buff[i] >= '0' && buff[i] <= '9')
			dftmp = dftmp * 10 + buff[i++] - '0';
		i++;
		dict[termN].setTermHead(term, termLen, dftmp);

		int docID,tf;
		while(i < len && dftmp > 0){
			docID = 0;
			while(i < len && buff[i] >= '0' && buff[i] <= '9')
				docID = docID * 10 + buff[i++] - '0';
			i++;
			tf = 0;
			while(i < len && buff[i] >= '0' && buff[i] <= '9')
				tf = tf * 10 + buff[i++] - '0';
			i++;
			struct RINodeType rinode;
			rinode.docID = docID;
			rinode.tf = tf;
			dict[termN].nodes.push_back(rinode);
			
			dftmp--;
		}

		termN++;
	}
}

int IsDir(const char *path){
	struct stat st;
	stat(path, &st);
	return S_ISDIR(st.st_mode);
}

bool cmp(struct RINodeType a, struct RINodeType b){
	return a.docID < b.docID;
}

void ReadReverseIndex(){
	printf("Reading reverse index into memeoy...\n");
	
	termN = 0;
	char filepath[256];
	char path[] = "IndexResult/indexfile/";
	struct dirent *ptr;
	DIR *dir = opendir(path);
	while((ptr = readdir(dir)) != NULL){
		if(ptr->d_name[0] == '.') continue;
		sprintf(filepath, "%s%s", path, ptr->d_name);
		if(IsDir(filepath) == 0){
		//	printf("Reading %s...\n", filepath);
			FILE *fin = fopen(filepath, "r");
			ReverseIndexToMemory(fin);
			fclose(fin);
		}
	}
		
	closedir(dir);
	printf("Preprocessing...\n");
	for(int i = 0; i < termN; i++)
		sort(dict[i].nodes.begin(), dict[i].nodes.end(), cmp);

	printf("Reverse index OK!\n");

#ifdef DEBUGREADINDEX
	printf("termN:%d\n",termN);
	for(int i = 0; i < 10; i++){
		printf("%s %d ", dict[i].term, dict[i].df);
		for(int j = 0; j < 10; j++)
			printf("%d %d, ", dict[i].nodes[j].docID, dict[i].nodes[j].tf);
		printf("\n");
	}	
#endif	

}

struct simTermType{
	int termIndex;
	float sim;
};

struct ResultType{
	double search_time;
	vector<int> docIDs;
	vector<vector<int> > clusters;
};

int termsCmp(char *str1, char *str2){
	return 0;
}

struct ResultType query(vector<string> vkeywords, int ord){

	bool find;
	struct timeval start_time, end_time;

	gettimeofday(&start_time, NULL);
				
	struct SearchResultType result = KeyWordSearch(dict, vkeywords, ord); 

	gettimeofday(&end_time, NULL);
	
	ResultType ret;
	ret.search_time = (double)(end_time.tv_sec+(double)end_time.tv_usec/1000000) - (double)(start_time.tv_sec+(double)start_time.tv_usec/1000000);
	
	for(int i = 0; i < result.docs.size(); i++)
		ret.docIDs.push_back(result.docs[i].docID);

	ret.clusters = result.clusters;

	return ret;
}

void AcceptSearch(){
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(9734);
	server_len  = sizeof(server_address);

	bind(server_sockfd, (struct sockaddr*)&server_address, server_len);

	listen(server_sockfd, 5);

	char *keywords = new char[128];
	while(true){
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_address, (socklen_t*)&client_len);
		
		for(int i = 0; i < 128; i++)
			keywords[i] = '\0';

		int len = 0;
		
		char ord;
		read(client_sockfd, &ord, 1);
		read(client_sockfd, keywords, 128);

		vector<string> vkeywords = WordsProcess(keywords);
		
		struct ResultType result;
		if(ord >= '0' && ord <= '3'){

			printf("Accept search for: %s\n", keywords);

			struct ResultType result = query(vkeywords, ord - '0');
			
			char time_str[20];
			sprintf(time_str, "%.8lf", result.search_time);
			char docN_str[20];
			sprintf(docN_str, "%d", result.docIDs.size());
		
			char *docIDs = new char[result.docIDs.size() * 7];
			int k = 0, t;
			int tmp_str[20];
			int tmp;
			for(int i = 0; i < result.docIDs.size(); i++){
				tmp = result.docIDs[i];
				t = 0;
				while(tmp > 0){
					tmp_str[t++] = tmp % 10 + '0';
					tmp /= 10;
				}
				t--;
				while(t >= 0)
					docIDs[k++] = tmp_str[t--];
				docIDs[k++] = ',';
			}
			docIDs[k] = '\0';
		
//			printf("time:%s\n", time_str);
//			printf("docN:%s\n", docN_str);
//			printf("%s\n",docIDs);		
//			printf("%d\n", vkeywords.size());
	
			char tmpfile[] = "tmp/socketdata.dat";
			FILE *fout = fopen(tmpfile, "w");
			char tmpstr[100];
			for(int i = 0; i < vkeywords.size(); i++){
				int j;
				for(j = 0; j < vkeywords[i].length(); j++)
					tmpstr[j] = vkeywords[i][j];
				tmpstr[j]='\0';
				fprintf(fout, "%s ", tmpstr);
			}
			fprintf(fout, "\n");
			fprintf(fout, "%.8lf\n", result.search_time);

			fprintf(fout, "%d\n", result.clusters.size());
			for(int i = 0; i < result.clusters.size(); i++){
				fprintf(fout, "%d\n", result.clusters[i].size());
				for(int j = 0; j < result.clusters[i].size(); j++)
					fprintf(fout, "/root/Desktop/MYProgram/searchEngine%s\n", fileInfo[result.clusters[i][j]].path);
			}

			fprintf(fout, "%d\n", result.docIDs.size());
			for(int i = 0; i < result.docIDs.size(); i++)
				fprintf(fout, "/root/Desktop/MYProgram/searchEngine%s\n", fileInfo[i].path);
//			fprintf(fout, "%s\n", docIDs);
			fclose(fout);
	
			write(client_sockfd, tmpfile, strlen(tmpfile));
			
			printf("Return %d results.\n", result.docIDs.size());
			printf("Search time comsume: %.6lf\n", result.search_time);
//			write(client_sockfd, docN_str, strlen(docN_str));
//			write(client_sockfd, docIDs, strlen(docIDs));
		}else{
		
		}

		close(client_sockfd);
	}
}

int main(){
	GetFileInfo();
	ReadReverseIndex();
	BuildIndex();
	WordsInit();
	printf("SearchServer boots successfully!\n");
	printf("Wait for connection...\n");
	AcceptSearch();
}

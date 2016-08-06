#include<iostream>
#include<string.h>
#include<cstdio>
#include<map>
#include<algorithm>
#include<fstream>
#include<vector>
using namespace std;

#define dic_num 200000
#define str_len 100

bool cmp(string p1,string p2)
{
    char p1s[str_len],p2s[str_len];
    int i;
    for(i=0; i<p1.size(); i++)
        p1s[i] = p1[i];
    p1s[i] = '\0';
    for(i=0; i<p2.size(); i++)
        p2s[i] = p2[i];
    p2s[i] = '\0';
    if(strcoll(p1s,p2s) == -1)
        return false;
    else return true;
}

map<string,int> pro_dic;
int idx = 0;
string word[dic_num];

void init()
{
    FILE *fp = fopen("Dic","r");
    char Str[str_len];

    while(fgets(Str,str_len,fp) != NULL)
    {
        Str[strlen(Str)-1] = '\0';
        if(pro_dic.find(Str) == pro_dic.end())
        {
            word[idx] = Str;
            pro_dic[Str] = idx++;
        }
    }

    fclose(fp);

    sort(word,word+idx,cmp);

    ofstream out("DicFile");
    for(int i=0; i<idx; i++)
    {
        out<<word[i]<<endl;
    }
}

int cmp1(string p1,char p2[])
{
    int i;
    char s1[str_len];

    for(i=0; i<p1.size(); i++)
        s1[i] = p1[i];
    s1[i] = '\0';

    if(strcoll(s1,p2) == 0) return 1;
    else return 0;
}

vector<string> complete(char p[])
{
    vector<string> res;
    int len = strlen(p);
    int i;
    int match = 0;
    
    for(i=0; i<idx; i++)
    {
        if(len > word[i].size()) continue;

        if(cmp1(word[i].substr(0,len),p) == 1)
        {
            match = 1;
            res.push_back(word[i]);
        }
        else if(match == 1) break;
    }
    return res;
}

int main()
{
    init();
    char p[100];

    while(true)
    {
        cin>>p;

        vector<string> res = complete(p);
        for(int i=0; i<res.size(); i++)
            cout<<res[i]<<endl;
    }

    return 0;
}

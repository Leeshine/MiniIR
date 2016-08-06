#include "searchServer.h"

#define len 30
#define que_item 20

extern map<string,int> stop;
extern map<string,int> dic;

extern int stop_num;
extern int dic_num;

void WordsInit()
{
    FILE *fp = fopen("dic/stop","r");
    char Str[len];

    while(fgets(Str,len,fp) != NULL)
    {
        Str[strlen(Str)-1]='\0';
        if(stop.find(Str) == stop.end())
            stop[Str] = stop_num++;

    }
    fclose(fp);

    FILE *fr = fopen("dic/Dic","r");

    while(fgets(Str,len,fr) != NULL)
    {
        Str[strlen(Str)-1]='\0';
        if(dic.find(Str) == dic.end())
            dic[Str] = dic_num++;
    }
    fclose(fr);
}

void max_seg(string str,vector<string> &res)
{
    int ids = 0;
    int i;
    string sb,sc;
    int match;

    while(ids < str.size())
    {
        match = 0;
        for(i=str.size()-ids; i>0;)
        {
            sb = str.substr(ids,i);
            if(dic.find(sb) != dic.end())
            {
                if(stop.find(sb) == stop.end())
                    res.push_back(sb);
                match = 1;
                break;
            }

            i-=3;

        }

        if(match == 0)
        {
            i = 3;
            sc = str.substr(ids,3);
            if(stop.find(sc) == stop.end())
            {
                res.push_back(sc);
            }
        }

        ids+=i;
    }
}

vector<string> WordsProcess(char p[])
{
    int i;
    vector<string> res;

    vector<string> query;
    string st="";

    for(i=0; i<strlen(p); i++)
    {
        if(p[i] != ' ')
            st += p[i];
        else 
        {
            query.push_back(st);
            st = "";
        }
    }
    
    query.push_back(st);

    for(i=0; i<query.size(); i++)
    {
        max_seg(query[i],res);
    }

    return res;
}


#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<dirent.h>
#include<string.h>
#include<cstring>
#include<fstream>
#include<iostream>
#include<map>
using namespace std;

#define dic_len 350000
#define file_num 150000
#define str_len 100
#define file_len 15000

int idx=0;//file_num
int idx_num=0;//dic_num
string file[file_num];//file name

map<string,int> pro_word;//item,id
map<string,int> pro_file;//file,id
map<string,int> count_word;//for each file

struct ele
{
    int file_id;
    int freq;
    struct ele *next;
    ele(int i=-1,int f=0,struct ele *ne=NULL):file_id(i),freq(f),next(ne){};
};

struct node
{
    string name;//item name
    int num;//iten num
    struct ele *tofile;

    node(int i=0,struct ele *to=NULL):num(i),tofile(to){};
};
node *wd[dic_len];

void  GetFile(char* filePath)//get file name
{
    DIR *dp=opendir(filePath);
    struct dirent *drip;

    if(dp == NULL)
    {
        printf("Can't open %s\n",filePath);
        exit(1);
    }
    while((drip=readdir(dp)) != NULL)
    {
        string p = drip->d_name;
        if(p[0] != '.')
        {
            file[idx++] = p;
        }
    }
}

void build_index()//file[] 
{
    string filename;
    char Str[100];
    int idw,idf;
    ele *ks[file_len];

    for(int i=0; i<idx; i++)
    {
        filename = "result/"+file[i];
        char Str[str_len];
        idf = 0;
        count_word.clear();

        FILE *fp = fopen(filename.c_str(),"r");
        if(fp == NULL)
        {
            continue;
        }
        else
        {
            while(fgets(Str,str_len,fp) != NULL)
            {
                Str[strlen(Str)-1] = '\0';

                if(pro_word.find(Str) == pro_word.end()) //尚未出现
                {
                    ks[idf] = new ele;
                    ks[idf]->file_id = i;
                    ks[idf]->freq = 1;
                    ks[idf]->next = NULL;

                    wd[idx_num] = new node;
                    wd[idx_num]->num = 1;
                    wd[idx_num]->name = Str;
                    wd[idx_num]->tofile = ks[idf];

                    pro_word[Str] = idx_num++;
                    count_word[Str] = idf++;
                }
                else
                {
                    idw = pro_word[Str];
                    wd[idw]->num++;
                    if(count_word.find(Str) == count_word.end())
                    {
                        ks[idf] = new ele;
                        ks[idf]->file_id = i;
                        ks[idf]->freq = 1;
                        ks[idf]->next = wd[idw]->tofile;
                        wd[idw]->tofile = ks[idf];
                        count_word[Str] = idf++;
                    }
                    else
                    {
                        ks[count_word[Str]]->freq++;
                    }
                }
            }
        }
    }
}

int main()
{
    GetFile("result");
    build_index();

    map<string,int>::iterator it = pro_word.begin();
    ele *sk;
    int jk;

    for(; it!=pro_word.end(); it++)
    {
        jk = it->second;
        if(wd[jk]->tofile == NULL)
            continue;
        cout<<it->first<<":";
        sk = wd[jk]->tofile;
        while(sk != NULL)
        {
            cout<<file[sk->file_id]<<"("<<sk->freq<<"),";
            sk = sk->next;
        }
        cout<<endl;
    }

    return 0;
}

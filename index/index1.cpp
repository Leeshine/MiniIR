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

int idx=0;//file_num
int idx_num=0;//dic_num
string file[file_num];

map<string,int> pro_word;//item,id
map<string,int> pro_file;//file,id

struct ele
{
    int file_id;
    struct ele *next;
    ele(int i=-1,struct ele *ne=NULL):file_id(i),next(ne){};
};

struct node
{
    string name;//item name
    int num;//iten num
    struct ele *tofile;

    node(int i=0,struct ele *to=NULL):num(i),tofile(to){};
};

node *wd[dic_len];

void init()//item of dic <--> id
{
    FILE *fp = fopen("Dic","r");
    char Str[str_len];
    if(fp == NULL)
        perror("Error open Dic");
    else
    {
        while(fgets(Str,str_len,fp) != NULL)
        {
            Str[strlen(Str)-1]='\0';
            if(pro_word.find(Str) == pro_word.end())
            {
                node *sk = new node;
                sk->name = Str;
                sk->num = 0;
                sk->tofile = NULL;
                wd[idx_num] = sk;
                pro_word[Str]=idx_num++;
            }
            else continue;
        }
    }
}

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
    int idw;

    for(int i=0; i<idx; i++)
    {
        filename = "result/"+file[i];
        char Str[str_len];

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
                cout<<Str<<", ";
                if(pro_word.find(Str) != pro_word.end())
                {
                    idw = pro_word[Str];
                    wd[idw]->num++;
                    ele *sk = new ele;
                    sk->file_id = i;
                    if(wd[idw]->tofile != NULL)
                    {
                        sk->next = wd[idw]->tofile;
                    }
                        wd[idw]->tofile = sk;
                }
                else //词典里没有
                {
                }
            }
            cout<<endl;
        }
    }
}

int main()
{
    init();
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
            cout<<file[sk->file_id]<<",";
            sk = sk->next;
        }
        cout<<endl;
    }

    return 0;
}

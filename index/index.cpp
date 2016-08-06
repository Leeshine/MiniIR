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

#define dic_len 250000
#define file_num 150000
#define str_len 100
int idx=0;
int idx_num=0;
char* file[file_num];

map<string,int> pro_word;

struct node
{
    string name;
    int num;
    struct node *next;

    node(){
        num = 0;
        next = NULL;
    }
};
node wd[dic_len]=node();

void init()
{
    int num=0;
    FILE *fp = fopen("Dic","r");
    char Str[100];
    if(fp == NULL)
        perror("Error open Dic");
    else
    {
        while(fgets(Str,100,fp) != NULL)
        {
            Str[strlen(Str)-1]='\0';
            wd[idx_num++].name = Str;
            pro_word[Str]=num++;
        }
    }
}

unsigned int BKDRHash(char *str)
{
    unsigned int seed = 131;
    unsigned int hash = 0;
    while(*str)
        hash = hash *seed+(*str++);

    return (hash & 0x7FFFFFFF);
}

void  GetFile(char* filePath)
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
        char *p = drip->d_name;
        if(p[0] != '.')
            file[idx++]=p;
    }
}

void build_index()
{
}

int main()
{
    init();
    return 0;
}

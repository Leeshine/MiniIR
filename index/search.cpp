#include<iostream>
#include<cstdio>
#include<map>
#include<fstream>
#include<sstream>
#include<string.h>
#include<cstring>
#include<stdlib.h>
using namespace std;

#define len 100000
#define que_len 20
#define dic_len 350000
#define file_num 110000
#define que_num 100

string query[que_len];
int idx=0;
int idx_num=0;

map<string,int> pro_word;
map<int,string> pro_file;

struct ele
{
    int fileid;
    int freq;
    ele* next;
    ele(int i=-1,int f=0,ele* ne=NULL):fileid(i),freq(f),next(ne){};
};

struct node
{
    string item;
    int num;
    ele* tofile;
    node(int n=0,ele* to=NULL):num(n),tofile(to){};
};

node *wd[dic_len];
node *re[que_num];

node* ProNode(node *n1,node *n2);

void init()
{
    ifstream in("fileindex");
    char line[len] = {0};
    string la = "";
    string lb = "";
    int nu = 0;

    while(in.getline(line,sizeof(line)))
    {
        stringstream num(line);
        num >> la; num >>lb;
        nu = atoi(lb.c_str());
        pro_file[nu]=la;
    }

    ifstream indexin("indexfile");
    while(indexin.getline(line,sizeof(line)))
    {
        stringstream num(line);
        num >> la; num >> lb;
        wd[idx_num] = new node;
        wd[idx_num]->item = la;
        wd[idx_num]->num =  atoi(lb.c_str());
        wd[idx_num]->tofile = NULL;
        pro_word[la]=idx_num;

        while(num >> la)
        {
            ele *sk = new ele;
            nu = atoi(la.c_str());
            sk->fileid = nu;
            num >> lb; nu = atoi(lb.c_str());
            sk->freq = nu;
            if(wd[idx_num]->tofile == NULL)
            {
                sk->next = NULL;
                wd[idx_num]->tofile = sk;
            }
            else
            {
                sk->next = wd[idx_num]->tofile;
                wd[idx_num]->tofile = sk;
            }
        }
        idx_num++;
    }
}

node* search(int length)
{
    int i;
    string str1,str2;
    int num1;
    node* ans;

    
    num1= pro_word[query[0]];
    ans = wd[num1];

    for(i=1; i<length; i++)
    {
        str1 = query[i];
        num1 = pro_word[str1];
        if(ans->tofile != NULL && wd[num1]->tofile != NULL)
            ans = ProNode(ans,wd[num1]);
    }

    return ans;
}

node* ProNode(node* n1,node* n2)
{
    node* ans = new node;
    ans->tofile = NULL;
    ele* e1 = n1->tofile;
    ele* e2 = n2->tofile;

    while(e1!=NULL && e2!= NULL)
    {
        if(e1->fileid == e2->fileid)
        {
            ele* e = new ele;
            e->fileid = e1->fileid;
            if(ans->tofile == NULL)
                ans->tofile = e;
            else{
                e->next = ans->tofile;
                ans->tofile = e;
            }
            e1 = e1->next;
            e2 = e2->next;
        }
        else if(e1->fileid > e2->fileid)
            e2 = e2->next;
        else
            e1 = e1->next;
    }

    if(ans->tofile == NULL)
        return n1;
    else return ans;
}

int main(int argc,char *argv[])
{
    init();

    int i;
    for(i=1; i<argc; i++)
    {
        query[i-1] = argv[i];
    }

/*    int nu;
    map<string,int>::iterator it;
    for(it=pro_word.begin(); it!=pro_word.end(); ++it)
    {
        nu = it->second;
        ele* sk = wd[nu]->tofile;
        cout<<wd[nu]->item<<","<<wd[nu]->num;
        while(sk != NULL)
        {
            cout<<sk->fileid<<","<<sk->freq<<" ";
            sk = sk->next;
        }
        cout<<endl;
    }*/

    node* ans = search(argc-1);
    ele* sk;
    if(ans == NULL)
        cout<<"Bad query!"<<endl;
    else
    {
        sk = ans->tofile;
        while(sk != NULL)
        {
            cout<<pro_file[sk->fileid]<<endl;
            sk = sk->next;
        }
    }
    delete ans;

    return 0;
}

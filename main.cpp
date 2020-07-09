#include <iostream>
#include <cstring>
#include <fstream>
#define TESTFILE "test1.txt"//文件名宏定义，测试修改该宏定义即可：test1.txt等，详情可查看设计报告

using namespace std;

struct nontoken//非终结符结构体
{
    char str[50];
    char firstvt[50][50];//firstvt集合
    char lastvt[50][50];//lastvt集合
    int firstvt_nr=0;
    int lastvt_nr=0;
    void addfirstvt(char *str)//加入新的firstvt元素
    {
        for(int i=0; i<50; i++)
        {
            if(strcmp(firstvt[i],str)==0)
                return;
        }
        strcpy(firstvt[firstvt_nr],str);
        firstvt_nr++;
        return;
    }
    void addlastvt(char *str)//加入新的lastvt元素
    {
        for(int i=0; i<50; i++)
        {
            if(strcmp(lastvt[i],str)==0)
                return;
        }
        strcpy(lastvt[lastvt_nr],str);
        lastvt_nr++;
        return;
    }
    void deletefirstvt(char *str)//删除指定str的firstvt元素
    {
        for(int i=0; i<firstvt_nr; i++)
        {
            if(strcmp(firstvt[i],str)==0)
            {
                for(int j=i; j<49; j++)
                {
                    strcpy(firstvt[j],firstvt[j+1]);
                }
                firstvt_nr--;
            }
        }
    }
    void deletelastvt(char *str)//删除指定str的lastvt元素
    {
        for(int i=0; i<lastvt_nr; i++)
        {
            if(strcmp(lastvt[i],str)==0)
            {
                for(int j=i; j<49; j++)
                {
                    strcpy(lastvt[j],lastvt[j+1]);
                }
                lastvt_nr--;
            }
        }
    }
};
struct tertoken//终结符结构体
{
    char str[50];
    char pre[50][50];//前继字符串集合
    char suc[50][50];//后继字符串集合
    int pre_nr=0;
    int suc_nr=0;
    void addpre(char *str)//添加前继字符串
    {
        for(int i=0; i<50; i++)
        {
            if(strcmp(pre[i],str)==0)
                return;
        }
        strcpy(pre[pre_nr],str);
        pre_nr++;
        return;
    }
    void addsuc(char *str)//添加后继字符串
    {
        for(int i=0; i<50; i++)
        {
            if(strcmp(suc[i],str)==0)
                return;
        }
        strcpy(suc[suc_nr],str);
        suc_nr++;
        return;
    }
};
class analyzer//分析器类
{

    nontoken nonterminal[50];//非终结符集合
    tertoken terminal[50];//终结符集合
public:
    int cur_ter_nr=0;
    int cur_non_nr=0;
    int table[50][50]= {{0}};//结果分析表
    bool iswrong=false;//错误判断标识符

    char words[50][50];//语句集合
    int find_non(char *str)//找到对应的非终结符，并返回下标
    {
        for(int i=0; i<cur_non_nr; i++)
        {
            if(strcmp(str,nonterminal[i].str)==0)
                return i;
        }
        return -1;
    }
    int find_ter(char *str)//找到对应的终结符，并返回下标
    {
        for(int i=0; i<cur_ter_nr; i++)
        {
            if(strcmp(str,terminal[i].str)==0)
                return i;
        }
        return -1;
    }
    void add_non(const char str[])//往非终结符集合中添加新的元素
    {
        bool isexit=false;
        for(int i=0; i<50; i++)
        {
            if(strcmp(str,nonterminal[i].str)==0)
            {
                isexit=true;
            }
        }
        if(isexit==false)
        {
            strcpy(nonterminal[cur_non_nr].str,str);
            cur_non_nr++;
        }
    }
    void add_ter(const char str[])//往终结符集合中添加新的元素
    {
        bool isexit=false;
        for(int i=0; i<50; i++)
        {
            if(strcmp(str,terminal[i].str)==0)
            {
                isexit=true;
            }
        }
        if(isexit==false)
        {
            strcpy(terminal[cur_ter_nr].str,str);
            cur_ter_nr++;
        }
    }
    bool isnon(const char str[])//判断指定字符串是不是非终结符
    {
        for(int i=0; i<cur_non_nr; i++)
        {
            if(strcmp(str,nonterminal[i].str)==0)
                return true;
        }
        return false;
    }
    void add_pre_suc(char str[]="",char pre[]="",char suc[]="")//加入前继或者后继，未指定即添加空字符串
    {
        int pos=0;
        pos=find_ter(str);
        if(strcmp(pre,"")!=0)
            terminal[pos].addpre(pre);
        if(strcmp(suc,"")!=0)
            terminal[pos].addsuc(suc);
    }
    void readnonminal()//读入函数
    {
        fstream in;
        in.open(TESTFILE);//打开文件
        char str[50]="";
        char secondstr[50]="";
        char *p,tmpnon[50],*q,*s;
        char pp[50],qq[50],ss[50];
        p=new char[50];
        q=new char[50];
        s=new char[50];
        bool isfirst=false;
        int i=0;
        while(in.getline(str,50))//一行行读取语句
        {
            strcpy(words[i],str);
            p=strtok(str," ");//截取非终结符并添加
            add_non(p);
            i++;
        }
        int num=i;
        in.close();
        for(int j=0; j<num; j++)
        {
            int linepos[10]= {0};
            int word_nr=0;
            char sentence[50][50];
            strcpy(str,words[j]);
            p=strtok(str," ");
            strcpy(tmpnon,p);
            p = strtok(NULL," ");
            if(strcpy(p,"->")==0)//判断非法输入
            {
                iswrong=true;
                return;
            }
            for(i=0; i<40; i++)
                secondstr[i]=str[strlen(tmpnon)+4+i];//将语句后半部分储存
            q=strtok(secondstr,"|");//以‘|’为分隔符号，切割语句
            while(q)
            {
                strcpy(sentence[word_nr],q);
                word_nr++;
                q=strtok(NULL,"|");
            }
            int tmp=find_non(tmpnon);
            for(int m=0; m<word_nr; m++)
            {
                p=strtok(sentence[m]," ");
                if(strcmp("ε",p)==0)
                    iswrong=true;
                if(isnon(p)==false)
                    add_ter(p);
                nonterminal[tmp].addfirstvt(p);
                //second word
                q=strtok(NULL," ");
                if(q==NULL)//特殊处理
                {
                    nonterminal[tmp].addlastvt(p);
                    continue;
                }
                if(isnon(q)==false)
                {
                    add_ter(q);
                    nonterminal[tmp].addfirstvt(q);//firstvt
                }
                if(isnon(p)==false)
                    terminal[find_ter(p)].addsuc(q);
                s=strtok(NULL," ");
                strcpy(pp,p);
                strcpy(qq,q);
                while(true)//主循环，滚动读取所有的终结符或非终结符并进行判断
                {
                    if(s==NULL)//读取到空字符结束滚动，处理后跳出循环
                    {
                        if(isnon(qq)==false)
                            add_pre_suc(qq,pp,"");
                        nonterminal[tmp].addlastvt(qq);
                        if(isnon(pp)==false)
                            nonterminal[tmp].addlastvt(pp);
                        break;
                    }
                    if(isnon(s)==false)
                        add_ter(s);
                    if(isnon(qq)==false)
                    {
                        add_pre_suc(qq,pp,s);
                    }
                    if(isnon(pp)==true&&isnon(qq)==true||isnon(qq)==true&&isnon(s)==true)
                        iswrong=true;
                    if(isnon(pp)==false&&isnon(s)==false)
                    {
                        table[find_ter(pp)][find_ter(s)]=3;//设置'='
                    }
                    strcpy(pp,qq);
                    strcpy(qq,s);
                    s=strtok(NULL," ");
                }
            }

        }
        p=NULL;
        q=NULL;
        s=NULL;
        delete p;
        delete q;
        delete s;

    }
    void clean()//
    {
        for(int i=0; i<cur_ter_nr; i++)
        {
            nonterminal[i].deletefirstvt(nonterminal[i].str);

            nonterminal[i].deletelastvt(nonterminal[i].str);
        }
    }
    void addfvt(int tonum,int fromnum)//将一个非终结符的firstvt集合加入到另一个非终结符的firstvt集合中去
    {
        for(int i=0; i<nonterminal[fromnum].firstvt_nr; i++)
            nonterminal[tonum].addfirstvt(nonterminal[fromnum].firstvt[i]);
    }
    void addlvt(int tonum,int fromnum)//将一个非终结符的lastvt集合加入到另一个非终结符的lastvt集合中去
    {
        for(int i=0; i<nonterminal[fromnum].lastvt_nr; i++)
            nonterminal[tonum].addlastvt(nonterminal[fromnum].lastvt[i]);
    }
    bool nonisend(int pos)//判断非终结符的lastvt,firstvt集合中是否还有非终结符
    {
        for(int i=0; i<nonterminal[pos].firstvt_nr; i++)
            if(isnon(nonterminal[pos].firstvt[i])==true)
                return false;
        for(int i=0; i<nonterminal[pos].lastvt_nr; i++)
            if(isnon(nonterminal[pos].lastvt[i])==true)
                return false;
        return true;
    }
    bool isend()//判断所有的非终结符的lastvt,firstvt集合中是否还有非终结符
    {
        for(int i=0; i<cur_non_nr; i++)
            if(nonisend(i)==false)
                return false;
        return true;

    }
    void final_vt()//设置所有非终结符的lastvt,firstvt集合，使其不包含非终结符
    {
        char tmp[50];
        clean();
        while(!isend())
        {
            for(int i=0; i<cur_non_nr; i++)
            {
                for(int j=0; j<nonterminal[i].firstvt_nr; j++)
                {
                    if(isnon(nonterminal[i].firstvt[j])==true)
                    {
                        strcpy(tmp,nonterminal[i].firstvt[j]);
                        nonterminal[i].deletefirstvt(nonterminal[i].firstvt[j]);
                        addfvt(i,find_non(tmp));
                    }
                }
                for(int j=0; j<nonterminal[i].lastvt_nr; j++)
                {
                    if(isnon(nonterminal[i].lastvt[j])==true)
                    {
                        strcpy(tmp,nonterminal[i].lastvt[j]);
                        nonterminal[i].deletelastvt(nonterminal[i].lastvt[j]);
                        addlvt(i,find_non(tmp));
                    }
                }
            }
        }
    }
    void settable()//设置表格内容，1代表'>',2代表'<',3代表'=',未设置便输出空格
    {
        add_ter("$");
        terminal[cur_ter_nr-1].addpre(nonterminal[0].str);
        terminal[cur_ter_nr-1].addsuc(nonterminal[0].str);
        for(int i=0; i<cur_ter_nr; i++)
        {
            for(int j=0; j<terminal[i].pre_nr; j++)
            {
                int pos=find_non(terminal[i].pre[j]);
                for(int k=0; k<nonterminal[pos].lastvt_nr; k++)
                {
                    if(table[find_ter(nonterminal[pos].lastvt[k])][find_ter(terminal[i].str)]!=1&&table[find_ter(nonterminal[pos].lastvt[k])][find_ter(terminal[i].str)]!=0)
                        iswrong=true;//二义性判断
                    table[find_ter(nonterminal[pos].lastvt[k])][find_ter(terminal[i].str)]=1;//设置'>'
                }
            }
            for(int j=0; j<terminal[i].suc_nr; j++)
            {
                int pos=find_non(terminal[i].suc[j]);
                for(int k=0; k<nonterminal[pos].firstvt_nr; k++)
                {
                    if(table[find_ter(terminal[i].str)][find_ter(nonterminal[pos].firstvt[k])]!=2&&table[find_ter(terminal[i].str)][find_ter(nonterminal[pos].firstvt[k])]!=0)
                        iswrong=true;//二义性判断
                    table[find_ter(terminal[i].str)][find_ter(nonterminal[pos].firstvt[k])]=2;//设置'<'
                }
            }
        }
        table[cur_ter_nr-1][cur_ter_nr-1]=3;//设置'='
    }
    void printtable()//打印表格
    {
        final_vt();//调用函数设置lastvt、firstvt集合
        settable();//调用函数设置表格

        fstream out;
        out.open("out.txt");
        if(iswrong==true)//错误判断，错误就将错误信息输入到命令行
        {
            cout<<"This is not an operator-first grammar!";
            return;
        }
        out<<'\t';
        for(int i=0; i<cur_ter_nr; i++)//打印非终结符表头
        {
            out<<terminal[i].str<<'\t';
        }

        out<<endl;
        for(int i=0; i<cur_ter_nr; i++)
        {
            out<<terminal[i].str<<'\t';
            for(int j=0; j<cur_ter_nr; j++)
            {
                switch(table[i][j])//分类打印字符
                {
                case 1:
                    out<<'>'<<'\t';
                    break;
                case 2:
                    out<<'<'<<'\t';
                    break;
                case 3:
                    out<<'='<<'\t';
                    break;
                default:
                    out<<'\t';
                }
            }
            out<<endl;
        }
    }
    void analyze()
    {
        readnonminal();
        printtable();
    }
};

int main()
{
    analyzer tar;//定义分析器类
    tar.analyze();


    return 0;
}

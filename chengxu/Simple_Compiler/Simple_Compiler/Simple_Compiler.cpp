#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>

#define KeyWordNum 13//关键字的个数
#define maxvartablep 500//符号表的最大行数
char *KeyWord[KeyWordNum]={" ","program","var","procedure","begin","end","if",
                       "then","else","while","do","integer","real"};//12个关键词，其中第1个设置为空是为了第二个的syn从1开始
int line;//记录程序的行数
int error_count;//错误编号
int syn;//种别码
int var_count;//变量的数目
char token[20],temp1[10],temp2[10],temp3,op1[10],op2[10];//一些单词码，临时数组，操作数
char place1[10],place2[10];//变量和常量的临时单元
char strTempID[10];//新的临时变量单元
int flag;//错误标记
int vartablep=0;//符号表的总行数     datap=0
int nNXQ,nT;//四元式的个数和临时变量的个数
int i;//常用变量
FILE *fin,*fout,*fp;//文件指针

struct{
	char name[8];
	int address;
}vartable[maxvartablep];//符号表，用来记录变量
struct
{
	char op[10];
	char argv1[10];
	char argv2[10];
	char result[10];
}pQuad[maxvartablep];//四元式
//函数的定义
void init();//初始化
void P_declear();
void P_begin();
char* S_let();
void P_analize();
void S_if();
void S_while();
void P_begin();
void enter(char *name);
void lookup(char *name);
void Scaner();
void parser();
void printQuaternion();

int main()
{
    printf("----------------------编译器----------------------\n");
    getchar();
	init();//初始化
	Scaner();//词法扫描，把结果写入Scaner.txt文件中
	parser();//语法分析程序
	printf("\n-----------------------------------------\n");
	printf("程序编译完毕，结果以四元式输出如下\n\n");
	printQuaternion();//显示输出所有的四元式
	printf("\n-----------------------------------------\n");
	return 0;
}

void init()//初始化
{
	line=1;
	error_count=0;
}
void error(int n)//出错处理
{
	flag=1;
	error_count++;
	switch(n)
	{
	case 1: printf("Error %d 格式出错,缺少'program' 错于 第%d行\n",error_count,line);
		break;
    case 2: printf("Error %d 格式出错,缺少程序名 错于 第%d行\n",error_count,line);
		break;
    case 3: printf("Error %d 格式出错,缺少; 错于 第%d行\n",error_count,line);
		break;
    case 4: printf("Error %d 格式出错,缺少var 错于 第%d行\n",error_count,line);
		break;
    case 5: printf("Error %d 未写任何语句 错于 第%d行!\n",error_count,line);
		break;
    case 6: printf("Error %d 程序结束缺少. 错于 第%d行!\n",error_count,line);
		break;
	case 7: printf("Error %d 说明语句出错! 错于 第%d行\n",error_count,line);
		break;
	case 8: printf("Error %d 不存在匹配的end 错于 第%d行!\n",error_count,line);
		break;
	case 9: printf("Error %d 语句出错 错于 第%d行!\n",error_count,line);
		break;
	case 10:printf("Error %d 赋值语句出错 错于 第%d行!\n",error_count,line);
		break;
	case 11: printf("Error %d 非法标识符 错于 第%d行!\n",error_count,line);
		break;
	case 12: printf("Error %d 没有匹配的注释符'*/' 错于 第%d行!\n",error_count,line);
		break;
	case 13: printf("Error %d 非正常结束 错于 第%d行!\n",error_count,line);
		break;
	case 14: printf("Error %d 非法字符 错于 第%d行!\n",error_count,line);
		break;
	case 15: printf("Error %d if语句出错 错于 第%d行!\n",error_count,line);
		break;
	case 16: printf("Error %d while语句出错 错于 第%d行!\n",error_count,line);
		break;
	case 17: printf("Error %d 符号表溢出 错于 第%d行!\n",error_count,line);
		break;
	case 18: printf("Error %d 变量重复定义 错于 第%d行!\n",error_count,line);
		break;
	case 19: printf("Error %d 变量未定义 错于 第%d行!\n",error_count,line);
		break;
	default: break;
	}
	//return;
}

void Scaner()//词法扫描，把结果写入文件中
{
    char ch,token[40];
    int j,n;
	printf("程序正在打开需要编译的程序文件 . . .\n");
    if((fin=fopen("fin.txt","r"))==NULL)//打开需要分析的程序的文件
	{
		printf("打开需要编译的程序文件fin.txt的时候出错!\n");
		exit(0);
	}
	printf("程序正在打开存放种别码的文件 . . .\n");
    if((fout=fopen("Scaner.txt","w"))==NULL)//打开分析程序完存放种别码的文件
	{
		printf("创建存放种别码的文件Scaner.txt 的时候出错!\n");
		exit(0);
	}

    ch=getc(fin);//从文件获得一个字符
    while(ch!=EOF)//文件没有到结尾
	{
	   if(ch==';')
            line++;
	   while(ch==' '||ch=='\n'||ch=='\t')  //消除空格回车和Tab键
		 ch=getc(fin);
      if(isalpha(ch))//ch是字母字符
	  {
           token[0]=ch;j=1;
           ch=getc(fin);
           while(isalnum(ch)||ch=='_')//ch为字母或数字字母
		   {
			   token[j++]=ch;
               ch=getc(fin);
		   }
           token[j]='\0';
		   n=0;
		   while((n<KeyWordNum)&&strcmp(token,KeyWord[n])) //关键字进行匹配
			   n++;
           if(n>=KeyWordNum)
               fprintf(fout,"13 %s\n",token);//13为标识符
           else
               fprintf(fout,"%d %s\n",n,token);
       }
	  else if (isdigit(ch))//ch为数字字符
	  {
		  token[0]=ch;j=1;
          ch=getc(fin);
		  if(ch=='.')//考虑数字为小数和指数时的情况（ch=='e'||buffer=='E没有考虑）
		  {
			  token[j++]=ch;
		      ch=getc(fin);
		  }
          while(isalnum(ch))
          {
               if(isalpha(ch))
			   {
                  error(11);//printf("Error %d 非法标识符 错于 第%d行!\n",error_count,line);
			   }
               token[j++]=ch;
               ch=getc(fin);
			   if(ch=='.')
			   {
			      token[j++]=ch;
		          ch=getc(fin);
			   }
          }

          token[j]='\0';
          fprintf(fout,"14 %s\n",token);//14  NUM	常量
      }
	  else if (ch=='/')//对注释处理/**/或者//
	  {
		  ch=getc(fin);
          if(ch=='*')
          {
			  char ch1;
              ch1=getc(fin);
              do
              {
				  ch=ch1;ch1=getc(fin);
			  }while((ch!='*'||ch1!='/')&&ch1!=EOF);
			  if(ch1==EOF)
			  {
				  error(13);//printf("Error %d 非正常结束 错于 第%d行!\n",error_count,line);
			  }
              ch=getc(fin);
		  }
		  else if(ch=='/')
		  {
              ch=getc(fin);
              do
              {
				  ch=getc(fin);
			  }while(ch!='\n');//行注释
              ch=getc(fin);
		  }
		  else
          {
			  token[0]='/';token[1]='\0';
              fprintf(fout,"18 %s\n",token);//把注释和除（种别码18）区分
		  }
	  }
	  else
	  {
		  switch(ch)
		  {
              case '+': token[0]=ch;
				  token[1]='\0';
	              ch=getc(fin);
	              fprintf(fout,"15  %s\n",token);
	              break;
              case '-': token[0]=ch;
	              token[1]='\0';
	              ch=getc(fin);
	              fprintf(fout,"16  %s\n",token);
                  break;
              case '*': token[0]=ch;
                  token[1]='\0';
	              ch=getc(fin);
	              fprintf(fout,"17  %s\n",token);
	              break;//除已经在注释的时候已经处理
              case ':':token[0]=ch;
	              ch=getc(fin);
	              if(ch=='=')//赋值
				  {
		              token[1]=ch;
                      token[2]='\0';
		              fprintf(fout,"22 %s\n",token);
                      ch=getc(fin);
				  }
                  else//变量定义
				  {
		              token[1]='\0';
		              fprintf(fout,"19 %s\n",token);
				  }
	              break;
                  case ',':token[0]=ch;
                      token[1]='\0';
	                  ch=getc(fin);
	                  fprintf(fout,"20  %s\n",token);
	              break;
                  case '.':token[0]=ch;
                      token[1]='\0';
	                  ch=getc(fin);
	                  fprintf(fout,"21  %s\n",token);
	                  break;
                  case ';':token[0]=ch;
                      token[1]='\0';
	                  ch=getc(fin);
	                  fprintf(fout,"23  %s\n",token);
	                  break;
					  /*24	<	25	<=	26	<>	27	=	28	>	29	>= 关系运算符	*/
                  case '<':token[0]=ch;
	                  ch=getc(fin);
                      if(ch=='=')
					  {
		                  token[1]=ch;
                          token[2]='\0';
	                      fprintf(fout,"25 %s\n",token);
                          ch=getc(fin);
					  }
	                  else if(ch=='>')
					  {
		                  token[1]=ch;
                          token[2]='\0';
                          ch=getc(fin);
	                      fprintf(fout,"26 %s\n",token);
					  }
	                  else
					  {
		                  token[1]='\0';
		                  fprintf(fout,"24 %s\n",token);
					  }
	                  break;
                 case '=':token[0]=ch;
                        token[1]='\0';
	                    ch=getc(fin);
	                    fprintf(fout,"27  %s\n",token);
	                    break;
                 case '>':token[0]=ch;
	                    ch=getc(fin);
	                   if(ch=='=')
					   {
		                   token[1]=ch;
                           token[2]='\0';
		                   fprintf(fout,"29 %s\n",token);
                           ch=getc(fin);
					   }
                       else
					   {
		                   token[1]='\0';
		                   fprintf(fout,"28 %s\n",token);
					   }
	                   break;
                   default:error(14);//printf("Error %d 非法字符 错于 第%d行!\n",error_count,line);
	                  ch=getc(fin);
	                  break;
		  }
	  }
	}
    fclose(fin);
    fclose(fout);
}
void parser()//语法分析程序
{
	var_count=0;
	line=0;
	nNXQ=1;
	nT=1;
	if((fp=fopen("Scaner.txt","r"))==NULL)
	{
		printf("打开存放种别码的文件Scaner.txt 的时候出错!\n");
		exit(0);
	}
	fscanf(fp,"%d %s\n",&syn,token);
    if(syn==1)//program
	{
		fscanf(fp,"%d %s\n",&syn,token);
		if(syn==13)//ID	标识符（变量）
		{
			fscanf(fp,"%d %s\n",&syn,token);
			if(syn==23)//;
			{
				//fscanf(fp,"%d %s\n",&syn,token);
				line++;
			}
			else
				error(3);//printf("Error %d 格式出错,缺少; 错于 第%d行\n",error_count,line);
			fscanf(fp,"%d %s\n",&syn,token);
			if(syn==2)//var
				P_declear();//变量定义分析
			else
				error(4);//printf("Error %d 格式出错,缺少var 错于 第%d行\n",error_count,line);
			if(syn==4)//4 begin
			{
				P_begin();
				if(syn!=21)
					error(6);// printf("Error %d 程序结束缺少. 错于 第%d行!\n",error_count,line);
			}
			else
				error(5);//printf("Error %d 未写任何语句 错于 第%d行!\n",error_count,line);
		}
		else
			error(2);//printf("Error %d 格式出错,缺少程序名 错于 第%d行\n",error_count,line);
	}
	else
		error(1);//printf("Error %d 格式出错,缺少'program' 错于 第%d行\n",error_count,line);
	fclose(fp);
}

void gen(char *op,char *argv1,char *argv2,char *result)//生成四元式
{
	sprintf(pQuad[nNXQ].op,op);
	sprintf(pQuad[nNXQ].argv1,argv1);
    sprintf(pQuad[nNXQ].argv2,argv2);
	sprintf(pQuad[nNXQ].result,result);
	nNXQ++;//四元式数目增1,指向当前要产生的四元式
}
char* Newtemp()//产生新的临时变量
{
	sprintf(strTempID,"T%d",nT++);
	return strTempID;
}

void enter(char *name)//用来把变量name填入到符号表中
{
	int i;
	if(vartablep>=maxvartablep)//溢出处理
		error(17);// printf("Error %d 符号表溢出 错于 第%d行!\n",error_count,line);
	for(i=0;i<=vartablep-1;i++)
	{
		if(strcmp(vartable[i].name,name)==0)//变量重复定义 处理
		{
			error(18);// printf("Error %d 变量重复定义 错于 第%d行!\n",error_count,line);
			break;
		}
	}
	strcpy(vartable[vartablep].name,name);
	vartable[vartablep].address=vartablep;
	//datap++;
	vartablep++;//写入符号表
}

void lookup(char *name)//检查是否在符号表中存在相应此名字的入口
{
	int i;
	for(i=0;i<=vartablep-1;i++)
	{
		if(strcmp(vartable[i].name,name)==0)
		{
			return;
		}
	}
	error(19);//printf("Error %d 变量未定义 错于 第%d行!\n",error_count,line);
}


/*int merge(int p1,int p2)//把以pl 和p2为链首的两条链合并为一，作为函数值，回送合并后的链首
{
	int p;
	if(!p2)
		return p1;
	else
	{
		p=p2;
		while(atoi(pQuad[p].result))
		{
			p=atoi(pQuad[p].result);
			sprintf(pQuad[p].result,"%d",p);
		}
		return p2;
	}
}

void BackPatch(int p,int t)//功能是完成“回填”，把p 所链接的每个四元式的第四区段都填为t
{
	int w,q=p;
	while(q){
		w=atoi(pQuad[q].result);
		sprintf(pQuad[q].result,"%d",t);
		q=w;
	}
}*/
//注释的函数会在分析布尔表达式的时候要用的，时间原因就没有继续分析了
void printQuaternion()//打印四元式
{
	int nloop;
	for(nloop=1;nloop<nNXQ;nloop++)
		printf("%d: (%s,%s,%s,%s)\n",nloop,pQuad[nloop].op,pQuad[nloop].argv1,pQuad[nloop].argv2,pQuad[nloop].result);
}
void P_declear()//变量定义
{
	fscanf(fp,"%d %s\n",&syn,token);
	if(syn==4)//begin
	{
		error(7);//printf("Error %d 说明语句出错! 错于 第%d行\n",error_count,line);
		return;
	}
	while(syn!=4)//4 begin
	{
		if(syn==13)//ID
		{
			var_count++;
			enter(token);//写人符号表
		}
		else
		{
			if(syn==19)//:
			{
			    fscanf(fp,"%d %s\n",&syn,token);
			    if(syn==11 ||syn==12)//11 integer12real
				{
				    fscanf(fp,"%d %s\n",&syn,token);
			        if(syn==23)//;
				        line++;
				    else
					    error(3);//printf("Error %d 格式出错,缺少; 错于 第%d行\n",error_count,line);
				}
			    else
				    error(7);//printf("Error %d 说明语句出错! 错于 第%d行\n",error_count,line);

			}
		    else if(syn!=20)//20	,
			{
			    error(7);//printf("Error %d 说明语句出错! 错于 第%d行\n",error_count,line);
			    //return;
			}
		}
		fscanf(fp,"%d %s\n",&syn,token);
	}
}

void P_begin()//begin分析，语句段开始
{
	line++;
	fscanf(fp,"%d %s\n",&syn,token);
	P_analize();
	if(syn!=5)//5 end
	{
		error(8);//printf("Error %d 不存在匹配的end 错于 第%d行!\n",error_count,line);
		line++;
		return;
	}
	else
	{
		line++;
		fscanf(fp,"%d %s\n",&syn,token);
	}
}

void P_analize()//语句的词法和语义分析
{
	char eplace[10],strTemp[10];
	switch(syn)
	{
	case 13:lookup(token);//ID	标识符（变量）
		strcpy(strTemp,token);
		//printf("%s",strTemp);
		strcpy(eplace,S_let());
		gen(":=",eplace,"-",strTemp);
		break;
	case 6:S_if();
		break;
	case 9:S_while();
		break;
	default:error(9);//printf("Error %d 语句出错 错于 第%d行!\n",error_count,line);
		return;
	}
	if(syn==23)//23	;
	{
		line++;
		fscanf(fp,"%d %s\n",&syn,token);
		if(syn==8)//8	else
			error(15);//printf("Error %d if语句出错 错于 第%d行!\n",error_count,line);
		switch(syn)
		{
		case 13:P_analize();
			break;
		case 9:P_analize();
			break;
		case 6:P_analize();
			break;
		default:return;
		}
	}
}
void L_analize()//算术运算的语法和语义分析
{
	if(syn==15||syn==16)//+,-
	{
		strcpy(op1,token);
	    fscanf(fp,"%d %s\n",&syn,token);
		if(syn==13||syn==14)//常量和变量
		{
			if(syn==13)
				lookup(token);
            strcpy(place2,token);
			fscanf(fp,"%d %s\n",&syn,token);
			if(syn==17||syn==18)//*,/
			{
				strcpy(op2,token);
				fscanf(fp,"%d %s\n",&syn,token);
                if(syn==13||syn==14)
				{
					if(syn==13)
						lookup(token);
                    strcpy(temp1,Newtemp());
					gen(op2,place2,token,temp1);
									//fscanf(fp,"%d %s\n",&syn,token);
				}
                strcpy(temp2,Newtemp());
				gen(op1,place1,temp1,temp2);
				strcpy(place1,temp2);
                strcpy(temp1,temp2);
				fscanf(fp,"%d %s\n",&syn,token);
			}
			else
			{
				strcpy(temp1,Newtemp());
				gen(op1, place1,place2,temp1);
			}
		}
	}
    else
	{
		strcpy(op1,token);
		fscanf(fp,"%d %s\n",&syn,token);
		if(syn==13||syn==14)
		{
			if(syn==13)
			   lookup(token);
			strcpy(place2,token);
		}
		strcpy(temp1,Newtemp());
		gen(op1, place1,place2,temp1);
        strcpy(place1,temp1);
        fscanf(fp,"%d %s\n",&syn,token);
	}

}

char* S_let()//赋值语句和条件、循环语句的语法和语义分析
{
	int flag1=0;//负号标识
	fscanf(fp,"%d %s\n",&syn,token);
	if(syn==22)
	{
		fscanf(fp,"%d %s\n",&syn,token);
		if(syn==17||syn==18)
			error(10);//printf("Error %d 赋值语句出错 错于 第%d行!\n",error_count,line);
		if(syn==16)//负号的处理
		{
			fscanf(fp,"%d %s\n",&syn,token);
			flag1=1;
		}
		while(syn!=23)
		{
			if(syn==13||syn==14)
			{
				if(syn==13)
					lookup(token);
				if(flag1)
				{
					char token1[10]="-";
					strcat(token1,token);
					strcpy(token,token1);
					flag=0;
				}
				strcpy(place1,token);
				fscanf(fp,"%d %s\n",&syn,token);
				if(syn>=15&&syn<=18)//关系运算符	四元式处理
				{
					if(syn==15||syn==16)
					{
						strcpy(op1,token);
					    fscanf(fp,"%d %s\n",&syn,token);
						if(syn==13||syn==14)
						{
							if(syn==13)
								lookup(token);
							strcpy(place2,token);
							fscanf(fp,"%d %s\n",&syn,token);
							if(syn==17||syn==18)
							{
								strcpy(op2,token);
								fscanf(fp,"%d %s\n",&syn,token);
								if(syn==13||syn==14)
								{
									if(syn==13)
										lookup(token);
									strcpy(temp1,Newtemp());
									gen(op2,place2,token,temp1);
									//fscanf(fp,"%d %s\n",&syn,token);
								}

                                strcpy(temp2,Newtemp());
								gen(op1,place1,temp1,temp2);
								//strcpy(place1," ");
                                strcpy(temp1,temp2);
								strcpy(place1,temp2);
								fscanf(fp,"%d %s\n",&syn,token);
								/*while(syn>=15&&syn<=18)
								{
									L_analize();
								}*/
							}
							else
							{
								strcpy(temp1,Newtemp());
								gen(op1,place1,place2,temp1);
								strcpy(place1,temp1);
								while(syn>=15&&syn<=18)
									L_analize();
							}
						}


					}
					else
					{
						strcpy(op1,token);
					    fscanf(fp,"%d %s\n",&syn,token);
						if(syn==13||syn==14)
						{
							 if(syn==13)
								 lookup(token);
							 strcpy(place2,token);
						}
					    strcpy(temp1,Newtemp());
						gen(op1,place1,place2,temp1);
						strcpy(place1,temp1);
						fscanf(fp,"%d %s\n",&syn,token);
						while(syn>=15&&syn<=18)
						{
							L_analize();
						}
					}
					   return temp1;
				}
				else if(syn==13||syn==14)
					error(3);
				else
					return place1;

			}
			if(syn==8)
				break;
		}
		//printf("%s\n",token);

	}
	else
	{
		error(10);
	}
	return(0);
}

void S_if()//条件语句的语法分析及语义分析程序设计
{
	fscanf(fp,"%d %s\n",&syn,token);
	if(syn==13||syn==14)
	{
		if(syn==13)
			lookup(token);
		strcpy(place1,token);
		fscanf(fp,"%d %s\n",&syn,token);
		if(syn>=24 && syn<=29)
		{
			strcpy(op1,"j");
			strcat(op1,token);
			fscanf(fp,"%d %s\n",&syn,token);
			if(syn==13||syn==14)
			{
				if(syn==13)
					lookup(token);
				strcpy(place2,token);
			    i=nNXQ+2;
				_itoa( i, temp1, 10 );
/*itoa(i,num,10);
  i   需要转换成字符的数字
  num   转换后保存字符的变量
  10   转换数字的基数（进制）10就是说按照10进制转换数字。还可以是2，8，16等等你喜欢的进制类型*/
				gen(op1,place1,place2,temp1);
				i=nNXQ+3;
				_itoa( i, temp1, 10 );
				gen("j","-","-",temp1);
			}
			else
				error(11);//printf("Error %d 非法标识符 错于 第%d行!\n",error_count,line);
		}
	}
	else
		error(11);//printf("Error %d 非法标识符 错于 第%d行!\n",error_count,line);
	fscanf(fp,"%d %s\n",&syn,token);
	if(syn==7)//then语句的处理
	{
		char eplace[10],strTemp[10];
		fscanf(fp,"%d %s\n",&syn,token);
		switch(syn)
		{
		case 13:lookup(token);
			strcpy(strTemp,token);
		//printf("%s",strTemp);
		    strcpy(eplace,S_let());
		    gen(":=",eplace,"-",strTemp);
			break;
		case 6:S_if();
			break;
		case 9:S_while();
			break;
		case 4:P_begin();
			break;
		default:error(9);
			break;
		}
	}
	else
		error(11);//printf("Error %d 非法标识符 错于 第%d行!\n",error_count,line);
	if(syn==8)//else语句的处理
	{
		char eplace[10],strTemp[10];
		fscanf(fp,"%d %s\n",&syn,token);
		switch(syn)
		{
		case 13:lookup(token);
			strcpy(strTemp,token);
		//printf("%s",strTemp);
		    strcpy(eplace,S_let());
		    gen(":=",eplace,"-",strTemp);
			break;
		case 6:S_if();
			break;
		case 9:S_while();
			break;
		case 4:P_begin();
			break;
		default:error(9);
			break;
		}
	}
}

void S_while()//循环语句的语法分析及语义分析程序设计
{
	int j;
	fscanf(fp,"%d %s\n",&syn,token);
	if(syn==13||syn==14)
	{
		if(syn==13)
			lookup(token);
		strcpy(place1,token);
		fscanf(fp,"%d %s\n",&syn,token);
		if(syn>=24 && syn<=29)
		{
			strcpy(op1,"j");
			strcat(op1,token);
			fscanf(fp,"%d %s\n",&syn,token);
			if(syn==13||syn==14)
			{
				if(syn==13)
					lookup(token);
				strcpy(place2,token);
			    i=nNXQ+2;
				j=nNXQ;//保存当前四元式的地址
				_itoa( i, temp1, 10 );
				gen(op1,place1,place2,temp1);
				i=nNXQ+4;
				_itoa( i, temp1, 10 );
				gen("j","-","-",temp1);
			}
			else
				error(16);//printf("Error %d while语句出错 错于 第%d行!\n",error_count,line);
		}
	}
	else
		error(16);//printf("Error %d while语句出错 错于 第%d行!\n",error_count,line);
	fscanf(fp,"%d %s\n",&syn,token);
	if(syn==10)
	{
		char eplace[10],strTemp[10];
		fscanf(fp,"%d %s\n",&syn,token);
		switch(syn)
		{
		case 13:lookup(token);
			strcpy(strTemp,token);
		//printf("%s",strTemp);
		    strcpy(eplace,S_let());
		    gen(":=",eplace,"-",strTemp);
			_itoa( j, temp1, 10 );
			gen("j","-","-",temp1);//跳回原while语句的四元式地址
			break;
		case 6:S_if();
			break;
		case 9:S_while();
			break;
		case 4:P_begin();
			break;
		default:error(9);//printf("Error %d 语句出错 错于 第%d行!\n",error_count,line);
			break;
		}
	}
	else
		error(16);//printf("Error %d while语句出错 错于 第%d行!\n",error_count,line);
}




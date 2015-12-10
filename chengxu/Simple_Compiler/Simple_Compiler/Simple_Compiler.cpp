#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>

#define KeyWordNum 13//�ؼ��ֵĸ���
#define maxvartablep 500//���ű���������
char *KeyWord[KeyWordNum]={" ","program","var","procedure","begin","end","if",
                       "then","else","while","do","integer","real"};//12���ؼ��ʣ����е�1������Ϊ����Ϊ�˵ڶ�����syn��1��ʼ
int line;//��¼���������
int error_count;//������
int syn;//�ֱ���
int var_count;//��������Ŀ
char token[20],temp1[10],temp2[10],temp3,op1[10],op2[10];//һЩ�����룬��ʱ���飬������
char place1[10],place2[10];//�����ͳ�������ʱ��Ԫ
char strTempID[10];//�µ���ʱ������Ԫ
int flag;//������
int vartablep=0;//���ű��������     datap=0
int nNXQ,nT;//��Ԫʽ�ĸ�������ʱ�����ĸ���
int i;//���ñ���
FILE *fin,*fout,*fp;//�ļ�ָ��

struct{
	char name[8];
	int address;
}vartable[maxvartablep];//���ű�������¼����
struct
{
	char op[10];
	char argv1[10];
	char argv2[10];
	char result[10];
}pQuad[maxvartablep];//��Ԫʽ
//�����Ķ���
void init();//��ʼ��
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
    printf("----------------------������----------------------\n");
    getchar();
	init();//��ʼ��
	Scaner();//�ʷ�ɨ�裬�ѽ��д��Scaner.txt�ļ���
	parser();//�﷨��������
	printf("\n-----------------------------------------\n");
	printf("���������ϣ��������Ԫʽ�������\n\n");
	printQuaternion();//��ʾ������е���Ԫʽ
	printf("\n-----------------------------------------\n");
	return 0;
}

void init()//��ʼ��
{
	line=1;
	error_count=0;
}
void error(int n)//������
{
	flag=1;
	error_count++;
	switch(n)
	{
	case 1: printf("Error %d ��ʽ����,ȱ��'program' ���� ��%d��\n",error_count,line);
		break;
    case 2: printf("Error %d ��ʽ����,ȱ�ٳ����� ���� ��%d��\n",error_count,line);
		break;
    case 3: printf("Error %d ��ʽ����,ȱ��; ���� ��%d��\n",error_count,line);
		break;
    case 4: printf("Error %d ��ʽ����,ȱ��var ���� ��%d��\n",error_count,line);
		break;
    case 5: printf("Error %d δд�κ���� ���� ��%d��!\n",error_count,line);
		break;
    case 6: printf("Error %d �������ȱ��. ���� ��%d��!\n",error_count,line);
		break;
	case 7: printf("Error %d ˵��������! ���� ��%d��\n",error_count,line);
		break;
	case 8: printf("Error %d ������ƥ���end ���� ��%d��!\n",error_count,line);
		break;
	case 9: printf("Error %d ������ ���� ��%d��!\n",error_count,line);
		break;
	case 10:printf("Error %d ��ֵ������ ���� ��%d��!\n",error_count,line);
		break;
	case 11: printf("Error %d �Ƿ���ʶ�� ���� ��%d��!\n",error_count,line);
		break;
	case 12: printf("Error %d û��ƥ���ע�ͷ�'*/' ���� ��%d��!\n",error_count,line);
		break;
	case 13: printf("Error %d ���������� ���� ��%d��!\n",error_count,line);
		break;
	case 14: printf("Error %d �Ƿ��ַ� ���� ��%d��!\n",error_count,line);
		break;
	case 15: printf("Error %d if������ ���� ��%d��!\n",error_count,line);
		break;
	case 16: printf("Error %d while������ ���� ��%d��!\n",error_count,line);
		break;
	case 17: printf("Error %d ���ű���� ���� ��%d��!\n",error_count,line);
		break;
	case 18: printf("Error %d �����ظ����� ���� ��%d��!\n",error_count,line);
		break;
	case 19: printf("Error %d ����δ���� ���� ��%d��!\n",error_count,line);
		break;
	default: break;
	}
	//return;
}

void Scaner()//�ʷ�ɨ�裬�ѽ��д���ļ���
{
    char ch,token[40];
    int j,n;
	printf("�������ڴ���Ҫ����ĳ����ļ� . . .\n");
    if((fin=fopen("fin.txt","r"))==NULL)//����Ҫ�����ĳ�����ļ�
	{
		printf("����Ҫ����ĳ����ļ�fin.txt��ʱ�����!\n");
		exit(0);
	}
	printf("�������ڴ򿪴���ֱ�����ļ� . . .\n");
    if((fout=fopen("Scaner.txt","w"))==NULL)//�򿪷������������ֱ�����ļ�
	{
		printf("��������ֱ�����ļ�Scaner.txt ��ʱ�����!\n");
		exit(0);
	}

    ch=getc(fin);//���ļ����һ���ַ�
    while(ch!=EOF)//�ļ�û�е���β
	{
	   if(ch==';')
            line++;
	   while(ch==' '||ch=='\n'||ch=='\t')  //�����ո�س���Tab��
		 ch=getc(fin);
      if(isalpha(ch))//ch����ĸ�ַ�
	  {
           token[0]=ch;j=1;
           ch=getc(fin);
           while(isalnum(ch)||ch=='_')//chΪ��ĸ��������ĸ
		   {
			   token[j++]=ch;
               ch=getc(fin);
		   }
           token[j]='\0';
		   n=0;
		   while((n<KeyWordNum)&&strcmp(token,KeyWord[n])) //�ؼ��ֽ���ƥ��
			   n++;
           if(n>=KeyWordNum)
               fprintf(fout,"13 %s\n",token);//13Ϊ��ʶ��
           else
               fprintf(fout,"%d %s\n",n,token);
       }
	  else if (isdigit(ch))//chΪ�����ַ�
	  {
		  token[0]=ch;j=1;
          ch=getc(fin);
		  if(ch=='.')//��������ΪС����ָ��ʱ�������ch=='e'||buffer=='Eû�п��ǣ�
		  {
			  token[j++]=ch;
		      ch=getc(fin);
		  }
          while(isalnum(ch))
          {
               if(isalpha(ch))
			   {
                  error(11);//printf("Error %d �Ƿ���ʶ�� ���� ��%d��!\n",error_count,line);
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
          fprintf(fout,"14 %s\n",token);//14  NUM	����
      }
	  else if (ch=='/')//��ע�ʹ���/**/����//
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
				  error(13);//printf("Error %d ���������� ���� ��%d��!\n",error_count,line);
			  }
              ch=getc(fin);
		  }
		  else if(ch=='/')
		  {
              ch=getc(fin);
              do
              {
				  ch=getc(fin);
			  }while(ch!='\n');//��ע��
              ch=getc(fin);
		  }
		  else
          {
			  token[0]='/';token[1]='\0';
              fprintf(fout,"18 %s\n",token);//��ע�ͺͳ����ֱ���18������
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
	              break;//���Ѿ���ע�͵�ʱ���Ѿ�����
              case ':':token[0]=ch;
	              ch=getc(fin);
	              if(ch=='=')//��ֵ
				  {
		              token[1]=ch;
                      token[2]='\0';
		              fprintf(fout,"22 %s\n",token);
                      ch=getc(fin);
				  }
                  else//��������
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
					  /*24	<	25	<=	26	<>	27	=	28	>	29	>= ��ϵ�����	*/
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
                   default:error(14);//printf("Error %d �Ƿ��ַ� ���� ��%d��!\n",error_count,line);
	                  ch=getc(fin);
	                  break;
		  }
	  }
	}
    fclose(fin);
    fclose(fout);
}
void parser()//�﷨��������
{
	var_count=0;
	line=0;
	nNXQ=1;
	nT=1;
	if((fp=fopen("Scaner.txt","r"))==NULL)
	{
		printf("�򿪴���ֱ�����ļ�Scaner.txt ��ʱ�����!\n");
		exit(0);
	}
	fscanf(fp,"%d %s\n",&syn,token);
    if(syn==1)//program
	{
		fscanf(fp,"%d %s\n",&syn,token);
		if(syn==13)//ID	��ʶ����������
		{
			fscanf(fp,"%d %s\n",&syn,token);
			if(syn==23)//;
			{
				//fscanf(fp,"%d %s\n",&syn,token);
				line++;
			}
			else
				error(3);//printf("Error %d ��ʽ����,ȱ��; ���� ��%d��\n",error_count,line);
			fscanf(fp,"%d %s\n",&syn,token);
			if(syn==2)//var
				P_declear();//�����������
			else
				error(4);//printf("Error %d ��ʽ����,ȱ��var ���� ��%d��\n",error_count,line);
			if(syn==4)//4 begin
			{
				P_begin();
				if(syn!=21)
					error(6);// printf("Error %d �������ȱ��. ���� ��%d��!\n",error_count,line);
			}
			else
				error(5);//printf("Error %d δд�κ���� ���� ��%d��!\n",error_count,line);
		}
		else
			error(2);//printf("Error %d ��ʽ����,ȱ�ٳ����� ���� ��%d��\n",error_count,line);
	}
	else
		error(1);//printf("Error %d ��ʽ����,ȱ��'program' ���� ��%d��\n",error_count,line);
	fclose(fp);
}

void gen(char *op,char *argv1,char *argv2,char *result)//������Ԫʽ
{
	sprintf(pQuad[nNXQ].op,op);
	sprintf(pQuad[nNXQ].argv1,argv1);
    sprintf(pQuad[nNXQ].argv2,argv2);
	sprintf(pQuad[nNXQ].result,result);
	nNXQ++;//��Ԫʽ��Ŀ��1,ָ��ǰҪ��������Ԫʽ
}
char* Newtemp()//�����µ���ʱ����
{
	sprintf(strTempID,"T%d",nT++);
	return strTempID;
}

void enter(char *name)//�����ѱ���name���뵽���ű���
{
	int i;
	if(vartablep>=maxvartablep)//�������
		error(17);// printf("Error %d ���ű���� ���� ��%d��!\n",error_count,line);
	for(i=0;i<=vartablep-1;i++)
	{
		if(strcmp(vartable[i].name,name)==0)//�����ظ����� ����
		{
			error(18);// printf("Error %d �����ظ����� ���� ��%d��!\n",error_count,line);
			break;
		}
	}
	strcpy(vartable[vartablep].name,name);
	vartable[vartablep].address=vartablep;
	//datap++;
	vartablep++;//д����ű�
}

void lookup(char *name)//����Ƿ��ڷ��ű��д�����Ӧ�����ֵ����
{
	int i;
	for(i=0;i<=vartablep-1;i++)
	{
		if(strcmp(vartable[i].name,name)==0)
		{
			return;
		}
	}
	error(19);//printf("Error %d ����δ���� ���� ��%d��!\n",error_count,line);
}


/*int merge(int p1,int p2)//����pl ��p2Ϊ���׵��������ϲ�Ϊһ����Ϊ����ֵ�����ͺϲ��������
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

void BackPatch(int p,int t)//��������ɡ��������p �����ӵ�ÿ����Ԫʽ�ĵ������ζ���Ϊt
{
	int w,q=p;
	while(q){
		w=atoi(pQuad[q].result);
		sprintf(pQuad[q].result,"%d",t);
		q=w;
	}
}*/
//ע�͵ĺ������ڷ����������ʽ��ʱ��Ҫ�õģ�ʱ��ԭ���û�м���������
void printQuaternion()//��ӡ��Ԫʽ
{
	int nloop;
	for(nloop=1;nloop<nNXQ;nloop++)
		printf("%d: (%s,%s,%s,%s)\n",nloop,pQuad[nloop].op,pQuad[nloop].argv1,pQuad[nloop].argv2,pQuad[nloop].result);
}
void P_declear()//��������
{
	fscanf(fp,"%d %s\n",&syn,token);
	if(syn==4)//begin
	{
		error(7);//printf("Error %d ˵��������! ���� ��%d��\n",error_count,line);
		return;
	}
	while(syn!=4)//4 begin
	{
		if(syn==13)//ID
		{
			var_count++;
			enter(token);//д�˷��ű�
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
					    error(3);//printf("Error %d ��ʽ����,ȱ��; ���� ��%d��\n",error_count,line);
				}
			    else
				    error(7);//printf("Error %d ˵��������! ���� ��%d��\n",error_count,line);

			}
		    else if(syn!=20)//20	,
			{
			    error(7);//printf("Error %d ˵��������! ���� ��%d��\n",error_count,line);
			    //return;
			}
		}
		fscanf(fp,"%d %s\n",&syn,token);
	}
}

void P_begin()//begin���������ο�ʼ
{
	line++;
	fscanf(fp,"%d %s\n",&syn,token);
	P_analize();
	if(syn!=5)//5 end
	{
		error(8);//printf("Error %d ������ƥ���end ���� ��%d��!\n",error_count,line);
		line++;
		return;
	}
	else
	{
		line++;
		fscanf(fp,"%d %s\n",&syn,token);
	}
}

void P_analize()//���Ĵʷ����������
{
	char eplace[10],strTemp[10];
	switch(syn)
	{
	case 13:lookup(token);//ID	��ʶ����������
		strcpy(strTemp,token);
		//printf("%s",strTemp);
		strcpy(eplace,S_let());
		gen(":=",eplace,"-",strTemp);
		break;
	case 6:S_if();
		break;
	case 9:S_while();
		break;
	default:error(9);//printf("Error %d ������ ���� ��%d��!\n",error_count,line);
		return;
	}
	if(syn==23)//23	;
	{
		line++;
		fscanf(fp,"%d %s\n",&syn,token);
		if(syn==8)//8	else
			error(15);//printf("Error %d if������ ���� ��%d��!\n",error_count,line);
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
void L_analize()//����������﷨���������
{
	if(syn==15||syn==16)//+,-
	{
		strcpy(op1,token);
	    fscanf(fp,"%d %s\n",&syn,token);
		if(syn==13||syn==14)//�����ͱ���
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

char* S_let()//��ֵ����������ѭ�������﷨���������
{
	int flag1=0;//���ű�ʶ
	fscanf(fp,"%d %s\n",&syn,token);
	if(syn==22)
	{
		fscanf(fp,"%d %s\n",&syn,token);
		if(syn==17||syn==18)
			error(10);//printf("Error %d ��ֵ������ ���� ��%d��!\n",error_count,line);
		if(syn==16)//���ŵĴ���
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
				if(syn>=15&&syn<=18)//��ϵ�����	��Ԫʽ����
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

void S_if()//���������﷨��������������������
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
  i   ��Ҫת�����ַ�������
  num   ת���󱣴��ַ��ı���
  10   ת�����ֵĻ��������ƣ�10����˵����10����ת�����֡���������2��8��16�ȵ���ϲ���Ľ�������*/
				gen(op1,place1,place2,temp1);
				i=nNXQ+3;
				_itoa( i, temp1, 10 );
				gen("j","-","-",temp1);
			}
			else
				error(11);//printf("Error %d �Ƿ���ʶ�� ���� ��%d��!\n",error_count,line);
		}
	}
	else
		error(11);//printf("Error %d �Ƿ���ʶ�� ���� ��%d��!\n",error_count,line);
	fscanf(fp,"%d %s\n",&syn,token);
	if(syn==7)//then���Ĵ���
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
		error(11);//printf("Error %d �Ƿ���ʶ�� ���� ��%d��!\n",error_count,line);
	if(syn==8)//else���Ĵ���
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

void S_while()//ѭ�������﷨��������������������
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
				j=nNXQ;//���浱ǰ��Ԫʽ�ĵ�ַ
				_itoa( i, temp1, 10 );
				gen(op1,place1,place2,temp1);
				i=nNXQ+4;
				_itoa( i, temp1, 10 );
				gen("j","-","-",temp1);
			}
			else
				error(16);//printf("Error %d while������ ���� ��%d��!\n",error_count,line);
		}
	}
	else
		error(16);//printf("Error %d while������ ���� ��%d��!\n",error_count,line);
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
			gen("j","-","-",temp1);//����ԭwhile������Ԫʽ��ַ
			break;
		case 6:S_if();
			break;
		case 9:S_while();
			break;
		case 4:P_begin();
			break;
		default:error(9);//printf("Error %d ������ ���� ��%d��!\n",error_count,line);
			break;
		}
	}
	else
		error(16);//printf("Error %d while������ ���� ��%d��!\n",error_count,line);
}




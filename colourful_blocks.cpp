#include <iostream>
#include <iomanip>
#include <sstream>
#include <windows.h>
#include <ctime>
#include <cstdio>
#include <queue>
#include <cmath>
#define MAX_SIZE 21
#define qwq MessageBox(NULL,"此功能正在研发中，关注作者以获取持续更新！","Information",MB_OK|MB_ICONINFORMATION);
#define wrong_input MessageBox(NULL,"输入有误，请重新输入","Error",MB_ICONERROR|MB_OK);
using namespace std;
int width,height,ans,linkcnt,c;
int mappc[MAX_SIZE][MAX_SIZE];//colour
bool mappv[MAX_SIZE][MAX_SIZE];//visited
bool linkblock[MAX_SIZE][MAX_SIZE];
TCHAR textbuf[201];
HANDLE hout,inh;
CONSOLE_SCREEN_BUFFER_INFO buf_pos;
stringstream qstream("");
int inputnum;
class POS
{
	public :
		int x,y;
		POS(int a=0,int b=0)
		{
			x=a; y=b;
		}
};
class block
{
	public:
		int c;//colour
		block(int a=FOREGROUND_INTENSITY)
		{
			c=a;
		}
		virtual inline int score()
		{
			return 1;
		}
		virtual ~block() {;}
};
class sblock : public block
{
	public:
		int s;//score
		sblock(int a=FOREGROUND_INTENSITY,int b=2)
		:block(a)
		{
			s=b;
		}
		virtual inline int score()
		{
			return s;
		}
		virtual ~sblock() {;}
};
block *mapp[MAX_SIZE][MAX_SIZE];
char show_mapp[MAX_SIZE][MAX_SIZE];
inline int rand(int l,int r)
{
	return rand()%(r-l+1)+l;
}
DWORD WINAPI input(LPVOID pM)
{
	cin>>inputnum;
}
void show()
{
	system("cls");
	block *t;
	for(int i=1;i<=height;i++)
	{
		for(int j=1;j<=width;j++)
		{
			t=mapp[i][j];
			if(t!=NULL)
			{
				if(t->score()==1)
				{
					show_mapp[i][j]='*';
					mappc[i][j]=t->c;
				}
				else if(t->score()==0)
				{
					show_mapp[i][j]='M';
					mappc[i][j]=t->c;
				}
				else
				{
					show_mapp[i][j]=t->score()+'0';
					mappc[i][j]=t->c;
				}
			}
		}
	}
	for(int i=0;i<=height+1;i++)
	{
		for(int j=0;j<=width+1;j++)
		{
			SetConsoleTextAttribute(hout,FOREGROUND_INTENSITY|mappc[i][j]);
			cout<<show_mapp[i][j];
		}
		cout<<endl;
	}
	SetConsoleTextAttribute(hout,FOREGROUND_INTENSITY);
	cout<<"score:"<<ans<<endl;
	cout<<"输入交换点的坐标：行 列 \n(输入0结束游戏)"<<endl;
}
void initm(bool m[MAX_SIZE][MAX_SIZE],int lx,int ly,int rx,int ry)
{
	for(int i=lx;i<=rx;i++)
	{
		for(int j=ly;j<=ry;j++)
		{
			m[i][j]=0;
		}
	}
}
int d[4][2]={{0,1},{0,-1},{1,0},{-1,0}};
void bfs(int x,int y)
{
	queue<POS> pos,road;
	int cnt=1,nx,ny;
	mappv[x][y]=1;
	pos.push(POS(x,y));
	road.push(POS(x,y));
	while(!pos.empty())
	{
		x=pos.front().x; y=pos.front().y;
		for(int i=0;i<4;i++)
		{
			nx=x+d[i][0];
			ny=y+d[i][1];
			if(1<=nx&&nx<=height&&1<=ny&&ny<=width&&mappv[nx][ny]==0&&mapp[nx][ny]!=NULL&&mapp[nx][ny]->c==mapp[x][y]->c)
			{
				pos.push(POS(nx,ny));
				road.push(POS(nx,ny));
				mappv[nx][ny]=1;
				cnt++;
			}
		}
		pos.pop();
	}
	if(cnt>=linkcnt)
	{
		while(!road.empty())
		{
			linkblock[road.front().x][road.front().y]=1;
			road.pop();
		}
	}
}
void award(int n)
{
	wsprintf(textbuf,TEXT("你得了%d分！"),n);
	MessageBox(NULL,textbuf,"Congratulation",MB_OK);
	ans+=n;
}
int search(int lx=1,int ly=1,int rx=height,int ry=width)
{
	int add=0;
	initm(mappv,lx,ly,rx,ry);
	initm(linkblock,lx,ly,rx,ry);
	for(int i=lx;i<=rx;i++)
	{
		for(int j=ly;j<=ry;j++)
		{
			if(mappv[i][j]==0) bfs(i,j);
		}
	}
	for(int i=rx;i>=lx;i--)
	{
		for(int j=ly;j<=ry;j++)
		{
			if(linkblock[i][j]) add++;
		}
	}
	return add;
}
void clear_blocks(int lx=1,int ly=1,int rx=height,int ry=width)
{
	int k;
	for(int i=rx;i>=lx;i--)
	{
		for(int j=ly;j<=ry;j++)
		{
			if(linkblock[i][j])
			{
				if(mapp[i][j]!=NULL)
				{
					delete mapp[i][j];					
				}
				k=i-1;
				while(k>=1&&linkblock[k][j]) k--;
				if(k>=1)
				{
					mapp[i][j]=mapp[k][j];
					mapp[k][j]=NULL;
					linkblock[k][j]=1;
				}
				else
				{
					mapp[i][j]=new block(pow(2,rand(0,2)));
				}	
			}
		}
	}
}
void show_menu()
{
	system("cls");
	cout<<setfill('-')<<setw(35)<<"COLOURFUL BLOCKS"<<setw(20)<<""<<setfill(' ')<<endl;
	cout<<"\n\n\n";
	cout<<"1.开始游戏\n";
	cout<<"\n\n\n";
	cout<<"2.新建地图\n";
	cout<<"\n\n\n";
	cout<<"3.帮助\n";
	cout<<"\n\n\n";
	cout<<setfill('-')<<setw(55)<<""<<setfill(' ')<<endl;
}
void newgame()
{
	for(int i=0;i<=height+1;i++)
	{
		for(int j=0;j<=width+1;j++)
		{
			if(i==0||i==height+1)
			{
				show_mapp[i][j]='-';
				continue;
			}
			if(j==0||j==width+1)
			{
				show_mapp[i][j]='|';
				continue;
			}
			mapp[i][j]=new block(pow(2,rand(0,2)));
		}
	}	
}
void game_over()
{
	for(int i=1;i<=height;i++)
	{
		for(int j=1;j<=width;j++)
		{
			if(mapp[i][j]!=NULL) delete mapp[i][j];
		}
	}		
}
void init()
{
	hout=GetStdHandle(STD_OUTPUT_HANDLE);
	cout<<setfill('-')<<setw(35)<<"COLOURFUL BLOCKS"<<setw(20)<<""<<setfill(' ')<<endl;
	cout<<setw(55)<<"AUTHOR:tlzx\n";
	Sleep(1000);
	srand(time(0));
	COORD pos;
	A:
	inh=CreateThread(NULL,0,input,NULL,CREATE_SUSPENDED,NULL);
	show_menu();
	GetConsoleScreenBufferInfo(hout,&buf_pos);
	for(int i=1;i<=10;i++)
	{
		for(int j=1;j<=25;j++)
		{
			if(rand(1,3)==1)
			{
				pos.X=j+20;
				pos.Y=i+2;
				SetConsoleCursorPosition(hout,pos);
				SetConsoleTextAttribute(hout,FOREGROUND_INTENSITY+pow(2,rand(0,2)));
				cout<<"*";
			}
		}
	}
	SetConsoleCursorPosition(hout,buf_pos.dwCursorPosition);
	ResumeThread(inh);
	while(1)
	{
		GetConsoleScreenBufferInfo(hout,&buf_pos);
		for(int i=1;i<=4;i++)
		{
			pos.X=rand(1,25)+20;
			pos.Y=rand(1,10)+2;
			SetConsoleCursorPosition(hout,pos);
			SetConsoleTextAttribute(hout,FOREGROUND_INTENSITY+pow(2,rand(0,2)));
			if(rand(1,2)==1) cout<<"*";
			else cout<<" ";			
		}
		SetConsoleCursorPosition(hout,buf_pos.dwCursorPosition);
		SetConsoleTextAttribute(hout,FOREGROUND_INTENSITY);
		if(WaitForSingleObject(inh,150)==WAIT_OBJECT_0) break;
	}
	CloseHandle(inh);
	c=inputnum;
	switch(c)
	{
		case 1:
		{
			B:
			system("cls");
			cout<<"请输入：\n";
			cout<<"1.经典模式\n";
			cout<<"2.自定义模式\n";
			cout<<"3.特殊地图\n";
			cin>>c;
			switch(c)
			{
				case 1:
				{
					width=5;
					height=5;
					linkcnt=5;
					newgame();
					break;				
				}
				case 2:
				{
					qwq
					goto B;
					break;
				}
				case 3:
				{
					qwq
					goto B;
					break;
				}
				default:
				{
					wrong_input
					cin.clear();
					cin.sync();
					goto B;
				}
			}
			break;
		}
		case 2:
		{
			qwq
			goto A;
			break;
		}
		case 3:
		{
			qwq
			goto A;
			break;
		}
		default:
		{
			wrong_input
			cin.clear();
			cin.sync();
			goto A;
		}
	}
}
void play()
{
	int x1,x2,y1,y2,t; 
	while(1)
	{
		show();
		cin>>x1;
		if(x1==0)
		{
			game_over();
			return;
		}
		cin>>y1>>x2>>y2;
		if(x1<1||x1>height||x2<1||x2>height||y1<1||y1>width||y2<1||y2>width)
		{
			wrong_input
			cin.clear();
			cin.sync();
			continue;
		}
		if(x1==x2&&y1==y2||abs(x1-x2)+abs(y1-y2)>1)
		{
			MessageBox(NULL,"请输入两个相邻的点","Error",MB_OK|MB_ICONERROR);
			cin.clear();
			cin.sync();
			continue;
		}
		swap(mapp[x1][y1],mapp[x2][y2]);
		t=search();
		if(t==0)
		{
			MessageBox(NULL,"交换无效，请重新输入","Error",MB_OK|MB_ICONERROR);
			swap(mapp[x1][y1],mapp[x2][y2]);
			continue;
		}
		clear_blocks();
		award(t);
	}
}
int main()
{
	init();
	play(); 
	return 0;
}

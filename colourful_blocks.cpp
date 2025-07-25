#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <windows.h>
#include <ctime>
#include <cstdio>
#include <queue>
#include <vector>
#include <list>
#include <cmath>
#include <conio.h>
#define MAX_SIZE 21
#define qwq MessageBox(NULL,"此功能正在研发中，关注作者以获取持续更新！","Information",MB_OK|MB_ICONINFORMATION);
#define wrong_input MessageBox(NULL,"输入有误，请重新输入","Error",MB_ICONERROR|MB_OK);
using namespace std;
int width,height,ans,linkcnt,c,magicpoint,addmagicpoint,mapcnt;
int mappc[MAX_SIZE][MAX_SIZE];//colour
bool mappv[MAX_SIZE][MAX_SIZE];//visited
bool linkblock[MAX_SIZE][MAX_SIZE];
TCHAR textbuf[201];
HANDLE hout,inh;
CONSOLE_SCREEN_BUFFER_INFO buf_pos;
stringstream qstream("");
ofstream fout,fapp;
ifstream fin;
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
class qmap
{
	public:
		int qwidth,qheight;
		bool mapp[MAX_SIZE][MAX_SIZE];
		qmap(int a=MAX_SIZE,int b=MAX_SIZE)
		{
			qheight=a;
			qwidth=b;
			for(int i=1;i<=qheight;i++)
			{
				for(int j=1;j<=qwidth;j++)
				{
					mapp[i][j]=1;
				}
			}
		}
		void setmap(ifstream &ifin)
		{
			for(int i=1;i<=qheight;i++)
			{
				for(int j=1;j<=qwidth;j++)
				{
					ifin>>mapp[i][j];
				}
			}			
		}
		void savemap(ofstream &ofout)
		{
			ofout<<qheight<<" "<<qwidth<<endl;
			for(int i=1;i<=qheight;i++)
			{
				for(int j=1;j<=qwidth;j++)
				{
					ofout<<mapp[i][j]<<" ";
				}
				ofout<<endl;
			}
			ofout<<endl;		
		}		
		void showmap()
		{
			cout<<"height:"<<qheight<<"\twidth:"<<qwidth<<endl;
			for(int i=1;i<=qheight;i++)
			{
				for(int j=1;j<=qwidth;j++)
				{
					if(mapp[i][j]) cout<<"*";
					else cout<<" ";
				}
				cout<<endl;
			}
		}
		~qmap() {;}
};
list<qmap*> maps;
list<qmap*>::iterator iter;
inline int rand(int l,int r)
{
	return rand()%(r-l+1)+l;
}
DWORD WINAPI input(LPVOID pM)
{
	cin>>inputnum;
}
bool map_empty()
{
	for(int i=1;i<=height;i++)
	{
		for(int j=1;j<=width;j++)
		{
			if(mapp[i][j]!=NULL) return 0;
		}
	}
	return 1;
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
			else
			{
				show_mapp[i][j]=' ';
				mappc[i][j]=0;
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
	cout<<"magicpoint:"<<magicpoint<<endl;
	cout<<"输入交换点的坐标：行 列 (如：1 1 1 2)\n(输入0结束游戏，输入-1使用magic)"<<endl;
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
	magicpoint+=addmagicpoint;
}
int search(int lx=1,int ly=1,int rx=height,int ry=width)
{
	int add=0;
	addmagicpoint=0;
	initm(mappv,lx,ly,rx,ry);
	initm(linkblock,lx,ly,rx,ry);
	for(int i=lx;i<=rx;i++)
	{
		for(int j=ly;j<=ry;j++)
		{
			if(mappv[i][j]==0&&mapp[i][j]!=NULL) bfs(i,j);
		}
	}
	for(int i=rx;i>=lx;i--)
	{
		for(int j=ly;j<=ry;j++)
		{
			if(linkblock[i][j])
			{
				add+=mapp[i][j]->score();
				if(mapp[i][j]->score()==0) addmagicpoint++;
			}
		}
	}
	return add;
}
void clear_blocks(int lx=1,int ly=1,int rx=height,int ry=width,bool ifill=0)
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
					if(ifill==1) mapp[i][j]=new block(pow(2,rand(0,2)));
					else mapp[i][j]=NULL;
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
	cout<<"2.编辑地图\n";
	cout<<"\n\n\n";
	cout<<"3.帮助\n";
	cout<<"\n\n\n";
	cout<<"0.退出\n";
	cout<<"\n\n\n";	
	cout<<setfill('-')<<setw(55)<<""<<setfill(' ')<<endl;
}
void newgame1()
{
	width=5;
	height=5;
	linkcnt=5;
	ans=0;
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
void newgame2()
{
	width=8;
	height=8;
	linkcnt=5;
	ans=0;
	magicpoint=0;
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
			if(rand(1,8)==1)
			{
				delete mapp[i][j];
				mapp[i][j]=new sblock(pow(2,rand(0,2)),0);
			} 			
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
	fapp.open("map.txt",ios::app);
	fin.open("map.txt",ios::in);
	if(!fin.eof()) fin>>mapcnt;
	if(fin.eof()) fapp<<0<<endl;
	int th,tw;
	for(int i=1;i<=mapcnt;i++)
	{
		fin>>th>>tw;
		maps.push_front(new qmap(th,tw));
		maps.front()->setmap(fin);
	}
	//fout.open("map.txt",ios::out);	
}
void help()
{
	system("cls");
	cout<<setfill('-')<<setw(35)<<"COLOURFUL BLOCKS"<<setw(20)<<""<<setfill(' ')<<endl;
	cout<<setw(55)<<"AUTHOR:tlzx\n";
	cout<<"\n";
	cout<<"按任意键返回\n";
	while(!kbhit()) ;
}
void make_map()
{
	int t1,t2;
	D:
	system("cls");
	cout<<"地图数量："<<mapcnt<<endl;
	cout<<"1.查看地图\n";
	cout<<"2.新建地图\n";
	cout<<"3.删除地图\n";
	cout<<"0.返回并保存更改\n";
	cin>>c;
	switch(c)
	{
		case 1:
		{
			cout<<"输入查看地图的编号\n";
			cin>>c;
			if(c<=0||c>mapcnt)
			{
				wrong_input
				goto D;
			}
			iter=maps.begin();
			for(int i=1;i<c;i++)
			{
				iter++;
			}
			(*iter)->showmap();
			cout<<"按任意键返回\n";
			while(!kbhit()) ;
			goto D;		
		}
		case 2:
		{
			cout<<"输入地图的height与width\n";
			cin>>t1>>t2;
			maps.push_front(new qmap(t1,t2));
			mapcnt++;
			iter=maps.begin();
			E:
			system("cls");
			(*iter)->showmap();
			cout<<"输入坐标（行 列）以反转\n输入0返回\n";
			cin>>t1;
			if(t1==0) goto D;
			cin>>t2;
			if(t1<1||t1>(*iter)->qheight||t2<1||t2>(*iter)->qwidth)
			{
				wrong_input
				goto E;
			}
			(*iter)->mapp[t1][t2]=!(*iter)->mapp[t1][t2];
			goto E;
		}
		case 3:
		{
			cout<<"输入删除地图的编号\n";
			cin>>c;
			if(c<=0||c>mapcnt)
			{
				wrong_input
				goto D;
			}
			iter=maps.begin();
			for(int i=1;i<=c;i++)
			{
				iter++;
			}
			delete (*iter);
			maps.erase(iter);
			mapcnt--;
			goto D;
		}
		case 0:
		{
			break;
		}
		default:
		{
			wrong_input
			cin.clear();
			cin.sync();
			goto D;
		}
	}
	cin.clear();
	cin.sync();
	fout.open("map.txt",ios::out);
	fout<<mapcnt<<endl;
	for(iter=maps.begin();iter!=maps.end();iter++)
	{
		(*iter)->savemap(fout);
	}
}
void play()
{
	int x1,x2,y1,y2,t;
	COORD pos;
	A:
	inh=CreateThread(NULL,0,input,NULL,CREATE_SUSPENDED,NULL);
	show_menu();
	GetConsoleScreenBufferInfo(hout,&buf_pos);
	for(int i=1;i<=15;i++)
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
			pos.Y=rand(1,15)+2;
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
			cout<<"2.消除模式\n";
			cout<<"3.自定义模式\n";
			cout<<"4.特殊地图\n";
			cout<<"0.返回\n";
			cin>>c;
			switch(c)
			{
				case 1:
				{
					newgame1();
					while(search()) newgame1();
					while(1)
					{
						show();
						cin>>x1;
						if(x1==0)
						{
							game_over();
							goto A;
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
						show();
						t=search();
						if(t==0)
						{
							MessageBox(NULL,"交换无效，请重新输入","Error",MB_OK|MB_ICONERROR);
							swap(mapp[x1][y1],mapp[x2][y2]);
							continue;
						}
						clear_blocks(1,1,height,width,1);
						award(t);
					}
					break;				
				}				
				case 2:
				{
					newgame2();
					while(search()) newgame2();
					while(1)
					{
						show();
						if(map_empty())
						{
							game_over();
							MessageBox(NULL,"你已经将此地图清理干净了！","Congratulation",MB_OK);
							goto A;
						}
						cin>>x1;
						if(x1==0)
						{
							game_over();
							goto A;
						}
						if(x1==-1)
						{
							if(magicpoint==0)
							{
								MessageBox(NULL,"你的magicpoint为0，无法使用magic","Error",MB_OK);
								continue;
							}
							C:
							//show();
							cout<<"发动magic：\n";
							cout<<"1.移动任意两个点\n";
							cout<<"2.清理一个点\n";
							cout<<"0.取消发动magic\n";
							cin>>c;
							if(c==0) continue;
							if(c==1)
							{
								cout<<"输入交换点的坐标：行 列 (如：1 1 1 2)\n输入0取消\n";
								cin>>x1;
								if(x1==0) continue;
								cin>>y1>>x2>>y2;
								if(x1<1||x1>height||x2<1||x2>height||y1<1||y1>width||y2<1||y2>width)
								{
									wrong_input
									cin.clear();
									cin.sync();
									continue;
								}
								swap(mapp[x1][y1],mapp[x2][y2]);
								magicpoint--;			 
							}
							if(c==2)
							{
								cout<<"输入清理点的坐标：行 列 (如：1 2)\n输入0取消\n";
								cin>>x1;
								if(x1==0) continue;
								cin>>y1;
								if(x1<1||x1>height||y1<1||y1>width||mapp[x1][y1]==NULL)
								{
									wrong_input
									cin.clear();
									cin.sync();								
									continue;									
								}
								delete mapp[x1][y1];
								t=x1-1;
								while(t>1&&mapp[t][y1]!=NULL)
								{
									mapp[t+1][y1]=mapp[t][y1];
									t--;
								}
								magicpoint--;
							}
							else
							{
								wrong_input
								cin.clear();
								cin.sync();								
								continue;
							}
							cin.clear();
							cin.sync();
							continue;
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
						show();
						t=search();
						if(t==0)
						{
							MessageBox(NULL,"交换无效，请重新输入","Error",MB_OK|MB_ICONERROR);
							swap(mapp[x1][y1],mapp[x2][y2]);
							continue;
						}
						clear_blocks();
						award(t);
						show();
						t=search();
						while(t)
						{
							clear_blocks();
							award(t);
							show();
							t=search();
						}
					}					
					break;
				}
				case 3:
				{
					qwq
					goto B;
				}
				case 4:
				{
					cout<<endl;
					cout<<"地图数量："<<mapcnt<<endl;
					cout<<"输入地图编号：\n";
					cin>>c;
					if(c<=0||c>mapcnt)
					{
						wrong_input
						goto B;
					}
					iter=maps.begin();
					for(int i=1;i<c;i++)
					{
						iter++;
					}
					height=((*iter)->qheight);
					width=((*iter)->qwidth);
					linkcnt=5;
					ans=0;
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
							if((*iter)->mapp[i][j]) mapp[i][j]=new block(pow(2,rand(0,2)));
						}
					}
					while(1)
					{
						show();
						if(map_empty())
						{
							game_over();
							MessageBox(NULL,"你已经将此地图清理干净了！","Congratulation",MB_OK);
							goto A;
						}
						cin>>x1;
						if(x1==0)
						{
							game_over();
							goto A;
						}
						if(x1==-1)
						{
							if(magicpoint==0)
							{
								MessageBox(NULL,"你的magicpoint为0，无法使用magic","Error",MB_OK);
								continue;
							}
							C2:
							//show();
							cout<<"发动magic：\n";
							cout<<"1.移动任意两个点\n";
							cout<<"2.清理一个点\n";
							cout<<"0.取消发动magic\n";
							cin>>c;
							if(c==0) continue;
							if(c==1)
							{
								cout<<"输入交换点的坐标：行 列 (如：1 1 1 2)\n输入0取消\n";
								cin>>x1;
								if(x1==0) continue;
								cin>>y1>>x2>>y2;
								if(x1<1||x1>height||x2<1||x2>height||y1<1||y1>width||y2<1||y2>width)
								{
									wrong_input
									cin.clear();
									cin.sync();
									continue;
								}
								swap(mapp[x1][y1],mapp[x2][y2]);
								magicpoint--;			 
							}
							if(c==2)
							{
								cout<<"输入清理点的坐标：行 列 (如：1 2)\n输入0取消\n";
								cin>>x1;
								if(x1==0) continue;
								cin>>y1;
								if(x1<1||x1>height||y1<1||y1>width||mapp[x1][y1]==NULL)
								{
									wrong_input
									cin.clear();
									cin.sync();								
									continue;									
								}
								delete mapp[x1][y1];
								t=x1-1;
								while(t>1&&mapp[t][y1]!=NULL)
								{
									mapp[t+1][y1]=mapp[t][y1];
									t--;
								}
								magicpoint--;
							}
							else
							{
								wrong_input
								cin.clear();
								cin.sync();								
								continue;
							}
							cin.clear();
							cin.sync();
							continue;
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
						show();
						t=search();
						if(t==0)
						{
							MessageBox(NULL,"交换无效，请重新输入","Error",MB_OK|MB_ICONERROR);
							swap(mapp[x1][y1],mapp[x2][y2]);
							continue;
						}
						clear_blocks();
						award(t);
						show();
						t=search();
						while(t)
						{
							clear_blocks();
							award(t);
							show();
							t=search();
						}
					}					
					break;									
					goto B;
				}
				case 0:
				{
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
			goto A;
		}
		case 2:
		{
			make_map();
			goto A;
		}
		case 3:
		{
//			qwq
			help();
			goto A;
		}
		case 0:
		{
			return;
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
int main()
{
	init();
	play(); 
	return 0;
}

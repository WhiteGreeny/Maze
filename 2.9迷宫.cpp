#include <iostream>
#include <cstdlib>
using namespace std;

#define MAXSIZE 110

//坐标
struct postype {
	int r; //行 row
	int c; //列 col
};//迷宫中r行c列的位置
struct position {
	int step; //当前位置在路径上的序号
	postype pos; //当前的坐标位置

};
bool operator==(const postype& a, const postype& b) {
	if (a.r == b.r && a.c == b.c)
		return true;
	return false;
}
void printPos(postype e) {//输出坐标
	cout << "(" << e.r << "," << e.c << ")" << endl;
}
//把坐标、步数合成一个栈节点
position MakeNode(postype p, int i) {
	position temp;
	temp.pos = p;
	temp.step = i;
	return temp;
}
//合成坐标
postype MakePos(int x, int y) {
	postype temp;
	temp.r = x;
	temp.c = y;
	return temp;
}
postype MakePosDirect(postype p, int d) {
	postype temp;
	temp = p;
	switch (d) {
	case 1:temp.r = p.r - 1; break;
	case 2:temp.c = p.c + 1; break;
	case 3:temp.r = p.r + 1; break;
	case 4:temp.c = p.c - 1; break;
	}
	return temp;
}
//栈
class myStack {
private:
	int counter;
	position* base;
	position* top;
public:
	int stacksize;
	myStack();   //构造函数,构造空栈
	~myStack(); //析构函数，摧毁栈
	void ClearStack(); //设为空栈
	bool StackEmpty();//栈是否为空
	int StackLength(); //返回栈的长度
	bool GetTop(position& e);//若栈不空，则用e返回栈顶
	void Push(position e);//插入e为新的栈顶元素
	bool Pop(position& e);//若栈不空，删除栈顶元素，并用e返回
	void StackTraverse() {}

};
myStack::myStack() { //空栈
	counter = 0;
	base = new position[MAXSIZE];
	top = base;
	//stacksize=counter;
}
myStack::~myStack() { //销毁栈
	delete[] base;
}
void myStack::ClearStack() {
	top = base;
}
bool myStack::StackEmpty() {
	if (top == base)
		return true;
	return false;
}
int myStack::StackLength() {
	return counter;
}
bool myStack::GetTop(position& e) {
	if (top == base)
		return false;
	e = *(top - 1);
	return true;
}
void myStack::Push(position e) {
	*(top++) = e;
	counter++;
}
bool myStack::Pop(position& e) {
	if (top == base)
		return false;
	e = *--top;
	counter--;
	return true;
}

//迷宫
struct MazePoint {//迷宫每个方块的属性
	int cross; //是否可通，0通1不通
	int foot; //是否有足迹，0无1有
	int mark; //默认为0，1表示为路径，2表示死路，在退格时标记
	postype mpos;//迷宫该点坐标
	char print; //打印时的字符
};
class Maze {
private:

	int row; //行数
	int col;//列数
	postype start;//入口
	postype mend;//出口
public:
	MazePoint point[11][11]; //最大有100个方块
	Maze();//构造函数
	Maze(int x, int y, postype op, postype ed);//行数、列数、入口、出口
	~Maze();//析构函数
	int getrow();//获得行数
	int getcol();//获得列数
	void InitMaze();//以用户输入构建迷宫
	bool MazePath();//求解迷宫，可解为true，不可解为false
	void PrintMaze();//打印迷宫
	void SetStart(int x, int y);//重设入口
	void SetEnd(int x, int y);//重设出口
	void CheckMaze();//检查当前迷宫每点属性，规范化
	bool NextStep(position nowstep, int dir, position& e); //产生下一步
};

Maze::Maze() {
	row = 10;
	col = 10;
	start = MakePos(0, 0);
	mend = MakePos(0, 0);
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++) {
			point[i][j].cross = 1;
			point[i][j].foot = 0;
			point[i][j].mark = 0;
			point[i][j].mpos = MakePos(i, j);
			point[i][j].print = '#';
		}
	}
}
Maze::Maze(int x, int y, postype op, postype ed) {
	row = x;
	col = y;
	start = op;
	mend = ed;
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++) {
			point[i][j].cross = 1;
			point[i][j].foot = 0;
			point[i][j].mark = 0;
			point[i][j].mpos = MakePos(i, j);
			point[i][j].print = '#';
		}
	}
}
Maze::~Maze() {
	cout << "Maze destroyed" << endl;
}
int Maze::getcol() {
	return col;
}
int Maze::getrow() {
	return row;
}
void Maze::SetEnd(int x, int y) {
	start = MakePos(x, y);
}
void Maze::SetStart(int x, int y) {
	mend = MakePos(x, y);
}
void Maze::InitMaze() { //由用户输入。
	cout << "请输入迷宫:" << endl;
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++) {
			cin >> point[i][j].cross;
		}
	}
	cout << endl;
}
void Maze::CheckMaze() {
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++) {
			if (point[i][j].cross == 0) {
				if (point[i][j].mark == 1) {
					point[i][j].print = '*';
				}
				else if (point[i][j].mark == 2) {
					point[i][j].print = '@';
				}
				else {
					point[i][j].print = ' ';
				}
			}
		}
	}
	point[start.r][start.c].print = 'E';
	point[mend.r][mend.c].print = 'O';
}
void Maze::PrintMaze() {
	for (int i = 1; i <= col * 2.7; i++) {
		cout << '-';
	}
	cout << endl;
	for (int i = 1; i <= row; i++) {
		cout << '|';
		for (int j = 1; j <= col; j++) {
			if (j != 1)
				cout << " ";
			cout << point[i][j].print;
			cout << '|';
		}
		cout << endl;
		for (int i = 1; i <= col * 2.7; i++) {
			cout << '-';
		}
		cout << endl;
	}
	cout << endl;
	cout << endl;

}
bool Maze::NextStep(position np, int d, position& e) { //当前步，方向，下一步
	e = np;
	postype temp = MakePosDirect(np.pos, d);
	//如果出界
	if (temp.r > row || temp.r<1 || temp.c>col || temp.c < 1) {
		return false;
	}
	//如果不可通
	else if (point[temp.r][temp.c].cross != 0) {
		return false;
	}
	//如果已走过
	else if (point[temp.r][temp.c].foot != 0)
		return false;
	else {
		e.pos = temp;
		e.step++;
		return true;
	}
}
bool Maze::MazePath() {
	myStack path;
	position nowstep; //当前步
	nowstep.step = 0;
	int direct = 1; //方向，1上，2右，3下，4左
	nowstep.pos = start;
	nowstep.step = 1;
	path.Push(nowstep);
	int ssss = 1;
	while (!path.StackEmpty()) {

		direct = 1;
		if (path.GetTop(nowstep)) {
			direct = 1;
		}
		if (nowstep.pos == mend) {
			point[nowstep.pos.r][nowstep.pos.c].foot = 1;
			point[nowstep.pos.r][nowstep.pos.c].mark = 1;
			return true;
		}
		else {
			point[nowstep.pos.r][nowstep.pos.c].foot = 1;
			point[nowstep.pos.r][nowstep.pos.c].mark = 1;
			while (direct <= 4) {
				if (NextStep(nowstep, direct, nowstep)) {
					path.Push(nowstep);
					break;
				}
				else {
					direct++;
				}
			}
			if (direct >= 5) {
				point[nowstep.pos.r][nowstep.pos.c].mark = 2;
				if (path.Pop(nowstep)) {
					direct = 6;
				}

			}
		}
	}
	return false;
}

int main() {
	//Maze one(9,8,MakePos(1,1),MakePos(9,8));

	int row, col, start_x, start_y, end_x, end_y;
	cout << "请依次输入迷宫的行数、列数" << endl;
	cin >> row >> col;
	cout << endl;
	cout << "请依次输入迷宫的入口横坐标，纵坐标" << endl;
	cin >> start_x >> start_y;
	cout << endl;
	cout << "请依次输入迷宫的出口横坐标，纵坐标" << endl;
	cin >> end_x >> end_y;
	cout << endl;
	Maze one(row, col, MakePos(start_x, start_y), MakePos(end_x, end_y));
	one.InitMaze();
	cout << "输入迷宫为:" << endl;
	one.CheckMaze();
	one.PrintMaze();
	cout << "开始寻找路径:" << endl;
	system("pause");
	cout << endl;
	if (one.MazePath()) {
		cout << "找到可行路径" << endl;
		cout << "迷宫绘制如下:" << endl;
		one.CheckMaze();
		one.PrintMaze();
	}
	else {
		cout << "该迷宫无解" << endl;
	}
	system("pause");
	return 0;

}

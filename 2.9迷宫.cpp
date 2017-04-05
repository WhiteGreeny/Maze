#include <iostream>
#include <cstdlib>
using namespace std;

#define MAXSIZE 110

//����
struct postype{
    int r; //�� row
    int c; //�� col
};//�Թ���r��c�е�λ��
struct position{
    int step; //��ǰλ����·���ϵ����
    postype pos; //��ǰ������λ��

};
bool operator==(const postype &a,const postype &b){
    if(a.r==b.r&&a.c==b.c)
        return true;
    return false;
}
void printPos(postype e){//�������
    cout<<"("<<e.r<<","<<e.c<<")"<<endl;
}
//�����ꡢ�����ϳ�һ��ջ�ڵ�
position MakeNode(postype p,int i){
    position temp;
    temp.pos=p;
    temp.step=i;
    return temp;
}
//�ϳ�����
postype MakePos(int x,int y){
    postype temp;
    temp.r=x;
    temp.c=y;
    return temp;
}
postype MakePosDirect(postype p,int d){
    postype temp;
    temp=p;
    switch(d){
        case 1:temp.r=p.r-1;break;
        case 2:temp.c=p.c+1;break;
        case 3:temp.r=p.r+1;break;
        case 4:temp.c=p.c-1;break;
    }
    return temp;
}
//ջ
class myStack{
private:
    int counter;
    position *base;
    position *top;
public:
    int stacksize;
    myStack();   //���캯��,�����ջ
    ~myStack(); //�����������ݻ�ջ
    void ClearStack(); //��Ϊ��ջ
    bool StackEmpty();//ջ�Ƿ�Ϊ��
    int StackLength(); //����ջ�ĳ���
    bool GetTop(position &e);//��ջ���գ�����e����ջ��
    void Push(position e);//����eΪ�µ�ջ��Ԫ��
    bool Pop(position &e);//��ջ���գ�ɾ��ջ��Ԫ�أ�����e����
    void StackTraverse(){}

};
myStack::myStack(){ //��ջ
    counter=0;
    base=new position[MAXSIZE];
    top=base;
    //stacksize=counter;
}
myStack::~myStack(){ //����ջ
    delete [] base;
}
void myStack::ClearStack(){
    top=base;
}
bool myStack::StackEmpty(){
    if(top==base)
        return true;
    return false;
}
int myStack::StackLength(){
    return counter;
}
bool myStack::GetTop(position &e){
    if(top==base)
        return false;
    e=*(top-1);
    return true;
}
void myStack::Push(position e){
    *(top++)=e;
    counter++;
}
bool myStack::Pop(position &e){
    if(top==base)
        return false;
    e=*--top;
    counter--;
    return true;
}

//�Թ�
struct MazePoint{//�Թ�ÿ�����������
    int cross; //�Ƿ��ͨ��0ͨ1��ͨ
    int foot; //�Ƿ����㼣��0��1��
    int mark; //Ĭ��Ϊ0��1��ʾΪ·����2��ʾ��·�����˸�ʱ���
    postype mpos;//�Թ��õ�����
    char print; //��ӡʱ���ַ�
};
class Maze{
private:

    int row; //����
    int col;//����
    postype start;//���
    postype mend;//����
public:
    MazePoint point[11][11]; //�����100������
    Maze();//���캯��
    Maze(int x,int y,postype op,postype ed);//��������������ڡ�����
    ~Maze();//��������
    int getrow();//�������
    int getcol();//�������
    void InitMaze();//���û����빹���Թ�
    bool MazePath();//����Թ����ɽ�Ϊtrue�����ɽ�Ϊfalse
    void PrintMaze();//��ӡ�Թ�
    void SetStart(int x,int y);//�������
    void SetEnd(int x,int y);//�������
    void CheckMaze();//��鵱ǰ�Թ�ÿ�����ԣ��淶��
    bool NextStep(position nowstep,int dir,position &e); //������һ��
};

Maze::Maze(){
    row=10;
    col=10;
    start=MakePos(0,0);
    mend=MakePos(0,0);
    for(int i=1;i<=row;i++){
        for(int j=1;j<=col;j++){
            point[i][j].cross=1;
            point[i][j].foot=0;
            point[i][j].mark=0;
            point[i][j].mpos=MakePos(i,j);
            point[i][j].print='#';
        }
    }
}
Maze::Maze(int x,int y,postype op,postype ed){
    row=x;
    col=y;
    start=op;
    mend=ed;
    for(int i=1;i<=row;i++){
        for(int j=1;j<=col;j++){
            point[i][j].cross=1;
            point[i][j].foot=0;
            point[i][j].mark=0;
            point[i][j].mpos=MakePos(i,j);
            point[i][j].print='#';
        }
    }
}
Maze::~Maze(){
    cout<<"Maze destroyed"<<endl;
}
int Maze::getcol(){
    return col;
}
int Maze::getrow(){
    return row;
}
void Maze::SetEnd(int x,int y){
    start=MakePos(x,y);
}
void Maze::SetStart(int x,int y){
    mend=MakePos(x,y);
}
void Maze::InitMaze(){ //���û����롣
    cout<<"�������Թ�:"<<endl;
    for(int i=1;i<=row;i++){
        for(int j=1;j<=col;j++){
            cin>>point[i][j].cross;
        }
    }
    cout<<endl;
}
void Maze::CheckMaze(){
    for(int i=1;i<=row;i++){
        for(int j=1;j<=col;j++){
            if(point[i][j].cross==0){
                if(point[i][j].mark==1){
                    point[i][j].print='*';
                }else if(point[i][j].mark==2){
                    point[i][j].print='@';
                }else{
                    point[i][j].print=' ';
                }
            }
        }
    }

}
void Maze::PrintMaze(){
    for(int i=1;i<=col*2.7;i++){
        cout<<'-';
    }
    cout<<endl;
    for(int i=1;i<=row;i++){
        cout<<'|';
        for(int j=1;j<=col;j++){
            if(j!=1)
                cout<<" ";
            cout<<point[i][j].print;
            cout<<'|';
        }
        cout<<endl;
        for(int i=1;i<=col*2.7;i++){
            cout<<'-';
        }
        cout<<endl;
    }
    cout<<endl;
    cout<<endl;

}
bool Maze::NextStep(position np,int d,position &e){ //��ǰ����������һ��
    e=np;
    postype temp=MakePosDirect(np.pos,d);
    //�������
    if(temp.r>row||temp.r<1||temp.c>col||temp.c<1){
        return false;
    }
    //�������ͨ
    else if(point[temp.r][temp.c].cross!=0){
        return false;
    }
    //������߹�
    else if(point[temp.r][temp.c].foot!=0)
        return false;
    else{
        e.pos=temp;
        e.step++;
        return true;
    }
}
bool Maze::MazePath(){
    myStack path;
    position nowstep; //��ǰ��
    nowstep.step=0;
    int direct=1; //����1�ϣ�2�ң�3�£�4��
    nowstep.pos=start;
    nowstep.step=1;
    path.Push(nowstep);
    int ssss=1;
    while(!path.StackEmpty()){

        direct=1;
        if(path.GetTop(nowstep)){
            direct=1;
        }
        if(nowstep.pos==mend){
            point[nowstep.pos.r][nowstep.pos.c].foot=1;
            point[nowstep.pos.r][nowstep.pos.c].mark=1;
            return true;
        }else{
            point[nowstep.pos.r][nowstep.pos.c].foot=1;
            point[nowstep.pos.r][nowstep.pos.c].mark=1;
            while(direct<=4){
                if(NextStep(nowstep,direct,nowstep)){
                    path.Push(nowstep);
                    break;
                }else{
                    direct++;
                }
            }
            if(direct>=5){
                point[nowstep.pos.r][nowstep.pos.c].mark=2;
                if(path.Pop(nowstep)){
                    direct=6;
                }

            }
        }
    }
    return false;
}

int main(){
    //Maze one(9,8,MakePos(1,1),MakePos(9,8));

    int row,col,start_x,start_y,end_x,end_y;
    cout<<"�����������Թ�������������"<<endl;
    cin>>row>>col;
    cout<<endl;
    cout<<"�����������Թ�����ں����꣬������"<<endl;
    cin>>start_x>>start_y;
    cout<<endl;
    cout<<"�����������Թ��ĳ��ں����꣬������"<<endl;
    cin>>end_x>>end_y;
    cout<<endl;
    Maze one(row,col,MakePos(start_x,start_y),MakePos(end_x,end_y));
    one.InitMaze();
    cout<<"�����Թ�Ϊ:"<<endl;
    one.CheckMaze();
    one.PrintMaze();
    cout<<"��ʼѰ��·��:"<<endl;
    system("pause");
    cout<<endl;
    if(one.MazePath()){
        cout<<"�ҵ�����·��"<<endl;
        cout<<"�Թ���������:"<<endl;
        one.CheckMaze();
        one.PrintMaze();
    }else{
        cout<<"���Թ��޽�"<<endl;
    }
    system("pause");
    return 0;

}

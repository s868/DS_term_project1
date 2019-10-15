#include <iostream>
#include <fstream>

using namespace std;

class shape{
    public:
        int dis_r[5]; //相對於要放的row
        int dis_c[5]; //相對於ref_pt
        int amt_rec[4]={0}; //一列有幾個方塊  [0]<->要放的row(row_to_place)
        int to_find_row[4]; //Max(col_top[ref_pt]+to_find_row[0],col_top[ref_pt+1]+to_find_row[2],...)
};

class ROW{
    public:
        int col[16]={0};
        int detect=0;  //if==n就消滅
        //int real_row;
        void clear_row(){
            for(int i=0;i<16;i++)
                col[i]=0;
            detect=0;
        }
};

shape initialize(char a,char b);

int main()
{
    int m,n;
    int _m[2]={0},_n[2]={0};
    char x;
    int i=0,j;
    char type[2];
    int ref_pt,row_to_place,col_top[25]={0};
    int _ref_pt[2];
    int *trans; //trans[real_row]=row index
    ROW *row;
    shape cur_type;
    int gameover=0;

    ifstream inFile("Tetris.data",ios::in);
    if(!inFile){
        cout<<"cannot open Tetris.data"<<endl;
        return 1;
    }

    ///讀m n
    inFile>>noskipws; //讀取空格
    while(1){
        inFile>>x;
        if(x==' ') break;
        _m[i]=x-'0';
        i++;
    }
    if(i==2) m=_m[0]*10+_m[1];
    else m=_m[0];

    i=0;
    while(1){
        inFile>>x;
        if(!isdigit(x)){
            break;
        }
        _n[i]=x-'0';
        i++;
    }
    if(i==2) n=_n[0]*10+_n[1];
    else n=_n[0];
    cout<<"m="<<m<<",n="<<n<<endl;

    trans=new int[m];
    row=new ROW[m];
    for(i=0;i<m;i++) trans[i]=i;

    //inFile>>skipws; //讀取第一個非空格字符
    while(!inFile.eof()){
        step=0;

        ///讀type和reference pt
        i=0;
        do{
            inFile>>x;
            if(x=='E'){  //END
                gameover=1;
                break;
            }
            type[i]=x;
            i++;
        }while(isalpha(x)||isdigit(x));
        if(gameover) break;

        i=0;
        do{
            inFile>>x;
            _ref_pt[i]=x-'0';
            if(isdigit(x)) i++;

        }while(isdigit(x));
        if(i==2) ref_pt=_ref_pt[0]*10+_ref_pt[1];
        else ref_pt=_ref_pt[0];

        cout<<type[0]<<type[1]<<" "<<ref_pt<<endl;

        cur_type=initialize(type[0],type[1]);


        ///get row_to_place
        int k=max(col_top[ref_pt]+cur_type.to_find_row[0],col_top[ref_pt+1]+cur_type.to_find_row[1]);
        for(i=2;i<4;i++){
            k=max(k,col_top[ref_pt+i]+cur_type.to_find_row[i]);
        }
        row_to_place=k;
        cout<<"row_to_place "<<row_to_place<<endl;

        ///fill blank and modify col_top
        for(i=1;i<=4;i++){
            row[trans[row_to_place+cur_type.dis_r[i]]].col[ref_pt+cur_type.dis_c[i]]=1;
            col_top[ref_pt+cur_type.dis_c[i]]=row_to_place+cur_type.dis_r[i];
            cout<<"row: row_to_place+cur_type.dis_r[i]] "<<row_to_place+cur_type.dis_r[i]<<endl;
            cout<<"col: ref_pt+cur_type.dis_c[i] "<<ref_pt+cur_type.dis_c[i]<<endl;
        }

        ///modify detect and see 消除與否
        for(i=0;i<4;i++){
            cout<<row[trans[row_to_place+i]].detect<<endl;
            row[trans[row_to_place+i]].detect = row[trans[row_to_place+i]].detect + cur_type.amt_rec[i];
            cout<<"row[trans["<<row_to_place+i<<"]].detect "<<row[trans[row_to_place+i]].detect<<endl;
            if(row[trans[row_to_place+i]].detect==n){  //row_to_place+i這行要被消滅

                clear_row=row_to_place+i;
                cout<<"clear_row "<<clear_row<<endl;
                //clear row
                row[trans[clear_row]].clear_row();

                //use _move to change trans and col_top later
                _move[i]=clear_row-step;
                step++;
            }
            else
                _move[i]=0;
        }

        ///超出範圍?
        

    }

    ///write into Tetris.output
    ofstream outFile("Tetris.output",ios::out);
    if(!outFile){
        cout<<"cannot open Tetris.output"<<endl;
        return 1;
    }

    for(i=m;i>=1;i--){
        for(j=1;j<=n;j++){
            outFile<<row[trans[i]].col[j];
        }
        outFile<<endl;
    }

    return 0;
}
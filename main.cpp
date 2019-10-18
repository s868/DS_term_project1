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
    int _move[4]={0},step=0; //_move: 由下往上，哪一列會被消滅 ; step: 放下這個方塊有幾列會被消滅
    int *trans; //trans[real_row]=row index
    ROW *row;
    shape cur_type;
    int clear_row,tmp;
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

    trans=new int[m+8];
    row=new ROW[m+8];
    for(i=0;i<m+8;i++) trans[i]=i;

    //inFile>>skipws; //讀取第一個非空格字符
    while(!inFile.eof()){
        step=0;

        ///讀type和reference pt
        i=0;

        //濾掉空格或換行
        do{
            inFile>>x;
        }
        while(!isalpha(x)&&!isdigit(x));

        do{
            if(x=='E'){  //END
                gameover=1;
                break;
            }
            type[i]=x;
            inFile>>x;
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
        int eliminate=0;  //看下面(合法區域內)有無消除
        for(i=0;i<4;i++){
            cout<<row[trans[row_to_place+i]].detect<<endl;
            row[trans[row_to_place+i]].detect = row[trans[row_to_place+i]].detect + cur_type.amt_rec[i];
            cout<<"row[trans["<<row_to_place+i<<"]].detect "<<row[trans[row_to_place+i]].detect<<endl;
            if(row[trans[row_to_place+i]].detect==n){  //row_to_place+i這行要被消滅

                clear_row=row_to_place+i;
                
                //clear row
                if(clear_row<=m){
                    row[trans[clear_row]].clear_row();
                    _move[i]=clear_row-step;
                    eliminate=1;
                    
                    cout<<"clear_row "<<clear_row<<endl;
                }
                else{
                    if(i>0 && eliminate!=0){  //如果下面有消除 m行以上的就可掉下來並消除
                        row[trans[clear_row]].clear_row();
                        _move[i]=clear_row-step;

                        cout<<"clear_row "<<clear_row<<endl;
                    }
                    else
                        _move[i]=0;
                }
                step++; 
            }
            else
                _move[i]=0;
        }

        ///if有消滅
        if(_move[0]||_move[1]||_move[2]||_move[3]){
            ///modify trans
            for(i=0;i<4;i++){
                if(_move[i]!=0){   //remove restrict of _move[i]==m
                        tmp=trans[_move[i]];  //要被消除的row的index
                        cout<<"_move["<<i<<"] "<<_move[i]<<endl;
                        for(j=_move[i];j<m+8-1;j++)
                            trans[j]=trans[j+1];

                        trans[m+8-1]=tmp;

                    }
                    
                cout<<"---------NOW---------"<<endl;
                for(int l=m+8-1;l>=1;l--){
                    for(int h=1;h<=n;h++){
                        cout<<row[trans[l]].col[h];
                    }
                    cout<<endl;
                }
                cout<<"----------------------"<<endl;

            }


            ///change col_top
            for(j=1;j<=n;j++){
                if(col_top[j]!=0){
                    cout<<"ori "<<col_top[j]<<endl;
                    col_top[j]--;
                    cout<<"row[trans[col_top[j]]].col[j]"<<row[trans[col_top[j]]].col[j]<<endl;
                    while(row[trans[col_top[j]]].col[j]==0){
                        if(col_top[j]==0) break;
                        else  col_top[j]--;
                    }
                }
                else{
                    cout<<"error: col_top should>0"<<endl;
                }
                cout<<"col_top["<<j<<"] "<<col_top[j]<<endl;
            }
        }


        ///超出範圍?
        for(i=1;i<=n;i++){
            if(col_top[i]>m){
                gameover=1;
                break;
            }
        }
        if(gameover) break;

    }

    ///write into Tetris.final
    ofstream outFile("Tetris.final",ios::out);
    if(!outFile){
        cout<<"cannot open Tetris.final"<<endl;
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


shape initialize(char a,char b)
{
    shape S;
    if( a=='T' && b=='1' ){
        S.dis_r[1]=0;
        S.dis_c[1]=1;
        S.dis_r[2]=1;
        S.dis_c[2]=0;
        S.dis_r[3]=1;
        S.dis_c[3]=1;
        S.dis_r[4]=1;
        S.dis_c[4]=2;

        S.amt_rec[0]=1;
        S.amt_rec[1]=3;

        S.to_find_row[0]=0;
        S.to_find_row[1]=1;
        S.to_find_row[2]=0;
        S.to_find_row[3]=-50;
    }
    else if( a=='T' && b=='2' ){
        S.dis_r[1]=0;
        S.dis_c[1]=1;
        S.dis_r[2]=1;
        S.dis_c[2]=0;
        S.dis_r[3]=1;
        S.dis_c[3]=1;
        S.dis_r[4]=2;
        S.dis_c[4]=1;

        S.amt_rec[0]=1;
        S.amt_rec[1]=2;
        S.amt_rec[2]=1;

        S.to_find_row[0]=0;
        S.to_find_row[1]=1;
        S.to_find_row[2]=-50;
        S.to_find_row[3]=-50;
    }
    else if( a=='T' && b=='3' ){
        S.dis_r[1]=0;
        S.dis_c[1]=0;
        S.dis_r[2]=0;
        S.dis_c[2]=1;
        S.dis_r[3]=0;
        S.dis_c[3]=2;
        S.dis_r[4]=1;
        S.dis_c[4]=1;

        S.amt_rec[0]=3;
        S.amt_rec[1]=1;

        S.to_find_row[0]=1;
        S.to_find_row[1]=1;
        S.to_find_row[2]=1;
        S.to_find_row[3]=-50;
    }
    else if( a=='T' && b=='4' ){
        S.dis_r[1]=0;
        S.dis_c[1]=0;
        S.dis_r[2]=1;
        S.dis_c[2]=0;
        S.dis_r[3]=1;
        S.dis_c[3]=1;
        S.dis_r[4]=2;
        S.dis_c[4]=0;

        S.amt_rec[0]=1;
        S.amt_rec[1]=2;
        S.amt_rec[2]=1;

        S.to_find_row[0]=1;
        S.to_find_row[1]=0;
        S.to_find_row[2]=-50;
        S.to_find_row[3]=-50;
    }
    else if( a=='L' && b=='1' ){
        S.dis_r[1]=0;
        S.dis_c[1]=0;
        S.dis_r[2]=0;
        S.dis_c[2]=1;
        S.dis_r[3]=1;
        S.dis_c[3]=0;
        S.dis_r[4]=2;
        S.dis_c[4]=0;

        S.amt_rec[0]=2;
        S.amt_rec[1]=1;
        S.amt_rec[2]=1;

        S.to_find_row[0]=1;
        S.to_find_row[1]=1;
        S.to_find_row[2]=-50;
        S.to_find_row[3]=-50;
    }
    else if( a=='L' && b=='2' ){
        S.dis_r[1]=0;
        S.dis_c[1]=0;
        S.dis_r[2]=1;
        S.dis_c[2]=0;
        S.dis_r[3]=1;
        S.dis_c[3]=1;
        S.dis_r[4]=1;
        S.dis_c[4]=2;

        S.amt_rec[0]=1;
        S.amt_rec[1]=3;

        S.to_find_row[0]=1;
        S.to_find_row[1]=0;
        S.to_find_row[2]=0;
        S.to_find_row[3]=-50;
    }
    else if( a=='L' && b=='3' ){
        S.dis_r[1]=0;
        S.dis_c[1]=1;
        S.dis_r[2]=1;
        S.dis_c[2]=1;
        S.dis_r[3]=2;
        S.dis_c[3]=0;
        S.dis_r[4]=2;
        S.dis_c[4]=1;

        S.amt_rec[0]=1;
        S.amt_rec[1]=1;
        S.amt_rec[2]=2;

        S.to_find_row[0]=-1;
        S.to_find_row[1]=1;
        S.to_find_row[2]=-50;
        S.to_find_row[3]=-50;
    }
    else if( a=='L' && b=='4' ){
        S.dis_r[1]=0;
        S.dis_c[1]=0;
        S.dis_r[2]=0;
        S.dis_c[2]=1;
        S.dis_r[3]=0;
        S.dis_c[3]=2;
        S.dis_r[4]=1;
        S.dis_c[4]=2;

        S.amt_rec[0]=3;
        S.amt_rec[1]=1;

        S.to_find_row[0]=1;
        S.to_find_row[1]=1;
        S.to_find_row[2]=1;
        S.to_find_row[3]=-50;
    }
    else if( a=='J' && b=='1' ){
        S.dis_r[1]=0;
        S.dis_c[1]=0;
        S.dis_r[2]=0;
        S.dis_c[2]=1;
        S.dis_r[3]=1;
        S.dis_c[3]=1;
        S.dis_r[4]=2;
        S.dis_c[4]=1;

        S.amt_rec[0]=2;
        S.amt_rec[1]=1;
        S.amt_rec[2]=1;

        S.to_find_row[0]=1;
        S.to_find_row[1]=1;
        S.to_find_row[2]=-50;
        S.to_find_row[3]=-50;
    }
    else if( a=='J' && b=='2' ){
        S.dis_r[1]=0;
        S.dis_c[1]=0;
        S.dis_r[2]=0;
        S.dis_c[2]=1;
        S.dis_r[3]=0;
        S.dis_c[3]=2;
        S.dis_r[4]=1;
        S.dis_c[4]=0;

        S.amt_rec[0]=3;
        S.amt_rec[1]=1;

        S.to_find_row[0]=1;
        S.to_find_row[1]=1;
        S.to_find_row[2]=1;
        S.to_find_row[3]=-50;
    }
    else if( a=='J' && b=='3' ){
        S.dis_r[1]=0;
        S.dis_c[1]=0;
        S.dis_r[2]=1;
        S.dis_c[2]=0;
        S.dis_r[3]=2;
        S.dis_c[3]=0;
        S.dis_r[4]=2;
        S.dis_c[4]=1;

        S.amt_rec[0]=1;
        S.amt_rec[1]=1;
        S.amt_rec[2]=2;

        S.to_find_row[0]=1;
        S.to_find_row[1]=-1;
        S.to_find_row[2]=-50;
        S.to_find_row[3]=-50;
    }
    else if( a=='J' && b=='4' ){
        S.dis_r[1]=0;
        S.dis_c[1]=2;
        S.dis_r[2]=1;
        S.dis_c[2]=0;
        S.dis_r[3]=1;
        S.dis_c[3]=1;
        S.dis_r[4]=1;
        S.dis_c[4]=2;

        S.amt_rec[0]=1;
        S.amt_rec[1]=3;

        S.to_find_row[0]=0;
        S.to_find_row[1]=0;
        S.to_find_row[2]=1;
        S.to_find_row[3]=-50;
    }
    else if( a=='S' && b=='1' ){
        S.dis_r[1]=0;
        S.dis_c[1]=0;
        S.dis_r[2]=0;
        S.dis_c[2]=1;
        S.dis_r[3]=1;
        S.dis_c[3]=1;
        S.dis_r[4]=1;
        S.dis_c[4]=2;

        S.amt_rec[0]=2;
        S.amt_rec[1]=2;

        S.to_find_row[0]=1;
        S.to_find_row[1]=1;
        S.to_find_row[2]=0;
        S.to_find_row[3]=-50;
    }
    else if( a=='S' && b=='2' ){
        S.dis_r[1]=0;
        S.dis_c[1]=1;
        S.dis_r[2]=1;
        S.dis_c[2]=0;
        S.dis_r[3]=1;
        S.dis_c[3]=1;
        S.dis_r[4]=2;
        S.dis_c[4]=0;

        S.amt_rec[0]=1;
        S.amt_rec[1]=2;
        S.amt_rec[2]=1;

        S.to_find_row[0]=0;
        S.to_find_row[1]=1;
        S.to_find_row[2]=-50;
        S.to_find_row[3]=-50;
    }
    else if( a=='Z' && b=='1' ){
        S.dis_r[1]=0;
        S.dis_c[1]=1;
        S.dis_r[2]=0;
        S.dis_c[2]=2;
        S.dis_r[3]=1;
        S.dis_c[3]=0;
        S.dis_r[4]=1;
        S.dis_c[4]=1;

        S.amt_rec[0]=2;
        S.amt_rec[1]=2;

        S.to_find_row[0]=0;
        S.to_find_row[1]=1;
        S.to_find_row[2]=1;
        S.to_find_row[3]=-50;
    }
    else if( a=='Z' && b=='2' ){
        S.dis_r[1]=0;
        S.dis_c[1]=0;
        S.dis_r[2]=1;
        S.dis_c[2]=0;
        S.dis_r[3]=1;
        S.dis_c[3]=1;
        S.dis_r[4]=2;
        S.dis_c[4]=1;

        S.amt_rec[0]=1;
        S.amt_rec[1]=2;
        S.amt_rec[2]=1;

        S.to_find_row[0]=1;
        S.to_find_row[1]=0;
        S.to_find_row[2]=-50;
        S.to_find_row[3]=-50;
    }
    else if( a=='I' && b=='1' ){
        S.dis_r[1]=0;
        S.dis_c[1]=0;
        S.dis_r[2]=1;
        S.dis_c[2]=0;
        S.dis_r[3]=2;
        S.dis_c[3]=0;
        S.dis_r[4]=3;
        S.dis_c[4]=0;

        S.amt_rec[0]=1;
        S.amt_rec[1]=1;
        S.amt_rec[2]=1;
        S.amt_rec[3]=1;

        S.to_find_row[0]=1;
        S.to_find_row[1]=-50;
        S.to_find_row[2]=-50;
        S.to_find_row[3]=-50;
    }
    else if( a=='I' && b=='2' ){
        S.dis_r[1]=0;
        S.dis_c[1]=0;
        S.dis_r[2]=0;
        S.dis_c[2]=1;
        S.dis_r[3]=0;
        S.dis_c[3]=2;
        S.dis_r[4]=0;
        S.dis_c[4]=3;

        S.amt_rec[0]=4;

        S.to_find_row[0]=1;
        S.to_find_row[1]=1;
        S.to_find_row[2]=1;
        S.to_find_row[3]=1;
    }
    else if( a=='O' ){
        S.dis_r[1]=0;
        S.dis_c[1]=0;
        S.dis_r[2]=0;
        S.dis_c[2]=1;
        S.dis_r[3]=1;
        S.dis_c[3]=0;
        S.dis_r[4]=1;
        S.dis_c[4]=1;

        S.amt_rec[0]=2;
        S.amt_rec[1]=2;

        S.to_find_row[0]=1;
        S.to_find_row[1]=1;
        S.to_find_row[2]=-50;
        S.to_find_row[3]=-50;
    }

    return S;
}

//編集を行う前のファイルです
#include <stdio.h> 
#include <stdlib.h>
#include <time.h>

#define NUM_OF_BOM 13
#define MASU_ROW  9
#define MASU_COLUMN 9

/*--乱数生成用関数--*/
int GetRandom(int max){
    //1~MAXまでの乱数を発生
    return rand() % max + 1;
}

/*--マスの状態--*/
enum MASU_KIND{ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,BOM
               ,Invalid};
enum MASU_FLAG{FALSE,TRUE,PIN};
/*--1マスを表す--*/
typedef struct
{
    enum MASU_KIND kind;//マスの現在状態
    enum MASU_FLAG flag;//
}MASU_BLOCK;

/*--フィールドを表示する関数です--*/
void MASU_draw(MASU_BLOCK masu[MASU_ROW][MASU_COLUMN]){
    printf("  １２３４５６７８９\n");
    printf("  ーーーーーーーーー\n");
    for(int i = 0;i < MASU_ROW;i ++){
        printf("%d|",i+1);
        for(int j = 0;j < MASU_COLUMN;j ++){
            //1:ブロックが判明していない時
            if(masu[i][j].flag==FALSE){
                printf("・");
            }
            //2:ブロックが判明していない時
            else if(masu[i][j].flag==PIN){
                printf("🚩");
            }
            //3:ブロックが判明している時
            else{
                switch(masu[i][j].kind){
                    case ZERO: printf("０");break;
                    case ONE: printf("１");break;
                    case TWO: printf("２");break;
                    case THREE: printf("３");break;
                    case FOUR: printf("４");break;
                    case FIVE: printf("５");break;
                    case SIX: printf("６");break;
                    case SEVEN: printf("７");break;
                    case EIGHT: printf("８");break;
                    case BOM: printf("💣");break;
                }
            }
        }
        puts("");//改行
    }
}

/*--再帰的に0のマスを開いてゆく関数--*/
/*-全て配列座標を用いています-*/
void OPEN_SAIKI(MASU_BLOCK masu[MASU_ROW][MASU_COLUMN],int i,int j){
    if(masu[i][j].kind==ZERO&&masu[i][j].flag==FALSE){
        masu[i][j].flag=TRUE;
        if((masu[i-1][j-1].kind==ZERO)&&i-1>-1&&j-1>-1)OPEN_SAIKI(masu,i-1,j-1);//上段左
        if((masu[i-1][j].kind==ZERO)&&i-1>-1)OPEN_SAIKI(masu,i-1,j);//上段中央
        if((masu[i-1][j+1].kind==ZERO)&&i-1>-1&&j+1<MASU_COLUMN)OPEN_SAIKI(masu,i-1,j+1);//上段右
        if((masu[i][j-1].kind==ZERO)&&j-1>-1)OPEN_SAIKI(masu,i,j-1);//中段左
        if((masu[i][j+1].kind==ZERO)&&j+1<MASU_COLUMN)OPEN_SAIKI(masu,i,j+1);//中段右
        if((masu[i+1][j-1].kind==ZERO)&&i+1<MASU_ROW&&j-1>-1)OPEN_SAIKI(masu,i+1,j-1);//下段左
        if((masu[i+1][j].kind==ZERO)&&i+1<MASU_ROW)OPEN_SAIKI(masu,i+1,j);//下段中央
        if((masu[i+1][j+1].kind==ZERO)&&i+1<MASU_ROW&&j+1<MASU_COLUMN)OPEN_SAIKI(masu,i+1,j+1);//下段右
    }
}

//*--0周りのマスを開く関数--*//
//*--全て配列座標を用いています--*//
void OPEN_AROUND_ZERO(MASU_BLOCK masu[MASU_ROW][MASU_COLUMN]){
    for(int i = 0;i < MASU_ROW;i ++){
        for(int j = 0;j < MASU_COLUMN;j ++){
            if(masu[i][j].flag==TRUE&&masu[i][j].kind==ZERO){
                for(int k = i-1;k < i+2;k ++){
                    for(int l = j-1;l < j+2;l ++){
                        if(-1<k&&k<MASU_ROW&&-1<l&&l<MASU_COLUMN){
                            if(masu[k][l].flag==FALSE){
                                masu[k][l].flag=TRUE;
                            }
                        }
                    }
                }       
            }
        }
    }
}

/*--ステージを作成する関数--*/
/*--引数:ブロックの配列,自分の選択したブロックの座標(現実座標)--*/
/*--注意:この関数内でのみ座標は現実座標(1~9)で表しています--*/
void MASU_make(MASU_BLOCK masu[MASU_ROW][MASU_COLUMN],int i,int j){
    int counter = 0;
    int memo[NUM_OF_BOM][2]={};//ボムを記録する配列です
    //1ボムの配置
    while(counter < NUM_OF_BOM){
        while(1){
            //ボム再検索の条件
            //・以前に指定したボムと位置が被る時
            int flag = 0;
            int a = GetRandom(9);
            int b = GetRandom(9);
            if((i-2<a&&a<i+2)&&(j-2<b&&b<j+2)){
                flag=1;
            }
            for(int k = 0;k < NUM_OF_BOM;k ++){
                if(a==memo[k][0]&&b==memo[k][1]){
                    flag=1;
                }
            }
            if(flag==0){//whileループ脱出用
                memo[counter][0] = a;
                memo[counter][1] = b;
                counter += 1;
                masu[a-1][b-1].flag = FALSE;
                masu[a-1][b-1].kind = BOM;
                break;
            }
        }
    }
    //2番号を振る
    for(int k = 0;k < MASU_ROW;k++){
        for(int l = 0;l < MASU_COLUMN;l++){
            if(masu[k][l].kind != BOM){
                int counter2 = 0;
                for(int m = k;m < k+3;m ++){//現実座標で探索
                    for(int n = l;n < l+3;n++){//現実座標で探索
                        for(int o = 0;o < NUM_OF_BOM;o ++){
                            if((memo[o][0]==m)&&(memo[o][1]==n)){
                                counter2 += 1;
                            }
                        }
                    }
                }
                masu[k][l].flag=FALSE;
                masu[k][l].kind=counter2;
            }
        }
    }
    //3OPEN処理をする(OPEN_SAIKI関数を呼び出す&OPEN_AOUND_ZEROでオープン処理)
    OPEN_SAIKI(masu,i-1,j-1);
    OPEN_AROUND_ZERO(masu);
}

/*--オープン用の関数--*/
/*--配列座標で管理--*/
/*--ZERO再帰オープン組み込み済み--*/
void OPEN(MASU_BLOCK masu[MASU_ROW][MASU_COLUMN],int i,int j){
    if(masu[i][j].flag==TRUE){
        printf("そのマスはもう空いているよ\n");
    }
    else{
        OPEN_SAIKI(masu,i,j);
        OPEN_AROUND_ZERO(masu);//零マスだけは先に処理する
        masu[i][j].flag=TRUE;
        printf("%d行%d列をオープンしました\n",i+1,j+1);
    }
}

/*--フラッグを立てる関数--*/
/*--配列座標で管理--*/
void FLAG(MASU_BLOCK masu[MASU_ROW][MASU_COLUMN],int i,int j){
    if(masu[i][j].flag==TRUE){
        printf("そこに旗をたてることは出来ません\n");
    }
    else if(masu[i][j].flag==PIN){
        printf("旗を回収しました\n");
        masu[i][j].flag=FALSE;
    }
    else{
        masu[i][j].flag=PIN;
    }
}

/*--GAME_CLEAR_OVER関数--*/
/*--ゲーム終了条件を満たしているかどうかを判別--*/
int GAME_CLEAR_OVER(MASU_BLOCK masu[MASU_ROW][MASU_COLUMN],int i,int j){
    int counter = 0;
    if(masu[i][j].kind==BOM&&masu[i][j].flag==TRUE){
        printf("爆弾を掘り当ててしまいました!\nGAME OVER\n");
        return -1;
    }
    else{
        for(int k=0;k < MASU_ROW;k++){
            for(int l = 0;l <MASU_COLUMN;l++){
                if(masu[k][l].flag==TRUE){
                    counter+=1;
                }
            }
        }
        if(counter==(MASU_ROW*MASU_COLUMN-NUM_OF_BOM)){
            printf("ゲームクリアです!おめでとう!\n");
            return 1;
        }
        else{
            return 0;//判定に何も引っかからない場合
        }
    }
}

int TYTLE(void){
    int a = 0;
    printf("🚩💣マインスイーパー💣🚩\n");
    while(a<1||a>2){
        printf("1ゲーム開始,2ゲーム終了:");
        scanf("%d",&a);
    }
    switch (a){
    case 1:return 1;break;
    case 2:return -1;break;
    }
}

void GAME(void){
    int a = TYTLE();
    while(a != -1){
        int row = -1;
        int column = -1;
        int how = -1;
        int b = -1;
        MASU_BLOCK blocks[MASU_ROW][MASU_COLUMN] = {};
        MASU_draw(blocks);
        while(row<1||row>MASU_ROW){
            printf("開始位置を指定しましょう 行:");
            scanf("%d",&row);
        }
        while(column<1||column>MASU_COLUMN){
            printf("開始位置を指定しましょう 列:");
            scanf("%d",&column);
        }
        MASU_make(blocks,row,column);MASU_draw(blocks);
        while(1){
            while(b!=0){//行動選択用のwhileループ
                how=-1;
                while(how<1||how>2){
                    printf("穴を掘る:1,旗を立てる2:");
                    scanf("%d",&how);
                }
                row=-1;
                while(row<1||row>MASU_ROW){
                    printf("1:位置を指定しましょう 行:");
                    scanf("%d",&row);
                }
                column=-1;
                while(column<1||column>MASU_COLUMN){
                    printf("2:位置を指定しましょう 列:");
                    scanf("%d",&column);
                }
                switch(how){
                    case 1:printf("%d行%d列に穴を掘りますyes...0 no...1:",row,column);
                           scanf("%d",&b);break;
                    case 2:printf("%d行%d列に旗をたてますyes...0 no...1:",row,column);
                           scanf("%d",&b);break;
                }
            }
            b = -1;
            switch(how){
                //穴を掘る
                case 1:OPEN(blocks,row-1,column-1);
                       break;
                //旗をたてる
                case 2:FLAG(blocks,row-1,column-1);
                       break;
            }
            MASU_draw(blocks);
            int flag = GAME_CLEAR_OVER(blocks,row-1,column-1);
            switch(flag){
                case 0:break;
                case -1:
                case 1:printf("再挑戦0,終了-1:");
                       scanf("%d",&a);
            }
            if(flag==-1||flag==1)break;
        }
    }
}

int main(void){
    srand(time(0));
    GAME();
    return 0;
}


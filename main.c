#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <wchar.h>
#include <locale.h>
#include "stdio.h"
int con(int*b)
{
    *b=*b+30;
}
recon(int*b)
{
    *b=*b-30;
}
int pos=30;
int pos2=30;
int q=0;
con2(int *x)
{
    *x=*x+1;
}
con3(int*x)
{
    *x=*x-1;
}

typedef struct
{
    int x;
    int y;
    int a;
    int b;
} index;
typedef struct
{
    index move;
    index castlemove;
    index enpassat;
    index castle_removed;
    char removed;
    char promotion;
    int is_promoted;
    int is_removed;
    int is_castled;
    int is_enpass;
} movement;
char castle[7]= {'r','k','r','R','K','R'};
char white_removed[16],black_removed[16],board[8][8],temp[8][8];
char filename[30];
int game = 1,roundgame=0;
int moviind[4];
int whichturn=0;
int num_of_undoes=0;
int num_of_redoes=0;
int will_redo;
int can_redo;
int checked_king;
int red_col;
int undo=0;
int whichgame=0;
FILE*newfile;
index red_col_button;
index moveindex;
index kingSindex;
movement history[1000];
movement redo_info[1000];
GtkStyleContext *invalid_context;
GtkWidget *fixed, *end_game_fixed;
GtkWidget *image,*images[32];
GtkWidget *buttons[8][8];
GtkWidget *promotionWindow, *window, *end_game_window;
GtkStyleContext *context1, *context2, *invalid_context, *context;
void boarddecl(char board[][8])
{
    for (int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(i==1)
            {
                board[i][j]='P';
            }
            else if (i==6)
            {
                board[i][j]='p';
            }
            else
            {
                if((i==0&&j==0)||(i==0&&j==7))
                {
                    board[i][j]='R';
                }
                else if(i==0&&(j==1||j==6))
                {
                    board[i][j]='N';
                }
                else if(i==0&&(j==2||j==5))
                {
                    board[i][j]='B';
                }
                else if(i==0&&j==3)
                {
                    board[i][j]='Q';
                }
                else if(i==0&&j==4)
                {
                    board[i][j]='K';
                }
                else if((i==7&&j==0)||(i==7&&j==7))
                {
                    board[i][j]='r';
                }
                else if(i==7&&(j==1||j==6))
                {
                    board[i][j]='n';
                }
                else if(i==7&&(j==2||j==5))
                {
                    board[i][j]='b';
                }
                else if(i==7&&j==4)
                {
                    board[i][j]='k';
                }
                else if(i==7&&j==3)
                {
                    board[i][j]='q';
                }
                else
                {
                    board[i][j]=' ';
                }

            }

        }
    }
}
int upOloCase(char a)
{
    if(a>='A'&&a<='Z')
    {
        return 1;
    }
    else if (a>='a'&&a<='z')
    {
        return 0;
    }
    else
    {
        return 2;
    }
}
int knight(int x,int y,int a,int b,char board[][8])
{
    int flag=0;
    if((b==y+2&&a==x+1)||(b==y-2&&a==x+1)||(b==y-2&&a==x-1)||(b==y+2&&a==x-1)||(b==y+1&&a==x+2)||(b==y-1&&a==x+2)||(b==y+1&&a==x-2)||(b==y-1&&a==x-2))
    {
        flag=1;
    }
    return flag;

}
int rook(int x,int y,int a,int b,char board[][8])
{
    int flag=1;
    if(a==x)
    {
        if(y<b)
        {
            for(int i=y+1; i<b; i++)
            {
                if(board[a][i]!=' ')
                {
                    flag=0;
                    break;
                }

            }
        }
        else if(y>b)
        {
            for(int i=y-1; i>b; i--)
            {
                if(board[a][i]!=' ')
                {
                    flag=0;
                    break;
                }
            }
        }
    }

    else if(b==y)
    {
        if(x<a)
        {
            for(int i=x+1; i<a; i++)
            {
                if(board[i][b]!=' ')
                {
                    flag=0;
                    break;
                }

            }
        }
        else if(x>a)
        {
            for(int i=x-1; i>a; i--)
            {
                if(board[i][b]!=' ')
                {
                    flag=0;
                    break;
                }
            }
        }
    }
    else
    {
        flag=0;
    }

    return flag;
}



int bishop(int x,int y,int a,int b,char board[][8])
{

    int flag =0;
    if(abs(a-x)==abs(b-y)&&a!=x&&b!=y)
    {
        flag=1;

        for(int i=0; i<abs(a-x); i++)
        {
            if     (x>a&&y<b)
            {
                if(board[x-i][y+i]!=' '&&i!=0)
                {
                    flag=0;
                }
            }
            else if(x<a&&y<b)
            {
                if(board[x+i][y+i]!=' '&&i!=0)
                {
                    flag=0;
                }
            }
            else if(x>a&&y>b)
            {
                if(board[x-i][y-i]!=' '&&i!=0)
                {
                    flag=0;
                }
            }
            else if(x<a&&y>b)
            {
                if(board[x+i][y-i]!=' '&&i!=0)
                {
                    flag=0;
                }
            }
        }
    }

    return flag;

}

int queen(int x,int y,int a,int b,char board[][8])
{

    int flag=0;
    if(abs(x-a)==abs(y-b))
    {
        return bishop(x,y,a,b,board);
    }
    else if(x==a||y==b)
    {
        return rook(x,y,a,b,board);
    }
    else
    {
        return flag;
    }

}
int king(int x,int y,int a,int b,char board[][8])
{
    int flag=0;

    if((b==y&&(a==x+1||a==x-1))||(a==x&&(b==y+1||b==y-1))||(a==x+1&&(b==y+1||b==y-1))||(a==x-1&&(b==y-1||b==y+1)))
    {
        flag =1;
        for(int n=a-1; n<a+2; n++)
        {
            if (n<8&&n>=0)
            {
                for(int k=b-1; k<b+2; k++)
                {
                    if (k<8&&k>=0)
                    {
                        if(((board[n][k]=='K')&&(board[x][y]=='k'))||((board[n][k]=='k')&&(board[x][y]=='K')))
                        {

                            flag =0;
                        }
                    }
                }
            }
        }
    }
    return flag;
}
int enpassant_w[8];
int enpassant_b[8];
void init_enPassant_arr(void)
{
    int i;
    for (i = 0; i < 8; ++i)
    {
        enpassant_w[i] = 0;
        enpassant_b[i] = 0;
    }
}
void enPassant_arr(int x, int y,char board[][8])
{
    if(board[x][y]=='p')
    {
        enpassant_w[y] =1;
        return ;
    }
    else if(board[x][y]=='P')
    {
        enpassant_b[y] = 1;
        return ;
    }
}
int enPassant(int x,int y,int b,char board[][8] )
{
    if(board[x][y]=='p')
    {
        if (x == 3)
        {
            return enpassant_b[b];
        }
        else
        {
            return 0;
        }
    }
    else if(board[x][y]=='P')
    {
        if (x == 4)
        {
            return enpassant_w[b];
        }
        else
        {
            return 0;
        }
    }
}
int pawns(int x,int y,int a,int b,char board[][8])
{
    int flag =0;
    if(board[x][y]=='p')
    {
        if(x==6)
        {

            if (a==x-2&&b==y&&board[x-1][y]==' '&&board[a][b]==' ')
            {
                flag=1;
                history[whichturn].is_enpass=0;

                enPassant_arr(x,y,board);
            }

        }
        if(a==x-1&&b==y&&board[a][b]==' ')
        {
            flag=1;
            history[whichturn].is_enpass=0;
            if(a==0)
            {
                history[whichturn].is_enpass=0;
                flag=2;
            }
        }

        else if(a==(x-1)&&(b==y+1))
        {
            if(board[a][b]!=' ')
            {
                history[whichturn].is_enpass=0;
                flag=1;
            }
            else if(enPassant(x,y,b,board))
            {
                history[whichturn].is_enpass=1;
                history[whichturn].enpassat.x=x;
                history[whichturn].enpassat.a=a;
                history[whichturn].enpassat.b=b;

                flag=1;
            }
        }
        else if(a==(x-1)&&b==y-1)
        {
            if(board[a][b]!=' ')
            {
                history[whichturn].is_enpass=0;
                flag=1;
            }
            else if(enPassant(x,y,b,board))
            {
                history[whichturn].is_enpass=1;
                history[whichturn].is_enpass=1;
                history[whichturn].enpassat.x=x;
                history[whichturn].enpassat.a=a;
                history[whichturn].enpassat.b=b;
                flag=1;
            }
        }
    }
    else if(board[x][y]=='P')
    {
        if(x==1)
        {
            if(a==(x+2)&&b==y&&board[x+1][y]==' '&&board[a][b]==' ')
            {
                flag=1;
                history[whichturn].is_enpass=0;

                enPassant_arr(x,y,board);
            }
        }
        if(a==x+1&&b==y&&board[a][b]==' ')
        {
            flag=1;
            history[whichturn].is_enpass=0;
            if(a==7)
            {
                history[whichturn].is_enpass=0;
                flag=2;
            }
        }
        else if(a==x+1&&(b==y+1))
        {
            if(board[a][b]!=' ')
            {
                history[whichturn].is_enpass=0;
                flag=1;
            }

            else if(enPassant(x,y,b,board))
            {
                history[whichturn].is_enpass=1;
                history[whichturn].enpassat.x=x;
                history[whichturn].enpassat.a=a;
                history[whichturn].enpassat.b=b;

                flag=1;
            }
        }
        else if(a==x+1&&b==y-1)
        {
            if (board[a][b]!=' ')
            {
                history[whichturn].is_enpass=0;
                flag=1;
            }
            else if(enPassant(x,y,b,board))
            {
                history[whichturn].is_enpass=1;

                history[whichturn].enpassat.x=x;
                history[whichturn].enpassat.a=a;
                history[whichturn].enpassat.b=b;
                flag=1;
            }
        }
    }
    return flag;
}

index findTheIndex(char board[][8])
{
    index kingSindex;
    for(int i =0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if (board[i][j]=='K')
            {
                kingSindex.x=i;
                kingSindex.y=j;
            }
            else if(board[i][j]=='k')
            {
                kingSindex.a=i;
                kingSindex.b=j;
            }
        }
    }
    return kingSindex;
}
int kesh(index kingSindex,char board[][8])
{
    int flag1=0,flag2=0;
    for (int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(knight(kingSindex.x,kingSindex.y,i,j,board)==1&&(board[i][j]=='n')||
                    bishop(kingSindex.x,kingSindex.y,i,j,board)==1&&(board[i][j]=='b'||board[i][j]=='q')||
                    rook(kingSindex.x,kingSindex.y,i,j,board)==1&&(board[i][j]=='r'||board[i][j]=='q')||
                    i==(kingSindex.x)+1&&abs(j-kingSindex.y)==1&&(board[i][j]=='p'))
            {
                flag1 =1;
            }
            if(knight(kingSindex.a,kingSindex.b,i,j,board)==1&&(board[i][j]=='N')||
                    bishop(kingSindex.a,kingSindex.b,i,j,board)==1&&(board[i][j]=='B'||board[i][j]=='Q')||
                    rook(kingSindex.a,kingSindex.b,i,j,board)==1&&(board[i][j]=='R'||board[i][j]=='Q')||
                    i==(kingSindex.a)-1&&abs(j-kingSindex.b)==1&&(board[i][j]=='P'))
            {
                flag2 =1;
            }
        }
    }
    if(flag1==1&&flag2==1)
    {
        return 3;
    }
    else if((flag1==1&&flag2!=1))
    {
        return 1;
    }
    else if(flag1!=1&&flag2==1)
    {
        return 2;
    }
    else if(flag1!=1&&flag2!=1)
    {
        return 0;
    }
}
void copyarr_withchange(char board[][8],char temp[][8],int x,int y,int a,int b)
{
    for(int i=0; i<8; i++)
    {
        for (int j=0; j<8; j++)
        {
            temp[i][j]=board[i][j];
        }
    }
    temp[a][b]=temp[x][y];
    temp[x][y]= ' ';
}
int castling(int x,int y,int a,int b,char board[][8],char castle[7])
{
    char temp[8][8];
    index trial;
    int flag=1;
    index kingind;
    kingind = findTheIndex(board);
    if((board[x][y]=='k'&&x==7&&b-y==-2&&(castle[0]!=' '&&castle[1]!=' '))||(board[x][y]=='K'&&x==0&&b-y==-2&&(castle[3]!=' '&&castle[4]!=' ')))
    {
        if(upOloCase(board[x][y])==1)
        {
            for(int j=2; j<4; j++)
            {
                copyarr_withchange(board,temp,0,y,0,j);
                trial=findTheIndex(temp);
                if(board[0][j]!=' '||kesh(trial,temp)==1||kesh(kingind,board)==1)
                {
                    flag=0;
                    return 0;
                }
            }
            if (flag==1&&board[0][1]==' ')
            {
                board[0][3]='R';
                board[0][0]=' ';
                set_image(3,0);
                set_image(0,0);
                history[whichturn].castlemove.x = 0;
                history[whichturn].castlemove.y = 0;
                history[whichturn].castlemove.a = 0;
                history[whichturn].castlemove.b = 3;

                for(int j=3; j<5; j++)
                {
                    castle[j]=' ';
                }
                return 1;
            }
        }
        else
        {
            for(int j=2; j<4; j++)
            {
                copyarr_withchange(board,temp,7,y,7,j);
                trial=findTheIndex(temp);
                if(board[7][j]!=' '||kesh(trial,temp)==2||kesh(kingind,board)==2)
                {
                    flag=0;
                    return 0;
                }
            }
            if (flag==1&&board[7][1]==' ')
            {

                board[7][3]='r';
                board[7][0]=' ';
                set_image(3,7);
                set_image(0,7);
                history[whichturn].castlemove.x = 7;
                history[whichturn].castlemove.y = 0;
                history[whichturn].castlemove.a = 7;
                history[whichturn].castlemove.b = 3;
                for(int j=0; j<2; j++)
                {
                    castle[j]=' ';
                }
                return 1;
            }

        }
    }
    else if((board[x][y]=='k'&&x==7&&b-y==2&&(castle[2]!=' '&&castle[1]!=' '))||(board[x][y]=='K'&&x==0&&b-y==2&&(castle[5]!=' '&&castle[4]!=' ')))
    {

        if(upOloCase(board[x][y])==1)
        {
            for(int j=5; j<7; j++)
            {
                copyarr_withchange(board,temp,0,y,0,j);
                trial=findTheIndex(temp);
                if(board[0][j]!=' '||kesh(trial,temp)==1||kesh(kingind,board)==1)
                {
                    flag=0;
                    return 0;
                }
            }
            if (flag==1)
            {
                board[0][5]='R';
                board[0][7]=' ';
                set_image(5,0);
                set_image(7,0);
                history[whichturn].castlemove.x = 0;
                history[whichturn].castlemove.y = 7;
                history[whichturn].castlemove.a = 0;
                history[whichturn].castlemove.b = 5;
                for(int j=4; j<6; j++)
                {
                    castle[j]=' ';
                }
                return 1;

            }
        }
        else
        {
            for(int j=5; j<7; j++)
            {
                copyarr_withchange(board,temp,7,y,7,j);
                trial=findTheIndex(temp);
                if(board[7][j]!=' '||kesh(trial,temp)==2||kesh(kingind,board)==2)
                {
                    flag=0;
                    return 0;
                }
            }
            if (flag==1)
            {
                board[7][5]='r';
                board[7][7]=' ';
                set_image(5,7);
                set_image(7,7);
                history[whichturn].castlemove.x = 7;
                history[whichturn].castlemove.y = 7;
                history[whichturn].castlemove.a = 7;
                history[whichturn].castlemove.b = 5;
                for(int j=1; j<3; j++)
                {
                    castle[j]=' ';
                }

                return 1;
            }
        }
    }
    else
    {
        return 0;
    }
}
int wOb (int i,int j)
{
    if((j%2!=0&&i%2==0||j%2==0&&i%2!=0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int moving(char board[][8],index moveindex)
{
    if(upOloCase(board[moveindex.x][moveindex.y])==upOloCase(board[moveindex.a][moveindex.b])&&board[moveindex.x][moveindex.y]!=' ')
    {
        return 0;
    }
    else
    {
        if(board[moveindex.x][moveindex.y]=='R'||board[moveindex.x][moveindex.y]=='r')
        {
            return rook(moveindex.x,moveindex.y,moveindex.a,moveindex.b,board);
        }
        else if(board[moveindex.x][moveindex.y]=='K'||board[moveindex.x][moveindex.y]=='k')
        {
            return king(moveindex.x,moveindex.y,moveindex.a,moveindex.b,board);
        }
        else if(board[moveindex.x][moveindex.y]=='N'||board[moveindex.x][moveindex.y]=='n')
        {
            return knight(moveindex.x,moveindex.y,moveindex.a,moveindex.b,board);
        }
        else if(board[moveindex.x][moveindex.y]=='B'||board[moveindex.x][moveindex.y]=='b')
        {
            return bishop(moveindex.x,moveindex.y,moveindex.a,moveindex.b,board);
        }
        else if(board[moveindex.x][moveindex.y]=='Q'||board[moveindex.x][moveindex.y]=='q')
        {
            return queen(moveindex.x,moveindex.y,moveindex.a,moveindex.b,board);
        }
        else if(board[moveindex.x][moveindex.y]=='P'||board[moveindex.x][moveindex.y]=='p')
        {
            return(pawns(moveindex.x,moveindex.y,moveindex.a,moveindex.b,board));
        }
        else
        {
            return 0;
        }
    }
}
void check_for_castle(char castle[7],char board[][8],int x,int y)
{
    if(board[x][y]=='k')
    {
        castle[1]=' ';
        history[whichturn].castle_removed.x=1;
        history[whichturn].castle_removed.b=1;
    }

    else if(board[x][y]=='K')
    {
        castle[4]=' ';
        history[whichturn].castle_removed.x=4;
        history[whichturn].castle_removed.b=1;
    }
    else if(board[x][y]=='r'&&y==0)
    {
        castle[0]=' ';
        history[whichturn].castle_removed.x=0;
        history[whichturn].castle_removed.b=1;
    }
    else if(board[x][y]=='R'&&y==0)
    {
        castle[3]=' ';
        history[whichturn].castle_removed.x=3;
        history[whichturn].castle_removed.b=1;
    }
    else if(board[x][y]=='r'&&y==7)
    {
        castle[2]=' ';
        history[whichturn].castle_removed.x=2;
        history[whichturn].castle_removed.b=1;
    }
    else if(board[x][y]=='R'&&y==7)
    {
        castle[5]=' ';
        history[whichturn].castle_removed.x=5;
        history[whichturn].castle_removed.b=1;
    }
}
gboolean usual_color(gpointer user_data)
{
    context = gtk_widget_get_style_context(buttons[red_col_button.x][red_col_button.y]);
    gtk_style_context_remove_class(context,"invalid");

    return FALSE;
}
void red_button(int x,int y,char class_of_button[])
{
    invalid_context = gtk_widget_get_style_context(buttons[y][x]);
    red_col_button.x=y;
    red_col_button.y=x;
    gtk_style_context_add_class(invalid_context,class_of_button);
}
gboolean another_button()
{
    red_button(moveindex.a,moveindex.b,"invalid");
    g_timeout_add(200,usual_color,NULL);
    return FALSE;
}
int change(char board[][8],index moved,char temp[][8])
{
    int redo=0;
    int flag=0;
    int wrong_move=0;
    checked_king=0;
    index kingSindex_in_board;
    index kingSindex_in_temp;
    copyarr_withchange(board,temp,moved.x,moved.y,moved.a,moved.b);
    kingSindex_in_board = findTheIndex(board);
    kingSindex_in_temp  = findTheIndex(temp);
    if((((kesh(kingSindex_in_board,board)==1) && (upOloCase(board[moved.x][moved.y])==1)&&(kesh(kingSindex_in_temp,temp)==1))||(kesh(kingSindex_in_board,board)==2 && (upOloCase(board[moved.x][moved.y])==0)&&(kesh(kingSindex_in_temp,temp)==2)))||(kesh(kingSindex_in_temp,temp)==3))
    {
        if(whichturn%2==0)
        {
            red_button(kingSindex_in_board.a,kingSindex_in_board.b,"invalid");
        }
        else
        {
            red_button(kingSindex_in_board.x,kingSindex_in_board.y,"invalid");
        }
        g_timeout_add(200,usual_color,NULL);
        flag=0;
    }
    else if((((kesh(kingSindex_in_temp,temp)==1) && (upOloCase(board[moved.x][moved.y])==1))||((kesh(kingSindex_in_temp,temp)==2) && (upOloCase(board[moved.x][moved.y])==0)))||(kesh(kingSindex_in_temp,temp)==3))
    {
        if(whichturn%2==0)
        {
            red_button(kingSindex_in_board.a,kingSindex_in_board.b,"invalid");
        }
        else
        {
            red_button(kingSindex_in_board.x,kingSindex_in_board.y,"invalid");
        }
        g_timeout_add(200,usual_color,NULL);
        flag =0;
    }
    else if(switching(moved.x,moved.y,board))
    {
        if(castling(moved.x,moved.y,moved.a,moved.b, board,castle))
        {
            board[moved.a][moved.b]=board[moved.x][moved.y];
            board[moved.x][moved.y]=' ';
            history[whichturn].is_castled=1;
            set_image(moved.b,moved.a);
            set_image(moved.y,moved.x);
            whichturn++;
            flag =1;

        }
        if(moving(board,moved)&&flag==0)
        {
            check_for_castle(castle,board,moved.x,moved.y);
            check_for_castle(castle,board,moved.a,moved.b);
            if(board[moveindex.x][moveindex.y]=='p'||board[moveindex.x][moveindex.y]=='P')
            {
                if(history[whichturn].is_enpass==1)

            {       history[whichturn].is_removed=1;
                    history[whichturn].removed=board[moveindex.x][moveindex.b];
                    board[moveindex.a][moveindex.b]=board[moveindex.x][moveindex.y];
                    table(moved.x,moved.b,redo,board);
                    board[moveindex.x][moveindex.y]=' ';
                    board[moveindex.x][moveindex.b]=' ';
                    set_image(moveindex.b,moveindex.a);
                    set_image(moveindex.y,moveindex.x);
                    set_image(moveindex.b,moveindex.x);
                    whichturn++;
                    flag=1;

                }
                else
                {
                    if(gtk_button_get_image(buttons[moved.b][moved.a])!=NULL)
                    {
                        history[whichturn].removed = board[moved.a][moved.b];
                        history[whichturn].is_removed=1;
                    }
                    table(moved.a,moved.b,redo,board);
                    board[moved.a][moved.b]=board[moved.x][moved.y];
                    board[moved.x][moved.y]=' ';
                    set_image(moved.b,moved.a);
                    set_image(moved.y,moved.x);
                    whichturn++;
                    flag=1;
                    for(int i=0; i<8; i++)
                    {
                        if(((board[0][i]=='p'&&(whichturn-1)%2==0)||(board[7][i]=='P'&&(whichturn-1)%2!=0))&&will_redo!=1)
                        {
                            history[whichturn-1].is_promoted=1;
                            disable_buttons(FALSE);
                            gtk_widget_show_all(promotionWindow);

                        }

                    }
                }
            }
            else
            {
                if(gtk_button_get_image(buttons[moved.b][moved.a])!=NULL)
                {
                    history[whichturn].removed = board[moved.a][moved.b];
                    history[whichturn].is_removed=1;
                }
                table(moved.a,moved.b,redo,board);
                board[moved.a][moved.b]=board[moved.x][moved.y];
                board[moved.x][moved.y]=' ';
                set_image(moved.b,moved.a);
                set_image(moved.y,moved.x);
                whichturn++;
                flag=1;
            }
        }
        else if(flag==0)
        {
            red_button(moveindex.a,moveindex.b,"invalid");
            g_timeout_add(200,usual_color,NULL);
            wrong_move=1;
        }
        if(flag ==1)
        {
            history[whichturn-1].move.x=moved.x;
            history[whichturn-1].move.y=moved.y;
            history[whichturn-1].move.a=moved.a;
            history[whichturn-1].move.b=moved.b;
        }
        index kingind = findTheIndex(board);
        if (kesh(kingind,board)==1&&wrong_move!=1)
        {
            red_button(kingSindex_in_board.x,kingSindex_in_board.y,"invalid");
            g_timeout_add(200,usual_color,NULL);
        }
        else if (kesh(kingind,board)==2&&wrong_move!=1)
        {
            red_button(kingSindex_in_board.a,kingSindex_in_board.b,"invalid");
            g_timeout_add(200,usual_color,NULL);

        }
    }
    return flag;
}
void clicked_button(GtkButton *button, gpointer user_data)
{
    int flag;
    if (roundgame==0)
    {
        int q =GPOINTER_TO_UINT (user_data);
        if (board[q%10][q/10]!=' '&&((upOloCase(board[q%10][q/10])==1&&whichturn%2!=0)||(upOloCase(board[q%10][q/10])==0&&whichturn%2==0)))
        {

            moviind[0]=q%10;
            moviind[1]=q/10;
            red_button(moviind[0],moviind[1],"first_click");
            roundgame++;
        }
    }
    else if(roundgame==1)
    {
        int q =GPOINTER_TO_UINT (user_data);
        if(upOloCase(board[q%10][q/10])==upOloCase(board[moviind[0]][moviind[1]]))
        {
            context = gtk_widget_get_style_context(buttons[moviind[1]][moviind[0]]);
            gtk_style_context_remove_class(context,"first_click");
            moviind[0]=q%10;
            moviind[1]=q/10;
            red_button(moviind[0],moviind[1],"first_click");
        }
        else if((upOloCase(board[q%10][q/10])==1&&whichturn%2==0)||(upOloCase(board[q%10][q/10])==0&&whichturn%2!=0)||(board[q%10][q/10]==' '))
        {
            moviind[2]=q%10;
            moviind[3]=q/10;
            moveindex.x = moviind[0];
            moveindex.y = moviind[1];
            moveindex.a = moviind[2];
            moveindex.b = moviind[3];
            flag = change(board,moveindex,temp);
            context = gtk_widget_get_style_context(buttons[moveindex.y][moveindex.x]);
            gtk_style_context_remove_class(context,"first_click");
            if(num_of_redoes!=num_of_undoes)
            {
                can_redo =0;
                for(int i=0; i<=whichturn+num_of_undoes+1; i++)
                {
                    redo_info[i].move.x=0;
                    redo_info[i].move.y=0;
                    redo_info[i].move.a=0;
                    redo_info[i].move.b=0;
                    redo_info[i].is_promoted=0;
                    redo_info[i].promotion=' ';
                }
                num_of_redoes=0;
                num_of_undoes=0;
            }
            checOrstale(board,whichturn,temp);
            roundgame=0;
        }
    }

}
void to_redo_castle(x,y)
{
    if(x==0)
    {
        castle[4]='K';
        if(y==2)
        {
            castle[3]='R';
        }
        else if (y==6)
        {
            castle[5]='R';
        }
    }
    else if(x==7)
    {
        castle[1]='k';
        if(y==2)
        {
            castle[0]='r';
        }
        else if (y==6)
        {
            castle[2]='r';
        }


    }
}
void undoFun(GtkButton *button, gpointer user_data)
{

    int redo=1;
    if (whichturn==0)
    {
        return 0;
    }
    else
    {
        whichturn--;
        if(board[history[whichturn].move.a][history[whichturn].move.b]=='p'||board[history[whichturn].move.a][history[whichturn].move.b]=='P')
        {
            if(history[whichturn].is_enpass==1)
            {
                if(history[whichturn].enpassat.a==2)
                {
                    board[history[whichturn].enpassat.x][history[whichturn].enpassat.b]='P';

                }
                else if(history[whichturn].enpassat.a==5)
                {
                    board[history[whichturn].enpassat.x][history[whichturn].enpassat.b]='p';

                }
                board[history[whichturn].move.x][history[whichturn].move.y] = board[history[whichturn].move.a][history[whichturn].move.b];
                board[history[whichturn].move.a][history[whichturn].move.b]=' ';
                table(history[whichturn].enpassat.x,history[whichturn].enpassat.b,redo,board);
                set_image(history[whichturn].move.y,history[whichturn].move.x);
                set_image(history[whichturn].enpassat.b,history[whichturn].enpassat.x);
                set_image(history[whichturn].move.b,history[whichturn].move.a);
            }

            else
            {
                board[history[whichturn].move.x][history[whichturn].move.y] = board[history[whichturn].move.a][history[whichturn].move.b];
                set_image(history[whichturn].move.y,history[whichturn].move.x);
                if(history[whichturn].is_removed==1)
                {
                    board[history[whichturn].move.a][history[whichturn].move.b] = history[whichturn].removed;
                    table(history[whichturn].move.a,history[whichturn].move.b,redo,board);
                    set_image(history[whichturn].move.b,history[whichturn].move.a);
                }
                else if(history[whichturn].is_removed!=1)
                {

                    board[history[whichturn].move.a][history[whichturn].move.b] = ' ';
                    set_image(history[whichturn].move.b,history[whichturn].move.a);
                }
            }
        }
        else
        {
            board[history[whichturn].move.x][history[whichturn].move.y] = board[history[whichturn].move.a][history[whichturn].move.b];
            set_image(history[whichturn].move.y,history[whichturn].move.x);
            if(history[whichturn].is_removed==1)
            {
                board[history[whichturn].move.a][history[whichturn].move.b] = history[whichturn].removed;
                table(history[whichturn].move.a,history[whichturn].move.b,redo,board);
                set_image(history[whichturn].move.b,history[whichturn].move.a);
            }
            else if(history[whichturn].is_removed!=1)
            {

                board[history[whichturn].move.a][history[whichturn].move.b] = ' ';
                set_image(history[whichturn].move.b,history[whichturn].move.a);
            }
        }

        if(history[whichturn].castle_removed.b==1&&(history[whichturn].removed=='R'||history[whichturn].removed=='r'))
        {
            char a = board[history[whichturn].move.a][history[whichturn].move.b];
            castle[history[whichturn].castle_removed.x]=a;
        }
        else if (history[whichturn].castle_removed.b==1)
        {
            char a = board[history[whichturn].move.x][history[whichturn].move.y];
            castle[history[whichturn].castle_removed.x]=a;
        }
        if(history[whichturn].is_castled==1)
        {
            board[history[whichturn].castlemove.x][history[whichturn].castlemove.y] = board[history[whichturn].castlemove.a][history[whichturn].castlemove.b];
            board[history[whichturn].castlemove.a][history[whichturn].castlemove.b] = ' ';
            set_image(history[whichturn].castlemove.y,history[whichturn].castlemove.x);
            set_image(history[whichturn].castlemove.b,history[whichturn].castlemove.a);
            to_redo_castle(history[whichturn].move.x,history[whichturn].move.b);
        }
        else if (history[whichturn].is_promoted==1)
        {
            if(history[whichturn].move.a==0)
            {
                board[history[whichturn].move.x][history[whichturn].move.y] = 'p';
            }
            else if(history[whichturn].move.a==7)
            {
                board[history[whichturn].move.x][history[whichturn].move.y] = 'P';
            }
            set_image(history[whichturn].move.y,history[whichturn].move.x);
        }
        memcpy((void *)&redo_info[whichturn], (void *)&history[whichturn], sizeof history[whichturn]);
        can_redo=1;
        history[whichturn].is_removed=0;
        history[whichturn].is_promoted=0;
        history[whichturn].is_castled=0;
        history[whichturn].is_enpass=0;
        history[whichturn].castlemove.x=0;
        history[whichturn].castlemove.y=0;
        history[whichturn].castlemove.a=0;
        history[whichturn].castlemove.b=0;
        history[whichturn].castle_removed = history[whichturn].castlemove;
        history[whichturn].move=history[whichturn].castlemove;
        history[whichturn].enpassat= history[whichturn].castlemove;
        history[whichturn].promotion=' ';
        history[whichturn].removed=' ';
        num_of_undoes++;
    }

}
void redoFun()
{
    int flag;
    will_redo = 1;
    if(can_redo==1)
    {
        flag = change(board,redo_info[whichturn].move,temp);
        if(redo_info[whichturn-1].is_promoted==1)
        {
            board[redo_info[whichturn-1].move.a][redo_info[whichturn-1].move.b] = redo_info[whichturn-1].promotion;
            set_image(redo_info[whichturn-1].move.b,redo_info[whichturn-1].move.a);
            history[whichturn-1].is_promoted=1;
            history[whichturn-1].promotion=redo_info[whichturn-1].promotion;
        }
        will_redo=0;
        redo_info[whichturn-1].is_promoted=0;
        redo_info[whichturn-1].promotion=' ';
        redo_info[whichturn-1].move.x=0;
        redo_info[whichturn-1].move.y=0;
        redo_info[whichturn-1].move.a=0;
        redo_info[whichturn-1].move.b=0;
        num_of_redoes++;
    }
}
void resignFun (GtkButton *button, gpointer user_data)
{
    if (whichturn%2==0)
    {
        end_game_window = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"WHITE RESIGNED!\nWell done! BLACK is the winner.\n");
        gtk_window_set_title(GTK_WINDOW(end_game_window), "GAME ENDED!");
        gtk_dialog_run(GTK_DIALOG(end_game_window));
        gtk_widget_destroy(window);
    }
    else
    {
        end_game_window = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"BLACK RESIGNED!\nWell done! WHITE is the winner.\n");
        gtk_window_set_title(GTK_WINDOW(end_game_window), "GAME ENDED!");
        gtk_dialog_run(GTK_DIALOG(end_game_window));
        gtk_widget_destroy(window);
    }
}
int switching(int x,int y,char board[][8])
{
    if(whichturn%2==0)
    {
        if(upOloCase(board[x][y])==1)
        {
            return 0;
        }
        else
        {
            if(enPassant(x,y,moviind[3],board)==0)
            {
                init_enPassant_arr();
            }
            return 1;
        }
    }
    else if(whichturn%2!=0)
    {
        if(upOloCase(board[x][y])==0)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
}

int validmoves(char a,char board[][8])
{
    int color = upOloCase(a);
    int flag=0;
    char temp[8][8];
    index kingSindex_in_temp;
    for(int k=0; k<8; k++)
    {
        for(int n=0; n<8; n++)
        {
            if (upOloCase(board[k][n])==color)
            {
                for(int m=0; m<8; m++)
                {
                    for(int b=0; b<8; b++)
                    {
                        copyarr_withchange(board,temp,k,n,m,b);
                        kingSindex_in_temp = findTheIndex(temp);
                        if((((upOloCase(board[k][n])!=upOloCase(board[m][b]))&&(m!=k&&n!=b))&&((knight(k,n,m,b,board)==1&&(board[k][n]=='n'||board[k][n]=='N'))||(bishop(k,n,m,b,board)==1&&(board[k][n]=='b'||board[k][n]=='B'||board[k][n]=='Q'||board[k][n]=='q'))||(rook(k,n,m,b,board)==1&&(board[k][n]=='r'||board[k][n]=='R'||board[k][n]=='Q'||board[k][n]=='q'))||(pawns(k,n,m,b,board)==1&&(board[k][n]=='p'||board[k][n]=='P')))))
                        {

                            if(whichturn%2==0&&kesh(kingSindex_in_temp,temp)!=2&&kesh(kingSindex_in_temp,temp)!=3)
                            {
                                flag=1;

                            }
                            else if(whichturn%2!=0&&kesh(kingSindex_in_temp,temp)!=1&&kesh(kingSindex_in_temp,temp)!=3)
                            {
                                // printf("%d %d %d %d %c\n\n",k,n,m,b, board[k][n]);
                                flag=1;
                            }
                        }
                    }
                }
            }
        }
    }
    return flag;
}
int left_pieces(char a,char arr[8][8],gboolean flag)
{
    int color = upOloCase(a);
    int num_total=0,num_piece=0;
    for(int i=0; i<8; i++)
    {
        for (int j=0; j<8; j++)
        {
            if(upOloCase(arr[i][j])==color)
            {
                num_total++;
            }
            if(arr[i][j]==a)
            {
                if (flag == TRUE)
                {
                    return i*10+j;
                    break;
                }
                else
                {
                    num_piece++;
                }
            }
        }
    }
    return num_total*10+num_piece;
}
void draw_situations()
{
    int c = left_pieces('n',board,FALSE);
    int d = left_pieces('N',board,FALSE);
    int e = left_pieces('b',board,FALSE);
    int f = left_pieces('B',board,FALSE);
    int e_index = left_pieces('b',board,TRUE);
    int f_index = left_pieces('B',board,TRUE);
    //KING VS KING
    if(c%10==0&&c/10==1&&d%10==0&&d/10==1)
    {
        game =0;
    }
    // 1 KNIGHT + KING Vs KING
    else if((c%10==1&&c/10==2&&d%10==0&&d/10==1)||(c%10==0&&c/10==1&&d%10==1&&d/10==2))
    {
        game =0;
    }
    // 1 BISHOP+KING Vs KING
    else if((e%10==1&&e/10==2&&f%10==0&&f/10==1)||(e%10==0&&e/10==1&&f%10==1&&f/10==2))
    {
        game =0;
    }
    //KING + BISHOP Vs KING + BISHOP (where both bishops move on same coloured blocks)
    else if(e%10==1&&e/10==2&&f%10==1&&f/10==2)
    {
        if(wOb(e_index/10,e_index%10)==wOb(f_index/10,f_index%10))
        {
            game =0;
        }
    }
    if(game==0)
    {
        end_game_window = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"GAME ENDED AS A DRAW\n");
        gtk_window_set_title(GTK_WINDOW(end_game_window), "GAME ENDED!");
        gtk_dialog_run(GTK_DIALOG(end_game_window));
        gtk_widget_destroy(window);
    }
}
void set_image(int i,int j)
{
    GtkSettings *default_settings = gtk_settings_get_default();
    g_object_set(default_settings, "gtk-button-images", TRUE, NULL);
    if(board[j][i]=='p')
    {
        image = gtk_image_new_from_file ("whitepawn.jpg");
        gtk_button_set_image (GTK_BUTTON (buttons[i][j]), image);
    }
    else if(board[j][i]=='b')
    {
        image = gtk_image_new_from_file ("whitebishop.jpg");
        gtk_button_set_image (GTK_BUTTON (buttons[i][j]), image);
    }
    else if(board[j][i]=='n')
    {
        image = gtk_image_new_from_file ("whiteknight.jpg");
        gtk_button_set_image (GTK_BUTTON (buttons[i][j]), image);
    }
    else if(board[j][i]=='r')
    {
        image = gtk_image_new_from_file ("whiterook.jpg");
        gtk_button_set_image (GTK_BUTTON (buttons[i][j]), image);
    }
    else if(board[j][i]=='q')
    {
        image = gtk_image_new_from_file ("whitequeen.jpg");
        gtk_button_set_image (GTK_BUTTON (buttons[i][j]), image);
    }
    else if(board[j][i]=='k')
    {
        image = gtk_image_new_from_file ("whiteking.jpg");
        gtk_button_set_image (GTK_BUTTON (buttons[i][j]), image);
    }
    else if(board[j][i]=='P')
    {
        image = gtk_image_new_from_file ("c.jpg");
        gtk_button_set_image (GTK_BUTTON (buttons[i][j]), image);
    }
    else if(board[j][i]=='Q')
    {
        image = gtk_image_new_from_file ("queen-removebg-preview_8.jpg");
        gtk_button_set_image (GTK_BUTTON (buttons[i][j]), image);
    }
    else if(board[j][i]=='N')
    {
        image = gtk_image_new_from_file ("black_knight-removebg-preview_6.jpg");
        gtk_button_set_image (GTK_BUTTON (buttons[i][j]), image);
    }
    else if(board[j][i]=='B')
    {
        image = gtk_image_new_from_file ("bishob_black.jpg");
        gtk_button_set_image (GTK_BUTTON (buttons[i][j]), image);
    }
    else if(board[j][i]=='K')
    {
        image = gtk_image_new_from_file ("blackking.jpg");
        gtk_button_set_image (GTK_BUTTON (buttons[i][j]), image);
    }
    else if(board[j][i]=='R')
    {
        image = gtk_image_new_from_file ("blackkkrok.jpg");
        gtk_button_set_image (GTK_BUTTON (buttons[i][j]), image);
    }
    else
    {
        gtk_button_set_image (GTK_BUTTON (buttons[i][j]), NULL);
    }
}
void checOrstale(char board[][8], int whichturn, char temp[][8])
{
    kingSindex = findTheIndex(board);
    index kings_temp;
    int flag=0;
    if (whichturn%2!=0)
    {
        for(int k=0; k<8; k++)
        {
            for(int j=0; j<8; j++)
            {
                copyarr_withchange(board,temp,kingSindex.x,kingSindex.y,k,j);
                kings_temp=findTheIndex(temp);
                if(king(kingSindex.x,kingSindex.y,k,j,board)==1&&kesh(kings_temp,temp)!=1&&upOloCase(board[kingSindex.x][kingSindex.y])!=upOloCase(board[k][j])||validmoves('K',board))
                {
                    flag=1;

                }

            }
        }
        if (flag==0&&kesh(kingSindex,board)==1)
        {
            end_game_window = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"CHECKMATE!\nWell done,WHITE is the winner");
            gtk_window_set_title(GTK_WINDOW(end_game_window), "GAME ENDED!");
            gtk_dialog_run(GTK_DIALOG(end_game_window));
            gtk_widget_destroy(window);
        }
        else if(flag==0&&kesh(kingSindex,board)!=1&&upOloCase(board[moveindex.a][moveindex.b])==0)
        {
            end_game_window = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"\nSTALEMATE!\nThe game ended as a draw!\nGood luck next time :)");
            gtk_window_set_title(GTK_WINDOW(end_game_window), "GAME ENDED!");
            gtk_dialog_run(GTK_DIALOG(end_game_window));
            gtk_widget_destroy(window);
        }
        else
        {
            draw_situations();
        }
    }
    else if(whichturn%2==0)
    {
        for(int k=0; k<8; k++)
        {
            for(int j=0; j<8; j++)
            {
                copyarr_withchange(board,temp,kingSindex.a,kingSindex.b,k,j);
                kings_temp=findTheIndex(temp);
                if(king(kingSindex.a,kingSindex.b,k,j,board)==1&&kesh(kings_temp,temp)!=2&&upOloCase(board[kingSindex.a][kingSindex.b])!=upOloCase(board[k][j])||validmoves('k',board))
                {
                    flag=1;
                }

            }
        }
        if (flag==0&&kesh(kingSindex,board)==2)
        {
            end_game_window = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"\nCHECKMATE!\nWell done,BLACK is the winner!");
            gtk_window_set_title(GTK_WINDOW(end_game_window), "GAME ENDED!");
            gtk_dialog_run(GTK_DIALOG(end_game_window));
            gtk_widget_destroy(window);
        }
        else if(flag==0&&kesh(kingSindex,board)!=2&&upOloCase(board[moveindex.a][moveindex.b])==1)
        {
            end_game_window = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"\nSTALEMATE!\nThe game ended as a draw!\nGood luck next time :)");
            gtk_window_set_title(GTK_WINDOW(end_game_window), "GAME ENDED!");
            gtk_dialog_run(GTK_DIALOG(end_game_window));
            gtk_widget_destroy(window);
        }
        else
        {
            draw_situations();
        }
    }


}
void close()
{
    gtk_widget_hide(promotionWindow);
}
void promotion_move(GtkButton *button, gpointer user_data)
{

    int q =GPOINTER_TO_UINT (user_data);
    gchar lower[4]= {'b','q','n','r'};
    gchar upper[4]= {'B','Q','N','R'};
    if((whichturn-1)%2==0)
    {
        board[moveindex.a][moveindex.b]=lower[q];
        set_image(moveindex.b,moveindex.a);
        history[whichturn-1].promotion=lower[q];
    }
    else if((whichturn-1)%2!=0)
    {
        board[moveindex.a][moveindex.b]=upper[q];
        set_image(moveindex.b,moveindex.a);
        history[whichturn-1].promotion=upper[q];
    }
    history[whichturn-1].is_promoted=1;
    disable_buttons(TRUE);
    checOrstale(board,whichturn,temp);
}
void disable_buttons(gboolean a)
{
    for (int i=0; i<8; i++)
    {
        for (int j=0; j<8; j++)
        {
            gtk_widget_set_sensitive(buttons[i][j],a);
        }
    }
}
void myCSS(void)
{
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    provider = gtk_css_provider_new ();
    display = gdk_display_get_default ();
    screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    const gchar *myCssFile = "styles.css";
    GError *error = 0;

    gtk_css_provider_load_from_file(provider, g_file_new_for_path(myCssFile), &error);
    g_object_unref (provider);
}
void save_game()
{
    char filename[30];
    FILE*newfile;
    int x,y;

    while(1)
    {
        gets(filename);
        newfile=fopen(filename,"r+");
        if(newfile!=NULL)
        {
            printf("exist");
        }
        else
        {
            newfile=fopen(filename,"w+");
            if(newfile==NULL)
            {
                printf("invalid");
            }
            else
                break;

        }
    }
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            fprintf(newfile,"%c",board[i][j]);
        }
    }

    fprintf(newfile,"\n%d\n",whichturn);
    for(int i=0; i<whichturn; i++)
    {
        fprintf(newfile,"%d\n",history[i].move.x);
        fprintf(newfile,"%d\n",history[i].move.y);
        fprintf(newfile,"%d\n",history[i].move.a);
        fprintf(newfile,"%d\n",history[i].move.b);
        fprintf(newfile,"%d\n",history[i].enpassat.x);
        fprintf(newfile,"%d\n",history[i].enpassat.y);
        fprintf(newfile,"%d\n",history[i].enpassat.a);
        fprintf(newfile,"%d\n",history[i].enpassat.b);
        fprintf(newfile,"%d\n",history[i].is_enpass);
        fprintf(newfile,"%d\n",history[i].is_removed);
        fprintf(newfile,"%c\n",history[i].removed);
        fprintf(newfile,"%d\n",history[i].is_promoted);
        fprintf(newfile,"%c\n",history[i].promotion);
        fprintf(newfile,"%d\n",history[i].is_castled);
        fprintf(newfile,"%d\n",history[i].castlemove.x);
        fprintf(newfile,"%d\n",history[i].castlemove.y);
        fprintf(newfile,"%d\n",history[i].castlemove.a);
        fprintf(newfile,"%d\n",history[i].castlemove.b);
        fprintf(newfile,"%d\n",history[i].castle_removed.x);
        fprintf(newfile,"%d\n",history[i].castle_removed.y);
        fprintf(newfile,"%d\n",history[i].castle_removed.a);
        fprintf(newfile,"%d\n",history[i].castle_removed.b);

        fprintf(newfile,"%d\n",can_redo);
        fprintf(newfile,"%d\n",will_redo);


    }
     fprintf(newfile,"%d\n",num_of_undoes);
    fclose(newfile);

}

void load(index moved)
{
    FILE*newfile;
    char filename[30];
    int k;
    char sboard[8][8];
    while(1)
    {
        gets(filename);
        newfile=fopen(filename,"r+");
        if(newfile==NULL)
        {
            printf("File does not exist\n");
        }
        else
        {
            break;
        }
    }
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {

            fscanf(newfile,"%c",&board[i][j]);
        }
    }
    fscanf(newfile,"\n%d\n",&whichturn);
    for(int i=0; i<whichturn; i++)
    {
        fscanf(newfile,"%d",&history[i].move.x);
        printf("%d\n",moveindex.x);
        fscanf(newfile,"%d",&history[i].move.y);
        fscanf(newfile,"%d",&history[i].move.a);
        fscanf(newfile,"%d",&history[i].move.b);
        fscanf(newfile,"%d",&history[i].enpassat.x);
        fscanf(newfile,"%d",&history[i].enpassat.y);
        fscanf(newfile,"%d",&history[i].enpassat.a);
        fscanf(newfile,"%d",&history[i].enpassat.b);
        fscanf(newfile,"%d",&history[i].is_enpass);
        fscanf(newfile,"\n%d",&history[i].is_removed);
        printf("%d\n",history[i].is_removed);
        fscanf(newfile,"\n%c",&history[i].removed);
        printf("%c\n",history[i].removed);
        fscanf(newfile,"\n%d",&history[i].is_promoted);
        fscanf(newfile,"\n%c",&history[i].promotion);
        fscanf(newfile,"\n%d",&history[i].is_castled);
        fscanf(newfile,"\n%d",&history[i].castlemove.x);
        fscanf(newfile,"\n%d",&history[i].castlemove.y);
        fscanf(newfile,"\n%d",&history[i].castlemove.a);
        fscanf(newfile,"\n%d",&history[i].castlemove.b);
        fscanf(newfile,"\n%d",&history[i].castle_removed.x);
        fscanf(newfile,"\n%d",&history[i].castle_removed.y);
        fscanf(newfile,"\n%d",&history[i].castle_removed.a);
        fscanf(newfile,"\n%d",&history[i].castle_removed.b);

        fscanf(newfile,"\n%d",&can_redo);
        fscanf(newfile,"\n%d",&will_redo);



    }
    fscanf(newfile,"\n%d",&num_of_undoes);
        printf("%d",num_of_undoes);
    fclose(newfile);
}
void printing_board(int argc, char *argv,char board[][8],GtkWidget *buttons[][8])
{

    if(whichgame==1)
    {
        load(moveindex);

    }
    GtkWidget  *promotionFixed;
    GtkWidget *redo,*save,*undo,*resign, *promotionButtons[4];
    gchar * label;
    GtkStyleContext *context1, *context2;
    myCSS();
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "CHESS");
    gtk_window_set_default_size(GTK_WINDOW(window), 650, 620);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    promotionWindow = gtk_window_new(GTK_WINDOW_POPUP);
    gtk_window_set_title(GTK_WINDOW(promotionWindow), "PROMOTION");
    gtk_window_set_default_size(GTK_WINDOW(promotionWindow), 400, 100);
    gtk_window_set_position(GTK_WINDOW(promotionWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_decorated(promotionWindow,FALSE);

    promotionFixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(promotionWindow), promotionFixed);

    promotionButtons[0] = gtk_button_new_with_label("Bishop");
    promotionButtons[1] = gtk_button_new_with_label("Queen");
    promotionButtons[2] = gtk_button_new_with_label("Knight");
    promotionButtons[3] = gtk_button_new_with_label("Rook");
    for(int i=0; i<4; i++)
    {
        gtk_fixed_put(GTK_FIXED(promotionFixed), promotionButtons[i], 100*i, 0);
        gtk_widget_set_size_request( promotionButtons[i], 100, 100);
        gpointer ptr = GINT_TO_POINTER (i);
        g_signal_connect(G_OBJECT(promotionButtons[i]), "clicked", G_CALLBACK(promotion_move), ptr);
        g_signal_connect(G_OBJECT(promotionButtons[i]), "clicked", G_CALLBACK(close), NULL);
    }

    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);
    redo = gtk_button_new_with_label("Redo");
    gtk_fixed_put(GTK_FIXED(fixed),  redo, 550, 175);
    undo = gtk_button_new_with_label("Undo");
    gtk_fixed_put(GTK_FIXED(fixed),  undo, 550, 250);
    save = gtk_button_new_with_label("Save \nGame");
    gtk_fixed_put(GTK_FIXED(fixed),  save, 550, 100);
    resign = gtk_button_new_with_label("resign");
    gtk_fixed_put(GTK_FIXED(fixed),  resign, 550, 325);
    GtkLabel *labelBoard[8][8];

    for (int i=0; i<8; i++)
    {
        for (int j=0; j<8; j++)
        {
            buttons[i][j]= gtk_button_new();
            set_image(i,j);
            gtk_fixed_put(GTK_FIXED(fixed),  buttons[i][j], 58*i+60, 58*j+60);
            gtk_widget_set_size_request( buttons[i][j], 60, 60);
            if(j%2!=0&&i%2==0||j%2==0&&i%2!=0)
            {
                context1 = gtk_widget_get_style_context(buttons[i][j]);
                gtk_style_context_add_class(context1,"black");
            }
            else if(j%2==0&&i%2==0||j%2!=0&&i%2!=0)
            {
                context2 = gtk_widget_get_style_context(buttons[i][j]);
                gtk_style_context_add_class(context2,"white");
            }
            int  q = i*10+j;
            gpointer ptr = GINT_TO_POINTER (q);
            g_signal_connect(G_OBJECT(buttons[i][j]), "clicked", G_CALLBACK(clicked_button), ptr);
        }
    }

    char title[11];
    GtkLabel*labels;
    sprintf(title,"%s","`");
    for (int i = 1; i < 9; i++)
    {
        title[0]++;
        labels = (GtkLabel *) gtk_label_new(title);
        gtk_widget_set_size_request((GtkWidget *) labels, 0, 30);
        gtk_fixed_put(GTK_FIXED(fixed),  labels,30+ i*58, 520);
    }
    int i = 8;
    for (int j = 0; j < 8; j++)
    {
        sprintf(title,"%d",i--);
        label = (GtkLabel *) gtk_label_new(title);
        gtk_widget_set_size_request((GtkWidget *) label, 50, 0);
        gtk_fixed_put(GTK_FIXED(fixed),  label,5,70+j*60);
    }
    if(whichgame==1)
    {
        int ndo=0;
        char sboard[8][8]= {{' ',' ',' ',' ',' ',' ',' ',' '},
            {' ',' ',' ',' ',' ',' ',' ',' '},
            {' ',' ',' ',' ',' ',' ',' ',' '},
            {' ',' ',' ',' ',' ',' ',' ',' '},
            {' ',' ',' ',' ',' ',' ',' ',' '},
            {' ',' ',' ',' ',' ',' ',' ',' '},
            {' ',' ',' ',' ',' ',' ',' ',' '},
            {' ',' ',' ',' ',' ',' ',' ',' '},
        };
        int k;
        newfile=fopen(filename,"r+");
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {

                fscanf(newfile,"%c",&board[i][j]);
            }
        }
        fscanf(newfile,"%d",&whichturn);
        for(int i=0; i<whichturn; i++)
        {
            fscanf(newfile,"\n%d",&history[i].move.x);
            printf("%d",history[i].move.x);
            fscanf(newfile,"\n%d",&history[i].move.y);
            fscanf(newfile,"\n%d",&history[i].move.a);
            fscanf(newfile,"\n%d",&history[i].move.b);
            fscanf(newfile,"\n%d",history[i].is_removed);
            fscanf(newfile,"\n%c",&history[i].removed);
            if(history[i].is_removed==1)
            {
                sboard[moveindex.a][moveindex.b]=history[i].removed;
                table(moveindex.a,moveindex.b,ndo,sboard);
            }
        }
        con3(&i);
        fclose(newfile);
    }
    g_signal_connect(G_OBJECT(window), "destroy",G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(undo), "clicked",G_CALLBACK(undoFun), NULL);
    g_signal_connect(G_OBJECT(redo), "clicked",G_CALLBACK(redoFun), NULL);
    g_signal_connect(G_OBJECT(save), "clicked",G_CALLBACK(save_game), NULL);
    g_signal_connect(G_OBJECT(resign), "clicked",G_CALLBACK(resignFun), NULL);
    gtk_widget_show_all(window);
    gtk_main();
}

void check()
{
    con2(&whichgame);
    gtk_main_quit();
}
void intialize(int argc,char*argv)
{
    GtkWidget*init,*newgame,*savedgame,*initial_window,*fixed2,*background,*label2;
    initial_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(initial_window), "start");
    gtk_window_set_default_size(GTK_WINDOW(initial_window), 350, 350);
    gtk_window_set_position(GTK_WINDOW(initial_window), GTK_WIN_POS_CENTER);
    fixed2=gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(initial_window), fixed2);
    newgame=gtk_button_new_with_label("new game");
    savedgame=gtk_button_new_with_label("saved game");
    gtk_fixed_put(GTK_FIXED(fixed2),  newgame, 125, 120);
    gtk_fixed_put(GTK_FIXED(fixed2),  savedgame, 125, 170);
    g_signal_connect(G_OBJECT(initial_window), "destroy",G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(newgame), "clicked",G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(savedgame), "clicked",G_CALLBACK(check), NULL);
    gtk_widget_show_all(initial_window);
    gtk_main();
}

void table(int i, int j,int redo,char board[][8])
{
    GtkSettings *default_settings = gtk_settings_get_default();
    if(board[i][j]=='p')
    {
        if(redo==0)
        {
            images[q]= gtk_image_new_from_file ("whitepawn.jpg");
            gtk_fixed_put(GTK_FIXED(fixed), images[q],pos,5);
            con2(&q);
            con(&pos);
        }
        else if(redo==1)
        {
            gtk_widget_destroy(images[q-1]);
            con3(&q);
            recon(&pos);
        }
    }
    else if(board[i][j]=='b')
    {
        if(redo==0)
        {
            images[q] = gtk_image_new_from_file ("whitebishop.jpg");
            gtk_fixed_put(GTK_FIXED(fixed), images[q],pos,5);
            con2(&q);
            con(&pos);
        }
        else if(redo==1)
        {
            gtk_widget_destroy(images[q-1]);
            con3(&q);
            recon(&pos);
        }
    }
    else if(board[i][j]=='n')
    {
        if(redo==0)
        {
            images[q] = gtk_image_new_from_file ("whiteknight.jpg");
            gtk_fixed_put(GTK_FIXED(fixed), images[q],pos,5);
            con2(&q);
            con(&pos);
        }
        else if(redo==1)
        {
            gtk_widget_destroy(images[q-1]);
            con3(&q);
            recon(&pos);
        }

    }
    else if(board[i][j]=='r')
    {
        if(redo==0)
        {
            images[q] = gtk_image_new_from_file ("whiterook.jpg");
            gtk_fixed_put(GTK_FIXED(fixed), images[q],pos,5);
            con2(&q);
            con(&pos);
        }
        else if(redo==1)
        {
            gtk_widget_destroy(images[q-1]);
            con3(&q);
            recon(&pos);
        }

    }
    else if(board[i][j]=='q')
    {
        if(redo==0)
        {
            images[q] = gtk_image_new_from_file ("whitequeen.jpg");
            gtk_fixed_put(GTK_FIXED(fixed), images[q],pos,5);
            con2(&q);
            con(&pos);
        }
        else if(redo==1)
        {
            gtk_widget_destroy(images[q-1]);
            con3(&q);
            recon(&pos);
        }

    }
    else if(board[i][j]=='k')
    {
        if(redo==0)
        {
            images[q] = gtk_image_new_from_file ("whiteking.jpg");
            gtk_fixed_put(GTK_FIXED(fixed), images[q],pos,5);
            con2(&q);
            con(&pos);
        }
        else if(redo==1)
        {
            gtk_widget_destroy(images[q-1]);
            con3(&q);
            recon(&pos);
        }

    }
    else if(board[i][j]=='P')
    {
        if(redo==0)
        {
            images[q] = gtk_image_new_from_file ("c.jpg");
            gtk_fixed_put(GTK_FIXED(fixed),images[q],pos2,550);
            con2(&q);
            con(&pos2);
        }
        else if(redo==1)
        {
            gtk_widget_destroy(images[q-1]);
            con3(&q);
            recon(&pos2);
        }
    }
    else if(board[i][j]=='Q')
    {
        if(redo==0)
        {
            images[q] = gtk_image_new_from_file ("queen-removebg-preview_8.jpg");
            gtk_fixed_put(GTK_FIXED(fixed), images[q],pos2,562);
            con2(&q);
            con(&pos2);
        }
        else if(redo==1)
        {
            gtk_widget_destroy(images[q-1]);
            con3(&q);
            recon(&pos2);
        }
    }
    else if(board[i][j]=='N')
    {
        if(redo==0)
        {
            images[q] = gtk_image_new_from_file ("black_knight-removebg-preview_6.jpg");
            gtk_fixed_put(GTK_FIXED(fixed), images[q],pos2,570);
            con2(&q);
            con(&pos2);
        }
        else if(redo==1)
        {
            gtk_widget_destroy(images[q-1]);
            con3(&q);
            recon(&pos2);
        }

    }
    else if(board[i][j]=='B')
    {
        if(redo==0)
        {
            images[q]= gtk_image_new_from_file ("bishob_black.jpg");
            gtk_fixed_put(GTK_FIXED(fixed), images[q],pos2,550);
            con2(&q);
            con(&pos2);
        }
        else if(redo==1)
        {
            gtk_widget_destroy(images[q-1]);
            con3(&q);
            recon(&pos2);
        }
    }
    else if(board[i][j]=='K')
    {
        if(redo==0)
        {
            images[q] = gtk_image_new_from_file ("blackking.jpg");
            gtk_fixed_put(GTK_FIXED(fixed), images[q],pos2,567);
            con2(&q);
            con(&pos2);
        }
        else if(redo==1)
        {
            gtk_widget_destroy(images[q-1]);
            con3(&q);
            recon(&pos2);
        }
    }
    else if(board[i][j]=='R')
    {
        if(redo==0)
        {
            images[q] = gtk_image_new_from_file ("blackkkrok.jpg");
            gtk_fixed_put(GTK_FIXED(fixed), images[q],pos2,567);
            con2(&q);
            con(&pos2);
        }
        else if(redo==1)
        {
            gtk_widget_destroy(images[q-1]);
            con3(&q);
            recon(&pos2);
        }

    }
    gtk_widget_show_all(window);
}

int main(int argc, char *argv)
{
    boarddecl(board);
    gtk_init(&argc, &argv);
    intialize(&argc,&argv);
    printing_board(&argc,&argv,board,buttons);
    return 0;
}

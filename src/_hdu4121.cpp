#include<cstdio>
#include<cstring>
#include<iostream>
#include<cmath>
#include<algorithm>
 using namespace std;
 char map[11][11];
 const int dir[4][2] = {0,1,1,0,-1,0,0,-1};
 int note[10][2];
 int cnt;
 int b_x, b_y;
 
 bool law_G(int x, int y)
 {
     return x<=3 && x>=1 && y<=6 && y>=4;    
 }
 
 bool deal_G(int x, int y, int n_x, int n_y)
 {
     if (n_y != y)return false;
     while (--x >= 1){
         if (x == n_x)return true;
         if (map[x][y] == '.'){
             continue;
         } else{
             return false;        
         }    
     }
     return false;
 }
 
 bool check_right(int x, int y, int n_y)
 {
     for (int i(y+1); i<=9; ++i){
         if (i == n_y)return true;
         if (map[x][i] == '.'){
             continue;    
         }    else{
             return false;    
         }
     }
     return false;    
 }
 
 bool check_left(int x,int y, int n_y)
 {
     for (int i(y-1); i>=1; --i){
         if (i == n_y)return true;
         if (map[x][i] == '.'){
             continue;    
         }    else{
             return false;    
         }
     }
     return false;    
 }
 
 bool check_up(int x, int y,int n_x)
 {
     for (int i(x-1); i>=1; --i){
         if (i == n_x)return true;
         if (map[i][y] == '.'){
             continue;    
         }    else{
             return false;    
         }
     }
     return false;    
 }
 
 bool check_down(int x, int y, int n_x)
 {
     for (int i(x+1); i<=10; ++i){
         if (i == n_x)return true;
         if (map[i][y] == '.'){
       continue;    
         } else{
           return false;    
         }
     }
     return false;    
 }
 
 bool deal_R(int x, int y, int n_x, int n_y)
 {
     if (x != n_x && y != n_y)return false;
     if (x == n_x){
         if (y < n_y){
             if (check_right(x,y,n_y))return true;
             return false;    
         } else{
             if (check_left(x,y,n_y))return true;
             return false;    
         }    
     } else{
         if (x < n_x){
             if (check_down(x,y,n_x))return true;
             return false;    
         }    else{
             if (check_up(x,y,n_x))return true;
             return false;    
         }
     }
 }
 
 bool deal_C(int x, int y, int n_x, int n_y)
 {
     bool flag = false;
     if (x != n_x && y != n_y)return false;
     if (x == n_x){
         if (y < n_y){
             while (++y < n_y){
                 if (map[x][y] == '.'){
                     continue;
                 } else{
                     flag = true;
                     break;    
                 }    
             }
             if (flag){
                 if (check_right(x,y,n_y))return true;
             }
         } else{
             while (--y > n_y){
                 if (map[x][y] == '.'){
                     continue;    
                 }    else{
                     flag = true;
                     break;    
                 }
             }
             if (flag){
                 if (check_left(x,y,n_y))return true;    
             }
         }    
     }    else{
         if (x < n_x){
             while (++x < n_x){
                 if (map[x][y] == '.'){
                     continue;    
                 }    else{
                     flag = true;
                     break;    
                 }
             }
             if (flag){
                 if (check_down(x,y,n_x))return true;    
             }    
         } else{
             while (--x > n_x){
                 if (map[x][y] == '.'){
                     continue;    
                 }    else{
                     flag = true;
                     break;    
                 }
             }
             if (flag){
                 if (check_up(x,y,n_x))return true;    
             }    
         }    
     }
     return false;
 }
 
 bool deal_H(int x, int y, int n_x, int n_y){
     if (x+2 == n_x && (y-1 == n_y || y+1 == n_y)){
         if (map[x+1][y] == '.')return true;
         return false;
     }    else{
         if (x-2 == n_x && (y-1 == n_y || y+1 == n_y)){
             if (map[x-1][y] == '.')return true;
             return false;        
         }    else{
             if (y+2 == n_y && (x-1 == n_x || x+1 == n_x)){
                 if (map[x][y+1] == '.')return true;
                 return false;    
             }    else{
                 if (y-2 == n_y && (x-1 == n_x || x+1 == n_x)){
                     if (map[x][y-1] == '.')return true;
                     return false;    
                 }    
             }
         }
     }
     return false;
 }
 
 bool dfs(int x, int y)
 {
     for (int i(0); i<4; ++i){
         bool flag = false;
         int now_x = x + dir[i][0];
         int now_y = y + dir[i][1];
         if (law_G(now_x,now_y)){
             for (int j(0); j<cnt; ++j){
                 switch (map[note[j][0]][note[j][1]]){
                     case 'G':{
                         if (deal_G(note[j][0],note[j][1],now_x,now_y))flag = true;
                         break;    
                     }
                     case 'R':{
                         if (deal_R(note[j][0],note[j][1],now_x,now_y))flag = true;
                         break;
                     }
                     case 'C':{
                         if (deal_C(note[j][0],note[j][1],now_x,now_y))flag = true;
                         break;    
                     }
                     case 'H':{
                         if (deal_H(note[j][0],note[j][1],now_x,now_y))flag = true;
                         break;    
                     }    
                 }
                 if (flag)break;    
             }
             if (!flag)return false;    
         }
     }
     return true;    
 }
 
 int main()
 {
     int red_num;
     while (cin>>red_num>>b_x>>b_y){
         if (red_num == 0 && b_x == 0 && b_y == 0)break;
         char temp;
         memset(map,'.',sizeof(map));
         cnt = 0;
         int x, y;
         while(red_num--){
             cin>>temp>>x>>y;
             switch(temp){
                 case 'G':{
                     map[x][y] = 'G';
                     break;    
                 }
                 case 'C':{
                     map[x][y] = 'C';
                     break;    
                 }
                 case 'H':{
                     map[x][y] = 'H';
                     break;    
                 }
                 case 'R':{
                     map[x][y] = 'R';
                     break;    
                 }    
             }
             note[cnt][0] = x;
             note[cnt++][1] = y;    
         }
         if (dfs(b_x,b_y)){
             cout<<"YES"<<endl;    
         } else{
             cout<<"NO"<<endl;    
         }
     }
     return 0;    
 }


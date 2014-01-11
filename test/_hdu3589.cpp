#include<cstdio>
#include<cstring>
#include<iostream>
#include<cmath>
#include<algorithm>
int MIN(int a,int b) { if( a<b )  return a;  else return b; }
int MAX(int a,int b) { if( a>b )  return a;  else return b; }
#define CLR(NAME,VALUE) memset(NAME,VALUE,sizeof(NAME))
using namespace std;
typedef long long LL;
const int MAXPRI=1000000+10;
bool isNotPri[MAXPRI];
int  priList[MAXPRI],priNum;
void CalPri() {  //线性筛法
 int i,j;
 memset(isNotPri,0,sizeof(isNotPri));
 isNotPri[0]=isNotPri[1]=1; 
 for(i=4;i<MAXPRI;i+=2) {
  isNotPri[i]=1;
 }
 priNum=0;
 for(i=3;i<MAXPRI;i+=2) {
  if( !isNotPri[i] ) {
    priList[priNum++]=i;
  }
  for(j=0;j<priNum&&i*priList[j]<MAXPRI;++j) {
   isNotPri[i*priList[j]]=1;
   if( i%priList[j]==0 ) {
    break;
   }
  }
 }
}

LL MyPow(LL a,LL p,LL m) {
 if( p==0 )  return 1;
 if( p==1 )  return a%m;
 LL tmp=MyPow(a,p/2,m);
 if( p&1 ) return ((tmp*tmp)%m)*a%m;
 else      return (tmp*tmp)%m;
}
int Cal(int a,int n) {
 if( a%n==0%n )  return 0;
 LL tmp=MyPow(a,(n-1)/2,n);
 if( tmp==1%n )   return 1;
 else             return -1; 
}
   
int main() {
  int a,n,cnt,ans,i,tmp;
  CalPri();
  while( scanf("%d%d",&a,&n)!=EOF ) {
   ans=1;
   if( isNotPri[n] ) {
    i=0;
    while( n!=1 ) {
     cnt=0;
     while( n%priList[i]==0 && n!=1 ) {
      n/=priList[i];
      ++cnt;
     }
     ++i;
     if( cnt!=0 ) {
      tmp=Cal(a,priList[i-1]);
      ans*=pow((double)tmp,cnt);
     }
    }
   }
   else {
    ans=Cal(a,n);
   }
   printf("%d\n",ans);
  }
     
     return 0;
}

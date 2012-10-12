#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;

const int MAXN = 20000;
const int INF = 1000000000;

struct Edge
{
    int v,cap;
    Edge *next,*pair;
}edge[MAXN*22*2+10];

struct Graph
{
    Edge *G[MAXN+10],*cur[MAXN+10],*pE;
    int dist[MAXN+10],num[MAXN+10];
    int n,s,t;
    void init(int nn,int ss,int tt)
    {
        n=nn;
        s=ss,t=tt;
        memset(G,0,sizeof(G));
        pE=edge;
    }
    void add(int u,int v,int cap,Edge *pair)
    {
        pE->v=v;
        pE->cap=cap;
        pE->next=G[u];
        pE->pair=pair;
        G[u]=pE++;
    }
    void add(int u,int v,int cap)
    {
        add(u,v,cap,pE+1);
        add(v,u,0,pE-1);
    }
    int aug(int u,int preCap)
    {
        if(u==t)return preCap;
        int leftCap=preCap;
        for(Edge *&it=cur[u];it;it=it->next)
        {
            if(it->cap&&dist[it->v]+1==dist[u])
            {
                int d=aug(it->v,min(leftCap,it->cap));
                it->cap-=d;
                it->pair->cap+=d;
                leftCap-=d;
                if(leftCap==0||dist[s]==n)return preCap-leftCap;
            }
        }

        int minDist=n;
        for(Edge *it=cur[u]=G[u];it;it=it->next)
            if(it->cap)minDist=min(minDist,dist[it->v]+1);//+1
        if(--num[dist[u]]==0)dist[s]=n;
        else num[dist[u]=minDist]++;
        return preCap-leftCap;
    }
    int maxFlow()
    {
        memset(dist,0,sizeof(dist));
        memset(num,0,sizeof(num));
        memmove(cur,G,sizeof(G));
        num[0]=n;
        int flow=0;
        while(dist[s]<n)flow+=aug(s,INF);
        return flow;
    }
}graph;
int main()
{    
    int n,m,a,b,c;
    while(~scanf("%d%d",&n,&m))
    {
        graph.init(n+2,0,n+1);
        for(int i=1;i<=n;i++)
        {
            scanf("%d%d",&a,&b);
            graph.add(0,i,a);
            graph.add(i,n+1,b);
        }
        for(int i=0;i<m;i++)
        {
            scanf("%d%d%d",&a,&b,&c);
            graph.add(a,b,c);
            graph.add(b,a,c);
        }
        printf("%d\n",graph.maxFlow());
    }
    return 0;
}


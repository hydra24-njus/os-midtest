#include "thread.h"
#include "thread-sync.h"
#ifdef TEST
long f(int x){return x;}
#else
long f(int x);
#endif

long sum = 0;
spinlock_t readlock=0,fxlock=0;
#ifdef TEST
FILE* fp=NULL;
int worker(){
  while (1) {
    spin_lock(&readlock);
    int x=0;
    char last=' ',ch=getc(fp);
    if(ch==EOF){spin_unlock(&readlock);break;}
    if(ch=='\n'){spin_unlock(&readlock);continue;}
    while(ch<'0' || ch>'9')last=ch,ch=getc(fp);
    while(ch>='0' && ch<='9')x=x*10+ch-'0',ch=getc(fp);
    if(last=='-')x=-x;
    spin_unlock(&readlock);
    long y=f(x);
    spin_lock(&fxlock);
    sum += y; 
    spin_unlock(&fxlock);
  }
}
#else
int worker(){
  while (1) {
    spin_lock(&readlock);
    int x=0;
    char last=' ',ch=getchar();
    if(ch==EOF){spin_unlock(&readlock);break;}
    if(ch=='\n'){spin_unlock(&readlock);continue;}
    while(ch<'0' || ch>'9')last=ch,ch=getchar();
    while(ch>='0' && ch<='9')x=x*10+ch-'0',ch=getchar();
    if(last=='-')x=-x;
    spin_unlock(&readlock);
    long y=f(x);
    spin_lock(&fxlock);
    sum += y;
    spin_unlock(&fxlock);
  }
}
#endif
int main() {
  #ifdef TEST
  fp=fopen("/home/hydra24/mid-test/test.txt","r");
  #endif
  create(worker);create(worker);create(worker);create(worker);
  join();
  printf("%ld\n", sum);
}

/* 
T1.1 T2.1 T1.2 T1.3 T1.4 T1.5 T1.6
T2.2 T2.3 T1.7
T2.4 T2.5 T2.6 T2.7 T2.8 T2.9
T1.8 T1.9
sum = 2 .
*/

#include<stdio.h>
#define k n[++i]
#define B (printf("B")&&(!k?1:i>=l?0:n[i]==99&&k==99?S()&&k==98&&k==99:(i--)))
unsigned char n[201],i=-1,l=0;int A(){return printf("A")&&++i<l&&n[i]==97?1:n[i]==98?(printf("C")&&A()&&A()):0;}int S(){return printf("S")&&++i<l&&n[i]==97?A()&&B:n[i]==98?B&&A():0;}int main(){scanf("%[^\n]",n);for(;n[l];l++);printf("\n%s",S()&&i==l-1?"DA":"NE");return 0;}
#include <stdio.h>

unsigned char in[201];
int i = -1, l, S(), A(), B(), C();

int S()
{
    return printf("S") && ++i < l && (in[i] == 'a' ? A() && B() : in[i] == 'b' ? B() && A() : 0);
}

int A()
{
    return printf("A") && ++i < l && (in[i] == 'a' ? 1 : in[i] == 'b' ? C() : 0);
}

int B()
{
    return printf("B") && (!in[++i] ? 1 : i >= l ? 0 : (in[i] == 'c' && in[i+1] == 'c') ? (300 + i++) && S() && (300 + (i+=2)) && in[i-1] == 'b' && in[i] == 'c' : (300 + --i > 0));
}

int C()
{
    return printf("C") && A() && A();
}

int main(void)
{
    scanf("%200[^\n]", in);
    for(l = 0; in[l]; l++) if(in[l] == '\n') in[l--] = 0;
    printf("\n%s", S() && (i == l || i == l-1) ? "DA" : "NE");
    return 0;
}
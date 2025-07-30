#include <iostream>
#include <vector>
#include <stack>

std::string get_kword(std::string in, char ch)
{
    size_t i;
    for(i = 0; in[i] != ch && i < in.size(); i++);
    return in.substr(0, i);
}

#define TAB(indent) for(unsigned TAB_VAR = 0; TAB_VAR < indent; TAB_VAR++) output += ' ';
    
#define PROGRAM               "<program>"
#define LISTA_NAREDBI         "<lista_naredbi>"
#define NAREDBA               "<naredba>"
#define NAREDBA_PRIDRUZIVANJA "<naredba_pridruzivanja>"
#define ZA_PETLJA             "<za_petlja>"
#define E                     "<E>"
#define E_LISTA               "<E_lista>"
#define T                     "<T>"
#define T_LISTA               "<T_lista>"
#define P                     "<P>"

#define IDN         "IDN"
#define BROJ        "BROJ"
#define KR_ZA       "KR_ZA"
#define KR_AZ       "KR_AZ"
#define KR_OD       "KR_OD"
#define KR_DO       "KR_DO"
#define OP_PRIDRUZI "OP_PRIDRUZI"
#define OP_PLUS     "OP_PLUS"
#define OP_MINUS    "OP_MINUS"
#define OP_PUTA     "OP_PUTA"
#define OP_DIJELI   "OP_DIJELI"
#define L_ZAGRADA   "L_ZAGRADA"
#define D_ZAGRADA   "D_ZAGRADA"

#define EPS "$"

#define F_01 swap({LISTA_NAREDBI});
#define F_02 swap({LISTA_NAREDBI, NAREDBA});
#define F_03 stay();
#define F_04 swap({NAREDBA_PRIDRUZIVANJA});
#define F_05 swap({ZA_PETLJA});
#define F_06 swap({E, OP_PRIDRUZI}); next();
#define F_07 swap({KR_AZ, LISTA_NAREDBI, E, KR_DO, E, KR_OD, IDN}); next();
#define F_08 swap({E_LISTA, T});
#define F_09 swap({E}); next();
#define F_10 swap({T_LISTA, P});
#define F_11 swap({T}); next();
#define F_12 swap({P}); next();
#define F_13 swap({D_ZAGRADA, E}); next();
#define F_14 unsigned ind = idts.top(); stack.pop(); idts.pop(); next(ind + 1); 

#define ERR  std::cout << "err " << (input[idx] == EPS ? "kraj" : input[idx]); exit(0);

unsigned idx = 0;
std::string temp, output = "";
std::vector<std::string> input, kword;
std::stack<std::string> stack;
std::stack<unsigned> idts;

void swap(const std::vector<std::string> _s)
{
    unsigned ind = idts.top();
    stack.pop();
    idts.pop();
    for(std::string s : _s)
    {
        stack.push(s);
        idts.push(ind + 1);
    }
}

inline void next()
{
    TAB(idts.top()) output += input[idx++] + "\n";
}

inline void next(const unsigned i)
{
    TAB(i) output += input[idx++] + "\n";
}

inline void stay()
{
    TAB(idts.top() + 1) output += "$\n";
    stack.pop();
    idts.pop();
}

inline int is_kw(const std::string &in)
{
    return in == IDN || in == BROJ || in == KR_ZA || in == KR_AZ || in == KR_OD || in == KR_DO || in == OP_PRIDRUZI || in == OP_PLUS || in == OP_MINUS || in == OP_PUTA || in == OP_DIJELI || in == L_ZAGRADA || in == D_ZAGRADA;
}

inline int is_st(const std::string &in)
{
    return in == PROGRAM || in == LISTA_NAREDBI || in == NAREDBA || in == NAREDBA_PRIDRUZIVANJA || in == ZA_PETLJA || in == E || in == E_LISTA || in == T || in == T_LISTA || in == P;
}

int main()
{
    while(std::getline(std::cin, temp))
    {
        input.push_back(temp);
        kword.push_back(get_kword(temp, ' '));
    }
    input.push_back(EPS);
    kword.push_back(EPS);

    stack.push(EPS);
    stack.push(PROGRAM);
    idts.push(0);

    std::string st, in;

    while(1)
    {
        if(stack.top() == EPS && kword[idx] == EPS)
            break;

        st = stack.top();
        
        if(is_kw(st))
        {
            if(st == kword[idx])
            {
                next();
                stack.pop(); 
                idts.pop();
                continue;
            }
            else
            {
                ERR;
            }
        }

        if(is_st(st))
        {
            TAB(idts.top()) output += st + "\n";
        }
        
        if(st == PROGRAM)
        {
            if(kword[idx] == IDN || kword[idx] == KR_ZA || kword[idx] == EPS)
            {
                F_01;
            }
            else
            {
                ERR;
            }
        }
        else if(st == LISTA_NAREDBI)
        {
            if(kword[idx] == IDN || kword[idx] == KR_ZA)
            {
                F_02;
            }
            else if(kword[idx] == KR_AZ || kword[idx] == EPS)
            {
                F_03;
            }
            else
            {
                ERR;
            }
        }
        else if(st == NAREDBA)
        {
            if(kword[idx] == IDN)
            {
                F_04;
            }
            else if(kword[idx] == KR_ZA)
            {
                F_05;
            }
            else
            {
                ERR;
            }
        }
        else if(st == NAREDBA_PRIDRUZIVANJA)
        {
            if(kword[idx] == IDN)
            {
                F_06;
            }
            else
            {
                ERR;
            }
        }
        else if(st == ZA_PETLJA)
        {
            if(kword[idx] == KR_ZA)
            {
                F_07;
            }
            else
            {
                ERR;
            }
        }
        else if(st == E)
        {
            if(kword[idx] == IDN || kword[idx] == BROJ || kword[idx] == OP_PLUS || kword[idx] == OP_MINUS || kword[idx] == L_ZAGRADA)
            {
                F_08;
            }
            else
            {
                ERR;
            }
        }
        else if(st == E_LISTA)
        {
            if(kword[idx] == IDN || kword[idx] == D_ZAGRADA || kword[idx] == KR_ZA || kword[idx] == KR_DO || kword[idx] == KR_AZ || kword[idx] == EPS)
            {
                F_03;
            }
            else if(kword[idx] == OP_PLUS || kword[idx] == OP_MINUS)
            {
                F_09;
            }
            else
            {
                ERR;
            }
        }
        else if(st == T)
        {
            if(kword[idx] == IDN || kword[idx] == BROJ || kword[idx] == OP_PLUS || kword[idx] == OP_MINUS || kword[idx] == L_ZAGRADA)
            {
                F_10;
            }
            else
            {
                ERR;
            }
        }
        else if(st == T_LISTA)
        {
            if(kword[idx] == IDN || kword[idx] == D_ZAGRADA || kword[idx] == KR_ZA || kword[idx] == KR_DO || kword[idx] == KR_AZ || kword[idx] == OP_PLUS || kword[idx] == OP_MINUS || kword[idx] == EPS)
            {
                F_03;
            }
            else if(kword[idx] == OP_PUTA || kword[idx] == OP_DIJELI)
            {
                F_11;
            }
            else
            {
                ERR;
            }
        }
        else if(st == P)
        {
            if(kword[idx] == IDN || kword[idx] == BROJ)
            {
                F_14;
            }
            else if(kword[idx] == OP_PLUS || kword[idx] == OP_MINUS)
            {
                F_12;
            }
            else if(kword[idx] == L_ZAGRADA)
            {
                F_13;
            }
            else
            {
                ERR;
            }
        }
    }

    std::cout << output << "\n";
}
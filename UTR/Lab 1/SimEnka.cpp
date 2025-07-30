#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <sstream>

std::vector<std::vector<std::string>> *f_matrix_g;

const std::string _$ = "$";
const std::string _h = "#";

void splitChar(std::string in, char ch, std::vector<std::string> &res)
{
    std::stringstream ss(in);
    std::string temp;

    while(std::getline(ss, temp, ch))
        res.push_back(temp);
}

void parsePair(std::string in, std::vector<std::vector<std::string>> &funct)
{
    int comma, arrow;
    for(comma = 0; in[comma] != ','; comma++);
    for(arrow = 0; in[arrow] != '>'; arrow++);

    funct.push_back({in.substr(0, comma), in.substr(comma + 1, arrow - comma - 2), in.substr(arrow + 1, in.size() - arrow - 1)}); //current state, read, new state vector
}

std::vector<std::string> f(std::string state, std::string in)
{
    std::vector<std::string> ret;
    for(auto x : *f_matrix_g)
        if(x[0] == state && x[1] == in)
            splitChar(x[2], ',', ret);
    
    if(in == _$)
    {
        auto it = std::remove(ret.begin(), ret.end(), state);
        ret.erase(it, ret.end());
    }

    if(ret.empty())
        ret.push_back(_h);

    return ret;
}

std::vector<std::string> epsilonRec(std::vector<std::string> &stateV)
{
    std::vector<std::string> epsV;

    for(std::string s : stateV)
    {
        std::vector<std::string> t = f(s, _$);
        for(auto x : t)
            if(x != _h && std::find(stateV.begin(), stateV.end(), x) == stateV.end())
                epsV.push_back(x);
    }

    if(epsV.empty())
        return {};

    for(auto x : stateV)
        epsV.push_back(x);

    std::vector<std::string> e2 = epsilonRec(epsV);
    if(!e2.empty())
        for(auto x : e2)
            epsV.push_back(x);

    return epsV;
}

std::vector<std::string> epsilon(std::vector<std::string> stateV)
{
    return epsilonRec(stateV);
}

void cleanTemp(std::vector<std::string> &temp)
{
    int clean = 0;
    for(auto x : temp)
        if(x != _h)
        {
            clean = 1;
            break;
        }
    
    if(clean)
    {
        auto it = std::remove(temp.begin(), temp.end(), _h);
        temp.erase(it, temp.end());
    }
}

int main()
{
    std::string inputs, states, alpha, acc_states, start;
    std::vector<std::string> function;
    std::cin >> inputs >> states >> alpha >> acc_states >> start;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while(1)
    {
        std::string temp;
        std::getline(std::cin, temp);
        if(temp.empty())
            break;
        if(temp.find('#') == std::string::npos)
            function.push_back(temp);
    }

    std::vector<std::string> tests;
    splitChar(inputs, '|', tests);

    std::vector<std::string> statesV;
    splitChar(states, ',', statesV);

    std::vector<std::string> alphaV;
    splitChar(alpha, ',', alphaV);

    std::vector<std::vector<std::string>> f_matrix;
    f_matrix_g = &f_matrix;
    for(std::string str : function)
        parsePair(str, f_matrix);

    std::vector<std::string> curr, temp;

    for(size_t i = 0; i < tests.size(); i++)
    {
        curr.push_back(start);
        auto t1 = epsilon(curr);
        for(auto x : t1)
            curr.push_back(x);

        std::sort(curr.begin(), curr.end());
        curr.erase(std::unique(curr.begin(), curr.end()), curr.end());
        cleanTemp(curr);

        for(size_t i = 0; i < curr.size(); i++)
        {
            std::cout << curr[i];
            if(i != curr.size() - 1)
                std::cout << ",";  
        }

        std::vector<std::string> test_i;
        splitChar(tests[i], ',', test_i);

        for(size_t j = 0; j < test_i.size(); j++)
        {
            for(auto x : curr)
                for(auto y : f(x, test_i[j]))    
                    temp.push_back(y);
            for(auto x : epsilon(temp))    
                temp.push_back(x);
            
            std::cout << "|";

            std::sort(temp.begin(), temp.end());
            temp.erase(std::unique(temp.begin(), temp.end()), temp.end());
            cleanTemp(temp);

            curr = temp;
            for(size_t i = 0; i < curr.size(); i++)
            {
                std::cout << curr[i];
                if(i != curr.size() - 1)
                    std::cout << ",";  
            }
            temp.clear();
        }
        printf("\n");
        curr.clear();
    }
}


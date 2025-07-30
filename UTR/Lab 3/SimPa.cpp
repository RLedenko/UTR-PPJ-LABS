#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

typedef struct Input
{
    std::string state;
    std::string input;
    std::string stack_c;
} Input;

typedef struct Output
{
    std::string state;
    std::string stack;

    inline void operator = (Output &other)
    {
        this->state = other.state;
        this->stack = other.stack;
    }
} Output;

std::vector<std::pair<Input, Output>> f_matrix_g;
std::vector<std::string> statesV, alphaV, stackV, acceptV, inputsV, function, stack;

void split_char(std::string in, char ch, std::vector<std::string> &res)
{
    std::vector<int> idx;
    size_t s = in.size(), i;
    for(i = 0; i < s; i++)
        if(in[i] == ch)
            idx.push_back(i);

    if(idx.empty())
    {
        res.push_back(in);
        return;
    }

    res.push_back(in.substr(0, idx[0]));

    for(i = 0; i < idx.size() - 1; i++)
        res.push_back(in.substr(idx[i] + 1, idx[i + 1] - idx[i] - 1));
        
    res.push_back(in.substr(idx[i] + 1, s - idx[i]));
}

void parse_pair(std::string in, std::vector<std::pair<Input, Output>> &funct)
{
    unsigned arrow;
    for(arrow = 0; in[arrow] != '>'; arrow++);
    
    std::string a = in.substr(0, arrow - 1), b = in.substr(arrow + 1, in.size() - arrow - 1);

    std::vector<std::string> av, bv;
    split_char(a, ',', av);
    split_char(b, ',', bv);

    Input ip = {av[0], av[1], av[2]};
    Output op = {bv[0], bv[1]};

    funct.push_back({ip, op});
}

int get_next(std::string st, std::string in, std::string sts, Output &res)
{
    for(auto x : f_matrix_g) 
        if(x.first.state == st && x.first.input == in && x.first.stack_c == sts)
        {
            res = x.second;
            return 0;
        }
    return 1;
}

std::string stack_string()
{
    std::string ret;
    for(auto x : stack) ret += x;
    std::reverse(ret.begin(), ret.end());
    if(ret == "")
        return "$";
    return ret;
}

int main()
{
    std::string inputs, states, alpha, stack_states, acc_states, start, stack_init;

    std::getline(std::cin, inputs);
    std::getline(std::cin, states);
    std::getline(std::cin, alpha);
    std::getline(std::cin, stack_states);
    std::getline(std::cin, acc_states);
    std::getline(std::cin, start);
    std::getline(std::cin, stack_init);
    
    while(1)
    {
        std::string temp;
        std::getline(std::cin, temp);
        if(temp.empty())
            break;
        function.push_back(temp);
    }

    for(auto x : function)
        parse_pair(x, f_matrix_g);

    split_char(inputs, '|', inputsV);    
    split_char(states, ',', statesV);
    split_char(alpha, ',', alphaV);
    split_char(stack_states, ',', stackV);
    split_char(acc_states, ',', acceptV);

    for(auto inputline : inputsV)
    {
        std::vector<std::string> _input;
        std::string _state = start;
        unsigned i = 0;

        split_char(inputline, ',', _input);

        stack.clear();
        stack.push_back(stack_init);
        
        std::cout << start << "#" << stack_string();
        while(1)
        {
            std::string current_char = i < _input.size() ? _input[i] : "$";
            Output _o;

            if(current_char == "$")
            {
                if(std::find(acceptV.begin(), acceptV.end(), _state) != acceptV.end())
                {    
                    std::cout << "|1";
                    break;
                }
                else if(get_next(_state, "$", stack.empty() ? "$" : stack[stack.size() - 1], _o))
                {
                    std::cout << "|0";
                    break;
                }
            }

            if(get_next(_state, current_char, stack.empty() ? "$" : stack[stack.size() - 1], _o))
            {
                if(get_next(_state, "$", stack.empty() ? "$" : stack[stack.size() - 1], _o))
                {
                    std::cout << "|fail|0";
                    break;
                }
            }
            else
                i++;

            _state = _o.state;

            stack.pop_back();

            if(_o.stack != "$")
            {
                std::string tempst = _o.stack, __t;
                std::reverse(tempst.begin(), tempst.end());
                for(char c : tempst)
                {
                    stack.push_back("");
                    stack[stack.size() - 1] += c;
                }
            }

            std::cout << "|" << _state << "#" << stack_string();
        }
        std::cout << "\n";
    }

    return 0;
}
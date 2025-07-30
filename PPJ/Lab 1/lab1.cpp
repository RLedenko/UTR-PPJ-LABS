#include <iostream>
#include <vector>
#include <algorithm>

#define CITAJ 99
#define KOMENTAR 100

#define DEFAULT 0
#define IDN 1
#define KONST 2
#define OP 3

std::vector<char> ops = {'=', '+', '-', '*', '/', '(', ')'};

std::string clear_com(std::string input)
{
    size_t len = input.size(), state = CITAJ;
    std::string output;
    for(size_t i = 0; i < len; i++)
    {
        if(state == KOMENTAR)
        {
            while(input[i] != '\n')
                i++;
            state = CITAJ;
            continue;
        }
        if(input[i] == '/' && input[i + 1] == '/')
        {
            state = KOMENTAR;
            output += '\n';
        }
        else 
            output += input[i];
    }
    return output;
}

int type(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) ? 1 : (c >= '0' && c <= '9') ? 2 : (c == ' ' || c == '\t' || c == '\n') ? 3 : (std::find(ops.begin(), ops.end(), c) != ops.end()) ? c : -1;
}

int main()
{
    std::string input, temp;
    size_t line_num = 1, len;
    int state = DEFAULT;
    
    while(std::getline(std::cin, temp))
        input += (temp + '\n');

    input = clear_com(input);
    len = input.length();
    temp = "";

    for(size_t i = 0; i < len; i++)
    {
        if(state == DEFAULT)
        {
            if(input[i] == ' ' || input[i] == '\t')
                continue;
            if(input[i] == '\n')
            {
                line_num++;
                continue;
            }
            char _t = type(input[i]);
            if(input[i] == 'z' && input[i + 1] == 'a' && type(input[i + 2]) == 3)
            {
                std::cout << "KR_ZA " << line_num << " " << input[i] << input[i + 1] << "\n";
                i++;
            }
            else if(input[i] == 'o' && input[i + 1] == 'd' && type(input[i + 2]) == 3)
            {
                std::cout << "KR_OD " << line_num << " " << input[i] << input[i + 1] << "\n";
                i++;
            }
            else if(input[i] == 'd' && input[i + 1] == 'o' && type(input[i + 2]) == 3)
            {
                std::cout << "KR_DO " << line_num << " " << input[i] << input[i + 1] << "\n";
                i++;
            }
            else if(input[i] == 'a' && input[i + 1] == 'z' && type(input[i + 2]) == 3)
            {
                std::cout << "KR_AZ " << line_num << " " << input[i] << input[i + 1] << "\n";
                i++;
            }
            else if(_t == 1)
            {
                state = IDN;
                temp += input[i];
            }
            else if(_t == 2)
            {
                state = KONST;
                temp += input[i];
            }
            else if(_t == '=')
                std::cout << "OP_PRIDRUZI " << line_num << " " << input[i] << "\n";
            else if(_t == '+')
                std::cout << "OP_PLUS " << line_num << " " << input[i] << "\n";
            else if(_t == '-')
                std::cout << "OP_MINUS " << line_num << " " << input[i] << "\n";
            else if(_t == '*')
                std::cout << "OP_PUTA " << line_num << " " << input[i] << "\n";
            else if(_t == '/')
                std::cout << "OP_DIJELI " << line_num << " " << input[i] << "\n";
            else if(_t == '(')
                std::cout << "L_ZAGRADA " << line_num << " " << input[i] << "\n";
            else if(_t == ')')
                std::cout << "D_ZAGRADA " << line_num << " " << input[i] << "\n";
        }
        else if(state == IDN)
        {
            char _t = type(input[i]);
            if(input[i] == ' ' || input[i] == '\t')
            {
                std::cout << "IDN " << line_num << " " << temp << "\n";
                temp.clear();
                state = DEFAULT;
                continue;
            }
            if(input[i] == '\n' || (_t != -1 && _t != 1 && _t != 2))
            {
                std::cout << "IDN " << line_num << " " << temp << "\n";
                temp.clear();
                state = DEFAULT;
                i--;
                continue;
            }
            if(_t == 1 || _t == 2)
                temp += input[i];
        }
        else if(state == KONST)
        {
            if(type(input[i]) != 2)
            {
                std::cout << "BROJ " << line_num << " " << temp << "\n";
                temp.clear();
                state = DEFAULT;
                i--;
                continue;
            }
            temp += input[i];
        }
    }
}
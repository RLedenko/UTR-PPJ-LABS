#include <iostream>
#include <vector>
#include <stack>
#include <string>

struct _lexer_line
{
    std::string type;
    unsigned l_num;
    std::string name;
};

std::vector<std::pair<std::string, unsigned>> vars;

_lexer_line split_line(std::string in)
{
    _lexer_line ret;

    std::vector<unsigned> idx;
    size_t s = in.size(), i;
    for(i = 0; i < s; i++)
        if(in[i] == ' ')
            idx.push_back(i);
    
    ret.type  = in.substr(0, idx[0]);
    ret.l_num = std::stoul(in.substr(idx[0] + 1, idx[1] - idx[0] - 1));
    ret.name  = in.substr(idx[1] + 1, idx[2] - idx[1] - 1);

    return ret;
}

unsigned oldest(std::string val)
{
    unsigned r = 0;
    for(unsigned i = vars.size() - 1; i > 0; i--)
    {
        if(vars[i].first == val)
            r = vars[i].second;
        if(vars[i].first == "$" && r && vars[i + 1].first == val)
            break;
    }
    return r;
}

unsigned line_num(std::string in)
{
    std::vector<unsigned> idx;
    size_t s = in.size(), i;
    for(i = 0; i < s; i++)
        if(in[i] == ' ')
            idx.push_back(i);

    return std::stoul(in.substr(idx[0] + 1, idx[1] - idx[0] - 1));
}

int main()
{
    std::string temp;
    std::vector<std::string> comm;
    std::vector<std::vector<std::string>> comms;
    unsigned l = 1;
    while(std::getline(std::cin, temp))
    {
        while(temp[0] == ' ')
            temp = temp.substr(1, temp.length() - 1);
        if((temp[0] == '<' || temp[0] == '$')) 
            continue;
        if(line_num(temp) > l)
        {
            comms.push_back(comm);
            comm.clear();
            l++;
        }
        if(!(temp[0] == '<' || temp[0] == '$'))
            comm.push_back(temp);
    }
    comms.push_back(comm);
    vars.push_back({"-", -1});

    unsigned s = comms.size();
    for(l = 0; l < s; l++)
    {
        std::string hold;
        unsigned s1 = comms[l].size(), state = 0, is_za = 0;
        for(unsigned i = 0; i < s1; i++)
        {
            auto [a, b, c] = split_line(comms[l][i]);
            if(a == "IDN")
            {
                if(state == 0)
                    hold = c;
                else
                {
                    unsigned l_pos = oldest(c);
                    if(!l_pos || (is_za && c == hold))
                    {
                        std::cout << "err " << l + 1 << " " << c << "\n";
                        return 0;
                    }
                    std::cout << l + 1 << " " << l_pos << " " << c << "\n";
                }
                state++;
            }
            else if(a == "KR_ZA")
            {
                is_za++;
                vars.push_back({"$", 0});
            }
            else if(a == "KR_AZ")
            {
                is_za--;
                while(vars[vars.size() - 1].second)
                    vars.pop_back();
                vars.pop_back();
            }
        }
        vars.push_back({hold, l + 1});
    }

    return 0;
}
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

std::string **matrix_g;
std::vector<std::string> statesV, alphaV, acceptV;

int mapToInt(std::string in)
{
    int s, i;
    for(i = 0, s = statesV.size(); i < s; i++) 
        if(statesV[i] == in) 
            return i;
    for(i = 0, s = alphaV.size(); i < s; i++) 
        if(alphaV[i] == in) 
            return i;
    return -1;
}

void resetMatrix(std::vector<std::vector<std::string>> f_matrix)
{
    matrix_g = new std::string*[statesV.size()];
    for (int i = 0, s = statesV.size(); i < s; i++)
        matrix_g[i] = new std::string[alphaV.size()];

    for(int i = 0, s = statesV.size(); i < s; i++)
        for(int j = 0, p = alphaV.size(); j < p; j++)
            matrix_g[i][j] = "\0";

    for(auto x : f_matrix)
        matrix_g[mapToInt(x[0])][mapToInt(x[1])] = x[2];
}

void memClearMatrix()
{
    for (int i = 0, s = statesV.size(); i < s; i++)
        delete[] matrix_g[i];
    delete[] matrix_g;
}

void splitChar(std::string in, char ch, std::vector<std::string> &res)
{
    std::stringstream sstream(in);
    std::string temp = "";
    
    while(std::getline(sstream, temp, ch))
        res.push_back(temp);
}

void parsePair(std::string in, std::vector<std::vector<std::string>> &funct)
{
    int comma, arrow;
    for(comma = 0; in[comma] != ','; comma++);
    for(arrow = 0; in[arrow] != '>'; arrow++);

    std::vector<std::string> r;
    
    r.push_back(in.substr(0, comma)); //current state
    r.push_back(in.substr(comma + 1, arrow - comma - 2)); //read from input
    r.push_back(in.substr(arrow + 1, in.size() - arrow - 1)); //new state vector

    funct.push_back(r);
}

std::string inline f(std::string state, std::string input)
{
    return matrix_g[mapToInt(state)][mapToInt(input)];
}

int getGroup(std::string item, std::vector<std::vector<std::string>> groups)
{
    int i = 0;
    for(auto x : groups)
    {
        for(auto y : x)
            if(y == item)
                return i;
        i++;
    }
    return -1;
}

int main()
{
    std::string states, alpha, acc_states, start;
    std::vector<std::string> function, reachable, statesV_FINAL, f_matrix_output;
    std::vector<std::vector<std::string>> f_matrix, groups(2);

    std::getline(std::cin, states);
    std::getline(std::cin, alpha);
    std::getline(std::cin, acc_states);
    std::getline(std::cin, start);
    while(1)
    {
        std::string temp;
        std::getline(std::cin, temp);
        if(temp.empty())
            break;
        function.push_back(temp);
    }

    splitChar(states, ',', statesV);
    splitChar(alpha, ',', alphaV);
    splitChar(acc_states, ',', acceptV);

    for(std::string str : function)
        parsePair(str, f_matrix);

    resetMatrix(f_matrix);

    reachable.push_back(start); 
    while(1)
    {
        std::vector<std::string> temp;
        for(auto x : reachable)
        {
            if(std::find(temp.begin(), temp.end(), x) == temp.end())
                temp.push_back(x);
            for(auto y : alphaV)
            {
                auto pushVal = f(x, y);
                if(!pushVal.empty() && std::find(temp.begin(), temp.end(), pushVal) == temp.end())
                    temp.push_back(pushVal);
            }
        }
        sort(temp.begin(), temp.end());
        if(temp == reachable)
            break;
        else
            reachable = temp;
    }

    f_matrix.clear();
    int ctr = 0;
    for(std::string str : function)
    {
        parsePair(str, f_matrix);
        if(std::find(reachable.begin(), reachable.end(), f_matrix[ctr][0]) != reachable.end())
            ctr++;
        else
            f_matrix.pop_back();
    }
    memClearMatrix();

    sort(reachable.begin(), reachable.end());
    statesV = reachable;
    reachable.clear();
    for(auto x : acceptV)
        for(auto y : statesV)
            if(x == y)
                reachable.push_back(x);
    acceptV = reachable;

    for(auto x : statesV)
    {
        char _t = 1;
        for(auto y : acceptV)
            if(x == y)
            {
                _t = 0;
                break;
            }
        if(_t && std::find(groups[0].begin(), groups[0].end(), x) == groups[0].end())
            groups[0].push_back(x);
    }
    sort(groups[0].begin(), groups[0].end());

    resetMatrix(f_matrix);

    for(auto x : acceptV) groups[1].push_back(x);
    while(1) //Algoritam 2
    {
        std::vector<std::vector<std::string>> _groupsTemp;
        for(auto group : groups)
        {
            int s = group.size();
            if(s == 0) continue;
            if(group.size() == 1)
            {
                _groupsTemp.push_back(group);
                continue;
            }
            
            std::vector<std::string> guaranteedGroup, potentialGroup;
            for(int i = 0; i < s; i++)
                for(int j = i; j < s; j++)
                {
                    if(i == j) continue;

                    if(std::find(potentialGroup.begin(), potentialGroup.end(), group[i]) != potentialGroup.end() || std::find(potentialGroup.begin(), potentialGroup.end(), group[j]) != potentialGroup.end())
                        continue;

                    int gm1 = mapToInt(group[i]), gm2 = mapToInt(group[j]);

                    for(int k = 0, p = alphaV.size(); k < p; k++)
                    {
                        int lval = getGroup(matrix_g[gm1][k], groups), rval = getGroup(matrix_g[gm2][k], groups);
                        if(lval == -1 || rval == -1) continue;
                        if(lval != rval)
                        {
                            potentialGroup.push_back(group[j]);
                            break;
                        }
                    }
                }
            if(potentialGroup.empty())
            {
                _groupsTemp.push_back(group);
                continue;
            }
            sort(potentialGroup.begin(), potentialGroup.end());

            for(auto x : group)
            {
                char _t = 1;
                for(auto y : potentialGroup)
                    if(x == y)
                    {
                        _t = 0;
                        break;
                    }
                if(_t && std::find(guaranteedGroup.begin(), guaranteedGroup.end(), x) == guaranteedGroup.end())
                    guaranteedGroup.push_back(x);
            }
            sort(guaranteedGroup.begin(), guaranteedGroup.end());

            if(!guaranteedGroup.empty())
                _groupsTemp.push_back(guaranteedGroup);
            _groupsTemp.push_back(potentialGroup);
        }
        sort(_groupsTemp.begin(), _groupsTemp.end());

        if(_groupsTemp == groups)
            break;
        groups = _groupsTemp;
    }

    statesV_FINAL = statesV;

    reachable.clear();
    for(auto x : groups)
        if(!x.empty())
            reachable.push_back(x[0]);

    sort(reachable.begin(), reachable.end());
    statesV = reachable;
    reachable.clear();
    for(auto x : acceptV)
        if(std::find(statesV.begin(), statesV.end(), x) != statesV.end())
            reachable.push_back(x);
    acceptV = reachable;

    for(int i = 0, s = statesV.size(); i < s; i++)
        std::cout << statesV[i] << ((i != s - 1) ? "," : "");
    std::cout << "\n";

    for(int i = 0, s = alphaV.size(); i < s; i++)
        std::cout << alphaV[i] << ((i != s - 1) ? "," : "");
    std::cout << "\n";

    for(int i = 0, s = acceptV.size(); i < s; i++)
        std::cout << acceptV[i] << ((i != s - 1) ? "," : "");
    std::cout << "\n";

    std::cout << groups[getGroup(start, groups)][0] << "\n";

    for(auto x : f_matrix)
        if(std::find(statesV_FINAL.begin(), statesV_FINAL.end(), x[0]) != statesV_FINAL.end() && std::find(statesV_FINAL.begin(), statesV_FINAL.end(), x[2]) != statesV_FINAL.end())
        {
            std::string _t = groups[getGroup(x[0], groups)][0] + "," + x[1] + "->" + groups[getGroup(x[2], groups)][0];
            if(std::find(f_matrix_output.begin(), f_matrix_output.end(), _t) == f_matrix_output.end())
                f_matrix_output.push_back(_t);
        }
    sort(f_matrix_output.begin(), f_matrix_output.end());

    for(auto x : f_matrix_output)
        std::cout << x << "\n";

    memClearMatrix();

    return 0;
}
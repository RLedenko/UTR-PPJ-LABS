#include <iostream>
#include <vector>
#include <stack>
#include <sstream>
#include <string>
#include <memory>
#include <fstream>

// #define MUL_DIV_V1 1

struct Node
{
    std::string data;
    std::vector<Node*> children;

    Node(std::string d) : data(d) {}
};

struct vardata
{
    unsigned varnum;
    unsigned l_num_def;
    unsigned l_num_var;
    std::string name;
};

struct _lexer_line
{
    std::string type;
    unsigned l_num;
    std::string name;
};

unsigned label_ctr = 0;
std::vector<vardata> allvars;
std::vector<Node*> node_collector;
std::vector<std::pair<std::string, unsigned>> vars;
std::stringstream frisc;
std::ofstream frisc_file("a.frisc");

Node* convert_p(Node* pos);
Node* convert_e(Node* pos);
Node* convert_e_list(Node* pos, Node* l_op);
Node* convert_t(Node* pos);
Node* convert_t_list(Node* pos, Node* l_op);

void process_command(Node* pos);
void process_instructions(Node* pos);

_lexer_line split_lexer_line(std::string in)
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

void lab3ify(std::string str)
{   
    auto line_num = [](std::string in) -> unsigned 
    {
        std::vector<unsigned> idx;
        size_t s = in.size(), i;
        for(i = 0; i < s; i++)
            if(in[i] == ' ')
                idx.push_back(i);

        return std::stoul(in.substr(idx[0] + 1, idx[1] - idx[0] - 1));
    };

    auto oldest = [](std::string val) -> unsigned 
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
    };


    unsigned varctr = 0; char rez_found = 0;
    std::vector<std::string> comm;
    std::vector<std::vector<std::string>> comms;
    unsigned l = 1;
    std::stringstream input(str);
    for(std::string temp; std::getline(input, temp); )
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
        unsigned s1 = comms[l].size(), is_za = 0;
        for(unsigned i = 0; i < s1; i++)
        {
            auto [a, b, c] = split_lexer_line(comms[l][i]);
            if(a == "IDN")
            {
                vars.push_back({c, l + 1});
                unsigned l_pos = oldest(c);
                unsigned var_num = 4294967295U;
                for(auto x : allvars)
                    if((x.l_num_def == l_pos) && x.name == c)
                    {
                        var_num = x.varnum;
                        break;
                    }
                allvars.push_back({(var_num == 4294967295U ? ++varctr : var_num), l_pos ? l_pos : l + 1, l + 1, c});
                if(!is_za && !rez_found && allvars[allvars.size() - 1].name == "rez")
                {
                    allvars[allvars.size() - 1].varnum = 0;
                    rez_found = 1;
                    varctr--;
                }
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
}

vardata get_vardata_from_node(std::string &node)
{
    auto l = split_lexer_line(node);
    for(auto x : allvars)
    {
        if(x.l_num_var == l.l_num && x.name == l.name)
            return x;
    }
    return {0, 0, 0, ""};
}

Node* generate_tree(std::string input)
{
    std::istringstream iss;
    iss.str(input);

    Node *root = nullptr;
    std::vector<Node*> layers;
    for(std::string temp; std::getline(iss, temp); )
    {
        unsigned dctr = 0;
        while(temp[0] == ' ') 
        {
            temp = temp.substr(1, temp.length() - 1);
            dctr++;
        }

        if(!dctr)
        {
            Node* origin = new Node(temp);
            node_collector.push_back(origin);
            root = origin;
            layers.push_back(root);
        }
        else
        {
            if(layers.size() - 1 < dctr)
                layers.push_back(nullptr);
            Node* layer = new Node(temp);
            node_collector.push_back(layer);
            if(temp != "$")
                layers[dctr - 1]->children.push_back(layer);
            if(temp[0] == '<') 
                layers[dctr] = layer;
        }
    }
    return root;
}

Node* convert_p(Node* pos)
{
    std::string selector = pos->children[0]->data;
    if(selector.substr(0, 9) == "L_ZAGRADA")
        return convert_e(pos->children[1]);
    Node* p_node = new Node(selector);
    node_collector.push_back(p_node);
    if(pos->children.size() > 1)
        p_node->children.push_back(convert_p(pos->children[1]));
    
    return p_node;
}

Node* convert_t(Node* pos)
{
    Node* p_node = convert_p(pos->children[0]);
    return convert_t_list(pos->children[1], p_node);
}

Node* convert_t_list(Node* pos, Node* l_op)
{
    if(pos->children.empty())
        return l_op;
    Node* root = new Node(pos->children[0]->data);
    node_collector.push_back(root);
    root->children.push_back(l_op);
    root->children.push_back(convert_p(pos->children[1]->children[0]));
    return convert_t_list(pos->children[1]->children[1], root);
}

Node* convert_e(Node* pos)
{
    Node* t_node = convert_t(pos->children[0]);
    return convert_e_list(pos->children[1], t_node);
}

Node* convert_e_list(Node* pos, Node* l_op)
{
    if(pos->children.empty())
        return l_op;
    Node* root = new Node(pos->children[0]->data);
    node_collector.push_back(root);
    root->children.push_back(l_op);
    root->children.push_back(convert_t(pos->children[1]->children[0]));
    return convert_e_list(pos->children[1]->children[1], root);
}


void evaluate(Node* pos)
{
    if(pos->data.substr(0, 3) == "IDN")
    {
        vardata __p = get_vardata_from_node(pos->data);
        frisc << " LOAD R0, (V" << __p.varnum << ")\n PUSH R0\n";
    }
    else if(pos->data.substr(0, 4) == "BROJ")
    {
        std::string __br = pos->data;
        for(int i = 0; i != 2; )
        {
            if(__br[0] == ' ') i++;
            __br = __br.substr(1, __br.length() - 1);
        }
        frisc << " MOVE %D " << __br << ", R0\n PUSH R0\n";
    }
    else if(pos->data.substr(0, 8) == "OP_MINUS")
    {
        if(pos->children.size() == 1)
        {
            evaluate(pos->children[0]);
            frisc << " MOVE %D 0, R0\n POP R1\n SUB R0, R1, R0\n PUSH R0\n";
        }
        else
        {
            evaluate(pos->children[0]);
            evaluate(pos->children[1]);
            frisc << " POP R1\n POP R0\n SUB R0, R1, R0\n PUSH R0\n";
        }
    }
    else if(pos->data.substr(0, 7) == "OP_PLUS")
    {
        if(pos->children.size() > 1)
        {
            evaluate(pos->children[0]);
            evaluate(pos->children[1]);
            frisc << " POP R1\n POP R0\n ADD R0, R1, R0\n PUSH R0\n";
        }
    }
    else if(pos->data.substr(0, 7) == "OP_PUTA")
    {
        evaluate(pos->children[0]);
        evaluate(pos->children[1]);
        frisc << " POP R3\n POP R4\n CALL MUL\n PUSH R3\n";
    }
    else if(pos->data.substr(0, 9) == "OP_DIJELI")
    {
        evaluate(pos->children[0]);
        evaluate(pos->children[1]);
        frisc << " POP R4\n POP R3\n CALL DIV\n PUSH R3\n";
    }
}

void process_command(Node* pos)
{
    if(pos->children[0]->data == "<naredba_pridruzivanja>")
    {
        Node* __e = convert_e(pos->children[0]->children[2]);
        vardata return_var = get_vardata_from_node(pos->children[0]->children[0]->data);
        evaluate(__e);
        frisc << " POP R0\n STORE R0, (V" << return_var.varnum << ")\n";
    }
    else if(pos->children[0]->data == "<za_petlja>")
    {
        Node* __e_od   = convert_e(pos->children[0]->children[3]),
            * __e_do   = convert_e(pos->children[0]->children[5]);
        unsigned this_loop = label_ctr++;
        vardata loop_var = get_vardata_from_node(pos->children[0]->children[1]->data);
        evaluate(__e_od);
        frisc << " POP R0\n STORE R0, (V" << loop_var.varnum << ")\nL" << this_loop << "\n";
        process_instructions(pos->children[0]->children[6]);
        evaluate(__e_do);

        frisc << " POP R0\n LOAD R1, (V" << loop_var.varnum << ")\n MOVE %D 1, R2\n ADD R1, R2, R1\n STORE R1, (V" << loop_var.varnum << ")\n CMP R1, R0\n JP_SLE L" << this_loop << "\n";
    }
}

void process_instructions(Node* pos)
{
    for(Node *comlist = pos; !comlist->children.empty(); comlist = comlist->children[1])
        process_command(comlist->children[0]);
}

std::vector<std::string> line_split(std::stringstream& str)
{
    std::vector<std::string> ret;
    std::string temp;
    while(std::getline(str, temp))
        ret.push_back(temp + '\n');
    return ret;
}

std::vector<std::string> command_split_2(std::string str)
{
    std::vector<std::string> vals(2);
    unsigned i = 0;
    for(i = 0; str[i] != ' '; i++)
        vals[0] += str[i];
    i++;
    for(; str[i] != '\n'; i++)
        vals[1] += str[i];
    return vals;
}

std::vector<std::string> command_split_move(std::string str)
{
    std::vector<std::string> vals(3);
    unsigned i = 0;
    for(i = 0; str[i] != ' '; i++)
        vals[0] += str[i];
    i++;
    if(str[i] == '%')
    {
        for(; str[i] != ' '; i++)
            vals[1] += str[i];
        i++;
        for(; str[i] != ','; i++)
            vals[2] += str[i];
        i += 2;
        vals.push_back("");
        for(; str[i] != '\n'; i++)
            vals[3] += str[i];
    }
    else
    {
        for(; str[i] != ','; i++)
            vals[1] += str[i];
        i += 2;
        for(; str[i] != '\n'; i++)
            vals[2] += str[i];
    }
    return vals;
}

std::string optimize_opt2(std::string prog)
{
    std::stringstream ret, __p(prog);
    std::string l1, l2;
    unsigned i = 0;

    auto lines = line_split(__p);
    for(i = 0; i < lines.size() - 2; i++)
    {
        l1 = lines[i], l2 = lines[i + 1];
        if(l1[0] != ' ' || l2[0] != ' ')
        {
            ret << l1;
            continue;
        }

        auto l1v = command_split_2(l1.substr(1, l1.length() - 1));
        auto l2v = command_split_2(l2.substr(1, l2.length() - 1));

        if(l1v[0] == "PUSH" && l2v[0] == "POP" && l1v[1] == l2v[1])
            i++;
        else if(l1v[0] == "PUSH" && l2v[0] == "POP" && l1v[1] != l2v[1])
        {
            ret << " MOVE " << l1v[1] << ", " << l2v[1] << "\n";
            i++;
        }
        else if(l1v[0] == "MOVE" && l2v[0] == "MOVE" && l1v[1][0] == '%' && l2v[1][0] == 'R')
        {
            l1v = command_split_move(l1.substr(1, l1.length() - 1));
            l2v = command_split_move(l2.substr(1, l2.length() - 1));
            
            if(l1v[3] == l2v[1])
            {
                ret << " MOVE %D " << l1v[2] << ", " << l2v[2] << "\n";
                i++;
            }
            else
                ret << l1;
        }
        else if(l1v[0] == "LOAD" && l2v[0] == "MOVE" && l2v[1][0] == 'R')
        {
            l1v = command_split_move(l1.substr(1, l1.length() - 1));
            l2v = command_split_move(l2.substr(1, l2.length() - 1));
            
            if(l1v[1] == l2v[1])
            {
                ret << " LOAD " << l2v[2] << ", " << l1v[2] << "\n";
                i++;
            }
            else
                ret << l1;
        }
        else 
            ret << l1;
    }
    for(; i < lines.size(); i++)
        ret << lines[i];
    return ret.str();
}

std::string optimize_opt4(std::string prog)
{
    std::stringstream _p(prog), ret;
    auto lines = line_split(_p);
    unsigned i = 0;
    std::string l1, l2, l3, l4;

    for(i = 0; i < lines.size() - 4; i++)
    {
        l1 = lines[i], l2 = lines[i + 1], l3 = lines[i + 2], l4 = lines[i + 3];
        if(l1[0] != ' ' || l2[0] != ' ' || l3[0] != ' ' || l4[0] != ' ')
        {
            ret << l1;
            continue;
        }

        auto l1v = command_split_2(l1.substr(1, l1.length() - 1));
        auto l2v = command_split_2(l2.substr(1, l2.length() - 1));
        auto l3v = command_split_2(l3.substr(1, l3.length() - 1));
        auto l4v = command_split_2(l4.substr(1, l4.length() - 1));

        if(l1v[0] == "MOVE" && l2v[0] == "PUSH" && l3v[0] == "MOVE" && l4v[0] == "POP")
        {
            l1v = command_split_move(l1.substr(1, l1.length() - 1));
            l3v = command_split_move(l3.substr(1, l3.length() - 1));

            if(l1v.size() == 4 && l3v.size() == 4 && l1v[3] == l2v[1] && l2v[1] == l3v[3] && l3v[3] != l4v[1])
            {
                ret << " MOVE %D " << l3v[2] << ", " << l3v[3] << "\n"
                    << " MOVE %D " << l1v[2] << ", " << l4v[1] << "\n";
                i += 3;
                continue;
            }
        }
        ret << l1;
    }
    for(; i < lines.size(); i++)
        ret << lines[i];
    return ret.str();
}

std::string optimize_push_pop(std::string prog)
{
    std::stringstream _p(prog), ret;
    auto lines = line_split(_p);
    unsigned i = 0, j = 0;
    size_t s = lines.size();

    for(i = 0; i < s; i++)
    {
        std::string l = lines[i], cl;
        auto lv = command_split_2(l.substr(1, l.length() - 1));

        if(lv[0] == "PUSH")
        {
            char flag = 0;
            for(j = i + 1; j < s - 1; j++)
            {
                cl = lines[j];
                if(cl[0] != ' ') 
                    continue;

                auto clv = command_split_2(cl.substr(1, cl.length() - 1));

                if(clv[0] != "POP" && cl.find(lv[1]) != std::string::npos)
                    break;
                if(clv[0] == "POP" && clv[1] != lv[1])
                {
                    flag = 2;
                    break;
                }
                if(clv[0] == "POP" && clv[1] == lv[1])
                {
                    flag = 1;
                    break;
                }
            }
            if(flag == 1)
            {
                lines[i] = "\n";
                lines[j] = "\n";
            }
            else if(flag == 2)
            {
                auto clv = command_split_2(cl.substr(1, cl.length() - 1));
                lines[i] = "\n";
                lines[j] = " MOVE " + lv[1] + ", " + clv[1] + "\n";
            }
        }
    }
    for(auto l : lines)
        if(l != "\n")
            ret << l;
    return ret.str();
}

std::string optimize(std::string init)
{
    std::string prog = optimize_opt2(init);
    prog = optimize_opt4(prog);
    prog = optimize_push_pop(prog);
    return prog == init ? prog : optimize(prog);
}

int main()
{
    std::string input, temp;
    std::stringstream vars;
    std::istringstream iss;
    unsigned varctr = 0;

    while(std::getline(std::cin, temp))
        input += temp + '\n';

    lab3ify(input);

    Node *gen_tree = generate_tree(input);

    frisc << " MOVE 40000, R7\n JP MAIN\nV0 DW 0 ; rez\n";
    for(vardata& x : allvars)
        if(x.varnum > varctr)
        {
            frisc << "V" << x.varnum << " DW 0\n";
            varctr++;
        }

    frisc << "DIV\n MOVE %D 0, R0\n MOVE %D 1, R1\n CMP R4, R1\n JP_EQ DIV_RET\n CMP R4, R0\n JP_NE D_O2_N0\n JP DIV_RET\nD_O2_N0\n CMP R3, R0\n JP_EQ DIV_RET\n MOVE %D 0, R2\n MOVE %D 0, R5\n CMP R3, R0\n JP_SGE D_1_P\n SUB R0, R3, R3\n ADD R5, R1, R5\nD_1_P\n CMP R4, R0\n JP_SGE DIV_2\n SUB R0, R4, R4\n ADD R5, R1, R5\nDIV_2\n AND R4, R1, R5\n CMP R5, R1\n JP_EQ DIV_LOOP_S\n SHR R3, R1, R3\n SHR R4, R1, R4\n JP DIV_2\nDIV_LOOP_S \n CMP R4, R1\n JP_EQ DIV_RET\n MOVE %D 0, R5\nDIV_LOOP\n SUB R3, R4, R3\n CMP R3, R0\n JP_SLT D_LO_END\n ADD R2, R1, R2\n JP DIV_LOOP\nD_LO_END\n MOVE R2, R3\n CMP R5, R1\n JP_NE DIV_RET\n SUB R0, R3, R3\nDIV_RET\n RET\n" << "MUL\n MOVE %D 0, R0\n MOVE %D 1, R1\n MOVE %D 0, R2\n MOVE %D 0, R5\n MOVE %D 0, R6\n CMP R3, R0\n JP_SGE M_1_P\n SUB R0, R3, R3\n ADD R5, R1, R5\nM_1_P\n CMP R4, R0\n JP_SGE MUL_LOOP_S\n SUB R0, R4, R4\n ADD R5, R1, R5\nMUL_LOOP_S\n PUSH R5\nMUL_LOOP\n CMP R4, R0\n JP_EQ MUL_END\n AND R4, R1, R5\n CMP R5, R0\n JP_EQ MUL_LOOP_M\n SHL R3, R2, R5\n ADD R6, R5, R6\nMUL_LOOP_M\n SHR R4, R1, R4\n ADD R2, R1, R2\n JP MUL_LOOP\nMUL_END\n POP R5\n CMP R5, R1\n JP_NE MUL_RET\n SUB R0, R6, R6\nMUL_RET\n MOVE R6, R3\n RET\nMAIN\n";
    process_instructions(gen_tree->children[0]);
    frisc << " LOAD R6, (V0)\n HALT";

    frisc_file << optimize(frisc.str());
    frisc_file.close();

    for(unsigned i = 0; i < node_collector.size(); i++)
        delete node_collector[i];

    return 0;
}
#include<bits/stdc++.h>

using namespace std;

class parser
{
public:
    bool system;                    //identifies windows vs Linux environment
    int gsize;                      //number of productions in grammar
    string start_symbol;            //starting symbol of grammar
	vector<string> p_str;           //string to be parsed
    vector<string> glhs[100];       //LHS part of i'th production
    vector<string> grhs[100];       //RHS part of i'th production
    vector<string> variable;        //list of all variables
    vector<string> terminal;        //list of all terminals
    vector<string> pre_first[100];  //preprocessed first values of all variables according to their index in "variable" vector
    vector<string> pre_follow[100]; //preprocessed follow values of all variables according to their index in "variable" vector
    vector<int> ptable[50][50]; //LL(1) parsing table is stored here.

    bool check_terminal (string);   //function to check validity of terminal symbol
    bool check_variable (string);   //function to check validity of variable symbol
    void read_grammar ();           //function to read grammar

    parser (int);                   //overloaded constructor
    parser ();                      //constructor
    ~parser ();                     //destructor
};

parser::parser(int x)       //overloaded constructor
{
    gsize = x;
    system=true;
}

parser::parser()            //constructor
{
    gsize = 0;
    system=true;
}

parser::~parser()           //destructor
{

}
bool parser::check_terminal (string x)
{
    bool ans = true;
    for(string::iterator i = x.begin() ; i != x.end() ; i++ )
    {
        if(isupper(*i))
        {
            ans = false;
            break;
        }
    }
    return ans;
}

bool parser::check_variable (string x)
{
    bool ans = true;
    for(std::string::iterator i = x.begin() ; i != x.end() ; i++ )
    {
        if(!isupper(*i))
        {
            ans = false;
            break;
        }
    }
    return ans;
}

void parser::read_grammar()
{
    char temp[1000];
    int startsym=1;
    FILE *fp;
    fp = fopen("grammar1t.txt","r");
    fscanf(fp,"%d",&gsize);
    cout << gsize << endl;
    for(int i = 0; i < gsize; i++)
    {
        if(system)
            fscanf(fp," %[^\n]s",temp);                             //Windows vs Linux crash
        else
            fscanf(fp," %[^\r]s",temp);
	//printf("%s\n",temp);
        char c=temp[0];
        int curr_pos=0;
        string left_var;
        while(c!=' ')
        {
            left_var.push_back(c);
            curr_pos++;
            c=temp[curr_pos];
        }
        if(check_variable(left_var))
        {
            glhs[i].push_back(left_var);
            if(startsym)
            {
                start_symbol=left_var;
                startsym=0;
            }
            int flag=0;
            for(int k=0;k<variable.size();k++)
            {
                if(variable[k].compare(left_var)==0)
                {
                    flag=1;
                    break;
                }
            }
            if(flag==0)
            {
                variable.push_back(left_var);
            }
        }
        else
        {
            cout<<"ERROR: Grammar not according to the READ_ME File norms1\n Aborting...\n";
            exit(1);
        }
        curr_pos+=3;
        c=temp[curr_pos];
        string right_alpha;
        lab:
        while(c==' ')
        {
            curr_pos++;
            c=temp[curr_pos];
        }
        right_alpha.clear();
        while(c!=' '&&c!='\0'&&c!='\n'&&c!='\r')
        {
            right_alpha.push_back(c);
            curr_pos++;
            c=temp[curr_pos];
        }
        if(check_terminal(right_alpha)||check_variable(right_alpha))
        {
            grhs[i].push_back(right_alpha);
            /*if(check_variable(right_alpha))
            {
                int flag=0;
                for(int k=0;k<variable.size();k++)
                {
                    if(variable[k].compare(right_alpha)==0)
                    {
                        flag=1;
                        break;
                    }
                }
                if(flag==0)
                {
                    variable.push_back(right_alpha);
                }
            }
            else*/ if(right_alpha.compare("null")!=0 && check_terminal(right_alpha))
            {
                int flag=0;
                for(int k=0;k<terminal.size();k++)
                {
                    if(terminal[k].compare(right_alpha)==0)
                    {
                        flag=1;
                        break;
                    }
                }
                if(flag==0)
                {
                    terminal.push_back(right_alpha);
                }
            }
        }
        else
        {
            cout<<"ERROR: Grammar not according to the READ_ME File norms2\nAborting...\n";
            exit(1);
        }
        while(c==' ')           //production right side white spaces removal
        {
            curr_pos++;
            c=temp[curr_pos];
        }
        if(c=='\0'||c=='\n'||c=='\r')
            continue;
        else
            goto lab;
    }
    fclose(fp);
}
vector<string> ans;
vector<string> dp;
void find_first(parser p,string alpha,int* locnull)
{
    //cout<<alpha<<endl;
    if(p.check_terminal(alpha)&&alpha.compare("null")!=0)
    {
        vector<string>::iterator it=find(ans.begin(),ans.end(),alpha);
        vector<string>::iterator itt=find(p.terminal.begin(),p.terminal.end(),alpha);
        if(it==ans.end()&&itt!=p.terminal.end())
            ans.push_back(alpha);
        *locnull=0;
        return;
    }
    if(alpha.compare("null")==0)
    {
        ans.push_back(alpha);
        //cout<<ans.size()<<" | ";
        *locnull=1;
        return;
    }
    for(int i=0;i<p.gsize;i++)
    {
        if(p.glhs[i][0].compare(alpha)==0)
        {
            for(vector<string>::iterator j=p.grhs[i].begin(); j!=p.grhs[i].end(); j++)
            {
                if(p.check_terminal(*j) && (*j).compare("null")!=0)
                {
                    vector<string>::iterator it=find(ans.begin(),ans.end(),*j);
                    vector<string>::iterator itt=find(p.terminal.begin(),p.terminal.end(),*j);
                    if(it==ans.end() && itt!=p.terminal.end())
                        ans.push_back(*j);
                    *locnull=0;
                    break;
                }
                else if(p.check_variable(*j))
                {

                    vector<string>::iterator itt=find(dp.begin(),dp.end(),*j);
                    if(itt==dp.end())
                    {
                        //cout<<*locnull<<*j<<"\t";
                        dp.push_back(*j);
                        find_first(p,*j,locnull);

                        vector<string>::iterator ittt=find(dp.begin(),dp.end(),*j);
                        dp.erase(ittt);
                    }
                    else
                    {
                        break;
                    }
                    if(*locnull==0)
                        break;
                    *locnull=0;
                    if(j+1!=p.grhs[i].end())
                    {
                        vector<string>::iterator it=find(ans.begin(),ans.end(),"null");
                        if(it!=ans.end())
                            ans.erase(it);
                    }
                    else
                    {
                        int it=i+1,flag=0;
                        for(    ;it<p.gsize;it++)
                        {
                            if(p.glhs[it][0].compare(p.glhs[i][0])==0)
                            {
                                flag=1;
                                break;
                            }
                        }
                        if(flag==1)
                        {
                            vector<string>::iterator itt=find(ans.begin(),ans.end(),"null");
                            if(itt!=ans.end())
                                ans.erase(itt);

                        }
                        else
                        {
                            return;
                        }
                    }
                }
                else
                {
                    *locnull=1;
                    ans.push_back("null");
                    break;
                }
            }
        }
    }
    /*for(vector<string>::iterator pj=dp.begin();pj!=dp.end();pj++)
        cout<<*pj<<" ";*/
    return;
}

void find_first_util(parser p,vector<string> s)
{
    ans.clear();
    int loc_null=0;

    for(vector<string>::iterator i=s.begin();i!=s.end();i++)
    {
        loc_null=0;
        dp.clear();
        dp.push_back(*i);
        //cout<<*i<<" | ";
        find_first(p,*i,&loc_null);
        vector<string>::iterator itt=find(ans.begin(),ans.end(),"null");
        if(itt!=ans.end() && i+1!=s.end())
        {
            ans.erase(itt);
        }
        else
            break;
    }

}

void populate_first(parser* p)       //function to compute FIRST sets of all variables
{
    int j=0;
    for(vector<string>::iterator i=p->variable.begin(); i!=p->variable.end();i++)
    {
        ans.clear();
        int loc_null=0;
        dp.clear();
        dp.push_back(*i);
        find_first(*p,*i,&loc_null);
        for(vector<string>::iterator it=ans.begin();it!=ans.end();it++)
        {
            p->pre_first[j].push_back(*it);
        }
        j++;
    }
    ans.clear();
}

vector<string> foll_sol;
vector<string> foll_pro;
void find_follow(string s,parser p)
{
    //cout<<s<<"\n";
    if(p.start_symbol.compare(s)==0)
    {
        vector<string>::iterator itt=find(foll_sol.begin(),foll_sol.end(),"$");
        if(itt==foll_sol.end())
            foll_sol.push_back("$");
    }
    //cout<<"gotcha ";
    for(int i=0;i<p.gsize;i++)
    {
        for(vector<string>::iterator it=p.grhs[i].begin();it!=p.grhs[i].end();it++)
        {
            if(s.compare(*it)==0 && it+1 != p.grhs[i].end())
            {
                //cout<<p.glhs[i][0]<<" ";
                vector<string> temp;
                it++;
                while(it!=p.grhs[i].end())
                {
                    temp.push_back(*it);
                    it++;
                }
                ans.clear();
                find_first_util(p,temp);
                int flag=0;
                for(vector<string>::iterator itt=ans.begin();itt!=ans.end();itt++)
                {
                    if((*itt).compare("null")==0)
                    {
                        flag=1;
                    }
                    else
                    {
                        vector<string>::iterator ittt=find(foll_sol.begin(),foll_sol.end(),*itt);
                        if(ittt==foll_sol.end())
                            foll_sol.push_back(*itt);
                    }
                }
                ans.clear();
                if(flag==0)
                {
                    break;
                }
                else
                {
                    vector<string>::iterator ittt=find(foll_pro.begin(),foll_pro.end(),p.glhs[i][0]);
                    if(p.glhs[i][0].compare(s)!=0 )  //to remove direct and indirect left recursion
                    {
                        if(ittt==foll_pro.end())
                        {
                            foll_pro.push_back(p.glhs[i][0]);
                            find_follow(p.glhs[i][0],p);
                            ittt=find(foll_pro.begin(),foll_pro.end(),p.glhs[i][0]);
                            if(ittt!=foll_pro.end())
                                foll_pro.erase(ittt);
                        }
                        else
                        {
                            break;
                        }
                    }

                    break;
                }
            }
            else if(s.compare(*it)==0 && it+1 == p.grhs[i].end())
            {
                //cout<<p.glhs[i][0]<<" ";
                vector<string>::iterator ittt=find(foll_pro.begin(),foll_pro.end(),p.glhs[i][0]);
                if(p.glhs[i][0].compare(s)!=0 )  //to remove direct and indirect left recursion
                    {
                        if(ittt==foll_pro.end())
                        {
                            foll_pro.push_back(p.glhs[i][0]);
                            find_follow(p.glhs[i][0],p);
                            ittt=find(foll_pro.begin(),foll_pro.end(),p.glhs[i][0]);
                            if(ittt!=foll_pro.end())
                                foll_pro.erase(ittt);
                        }
                        else
                        {
                            break;
                        }
                    }
                break;
            }
        }
    }
    return;
}

void populate_follow(parser *p)      //function to compute FOLLOW sets of all variables
{
    for(int i=0; i<p->variable.size(); i++)
    {
        foll_sol.clear();
        foll_pro.clear();
        foll_pro.push_back(p->variable[i]);
        find_follow(p->variable[i],*p);
        for(vector<string>::iterator it=foll_sol.begin(); it!=foll_sol.end(); it++)
        {
            p->pre_follow[i].push_back(*it);
        }
        foll_pro.clear();
        foll_sol.clear();
    }
}

void compute_table(parser *p)
{
    for(int i=0;i<p->gsize;i++)
    {
        //cout<<i<<"  -> ";
        vector<string> temp;
        temp.clear();
        for(int j=0;j<p->grhs[i].size();j++)
        {
            temp.push_back(p->grhs[i][j]);
        }
        find_first_util(*p,temp);
        int fflag=0,i_ind;
        /*for(int j=0;j<ans.size();j++)
            cout<<ans[j]<<" ";*/
        vector<string>::iterator it=find(ans.begin(),ans.end(),"null");
        if(it!=ans.end())
        {
            fflag=1;
            ans.erase(it);
        }
        it=find(p->variable.begin(),p->variable.end(),p->glhs[i][0]);
        i_ind=it-p->variable.begin();
        for(it=ans.begin();it!=ans.end();it++)
        {
            vector<string>::iterator itt=find(p->terminal.begin(),p->terminal.end(),*it);
            int j_ind=itt-p->terminal.begin();
            p->ptable[i_ind][j_ind].push_back(i);
            //cout<<*it<<" "<<i_ind<<" "<<j_ind<<"  ";
        }
        if(fflag==1)
        {

            for(int j=0;j<p->pre_follow[i_ind].size();j++)
            {
                vector<string>::iterator itt=find(p->terminal.begin(),p->terminal.end(),p->pre_follow[i_ind][j]);
                if(itt==p->terminal.end())
                {
                    p->ptable[i_ind][p->terminal.size()].push_back(i);
                    //cout<<"$ "<<i_ind<<" "<<p->terminal.size()<<"  ";
                }
                else
                {
                    int j_ind=itt-p->terminal.begin();
                    p->ptable[i_ind][j_ind].push_back(i);
                    //cout<<*itt<<" "<<i_ind<<" "<<j_ind<<"  ";
                }
            }
        }
        ans.clear();
        //cout<<endl;
    }
}

void input_parsing_string(parser *p)
{
	FILE *fp;
	fp=fopen("string1t.txt","r");
	char str[10000];
	if(p->system)
        fscanf(fp," %[^\n]s",str);                                  //windows vs linux crash
	else
        fscanf(fp," %[^\r]s",str);
	int curr=0;
	char c=str[curr];
	string temp;
	lab:
	temp.clear();
	while(c==' ')
	{
		curr++;
		c=str[curr];
	}
	while(c!=' '&&c!='\r'&&c!='\0'&&c!='\n')
	{
		temp.push_back(c);
		curr++;
		c=str[curr];
	}
	p->p_str.push_back(temp);
	if(c=='\r' || c=='\0' || c=='\n')
		return;
	goto lab;
}

void parse(parser *p)
{
	stack<string> st;
	st.push("$");
	st.push(p->start_symbol);
	bool fail=false;
	for(vector<string>::iterator i=p->p_str.begin();i!=p->p_str.end();i++)
    {
        vector<string>::iterator temp=find(p->terminal.begin(),p->terminal.end(),*i);
        if(temp==p->terminal.end())
        {
            cout<<"Input String Contains Invalid Token ->> "<<*i<<endl;
            fail=true;
        }
    }
    if(fail)
    {
        cout<<"Hence, Parsing Failed\nTerminating...\n";
        return;
    }
	p->p_str.push_back("$");
	int parsed=0;
	vector<string>::iterator inp_ptr=p->p_str.begin();
	cout<<"Stack\t\t\t\t\t\tINPUT\t\t\t\t\t\tACTION\n";
	while(1)
	{
	    stack<string> t_st;
	    while(!st.empty())
        {
            t_st.push(st.top());
            st.pop();
        }
        while(!t_st.empty())
        {
            st.push(t_st.top());
            cout<<t_st.top()<<" ";
            t_st.pop();
        }
        cout<<"\t\t\t\t   ->> ";
        vector<string>::iterator itt=inp_ptr;
        while(itt!=p->p_str.end())
            cout<<*itt<<" ",itt++;
        cout<<"\t\t\t\t  ->> ";
		if(st.top().compare(*inp_ptr)==0 && st.top().compare("$")==0)
		{
			cout<<"Successfull parsing\n";
			parsed=1;
			break;
		}
		else if(st.top().compare(*inp_ptr)==0)
		{
			st.pop();
			inp_ptr++;
			cout<<"Match\n";
		}
		else if(p->check_variable(st.top())==true)
		{
			vector<string>::iterator var=find(p->variable.begin(),p->variable.end(),st.top());
			int v_ind=var-p->variable.begin();
			var=find(p->terminal.begin(),p->terminal.end(),*inp_ptr);
			int t_ind=var-p->terminal.begin();
			if(p->ptable[v_ind][t_ind].size()!=1)
			{
			    if(p->ptable[v_ind][t_ind].size()==0)
				cout<<"ERROR::\nThe input string is not valid\n";
				else
                cout<<"ERROR::\nThe parsing table is ambiguous\n";
				break;
			}
			int production=p->ptable[v_ind][t_ind][0];
			st.pop();
			cout<<p->glhs[production][0]<<" -> ";
			for(int j=0;j<p->grhs[production].size();j++)       //to show in console
            {
                cout<<p->grhs[production][j]<<" ";
            }
            cout<<endl;
			for(int j=p->grhs[production].size()-1;j>=0;j--)        //to actually push in stack
			{
			    if(p->grhs[production][j].compare("null")!=0)
                    st.push(p->grhs[production][j]);
			}
		}
	}
	if(parsed==1)
        cout<<"\n\n\tRESULT: String Successfully parsed\n\n";
    else
        cout<<"\n\n\tRESULT: ERROR\n\n";
}

int main()
{
    freopen("output.txt", "w", stdout);
    cout<<"JAI GANESH JI\n";
    parser p;       //parser declaration
    //cerr<<"Enter 1 if running in Linux environment else press 0:\t";
    int choice = 0;
    //cin>>choice;
    if(choice==1)
        p.system=false;
    p.read_grammar();       //reading grammar from file
    cout<<"\t\tNo. of productions in grammar: "<<p.gsize<<endl<<endl;
    cout<<"\t\tStart Symbol->> "<<p.start_symbol<<endl<<endl;
    cout<<"_______________________________________________________________________________________\n";
    //________________________________________________________________________
    cout<<endl<<"\n\t\tGrammar:\n\n";
    for(int i=0;i<p.gsize;i++)      //print the given grammar to console
    {
        cout<<"\t\t"<<*p.glhs[i].begin()<<" -> ";
        for(vector<string>::iterator j=p.grhs[i].begin(); j!=p.grhs[i].end(); j++)
        {
            cout<<*j<<" ";
        }
        cout<<endl;
    }
    cout<<"_______________________________________________________________________________________\n";
    //_______________________________________________________________________
    /*cout<<"\nList of Variables:\n";     //print all variables(non-terminals) to console
    for(int i=0;i<p.variable.size();i++)
        cout<<p.variable[i]<<" ";
    cout<<endl<<endl;
    cout<<"List of Terminals:\n";       //print all terminal symbols to console
    for(int i=0;i<p.terminal.size();i++)
        cout<<p.terminal[i]<<" ";
    cout<<endl<<endl;*/
    //_______________________________________________________________________
    /*vector<string> chck;                //big string FIRST computation
    //chck.push_back("TP");
    chck.push_back("E");
    ans.clear();
    find_first_util(p,chck);
    if(ans.empty())
        cout<<"Invalid query for FIRST computation";
    else
    for(vector<string>::iterator j=ans.begin(); j!=ans.end(); j++)
        cout<<*j<<" ";
    cout<<endl;
    ans.clear();*/
    //_______________________________________________________________________
    populate_first(&p);                 //Pre-process the first of all variables
    cout<<"\n\tFIRST() sets of non-terminals:\n\n";
    for(int j=0;j<p.variable.size();j++)
    {
        cout<<"\tFIRST of "<<p.variable[j]<<" ->>\t\t";
        for(vector<string>::iterator it=p.pre_first[j].begin(); it!=p.pre_first[j].end(); it++)
        {
            cout<<*it<<"  ";
        }
        cout<<endl;
    }
    cout<<endl;
    cout<<"_______________________________________________________________________________________\n";
    //_______________________________________________________________________
    /*foll_sol.clear();
    foll_pro.clear();
    find_follow("D",p);
    foll_pro.push_back("D");
    for(vector<string>::iterator it=foll_sol.begin(); it!=foll_sol.end(); it++)
        cout<<*it<<" ";
    cout<<endl;
    foll_sol.clear();*/
    //_______________________________________________________________________
    populate_follow(&p);         //follow sets for each variable symbol
    cout<<"\n\tFOLLOW() sets of non-terminals:\n\n";
    for(int i=0;i<p.variable.size();i++)
    {
        cout<<"\tFollow of "<<p.variable[i]<<" ->>\t\t";
        for(vector<string>::iterator it=p.pre_follow[i].begin(); it!=p.pre_follow[i].end(); it++)
            cout<<*it<<" ";
        cout<<endl;
    }
    cout<<"_______________________________________________________________________________________\n";
    //_______________________________________________________________________
    compute_table(&p);			//construction of parsing table
    cout<<"\n\n\t\t\t\tPARSING TABLE\n\n";
    bool mul_entry=false;
    cout<<" \t";
    for(int i=0;i<p.terminal.size();i++)
        cout<<p.terminal[i]<<"  \t";
    cout<<"$\n\n";
    for(int i=0;i<p.variable.size();i++)
    {
        cout<<p.variable[i]<<"\t";
        for(int j=0;j<=p.terminal.size();j++)
        {
            if(p.ptable[i][j].size()==0)
            {
                cout<<" -----  ";
                continue;
            }
            if(p.ptable[i][j].size()>1)
            {
                mul_entry=true;
                cout<<"*";
            }
            int index=p.ptable[i][j][0];
            cout<<p.glhs[index][0]<<"->";
            for(vector<string>::iterator it=p.grhs[index].begin();it!=p.grhs[index].end();it++)
            {
                cout<<*it;
            }
            cout<<"  ";
        }
        cout<<endl<<endl;
    }
    if(mul_entry)
    {
        cout<<"\nNOTE: Parsing table has multiple entries in the same blocks which are marked by asterisk(*)\n\n";
        cout<<"This multiple entries is due to either ambiguous grammar or left recursion or Common prefixes\n\n";
        cout<<"Thus parsing of string is not possible due to ambiguity\n\nTerminating...\n\n";
        exit(1);
    }
    cout<<"_______________________________________________________________________________________\n";
	//_______________________________________________________________________
	input_parsing_string(&p);		//taking the input for string to be parsed
	cout<<"\n\tString to be Parsed:\n\n\t";
	for(vector<string>::iterator it=p.p_str.begin();it!=p.p_str.end();it++)
		cout<<*it<<" ";
	cout<<endl;
	cout<<"_______________________________________________________________________________________\n";
	//_______________________________________________________________________
	cout<<"\n\tSteps involved in Parsing:\n\n";
	parse(&p);						//parsing the string according to read grammar
	cout<<"_______________________________________________________________________________________\n";
	//_______________________________________________________________________
    return 0;
}

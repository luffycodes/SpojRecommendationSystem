// cpp code

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>

using namespace std;

#define userfiles "userfiles.txt"
#define MAXUSER 5010

struct item
{
	int total;
	int total_acc;
	int distinct_att;
	int distinct_acc;
};

typedef struct item item;

string username, line;
int userId;

map<string, item> item_set;
map<string, int> user_set;
map<string, item>::iterator it1;
map<string, int>::iterator it2;

int user_subs[MAXUSER];
int user_acc[MAXUSER];
int user_distinct_subs[MAXUSER];
int user_distinct_acc[MAXUSER];

int do_something(string signedlistname)	//handles a user's signed list completely
{
	size_t found;
	string date, pidTemp, pid, resultTemp, result;
	string userRank;

	found = signedlistname.find("_");
	userRank = signedlistname.substr(found+1);
	found = userRank.find("_");
	userRank = userRank.substr(0, found);

	userId = atoi(userRank.c_str());
	user_subs[userId] = user_acc[userId] = user_distinct_subs[userId] = user_distinct_acc[userId] = 0;

	user_set.clear();	//clearing the map to store distinct problem Id's for this user only

	bool add, att;

	int c=0;
	signedlistname = "./Data/" + signedlistname;
	ifstream signlist(signedlistname.c_str());

	if(signlist.is_open())
	{
		while(signlist.good())
		{
			getline(signlist, line);
			found=line.find("|");
			if (found!=string::npos)
			{
				c++; //removing the first 2 lines
				if(c>2) //means we have useful data now
				{
					line = line.substr(found+1);
					found=line.find("|"); //now this have id before this
					line = line.substr(found+1); //line now starts from date
					found=line.find("|");
					date =  line.substr(0,found);
					found=line.find("|");
					line = line.substr(found+1); //line now starts from problem id
					found=line.find("|");
					pidTemp =  line.substr(0,found);
					found=line.find("|");
					line = line.substr(found+1); //line now starts from problem result
					resultTemp =line.substr(0,found);

					pid = "";
					for(size_t i=0; i<pidTemp.length(); i++)
						if(pidTemp.at(i)!=' ')
							pid += pidTemp.at(i);

					//change result to remove spaces
					result = "";
					for(size_t i=0; i<resultTemp.length(); i++)
						if(resultTemp.at(i)!=' ')
							result += resultTemp.at(i);
					
					//cout << date << endl << pid << endl << result << endl;

					add = att = false;

					user_subs[userId]++;

					it2 = user_set.find(pid);
					if(it2!=user_set.end())
					{
						//this item was already seen for this particular user
						if(strcmp(result.c_str(), "AC")==0)
						{
							user_acc[userId]++;
							if((*it2).second==1)
							{
								(*it2).second = 2;	//that is this problem was not acc, but now it is
								user_distinct_acc[userId]++;
								add = true;		//add this acc user in the item list
							}
						}
					}
					else	//this problem/item is seen for the first time
					{
						user_distinct_subs[userId]++;
						att = true;
						if(strcmp(result.c_str(), "AC")==0)
						{
							user_acc[userId]++;
							user_distinct_acc[userId]++;
							user_set[pid] = 2;	//accepted
							add = true;
						}
						else
							user_set[pid] = 1;	//tried but not accepted
					}

					it1 = item_set.find(pid);
					if(it1!=item_set.end())
					{
						//item already seen
						item temp = (*it1).second;
						temp.total++;
						if(strcmp(result.c_str(), "AC")==0)
							temp.total_acc++;

						if(add)
							temp.distinct_acc++;
						if(att)
							temp.distinct_att++;
						item_set[pid] = temp;
					}
					else
					{
						//item seen first time in the item list
						item newItem;
						newItem.total=1;
						newItem.distinct_att=1;
						if(strcmp(result.c_str(), "AC")==0)
						{
							newItem.total_acc=1;
							newItem.distinct_acc = 1;
						}
						else
						{
							newItem.total_acc=0;
							newItem.distinct_acc = 0;
						}
						item_set[pid] = newItem;
					}
				}
			}
		}
	}
	else
		cout <<"cant open file for user " << signlist << "using path " << signedlistname<< endl;
	return userId;
}

int main()
{
	ifstream myfile (userfiles);
	int presentUser, ctr=0;
	if (myfile.is_open())
	{
		while ( myfile.good() )
		{
			getline (myfile,username);
			presentUser = do_something(username);
			ctr++;
			/*if(ctr==2)
				break;*/
			if(ctr%200==0)
				cout << "ctr at " << ctr << "for user " << presentUser << endl;
			/*cout << "user items for user " << username<< endl;
			cout << user_set.size() << endl;
			cout << user_subs[presentUser] << " " << user_acc[presentUser] << " " << user_distinct_acc[presentUser]<< " " << user_distinct_subs[presentUser] << endl;*/
		}
		myfile.close();
		//read all the user signed list, now print it
		ofstream user_file("user_subs.txt");
		user_file << "User rank \t Total subs \t Accepted subs \t Distinct problems \t Distinct problems accepted\n";
		for(int i= 1; i<MAXUSER; i++)
		{
			user_file << i << " " << user_subs[i] << " " << user_acc[i] << " " << user_distinct_subs[i]<< " " << user_distinct_acc[i] << endl;
		}
		user_file.close();

		ofstream item_file("item_subs.txt");
		item_file << "Problem Code \t Total submissions \t Total accepted \t Distinct user count \t Distinct user accepted count\n";
		for(it1 = item_set.begin(); it1!=item_set.end();it1++)
		{
			item temp = (*it1).second;
			item_file <<  (*it1).first << "\t" << temp.total << " " << temp.total_acc << " " << temp.distinct_att << " " << temp.distinct_acc << endl;
		}
		item_file.close();
	}
	else
	{
		cout << "cant open file userfiles.txt "<<endl;
	}
	return 0;
}

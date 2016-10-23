#include <stdio.h>
#include <iostream>
#include <string.h>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>

using namespace std;

#define itemFile "item_subs.txt"
#define MAXUSER 10

string problem;

int main()
{
	ifstream myfile (itemFile);
	ofstream small_item("small_items.txt");
	small_item << "Problem with very small submissions\n";
	small_item << "Problem Code \t Total submissions \t Total accepted \t Distinct user count \t Distinct user accepted count\n";
	int ctr=0;
	string pid;
	int totalsub, totalacc, distinctuser, distictaccuser;
	if (myfile.is_open())
	{
		while ( myfile.good() )
		{
			if(ctr==0)
			{
				getline (myfile,problem);
				ctr++;
				continue;
			}
			else
			{
				myfile >> pid;
				myfile >>  totalsub >> totalacc >> distinctuser >> distictaccuser;
				if(distinctuser<MAXUSER)
					small_item << pid << endl;
			}
		}
		myfile.close();
		small_item.close();
	}
	else
	{
		cout << "cant open file userfiles.txt "<<endl;
	}
	return 0;
}

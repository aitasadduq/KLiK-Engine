
//Reverse indexer for "Enron email dataset
//Group Members
//Muhammmad Shaharyar 
//Anas Imran Tasadduq
//Muhammad Saad Hussaini 
//Ahmad Ali Khan 

//Include All the Required  Librarie
#include <mysqlx/xdevapi.h>
#include <unordered_map>
#include <string>
#include <sstream>
#include <iostream>
#include <time.h>//Time calculations
using namespace std;


//Specifying Sql crendentials for Connection on server
const string hostname = "localhost";
const unsigned portno = 33060;
const string username = "root";
const string password = "applepie95";
const string Dname = "Search_Engine";


void ReverseIndex()//Reverse Index Function

{	//mysql session object creation
	mysqlx::Session session(hostname, portno, username, password);
	mysqlx::Schema database = session.getSchema(Dname);
	mysqlx::Table wordsTable = database.getTable("forwardindex");
	mysqlx::RowResult reverseindex = wordsTable.select("document_index").execute();
	int count = 1;
	unordered_map<string, stringstream> rowmap;// Initailizing Maps of string and stringstreams for maping words 
	stringstream document;
	
	session.sql("USE " + Dname).execute();//Using schema
	//Create tabel 
	session.sql("CREATE TABLE reverseindex(word VARCHAR(200), document_files LONGTEXT, PRIMARY KEY(word));").execute();
	for (mysqlx::Row row : reverseindex)//Iterate through reverseindex tables row by row
	{
		if (count == -1)
		{
			break;
		}
		else if (count % 100000== 0)//If 10000 limit is reached then insert in to database
		{
			string query = "INSERT INTO reverseindex(word, document_files) VALUES";//Inserting words,subject,body files
			for (auto &insertrow : rowmap)
			{
				query += "('" + insertrow.first + "','" + insertrow.second.str() +  "'),";
				
			}
			query[query.length() - 1] = ' ';
			query += "ON DUPLICATE KEY UPDATE document_files = CONCAT(document_files,VALUES(document_files),' ');";
			session.sql(query).execute();
			rowmap.clear();
		}
		
		cout << "File: " << count << endl;
		document << row[0];
		string value;
	    
		while (document >> value)//For body words
		{
			rowmap[value.substr(0,value.find(','))] << value.substr(value.find(',') + 1) << "," << count << " ";
		}
		
		document.clear();
		document.str(string());
		
		count++;
	}
	string query = "INSERT INTO reverseindex(word, document_files) VALUES";//Inserting 
	for (auto &insertrow : rowmap)
	{
		query += "('" + insertrow.first + "','" + insertrow.second.str() +  "'),";
		
	}
	query[query.length() - 1] = ' ';
	query += "ON DUPLICATE KEY UPDATE document_files = CONCAT(document_files,VALUES(document_files),' ');";
	session.close();
}

int main()
{
	clock_t start = clock();
	ReverseIndex();
	cout << double(clock() - start) / CLOCKS_PER_SEC << endl;
}

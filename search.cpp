#include <iostream>
#include <fstream>
#include <regex>//To Find The required format
#include <string>
#include <iterator>
#include <algorithm>
#include <unordered_map>
#include <boost/range.hpp>
#include <boost/filesystem.hpp>//Boost Library for Fectching files in a given directory.
#include <mysqlx/xdevapi.h> //Connect with mysql server 
#include <ctime>
#include <cmath>

using namespace std;

const string hostname = "localhost";
const unsigned portno = 33060;
const string username = "root";
const string password = "applepie95";
const string Dname = "Search_Engine";
bool resultsFound = false;

// trim from left
inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from right
inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v")
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// trim from left & right
inline std::string& trim(std::string s, const char* t = " \t\n\r\f\v")
{
    return ltrim(rtrim(s, t), t);
}

unordered_map<string, int> skippingwordsmap()
{
	ifstream file;
	file.open("skippingwords");
	string word;
	int count;
	unordered_map<string, int> skippingwords;
	while (file >> word)
	{
		skippingwords.insert({ word,count });
	}
	file.close();
	return skippingwords;
}

void queryResults(string queryString) {
	mysqlx:: Session session(hostname, portno, username, password);
	mysqlx::Schema dbp = mysqlx::Schema(session, Dname);
	session.sql("USE " + Dname).execute();
	mysqlx::RowResult reverseindex;
	string query = "SELECT * FROM reverseindex WHERE word IN (";

	map<double, pair<string, string>, greater<double>> results;

	std::stringstream queryStream(queryString);
	std::string word;
	unordered_map<string, int> skippingwords = skippingwordsmap();
	while (queryStream >> word) {
		if (skippingwords.count(word) == 0) {
			query += "'" + word + "',";
		}
	}
	query += "'');";
	reverseindex = session.sql(query).execute();
	unordered_map<string, double> docs;

	for (mysqlx::Row row : reverseindex) {
		resultsFound = true;
		stringstream riword;
		stringstream ridocuments;
		stringstream riidf;
		riword << row[0];
		riidf << row[2];
		ridocuments << row[1];
		string rivalue;
		while (ridocuments >> rivalue) {
			double tfidf = stod(rivalue.substr(rivalue.find(',') + 1)) * stod(riidf.str());
			docs[(rivalue.substr(0,rivalue.find(',')))] += tfidf;
		}
	}

	if (resultsFound) {
		query = "SELECT file_id, file_path, file_subject FROM forwardindex WHERE file_id IN (";
		int count = 0;
		for (auto &file : docs) {
			query += file.first;
			if (count < docs.size() - 1) {
				query += ", ";
			}
			count++;
		}
		query += ");";

		auto document = docs.begin();
		mysqlx::RowResult res = session.sql(query).execute();
		for (mysqlx::Row row : res) {
			stringstream row2;
			stringstream row1;
			row2 << row[2];
			row1 << row[1];
			pair<string, string> resPair (row2.str(), row1.str());
			results[document -> second] = resPair;
			++document;
		}
		
		int counter = 0;
		for (auto const& rank : results) {
			cout << trim(rank.second.first) << "-:-" << trim(rank.second.second) << "-;;-";
			if (counter == 14) {
				break;
			}
			counter++;
		}
	}
	else {
		cout << "--:-:--";
	}	
}

int main(int argc, char* argv[]) {	
	queryResults(argv[1]);
	return 0;
}
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <regex>


std::string do_replace( std::string const & in, std::string const & from, std::string const & to )
{
  return std::regex_replace( in, std::regex(from), to );
}
//https://cplusplus.com/forum/beginner/251052/
std::string trim(const std::string& line)
{
    const char* WhiteSpace = " \t\v\r\n";
    std::size_t start = line.find_first_not_of(WhiteSpace);
    std::size_t end = line.find_last_not_of(WhiteSpace);
    return start == end ? std::string() : line.substr(start, end - start + 1);
}

std::string* extract_vars;

std::vector<std::string> split(std::string line, char token) {
	line = trim(line);

	std::vector<std::string> strlist;
 
	int i = 0;
	while(line.find(token) != std::string::npos) {
		strlist.push_back(line.substr(0, line.find(token)));
		line = line.substr(line.find(token)+1, line.length());
		if(i > line.length()) break;
		i++;
	}
	
	strlist.push_back("");
	return strlist;
}

int count_newlines(std::string s) {
  int count = 0;

  for (int i = 0; i < s.size(); i++)
    if (s[i] == '\n') count++;

  return count;
}

int main(int argc, char* argv[]) {
	if(argv[1] == nullptr) {
		std::cout << "where filename??\n";
		return 1;
	}

	std::string file = "";
	std::string line;
	std::string writefile = "#include <iostream>\n\nint main() {float result = false; bool resultbool = false;\n";
	std::ofstream outfile("out.cpp");
	std::ifstream filef(argv[1]);
	int newlinec = count_newlines(file);
	int len;
	std::string token;
	std::vector<std::string> args;
	bool isVar = false;
	while(std::getline(filef, line)) {
		args.clear();
		isVar = false;
		file += line + "\n";
		line = trim(line);
		token = line.substr(0, line.find("("));

		if(token == "#p") {
			token = line.substr(line.find("(")+1, line.rfind(")")-1-line.find("("));
			token = do_replace(token, "\"", "\\\"");

			if(token.find("#g") != std::string::npos ) isVar = true;

			token = do_replace(token, "\\\\", "\\\\");
			if(isVar) {
				isVar = false;
			writefile += "std::cout << \"" + token + "\";\n";

			} else {
			writefile += "std::cout << \"" + token + "\";\n";
			}
		} else
		if(token == "#s") {
			token = line.substr(line.find("(")+1, line.find(")")-1-line.find("("));
			token = do_replace(token, "\"", "\\\"");

			token = do_replace(token, "\\\\", "\\\\");
			writefile += "std::cout << \"" + token + "\";\n";
		}

	}

	writefile+="}";

	outfile << writefile;

	

	
	return 0;
}

#include <fstream>
#include <string>
#include <stdexcept>
#include <string>
#include <vector>
#include <sstream>

#include "ASGraph.h"

inline void tokenize_line(const std::string& line, std::vector<std::string>& vec){
	std::string token;
	std::stringstream ss(line);
	for(int i = 0; i < 4; i++){
		std::getline(ss, token, '|');
		vec[i] = token;
	}
}

int ASGraph::build_graph(const std::string& filepath){
	fstream file(filepath, ios::in);

	if(!file.is_open()){
		std::cerr << "Error: Unable to open file at" << filepath << std::endl;
		return 1;
	}

	std::string cur_line;
	//0 = left as, 1 = right as, 3 = relationship, 4 = policy
	std::vector<std::string> tokens;
	tokens.reserve(4);
	
	uint32_t left_asn = 0;
	ASNode* left_node = nullptr;
	uint32_t right_asn = 0;
	ASNode* right_node = nullptr;

	while(true){
		std::getline(file, cur_line);
		tokenize_line(cur_line&, tokens&);
		
		
		if(left_node==nullptr || left_node
	}

	return 0;
}

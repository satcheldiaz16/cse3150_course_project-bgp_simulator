#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include "ASGraph.h"
#include "ASNode.h"

const uint32_t estimated_graph_size = 80000;

int main(){
	ASGraph graph = ASGraph(estimated_graph_size);

	std::string file_path; //need to make a call to whatever downloads and unzips caida file

	graph.build_graph("");

	return 0;
}

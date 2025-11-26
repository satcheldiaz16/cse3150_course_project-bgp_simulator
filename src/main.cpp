#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include "ASGraph.h"
#include "ASNode.h"

ASGraph& build_graph(std::string fp){
	uint32_t graph_size = 10; //temporary
				  
	ASGraph graph = ASGraph(graph_size);
	for(uint32_t cur_asn; cur_asn < graph_size; cur_asn++;){
		ASNode node = ASNode(cur_asn);
		//other stuff here
		graph.insert_node_at(std::move(node), cur_asn);
	}

	return graph;
}

int main(){
	ASGraph graph = build_graph();

	return 0;
}

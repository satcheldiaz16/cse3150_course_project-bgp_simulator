#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include "ASGraph.h"
#include "ASNode.h"

const uint32_t estimated_graph_size = 80000;

int main(int argc, char *argv[]){
	ASGraph graph = ASGraph(estimated_graph_size);

    if(argc < 4){
        std::cerr << "Invalid input format: needs to be ./bgp_simulator --relationships {path} --announcements {path} --rov_invalid {path}" << std::endl;
        return 1;
    }

	std::string file_path; //need to make a call to whatever downloads and unzips caida file

    int status = 0;

    status = graph.build_rov_invalid_list(argv[3]);

	status = graph.build_graph(argv[1]);

    status = graph.seed_announcements(argv[2]);

    status = graph.propogate_announcements();

    std::string output_path = "./ribs.csv";

    status = graph.output_graph(output_path);

	return status;
}

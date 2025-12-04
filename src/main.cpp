#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstring>

#include "ASGraph.h"
#include "ASNode.h"

const uint32_t estimated_graph_size = 80000;

/*
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
*/

int main(int argc, char* argv[]) {
    std::string relationships_file;
    std::string announcements_file;
    std::string rov_asns_file;
    
    // Parse command line arguments
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "--relationships") == 0 && i + 1 < argc) {
            relationships_file = argv[++i];
        }
        else if(strcmp(argv[i], "--announcements") == 0 && i + 1 < argc) {
            announcements_file = argv[++i];
        }
        else if(strcmp(argv[i], "--rov-asns") == 0 && i + 1 < argc) {
            rov_asns_file = argv[++i];
        }
        else {
            std::cerr << "Unknown argument: " << argv[i] << std::endl;
            std::cerr << "Usage: " << argv[0] 
                      << " --relationships <file> --announcements <file> --rov-asns <file>" 
                      << std::endl;
            return 1;
        }
    }
    
    // Validate required arguments
    if(relationships_file.empty() || announcements_file.empty() || rov_asns_file.empty()) {
        std::cerr << "Error: Missing required arguments" << std::endl;
        std::cerr << "Usage: " << argv[0] 
                  << " --relationships <file> --announcements <file> --rov-asns <file>" 
                  << std::endl;
        return 1;
    }
    
    std::cout << "Building graph from: " << relationships_file << std::endl;
    std::cout << "Loading announcements from: " << announcements_file << std::endl;
    std::cout << "Loading ROV ASNs from: " << rov_asns_file << std::endl;

    ASGraph graph = ASGraph(estimated_graph_size);

    int status = 0;

    // Load ROV ASNs BEFORE building graph so nodes can be created with correct policy
    status = graph.build_rov_invalid_list(rov_asns_file);
    if(status != 0) {
        std::cerr << "Error: Failed to load ROV ASNs" << std::endl;
        return 1;
    }

    status = graph.build_graph(relationships_file);
    if(status != 0) {
        std::cerr << "Error: Failed to build graph" << std::endl;
        return 1;
    }

    status = graph.seed_announcements(announcements_file);
    if(status != 0) {
        std::cerr << "Error: Failed to seed announcements" << std::endl;
        return 1;
    }

    status = graph.propogate_announcements();
    if(status != 0) {
        std::cerr << "Error: Failed to propagate announcements" << std::endl;
        return 1;
    }

    std::string output_path = "./ribs_output.csv";

    status = graph.output_graph(output_path);
    if(status != 0) {
        std::cerr << "Error: Failed to output graph" << std::endl;
        return 1;
    }

    std::cout << "Successfully generated output: " << output_path << std::endl;

	return status;
}

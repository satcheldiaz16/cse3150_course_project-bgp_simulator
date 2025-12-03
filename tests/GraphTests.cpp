#include <gtest/gtest.h>
#include <iostream>
#include <cstdlib>

#include "ASGraph.h"

class GraphTest: public ::testing::Test{
protected:
	void SetUp() override{
		graph = ASGraph(10);
	}
public:
	ASGraph graph;
};

TEST_F(GraphTest, TestInput1){
	ASGraph graph;

	graph.build_graph("../tests/test_input_1.txt");

	std::cout << graph << std::endl;
}

TEST_F(GraphTest, RealInput){
	ASGraph graph;
    int result = 0;
	//graph.build_graph("../data/20250901.as-rel2.txt");
    result = graph.build_rov_invalid_list("../bench/many/rov_asns.csv");
    std::cout << "build rov inv list= " << result << std::endl;

    result = graph.build_graph("../bench/many/CAIDAASGraphCollector_2025.10.16.txt");
    std::cout << "build graph= " << result << std::endl;

    result = graph.seed_announcements("../bench/many/anns.csv");
    std::cout << "seed announcements= " << result << std::endl;

    result = graph.propogate_announcements();
    std::cout << "propogate announcements= " << result << std::endl;

    result = graph.output_graph("../output.csv");
    std::cout << "output graph= " << result << std::endl;
	//std::cout << graph << std::endl;
    std::system("../bench/compare_output.sh ../bench/many/ribs.csv ../output.csv");
}

TEST_F(GraphTest, PrefixInput){
	ASGraph graph;
    int result = 0;
	//graph.build_graph("../data/20250901.as-rel2.txt");
    result = graph.build_rov_invalid_list("../bench/prefix/rov_asns.csv");
    std::cout << "build rov inv list= " << result << std::endl;

    result = graph.build_graph("../bench/prefix/CAIDAASGraphCollector_2025.10.16.txt");
    std::cout << "build graph= " << result << std::endl;

    result = graph.seed_announcements("../bench/prefix/anns.csv");
    std::cout << "seed announcements= " << result << std::endl;

    result = graph.propogate_announcements();
    std::cout << "propogate announcements= " << result << std::endl;

    result = graph.output_graph("../p_output.csv");
    std::cout << "output graph= " << result << std::endl;
	//std::cout << graph << std::endl;
    std::system("../bench/compare_output.sh ../bench/prefix/ribs.csv ../p_output.csv");
}

TEST_F(GraphTest, SubPrefixInput){
	ASGraph graph;
    int result = 0;
	//graph.build_graph("../data/20250901.as-rel2.txt");
    result = graph.build_rov_invalid_list("../bench/subprefix/rov_asns.csv");
    std::cout << "build rov inv list= " << result << std::endl;

    result = graph.build_graph("../bench/subprefix/CAIDAASGraphCollector_2025.10.16.txt");
    std::cout << "build graph= " << result << std::endl;

    result = graph.seed_announcements("../bench/subprefix/anns.csv");
    std::cout << "seed announcements= " << result << std::endl;

    result = graph.propogate_announcements();
    std::cout << "propogate announcements= " << result << std::endl;

    result = graph.output_graph("../sp_output.csv");
    std::cout << "output graph= " << result << std::endl;
	//std::cout << graph << std::endl;
    std::system("../bench/compare_output.sh ../bench/subprefix/ribs.csv ../sp_output.csv");
}

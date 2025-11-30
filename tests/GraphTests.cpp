#include <gtest/gtest.h>
#include <iostream>

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
/*
TEST_F(GraphTest, RealInput){
	ASGraph graph;

	graph.build_graph("../data/20250901.as-rel2.txt");

	std::cout << graph << std::endl;
}
*/

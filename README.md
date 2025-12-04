# cse3150_course_project

run program format: ./bgp_simulator --relationships ../bench/many/CAIDAASGraphCollector_2025.10.16.txt --announcements ../bench/many/anns.csv --rov-asns ../bench/many/rov_asns.csv

caida link: https://publicdata.caida.org/datasets/as-relationships/serial-2/

My ASGraph is structured as an std::unordered_map k:uint32_t v:unique_ptr to ASNode

I chose an unordered map so I could index each node with their corresponding asn, as well as
average case lookup time of O(1). However, this introduces undesireable hashing overhead.

For each ASNode class, their relationships are stored in vectors of ASNode*. I can avoid worrying about ownership and manual memory management by using the pointers only as observers of the actual nodes themselves, which exist in the unordered map.

I've done the same for my flattened graph; it also stores pointers to these ASNodes. This way, I can avoid the hashing overhead when I propogate announcements, without incuring the cost of shared pointers, or the hardship of manual memory management. Once the Graph is created, I intend on never moving it (doing so would surely break the program, given these design choices), so the location of all of the nodes is safe.

When doing research into how to check for cycles, I noticed that the cycle check and the process for flattening the graph were basically the same, even tho they were listed very far apart in the project instructions, and upon further investigation determined I could do both at once for effeciencies sake.

I've decided not to go for this, but I will leave the implementation in my code, and this documentation here just for history's sake. Starting top-down would only save me one o(V) pass, not super helpful and was producing incorrect results. - After closer inspection of the CAIDA file, I discovered the input clique info at the top of it, which according to a google search is the top most rank of the flattened graph. I can use this information to flatten the graph in reverse, and then reverse my vec of vec, which is O(67), as opposed to cycling through the graph to figure out my rank[0] layer, O(80000+). Small optimization but still.

Announcement Paths: after thinking hard on it, I don't really see how arena pattern works with diverging paths. I've decided to go with this strategy: each announcement will store a Pointer to the ASNode it currently resides at, as well as a pointer to where it just came from. The origin announcement will just store a nullptr for where it came from, this will denote it as the origin. For reading the entire path, I'll build out a recursive method that backtracks through the pointer connections. Announcements will be memory safe with the same observer pattern I used for the graph itself; a Nodes local rib store a unique ptr to the location of the announcement, and all other pointers will be observers of this original unique ptr.


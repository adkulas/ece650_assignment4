#pragma once

#include <iostream>
#include <vector>
#include <minisat/core/SolverTypes.h>
#include <minisat/core/Solver.h>

class VertexCover {
private:
    int vertices;
    int k;
    std::vector< std::pair<int,int> > edges;
    Minisat::Solver solver;

    Minisat::Var toVar(int row, int column);
    bool check_valid_input(std::vector< std::pair<int,int> > edges);
    void add_clause_at_least_one_in_cover();
    void add_clause_vertex_only_once();
    void add_clause_one_per_cover_pos();
    void add_clause_every_edge_covered();

    

public:
    VertexCover ( int v = 0 );
    
    // Accessors
    void print_vertex_cover();
    int get_vertices() const;
    bool solve();
    
    // Mutators
    void add_edges(std::vector< std::pair<int,int> > edges);

};
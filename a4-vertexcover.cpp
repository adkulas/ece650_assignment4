#include <iostream>
#include <vector>
#include <climits>
#include "a4-vertexcover.hpp"

// Constructor
VertexCover::VertexCover( int v ):
    vertices(v), k(0) {/*empty constructor*/}


// Private
Minisat::Var VertexCover::toVar(int row, int column) {
    // Map all vertices to a SAT boolean problem
    // The rows indicate vertices and columns indicate position
    // in the minimum vertex cover solution
    int columns = k;
    return row * columns + column;
}


bool VertexCover::check_valid_input(std::vector< std::pair<int,int> > edges) {
    for ( auto& e : edges) {
        if (vertices <= e.first || vertices <= e.second || e.first < 0 || e.second < 0) {
            std::cerr << "Error: Attempted to add edge to vertex that does not exist"
                      << std::endl;
            return false;
        } 
    }
    return true;
}


void VertexCover::add_clause_at_least_one_in_cover() {
    //---------------------------------------
    // At least one vertex is the ith vertex in the vertex cover
    //---------------------------------------

    for (int i = 0; i < k; i++) {
        Minisat::vec<Minisat::Lit> clause;
        for (int n = 0; n < vertices; n++) {
            clause.push(Minisat::mkLit(toVar(n, i)));
        }
        solver.addClause(clause);
    }
}


void VertexCover::add_clause_vertex_only_once() {
    //----------------------------------------
    // No more than one vertex in the ith position of the vertex cover
    //----------------------------------------

    for (int i = 0; i < vertices; i++) {   
        for (int q = 0; q < k; q++) {
            for (int p = 0; p < q; p++) {
                solver.addClause( ~Minisat::mkLit(toVar(i,p)), ~Minisat::mkLit(toVar(i,q)));  
            }   
        }        
    }
}


void VertexCover::add_clause_one_per_cover_pos() {
    //----------------------------------------
    // No more than one vertex in the ith position of the vertex cover
    //----------------------------------------
    
    for (int i = 0; i < k; i++) {   
        for (int q = 0; q < vertices; q++) {
            for (int p = 0; p < q; p++) {
                solver.addClause( ~Minisat::mkLit(toVar(p,i)), ~Minisat::mkLit(toVar(q,i)));  
            }   
        }        
    }
}


void VertexCover::add_clause_every_edge_covered() {
    //----------------------------------------
    // Every edge is incident to at least one vertex
    //----------------------------------------
    for ( auto& e : edges) {
        Minisat::vec<Minisat::Lit> literals; 
        for (int k = 0; k < columns; k++) {
            literals.push(Minisat::mkLit(toVar(e.first, k)));
            literals.push(Minisat::mkLit(toVar(e.second, k)));
            }
        solver.addClause(literals);
        }
}


// Mutators
void VertexCover::add_edges(std::vector< std::pair<int,int> > e) {
    if ( check_valid_input(e) ) {
        edges = e;
    }
    return;
}


// Accessors
int VertexCover::get_vertices() const {
    return vertices;
}


bool VertexCover::solve() {
    std::cout << "Hello" << std::endl;
}


void VertexCover::print_vertex_cover() {
    std::cout << "Hello" << std::endl;
}
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include "a4-vertexcover.hpp"
#include <minisat/core/SolverTypes.h>
#include <minisat/core/Solver.h>

// Constructor
VertexCover::VertexCover( int v ):
    vertices(v) {/*empty constructor*/}


// Private
Minisat::Var VertexCover::toVar(int row, int column, int k) {
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


void VertexCover::add_clause_at_least_one_in_cover(Minisat::Solver& solver, int k) {
    //---------------------------------------
    // At least one vertex is the ith vertex in the vertex cover
    //---------------------------------------

    for (int i = 0; i < k; i++) {
        Minisat::vec<Minisat::Lit> clause;
        for (int n = 0; n < vertices; n++) {
            clause.push(Minisat::mkLit(toVar(n,i,k)));
        }
        solver.addClause(clause);
    }
}


void VertexCover::add_clause_vertex_only_once(Minisat::Solver& solver, int k) {
    //----------------------------------------
    // No more than one vertex in the ith position of the vertex cover
    //----------------------------------------

    for (int i = 0; i < vertices; i++) {   
        for (int q = 0; q < k; q++) {
            for (int p = 0; p < q; p++) {
                solver.addClause( ~Minisat::mkLit(toVar(i,p,k)), ~Minisat::mkLit(toVar(i,q,k)));  
            }   
        }        
    }
}


void VertexCover::add_clause_one_per_cover_pos(Minisat::Solver& solver, int k) {
    //----------------------------------------
    // No more than one vertex in the ith position of the vertex cover
    //----------------------------------------
    
    for (int i = 0; i < k; i++) {   
        for (int q = 0; q < vertices; q++) {
            for (int p = 0; p < q; p++) {
                solver.addClause( ~Minisat::mkLit(toVar(p,i,k)), ~Minisat::mkLit(toVar(q,i,k)));  
            }   
        }        
    }
}


void VertexCover::add_clause_every_edge_covered(Minisat::Solver& solver, int k) {
    //----------------------------------------
    // Every edge is incident to at least one vertex
    //----------------------------------------
    for ( auto& e : edges) {
        Minisat::vec<Minisat::Lit> literals;
        for (int i = 0; i < k; i++) {
            literals.push(Minisat::mkLit(toVar(e.first, i, k)));
            literals.push(Minisat::mkLit(toVar(e.second, i, k)));
            }
        solver.addClause(literals);
        }
}


bool VertexCover::solve(Minisat::Solver& solver, int k) {
    // init_variables();
    for (int r = 0; r < vertices; r++) {
        for (int c = 0; c < k; c++) {
            auto var = solver.newVar();
            // std::cout << "r: " << r << " c: " << c << " var: " << var << std::endl;
        }
    }

    // add clauses
    add_clause_at_least_one_in_cover(solver, k);
    add_clause_vertex_only_once(solver, k);
    add_clause_one_per_cover_pos(solver, k);
    add_clause_every_edge_covered(solver, k);
    // std::cout << solver.nClauses() << std::endl;

    auto sat = solver.solve();

    if (sat) {
    std::cout << "K: " <<  k  << " SAT: ";
        for (int r = 0; r < vertices; r++) {
            for (int c = 0; c < k; c++) {
                if (solver.modelValue(toVar(r,c,k)) == Minisat::l_True) {
                    std::cout << r << " ";
                }
            }      
        }
        std::cout << std::endl;
        return true;

    } else {
        std::cout << "K: " << k << " UNSAT" << std::endl;
        return false;
    }
}


// Accessors
int VertexCover::get_vertices() const {
    return vertices;
}


// Mutators
void VertexCover::add_edges(std::vector< std::pair<int,int> > e) {
    if ( check_valid_input(e) ) {
        edges = e;
    }
    return;
}


void VertexCover::find_minimum() {
    
    int low = 0;
    int high = vertices;
    int mid;
    
    //-1 is UNSAT, 1 is SAT, 0 is undefined
    int results[vertices];
    std::fill_n(results, vertices, -1);

    while (low <= high) {
        mid = (high-low)/2;

        Minisat::Solver solver;
        results[mid] = solve(solver, mid);
        if (results[mid]) {
            high = mid - 1;
        } 
        else {
            low = mid + 1;
        }
    }

    for (int i = 0; i <= vertices+1; i++) {
        Minisat::Solver solver;
        solve(solver, i);
    }
}
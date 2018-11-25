// main.cpp:
#include <minisat/core/SolverTypes.h>
#include <minisat/core/Solver.h>
#include <vector>
#include <chrono>
#include <iostream>
#include "a4-vertexcover.hpp"

// void init_variables() {
//     for (int r = 0; r < rows; r++) {
//         for (int c = 0; c < columns; c++) {
//             auto var = solver.newVar();
//             std::cout << "r: " << r << " c: " << c << " var: " << var << std::endl;
//         }
//     }
// }



Minisat::Var toVar(int row, int column) {
    int columns = 2;
    return row * columns + column;
}

int main() {
    
    const int rows = 5;      // this is i or |v| or n
    const int columns = 2;   // this is k or vertex cover length
    Minisat::Solver solver;
    std::vector< std::pair<int,int> > edges{ std::make_pair(0,4), 
                                            std::make_pair(4,1),
                                            std::make_pair(0,3),
                                            std::make_pair(3,4),
                                            std::make_pair(3,2),
                                            std::make_pair(1,3)
                                            };




    // Create variables
    // init_variables();
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            auto var = solver.newVar();
            std::cout << "r: " << r << " c: " << c << " var: " << var << std::endl;
        }
    }

    // Create the clauses
    
    //=============================================================
    //    At least one vertex is the ithe vertex in the vertex cover
    //=============================================================
    for (int i = 0; i < columns; i++) {
        Minisat::vec<Minisat::Lit> clause;
        for (int n = 0; n < rows; n++) {
            clause.push(Minisat::mkLit(toVar(n, i)));
        }
        solver.addClause(clause);
    }
    std::cout << "num clauses= " << solver.nClauses() << std::endl;

    //=============================================================
    //    No vertex can appear twice in vertex cover
    //=============================================================
    for (int i = 0; i < rows; i++) {   
        for (int q = 0; q < columns; q++) {
            for (int p = 0; p < q; p++) {
                solver.addClause( ~Minisat::mkLit(toVar(i,p)), ~Minisat::mkLit(toVar(i,q)));  
                // std::cout << "q: " << q << " p: " << p << std::endl;
            }   
        }        
    }
    std::cout << "num clauses= " << solver.nClauses() << std::endl;

    //=============================================================
    //    No more than one vertex in the ith position of the vertex cover
    //=============================================================
    for (int i = 0; i < columns; i++) {   
        for (int q = 0; q < rows; q++) {
            for (int p = 0; p < q; p++) {
                solver.addClause( ~Minisat::mkLit(toVar(p,i)), ~Minisat::mkLit(toVar(q,i)));  
                // std::cout << "q: " << q << " p: " << p << std::endl;
            }   
        }        
    }
    std::cout << "num clauses= " << solver.nClauses() << std::endl;

    //=============================================================
    //    Every edge is incident to at least one vertex
    //=============================================================
    for ( auto& e : edges) {
        Minisat::vec<Minisat::Lit> literals; 
        for (int k = 0; k < columns; k++) {
            literals.push(Minisat::mkLit(toVar(e.first, k)));
            literals.push(Minisat::mkLit(toVar(e.second, k)));
            }
        solver.addClause(literals);
        }
    std::cout << "num clauses= " << solver.nClauses() << std::endl;

    auto start = std::chrono::system_clock::now();
    std::cout << "start timer" << std::endl;


    auto sat = solver.solve();

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end-start;
    std::cout << diff.count() << std::endl;


    if (sat) {
        std::cout << "SAT\n" << "Model Found:" << std::endl;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                std::cout << (solver.modelValue(toVar(r,c)) == Minisat::l_True) << " ";
            }
            std::cout << std::endl;
        }

    } else {
        std::cout << "UNSAT" << std::endl;
    }   

    
    return 0;
}
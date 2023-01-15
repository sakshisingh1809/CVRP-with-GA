/**
 * @file main.cpp
 * @author Sakshi Singh
 * @brief The main function that sets up the problem and runs the solution
 * algorithms
 */

#include <iostream>

#include "cvrp/genetic_algorithm.hpp"
#include "cvrp/greedy.hpp"
#include "cvrp/local_search_inter_intra.hpp"
#include "cvrp/local_search_intra.hpp"
#include "cvrp/simulated_annealing.hpp"
#include "cvrp/tabu_search.hpp"
#include <random>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <filesystem>
#include <set>
#include <sstream>
#include <tuple>

namespace fs = std::filesystem;
using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

std::tuple<std::vector<float>, std::vector<float>, std::vector<float> > parsedata(std::vector<std::string> list)
{ 
  std::string file = "data.csv";
  std::vector<float> x, y, demand;
  
  for (auto& row : list){
    if (row.find(file) != std::string::npos){
      std::ifstream data(row);
      std::string line;
      while(getline(data, line))
      {
          std::string val;                     /* string to hold value */
          std::vector<float> rows;                /* vector for row of values */
          std::stringstream s (line);          /* stringstream to parse csv */
          while (getline (s, val, ','))   /* for each value */
              rows.push_back(std::stof(val));  /* convert to int, add to row */
          x.push_back(rows[0]);
          y.push_back(rows[1]);
          demand.push_back(rows[2]);
      }
      data.close();
    }
  }
  return {x, y, demand}; 
}

std::tuple<int, int, int > parseinfo(std::vector<std::string> list)
{ 
  std::string file = "info.csv";
  int noc, nov, capacity;
  std::vector<int> v; 
  
  for (auto& row : list){
    if (row.find(file) != std::string::npos){
      std::ifstream data(row);
      std::string line;
      while(getline(data, line)){
        std::string val;   /* string to hold value */
        while (getline (data, val, ','))   /* for each value */
          v.push_back(std::stoi(val));  /* convert to int, add to row */
      }
      noc = (v[0]);
      nov = (v[1]);
      capacity = (v[2]);
      data.close();
    }
  }
  return {noc, nov, capacity}; 
}

std::vector<std::string> listOfFilesSubdir(std::string directory) {
  std::vector<std::string> list; //list of all files in a subdirectory
  for (auto it= fs::recursive_directory_iterator(directory); it!= fs::recursive_directory_iterator(); ++it) { 
    if( it->path().filename() == "info.csv\\ga_output.txt"){
      it.pop(); //ignore output file
    }
    list.push_back(it->path()); 
  }
  return list;
}


int main(int /* argc */, char** /* argv[] */) {
  std::string directory = "/Users/sakshisingh/Desktop/vrp/cvrp/data/training"; 
  std::string subdir;
  
  for (const auto& dirEntry : recursive_directory_iterator(directory)) {
    subdir = dirEntry.path();
    std::vector<std::string> listSubdir = listOfFilesSubdir(directory);
    auto [x, y, demand] = parsedata(listSubdir);
    auto [noc, nov, capacity] = parseinfo(listSubdir);
    nov = (nov+(nov/2)); //extra padding in case vehicles don't satisfy the needs of customers

    if (dirEntry.path().filename() == "info.csv"){
      
      Problem p(x, y, demand, noc, nov, capacity, "uniform");
      std::cout << "\n______________INSTANCE "  << subdir << "(cust:" << noc << ", vehicle:"<<nov<< ", capacity:"<< capacity << ")______________";
      constexpr int n_chromosomes = 20;
      constexpr int generations = 20;
      GASolution vrp_ga(p, n_chromosomes, generations);
      vrp_ga.Solve();
      vrp_ga.PrintSolution("route", dirEntry.path().parent_path().relative_path(), generations);
      std::cout << '\n';
    }
  }   
  return 0;
}

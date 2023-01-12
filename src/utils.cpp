/**
 * @file utils.cpp
 * @author vss2sn
 * @brief Contains the structs, classes and functions used for the set up of the
 * problem aand solution as well as some functions that aid in debugging.
 */

#include "cvrp/utils.hpp"

#include <algorithm>
#include <iostream>
#include <limits>
#include <random>
#include <tuple>
#include <utility>
#include <fstream>

std::ostream &operator<<(std::ostream &os, const Node &node) {
  os << "Node Status" << '\n';
  os << "ID    : " << node.id_ << '\n';
  os << "X     : " << node.x_ << '\n';
  os << "Y     : " << node.y_ << '\n';
  os << "Demand: " << node.demand_ << '\n';
  os << '\n';
  return os;
}

void Vehicle::CalculateCost(
  const std::vector<std::vector<double>> &distanceMatrix) {
  cost_ = 0;
  for (size_t i = 0; i < nodes_.size() - 1; i++) {
    cost_ += distanceMatrix[nodes_[i]][nodes_[i + 1]];
  }
}

std::ostream &operator<<(std::ostream &os, const Vehicle &v) {
  os << "Vehicle Status" << '\n';
  os << "Cost    : " << v.cost_ << '\n';
  os << "ID      : " << v.id_ << '\n';
  os << "Load    : " << v.load_ << '\n';
  os << "Capacity: " << v.capacity_ << '\n';
  os << "Path    : ";
  // the nodes_.size()-1 limit is only added to ensure that there isnt a --->
  // after the last node, which is always the depot, ie node 0.
  for (size_t i = 0; i < v.nodes_.size() - 1; ++i) {
    os << v.nodes_[i] << " ---> ";
  }
  os << "0";
  os << '\n' << '\n';
  return os;
}

void PrintVehicleRoute(const Vehicle &v) {
  std::cout <<"(";
  for (size_t i = 0; i < v.nodes_.size() - 1; ++i) {
    std::cout << v.nodes_[i] << "->";
    
  }
  std::cout << "0)";
  std::cout << '\n';
}

Solution::Solution(std::vector<Node> nodes,
                   const std::vector<Vehicle> &vehicles,
                   std::vector<std::vector<double>> distanceMatrix)
    : nodes_(std::move(nodes)),
      vehicles_(vehicles),
      distanceMatrix_(std::move(distanceMatrix)) {
  depot_ = nodes_[0];
  capacity_ = vehicles[0].load_;
}

Solution::Solution(const Problem &p)
    : nodes_(p.nodes_),
      vehicles_(p.vehicles_),
      distanceMatrix_(p.distanceMatrix_),
      capacity_(p.capacity_) {
  depot_ = nodes_[0];
}

void Solution::CreateInitialSolution() {
  for (auto &v : vehicles_) {
    while (true) {
      const auto [found, closest_node] = find_closest(v);
      if (found && v.load_ - closest_node.demand_ >= 0) {  // }.2*capacity){
        v.load_ -= closest_node.demand_;
        v.cost_ += distanceMatrix_[v.nodes_.back()][closest_node.id_];
        v.nodes_.push_back(closest_node.id_);
        nodes_[closest_node.id_].is_routed_ = true;
      } else {
        v.cost_ += distanceMatrix_[v.nodes_.back()][depot_.id_];
        v.nodes_.push_back(depot_.id_);
        break;
      }
    }
  }
}

std::tuple<bool, Node> Solution::find_closest(const Vehicle &v) const {
  double cost = std::numeric_limits<double>::max();
  size_t id = 0;
  bool found = false;
  for (size_t j = 0; j < distanceMatrix_[0].size(); j++) {
    if (!nodes_[j].is_routed_ && nodes_[j].demand_ <= v.load_ &&
        distanceMatrix_[v.nodes_.back()][j] < cost) {
      cost = distanceMatrix_[v.nodes_.back()][j];
      id = j;
      found = true;
    }
  }
  if (found) {
    return {true, nodes_[id]};
  }
  return {false, Node()};
}

bool Solution::CheckSolutionValid() const {
  // double cost = 0;
  std::vector<bool> check_nodes(nodes_.size(), false);
  check_nodes[0] = true;
  for (const auto &v : vehicles_) {
    int load = capacity_;
    for (const auto &n : v.nodes_) {
      load -= nodes_[n].demand_;
      check_nodes[n] = true;
    }
    if (load < 0) {
      return false;
    }
  }
  return std::all_of(std::begin(check_nodes), std::end(check_nodes),
                     [](const bool b) { return b; });
}


Problem::Problem(std::vector<float> xc,
                 std::vector<float> yc, std::vector<float> demandc,
                 const int noc, const int nov, const int capacity, std::string distribution) {
  this->capacity_ = capacity;

  if (distribution != "uniform" && distribution != "cluster") {
    distribution = "uniform";
  }
  for (int i = 0; i <= noc; ++i) {
    if (i==0) {
      nodes_.emplace_back(xc[i], yc[i], i, demandc[i], true);
    }
    else {
      nodes_.emplace_back(xc[i], yc[i], i, demandc[i], false);
    }
  }
  
  std::vector<double> tmp(nodes_.size());
  for (size_t i = 0; i < nodes_.size(); ++i) {
    distanceMatrix_.push_back(tmp);
  }
  for (size_t i = 0; i < nodes_.size(); ++i) {
    for (size_t j = i; j < nodes_.size(); ++j) {
      distanceMatrix_[i][j] = sqrt(pow((nodes_[i].x_ - nodes_[j].x_), 2) +
                                   pow((nodes_[i].y_ - nodes_[j].y_), 2));
      distanceMatrix_[j][i] = distanceMatrix_[i][j];
    }
  }

  int load = capacity_;
  for (int i = 0; i < nov; ++i) {
    vehicles_.emplace_back(i+1, load, capacity_);
    vehicles_[i].nodes_.push_back(0);
  }
  /* std::cout << "nodes:demand \n " ;
  for (size_t i = 0; i < nodes_.size(); ++i) {
    std::cout << nodes_[i].id_ << ":" <<nodes_[i].demand_ << " "; 
  }
  std::cout <<"\n\nDistance matrix\n";
  for (size_t i = 0; i < distanceMatrix_.size(); ++i) {
    for (size_t j = i; j < distanceMatrix_.size(); ++j) {
      std::cout << distanceMatrix_[i][j] << " "; 
      } 
    std::cout << "\n"; 
    }

  std::cout <<"\n"; */
  //std::cout << std::endl << nodes_.size();
}

void Solution::PrintSolution(const std::string &option, std::string dir, const int gen) const {
  std::ofstream myfileoutput;
  std::ofstream myfilesolutions;
  std::string path = dir.substr(dir.find_last_of("/\\") + 1);
  std::string solver = "GA";

  myfileoutput.open ("solutionh1.csv", std::ios_base::app);
  myfilesolutions.open ("outputnewh1.txt", std::ios_base::app);
  myfilesolutions << path << " ; ";

  double total_cost = 0;
  int optimality = 0;
  double vehicles = 0;
  double avg_dist_btw_routes = 0;
  double avg_dist_btw_cust_depot = 0;
  double longest_dist = 0;
  double depth = 0;
  double variance = 0;

  bool valid = CheckSolutionValid();
  if (valid && gen >= 1000){
    optimality = 1;
  }
  myfilesolutions << optimality << " ; ";
  //std::cout<< "Solution: \n";
  for (const auto &v : vehicles_) {
    total_cost += v.cost_;
    double mean = 0;
    double avg_cust_depot = 0;
    double max_dist = 0;
    double max_depth = 0;
    double vari = 0;
    if (option == "status") {
      PrintVehicleRoute(v);
    } else if (option == "route") {
        if (v.nodes_.size() != 1) {
          vehicles +=1;
          std::cout << "Vehicle: " << v.id_ << " | ";
          myfilesolutions << "v" <<v.id_ << ":";
          myfilesolutions << "(";
          for (size_t i = 0; i < v.nodes_.size() - 1; ++i) {
            std::cout << v.nodes_[i] << "->";
            //std::cout << distanceMatrix_[v.nodes_[i]][v.nodes_[i+1]]<< " ";
            myfilesolutions << v.nodes_[i] << ",";
            mean += distanceMatrix_[v.nodes_[i]][v.nodes_[i+1]];
            avg_cust_depot += distanceMatrix_[0][v.nodes_[i]];
            vari += (i - v.nodes_.size())*(i - v.nodes_.size());
            if (distanceMatrix_[v.nodes_[i]][v.nodes_[i+1]] > max_dist){
              max_dist = distanceMatrix_[v.nodes_[i]][v.nodes_[i+1]];
            }
            if (distanceMatrix_[0][v.nodes_[i]] > max_depth){
              max_depth = distanceMatrix_[0][v.nodes_[i]];
            }
            
          }
          myfilesolutions << "0) ";
          std::cout << "0 \n";
          avg_dist_btw_routes += mean/(v.nodes_.size()-1);
          vari /= (v.nodes_.size());
          avg_dist_btw_cust_depot += avg_cust_depot/(v.nodes_.size()-1);


          //std::cout << mean << " "<< vari << std::endl;
          //std::cout << avg_dist_btw_cust << std::endl;
          //PrintVehicleRoute(v);
        }
    }
    if (max_dist > longest_dist){
      longest_dist = max_dist;
    }
    if (max_depth > depth){
      depth = max_depth;
    }
    variance += vari;
  }
    
  myfilesolutions << '\n';
  avg_dist_btw_routes = avg_dist_btw_routes/vehicles;
  depth = longest_dist/vehicles;
  variance = variance/vehicles;
  
  if (!valid) {
    for (const auto &i : nodes_) {
      if (!i.is_routed_) {
        std::cout << "Unreached node: " << '\n';
        std::cout << i << '\n';
      }
    }
  } 

  /* std::cout << "\nInstance name: " << path;
  std::cout << "\nSolver: " << solver;
  std::cout << "\nAverage distance between depot to directly connected customers: " << avg_dist_btw_cust_depot;
  std::cout << "\nAverage distance between routes: " << avg_dist_btw_routes;
  std::cout << "\nVariance in number of routes: " << variance;
  std::cout << "\nAverage depth per route: " << depth;
  std::cout << "\nLongest distance between two connected customers, per route: "<< longest_dist;
  std::cout << "\nNumber of routes: " << vehicles;
  std::cout << "\nTotal cost: " << total_cost;
  std::cout << "\nOptimality: " << optimality;
   */
  myfileoutput << path<<","<<solver << "," << avg_dist_btw_cust_depot << "," << avg_dist_btw_routes << "," << variance << "," << longest_dist << "," << depth << "," << vehicles << "," << total_cost << "," << optimality << "\n";
  myfilesolutions.close();
  myfileoutput.close();
}

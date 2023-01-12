/**
 * @file greedy.cpp
 * @author vss2sn
 * @brief Contains the GreedySolution class
 */

#include "cvrp/greedy.hpp"

#include <iostream>
#include <numeric>
#include <fstream>
#include <iomanip>
#include <ctime>

GreedySolution::GreedySolution(
    const std::vector<Node>& nodes, const std::vector<Vehicle>& vehicles,
    const std::vector<std::vector<double>>& distanceMatrix)
    : Solution(nodes, vehicles, distanceMatrix) {}

GreedySolution::GreedySolution(const Problem& p)
    : Solution(p.nodes_, p.vehicles_, p.distanceMatrix_) {}

void GreedySolution::Solve() {
  for (auto& v : vehicles_) {
    while (true) {
      const auto [found, closest_node] = find_closest(v);
      if (found && v.load_ - closest_node.demand_ >= 0) {
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

  double cost = std::accumulate(
      std::begin(vehicles_), std::end(vehicles_), 0.0,
      [](const double sum, const Vehicle& v) { return sum + v.cost_; });
  
  for (const auto& i : nodes_) {
    if (!i.is_routed_) {
      std::cout << "\t Unreached node: ";
      std::cout << i << '\n';
    }
  }
  
}

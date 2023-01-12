/**
 * @file utils.hpp
 * @author vss2sn
 * @brief Contains the structs, structes and functions used for the set up of
 * the problem aand solution as well as some functions that aid in debugging.
 */
#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <tuple>
#include <vector>
/**
 * @brief struct node
 * @details Contains the x, y coordinates of the location of the node, its id,
 * its demand, and whether it has been added to the routes of any of the
 * vehicles
 */
struct Node {
 public:
  float x_, y_, id_, demand_;
  bool is_routed_;

  /**
   * @brief Consructor
   * @param x x coordinate
   * @param y y coordinate
   * @param id node id
   * @param demand node demand
   * @param is_routed has the node been included in a route
   * @return no return parameter
   * @details Constructor for a node
   */
  Node(const float x = 0, const float y = 0, const int id = 0, const int demand = 0,
       const bool is_routed = true)
      : x_(x), y_(y), id_(id), demand_(demand), is_routed_(is_routed) {}

  friend std::ostream &operator<<(std::ostream &os, const Node &node);
};

/**
 * @brief Overloads the << operator to print the data in the Node struct
 * @param [in] os ostream to which the printing is to be done
 * @param [in] node node who's status is to be printed
 * @return ostream
 */
std::ostream &operator<<(std::ostream &os, const Node &node);

struct Vehicle {
 public:
  int id_, load_, capacity_;
  double cost_ = 0;
  std::vector<int> nodes_;

  /**
   * @brief Constructor
   * @param id Vehicle id
   * @param load Current vehicle load
   * @param capacity Maximum vehicle capacity (initial load)
   * @return no return value
   * @details Constructor of vehicle struct
   */
  Vehicle(const int id = 0, const int load = 0, const int capacity = 0)
      : id_(id), load_(load), capacity_(capacity) {}

  friend std::ostream &operator<<(std::ostream &os, const Vehicle &v);

  /**
   * @brief Calculates cost of the visiting the nodes in order
   * @param distanceMatrix Holds the distances between each pair of nodes
   * @return void
   * @details Calculates cost of the route nd updates the cost variable
   */
  void CalculateCost(const std::vector<std::vector<double>> &distanceMatrix);
};

/**
 * @brief Overloads the << operator to print the data in the Vehicle struct
 * @param [in] os ostream to which the printing is to be done
 * @param [in] v vehicle who's status is to be printed
 * @return ostream
 */
std::ostream &operator<<(std::ostream &os, const Vehicle &v);

/**
 * @brief Prints the vehicle route
 * @param [in] v vehicle who's route is to be printed
 * @return void
 */
void PrintVehicleRoute(const Vehicle &v);

struct Problem {
 public:
  /**
   * @brief Constructor
   * @param xc X coordinates array
   * @param yc Y coordinates array
   * @param demandc demand array
   * @param noc number of nodes (centres/dropoff points)
   * @param nov number of vehicles
   * @param capacity maximum capacityof each vehicle
   * @param distribution distribution of nodes. Can be either clustered or
   * uniform.
   * @return no return value
   * @details Constructor for problem struct
   */
  Problem(const std::vector<float> xc, 
          const std::vector<float> yc, 
          const std::vector<float> demandc,
          const int noc = 1000, const int nov = 50,
          const int capacity = 800, std::string distribution = "uniform");

  std::vector<Node> nodes_;
  std::vector<Vehicle> vehicles_;
  std::vector<std::vector<double>> distanceMatrix_;
  Node depot_;
  int capacity_;
};

// Solution class should not call problems's constructor so not inheriting.
class Solution {
 public:
  /**
   * @brief Constructor
   * @param nodes Vector of all nodes
   * @param vehicles Vector of vehicles
   * @param distanceMatrix Matrix containing distance between each pair of nodes
   * @return no return type
   * @details Constructor for solution struct
   */
  Solution(std::vector<Node> nodes, const std::vector<Vehicle> &vehicles,
           std::vector<std::vector<double>> distanceMatrix);

  /**
   * @brief Constructor
   * @param p Instance of Problem struct defining the problem parameters
   * @return no return type
   * @details Constructor for solution struct
   */
  explicit Solution(const Problem &p);

  /**
   * @brief Copy constructor
   * @param s object to be copied
   * @return no return value
   */
  Solution(const Solution &s) = default;

  /**
   * @brief Copy assignment
   * @param s object to be copied
   * @return solution object
   */
  Solution &operator=(const Solution &s) = default;

  /**
   * @brief Move constructor
   * @param s object to be moved
   * @return no return value
   */
  Solution(Solution &&s) = default;

  /**
   * @brief Move assignment
   * @param s object to be moved
   * @return solution object
   */
  Solution &operator=(Solution &&s) = default;

  /**
   * @brief Destructor
   * @return no return value
   */
  virtual ~Solution() = default;

  /**
   * @brief Creates initial solution
   * @return void
   * @details Creates initial solution, currently set to greedy.
   */
  void CreateInitialSolution();

  /**
   * @brief Check whether the solution is valid
   * @return bool True is solution is valid
   * @details Check whether the solution is valid by checking whether all the
   * nodes are reached and whether the total edmand of all the nodes on any of
   * the routes exceed the capacity of the respective vehicles
   */
  bool CheckSolutionValid() const;

  //std::vector<double> Solution::GetDistances(const Node &n) const;
  
  /**
   * @brief Virtual function overloaded by solution structes to solve the given
   * problem.
   * @return void
   * @details Virtual function overloaded by solution struct to solve the
   * given problem.
   */
  virtual void Solve() = 0;

  /**
   * @brief find closest node
   * @param v vehicle under consideration
   * @return tuple containing bool as to whether a Node was found and the
   * nearest node
   * @details Finds the node nearest to the last node in the route of the
   * vehicle under constideration that has not been routed already
   */
  std::tuple<bool, Node> find_closest(const Vehicle &v) const;

  /**
   * @brief Prints the status of solution
   * @param option allows the option to print routes or vehicle statuses
   * @param path path of the instance being solved
   * @param gen number of generations to set the optimality
   * @return void
   * @details Prints the solution status including cost, the vehicle status, and
   * solution validity
   */
  void PrintSolution(const std::string &option = "", std::string path="", const int gen=100) const;

  std::vector<Node> GetNodes() const { return nodes_; }

  std::vector<Vehicle> GetVehicles() const { return vehicles_; }
protected:
  std::vector<Node> nodes_;
  std::vector<Vehicle> vehicles_;

 protected:
  std::vector<std::vector<double>> distanceMatrix_;
  Node depot_;
  int capacity_;
};

#endif  // UTILS_HPP

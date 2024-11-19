# Capacitated Vehicle Routing Problem with Machine Learning #

This repository is the part 2 for ### CVRP-GA ### . 

### This repository contains algorithms to solve the CVRP (Capacitated Vehicle Routing Problem) in C++ and incorporating Machine Learning (ML) from python ###


<a name="algorithms"></a>
#### Algorithms: ####
1. Genetic Algorithm (GA)
2. Genetic Algorithm (GA) - Machine Learning (ML)

<a name="instructions"></a>
#### To build and run: ####
    git clone https://github.com/sakshisingh1809/CVRP-with-GA.git  
    cd cvrp
    mkdir build  
    cd build
    cmake .. && make -j4
    ./cvrp  

<a name="toc"></a>
#### Table of contents: ####
- [Algorithms](#algorithms)
- [Instructions](#instructions)
- [Table of contents](#toc)
- [Code Overview](#overview)
- [Notes](#notes)

<a name="overview"></a>
#### Code Overview: ####
1. The code contains `Problem`, `Solution` and `Vehicle` classes. Each algorithm implementation has its own class and inherits the `Solution` class.
2. The problem is setup using the `Problem` class which specifies the number of nodes (centres/dropoff points), maximum demand, number of vehicles, their capacity, the grid range and the type of distribution. The demand for each centre as well as its location is randomly generated.
3. A base class called `Solution` has been created to store the basic elements of the solution in a user friendly format. This includes a `vector` of instances of the `Vehicle` class.
4. The `Vehicle` class stores the vehicle id, the route it takes, the total capacity, the number of units still left in the vehicle, and the cost associated with the vehicle's route. The `<<` operator is overloaded to show the status of the node and vehicle respectively. `PrintVehicleRoute()` prints only the route of the vehicle.
5. The `Solution` class also contains a virtual method called `Solve()`. Each algorithm class overrides the `Solve()` method.
6. The `Solution` class also contains a method called `PrintSolution(option)` with the an input option (`option`) to print vehicles' statuses or routes in addition to the total cost and validity of the solution.


<a name="notes"></a>
#### Notes: ####
1. The documentation for private functions (such as operators in the `GASolution` class) has been made available to aid understanding.
2. Custom hybrid algorithms, that involve feeding in the solution of 1 algorithm to another can easily be implemented, as the structure allows the extraction of solution from the algorithm classes. An example is shown at the end of `main.cpp`.


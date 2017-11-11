#include "Neuron.hpp"
#include "Simulation.hpp"
#include <iostream>
#include <cmath>
#include <fstream>

int main()
{
	//uncomment the simulate one specific case
	Simulation sim;
	//sim.oneNeuronSimulation();
	//sim.twoNeruonsSimulation();
	sim.networkSimulation(5,2);
	//sim.plotGraph_A();
	//sim.plotGraph_B();
	//sim.plotGraph_C();
	//sim.plotGraph_D();
	std::cout << "Simulation done" << std::endl;
	return 0;
}

	

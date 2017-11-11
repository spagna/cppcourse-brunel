#include "Simulation.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>


Simulation::Simulation()
{}
void Simulation::oneNeuronSimulation()
{
	Neuron n(true); 
	
	//open a file to write down the values of the membrane potential at each time step
	std::ofstream file; 
	file.open("Datas.txt");
	assert(not file.fail()); //check if the file opens correctly
	
	n.setExternalInput(externalInput());
	
	int simulation_time = t_start; 
	//update all the neurons of the simulation
	do { 
		n.update(1, 0.0, 5); //no noise is added at the update function for one neuron simulation
		//write in the terminal the time of the spike
		if (n.getSpikeState()){ 
			std::cout << "A spike occured at time: " << simulation_time*h << std::endl;
		}
		simulation_time += N; //the simulation time advanced of a time step N
		//the membrane potential is stored in Datas.txt
		file << "Membrane potential at " << simulation_time*h << " milliseconds: " << n.getV_membrane() << std::endl; 
	} while (simulation_time < t_stop); 
}

void Simulation::twoNeruonsSimulation()
{
	//neuron 1 is the spiking neuron, neuron2 is a post-synaptic neuron
	Neuron neuron1(true), neuron2(true); 
	neuron1.addTargetNeuron(&neuron2);
	
	 //the external input is added only to the spiking neuron
	neuron1.setExternalInput(externalInput());
	
	int simulation_time = t_start;
	//update all the neurons of the simulation
	do {
		neuron1.update(1, 0.0, 5); //no noise is added at the update function for two neurons simulation
		if (neuron2.getTimeBuffer((simulation_time)%(D+1)) != 0){ //read if there is an amplitude in the time buffer
			//if an amplitude is present, the time the neurons receives the spike is written in the terminal
			std::cout << "The spike is received at time: " << simulation_time*h << std::endl;
		}
		neuron2.update(1, 0.0, 5);
		if (neuron1.getSpikeState()){
			 //write in the terminal the time of the spike
			std::cout << "A spike occured at time: " << simulation_time*h << std::endl;
		}
		simulation_time += N; //the simulation time advanced of a time step N
	} while (simulation_time < t_stop); 
	
}
	
void Simulation::networkSimulation(double g, double pois)
{
	//open a file to store the time of all the spikes and the neuron spiking id
	std::ofstream file2; 
	file2.open("Spike_time.txt");
	assert (not file2.fail());  //check if the file opens correctly

	//initiliaze the array representing the network of 12500 neurons
	std::array <Neuron*, total_neurons> neurons;
	initializeNeurons(neurons);
	
	//add the connections between each neuron
	for (auto const& n : neurons){
		n->addConnections(neurons);	
	}
	std::cout << "Connections added" << std::endl;
	
	int simulation_time = t_start; 
	//update all the neurons present in the network
	do {
		for (size_t i(0); i<neurons.size(); ++i){ 
			assert (neurons[i] != nullptr); //check always that the neurons aren't nullptr
			//update with the noises given by random spikes
			neurons[i]->update(N, neurons[i]->randomSpikes(pois), g);  
			if (neurons[i]->getSpikeState()){ 
				//when a neuron spikes, write down the time and the index of the neuron
				file2 << neurons[i]->getTimeSpike()/h << '\t' << i << '\n';
			}
		}
		simulation_time += N; //the simulation time advanced of a time step N after all neuron clocks have already advanced
	} while (simulation_time < t_stop); 
	
	
	//at the end of the simulation the memory has to be desallocated
	for (auto& n : neurons){ 
		n = nullptr;
		delete n;
	}
}

void Simulation::plotGraph_A()
{
	networkSimulation(3,2);
	pythonScript();
}

void Simulation::plotGraph_B()
{
	networkSimulation(6,4);
	pythonScript();
}

void Simulation::plotGraph_C()
{
	networkSimulation(5,2);
	pythonScript();
}

void Simulation::plotGraph_D()
{
	networkSimulation(4.5, 0.9);
	pythonScript();
}

double Simulation::externalInput()
{
	double input(0.0); 
	std::cout << "Choose a value for the external input" << std::endl;
	std::cin>>input;
	return input;
}

void Simulation::initializeNeurons(std::array <Neuron*, total_neurons>& ns)
{
	Neuron n_excitatory(true);
	Neuron n_inhibitory (false);
	for (size_t i(0); i<ns.size(); ++i){
		if (i<excitatory_neurons){//from 0 to 9999 the neurons are excitatory
			ns[i] = new Neuron(n_excitatory);
		} else {
			ns[i] = new Neuron (n_inhibitory);//from 9999 to 12499 the neurons are inhibitory
		}
		assert(ns[i] != nullptr); //check that no neurons in the end is nullptr
	}
	std::cout << "Network initialized" << std::endl;
}

void Simulation::pythonScript()
{
	std::string name ("python ../Graphs.py &");
	system (name.c_str()); //name stays constant
}
	

Simulation::~Simulation()
{}

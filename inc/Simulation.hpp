#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <array>
#include "Neuron.hpp"

/*!
     * @class Simulation
     * @details This class simulates the Brunel's network.
     * First, it simulates the network when it contains one neuron only.
     * On the terminal appears the time of the spikes occured and one
     * document will store the membrane potential at every time step interval.
     * Then it simulates the model with two neurons connected and a delay of reception.
     * On the terminal appears the time of the spikes and the time when the spikes has been received.
     * Finally, it simulates the whole network with 12500 neurons. Here the connections are 
     * created and all the neurons updated. A document will store all the 
     * time of the spikes occured and the corresponding id of the spiking neuron. This
     * documentaiton will be used for generating the graphs with Python.
     * After the whole network simulation, 4 graphs with two different ratios can
     * be generated. 
     * Some additional functions allow a better modularisation.
     */
     
class Simulation
{
public:

	/*!
     * @brief Constructor of the Simulation class
     */
	Simulation();
	/*!
     * @brief Simulate the brunel's network with one neuron
     * @details The neuron in the network has to spikes when its membrane potential cross the threshold. 
     * The membrane potential is increased by the external input received. If the input is between 0 and 1, nothing happens.
     * The membrane potential increases by the threshold is not crossed. If the input is more than 1, the neuron will spikes.
     * If we test the model with a input of 1.01, we will see five spikes and their relative time in the terminal. 
     * The user decides the value of the external input.
     * For this model there is a file that stores the values of the memrbane potential at each time steps.
     * There are no noises coming from external random spikes.
     */
	void oneNeuronSimulation();
	/*!
     * @brief Simulate the brunel's network with two neurons
     * @details One neuron is the spiking neuron, the other one is the post-synaptic neuron.
     * The second one will be added to the target of the first one and he will receives the spikes. 
     * An external input is added for the spiking neuron but not for the post-synaptic neuron. 
     * When the first neuron spikes, the time will be stored and the second neuron will recevies the signal 
     * after a certain delay. On the terminal there are the corresponding values of spikes and received spikes.
     * The use decides the value of the external input.
     * There are no noises coming from external random spikes
     * 
     */
	void twoNeruonsSimulation();
	/*!
     * @brief Simulate the brunel's network with the whole network of 12500 neurons.
     * @details An array of neurons will be created to store all the inhibitory and excitatory neurons. After initializing it,
     * the conncetions are created between all the neurons. Finally all the neurons are updated. When a neuron spikes it sends 
     * the corresponding amplitude to all the post-synaptic neurons which will see their membrane potential increase, and will
     * spike too, and so on for the whole simulation.
     * The noises from the rest of the brain are presents and contribute increasing the memrane potential at each time step.
     * There is a file that stores all the time a neuron spikes and its index. This will allow creating the 4 graphs of the
     * brunel's network.
     * 
     * @param g :  a double indicating the rate between inhibitory connections and excitatory connections 
     * @param pois : a double indicating the rate between inhibitory connections and excitatory connections 
     * 
     * 
     */
	void networkSimulation(double g, double pois);
	
	/*!
     * @brief Plot the graph A of the brunel's model
     * @details This function plots with python the graph A of the brunel's model.
     * The graph depends on two ratio: the g ratio and nu_ext over nu_threshold. In this
     * case the first one is 3 and the second one is 2, so the negative amplitude will be 0.3 and 2 will be the 
     * central value of the poisson curbe
     */
	void plotGraph_A();
	/*!
     * @brief Plot the graph B of the brunel's model
     * @details This function plots with python the graph B of the brunel's model.
     * The graph depends on two ratio: the g ratio and nu_ext over nu_threshold. In this
     * case the first one is 6 and the second one is 4, so the negative amplitude will be 0.4 and 2 will be the 
     * central value of the poisson curbe.
     */
	void plotGraph_B();
	/*!
     * @brief Plot the graph C of the brunel model
     * @details This function plots with python the graph C of the brunel's model.
     * The graph depends on two ratio: the g ratio and nu_ext over nu_threshold. In this
     * case the first one is 3 and the second one is 2, so the negative amplitude will be 0.6 and 2 will be the 
     * central value of the poisson curbe
     */
	void plotGraph_C();
	/*!
     * @brief Plot the graph D of the brunel model
     * @details This function plots with python the graph D of the brunel's model.
     * The graph depends on two ratio: the g ratio and nu_ext over nu_threshold. In this
     * case the first one is 4.5 and the second one is 0.9, so the negative amplitude will be 0.45 and 0.9 will be the 
     * central value of the poisson curbe.
     */
	void plotGraph_D();
	/*!
     * @brief Let the user set the external input for the simulation
     *  
     * @return a double input : the value of the external input
     */
     double externalInput();
     /*!
     * @brief Initialize the table of fixed size of pointers on neurons (12500)
     * @details There are 10000 excitatory neruons and 2500 inhibitory neurons
     *  
     * @param ns : array of pointer on neurons that has to be inizialize. The parameter is 
     * passed by reference because it has to be modified.
     */
     void initializeNeurons(std::array <Neuron*, total_neurons>& ns);
     /*!
     * @brief Open the python script
     * @details The name of the python script in "system" remains constant
     * 
     */
     void pythonScript();
	/*!
     * @brief Destructor of the class Simulation 
     */
	~Simulation();

	
};

#endif



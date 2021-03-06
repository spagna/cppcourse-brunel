#ifndef NEURON_H
#define NEURON_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <array>
#include <vector>
#include <cassert> 


/**********************************************************************************************//**
 * This list represent all the constant needed in the simulation;
 * there are time for start and end, time constants,
 * membrane potential constants, step constant, the delay, the total number of neurons
 * and of the two different groups of neurons, the numbers of connections, 
 * the amplitude of excitatory/inhibitory spikes for one graph
 * and the number needed for the poisson generator, always for one graph
 * NB: time depending constants are in terms of time step to make the simulation easier and
 * to avoid errors given by double floating numbers. 
 * To have the constants in terms of time step, the values in milliseconds has been multiplied by h.
 *************************************************************************************************/

constexpr int t_start (0);//!< step the simulation starts (0.0 milliseconds)
constexpr int t_stop (12000); //!< step the simulation stops (500.0 milliseconds)


constexpr int tau (200); //!< time constant in terms of time steps (20.0 in milliseconds) (note tau = R/C)
constexpr int tau_rp (20); //!< time constat for the refractory period in terms of time steps (2.0 in milliseconds) 

constexpr double V_thr (20.0); //!< firing threshold in microvolts 
constexpr double V_refractory (0.0); //!< refractory membrane potential in microvolts

constexpr double h (0.1); //!< equidistant step of time during which the simulation evolves (in milliseconds)
constexpr int N(1); //!< step of simulations 

constexpr unsigned int C (1); //!< number of connections from other neurons
constexpr double R (20.0); //!< /membrane resistance
constexpr double const1 (exp(-h/(tau*h))); //!< constant 1 of the membrane equation
constexpr double const2 (R*(1 - const1)); //!< constante 2 of the membrane equation

constexpr int D(15); //!< delay: time steps needed before the post-synpatic neuron receives a spike (1.0 in milliseconds)

constexpr int total_neurons (12500); //!< total number of neurons in the network
constexpr int excitatory_neurons (10000); //!< number of excitatory neurons
constexpr int inhibitory_neurons (2500); //!< number of inhibitory neurons

constexpr int c_e (1000); //!< connections received from excitatory neurons
constexpr int c_i (250); //!< connections received from inhibitory neurons
constexpr int c_ext (c_e); //!< connections received from the rest of the brain 

constexpr double J_e (0.1); //!< amplitude of the signal from an excitatory neuron
constexpr double J_i (0.5); //!< /amplitude of the signal from an inhibitory neuron

constexpr double nu_ext (20); //!< mean of the number of spikes in the network (spikes par milliseconds)
constexpr double poisson_gen(nu_ext*h); //!< rate at which every neuron receives additional random input from the rest of the brain

/*!
     * @class Neuron
     * @details This class represents a single neuron of the Brunel's network.
     * Each neuron has specific members describing its behaviour and what happens when 
     * a spike occured or a spike is received. 
     * Every neuron is connected with other neurons and each neuron receives a total of 1000 excitatory connections
     * and 250 inhibitory connections. The connections are created randomly. 
     * The neurons are devided in excitatory and inhibitory neurons: the excitatory ones send
     * positive amplitudes to the targets, the inhibitory ones negative amplitudes. 
     * Moreover, each neuron receives some random external spikes from the rest of the brain. Those random
     * spikes are generated by a poisson generator. 
     * The membrane potential of the neuron is updated at the end of each time step of the simulation.
     * When it crosses a certain threshold, the neruon spikes and sends the amplitude of the spike to all
     * its target neurons. 
     * Each neuron enters in refractory time for a certain period after it spikes, and then starts incrising
     * its membrane potential again depending on the whole signals.
     * The spikes are received by the targets with a certain delay. They use a time buffer with a fixed size
     * to store the amplitude of the last spike in a box, which will be read after the delay.
     */

class Neuron 
{
public:
	/*!
     * @brief The constructor of the class neuron.
     * @details The constructor initialize the following argument by default:
     * the membrane potential is 0.0 mV, the number of spikes is 0, the time of the last spike is 0.0 msec,
     * the spike state is false because no spikes has occured yet,
     * the local neuron clock is 0 time step, the external input is 0.0 mV and the refractory period 
     * is initialize to false so that the neuron can start receiving spike at time 0.
     * The only argument needed by the neuron at the moment of its creation is the boolean excitatory.
     * It's true if we want to create an excitatory neuron and false for an inhibitory one.
     * The D+1 boxes of time buffer are inizialized to 0.0 mV.
     * 
     * @param excitatory_neuron : a boolean indicating that the neuron is excitatory
     * @param membranePotential : a double indicating the initial membrane potential
     * @param nb_spikes : a positive integer indicating the initial number of spikes
     * @param t_spike : a double indicating the initial time stored for a spike in milliseconds
     * @param spike : a boolean indicating that no spikes occured before the simulation starts
     * @param neuron_clock : an integer indicating the initial local time of the neuron in terms of time steps
     * @param external_input : a double indicating the initial input percived 
     * @param r_period : true only when the neuron is in its refractory period
     * 
     */
	
	Neuron (bool excitatory_neuron,
			double membranePotential = 0.0, unsigned int nb_spikes = 0, 
			double t_spike = 0.0, bool spike = false,	
			unsigned int neuron_clock = 0, double external_input = 0.0,
			bool r_period = false);
	
	
	 /*!
     * @brief Get the membrane potential.
     *
     * @return A double V_membrane_: the membrane potential
     */
	double getV_membrane() const;
	 /*!
     * @brief Get the number of spikes done by a neuron.
     *
     * @return A positive integer nb_spikes_: the number of spikes 
     */
	int getNumberSpikes() const;
	 /*!
     * @brief Get the time in milliseconds of the last spike that has occured.
     *
     * @return A double t_spike_: the time of the last spike occured 
     */
	double getTimeSpike() const;
	 /*!
     * @brief Get the spike state, that is true if a neuron has spiked in a certain time step.
     * @details This function allows to know if the neuron has spiked in the current time step 
     * 
     * @return A boolean spike_: if the spike occured (true) or not (false) 
     */
	bool getSpikeState() const; 
	/*!
     * @brief Get the local time of the neuron, which is different form the time of the simulation
     *		  
     * @return A positive integer neuron_clock_: the neuron local clock in term of time step
     */
	unsigned int getNeuronClock() const;
	/*!
     * @brief Get the external input received by one or two neurons. 
     * @details When the simulation has more than two neurons, the external input is always 0.0 
     * 
     * @return A double external_input_: the external input received external_input_
     */
	double getExternalInput() const;
	
	/*!
     * @brief Get the refractory state 
     * @details If it's true, the neuron is in his refractory period, if false it can receive stimuli
     * 
     * @return A boolean r_period_: the refractory state of the neruon
     */
	bool getRefractoryState() const;
	
	/*!
	 * @brief Get the amplitude stored in the time buffer in a specific case
	 * @details The amplitude is stored depending on the current time and the delay.
	 *
	 * @param i : an integer refering to a certain case 
	 * @return A double t_buffer[i]: the amplitude stored in the specific case i
	 */
	double getTimeBuffer (int i) const; 
	/*!
	 * @brief Get one specific neuron chosen between the vector of target neurons
	 *
	 * @param i: an integer refering to the neuron needed
	 * @return A pointer  n_target_[i]: points on the i element of the targets vector
	 */
	Neuron* getTargetNeuron (int i) const;
	/*!
	 * @brief Get the whole vector of targets
	 *
	 * @return A vector n_target_: vector of pointers on target neurons
	 */
	std::vector <Neuron*> getTargets() const;
	
	/*!
	 * @brief Get the role of the neuron.
	 * @details If the boolean is true, the neuron is an excitatory neuron, 
	 * if false it's an inhibitory neuron.
	 *
	 * @return A boolean excitatory_neuron: true or false depending on the role of the neuron
	 */
	bool getExcitatoryNeuron() const;
	/*!
	 * @brief Get the number of excitatory connections received by a neuron.
	 * @details This getter is needed for tests.
	 *
	 * @return An integer nb_excitatory_connections: the number of excitatory connections
	 */
	int getExcitatoryConnections() const;
	/*!
	 * @brief Get the number of inhibitory connections received by a neuron.
	 * @details This getter is needed for tests.
	 *
	 * @return An integer nb_inhibitory_connections: the number of inhibitory connections
	 */
	int getInhibitoryConnections() const;
	
	
	/*!
	 * @brief Set the membrane potential with the value in parameter
	 *
	 * @param V_membrane : a double containing the value of the membrane potential
	 */
	void setV_membrane(double V_membrane);
	/*!
	 * @brief Set the number of spikes with the value in parameter
	 *
	 * @param nb : a positive integer containing the number of spikes
	 */
	void setNumberSpikes(unsigned int nb);
	/*!
	 * @brief Set the time of the last occured spike in milliseconds
	 *
	 * @param t : a double containing the time of the last spike 
	 */
	void setTimeSpike (double t);
	/*!
	 * @brief Set the spike state to true if a spike has occured
	 *
	 * @param spike : a boolean corresponding to true if the spike has occured
	 */
	void setSpikeState (bool spike);
	/*!
	 * @brief Set the time of the local clock in time step
	 * @details The time step is given by the time in milliseconds multiplied by the h value
	 * that corresponds to the time interval between each step.
	 * 
	 * @param clock : an integer refering to a certain time step 
	 */
	void setNeuronClock (unsigned int clock);
	/*!
	 * @brief Set the external current received 
	 *
	 * @param external_input : a double indicating the external input received
	 */
	void setExternalInput (double external_input);
	/*!
	 * @brief Set the refractory state of the neuron
	 *
	 * @param r : a boolean indicating if the neruon is in its refractory period or not
	 */
	void setRefractoryState (bool r);
	/*!
	 * @brief Set a case of the time buffer 
	 *
	 * @param i : an integer indicating the case that has to be filled
	 * @param val : a double indicating the amplitude of the signal that need to be stored
	 */
	 
	void setTimeBuffer (int i, double val); 
	/*!
	 * @brief Set a case of the vector of target neurons.
	 *
	 * @param i : an integer indicating the case that has to be filled
	 * @param n : a pointer on the neuron that has to replace another target or an empty case
	 */
	void setTargetNeuron (int i, Neuron* n);
	/*!
	 * @brief Set the role of a neuron
	 * @details The parameter is setted to true when the neuron is defined as an excitatory neuron, 
	 * and false if it's an inhibitory neuron
	 * 
	 * @param n : a boolean indicating the role
	 */
	void setExcitatoryNeuron (bool n);
	/*!
	 * @brief Set the number of the excitatory connections received
	 *
	 * @param nb : an integer indicating the number of connections
	 */
	void setExcitatoryConnections(int nb);
	/*!
	 * @brief Set the number of the inhibitory connections received
	 *
	 * @param nb : an integer indicating the  number of connections
	 */
	void setInhibitoryConnections(int nb);


	/*!
	 * @brief Fill the time buffer 
	 * @details The time buffer is filled with a certain amplitude 
	 * of a spike it just has happened. If another value is already present in the box, 
	 * the new value is summed up. To decide the box that has to be filled, 
	 * the neuron takes in account the current local time and the delay. 
	 * 
	 * @param i : an integer inidcating the box that has to be filled
	 * @param val : a double indicating the amplitude of the signal received that has to be stored
	 */
	void addTimeBuffer (int i, double val); 
	/*!
	 * @brief Fill the vector of target neurons
	 * @details Each time a neuron is connected to another neuron, it has to store a pointer on the 
	 * target neuron in a vector to remember it. So, when the neuron spikes, it just has to
	 * accede the vector to find all the targets and send them the amplitude of the spike.
	 * To add the pointer, because the order is not important, the function push_back is used.
	 *
	 * @param n : a pointer to a neuron that has to be stored as target neuron
	 */
	void addTargetNeuron (Neuron* n);
	/*!
	 * @brief Update the neuron's parameters when the membrane potential cross the threshold and the neuron spikes
	 * @details When a neuron spikes, some attributes have to change. The function stores the time 
	 * of the new spike in milliseconds, increases the number of spikes occured, 
	 * sets the state of the spike to true and the refractory period to true too.
	 * 
	 * @param dt : an integer indicating the time the spike has occured and has to be stored in temrs of time step
	 * @details The parameter corresponds to the local time of the neuron.
	 */
	void updateNeuronState (int dt); 
	/*!
	 * @brief Update the neuron's target when the neuron spikes
	 * @details When a neuron spikes, the time buffer of the target neurons has to be filled with the
	 * amplitude of the singal sent by the spiking neuron, depending on the role of the neuron. 
	 * The box is filled based on the local time of the spiking neuron and the delay of reception.
	 * 	
	 *	@param g : a double indicating the rate between inhibitory and excitatory connections  
	 */
	void updateTargets(double g); 
	/*!
	 * @brief Update the neuron depending on its situation.
	 * @details Every time step the neuron has to be checked and updated.
	 * A neuron has three possible situations: it's membrane potential could cross the threshold
	 * and so the neruon would spike, it could be in his refractory period if a spike occured 
	 * in less than 2 milliseconds before or neither of them and so the neuron would just update it's
	 * membrane potential depending on the external input received and the amplitudes stored in its time buffer.
	 * When a neuron is updated, it has to receive the amplitudes of the noise in parameters. This also
	 * allows futher simulations or tests.
	 * 
	 * @param dt : an integer indicating the time step. 
	 * @param noise : a double indicating the random amplitudes recevied by the rest of the brain 
	 * @param g : a double indicating the the rate J_i/J_e
	 * 
	 */
	void update(int dt, double noise, double g); 	


	/*!
	 * @brief Solve the membrane equation and update the membrane potential
	 * @details During each time step interval, the neuron has to update its membrane potential 
	 * depending on all the external inputs received. 
	 * The equation depends on some external parameters and two constants.
	 * 		 
	 * @param input : a double indicating the external input
	 * @param ampl : a double indicating the amplitude found in the time buffer
	 * @param random : a double indicating the amplitude of the external signal that randomly occure
	 */
	void solveMembraneEquation(double input, double ampl, double random);
	
	/*!
	 * @brief Add the connections between all neurons in the network
	 * @details Before the simulation starts, the connections have to be setted.
	 * Every neuron receives 1000 excitatory connections and 250 inhibitory connections.
	 * Those connections are choosen randomly. 
	 * 
	 * @param neurons : an array of pointer on neurons correponding to the neurons in the whole network (12500).
	 */
	void addConnections (std::array<Neuron*, total_neurons>  const& neurons); 
	
	/*!
	 * @brief Generate random amplitudes from the rest of the brain
	 * @details At each time step, a neuron receives a certain number of amplitudes that come
	 * from the rest of the brain. The number of random spikes received is implemented by a poisson generator.
	 * The external neurons spike randomly and give only excitatory impulses. 
	 * The random generators are static members because we need just one copy of the values
	 * and the neurons don't have to generate them every update every step. This makes the 
	 * compilation faster.
	 * 
	 *@param pois : an integer rate nu_ext/nu_threshold that has to be inserted in the poisson generator 
	 *
	 * @return A double J_e*dis_ext(gen): the total amplitudes.
	 * @details The amplitude given by an excitatory neuron multiplied by the number 
	 * generated randomly by the poisson distribution.
	 */
	double randomSpikes (double pois) const; 
	
	/*!
     * @brief The destructor of the class Neuron
     */
	~Neuron();
	
	

private:
	bool excitatory_neuron_; //!< Determine the role of a neuron  
	//!< If this member is true, the neuron will be an excitatory neuron.
	//!< Otherwise it will be an inhibitory neuron.
	//!< An excitatory neuron will give a different amplitude from an inhibitory neuron. 
	
	double V_membrane_; //!< Membrane potential in millivolts  
	
	unsigned int nb_spikes_; //!< Number of spikes stored during the simulation  
	
	double t_spike_; //!< Time the last spike occured in milliseconds  
	
	bool spike_; //!< True if a spike has occured during the last update  
	
	unsigned int neuron_clock_; //!< Local time of the neuron calculated in time step intervals
	
	double external_input_; //!< External input received in millivolts  
	
	bool r_period_;	//!< Refractory state: true if the neuron is in its refractory period and can't receive stimuli 
	
	std::array <double, D+1> t_buffer_;//!<  Time buffer  
	//!< The time buffer has a size of D+1 to allow to correct add the amplitudes in a case
	//!< that will be read with the correct delay by a target neuron.
	//!< It stores the amplitude/the sum of the amplitudes of the signals received by a neuron 	
	
	std::vector <Neuron*> n_target_; //!< Vector of targets 
	//!< Each neuron has a number of post synaptic neurons that 
	//!< has to update when a spike occurs 
	
	int nb_excitatory_connections_; //!< Number of excitatory connections received by a neuron 
	
	int nb_inhibitory_connections_; //!< Number of inhibitory connections received by a neuron  

};

#endif



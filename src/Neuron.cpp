#include "Neuron.hpp"
#include <iostream>
#include <random>


Neuron::Neuron (bool excitatory_neuron, 
				double V_membrane, unsigned int nb_spikes,
				double t_spike, 
				bool spike, unsigned int neuron_clock, 
				double external_input,
				bool r_period)
				
: excitatory_neuron_(excitatory_neuron),
  V_membrane_ (V_membrane),
  nb_spikes_(nb_spikes), 
  t_spike_(t_spike), spike_(spike), 
  neuron_clock_ (neuron_clock), 
  external_input_(external_input),
  r_period_(r_period)
{
	//each box of the buffer is setted to zero at the beginning
	for (size_t i(0); i<t_buffer_.size(); ++i){ 
		t_buffer_[i] = 0.0;
	}
} 

double Neuron::getV_membrane() const
{
	return V_membrane_;
}

int Neuron::getNumberSpikes() const
{
	return nb_spikes_;
}

double Neuron::getTimeSpike() const
{
	return t_spike_;
}

bool Neuron::getSpikeState() const
{
	return spike_;
}

unsigned int Neuron::getNeuronClock() const
{
	return neuron_clock_;
}

double Neuron::getExternalInput() const
{
	return external_input_;
}

bool Neuron::getRefractoryState() const
{
	return r_period_;
}

double Neuron::getTimeBuffer (int i) const
{
	return t_buffer_[i];
}

Neuron* Neuron::getTargetNeuron(int i) const
{
	return n_target_[i];
}

std::vector<Neuron*> Neuron::getTargets() const
{
	return n_target_;
}

bool Neuron::getExcitatoryNeuron() const
{
	return excitatory_neuron_;
}

int Neuron::getExcitatoryConnections() const
{
	return nb_excitatory_connections_;
}

int Neuron::getInhibitoryConnections() const
{
	return nb_inhibitory_connections_;
}

void Neuron::setV_membrane(double V_membrane)
{
	V_membrane_ = V_membrane;
	
}

void Neuron::setNumberSpikes(unsigned int nb)
{
	nb_spikes_ = nb;
}

void Neuron::setTimeSpike (double t)
{
	t_spike_ = t;
}

void Neuron::setSpikeState (bool spike)
{
	spike_ = spike;
}

void Neuron::setNeuronClock (unsigned int clock)
{
	neuron_clock_ = clock;
}

void Neuron::setExternalInput (double external_input)
{
	
	external_input_ = external_input;
}

void Neuron::setRefractoryState(bool r)
{
	r_period_ = r;
}

void Neuron::setTimeBuffer (int i, double val)
{
	t_buffer_[i] = val;
}

void Neuron::setTargetNeuron(int i, Neuron* n)
{
	n_target_[i] = n;
}

void Neuron::setExcitatoryNeuron (bool n)
{
	excitatory_neuron_ = n;
}

void Neuron::setExcitatoryConnections(int nb)
{
	nb_excitatory_connections_ = nb;
}

void Neuron::setInhibitoryConnections(int nb)
{
	nb_inhibitory_connections_ = nb;
}

void Neuron::addTimeBuffer (int i, double val)
{
	t_buffer_[i] += val;
}

void Neuron::addTargetNeuron(Neuron* n)
{
	n_target_.push_back(n);
}


void Neuron::updateNeuronState (int dt)
{
	t_spike_ = dt*h; //store the time of the spike in milliseconds
	++nb_spikes_; 
	spike_ = true; 
	r_period_= true; 
}

void Neuron::updateTargets(double g)
{
	for (auto const& n: n_target_){
		assert(n != nullptr); //controls that the targets aren't nullptr
		// the time buffer is filled with amplitudes of excitatory or inhibitory neurons
		// the modulo allows a correct insertion of the values in a table of fixed  size
		assert ((neuron_clock_+D)%(D+1) < t_buffer_.size()); //check the insertion doesn't exceed the vector size
		if (excitatory_neuron_){
			n->addTimeBuffer((neuron_clock_+D)%(D+1), J_e);
		} else {
			n->addTimeBuffer((neuron_clock_+D)%(D+1), -g*J_e);
		}
	}
}

void Neuron::update(int dt, double noise, double g)
{	
	//the value has to be setted at false at the beginning of every update until a spike occurs
	spike_ = false; 
	// if the membrane potential crosses the threshold, the current neuron and its targets are updated
	if (V_membrane_ > V_thr){ 
		updateNeuronState(neuron_clock_); 
		updateTargets(g); 
	} 
	//the neuron is in its refractory period if the distance in time between the last spike
	// and the current neuron is less than tau, a constant, and just if a spike just occured
	if (r_period_ and tau_rp > neuron_clock_ - t_spike_/h){ 
		V_membrane_ = V_refractory; 
	//if the neuron doesn't spike and it isn't in a refractory state, the membrane potential has to be updated
	} else {
		r_period_ = false; 
		solveMembraneEquation(external_input_, getTimeBuffer((neuron_clock_)%(D+1)), noise); 
	}
	//the box is resetted to 0 after each update to allow the next spikes to be stored
	setTimeBuffer(neuron_clock_%(D+1), 0.0); 
	neuron_clock_ += dt;	
}

void Neuron::solveMembraneEquation(double input, double ampl, double noise)
{
	//equation of the membrane potential
	V_membrane_ = const1*V_membrane_ + const2*input + ampl + noise;
}

void Neuron::addConnections(std::array<Neuron*, total_neurons>  const& neurons)
{
	//algorithmes for generating random numbers
	std::random_device rd; 
	std::mt19937 gen(rd()); 
	
	//distribution of random integer numbers to determine which excitatory
	//or inhibitory neuron is connected to the current neuron. 
	
	//Those values correpond to an index of an excitatory neuron in the array of neurons
	std::uniform_int_distribution<> dis_e (0, excitatory_neurons-1); 
	
	//the excitatory neuron randomly chosen adds the current neuron as target
	for (int i(0); i<c_e; ++i){ 
		neurons[dis_e(gen)]->addTargetNeuron(this); 
		//takes the count of the excitatory connections received by a neuron
		++nb_excitatory_connections_; 
	}
	
	//Those values correpond to an index of an inhibitory neuron in the array of neurons
	std::uniform_int_distribution<> dis_i (0, inhibitory_neurons-1);

	//the inhibitory neuron randomly chosen adds the current neuron as target
	for (int i(0); i<c_i; ++i){
		neurons[excitatory_neurons+dis_i(gen)]->addTargetNeuron(this);
		//take the count of the inhibitory connections received by a neuron
		++ nb_inhibitory_connections_;
	}
}

double Neuron::randomSpikes(double pois) const
{
	//algorithmes for generating random numbers
	static std::random_device rd; 
	static std::mt19937 gen(rd()); 
	//decides how many random spikes from external connections a neuron will receive 
	static std::poisson_distribution<> dis_ext (pois);
	//return the amplitudes of the random generated spike
	return J_e*dis_ext(gen);
}
			
Neuron::~Neuron()
{}

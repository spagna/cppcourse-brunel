#include <iostream>
#include "Neuron.hpp"
#include "gtest/gtest.h"
#include <cmath>
#include <cassert>


//Run all the tests of gtest

int main(int argc, char**argv){
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
/*
 * TEST1: Test if the membrane potential of the neuron is coeherent with the membrane equation.
 * The used external input is positive, thus the membrane potential has to be positive too.
*/

TEST (NeuronTest, PositiveMembranePotential){
	Neuron neuron(true);
	neuron.setExternalInput(1.0); //positive input given
	neuron.update(1, 0.0, 5); //one update without noises
	//check if the membrane potential increases in positif according to the membrane equation
	EXPECT_NEAR(20.0*(1.0-exp(-0.1/20.0)), neuron.getV_membrane(), 0.001);
	
}

/*
 * TEST2: Test if the membrane potential of the neuron is coeherent with the membrane equation.
 * The used external input is negative, thus the membrane potential has to be negative too.
*/

TEST (NeuronTest, NegativeMembranePotential){
	Neuron neuron(true);
	neuron.setExternalInput(-1.0); //negative input given
	neuron.update(1, 0.0, 5);//one update without noises
	//check if the membrane potential decreases in negative according to the membrane equation
	EXPECT_NEAR(-20.0*(1.0-exp(-0.1/20.0)), neuron.getV_membrane(), 0.001); 
}

/*
 * TEST3: Test if the membrane potential of the neuron is coeherent with the membrane equation.
 * The used external input null, thus the membrane pontential shouldn't increase if no noise is added.
*/

TEST (NeuronTest, NulMembranePotential){
	Neuron neuron (true);
	neuron.setExternalInput (0.0); //no input 
	neuron.update(1, 0.0, 5); //no noise 
	EXPECT_NEAR(0.0, neuron.getV_membrane(), 0.001); //check if there is no changes in the membrane potential
}

/*
 * TEST4: Test if the time of the first spike is 92.4 milliseconds for an external input of 1.01
*/

TEST (NeuronTest, SpikeTimes){
	Neuron neuron (true); 
	neuron.setExternalInput(1.01); // the neuron will cross the threshold with this input
	//update the neuron until it's ready to spike
	do {
		neuron.update(1, 0.0, 5); //no noise
	} while (neuron.getNeuronClock() < 924);
	EXPECT_NEAR (neuron.getV_membrane(), 20.0, 0.001); //check if neuron is ready to spike
	neuron.update(1, 0.0, 5); //neuron spikes
	 //check if the values of the time of the last spikes is the same as the values written in the 
	 //terminal during the simulation
	EXPECT_EQ (92.4, neuron.getTimeSpike());
	EXPECT_NEAR (neuron.getV_membrane(), 0.0, 0.001); //check if neuron enters its refractory period
}
/*
 * TEST5: Test if the post-synaptic neuron receives the spike with the correct delay
*/
TEST (NeuronTest, Delay){
	Neuron neuron1(true); //the spiking neuron is an excitatory neuron
	Neuron neuron2(true);
	neuron1.addTargetNeuron(&neuron2); //neuron2 is added as a target of neuron1
	neuron1.setExternalInput(1.01); //neuron1 is the only one receiving an external input
	
	//the neurons are updated unitl neuron1 is ready to spike
	do {
		neuron1.update(1, 0.0, 5); //no noise
		neuron2.update(1, 0.0, 5); 	
	} while (neuron1.getNeuronClock() < 924);
	EXPECT_NEAR (neuron1.getV_membrane(), 20.0, 0.001); //chek if neuron1 is ready to spike
	EXPECT_NEAR (neuron2.getV_membrane(), 0.0, 0.001); //check if neuron2 doesn't receive any stimuli
	neuron1.update(1, 0.0, 5); //neuron1 spikes
	neuron2.update(1, 0.0, 5);
	EXPECT_NEAR (neuron1.getV_membrane(), 0.0, 0.001); //check if neuron1 goes in its refractory period
	EXPECT_NEAR (neuron2.getV_membrane(), 0.0, 0.001); //check if neuron2 doesn't spike
	//neuron2 is update until the delay ends
	for (int i(0); i<D; ++i){ 
		neuron2.update(1, 0.0, 5); //after D time steps neuron2 should receives the spike
		}
	EXPECT_NEAR (neuron2.getV_membrane(), 0.1, 0.001); //check if neuron2 receives the spike
}

/*
 * TEST6: Test if every neuron receives exactly 1000 excitatory connections and 250 inhibitory connections 
*/
TEST (NeuronTest, Connections){
	std::array <Neuron*, 12500> neurons; //network
	Neuron n_excitatory(true);
	Neuron n_inhibitory (false);
	 //initiliaze the array representing the network of 12500 neurons
	for (size_t i(0); i<neurons.size(); ++i){
		if (i<excitatory_neurons){//from 0 to 9999 the neurons are excitatory
			neurons[i] = new Neuron(n_excitatory);
		} else {
			neurons[i] = new Neuron (n_inhibitory);//from 9999 to 12499 the neurons are inhibitory
		}
		assert(neurons[i] != nullptr); //check that no neurons in the end is nullptr
	}
	//add the connections between every neuron
	for (auto const& n: neurons){
		n->addConnections(neurons);
		EXPECT_EQ (n->getExcitatoryConnections(), 1000);//check the number of excitatory connections
		EXPECT_EQ (n->getInhibitoryConnections(), 250); //check the number of inhibitory connections 
	}
	int j(0);
	//this is another way to test the number of connections: instead of counting the connections added
	//to each neuron, the sizes of the vector of targets are summed up and have to give the total
	//number of connections present in the whole network
	for (auto const& n: neurons){
		j += n->getTargets().size();
	}
	EXPECT_EQ (12500*1250, j);
		
	for (auto& n:neurons){ //clear the memory
		n = nullptr;
		delete n;
	}
}

/*
 * TEST7: Test if the neuron respects its refractory period even 
 * if an external input and noises are applied during the refractory period (after the spike)
*/

TEST (NeuronTest, RefractoryPeriod){
	Neuron neuron(true);
	neuron.setExternalInput(1.01);
	do { //update the neuron until it spikes
		neuron.update(1, 0.0, 5);
	} while (neuron.getNeuronClock() < 925);
	double noise = neuron.randomSpikes(2);
	for (int i(0); i<tau_rp; ++i){ //for 2.0 milliseconds the neuron has to stay in its refractory period
		EXPECT_NEAR (0.0, neuron.getV_membrane(), 0.001); //check if the membrane potential is 0.0 
		neuron.update(1,noise, 5);
	}
	//check if after the refractory period the membrane potential starts increasing again
	EXPECT_NEAR(noise + 20.0*(1.0-exp(-0.1/20.0)), neuron.getV_membrane(), 0.001);  
}
/*
 * TEST8: Test that the amplitude received by an inhibitory neuron gives a negative membrane potential
 * after the correct delay.
*/
TEST (NeuronTest, Inhibitory){
	Neuron n1 (false); //inhibitory neuron
	Neuron n2 (true); //target of the inhibitory neuron
	n1.setExternalInput(1.01);
	n1.addTargetNeuron(&n2);
	//update until n1 is ready to spike
	do {
		n1.update(1, 0.0, 5);
		n2.update(1, 0.0, 5);
	} while (n1.getNeuronClock() < 924);
	n1.update(1, 0.0, 5); //n1 spikes, the amplitude is -0.5 (g=5)
	n2.update(1, 0.0, 5);
	//after the correct delay n2 receives the signal
	for (int i(0); i<D; ++i){ 
		n2.update(1, 0.0, 5);
	}
	//check if the membrane potential is negative when the spikes is received
	EXPECT_NEAR (-0.5, n2.getV_membrane(), 0.001);
}


	




	

	

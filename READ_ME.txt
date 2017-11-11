	Presentation of the program

The program simulates the Nicolas Brunel's network (Dynamics of Sparsely Connected Networks of Excitatory and Inhibitory Spiking Neurons). The network contains 12500 neurons: 10000 neurons are excitatory and send excitatory signals, 2500 neurons are inhibitory and send inhibitory signals. 

The neurons are all connected together and those connections are created randomly. They receive a fixed number of them (1000 excitatory connections and 250 inhibitory connections) but they can have a different number of post-synaptic neurons. Moreover, each neuron receives a random number of external inputs from the rest of the brain (cortex), which changes at every time step.

A neuron has a certain membrane potential, which increases at each time step according to a specific membrane equation. The equation depends on different factors: two constants, the previous membrane potential, the signals received from the connected neurons and the signals received from random external inputs.
 
When the membrane potential crosses the threshold (here is set to 20mV), the neuron spikes. The amplitude of the signal depends on the role of the neuron and on two specific parameters of the simulation (which are the ratio between excitatory and inhibitory neurons and the value of the poisson generator).
 
The spike is sent to all the post-synaptic neurons. 
The signal is received and thus increases the membrane potential of the post-synaptic neuron after a certain delay (1.5 msec in this case). The amplitude of the signal is stored temporarily in a ring buffer. The ring buffer allows the post-synaptic neuron to read and perceive the signal in a different moment of the simulation.

The external random signals are created following a poisson distribution. The mean value of the distribution doesn't change during one simulation but can change between different simulations. These external amplitudes create a background noise.

Finally, when a neuron spikes, it enters in a refractory state, during which it can't receive any other signal and its membrane potential stays at 0 mV for the whole refractory time (2.0 msec).

The neurons are updated in term of time step. Each time step corresponds to 0.1 ms. 

There are four possible simulations for the whole network, each of them plotting a different graph. Two parameters changes between the simulations: the ratio between excitatory and inhibitory neurons and the mean value of the poisson distribution. The values are the same used by Nicolas Brunel. The graphs can be drawn thank to a python script and thank to a file which contains the times (in time step) of the spikes and the id of the neurons.

Moreover, there is the possibility to simulate a network containing one neuron or two connected neurons.

The class Neuron contains all the functions needed to a neuron to update its parameters. 
The class Simulation contains the functions to plot the four different graphs, to start a simulation for the whole network or for the two more simply models. In this class the network is initialized and the connections are created.
	
	

	GitHub

To run the program it's necessary to have Python, Doxygen and GoogleTest. 

On github there are: the folders docs, src and inc, the four graphs, the files CMakeLists.txt, .gitignore and Graphs.py.
In docs there is the Doxyfile.in file, in srx the .cpp files and in inc the .hpp files. Because of a problem in the conception of the folders and the CMakeLists, the documentations html and latex for doxygen will be created in inc and not in docs.



	How to run the program

After downloading, create a new folder build:
	mkdir build

Enter in build:
	cd build

Compile the CMake file:
	cmake ..
(Note: the CMakeLists.txt shouldn't be in any folder)

Compile the program
	make

If you want to execute the main program do:
	./NeuronProject

If you want to execute the tests do:
	./Neuron_unittest

If you want to create the Doxygen documentation (html and latex folder) do (always in build):
	make doc

	


	The main program

Program start at 0 milliseconds and ends at 1200 milliseconds.

By default, if you run the program without changing anything, the simulation has g = 5 and the value of the poisson generator = 2. No graphs will be plotted.
You will see on the terminal the progression of the simulation: you will know when the network has been initialized, when the connections have been added and when the simulation has finished. 

If you want to simulate the one neuron's network, go to the main, comment the "sim.networkSimulation(5,2);" line  and uncomment the "sim.oneNeuronSimulation();" line and execute. In the terminal you will see the time of the spikes appearing. Moreover, in build you will find a file.txt named "Datas.txt" containing the values of the membrane potential at each time step.  

If you want to simulate the two neurons' network, go the the main, comment the "sim.networkSimulation(5,2);" line  and uncomment the "sim.twoNeruonsSimulation();" line and execute. In the terminal you will see the time of the spikes appearing and the time when the post-synaptic neuron will receive the spikes. No file is generated.

If you want to plot the graphs, go to the main comment the "sim.networkSimulation(5,2);" line and uncomment the corresponding line of the graph you want to see (for example, for graph A uncomment "sim.plotGraph_A();") and execute.

In the github repository, four examples of the four graphs drawn by python have been uploaded. 
For each graph, to try to simulate the Brunel's network at best, I've changed some parameters in the Graphs.py file. Here I explain how to plot the graphs as I did. 
Graph A: 
	ax1.set_xlim([500,600])

	n, bins, patches = ax2.hist(0.1*data2[0], 1000, normed=0, 		alpha=0.75)

	ax2.set_xlim([500,600])

	ax2.set_ylim([0,10000])

Graph B, C and D: 
	ax1.set_xlim([1000,1200])

	n, bins, patches = ax2.hist(0.1*data2[0], 500, normed=0, 		alpha=0.75)

	ax2.set_xlim([1000,1200])

	ax2.set_ylim([0,6000])




	The tests

When executing the test with googletest, you will see on the terminal the progression of those tests and if they've been passed or not. The role of each different tests is detailed in the Neuron_unittest.cpp file.








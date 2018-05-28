/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "ns3/netanim-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/animation-interface.h"

#include "hleach.h"
#include "const.h" 

#include <vector>
#include <stdio.h>  
#include <stdlib.h>  
#include <math.h>  
#include <ctype.h>  
#include <string.h>  
#include <time.h>  
#include <iterator>

#include <iostream>
#include <fstream>


#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wunused-variable" 
#pragma GCC diagnostic ignored "-Wunused-value" 
#pragma GCC diagnostic ignored "-Wwrite-strings" 
#pragma GCC diagnostic ignored "-Wparentheses" 

using namespace std;
using std::vector;

int NUM_NODES = 50;    // number of nodes in the network    
                       // default is 50  
int NETWORK_X = 100;   // X-size of network  
                       // default is 100  
int NETWORK_Y = 100;   // Y-size of network  
                // default is 100  
double B_POWER = 0.5;   // initial battery power of sensors  
                // default is 0.75  
                  
                // the percentage of the nodes in the   
                // network that would ideally be cluster   
                // heads during any one round of the   
                // LEACH simulation, default is 0.05  
double CLUSTER_PERCENT = 0.3;  
      
                // the total rounds that the simulation  
                // should run for - the network lifetime  
                // default is 2000  
const int TOTAL_ROUNDS = 2000;  
  
                // the distance that the advertisement  
                // phase should broadcast to, in order   
                // to alert other nodes that there is a   
                // cluster head, default is 25.0  
double LEACH_AD_DISTANCE = 25;    
                // the message length of the advertisement  
                // that there is a cluster head  
                // default is 16  
int LEACH_AD_MESSAGE = 16;    
                // the distance for the cluster head to broadcast   
                // the schedule for transmission to the other nodes   
                // in the cluster, default is 25.0  
double SCHEDULE_DISTANCE = 25;    
                // the message length of the schedule which is   
                // sent to the nodes in the cluster during the   
                // scheduling phase of the LEACH simulation,   
                // default is 16.

int SCHEDULE_MESSAGE = 16;  

int BASE_STATION_X_DEFAULT = 300;

int BASE_STATION_Y_DEFAULT = 300;  

int DEAD_NODE = -2;

int MESSAGE_LENGTH = 8;

int TRIALS = 1;

string CHANGE = "Mod_CLUSTER_PERCENT0_3";
  
struct sensor {
     short xLoc;        // X-location of sensor  
     short yLoc;        // Y-location of sensor  
     short lPeriods;        // number of periods the sensor  
                // has been in use for  
     double bCurrent;       // current battery power  
     double bPower;     // initial battery power 
     double pAverage; 

     int round;         // the last round that the sensor   
                // served as a cluster head  
     int head;  // stores the index of the cluster head   
                // for the sensor to transmit to, set to -1 if the   
                // sensor is a cluster head  
     int cluster_members;   // stores the total number of nodes in   
                // the cluster, applicable only for   
                // cluster head nodes  
     int head_count;        // this contains the count of the   
                            // number of times a sensor has been   
                            // the head of a cluster, can be   
                            // removed for optimization later  
     double distance_BASE;
     double distance_current_head;

     int V_round[TOTAL_ROUNDS];
     double V_bPower[TOTAL_ROUNDS];
     int V_head[TOTAL_ROUNDS];
     int V_cluster_members[TOTAL_ROUNDS];
     double V_distanceToHead[TOTAL_ROUNDS];
};  
  
  
struct sensor BASE_STATION;  

struct network_stats{
    int BASE_STATION_X;
    int BASE_STATION_Y;
    int NETWORK_X;
    int NETWORK_Y;
    int NUM_NODES;

    double network_comparison;
    int LEACH_ROUNDS;
    int LEACH_NEW_ROUNDS;
    int DIRECT_ROUNDS;
    int Improvement;
    double CLUSTER_PERCENT;

    //LEACH
    int LEACH_threshold;

    int LEACH_rounds[TOTAL_ROUNDS];
    double LEACH_network_average_energy[TOTAL_ROUNDS];
    double LEACH_network_total_energy[TOTAL_ROUNDS];
    int LEACH_num_dead_node[TOTAL_ROUNDS];
    int LEACH_num_cluster_head[TOTAL_ROUNDS];
    double LEACH_percent_head[TOTAL_ROUNDS];

 //LEACH_NEW
     int LEACH_NEW_rounds[TOTAL_ROUNDS];
    double LEACH_NEW_network_average_energy[TOTAL_ROUNDS];
    double LEACH_NEW_network_total_energy[TOTAL_ROUNDS];
    int LEACH_NEW_num_dead_node[TOTAL_ROUNDS];
    int LEACH_NEW_num_cluster_head[TOTAL_ROUNDS];
    double LEACH_NEW_percent_head[TOTAL_ROUNDS];

    //DIRECT
    int DIRECT_rounds[TOTAL_ROUNDS];
    double DIRECT_network_average_energy[TOTAL_ROUNDS];
    double DIRECT_network_total_energy[TOTAL_ROUNDS];
    int DIRECT_num_dead_node[TOTAL_ROUNDS];
};

  
double computeEnergyTransmit(float distance, int messageLength);  
// computes the energy needed to transmit the message  
// inputs are the distance between the two nodes and  
// the length of the message  
  
double computeEnergyReceive(int messageLength);  
// computes the energy needed to receive a message   
// input is the message length  
  
void initializeNetwork(struct sensor network[]);  
// initializes the network; randomly places nodes within   
// the grid and sets battery power to default value  
  
double averageEnergy(struct sensor network[]);  
// computes the average power of the sensors in the   
// network as a percentage of starting power  
// input is a sensor network  
  
double totalEnergy(struct sensor network[]);

int runLeachSimulation_New(const struct sensor network[], struct network_stats * new_network_stats);  
int runLeachSimulation(const struct sensor network[], struct network_stats * new_network_stats);  
// takes an initialized sensor network and simulates   
// the LEACH protocol on that network  
// The network is unchanged so that the same network   
// can be passed to other simulator functions  
// The function returns the number of rounds for which the   
// average power of the network remained above the threshold.  

int runDirectSimulation(const struct sensor network[], struct network_stats * new_network_stats);  
// takes an initialized sensor network and simulates  
// the direct transmission protocol wherein all sensors   
// transmit directly to the base station. The network is   
// unchanged so other protocols can be run on it  
// The function returns the number of rounds for which the   
// average power of the network remained above the threshold.  
    
int sensorTransmissionChoice(const struct sensor a);  
// using the NPP, this function determines whether sensor a  
// should transmit in the current round based on the expected  
// number of rounds the network will be in use and the average   
// energy the sensor has used per round thus far in the simulation.  
// Returns 1 if the sensor should transmit, 0 otherwise. 
// 
void initializeNetworkStats(struct network_stats network_stats_data[], int TRIALS);  

void exportSensorData(struct sensor * network[], std::string label);

void exportNetworkData(struct network_stats new_network_stats[], std::string label);

int main(int argc, char * argv[])  
{  
    cout << "main " << endl;
    struct sensor * network = (struct sensor *) malloc(NUM_NODES * sizeof(struct sensor));
    cout << "sensor " << endl;

    TRIALS = 1;

    struct network_stats * network_stats_data = (struct network_stats *) malloc(TRIALS * sizeof(struct network_stats)); 

    cout << "network_stats " << endl;    
    int i = 0;  
    //int j = 0;  
    int rounds_LEACH = 0;  
    int rounds_DIRECT = 0;  
    int rounds_LEACH_NEW = 0;

    BASE_STATION.xLoc = BASE_STATION_X_DEFAULT;  
    BASE_STATION.yLoc = BASE_STATION_Y_DEFAULT;

      
    initializeNetwork(network);  
        cout << "initializeNetwork " << endl;    
    initializeNetworkStats(network_stats_data,TRIALS);

    cout << "initializeNetworkStats " << endl;  
    for(i = 0; i < TRIALS; i++){  
        rounds_LEACH = runLeachSimulation(network,&network_stats_data[i]);
		cout << "runLeachSimulation " << endl; 
        rounds_LEACH_NEW= runLeachSimulation_New(network,&network_stats_data[i]);  
        cout << "runLeachSimulation_NEW " << endl; 
	rounds_DIRECT = 1;
        //rounds_DIRECT = runDirectSimulation(network,&network_stats_data[i]);  
        cout << "runDirectSimulation " << endl; 
        printf("\n");
        printf("The LEACH simulation was able to remain viable for %d rounds\n", rounds_LEACH);
        network_stats_data[i].BASE_STATION_X = BASE_STATION_X_DEFAULT;
        network_stats_data[i].BASE_STATION_Y = BASE_STATION_Y_DEFAULT;
        network_stats_data[i].NETWORK_X = NETWORK_X;
        network_stats_data[i].NETWORK_Y = NETWORK_Y;
        network_stats_data[i].NUM_NODES = NUM_NODES;

        network_stats_data[i].LEACH_ROUNDS = rounds_LEACH;
        network_stats_data[i].LEACH_ROUNDS = rounds_LEACH_NEW;
        network_stats_data[i].DIRECT_ROUNDS = rounds_DIRECT;
        network_stats_data[i].CLUSTER_PERCENT = CLUSTER_PERCENT;

        double improvement = (100.0-((double)rounds_DIRECT/(double)rounds_LEACH)*100);

        printf("The direct transmission simulation was able to remain viable for %d rounds\n", rounds_DIRECT);
        printf("This is an improvement of %f%%\n", improvement);
        network_stats_data[i].Improvement = improvement;

        string label = CHANGE;

        exportNetworkData(network_stats_data,label);
        
        if(i < TRIALS - 1){
        	initializeNetwork(network);  
        }
        
    }  
  
    return 0;    
}           // end main function  
  
 int runLeachSimulation_New(const struct sensor network[], struct network_stats * new_network_stats){  
	// Preconditions:   the network variable contains an initiailized    
	//          sensor network and all global variables have been   
	//          set using the loadConfiguration function. The simulation   
	//          runs a number of times equal to the TOTAL_ROUNDS variable.  
	// Postconditions: the LEACH protocol simulation has been   
	//              run on the supplied network although the instance   
	//              of that variable within main memory has NOT been   
	//              modified. Output of statistics is currently to the   
	//              screen.  
	  
	struct sensor * network_LEACH;      // wireless sensor network to run sim on 
	  
	// indexing variables  
	int i = 0;                          
	int j = 0;  
	int k = 0;  
	int closest = 0;  
	  
	int round = 0;                  // current round  
	int failed_transmit = 0;        // round where a failed transmission occurred  
	  
	int testing = 0;            // testing variable, TO BE REMOVED  
	int bits_transmitted = 0;       // count of bits transmitted  
	int power = FALSE;  
	int temp_cluster_members = 0;  
	  
	double average_energy = 0.0;  
	double distance_X_old = 0.0;  
	double distance_Y_old = 0.0;  
	double distance_old = 0.0;  
	double distance_X_new = 0.0;  
	double distance_Y_new = 0.0;  
	double distance_new = 0.0;  
	int recent_round = 1/CLUSTER_PERCENT;  
	double threshold = CLUSTER_PERCENT/(1-(CLUSTER_PERCENT*(round % recent_round)));
	double random_number;  
	int cluster_head_count = 0;  
	double percent_head = 0.0; 
	double mid_value = 0.0;
	bool flag = 1 ;
	int NUM_DEAD_NODE = 0;

	double AVE_ENERGY = 0;

	vector<double> dir;
	//old :: network_LEACH = (struct sensor *) malloc(NUM_NODES * sizeof(struct sensor)); 
	network_LEACH = (struct sensor *) malloc(NUM_NODES * sizeof(struct sensor));  
	network_LEACH = new struct sensor[NUM_NODES];  
	// copy the contents of the passed network to a temporary   
	// network so the same network can be passed to different   
	// protocol simulations  
	  
	  
	for(i = 0; i  < NUM_NODES; i++){  
	    network_LEACH[i].bPower = network[i].bPower;      
	    network_LEACH[i].xLoc = network[i].xLoc;  
	    network_LEACH[i].cluster_members = 0;  
	    network_LEACH[i].yLoc = network[i].yLoc;   
	    network_LEACH[i].bCurrent = network[i].bCurrent;     

	    //statistic
	    network_LEACH[i].V_round[round] = 0;
	    network_LEACH[i].V_bPower[round] = network[i].bPower;
	    network_LEACH[i].V_cluster_members[round] = 0;
	    network_LEACH[i].V_head[round] = -3;
	    network_LEACH[i].V_distanceToHead[round] = 0;
	}  

	  
	printf("\nRunning the LEACH Transmission Simulation \n");  
	// our iterating loop runs of total rounds, this is   
	// the expected lifetime of the network  
	AVE_ENERGY = averageEnergy(network_LEACH);
	(*new_network_stats).LEACH_threshold = threshold;

	while(AVE_ENERGY > .10 && round < TOTAL_ROUNDS){

		    // here we recalculate all the variables   
		    // which are round dependent

		    cluster_head_count = 0;
		    NUM_DEAD_NODE = 0;  
		    // advertisement phase  
		    // we determine which nodes will be cluster heads
	        //   
			for(i = 0; i < NUM_NODES; i++){  
					if((network_LEACH[i].round + recent_round < round) || (network_LEACH[i].round == 0)){  
					    if(network_LEACH[i].head != DEAD_NODE){  
						random_number = .00001*(rand() % 100000);          
						//cout << "random_number " << random_number << endl;
			    			if(random_number <= threshold && network_LEACH[i].bCurrent > 0.03){  
			    			    // the random number selected is less   
			    			    // than the threshold so the node becomes   
			    			    // a cluster head for the round  
			    			    network_LEACH[i].head_count++;
			    			    // update the round variable   
			    			    // so we know that this sensor was   
			    			    // last a cluster head at round i  
			    			    network_LEACH[i].round = round;  
			    			    network_LEACH[i].head = -1;

			    			    // store the index of the node in the   
			    			    // cluster_heads array  
			    			    // increment the cluster_head_count  
			    			    cluster_head_count++; 
			    			}  
					    }  
					}

			} 
			 
			(*new_network_stats).LEACH_NEW_num_cluster_head[round] = cluster_head_count;
		    (*new_network_stats).LEACH_NEW_percent_head[round] = ((double)cluster_head_count/(double)NUM_NODES);
		    // now the cluster heads must transmit the fact that they   
		    // are cluster heads to the network, this will be a constant   
		    // transmit energy, during this period the other nodes must   
		    // keep their receivers on - which has an energy cost, again   
		    // this is constant  
		    for(i = 0; i  < NUM_NODES; i++){  
		        if(network_LEACH[i].head == -1){  
		            network_LEACH[i].bCurrent -=   
		                computeEnergyTransmit(LEACH_AD_DISTANCE,  
		                            LEACH_AD_MESSAGE);  
		        }  
		        else{  
		            network_LEACH[i].bCurrent -=   
		                computeEnergyReceive(LEACH_AD_MESSAGE);  
		        }  
		    }  
		  
			// CLUSTER SET-UP PHASE  
		    // for this phase, all non-cluster heads determine to which   
		    // cluster they will broadcast and alert the cluster head to that  
		  
		  
		    for(i = 0; i  < NUM_NODES; i++){  
		            closest = -1;  
		            if((network_LEACH[i].head != -1) &&   
		                network_LEACH[i].head != DEAD_NODE){  
		                    // if the node's round is not equal to the    
		                    // current round, the node is not a cluster  
		                    // head and we must find a cluster head for  
		                    // the node to transmit to  
		            for(k = 0; k < NUM_NODES; k++){  
		                        if(network_LEACH[k].head == -1 && closest != -1){  
		                            distance_X_old = network_LEACH[i].xLoc - network_LEACH[closest].xLoc;  
		                            distance_Y_old = network_LEACH[i].yLoc - network_LEACH[closest].yLoc;  
		                            distance_old = sqrt(pow(distance_X_old, 2) + pow(distance_Y_old, 2));  
		                            distance_X_new = network_LEACH[i].xLoc - network_LEACH[k].xLoc;  
		                            distance_Y_new = network_LEACH[i].yLoc - network_LEACH[k].yLoc;  
		                            distance_new = sqrt(pow(distance_X_new, 2) + pow(distance_Y_new, 2));  
		                            if(distance_new < distance_old)  
		                                closest = k;  
		                        }  
		                        else if(network_LEACH[k].head == -1 && closest == -1){  
		                            closest = k;  
		                        }  
		            }  
		                      
		            network_LEACH[i].head = closest;  
		            network_LEACH[closest].cluster_members++;  
		        }
		        
		    }

		     for(i = 0; i <= NUM_NODES; i++){ 
		      	network_LEACH[i].V_head[round] = network_LEACH[i].head;
			    network_LEACH[i].V_cluster_members[round] = network_LEACH[i].cluster_members;  
		     }

			// SCHEDULE CREATION  
		    // the cluster head then creates a schedule telling the nodes   
		    // when they can transmit; the schedule is broadcast to the nodes  
		    // but no simulation of this is neccescary outside of the loss   
		    // of battery power for broadcasting the schedule, which is a constant  
		    for(i = 0; i <= NUM_NODES; i++){  
		        if(network_LEACH[i].head == -1){  
		            // if the node is going to be a cluster head, it transmits   
		            // the schedule to the other nodes  
		            network_LEACH[i].bCurrent -=   
		                computeEnergyTransmit(SCHEDULE_DISTANCE, SCHEDULE_MESSAGE);  
		        }  
		        else  
		            network_LEACH[i].bCurrent -=   
		                computeEnergyReceive(SCHEDULE_MESSAGE);  
		    }  

			// DATA TRANSMISSION  
		    // non cluster heads send their data to the cluster heads who then   
		    // broadcast the data to the base station  
		    for(i = 0; i < NUM_NODES; i++){ 

		        network_LEACH[i].lPeriods++;

		        if(network_LEACH[i].head != -1){ 

		                distance_X_new = network_LEACH[i].xLoc - network_LEACH[network_LEACH[i].head].xLoc;  
		                distance_Y_new = network_LEACH[i].yLoc - network_LEACH[network_LEACH[i].head].yLoc;  
		                distance_new = sqrt((pow(distance_X_new, 2) + pow(distance_Y_new, 2)));

		                network_LEACH[i].V_distanceToHead[round] = (distance_new);

		                if(network_LEACH[i].head != DEAD_NODE){
		                	network_LEACH[i].bCurrent -= computeEnergyTransmit(distance_new, MESSAGE_LENGTH);  
		                	network_LEACH[network_LEACH[i].head].bCurrent -= computeEnergyReceive(MESSAGE_LENGTH);

		                	if(network_LEACH[i].bCurrent < 0.0 && network_LEACH[i].head != -1){  
		                    		network_LEACH[i].head = DEAD_NODE; 
		               		 } 
		                } 
		        }
		        else{
		        	network_LEACH[i].V_distanceToHead[round] = 0;
		        }  
		    }  
		  
		  
		    for(i = 0; i <= NUM_NODES; i++){  
		        if(network_LEACH[i].head == -1){  

			    	double energy_enough = network_LEACH[i].bCurrent - computeEnergyTransmit(network_LEACH[i].distance_BASE, (MESSAGE_LENGTH * (network_LEACH[i].cluster_members+1)));
			    
				//network_LEACH[i].bCurrent -= computeEnergyTransmit(distance_new,(MESSAGE_LENGTH * (network_LEACH[i].cluster_members+1)));  
				//if(network_LEACH[i].bCurrent > 0.0){  
			      	if(energy_enough > 0.0){

						network_LEACH[i].bCurrent -= computeEnergyTransmit(network_LEACH[i].distance_BASE,(MESSAGE_LENGTH * (network_LEACH[i].cluster_members+1)));
		                bits_transmitted += (MESSAGE_LENGTH * (network_LEACH[i].cluster_members+1));  
		         	}  
		            else
		            {  
		                failed_transmit++;    
		            }  
		        }  
		    }   
		    // round has completed, increment the round count

		    for(i = 0; i <= NUM_NODES; i++){  
		    	network_LEACH[i].V_bPower[round] = network_LEACH[i].bCurrent;
		        network_LEACH[i].cluster_members = 0;  
		       if(network_LEACH[i].bCurrent > 0.0)
		       {
		       		network_LEACH[i].head = -3;  
		       }
		       else
		       {
		       		network_LEACH[i].head = DEAD_NODE;
		       		NUM_DEAD_NODE++;		       		
		       }
		      network_LEACH[i].V_round[round] = round;
		    }

		   
		    (*new_network_stats).LEACH_rounds[round] = round;


		    (*new_network_stats).LEACH_num_dead_node[round] = NUM_DEAD_NODE; 
		    

		    cluster_head_count = 0;  

		    AVE_ENERGY = averageEnergy(network_LEACH);

		    (*new_network_stats).LEACH_NEW_network_average_energy[round] = AVE_ENERGY;
		    (*new_network_stats).LEACH_NEW_network_total_energy[round] = totalEnergy(network_LEACH);

		    if(round % 5000 == 1 )
		    {
		    	cout  << "network_pwr: " << AVE_ENERGY << endl;	
		    }


	    	round += 1;
	 } 

	cout << "LEACH_NEW: " << round << endl;

	string label = "LEACH_NEW" + CHANGE;
	exportSensorData(&network_LEACH,label);	
	//free(network_LEACH);  
	printf("The average energy of the network remained above 10%% for %d tranmission periods\n", round);

    return round;  
}       // end runLeachSimulation function  
  
  
int runLeachSimulation(const struct sensor network[], struct network_stats * new_network_stats){  
	// Preconditions:   the network variable contains an initiailized    
	//          sensor network and all global variables have been   
	//          set using the loadConfiguration function. The simulation   
	//          runs a number of times equal to the TOTAL_ROUNDS variable.  
	// Postconditions: the LEACH protocol simulation has been   
	//              run on the supplied network although the instance   
	//              of that variable within main memory has NOT been   
	//              modified. Output of statistics is currently to the   
	//              screen.  
	  
	struct sensor * network_LEACH;      // wireless sensor network to run sim on 
	  
	// indexing variables  
	int i = 0;                          
	int j = 0;  
	int k = 0;  
	int closest = 0;  
	  
	int round = 0;                  // current round  
	int failed_transmit = 0;        // round where a failed transmission occurred  
	  
	int testing = 0;            // testing variable, TO BE REMOVED  
	int bits_transmitted = 0;       // count of bits transmitted  
	int power = FALSE;  
	int temp_cluster_members = 0;  
	  
	double average_energy = 0.0;  
	double distance_X_old = 0.0;  
	double distance_Y_old = 0.0;  
	double distance_old = 0.0;  
	double distance_X_new = 0.0;  
	double distance_Y_new = 0.0;  
	double distance_new = 0.0;  
	int recent_round = 1/CLUSTER_PERCENT;  
	double threshold = CLUSTER_PERCENT/(1-(CLUSTER_PERCENT*(round % recent_round)));
	double random_number;  
	int cluster_head_count = 0;  
	double percent_head = 0.0; 
	double mid_value = 0.0;
	bool flag = 1 ;
	int NUM_DEAD_NODE = 0;

	double AVE_ENERGY = 0;

	vector<double> dir;
	//old :: network_LEACH = (struct sensor *) malloc(NUM_NODES * sizeof(struct sensor)); 
	network_LEACH = (struct sensor *) malloc(NUM_NODES * sizeof(struct sensor));  
	network_LEACH = new struct sensor[NUM_NODES];  
	// copy the contents of the passed network to a temporary   
	// network so the same network can be passed to different   
	// protocol simulations  
	  
	  
	for(i = 0; i  < NUM_NODES; i++){  
	    network_LEACH[i].bPower = network[i].bPower;      
	    network_LEACH[i].xLoc = network[i].xLoc;  
	    network_LEACH[i].cluster_members = 0;  
	    network_LEACH[i].yLoc = network[i].yLoc;   
	    network_LEACH[i].bCurrent = network[i].bCurrent;     

	    //statistic
	    network_LEACH[i].V_round[round] = 0;
	    network_LEACH[i].V_bPower[round] = network[i].bPower;
	    network_LEACH[i].V_cluster_members[round] = 0;
	    network_LEACH[i].V_head[round] = -3;
	    network_LEACH[i].V_distanceToHead[round] = 0;
	}  

	  
	printf("\nRunning the LEACH Transmission Simulation \n");  
	// our iterating loop runs of total rounds, this is   
	// the expected lifetime of the network  
	AVE_ENERGY = averageEnergy(network_LEACH);
	(*new_network_stats).LEACH_threshold = threshold;

	while(AVE_ENERGY > .10 && round < TOTAL_ROUNDS){

		    // here we recalculate all the variables   
		    // which are round dependent

		    cluster_head_count = 0;
		    NUM_DEAD_NODE = 0;  
		    // advertisement phase  
		    // we determine which nodes will be cluster heads
	        //   
			for(i = 0; i < NUM_NODES; i++){  
					if((network_LEACH[i].round + recent_round < round) || (network_LEACH[i].round == 0)){  
					    if(network_LEACH[i].head != DEAD_NODE){  
						random_number = .00001*(rand() % 100000);          
						//cout << "random_number " << random_number << endl;
			    			if(random_number <= threshold){  
			    			    // the random number selected is less   
			    			    // than the threshold so the node becomes   
			    			    // a cluster head for the round  
			    			    network_LEACH[i].head_count++;
			    			    // update the round variable   
			    			    // so we know that this sensor was   
			    			    // last a cluster head at round i  
			    			    network_LEACH[i].round = round;  
			    			    network_LEACH[i].head = -1;

			    			    // store the index of the node in the   
			    			    // cluster_heads array  
			    			    // increment the cluster_head_count  
			    			    cluster_head_count++; 
			    			}  
					    }  
					}

			} 
			 
			(*new_network_stats).LEACH_num_cluster_head[round] = cluster_head_count;
		    (*new_network_stats).LEACH_percent_head[round] = ((double)cluster_head_count/(double)NUM_NODES);
		    // now the cluster heads must transmit the fact that they   
		    // are cluster heads to the network, this will be a constant   
		    // transmit energy, during this period the other nodes must   
		    // keep their receivers on - which has an energy cost, again   
		    // this is constant  
		    for(i = 0; i  < NUM_NODES; i++){  
		        if(network_LEACH[i].head == -1){  
		            network_LEACH[i].bCurrent -=   
		                computeEnergyTransmit(LEACH_AD_DISTANCE,  
		                            LEACH_AD_MESSAGE);  
		        }  
		        else{  
		            network_LEACH[i].bCurrent -=   
		                computeEnergyReceive(LEACH_AD_MESSAGE);  
		        }  
		    }  
		  
			// CLUSTER SET-UP PHASE  
		    // for this phase, all non-cluster heads determine to which   
		    // cluster they will broadcast and alert the cluster head to that  
		  
		  
		    for(i = 0; i  < NUM_NODES; i++){  
		            closest = -1;  
		            if((network_LEACH[i].head != -1) &&   
		                network_LEACH[i].head != DEAD_NODE){  
		                    // if the node's round is not equal to the    
		                    // current round, the node is not a cluster  
		                    // head and we must find a cluster head for  
		                    // the node to transmit to  
		            for(k = 0; k < NUM_NODES; k++){  
		                        if(network_LEACH[k].head == -1 && closest != -1){  
		                            distance_X_old = network_LEACH[i].xLoc - network_LEACH[closest].xLoc;  
		                            distance_Y_old = network_LEACH[i].yLoc - network_LEACH[closest].yLoc;  
		                            distance_old = sqrt(pow(distance_X_old, 2) + pow(distance_Y_old, 2));  
		                            distance_X_new = network_LEACH[i].xLoc - network_LEACH[k].xLoc;  
		                            distance_Y_new = network_LEACH[i].yLoc - network_LEACH[k].yLoc;  
		                            distance_new = sqrt(pow(distance_X_new, 2) + pow(distance_Y_new, 2));  
		                            if(distance_new < distance_old)  
		                                closest = k;  
		                        }  
		                        else if(network_LEACH[k].head == -1 && closest == -1){  
		                            closest = k;  
		                        }  
		            }  
		                      
		            network_LEACH[i].head = closest;  
		            network_LEACH[closest].cluster_members++;  
		        }
		        
		    }

		     for(i = 0; i <= NUM_NODES; i++){ 
		      	network_LEACH[i].V_head[round] = network_LEACH[i].head;
			    network_LEACH[i].V_cluster_members[round] = network_LEACH[i].cluster_members;  
		     }

			// SCHEDULE CREATION  
		    // the cluster head then creates a schedule telling the nodes   
		    // when they can transmit; the schedule is broadcast to the nodes  
		    // but no simulation of this is neccescary outside of the loss   
		    // of battery power for broadcasting the schedule, which is a constant  
		    for(i = 0; i <= NUM_NODES; i++){  
		        if(network_LEACH[i].head == -1){  
		            // if the node is going to be a cluster head, it transmits   
		            // the schedule to the other nodes  
		            network_LEACH[i].bCurrent -=   
		                computeEnergyTransmit(SCHEDULE_DISTANCE, SCHEDULE_MESSAGE);  
		        }  
		        else  
		            network_LEACH[i].bCurrent -=   
		                computeEnergyReceive(SCHEDULE_MESSAGE);  
		    }  

			// DATA TRANSMISSION  
		    // non cluster heads send their data to the cluster heads who then   
		    // broadcast the data to the base station  
		    for(i = 0; i < NUM_NODES; i++){ 

		        network_LEACH[i].lPeriods++;

		        if(network_LEACH[i].head != -1){ 

		                distance_X_new = network_LEACH[i].xLoc - network_LEACH[network_LEACH[i].head].xLoc;  
		                distance_Y_new = network_LEACH[i].yLoc - network_LEACH[network_LEACH[i].head].yLoc;  
		                distance_new = sqrt((pow(distance_X_new, 2) + pow(distance_Y_new, 2)));

		                network_LEACH[i].V_distanceToHead[round] = (distance_new);

		                if(network_LEACH[i].head != DEAD_NODE){
		                	network_LEACH[i].bCurrent -= computeEnergyTransmit(distance_new, MESSAGE_LENGTH);  
		                	network_LEACH[network_LEACH[i].head].bCurrent -= computeEnergyReceive(MESSAGE_LENGTH);

		                	if(network_LEACH[i].bCurrent < 0.0 && network_LEACH[i].head != -1){  
		                    		network_LEACH[i].head = DEAD_NODE; 
		               		 } 
		                } 
		        }
		        else{
		        	network_LEACH[i].V_distanceToHead[round] = 0;
		        }  
		    }  
		  
		  
		    for(i = 0; i <= NUM_NODES; i++){  
		        if(network_LEACH[i].head == -1){  

			    	double energy_enough = network_LEACH[i].bCurrent - computeEnergyTransmit(network_LEACH[i].distance_BASE, (MESSAGE_LENGTH * (network_LEACH[i].cluster_members+1)));
			    
				//network_LEACH[i].bCurrent -= computeEnergyTransmit(distance_new,(MESSAGE_LENGTH * (network_LEACH[i].cluster_members+1)));  
				//if(network_LEACH[i].bCurrent > 0.0){  
			      	if(energy_enough > 0.0){

						network_LEACH[i].bCurrent -= computeEnergyTransmit(network_LEACH[i].distance_BASE,(MESSAGE_LENGTH * (network_LEACH[i].cluster_members+1)));
		                bits_transmitted += (MESSAGE_LENGTH * (network_LEACH[i].cluster_members+1));  
		         	}  
		            else
		            {  
		                failed_transmit++;    
		            }  
		        }  
		    }   
		    // round has completed, increment the round count

		    for(i = 0; i <= NUM_NODES; i++){  
		    	network_LEACH[i].V_bPower[round] = network_LEACH[i].bCurrent;
		        network_LEACH[i].cluster_members = 0;  
		       if(network_LEACH[i].bCurrent > 0.0)
		       {
		       		network_LEACH[i].head = -3;  
		       }
		       else
		       {
		       		network_LEACH[i].head = DEAD_NODE;
		       		NUM_DEAD_NODE++;		       		
		       }
		      network_LEACH[i].V_round[round] = round;
		    }

		   
		    (*new_network_stats).LEACH_rounds[round] = round;


		    (*new_network_stats).LEACH_num_dead_node[round] = NUM_DEAD_NODE; 
		    

		    cluster_head_count = 0;  

		    AVE_ENERGY = averageEnergy(network_LEACH);

		    (*new_network_stats).LEACH_network_average_energy[round] = AVE_ENERGY;
		    (*new_network_stats).LEACH_network_total_energy[round] = totalEnergy(network_LEACH);

		    if(round % 5000 == 1 )
		    {
		    	cout  << "network_pwr: " << AVE_ENERGY << endl;	
		    }


	    	round += 1;
	 } 

	cout << "LEACH: " << round << endl;

	string label = "LEACH_" + CHANGE;
	exportSensorData(&network_LEACH,label);	
	//free(network_LEACH);  
	printf("The average energy of the network remained above 10%% for %d tranmission periods\n", round);

    return round;  
}       // end runLeachSimulation function  
  
  
  
  
int runDirectSimulation(const struct sensor network[], struct network_stats * new_network_stats){  
	// Preconditions:   the network variable contains an initiailized    
	//          sensor network and all global variables have been   
	//          set using the loadConfiguration function. The simulation   
	//          runs a number of times equal to the TOTAL_ROUNDS variable.  
	// Postconditions:      the direct transmission simulation has been   
	//              run on the supplied network although the instance   
	//          of that variable within main memory has NOT been   
	//          modified. Output of statistics is currently to the   
	//          screen.  
	  
	int j = 0;  
	int i = 0;  
	int round = 0;  
	int bits_transmitted = 0;  

	int nodes_transmitting = 0;  
	int failed_transmission = 0;  
	float distance_X = 0.0;  
	float distance_Y = 0.0;  
	float distance = 0.0;  

	struct sensor * network_DIRECT;  
	bool flag = 1;

	double AVE_ENERGY = 0;

  	int NUM_DEAD_NODE = 0;

	network_DIRECT = (struct sensor *) malloc(60 * sizeof(struct sensor));  

	for(i = 0; i < NUM_NODES; i++){  
	    network_DIRECT[i].bPower = network[i].bPower;  
	    network_DIRECT[i].xLoc = network[i].xLoc;
	    network_DIRECT[i].yLoc = network[i].yLoc;

	    network_DIRECT[i].head = 0; // use head to label if the node is dead or not
	    network_DIRECT[i].bCurrent = network[i].bCurrent;

	    distance_X = network_DIRECT[i].xLoc - BASE_STATION.xLoc;  
		distance_Y = network_DIRECT[i].yLoc - BASE_STATION.yLoc;  
		distance = sqrt(pow(distance_X, 2) + pow(distance_Y,2));  

	    network_DIRECT[i].distance_BASE = distance;
	    network_DIRECT[i].pAverage = computeEnergyTransmit(distance, MESSAGE_LENGTH);

		network_DIRECT[i].V_round[round] = round;
	    network_DIRECT[i].V_bPower[round] = network_DIRECT[i].bPower;


	}  
  
    printf("\nRunning the Direct Transmission Simulation \n");  
      
	while(averageEnergy(network_DIRECT) > .10 && round < TOTAL_ROUNDS){  
	 	NUM_DEAD_NODE = 0;
		for(i = 0; i < NUM_NODES; i++) {  
		// cycle through all nodes in network and attempt to   
		// transmit  
			if(network_DIRECT[i].bCurrent > 0 && network_DIRECT[i].head != DEAD_NODE){  
				    nodes_transmitting++;  

					 if(network_DIRECT[i].pAverage <= network_DIRECT[i].bCurrent){  
						bits_transmitted += MESSAGE_LENGTH;  
						network_DIRECT[i].bCurrent -= network_DIRECT[i].pAverage;
					 }  
					 else
					 { 
						failed_transmission++;

						network_DIRECT[i].head = DEAD_NODE;
						NUM_DEAD_NODE++;	
					}

				 
					if(network_DIRECT[i].bCurrent < 0)
					{  
						network_DIRECT[i].bCurrent = 0;
						network_DIRECT[i].head = DEAD_NODE;
						NUM_DEAD_NODE++;  
					}

					network_DIRECT[i].V_round[round] = round;
					network_DIRECT[i].V_bPower[round] = network_DIRECT[i].bCurrent;

			}

			
		}

		AVE_ENERGY = averageEnergy(network_DIRECT);

		if(round % 5000  == 1) 
		{
			cout  << "network_pwr: " << AVE_ENERGY << endl; 
		}

		(*new_network_stats).DIRECT_rounds[round] = round;
		(*new_network_stats).DIRECT_network_average_energy[round] = AVE_ENERGY;
		(*new_network_stats).DIRECT_num_dead_node[round] = NUM_DEAD_NODE;
		(*new_network_stats).DIRECT_network_total_energy[round] = totalEnergy(network_DIRECT);


		round += 1;

	}
	cout  << "CHECK1: "<< endl; 
	string label = "DIRECT_" + CHANGE;
	cout  << "CHECK2: "<< endl; 
	exportSensorData(&network_DIRECT,label);
	//free(network_DIRECT);  
	return round;  
}           // end runDirectSimulation function

  
  
void initializeNetwork(struct sensor network[]) {  
// Preconditions:   network is an unitialized sensor network.  
// Postconditions:  network is an initialized sensor network   
//                  whose values are loaded from global varibles   
//          which are set using the loadConfiguration   
//          function or the defaults are used  
    int i = 0;  
    srand((unsigned int) time(0));
    double distance_X_new =0.0;
	double distance_Y_new =0.0;
    double distance_new =0.0;
          
    for(i = 0; i < NUM_NODES; i++) {  
                network[i].xLoc = rand() % NETWORK_X;  
                network[i].yLoc = rand() % NETWORK_Y;

                distance_X_new = network[i].xLoc - BASE_STATION.xLoc;  
            	distance_Y_new = network[i].yLoc - BASE_STATION.yLoc;  
            	distance_new = sqrt(pow(distance_X_new, 2) + pow(distance_Y_new, 2));  

            	network[i].distance_BASE = distance_new;

                network[i].lPeriods = 0;  
                network[i].bCurrent = B_POWER;  
                network[i].bPower = B_POWER;  
		        network[i].round = 0;  
		        network[i].head = -3;  
    }  
       
}// end initializeNetwork function  


void initializeNetworkStats(struct network_stats network_stats_data[],int TRIALS){
	// Preconditions:   network is an unitialized sensor network.  
// Postconditions:  network is an initialized sensor network   
//                  whose values are loaded from global varibles   
//          which are set using the loadConfiguration   
//          function or the defaults are used  
    int i = 0;
          
    for(i = 0; i < TRIALS; i++) {  
	    network_stats_data[i].BASE_STATION_X = 0;
	    network_stats_data[i].BASE_STATION_Y = 0;
	    network_stats_data[i].NETWORK_X = 0;
	    network_stats_data[i].NETWORK_Y = 0;
	    network_stats_data[i].NUM_NODES = 0;

	    network_stats_data[i].network_comparison = 0;
	    network_stats_data[i].LEACH_ROUNDS = 0;
	    network_stats_data[i].DIRECT_ROUNDS = 0;
	    network_stats_data[i].Improvement = 0;

	    //LEACH


	    network_stats_data[i].LEACH_threshold = 0;  

    }
}// end initializeNetwork function  


//                  node and the receiving node. messageLength contains   
//                  the length of the message in bits.  
// Postconditions:  the total energy consumed by the transmission is   
//                  returned  

double computeEnergyTransmit(float distance, int messageLength){  
// Preconditions:   distance contains the distance between the transmitting    
    float E_elec = 50 * pow(10,-9);  
    float epsilon_amp = 100 * pow(10,-12);  
    double EnergyUse = 0.00;  
          
    EnergyUse = (messageLength * E_elec) + (messageLength * epsilon_amp * pow(distance,2));   
      
    return EnergyUse;  
  
}           // end computeEnergyTransmit function  
          
double computeEnergyReceive(int messageLength) {  
// Preconditions:   messageLength contains the length of the  
//                  message in bits to be received by the station.  
// Postconditions:  the total energy consumed by the reception   
//              of the transmission is returned.      
    return (messageLength * (50 * pow(10,-9)));  
}           // end computeEnergyReceive function  


double averageEnergy(struct sensor network[]) {  
// Preconditions:   network is an initialized sensor network  
// Postconditions:  the average percentage of power in the   
//                  batteries across the network is returned.  
//                  i.e. 0.90 means that the average battery   
//                  in the network is at 90% of its original   
//                  power  
    float average = 0.0;  
    float starting_power = 0.00;  
    float current_power = 0.00;  
    int i = 0;  
  
    for(i = 0; i <= NUM_NODES; i++) {  
        starting_power += network[i].bPower;  
        current_power += network[i].bCurrent;  
    }  
  
    return current_power/starting_power;  
  
}           // end averageEnergy function 

double totalEnergy(struct sensor network[]) {  
// Preconditions:   network is an initialized sensor network  
// Postconditions:  the average percentage of power in the   
//                  batteries across the network is returned.  
//                  i.e. 0.90 means that the average battery   
//                  in the network is at 90% of its original   
//                  power   
    double total = 0.00;  
    int i = 0;  
  
    for(i = 0; i <= NUM_NODES; i++) {  
        total += network[i].bCurrent;  
    }  
  
    return total;  
  
} // end totalEnergy function  

void exportSensorData(struct sensor * network[], std::string label)
{	cout  << "CHECK3: "<< endl; 
	ofstream SensorData;
	string name1 = "SensorData_";
	string filename = name1 + label;
	int row = 0;
	int col = 0;
	SensorData.open(filename);
	for (col = 0; col < TOTAL_ROUNDS; col++)
	{
		for(row = 0; row < NUM_NODES;row++)
		{
			SensorData << (*network)[row].V_round[col] << "," << (*network)[row].V_bPower[col]  << "," << (*network)[row].V_head[col]  << "," << (*network)[row].V_cluster_members[col]  << "," << (*network)[row].V_distanceToHead[col] << ",";
		}

		SensorData << endl;

	}

	SensorData.close();
}

void exportNetworkData(struct network_stats new_network_stats[], std::string label)
{
	ofstream NetworkData_LEACH;
	ofstream NetworkData_DIRECT;
	ofstream NetworkData_Parameters;
	ofstream NetworkData_LEACH_NEW;

	string name1 = "NetworkData_LEACH_";
	string name2 = "NetworkData_DIRECT_";
	string name4 = "NetworkData_LEACH_NEW_";
	string name3 = "NetworkData_Parameters_";
	string filename1 = name1 + label;
	string filename2 = name2 + label;
	string filename3 = name3 + label;
	string filename4 = name4 + label;
	int row = 0;
	int col = 0;

	NetworkData_LEACH.open(filename1);
	NetworkData_DIRECT.open(filename2);
	NetworkData_Parameters.open(filename3);
	NetworkData_LEACH_NEW.open(filename4);

	for (col = 0; col < TOTAL_ROUNDS; col++)
	{
		for(row = 0; row < TRIALS;row++)
		{
			

			NetworkData_LEACH << new_network_stats[row].LEACH_rounds[col] << "," << new_network_stats[row].LEACH_network_average_energy[col]  << "," << new_network_stats[row].LEACH_network_total_energy[col]  << "," << new_network_stats[row].LEACH_num_dead_node[col]  << "," << new_network_stats[row].LEACH_num_cluster_head[col] << ","<< new_network_stats[row].LEACH_percent_head[col];
			NetworkData_DIRECT << new_network_stats[row].DIRECT_rounds[col] << "," << new_network_stats[row].DIRECT_network_average_energy[col] << "," << new_network_stats[row].DIRECT_network_total_energy[col] << ","<< new_network_stats[row].DIRECT_num_dead_node[col];
			NetworkData_LEACH_NEW << new_network_stats[row].LEACH_NEW_rounds[col] << "," << new_network_stats[row].LEACH_NEW_network_average_energy[col]  << "," << new_network_stats[row].LEACH_NEW_network_total_energy[col]  << "," << new_network_stats[row].LEACH_NEW_num_dead_node[col]  << "," << new_network_stats[row].LEACH_NEW_num_cluster_head[col] << ","<< new_network_stats[row].LEACH_NEW_percent_head[col];
		}

		NetworkData_LEACH << endl;
		NetworkData_LEACH_NEW << endl;
		NetworkData_DIRECT << endl;
	}

	NetworkData_Parameters << new_network_stats[0].BASE_STATION_X << endl;
	NetworkData_Parameters << new_network_stats[0].BASE_STATION_Y << endl;
	NetworkData_Parameters << new_network_stats[0].NETWORK_X << endl;
	NetworkData_Parameters << new_network_stats[0].NETWORK_Y << endl;
	NetworkData_Parameters << new_network_stats[0].NUM_NODES << endl;
	NetworkData_Parameters << new_network_stats[0].LEACH_ROUNDS << endl;
	NetworkData_Parameters << new_network_stats[0].LEACH_NEW_ROUNDS << endl;
	NetworkData_Parameters << new_network_stats[0].DIRECT_ROUNDS << endl;

	NetworkData_Parameters << new_network_stats[0].Improvement << endl;
	NetworkData_Parameters << new_network_stats[0].CLUSTER_PERCENT << endl;


	NetworkData_LEACH.close();
	NetworkData_LEACH_NEW.close();
	NetworkData_DIRECT.close();
	NetworkData_Parameters.close();
}

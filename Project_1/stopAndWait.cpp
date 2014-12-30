/*
Name: Wesley Kepke
Class: CPE 400
Due Date: October 5, 2014

This is the stop-and-wait implementation for project 1. 

Assumptions: 
Each packet is of size 100 bytes; there are 100 packets to transmit. 
For each packet, assume RTT to be a random value between 10-50 ms. Assume 
timeout as 45 ms. 
*/

// Header Files
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

// Global Constants / Typedefs / Global Variables
const int NUM_PACKETS = 100; 
const int PACKET_SIZE = 100; 
const int TIMEOUT = 45;
float packetErrorRate = 0; 
int totalRetransmissions = 0; 

// Function Prototypes
float stopAndWaitSimulation( void ); 
bool send( int packetNum, 
           int &roundTripTime ); 

// Main Program
int main( int argc, char **argv )
   {
   // Initialize program
   srand( time( NULL ) ); 
   float throughput = 0.0; 
   float averageThroughput = 0.0; 
   float totalThroughput = 0.0; 
   int tempErrorRate = 0; 
   float averageRetransmissions = 0.0; 

   // Perform the stop-and-wait simulation
   tempErrorRate = atoi( argv[1] ); 
   packetErrorRate = (float)tempErrorRate / 10.0;
   cout << "Stop and Wait - Window Size: 1" << endl;
   cout << "Error Rate: " << packetErrorRate << endl;  
   for( int i = 0; i < 10; i++ )
      {
      throughput = stopAndWaitSimulation();
      totalThroughput += throughput;  
      //cout << "Throughput for Simulation (Iteration: #" << (i + 1) << ") => ";
      //cout << throughput << endl; 
      }
   averageThroughput = (float)totalThroughput / 10.0;
   averageRetransmissions = (float)totalRetransmissions / 10.0; 
   cout << "AVERAGE THROUGHPUT FOR 10 ITERATIONS: " << averageThroughput << endl;
   cout << "AVERAGE RETRANSMISSIONS FOR 10 ITERATIONS: " << averageRetransmissions; 
   cout << endl;

   // Main program conclusion
   return 0; 
   }

// Function Implementations
float stopAndWaitSimulation( void )
   {
   // Initialize function/variables
   int i = 0; 
   int duration = 0; 
   int RTT = 0; 
   float throughput = 0.0; 
   bool successfulTransmit; 
   int retransmissionCount = 0; 

   // Transmit the 100 packets
   for( i; i < NUM_PACKETS; i++ )
      {
      // Send the packet until it has been successfully acknowledged by the
      // receiver
      successfulTransmit = false; 
      while( successfulTransmit == false )
         {
         retransmissionCount++; 
         successfulTransmit = send( i, RTT ); 
         duration += RTT; 
         }
      }

   // Calculate the throughput for the program
   //cout << "Total Duration for Simulation: " << duration << endl;
   //cout << "Number of Retransmissions: " << retransmissionCount << endl;  
   totalRetransmissions += retransmissionCount; 
   throughput = ( (NUM_PACKETS * PACKET_SIZE) / (float)duration ); 
   return throughput;       
   } 
 
bool send( int packetNum,
           int &roundTripTime )
   {
   // Initialize function/variables
   //float packetErrorRate = 0.0; // Random value between 0-0.5 (intervals of 0.1)
   float packetProbability = 0.0; // Random value between 0-1

   // Determine random numbers  
   roundTripTime = (rand() % 41) + 10; // Random value between 10-50
   //packetErrorRate = (rand() % 6) / 10.0; 
   packetProbability = (rand() % 11) / 10.0;

   // Determine if the packet is in the error zone (it was lost somewhere)
   if( packetProbability <= packetErrorRate )
      {
      roundTripTime = TIMEOUT; 
      return false;  
      }

   // Otherwise, the packet is being returned successfully (even if it is
   // taking longer than the timeout)  
   else
      {
      return true;
      }
   } 

/*
Name: Wesley Kepke
Class: CPE 400
Due Date: October 5, 2014

This is the go-back-n (GBN) implementation for project 1. 

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
int windowSize = 0;
float packetErrorRate = 0; 
int totalRetransmissions = 0; 
struct packet
   {
   int sequenceNumber; 
   bool successfulTransfer; 
   int RTT; 
   };

// Function Prototypes
void initializePackets( packet packetArray[],
                        int packetArraySize ); 
float gbnSender( packet packetArray[] ); 
void gbnReceiver( packet pipelinePackets[] ); 

// Main Program
int main( int argc, char **argv )
   {
   // Initialize program
   packet mainPacketArray[NUM_PACKETS]; 
   srand( time(NULL) ); 
   initializePackets( mainPacketArray, NUM_PACKETS ); 
   float throughput = 0.0; 
   float averageThroughput = 0.0;
   float totalThroughput = 0.0;
   float averageRetransmissions = 0.0;  
   int tempErrorRate = 0; 

   // Determine certain program attributes from the command prompt
   windowSize = atoi( argv[1] );  
   tempErrorRate = atoi( argv[2] );

   // Perform the go-back-n simulation
   packetErrorRate = (float)tempErrorRate / 10.0; 
   cout << "Go-Back-N - Window Size: " << windowSize << endl;
   cout << "Error Rate: " << packetErrorRate << endl;  
   for( int i = 0; i < 10; i++ )
      {
      throughput = gbnSender( mainPacketArray );
      totalThroughput += throughput; 
      //cout << "Throughput for Simulation (Iteration: #" << (i + 1) << ")  => "; 
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
void initializePackets( packet packetArray[],
                        int packetArraySize )
   {
   // Initialize the packet array for the sending process
   for( int i = 0; i < packetArraySize; i++ )
      {
      packetArray[i].sequenceNumber = i;
      packetArray[i].successfulTransfer = false; 
      packetArray[i].RTT = 0; 
      }
   }

float gbnSender( packet packetArray[] )
   {
   // Initialize function/variables
   int base = 0;
   bool stoppingCondition = false; 
   int roundTripTime = 0;
   int duration = 0;  
   int windowIndex = 0;
   int retransmissionCount = 0;  
   packet *windowPacketArray = new packet[windowSize]; 
   initializePackets( windowPacketArray, windowSize ); 

   // Perform the go-back-n simulation
   while( stoppingCondition != true )
      {
      // Send packets to the receiver 
      gbnReceiver( windowPacketArray );

      // Check to see if any of the packets in the window did not transmit
      // successfully 
      for( windowIndex = 0; windowIndex < windowSize; windowIndex++ )
         {
         if( windowPacketArray[windowIndex].successfulTransfer == false )
            {
            // Keep track of the retransmissions and exit the loop early
            retransmissionCount++; 
            break;
            }
         }

      // If there were unsuccessful packets, we need to go-back-n and resend
      // the packets
      if( windowIndex < windowSize )
         {
         // Adjust base
         base += windowIndex;

         // Since a packet was not transmitted successfully, the sender
         // waited for a max time of 45 ms
         duration += TIMEOUT;  
         }

      // Otherwise, the current window's packets all returned successfully
      else if( windowIndex == windowSize )
         {
         // Adjust base and see if we have reached a stopping condition
         base += windowSize;
         if( base >= NUM_PACKETS )
            {
            stoppingCondition = true; 
            } 

         // Find the longest RTT time of the successful packets
         roundTripTime = windowPacketArray[0].RTT; 
         for( windowIndex = 0; windowIndex < windowSize; windowIndex++ )
            {
            if( roundTripTime > windowPacketArray[windowIndex].RTT )
               {
               roundTripTime = windowPacketArray[windowIndex].RTT;
               }
            }
         duration += roundTripTime; 
         }

      // Prepare next window to be sent
      initializePackets( windowPacketArray, windowSize ); 
      } 

   // Calculate the throughput for the program
   delete []windowPacketArray;
   windowPacketArray = NULL;
   //cout << "Total Duration for Simulation: " << duration << endl;   
   //cout << "Number of Retransmissions: " << retransmissionCount << endl;
   totalRetransmissions += retransmissionCount;  
   return ( (NUM_PACKETS * PACKET_SIZE) / (float)duration );
   }

void gbnReceiver( packet pipelinePackets[] )
   {
   // Initialize function/variables
   //float packetErrorRate = 0.0; // Random value between 0-0.5 (intervals of 0.1)
   float packetProbability = 0.0; // Random value between 0-1
   int roundTripTime = 0; // Random value between 10-50

   // Process "windowSize" amount of packets at a time 
   for( int i = 0; i < windowSize; i++ )
      {
      // Determine random numbers
      roundTripTime = (rand() % 41) + 10;
      packetErrorRate = (rand() % 6) / 10.0;
      packetProbability = (rand() % 11) / 10.0; 

      // Determine which packets made if to the receiver and back to the sender
      if( packetProbability < packetErrorRate )
         {
         pipelinePackets[i].RTT = TIMEOUT; 
         pipelinePackets[i].successfulTransfer = false;     
         }
 
      else
         {
         pipelinePackets[i].RTT = roundTripTime;
         pipelinePackets[i].successfulTransfer = true; 
         } 
      } 
   } 

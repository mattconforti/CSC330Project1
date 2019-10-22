
/*
 *
 * Matt Conforti
 * Project 1
 * 9/21/17
 *
 */

#include <mpi.h>
#include <stdio.h>
#include <string.h>

using namespace std;

int main(int argc, char *argv[]){

	// the number of times this program is run
	const int numCycles = 10000;

	// the number of processors running
	int numProc;

	// which rank the current processor is
	int rankNum;

	// Initialize the MPI
	MPI::Init(argc,argv);         

	// set the numProc variable
	numProc = MPI::COMM_WORLD.Get_size();

	// set the rankNum variable
	rankNum = MPI::COMM_WORLD.Get_rank();

	// set up an array to count which baby node responds
	int countArr[numProc - 1];
	
	// initialize the array
	for (int i = 0; i < (numProc - 1); i++){ 
		countArr[i] = 0;
	}

	// set the value which the baby nodes send to the head node
	int sendVal = 1;

	for(int n = 0; n < numCycles; n++){	// for as many cycles as defined above
		// set up a barrier to synchronize processes
		MPI::COMM_WORLD.Barrier();	

		// if current rank is the head node
		if (rankNum == 0){
                	// print a statement about what iteration of the loop we are on
                        cout << "\niteration #: " << n + 1 << "\n";

			for (int babyNum = 1; babyNum < numProc; babyNum++){   // for all the baby nodes
				// recieve a message
 				MPI::Status stat;
                		MPI::COMM_WORLD.Recv(&sendVal, 1, MPI::INT, MPI::ANY_SOURCE, 0, stat);
			
				// check to see which baby node sent it, and update the array accordingly
				int sourceNode = stat.Get_source();
				cout << "\nvalue recieved from baby" << sourceNode << "\n";
	
				countArr[babyNum - 1] = countArr[babyNum - 1] + sourceNode;
			}
			// print the array
			cout << "\n";
			for( int index = 0; index < numProc - 1; index++){
				cout << countArr[index] << ", ";
			}
			cout << "\n";				 
		}		
		else{ // the current rank is one of the baby nodes
			// send a message to the head node
			MPI::COMM_WORLD.Send(&sendVal, 1, MPI::INT, 0, 0);
		}
	}

	// Finalize the MPI
	MPI::Finalize();

	return 0;

}

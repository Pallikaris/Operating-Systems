

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
#include <string>
#include <map>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <random>
#include <iterator>
#include <list>

#include "simule.h"

using namespace std;


///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


/*
Input parameters (  χρονος μεταξυ διαδοχικων αφιξεων -a,
					μεση διαρκεια ζωης διεργασιας    -l,
					μεση διαρκεια εκτελεσης κρισιμης περιοχης -c ,
					μεγιστο πληθως διεργασιων   -m,
					πιθανοτητα εισοδου στην κρισιμη περιοχη -p,
					πληθως σημαφορων -s,
				)
*/

int main ( int argc,char ** argv)
{	int avgtm_lifes_duration,avgtm_between_arrivals,avgtm_critical_duration,maximum_processes,probability_of_locking,no_sems;

	string logfile=LOG_FILE_SCHEDULE_MATRIX;
	string errorfile=LOG_FILE_ERROR;
	string debugfile=DEBUG_FILE;

	ofstream log(logfile);
	ofstream err(errorfile);
	ofstream debug(debugfile);



	double  block_counter_array[7]={0,0,0,0,0,0,0};


	if (argc!=13){
		cout <<"Wrong number of parameters "<<argc<<endl;
		err<<"Error_1::Critical|Handled:: Wrong number of parameters "<<endl;
		return 16;
	}
	else 
		{
			if ( argv[1]=="-arrivals") cout<<argv[1]<<endl;
				else if ( argv[3]=="-life") cout<<argv[3]<<endl;
					else if ( argv[5]=="-critical") cout<<argv[3]<<endl;
						else if ( argv[7]=="-maximum") cout<<argv[3]<<endl;
							else if ( argv[9]=="-prob") cout<<argv[3]<<endl;
								else if ( argv[11]=="-semaph") cout<<argv[3]<<endl;
									else 
										{
											//cout <<" Proper input !"<<endl;
											 avgtm_between_arrivals = stoi(argv[2]);
											 avgtm_lifes_duration=stoi(argv[4]);
											 avgtm_critical_duration=stoi(argv[6]);
											 maximum_processes=stoi(argv[8]);
											 probability_of_locking=stoi(argv[10]);
											 no_sems=stoi(argv[12]);


										}

		}

		srand(time(NULL));
		int actual_num_of_processes=rand() % maximum_processes;
		if (actual_num_of_processes<=1)
				{	
					
					err<<"Error_2::Minor|Handled:: Can't have less than 2 processes"<<endl;

					actual_num_of_processes=2;
				}
		int actual_num_of_semaphores=rand()% no_sems;
		if (actual_num_of_semaphores<=1)
				{	
					
					err<<"Error_3::Minor|Handled :: Can't have less than 1 semaphore"<<endl;
					actual_num_of_semaphores=1;
				}
		///////////////
		int max_arrive=actual_num_of_processes*avgtm_lifes_duration;
		int simulation_max_time= actual_num_of_processes * avgtm_lifes_duration * 2;
		//cout <<"Demonstration will have "<<++actual_num_of_processes<<"  processes and "<<++actual_num_of_semaphores<<"  semaphores ."<<endl<<endl;
		log<<"Demonstration will have "<<++actual_num_of_processes<<"  processes , "<<++actual_num_of_semaphores<<"  semaphores  and will run at least for "<<simulation_max_time<<" timeslots ."<<endl<<endl;
		//////////////////////////



srand(time(NULL));

// Initializing seed for the distribution 
int seed = chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator (seed);

exponential_distribution<double> duration_distribution (avgtm_lifes_duration);



int durations_array[actual_num_of_processes];
int id_array[actual_num_of_processes];
int arriving_time[actual_num_of_processes];


for ( int i=0; i <actual_num_of_processes ; i ++)
{
	double number = duration_distribution(generator);
    
    number=number * (avgtm_lifes_duration*avgtm_lifes_duration);
    
    
    
    //Normalize distribution with actual mean defined by parameters.
    durations_array[i]=int(number);
    if ( durations_array[i]<1)
    {
    	durations_array[i]=1;
    }

    arriving_time[i]=rand()%max_arrive;
    //cout<<"arriving_time "<<i<<" = "<< arriving_time[i]<<endl;
    //cout <<durations_array[i]<<endl;    
    id_array[i]=i;




}
debug <<" breaker 1"<<endl;
///Exponential distributed critical section durations Bank

//int kalos=actual_num_of_processes *(avgtm_lifes_duration+1)]
//cout <<"*****************************"<<endl;
int kalos =100;
int critical_section_randoms[kalos];
random_device rd;
mt19937 gen(rd());

exponential_distribution<double> critical_distribution (avgtm_critical_duration);


for (int  j=0; j<=kalos ; ++j){

	double number = critical_distribution(gen);
    
    number=number * (avgtm_critical_duration*avgtm_critical_duration);
    critical_section_randoms[j]=int(number);

    if ( critical_section_randoms[j]<1)
    {
    	critical_section_randoms[j]=1;
    }

    


   
}
	debug <<" breaker 2"<<endl;

	// Creating and initializing the processes.
	// This one will be our basic iteration entity to simulate schedulers load queue

	
	Process * process_array= (Process*) malloc (sizeof(Process) * actual_num_of_processes);
	for ( int w=0;w <actual_num_of_processes;w++)
	{
		process_array[w]=Process(arriving_time[w],id_array[w] , id_array[w] , durations_array[w], probability_of_locking , critical_section_randoms,kalos,false);

	}

	debug <<" breaker 3"<<endl;
	Semaphore* semaphore_array= ( Semaphore*) malloc (sizeof(Semaphore) *actual_num_of_semaphores );
	for ( int w=0; w < actual_num_of_semaphores ; w++)
	{

		semaphore_array[w]=Semaphore(w);
	}
	/*///////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////*/
	// Scheduling per timeslot
	/*///////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////*/
	debug <<" breaker 4"<<endl;
	for ( int current_timeslot = 0;  current_timeslot <= simulation_max_time ; current_timeslot++)
	{	
		
		int i =0;
		int best_prior=8;
		int best_id=-1;
		int best_index=-1;

		for (  ; i < actual_num_of_processes; i++)
			{	
				/// Update arrived processes
				if (process_array[i].get_arriving_time()  ==current_timeslot)
					{
						process_array[i].set_ready();
						
						//cout <<"Process "<<process_array[i].getpid()<< " arrived at timeslot"<<current_timeslot<<" with work load "<<process_array[i].get_total_duration();
						//cout<< " and priority "<<process_array[i].get_priority()<<endl;

						log <<"Process "<<process_array[i].getpid()<< " arrived at timeslot  "<<current_timeslot<<" with work load "<<process_array[i].get_total_duration();
						log<< " and priority "<<process_array[i].get_priority()<<endl;


					}

				//from those who are ready
				//find the one with the Highest (min) priority

				if ( process_array[i].get_state()==READY)
					{
						int temp=process_array[i].get_priority();
						if (temp <best_prior)
							{	
								// update the highest priority
								best_id=process_array[i].getpid();
								best_index=i;
								best_prior=temp;
							}

					}


			}

		/// update the metrics
		for ( i=0 ; i < actual_num_of_processes ; i++)
			{
				if ( process_array[i].get_state()==READY)
					{
						if (best_id !=process_array[i].getpid())
								{
									process_array[i].increase_waiting();
								}
					}	

			}
		
		//////REPORT FOR TIMESLOT////
		if ( best_id==-1)
			{
				cout<<"Timeslot  "<< current_timeslot<<":  no processes to schedule ."<<endl;

			}
		else
			{
				cout <<"Timeslot  "<<current_timeslot<<  ":  Process " <<best_id<<" with priority "<<best_prior<<" gets the CPU ::";
			}
		////////////
		//// RUN ///
		////////////
		///  if READY LIST is not empty
			//debug<<" 0000"<<endl;
			if (best_id !=-1)
					{	
						//debug<<" 0"<<endl;
						int pg=-1;
						pg =process_array[best_index].get_programm_counter ();
						// if programm is choosing down()
						if (  pg !=-1 )
							{	//debug<<" break 40"<<endl;
								
								if  (  process_array [best_index].steps_down(pg)  )
									{//  process tries down()
										//debug<<" break 51"<<endl;
										int chance_sem=rand() % actual_num_of_semaphores;

										//tries down
										 if (semaphore_array[chance_sem].down( process_array[best_index]) )
											 {//succeed down
											 	process_array[best_index].set_semaphore(chance_sem);
											 	cout <<"Succesful  DOWN() at sem "<< chance_sem;

											 	int rest_comm=process_array[best_index].run();
												if (rest_comm != -1)
													 	{
													 		cout << "  Process "<< best_id<< " runnned "<< pg << endl;
													 		
													 		if (rest_comm>0)
													 			{
													 				simulation_max_time++;
													 			}
													 		else
													 			{	
													 				cout << " bbbb1111pppppp"<<endl;
													 				int temporary=process_array[best_index].get_semaphore();
													 				if ( temporary >-1)
													 				{	
													 					int popped_id=semaphore_array[temporary].up(process_array[best_index]);
													 					process_array[popped_id].set_ready();
													 					cout<<" System forced up sem  " << temporary<<" ";
													 					

													 				} 


															 		err<<"Error_4::Critical|Unhandled: no more service time for process although succed down"<<best_id<<endl;
															 		int return_tat=-1;
															 		return_tat =process_array[best_index].set_tat(current_timeslot+1);
															 		process_array[best_index].set_exit();
															 		process_array[best_index].restore_priority();
															 		log<<"Process " << process_array[best_index].getpid() << "  exited at timeslot " << current_timeslot;
															 		log << ", with T.A.T. "<< return_tat<<",  waiting time "<<process_array[best_index].get_waiting()<<endl;
													 				

													 				

													 		 												 	
													 			}
													 	}




											 }
										 else
											 {//failed down
											 	cout <<endl<< "Unsuccesful DOWN() at sem "<<chance_sem <<" : locked " <<endl;
											 	process_array[best_index].set_blocked(chance_sem);
											 	


											 }



									}


								else if ( process_array[best_index].steps_up(pg)  )
									{	// process is doing up
										//debug<<" break 61"<<endl;
										
										int semretrieve=process_array[best_index].get_semaphore();
										//finds where is locked
										if (semretrieve >=0 )
												{	
													int popped_id;
													popped_id= semaphore_array[semretrieve].up(process_array[best_index]);
													process_array[best_index].restore_priority();
													if ( popped_id >=0)
															{
																cout << "UP() at sem "<< semretrieve << " released pid "<< popped_id <<endl;
																for ( int pipi=0 ; pipi < actual_num_of_processes ; pipi++)
																		{
																			if (process_array[pipi].getpid()==popped_id)
																			{

																				process_array[pipi].set_ready();
																			}

																		}


															}
													else 
															{

																cout << "UP() at sem "<< semretrieve << " no blocked process to release in this semaphore.";
															}

												}
											else
											{	
												//cout<<" Error_5::minor " <<endl;
												err <<" Error_5::Minor|Unhandled" <<endl;
											}
										int rest_comm=process_array[best_index].run();
										if (rest_comm != -1)
											 	{
											 		cout << "Process "<< best_id<< " runnned "<< pg ;
											 		if (rest_comm>0)
											 			{
											 				simulation_max_time++;
											 			}
											 											 	
													else
													 	{	
													 			
													 			int temporary=process_array[best_index].get_semaphore();
													 			if ( temporary >-1)
													 				{
													 					int popped_id=semaphore_array[temporary].up(process_array[best_index]);
													 					
													 					for ( int pipi=0 ; pipi < actual_num_of_processes ; pipi++)
																		{
																			if (process_array[pipi].getpid()==popped_id)
																			{
																				process_array[pipi].set_ready();
																				cout<<" System forced up sem  " << temporary<<"  retrieved process "<<popped_id;
																			}

																		}
													 					
													 					
													 				}

														 		int return_tat=-1;
														 		cout<<" No more service time for process "<<best_id<<endl;
														 		return_tat= process_array[best_index].set_tat(current_timeslot+1);
														 		process_array[best_index].set_exit();
														 		process_array[best_index].restore_priority();
														 		log<<"Process " << process_array[best_index].getpid() << "  exited at timeslot " << current_timeslot ;
														 		log << ", with T.A.T. "<< return_tat<<",  waiting time "<<process_array[best_index].get_waiting()<<endl;

													 			 
													 		
														}
												}
									}

									else
									{// programm counter block is not ending neither with  down nor with up
										//debug<<" break 71"<<endl;
										int rest_comm=process_array[best_index].run();
										
										if (rest_comm!=-1 )
											 	{
											 		cout << "Process "<< best_id<< " runned "<< pg ;
											 		if (rest_comm>0)
											 			{
											 				simulation_max_time++;
											 			}
												 	else
													 	{	
													 		
													 		int temporary=process_array[best_index].get_semaphore();
													 		if ( temporary >-1)
													 				{
													 					int popped_id=semaphore_array[temporary].up(process_array[best_index]);
													 					for ( int pipi=0 ; pipi < actual_num_of_processes ; pipi++)
																		{
																			if (process_array[pipi].getpid()==popped_id)
																			{
																				process_array[pipi].set_ready();
																				cout<<" System forced up sem  " << temporary<<"  retrieved process "<<popped_id;
																			}

																		}
													 					
													 												 				

													 				} 

													 		int return_tat=-1;
													 		cout<<"No more service time for process "<<best_id<<endl;
													 		return_tat=process_array[best_index].set_tat(current_timeslot+1);
													 		process_array[best_index].set_exit();
													 		process_array[best_index].restore_priority();
													 		log<<"Process " << process_array[best_index].getpid() << "  exited at timeslot " << current_timeslot;
													 		log << ", with T.A.T. "<< return_tat<<",  waiting time "<<process_array[best_index].get_waiting()<<endl;

													 		
													 	}
												}
									} 
								


							}
							



					}

		//debug<<" break 10"<<endl;
		int y=0;
		for ( ; y <actual_num_of_semaphores ; y++)
			{	
				
				semaphore_array[y].inheritance();
			}
		cout <<endl;

	}

	log << "*********************************************"<<endl;
	for ( int prio=1 ; prio <= 7 ; prio++)
			{ 	
				double counter =0.0;
				block_counter_array[prio-1]=0.0;
				for ( int ww=0 ; ww < actual_num_of_processes ; ww++)
						{
							if (process_array[ww].get_priority()==prio)
								{
									counter +=1.0;
									block_counter_array[prio-1]+= process_array[ww].get_waiting();

								}
						}
			double res=0.0;
			if ( counter!=0.0)
			{res= block_counter_array[prio-1] / counter;}
			else
				{res=0.0;}

			log<<" Average waiting time for process with base priority = "<<prio<<" is " << res<< " timeslots ." <<endl;
			}
	debug<<" No bugs!Great!"<<endl;
}


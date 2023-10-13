#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <random>
#include <string>
#include <map>
#include <ctime>
#include <cstring>

#include <cstdlib>
#include <chrono>
#include <thread>
#include <random>
#include <iterator>
#include <list>

#include <queue> 

using namespace std;

enum states { BLOCKED , READY , RUNNING ,EXIT,UNARRIVED};

enum value { DOWN, UP};

#define LOG_FILE_SCHEDULE_MATRIX "schedule_matrix.txt"
#define LOG_FILE_ERROR "error_file.txt"
#define DEBUG_FILE "debug.txt"
#define PERM 0644




class Programm {

	public:
		int total_duration;
		int programm_id;

		vector<int> downs;
		vector<int> ups;

		int downcount;
		int next_down;
		int next_up;
		
		
		Programm(int prog_id,int duration,int probability, int critical_section_random_durations[],int sz,bool printall=true)
				{
					programm_id=prog_id;
					total_duration=duration;
					downcount=0;
					next_up=0;
					next_down=0;	
					int restdur,chance,moder,uppoint;

					//while there are at least 2 more timeslots 
					// - the current and the next one- there is
					//   a chance of hiding a critical section
					int i=1;
					restdur =total_duration-1;
					while(restdur>1)
					{	//cout <<"while"<<endl;
						

						chance =rand () %100 ;


								// we suppose that every programm is well defined and doesn't provoke
								// deadlocks
								// every resource that is holded must be returned to the system .
										if (chance < probability)
										{	// down() succeed
											downcount++;
											downs.push_back(i);
											//cout<<"down"<<i;
											moder= rand() % sz ;
											
											while ( critical_section_random_durations[moder] > restdur) 
													{	//cout<<"kakos"<<endl;
														moder= rand() % sz ;};

											uppoint= critical_section_random_durations[moder] + i;
											//cout<< "up"<<uppoint<<endl;
											ups.push_back(uppoint);
											i=uppoint;

										}
						i++;
						restdur=total_duration-i;

					}

					if (printall)
					{
								cout<<"Constructed programm with id : "<<programm_id<<"  and burst time "<< total_duration<<endl;
								
								int ss=downs.size();
								int uu=ups.size();
								if ( uu != ss){ 
									cout <<" ups and downs have different size "<<endl;
									
									}
								//cout <<ss<<endl;
								
								bool flage=true;
								for ( int k =0 ; k < ss ; k++)
								{	if (flage)
									{
									cout<<" Contains  "<<ss<< " critical sections  :"<<endl;
									flage=false;
									}
									cout<< " semaphore DOWN() at programm counter block/timeslot "<< downs[k]<<endl;
									cout<< " semaphore UP()   at programm counter block/timeslot "<< ups[k]<<endl<<endl;

								}
					}
			

		}

		int getduration()
		{
			return total_duration;

		}	

		//////////////////////////////////////////
		//////////////////////////////////////////
		//////////////////////////////////////////
		//////////////////////////////////////////
		 virtual bool steps_to_up ( int program_counter)
		 {	
		 	int sz=ups.size();
		 	for  ( int i=0 ; i < sz ; i++)
		 	{
		 		if ( program_counter== ups[i])
		 		{

		 			return true;

		 		}
		 	}

		 	return false;
		 }

		 virtual bool steps_to_down ( int program_counter)
		 {	
		 	

		 	int sz=downs.size();
		 	

		 	for  ( int i=0 ; i < sz ; i++)
		 	{	
		 		if ( program_counter== downs[i])
		 		{

		 			return true;

		 		}
		 	}

		 	return false;
		 }






		 //////////////////////////////////////////
		//////////////////////////////////////////
		 //////////////////////////////////////////
		//////////////////////////////////////////

		int increase_nextdown()
		{
			int t=next_down;
			next_down++;
			if (next_down>downcount){next_down=downcount;}
			return t;

		}

		int increase_nextup()
		{
			int t=next_up;
			next_up++;
			if (next_up>downcount){next_up=downcount;}
			return t;
		}

		///////////////////////////////////
		///////////////////////////////////

		int getid()
		{
			return programm_id;
		}
		~Programm()
		{cout<< "  ";}





};


class Process:public Programm
{
	public:
		int rest_duration;
		int state;
		int program_counter;

		int base_priority;
		int current_priority;
		int process_id;

		int metric_waiting;
		int metric_blocked;

		int arriving_time;

		int tat;

		// we use this member either we lock a semaphore or we are locked up in a semaphore

		int sem_blocked;

		Process(int arrtime,int proc_id , int prog_id,int duration,int probability, int critical_section_randoms[],int sz,bool printall=true)
		:Programm( prog_id, duration, probability,  critical_section_randoms, sz, printall)
		{	program_counter=0;
			arriving_time=arrtime;
			process_id=proc_id;
			rest_duration=total_duration;
			state=UNARRIVED;
			base_priority=(rand()%7) +1 ;
			metric_blocked=0;
			metric_waiting=0;
			sem_blocked=-1;
			current_priority=base_priority;
			tat=-1;

		}

		int get_waiting()
		{
			return metric_waiting;
		}

		int get_blocked()
		{
			return metric_blocked;
		}

		int set_tat(int finish_time)
		{
			tat=finish_time -arriving_time;
			return tat;
		}


		bool steps_up ( int counter)
		{
			return Programm::steps_to_up(counter);

		}


		bool steps_down(int counter)
		{	
			return Programm::steps_to_down(counter);
		}

		int get_total_duration()
		{
			return Programm::getduration();
		}

		void set_blocked(int semid)
		{	
			sem_blocked=semid;
			state=BLOCKED;
		}

		// returns the semaphore if it was blocking one 
		int set_ready()
		{
			state=READY;
			current_priority=base_priority;
			int temp = sem_blocked;

			sem_blocked=-1;
			return temp;

		}

		void set_running()
		{
			state=RUNNING;
		}

		void set_exit()
		{
			state=EXIT;
		}

		void set_state(int st)
		{	if (st <=4 && st >=0)
				state=st;
			else 
				cout <<" errror 222"<<endl;
		}
		///////////////
		
		void set_priorrity( int new_priority)
		{
			if( new_priority>=1 && new_priority <=7)
				current_priority=new_priority;
			else
				cout<<" wrong ipnut priority 777"<<endl;

		}
		int get_state()
		{
			return state;
		}
		int get_priority()
		{

			return current_priority;

		}

		int get_programm_counter()
		{
			if ( (total_duration-rest_duration)!= program_counter)
				{
					cout<<"errror in programm counter "<<endl;
					return -1;	
				}
			else
				return program_counter;	

		}
		int get_arriving_time()
		{
			return arriving_time;
		}

		void restore_priority( )
		{
			current_priority=base_priority;
		}

		void increase_waiting()
		{
			metric_waiting++;

		}

		void increase_blocked()
		{
			metric_blocked++;
		}

		int run()
		{
			if (rest_duration >0 && program_counter <= total_duration)
				{	
					
					rest_duration= rest_duration -1;
					program_counter++;
					return rest_duration;

				}
			else
			{
				
				return -1;
			}
		}

		int getpid()
		{
			return process_id;
		}

		void set_semaphore(int semaphore_id)
		{

			sem_blocked=semaphore_id;
		}

		int get_semaphore()
		{
			if (sem_blocked >=0)
				return sem_blocked;
			else
			{
				//cout<<"wrong semaphore id error 23411"<<endl;
				return -1;
			}
		}

		~Process()
		{}





};

class Semaphore {
public:
    	
    Process* locking_process;

    int sem_id;
    int lock;
    
    // queue made from vector is not optimal but our data size is  small
    // so i will use this index accessible structure
    vector <Process> sem_q;
 	


    Semaphore (int semid)
	
    {	
    	sem_id=semid;
	  	lock= UP;
    	locking_process=nullptr;
    	
    }



    // Collecting metric data for processes that are blocked from lower priority processes.
    // increase_blocked() increases a the member metric_blocked for every process that is waiting in the queue with actual priority
    // higher than the process which locks the semaphore ( executing critical section)
    void increase_blocked()
    {	
    	if (locking_process==nullptr)
    		{return;}

    	int holder = locking_process->get_priority();
    	
    	for ( int i =0 ; i < sem_q.size() ; i++)
    	{	cout<<"////////////********************************************** ";
    		cout << " holder = "<<holder;
    		cout<<"******************************************************************* ";
    		cout<<sem_q.at(i).get_priority()<<" ////////////////////////////";
    		if ( sem_q.at(i).get_priority() < holder)
    		{	cout << sem_q.at(i).getpid()<<" and sem q size ="<< sem_q.size()<<endl;
    			sem_q.at(i).increase_blocked();
    		}
    	}

    }

    int get_semid()
    {
    	return sem_id;
    }

    // returns 1 for successful down
    // returns 0 for locking 
    int down (  Process p)
    {
    	if ( lock==UP)
    	{
    		lock=DOWN;
    		locking_process=&p;
    		
    		return 1;
    	}


    	else {
    		sem_q.push_back(p);
    		return 0;

    	}


    }

    void pop_front()
		{
		    if (sem_q.size() > 0) 
			    {
			        sem_q.erase(sem_q.begin());
			    }
		}

	


    // RETURNS -1 if q is empty
    // returns FiFo  first element , if q is not empty  so the systems restores it too
    int up ( Process p)
    {

    	if (sem_q.empty())
    	{
    		lock=UP;
    		locking_process=nullptr;
    		return -1;

    	}

    	else
    	{	
    		lock=UP;
    	 	locking_process=nullptr;
    		Process old = sem_q.front();
    		pop_front();
    		
    		return old.getpid();

    	}
    }

    
    ~Semaphore()
    {

    }

    

};
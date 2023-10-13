### README.txt  ### <br /> 
### SUBJECT Operating Systems ### <br />
### ATHENS KAPODISTRIAN UNIVERSITY ### <br />
### Giorgos Pallikaris 1115201400335  ### <br /> 15/9/2o22 - 21/9/2o22 <br />



Grading
100/100


//////**** IMPORTANT *****//////

Just run  bash fa.sh at each working dir.
Scripts will do everything for you.

You can change the running parameters in there. 
I have already made some examples.
Feel free to test on your own.


_____________________________________________
Summary of the Announcement
_____________________________________________

The simulator will generate random arrivals of processes that have exponentially distributed load.
Each process has a priority [1,7].
Time is organized in time slots.
A process can attempt, with a probability of k, down() on a random system semaphore [1,S].

The time between down() and up() is exponentially distributed.

Show the average waiting time per priority.

Show the blocking time of each process by processes with lower priority.

Implement the priority inheritance scheme in a separate model.


______________
Executable File's Names
___________________

"simulator" in both subdirectories 

/inheritance 
&
/inversion 



_________________________________
DESCRIPTION
_________________________________

This set of  programms simulates 2 scheduling algorithms in a system which uses semaphores as usual.
The minimal ideas  here are that there is a single CPU and each process is using the same semaphore ( far from true).

I chose to implement structure and terminology corresponding to Operating Systems Theory.

The 2 similar projects are directory that implements inversion and directory that implements inheritance.

//##
At directory inversion the main scheduling algorithm ( preemptive) , divides time to timeslots. 
Timeslot is for the system the absolute indivisible unit ,prior only to assembly jumps and below.
If it contains syscall that will always be at the end of the command block.


At each timeslot a process is selected to execute a command block.
This command block may or may not end with syscalls ,sem down ,sem up etc...
Algorithm chooses the process with the highest priority to run. If it locks a semaphore the time until it unlocks it have to follow the exponential distribution given at the input parameters.

Underlying semaphore structure simulates mutexes as usual.

Input parameters are means of several exponential distributions. 
flags are :

average time between 2 arrivals -a,
avg lifetime -l,
average critical section duration -c ,
maximum number of processes   -m,
chance to enter critical section -p,
number of semaphores -s


//##
At directory inheritance , the main algorithm updates every Semaphore Controlling  process such that it steals the highest priority from those that it blocks.



__________________________________________
_____________Comparison __________________

For high number of processes and stabilized rest of the parameters the inheriting algorithm demonstrates far better results tha the simple one.
For big critical section duration and stabilized rest of the parameters the innheriting algorithm demonstrates far better results tha the simple one.
At the rest instances, inheriting algorithms also are at least 30% better.




_________________________________________

Usefoul Info
_________________________________________

*10th argument after the flag   -prob is the probability /1000 for any given timeslot of a process to attempt locking a semaphore.

*We suppose that programms behind these processes neither  attempt down() at their last timeslot nor up() at their first.

*Since each timeslot is the minimum indivisible unit from the point of view of the simulated system ,we will suppose that each semaphore action is taking place at the end of the corresponding timeslot.

* Εarly exiting/ kill is handled by the system by taking control of the semaphores that the process was using.
Although my programm will never demonstrate such an issue.





This project is not off great external reusage.
Its' purpose is to test students with a riddle like homework announcement that combines OS Theory subjects and problem solving techniques.



As a consequence,

I will NOT  optimize C++ usage at the aspects of Memory Management ,virtual methods , private members.
I will optimize C++ at the aspect of programmers developement time and executional resources.
I will try to make as clear as possible what is going on.


Debug File +Log file + Error file + current output at command window are used for better supervision of the system .




_________________________________
Αnnouncement
_________________________________
Να αναπτυχθεί προσομοιωτής συστήματος χρονοδρομολόγησης βάσει προτεραιοτήτων 
με εξαρτήσεις μεταξύ των διεργασιών. 
Συγκεκριμένα ο προσομοιωτής θα παράγει τυχαίες αφίξεις διεργασιών οι οποίες 
παραμένουν ενεργές για τυχαίο χρόνο (εκθετικά κατανεμημένο). Οι διεργασίες 
χαρακτηρίζονται από το σύστημα με τυχαία (ακέραια) προτεραιότητα (p) η οποία
είναι ομοιόμορφα κατανεμημένη στο διάστημα [1, 7] (υψηλότερη προτεραιότητα: 1).
 Ο χρόνος οργανώνεται σε χρονοθυρίδες (προσομοιωτής διακριτού χρόνου). Σε κάθε
χρονοθυρίδα, μία διεργασία μπορεί να επιχειρήσει, με συγκεκριμένη πιθανότητα
(παράμετρος k), λειτουργία down() σε σημαφόρο του συστήματος. Οι σημαφόροι
στους οποίους τελούν οι διεργασίες είναι συγκεκριμένοι (παράμετρος S) και
επιλέγονται τυχαία από τον προσομοιωτή (στο διάστημα [1, S]). Αν επιτύχει η
εκτέλεση της down() το χρονικό διάστημα μέχρι την εκτέλεση της up()
(αποδέσμευση) είναι επίσης εκθετικά κατανεμημένο. Μία διεργασία που έχει
επιτυχώς εκτελέσει την αλληλουχία down()/up() μπορεί, κατά τη διάρκεια της ζωής
της, να επιχειρήσει και πάλι ανάλογη ενέργεια (δηλαδή down() ακολουθούμενο από
up()). Ο μηχανισμός χρονοδρομολόγησης αναφέρεται σε στατικές προτεραιότητες οι
οποίες δεν μεταβάλλονται κατά την εκτέλεση των διεργασιών. Η υλοποίηση των
πράξεων down() και up() ακολουθεί πιστά το μοντέλο υλοποίησης των σημαφόρων
σε υφιστάμενα λειτουργικά συστήματα (δηλαδή υλοποιεί ουρά αναμονής για τη
δέσμευση/κατάληψη του σημαφόρου). Παρακολουθείται, καταγράφεται και
αναφέρεται ο μέσος χρόνος αναμονής ανά εκχωρούμενη τιμή προτεραιότητας (π.χ.,
105 χρονοθυρίδες για τις διεργασίες προτεραιότητας 4). Επίσης παρακολουθείται ο
χρόνος μπλοκαρίσματος διεργασίας από διεργασία χαμηλότερης προτεραιότητας (με
ενδεχόμενη εμπλοκή ενδιάμεσων προτεραιοτήτων) λόγω
χρήσης του ιδίου
σημαφόρου (priority inversion). Εκτός από το σύνηθες μοντέλο χειρισμού
προτεραιοτήτων να υλοποιηθεί και το σχήμα κληρονόμησης προτεραιότητας (Priority
inheritance) στο οποίο η χαμηλής προτεραιότητας διεργασίας που έχει ήδη καταλάβει
το σημαφόρο (εκτελεί κρίσιμη περιοχή) υιοθετεί την τιμή προτεραιότητας της
διεργασίας υψηλότερης προτεραιότητας η οποία έχει ανασταλεί λόγω ταυτόχρονης
χρήσης του ιδίου σημαφόρου.
Βασικές παράμετροι του προσομοιωτή: μέσος χρόνος μεταξύ διαδοχικών αφίξεων
διεργασιών (εκθετικά κατανεμημένος χρόνος), μέση διάρκεια ζωής διεργασίας
(εκθετικά κατανεμημένη διάρκεια), μέση διάρκεια εκτέλεσης κρίσιμης περιοχής
(εκθετικά κατανεμημένη διάρκεια), μέγιστο πλήθος διεργασιών προσομοιωτή,
πιθανότητα εισόδου σε κρίσιμη περιοχή ανά χρονοθυρίδα (k), πλήθος σημαφόρων
(S).


____________________



*Προσομοιωτης χρονοδρομολογησης προτεραιοτητων

*Τυχαιες αφιξεις διεργασιων

*Εκθετικα κατανεμημενος ο χρονος που παραμενουν ενεργες διεργασιες

*Τυχαια προτεραιοτητα [1,7]

*Ο χρονος ειναι διακριτος σε χρονοθυριδες

*Καθε διεργασια που τρεχει εχει μια πιθανοτητα να μπει σε critical section κανωντας down

*Tο χρονικο διαστημα μεχρι το τελος του critical section και το up ειναι εκθετικα κατανεμημενο

*Οι σημαφοροι εχουν υλοποιηση τυπου ουρας αναμονης

*Priority Inheritance from high to low 



Input parameters (  χρονος μεταξυ διαδοχικων αφιξεων,
					μεση διαρκεια ζωης διεργασιας,
					μεση διαρκεια εκτελεσης κρισιμης περιοχης,
					μεγιστο πληθως διεργασιων,
					πιθανοτητα εισοδου στην κρισιμη περιοχη ,
					πληθως σημαφορων,
				 )
	

Περίληψη εκφώνησης:

Ο προσομοιωτής θα παράγει τυχαίες αφίξεις διεργασιών οι οποίες έχουν εκθετικά κατανεμημένο φόρτο.
Κάθε διεργασία εχει προτεραιότητα [1,7]. 
Ο χρόνος οργανώνεται σε χρονοθυρίδες.
Μία διεργασία μπορεί να επιχειρήσει, με πιθανότητα k
down() σε τυχαίο σημαφόρο του συστήματος [1,S]. 

To διάστημα μεταξύ down() και up() είναι εκθετικά κατανεμημένο.

Να δείξετε το μέσο χρόνο αναμονής ανά προτεραιότητας 

Να δείξετε το χρόνο μπλοκαρίσματος κάθε διεργασίας από διεργασίες χαμηλότερης προτεραιότητας .

Να υλοποιηθεί και το σχήμα κληρονόμησης προτεραιότητας σε ξεχωριστό μοντέλο.




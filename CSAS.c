#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*
 *  Linked list important pointers
 */
struct node *head = NULL;                                               //pointer to head of list
struct node *tail = NULL;                                               //pointer to tail of list
struct node *current = NULL;                                            //temp current point


/*
 *  Usage print out
 */
void print_usage() {
    fprintf(stderr,"\nUsage: simulate <filename>\n\n");                 //print usage format
    exit(0);                                                            //exit
}


/*
 * Node structure for linked list
 *
 */
struct node {
    int pid;                                                            //process id
    int burst_length;                                                   //burst length
    int wait_time;                                                      //wait time till run
    struct node *next;                                                  //next item in list
};


/*
 * Print the list out
 */
void print_list(struct node *head) {
    struct node *ptr = head;
    printf("\n");                                                       //print format spacing
    
    while(ptr != NULL)                                                  //start from the beginning
    {
        printf("(%d,%d,%d)",ptr->pid,ptr->burst_length,ptr->wait_time); //print current item in list
        ptr = ptr->next;                                                //move to next item in the list
        if(ptr != NULL){
           printf("->");                                                //print a link between items
        }
    }
    printf("\n\n");                                                     //print format spacing
}


/*
 *  Insert new node to tail of list and link
 */
void insert_last(int pid, int burst_length, int wait_time) {
    struct node *link = (struct node*) malloc(sizeof(struct node));     //create a new link for the item
    struct node *current;                                               //initialize current pointer
    struct node *previous = NULL;                                       //initialize previous pointer
    
    link->pid = pid;                                                    //set new item pid
    link->burst_length = burst_length;                                  //set new item burst length
    link->wait_time = 0;                                                //set new item wait time
    
    struct node *ptr = head;
                                                                        //start from the beginning
    if(ptr == NULL) {
        link->next = head;                                              //point it to old first node
        head = link;                                                    //point first to new first node
    }
    else{
        while(ptr->next != NULL){                                       //find end of list
            ptr = ptr->next;
        }
        ptr->next = link;                                               //point end of list to new (end)item
        link->next = NULL;                                              //point end to null
    }
    
}


/*
 *  Remove item from list by pid
 */
void remove_process(struct node **list, int pid_number) {
    struct node *current;                                               //Initialize temp pointers
    struct node *previous = NULL;
    
    for (current = *list; current != NULL; previous = current, current = current->next) {   //For each item in the list while current is not the end of list move to next
        
        if (current->pid == pid_number) {                               //If current item is the one to remove
            if (previous == NULL) {                                     //If item is at start of list
                *list = current->next;                                  //Point head to the nest item after the head
            }
            else {
                previous->next = current->next;                         //Point the previous pointer past the item to the next item
            }
            
            free(current);                                              //Deallocate the node being removed
            free(*list);
            return;                                                     //Done return
        }
    }
}


/*
 * Get middle of list
 */
struct node *get_middle(struct node *head) {
    struct node *single_speed, *double_speed;
    
    if(head == NULL) {                                                  //if middle doest exist
        return head;                                                    //return head
    }
    
    single_speed = double_speed = head;
    while(double_speed->next != NULL && double_speed->next->next != NULL) { //while next and two ahead is not null
        single_speed = single_speed->next;                              //move foward one item
        double_speed = double_speed->next->next;                        //move double to furtherest seen
    }
    return single_speed;                                                //return pointer to middle item
}


/*
 * Merge Two Lists
 */
struct node* merge(struct node* list_1, struct node* list_2){
    struct node* result = NULL;                                         //pointer to the merged lists
    
    if (list_1 == NULL){                                                //if list list_1 is empty
        return(list_2);                                                 //return list b
    }
    else if (list_2 == NULL){                                           //if list b is empty
        return(list_1);                                                 //return list list_1
    }
                                                                        //sort and merge
    if (list_1->burst_length <= list_2->burst_length){                  //if list 2 is bigger
        result = list_1;                                                //result is list_1
        result->next = merge(list_1->next, list_2);                     //merge next item
    }
    else{                                                               //if list 1 is bigger
        result = list_2;                                                //result is list_2
        result->next = merge(list_1, list_2->next);                     //merge next item
    }
    return(result);                                                     //return sorted list
}


/*
 * Merge sort list
 */
struct node *merge_sort(struct node *head) {
    
    struct node *middle;                                                //pointer to node in middle to split
    struct node *second_half;                                           //pointer to second half of inital list
    
    if(head == NULL || head->next == NULL) {                            //if head or next is null
        return head;                                                    //return pointer to head
    }
    else{
        middle = get_middle(head);                                      //get the middle of the list
        second_half = middle->next;                                     //get the second half
        
        middle->next = NULL;                                            //split the list into two halfs
        
        return merge(merge_sort(head),merge_sort(second_half));         //merge the two sorted lists recursively
    }
}


/*
 *  Calculate the average wait time
 */
void calc_wait(){
    int item_count = 0;                                                 //count of items in the list
    int total = 0;                                                      //accumulitive wait time count
    struct node *current = head;                                        //pointer to head
    while(current != NULL){                                             //loop though list
        item_count++;                                                   //increase task item count
        total = total + current->wait_time;                             //increase wait time
        current = current->next;                                        //move to next item
    }
    printf("Average wait time: %.1f milliseconds",(float)total/(float)item_count); //print formatted wait time information
}


/*
 *  First come first served
 *  Calculate wait times to processes
 */
void fcfs(){
                                                                        //iterate and calculate the accumulitive wait time
                                                                        //get average of wait times
    struct node *current;   //Initialize temp pointers
    struct node *previous = NULL;
    
    for (current = head; current != NULL; previous = current, current = current->next) {   //For each item in the list while current is not the end of list move to next
        if(previous != NULL){
            current->wait_time = previous->burst_length + previous->wait_time;  //calculate the wait time for each task in the list
        }
    }
    calc_wait();                                                        //calculate the average wait time
}


/*
 *  sjf Algorithm
 *  Calculate wait times to processes
 */
void sjf(){
    struct node *current = head;                                        //sort list by burst time
                                                                        //iterate and calculate the accumulitive wait time
    
    while(current->next != NULL){                                       //set all wait times to 0 incase fcfs has already been run
        current->wait_time = 0;
        current = current->next;
    }
    head = merge_sort(head);                                            //merge sort list by burst length
    
    fcfs();                                                             //calculate waittimes and get average of wait times
}


/*
 *  Main method of the program
 */
int main (int argc, char *argv[]) {
	
    int count = 0;                                                      //initialise variables
    char filename[20];
    char data[256];
    FILE *file;
    char *token;
    char *token2;
    
    if(argc != 2){                                                      //if filename argument is missing
        print_usage();                                                  //print usage
    }
    else{                                                               //populate list from csv file
        strcpy(filename, argv[1]);                                      //copy filename to beable to open it
        printf("%s\n",filename);                                        //print filename
        file = fopen(filename, "r");                                    //open file
        
        while (fgets(data, sizeof(data), file) != NULL) {               //get all data from file
            token = strtok(data,",");                                   //Split the line and get pid and burst length values
            token2 = strtok(NULL, ",");
            
            if(count > 0){                                              //Skip the first line
                //printf("%d, %d\n", atoi(token), atoi(token2) );       //print items being added
                insert_last(atoi(token),atoi(token2),0);                //insert item into end of list
            }
            count++;                                                    //increase item count
        }
        
        printf("\nInital List:");                                         //print populated lsit
        print_list(head);
        
                                                                        //run first in first served
        printf("FCFS List:\n");
        fcfs();
        print_list(head);                                               //print list
        
                                                                        //run sjf algorithm
        printf("SJF List:\n");
        sjf();
        print_list(head);                                               //print list

        fclose(file);                                                   //close file
    }
	return 0;                                                           //return o finish program
}



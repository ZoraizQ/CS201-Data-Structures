#ifndef VCS_CPP
#define VCS_CPP
#include "VCS.h"
#include <sstream>
#include <iostream>

/*
STACK: Selected because the top-most node in the main stack always corresponds to the current version as the stack
stores items in reverse order (bottom-to-top) and to switch or commit versions we only need to generally 
pop and push nodes between the two stacks both of which are O(1) operations (inserting and deleting from the linked 
list head), so commit(), undo() and redo() all take O(1) time with the assumption that we 
ignore the file processing times. 

Meanwhile, our queue without a tail pointer would have taken O(n) time in the worst-case to dequeue a VCS_node
in the case of an undo() for example.
*/

//base constructor, init
VCS::VCS(string fname) //corrected parameter
{
	curr_size=0; //size of main stack
	filename = fname; //member variable filename for the file the VCS is currently operating on
}

//destructor
VCS::~VCS()
{
//destructors for the containers called automatically
}


//time to commit
void VCS::commit(string msg)
{
    int curr_time;
    if (container1.isEmpty()){
        curr_time = 0;
    }
    else{
        curr_time = container1.top().time_stamp + 1;
    }
    container1.push(VCS_node(msg, curr_time));
    curr_size++;

    //read from original file
    ifstream originalfile;
    string original_filename = filename+".txt";
    originalfile.open(original_filename.c_str());
    if (originalfile.fail()){
        return;
    }
    string line = "", original_data = "";
    while(originalfile >> line){
    original_data += line;
    }
    originalfile.close();

    //write to file
    ofstream outfile;
    stringstream converter;
    converter << curr_time;
    string finalname = filename+converter.str()+".txt";
    outfile.open(finalname.c_str());
    if (outfile.fail()){
        return;
    }
    outfile << original_data;
    outfile.close();

    //whenever a new version is committed, the previous version's revert table is cleared, clear container2
    while (!container2.isEmpty()){
        container2.pop();
    }
}

//print status of every version
void VCS::status()
{
    /*
    Step1 Step2 Step3
    4       0    4
    3       1    3
    2       2    2
    1       3    1
    0 5     4    0 5
          - 5
    curr_size = 5
    */
    for (int i = 0; i < curr_size; i++){
        container2.push(container1.pop());
    }
    for (int i = 0; i < curr_size; i++){
        VCS_node popped2 = container2.pop();
        container1.push(popped2);
        cout << "\nVersion: " << popped2.time_stamp << ", Message: " << popped2.message << "\n";
    }

   // Since we are supposed to print all VCS_nodes' data stored within our VCS it also applies to Stack 2.  
   int size2 = container2.length();
   for (int i = 0; i < size2; i++){
       container1.push(container2.pop());
   }
   for (int i = 0; i < size2; i++){
       VCS_node popped1 = container1.pop();
       container2.push(popped1);
       cout << "\nVersion: " << popped1.time_stamp << ", Message: " << popped1.message << "\n";
   }

}

//goes one step back
string VCS::undo()
{
    /*
    Ensure the version stack has atleast 1 node.
    Pop node from container1 and push it into container2
    decrement curr_size
    Read data from curr_top node's corresponding file
    Write data into original file
    */
    if (container1.length() >= 2){ //if there is atleast 1 more node in the main stack to undo to, because the
    //top-mode will be popped (it will be the version undo-ed from)
    container2.push(container1.pop());
    curr_size -= 1;

    //read from version file corresponding to top-most node
    VCS_node latest_top = container1.top();
    string latest_filename = "";
    ifstream latestfile;
    stringstream converter;
    converter << latest_top.time_stamp;
    latest_filename = filename+converter.str()+".txt";
    converter.str("");
    latestfile.open(latest_filename.c_str());
    if (latestfile.fail()){
        return "";
    }
    string line = "", latest_data = "";
    while(latestfile >> line){
    latest_data += line;
    }
    latestfile.close();

    //write data obtained from version file to original file to modify it
    ofstream originalfile;
    string original_filename = filename+".txt";
    originalfile.open(original_filename.c_str());
    if (originalfile.fail()){
        return "";
    }
    originalfile << latest_data;
    originalfile.close();

    return latest_data;
    }
}

//one step ahead
string VCS::redo()
{
    /*
    Ensure the undo-redo stack (container2) has atleast 1 node.
    Pop node from container2 and push it into container1
    increment curr_size
    rest is same as undo
    */
    if (container2.length() >= 1){ // redo only works if the there is atleast 1 node to redo to
    container1.push(container2.pop());
    curr_size += 1;

    //read from version file corresponding to top-most node
    VCS_node latest_top = container1.top();
    string latest_filename = "";
    ifstream latestfile;
    stringstream converter;
    converter << latest_top.time_stamp;
    latest_filename = filename+converter.str()+".txt";
    converter.str("");
    latestfile.open(latest_filename.c_str());
    if (latestfile.fail()){
        return "";
    }
    string line = "", latest_data = "";
    while(latestfile >> line){
    latest_data += line;
    }
    latestfile.close();

    //write data obtained from version file to original file to modify it
    ofstream originalfile;
    string original_filename = filename+".txt";
    originalfile.open(original_filename.c_str());
    if (originalfile.fail()){
        return "";
    }
    originalfile << latest_data;
    originalfile.close();

    return latest_data;
    }

    return "";
}

//goes t_stamp step back or ahead
string VCS::revert(int t_stamp)
{
    // Make the top-most node of the main stack the VCS_node with t_stamp
    if (t_stamp < container1.top().time_stamp)
        while (t_stamp != container1.top().time_stamp){
            container2.push(container1.pop());
        }
    else if (t_stamp > container1.top().time_stamp)
        while (t_stamp != container1.top().time_stamp){
            container1.push(container2.pop());
        }

    //Copy top-most node / current version node's data from its corresponding file to the original file
    VCS_node latest_top = container1.top();
    string latest_filename = "";
    ifstream latestfile;
    stringstream converter;
    converter << latest_top.time_stamp;
    latest_filename = filename+converter.str()+".txt";
    converter.str("");
    latestfile.open(latest_filename.c_str());
    if (latestfile.fail()){
        return "";
    }
    string line = "", latest_data = "";
    while(latestfile >> line){
    latest_data += line;
    }
    latestfile.close();

    //write to file
    ofstream originalfile;
    string original_filename = filename+".txt";
    originalfile.open(original_filename.c_str());
    if (originalfile.fail()){
        return "";
    }
    originalfile << latest_data;
    originalfile.close();

    return latest_data;
}

#endif

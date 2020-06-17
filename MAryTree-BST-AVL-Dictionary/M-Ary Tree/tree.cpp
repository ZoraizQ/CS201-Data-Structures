#ifndef __TREE_CPP
#define __TREE_CPP
#include "tree.h"
#include <string>
#include <sstream>

// try to make get_node() and get_file() after tree initialization

/*
m-ary tree, with the assumption that m = 9, used to create VCS-ver2.0
No need to do file handling for all the different version files, just handle data in nodes

Every (TreeItem) node in the tree has an ID, contains a string file 
(latest_data -- same as data of VCS_node with highest time_stamp) + 
all of its versions (VCS_node's) in a vector (container) 
-- note that VCS_node also store data now of their version file
-- also VCS_node's parametric constructor is VCS_node(d, t) for data and time stamp
and a <TreeItem*> vector for children pointers called child,
also has parametric constructor TreeItem(Id, Val)

branch(n) -- creates a new child of n
e.g. if the id of n was 1, and child for 11 existed, then child for 12 created
so each child specifies a branch

source_node.merge(destination_node) --  data from src is appended to latest version data of dst

merge_state struct storing the ints merge_state.src_id and merge_state.dst_id
(source and destination node ids that merged)
history = vector in the tree of type T = merge_state which stores the merge history 

Convention:
TreeItem* root has an ID of 1
Further branches have IDs -- 11,12,13..19
Branches of node 11 will have IDs 111,112,113...119

Vector functions:
begin() -- return first element
end() -- return last element
rbegin() -- reverse begin, rend() -- reverse end
possibility to traverse the vector in reverse if element exists in second half of the vector
(if version to get is 113, then extract last digit (3) on the tree node with ID (11) and check if 3 >= 9/2 or 3 < 9/2)
size()
max_size()
resize(new_size)
empty() -- check if container is empty
push_back(new_elem)
pop_back(existing_elem)
*/

//data = data of orignal file under the VCS
Tree::Tree(string data) 
{
	TreeItem* new_node= new TreeItem();
	new_node->id = 1;

	// write the rest of the code here
	new_node->latest_data = data; //latest data updated to the first data given to us from the original file
	//will have no children
	//cout << new_node->child.empty() << '\n' << new_node->container.empty() << '\n';

	//very first node pushed
	new_node->container.push_back(VCS_node(data, 0)); //initial time_stamp = 0

	//since this is the first node, becomes the root of the tree
	root = new_node;
}

void recursiveDelete(TreeItem* p){
	if (p == NULL)
		return;
	else if (p->child.empty()){
		cout << "Deleting " << p->id << endl;
		delete p;
	}
	else{
		for(int i = 0; i < p->child.size(); i++){
			recursiveDelete(p->child[i]);
		}
		cout <<"Deleting " << p->id << endl;
		delete p;
	}
}

Tree::~Tree()
{	
	cout <<"\nDestructor initiated.\n";
	recursiveDelete(root);
	//delete all tree nodes
}


void Tree::branch(int id)
{
	TreeItem* parent_node = get_node(id);
	if (parent_node == NULL) //node not found
		return;

	string parent_data = parent_node->latest_data;
	int new_id = id*10 + 1; //if very first child, then id would be 1121 from 112 of parent
	if (!parent_node->child.empty()){
		new_id = parent_node->child.back()->id + 1; //calculating child id by increment of previous child's id
	}

	//create new child node with parent node's latest data, and new id calculated
	TreeItem* new_child =  new TreeItem(new_id, parent_data);
	//also create very first version node (t=0) for the child as in initialise, so container is never empty for any tree node
	new_child->container.push_back(VCS_node(parent_data, 0));

	//push child into parent's child vector, becomes part of the children, new branch created
	parent_node->child.push_back(new_child);
}

void Tree::merge(int src_id,int dst_id)
{
	TreeItem* src_node = get_node(src_id);
	TreeItem* dst_node = get_node(dst_id);
	if (src_node == NULL || dst_node == NULL) //if even one of these nodes are not found
		return;

	//new merged data for dst
	string new_dst_data = dst_node->latest_data + src_node->latest_data;

	//update latest data and latest version node to new merged data
	dst_node->container.back().data = new_dst_data;
	dst_node->latest_data = new_dst_data;

	//update merge history
	merge_state new_ms;
	new_ms.src_id = src_id;
	new_ms.dst_id = dst_id;
	history.push_back(new_ms);
}

void Tree::commit(int src_id,string new_data)
{
	TreeItem* src_node = get_node(src_id); //get corresponding source node
	if (src_node == NULL) //node not found
		return;

	//get last version node's time stamp and increment it for new time stamp
	int new_ts = src_node->container.back().time_stamp+1;

	//push new version node into container vector for current node
	src_node->container.push_back(VCS_node(src_node->latest_data+new_data, new_ts));

	src_node->latest_data += new_data;  //update latest_data to new data added

}

string Tree::get_file(int id, int t_stamp)
{
	TreeItem* file_node = get_node(id);
	if (file_node == NULL) //node not found
		return "";

	//t_stamp = 0, version node at index 0 needed, so same thing
	int version_node_index = t_stamp;

	if (version_node_index <= file_node->container.size()-1){ //to check if such a version node exists
		return file_node->container[version_node_index].data;
	}
	else //no such version exists
		return "";
} 

//x = number of digits to get starting from first
int get_digits(int integer, int x){
	stringstream converter, converter2;
	converter << integer;
	converter2 <<converter.str().substr(0,x);
	int ret;
	converter2 >> ret;
	return ret;
}


TreeItem* Tree::get_node(int id)
{	
	if (root == NULL) //empty tree
		return NULL;
	else if(get_digits(id,1) != root->id){ //if first digit does not match the root ID specified in initialise (1) then invalid id given
		return NULL;
	}
	else{
		TreeItem* curr_node = root;
		int curr_pos = 2; //initially 2, since root's next children to be checked
		
		while(curr_node != NULL){ // until it is not found 
		if (curr_node->id == id){ //if found
			return curr_node;	
		}
		else{ //else check children
			//e.g. 11 for get_digits(1123, 2)
			int next_id = get_digits(id, curr_pos); //to check children
			int next_child_index = next_id%10-1;//last digit of id - 1 will be the index of the vector e.g. node 112 will be the child at index 1 in the vector

			//if vector size is 4, that means i = 3 possible at max
			if (curr_node->child.empty())
				curr_node = NULL;
			else if (next_child_index <= curr_node->child.size()-1) //if there exists a child ptr for the next child id within the child vector of the current node
				curr_node = curr_node->child[next_child_index]; //curr_node becomes the next child node
			else
				curr_node = NULL; //dead end, could not find such a node

		}
	
		curr_pos++;
		}
		//if not found
		return NULL;
	}

} 

vector<merge_state> Tree::merge_history()
{
	return history;
}

void Tree::level_traversal() 
{ 
	if (root == NULL)
		cout << "Empty Tree" << '\n';
	else{
		//print queue for nodes
		Queue<TreeItem*> print_queue;

		print_queue.enqueue(root); //enqueue the root first, before starting the printing

		cout << "[" << root->id << ", " << root->latest_data <<"]\n"; //root
		while(!print_queue.isEmpty()){
			TreeItem* curr_node = print_queue.dequeue();
			if (!curr_node->child.empty()){
				for (int i = 0; i < curr_node->child.size(); i++){
					cout << "[" << curr_node->child[i]->id << ", " << curr_node->child[i]->latest_data << "] "; //root
					print_queue.enqueue(curr_node->child[i]);
				}
				cout << '\n';
			}
		}
	}
} 


#endif

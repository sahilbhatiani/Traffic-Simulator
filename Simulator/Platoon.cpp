#include "Platoon.h"

Platoon::Platoon(string init)
{
    // the string 'init' looks like:
    // a,b ; c,d ; ... where, a and c are the vehicle id, b and d are positions.
    stringstream ss(init);
	string segment;
	while(getline(ss, segment, ';'))
	{
		size_t found = segment.find(",");
		if (found!=std::string::npos){
			string id = segment.substr(0, found);
			string pos = segment.substr(found+1, segment.size()-1);

			// conversion to integer
			stringstream s1(id);
			int id_value;
			s1 >> id_value;
			stringstream s2(pos);
			int pos_value;
			s2 >> pos_value;
			Car* car = new Car(id_value, pos_value);

			this->insert(car); //Makes sure to sort the plattoon in ascending order of position value.
           
		}
	}
}

Car* Platoon::get_tail()
{
	return tail;
}

Car* Platoon::get_head()
{
	return head;
}

//Adds node to the start of the linked list
void Platoon::prepend(Car* c){
	c->set_next(head);
	head->set_prev(c);
	head = c;
	c->set_prev(NULL);
}
//Adds node to the end of the linked list
void Platoon::append(Car* c){
	tail->set_next(c);
	c->set_prev(tail);
	tail = c;
	c->set_next(NULL);
}



//Inserts the node in a platoon, placing it in ascening order of position.
void Platoon::insert(Car* c){
	Car* positionptr = head; 
	int count = 0;
	
	//Case 1: If the platoon is an empty linked list
	if(head == NULL && tail == NULL){
		head = c;
		tail = c;
		c->set_next(NULL);
		c->set_prev(NULL);
	}
	else{
		//The while loop and the count variable is used to determine which position the node needs to be inserted in.
		while(count<this->size() && c->get_position() > positionptr->get_position()){
				positionptr = positionptr->get_next();
				count++;
		}
		
		//Case 2: When the node to be inserted is going to be the new head node.
		if(count == 0){
			this->prepend(c);
		}
		else{
			//Case 3: When the node to be inserted is going to the the new tail node.
			if(count == this->size()){    
				this->append(c);
			}
			//Case 4: When the node is to be inserted anywhere else in the platoon linked list.
			else{
					c->set_prev(positionptr->get_prev());
					c->get_prev()->set_next(c);
					c->set_next(positionptr);
					positionptr->set_prev(c);
			}
		}
	

	}


	
	
	
	

}

//Removes a particular node from a platoon linked list.
void Platoon::remove(Car* c){
	//Case 1: If node to be removed is the only node in the linked list.
	if(head == c && tail==c){
		head = NULL;
		tail = NULL;
	}
	else{
		//Case 2: If the node to be removed is the head node.
		if(c == head){
			head = c->get_next();
			head->set_prev(NULL);
		}
		else{
			//Case 3: If the node to be removed is the tail node.
			if(c == tail){
				tail = c->get_prev();
				tail->set_next(NULL);
			}
			//Case 4: If a node from any other position needs to be removed.
			else{
				c->get_prev()->set_next(c->get_next());
				c->get_next()->set_prev(c->get_prev());
			}
		}
	}
}

//Finds the number of nodes present in a platoon.
int Platoon::size(){
	int count = 0;
	Car* testpointer = head;
	while(testpointer != NULL){
		testpointer = testpointer->get_next();
		count++;
	}

	return count;
}

//Checks if there is any other node present in the platoon with the same position value.
int Platoon::checkAvailablePosition(int pos){
	int result = 0;
	Car* testpointer = head;
	while(testpointer != NULL){
		if(testpointer->get_position() == pos){
			result = 1;
		}
		testpointer = testpointer->get_next();
	}
	return result;
}

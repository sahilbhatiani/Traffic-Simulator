#include "TrafficModel.h"
#include "iostream"

TrafficModel::TrafficModel() { }
TrafficModel::~TrafficModel(){ }

void TrafficModel::set_commands(vector<string> commands)
{
	this->commands = commands;
}

/* A helper function. 
 * Given a specific id as the input, it searches the lane change command stored in
 * "commands". Returns the value of the turn-light in the command for this car id.
 * 0 = no light, 1 = left light, 2 = right light.
 */
int TrafficModel::get_lane_change_command(int id)
{
	int count = commands.size();
	for (int i = 0; i < count; i++){
		size_t found = commands[i].find(',');
		string iter_id = commands[i].substr(0, found);
		string light = commands[i].substr(found+1, commands[i].size()-1);
		stringstream ss1(iter_id);
		int id_value;
		ss1 >> id_value;
		if (id_value == id){
			stringstream ss2(light);
			int light_value;
			ss2 >> light_value;
			return light_value;
		}
	}
	return 0;
}

/*
 * The function that updates the vehicle positions and states.
 */
void TrafficModel::update()
{
	//TODO
	Car* ptr;  //This pointer is used to loop through all the nodes in the platoon and update their position and state.
	Car* ptr2;  //This pointer is used to reset the "moved" variable of the nodes.
	int ptrcommand;
	for(unsigned int i=0; i<platoons.size();i++){
		
		ptr = platoons[i].get_tail();
		while(ptr != NULL){
			
			ptrcommand = get_lane_change_command(ptr->get_id());

			//Case 1: Command to move vehicle forward.
			if(ptrcommand == 0){
				//Condition checks if there is space in front and moves or doesnt move the car accordingly.
				if(platoons[i].checkAvailablePosition(ptr->get_position()+1) == 0 && ptr->isMoved() == false){
					ptr->set_position(ptr->get_position()+1);
				}
				ptr->changeMove(true);  //Update that car has been moved. Will ensure that it is not moved twice in the same tick.
				ptr = ptr->get_prev();  
			}

			//Case 2: Command to move car left.
			if(ptrcommand == 1){ 
				//Condition checks if car is in the leftmost lane or not. If it is, it cannot take a left. So must move straight 
				// instead. So condition checks for moving straight.
				if(i == 0){
					if(platoons[i].checkAvailablePosition(ptr->get_position()+1) == 0  && ptr->isMoved() == false){
						ptr->set_position(ptr->get_position()+1);
					}
					ptr->changeMove(true);
					ptr = ptr->get_prev();
					

				}
				else{
					//If car is not in leftmost lane, then condition checks if destination spot in the left lane
					// is available and takes action accordingly.
					if(platoons[i-1].checkAvailablePosition(ptr->get_position()) == 0 && ptr->isMoved() == false){
						Car* dummy = ptr;
						ptr->changeMove(true);
						ptr = ptr->get_prev();
						platoons[i].remove(dummy);
						platoons[i-1].insert(dummy);
						

					}
					else{
						//If there's no space to move left, the condition checks if there's space to move forward and takes action accordingly.
						if(platoons[i].checkAvailablePosition(ptr->get_position()+1) == 0 && ptr->isMoved() == false){
							ptr->set_position(ptr->get_position()+1);
					}
						ptr->changeMove(true);
						ptr = ptr->get_prev();
					}
				}
			}

			//Case 3: Command to move car right.
			//Similar logic used as in Case 2.
			if(ptrcommand == 2){  
				if(i == (platoons.size()-1)){
					if(platoons[i].checkAvailablePosition(ptr->get_position()+1) == 0 && ptr->isMoved() == false){
						ptr->set_position(ptr->get_position()+1);
					
					}
					ptr->changeMove(true);
					ptr = ptr->get_prev();
				}
				else{
					if(platoons[i+1].checkAvailablePosition(ptr->get_position()) == 0 && ptr->isMoved() == false){
						Car* dummy = ptr;
						ptr->changeMove(true);
						ptr = ptr->get_prev();
						platoons[i].remove(dummy);
						platoons[i+1].insert(dummy);
						

					}
					else{
						if(platoons[i].checkAvailablePosition(ptr->get_position()+1) == 0 && ptr->isMoved() == false){
						ptr->set_position(ptr->get_position()+1);
					}
						ptr->changeMove(true);
						ptr = ptr->get_prev();
					}
				}

			}
			
		
		}
		
		
		}
		//This for loop refreshes every car's moved variable status to false before the next tick.
		for(unsigned int i=0; i<platoons.size(); i++){
			ptr2 = platoons[i].get_head();
			while(ptr2 != NULL){
				ptr2->changeMove(false);
				ptr2 = ptr2->get_next();

			}
	
	}
}


/*
 * Initialization based on the input information
 */
void TrafficModel::initialize(vector<string> info)
{
	int lane_count = info.size();
	for (int i = 0; i < lane_count; i++){
		Platoon p = Platoon(info[i]);
		platoons.push_back(p);
	}
}

// Returns all the vehicle states in the system
vector<string> TrafficModel::get_system_state()
{
	vector<string> output;
	int size = platoons.size();
	for (int i = 0; i < size; i++){
		// get the last vehicle in the platoon
		Car* temp = platoons[i].get_tail();
		string s = "";
		ostringstream out;
		while (temp != NULL){
			out << ";(" << temp->get_id() << "," << i << "," << temp->get_position() << \
					 "," << get_lane_change_command(temp->get_id()) << ")";
			temp = temp->get_prev();
		}

		output.push_back(out.str());
	}
	return output;
}

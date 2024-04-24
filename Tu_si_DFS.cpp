#include<iostream>
#include<queue>
#include<cstdlib>
#include<stack>
using namespace std;
#define Maxlength 1000
#define tankcapacity_Monk 3
#define tankcapacity_Devil 3
const char* action[]={"First State","Move 1 Monk","Move 1 Devil","Move 2 Monk","Move 2 Devil","Move 1 Monk and 1 Devil"};
typedef struct{
	int Monk;
	int Devil;
	int pos_boat; //0:thuyen ben A, 1:thuyen ben B
}State;
//Khoi tao trang thai ban dau
void makeNullState(State *state){
	state->Monk=3;
	state->Devil=3;
	state->pos_boat=0;
}
//Kiem tra muc tieu
int checkgoal(State state){
	return(state.Monk==0 && state.Devil==0 && state.pos_boat==1);
}
//Kiem tra dead
int checkdead(State state){
	if((state.Monk>0 && state.Monk<state.Devil)||(tankcapacity_Monk-state.Monk>0 && tankcapacity_Monk-state.Monk<tankcapacity_Devil-state.Devil))
		return 1;
	return 0;
}
//In trang thai
void printState(State state){
	printf("\n Monk: %d --- Devil: %d --- Ben bo: ",state.Monk,state.Devil);
	if(state.pos_boat==0){
		printf("A");
	}
	else{
		printf("B");
	}
}
//Di chuyen 1 Monk
int Move1Monk(State cur_state, State *result){
	if(cur_state.pos_boat==0){ //ben bo A
		if(cur_state.Monk>0){
			result->Monk=cur_state.Monk-1;
			result->Devil=cur_state.Devil;
			result->pos_boat=1;
			return 1;
		}
	}
	else{ //ben bo B
		if(cur_state.Monk<3){
			result->Monk=cur_state.Monk+1;
			result->Devil=cur_state.Devil;
			result->pos_boat=0;
			return 1;
		}
	}
	return 0;
}
//Di chuyen 1 Devil
int Move1Devil(State cur_state, State *result){
	if(cur_state.pos_boat==0){ //ben bo A
		if(cur_state.Devil>0){
			result->Devil=cur_state.Devil-1;
			result->Monk=cur_state.Monk;
			result->pos_boat=1;
			return 1;
		}
	}
	else { //ben bo B
		if(cur_state.Devil<3){
			result->Devil=cur_state.Devil+1;
			result->Monk=cur_state.Monk;
			result->pos_boat=0;
			return 1;
		}
	}
	return 0;
}
//Di chuyen 2 Monk
int Move2Monk(State cur_state, State *result){
	if(cur_state.pos_boat==0){ //ben bo A
		if(cur_state.Monk>=2){
			result->Monk=cur_state.Monk-2;
			result->Devil=cur_state.Devil;
			result->pos_boat=1;
			return 1;
		}
	}
	else{ //ben bo B
		if(cur_state.Monk<=1){
			result->Monk=cur_state.Monk+2;
			result->Devil=cur_state.Devil;
			result->pos_boat=0;
			return 1;
		}
	}
	return 0;
}
//Di chuyen 2 Devil
int Move2Devil(State cur_state, State *result){
	if(cur_state.pos_boat==0){ //ben bo A
		if(cur_state.Devil>=2){
			result->Devil=cur_state.Devil-2;
			result->Monk=cur_state.Monk;
			result->pos_boat=1;
			return 1;
		}
	}
	else{ //ben bo B
		if(cur_state.Devil<=1){
			result->Devil=cur_state.Devil+2;
			result->Monk=cur_state.Monk;
			result->pos_boat=0;
			return 1;
		}
	}
	return 0;
}
//Di chuyen 1 Monk 1 Devil
int Move1Monk1Devil(State cur_state, State *result){
	if(cur_state.pos_boat==0){ //ben bo B
		if(cur_state.Monk>0 && cur_state.Devil>0){
			result->Monk=cur_state.Monk-1;
			result->Devil=cur_state.Devil-1;
			result->pos_boat=1;
			return 1;
		}
	}
	else{
		if(cur_state.Monk<3 && cur_state.Devil<3){
			result->Monk=cur_state.Monk+1;
			result->Devil=cur_state.Devil+1;
			result->pos_boat=0;
			return 1;
		}
	}
	return 0;
}
//Goi cac phep toan tren trang thai
int call_operator(State cur_state, State *result, int option){
	switch(option){
		case 1:return Move1Monk(cur_state, result);
		case 2:return Move1Devil(cur_state, result);
		case 3:return Move2Monk(cur_state, result);
		case 4:return Move2Devil(cur_state, result);
		case 5:return Move1Monk1Devil(cur_state, result);
		default:printf("Error calls operator\n");
			return 0;
	}
}
//Khai bao cau truc Node
typedef struct Node{
	State state;
	struct Node* Parent;
	int no_function;
}Node;
//compareStates
int compareStates(State s1, State s2){
	if(s1.Monk==s2.Monk && s1.Devil==s2.Devil && s1.pos_boat==s2.pos_boat)
		return 1;
	return 0;
}
//Tim kiem trang thai trong Stack Open/Close
int find_State(State state, stack<Node*> Q){
	while(!Q.empty()){
		if(compareStates(Q.top()->state,state))
			return 1;
		Q.pop();
	}
	return 0;
}
//DFS_Algorithm
Node* DFS_Algorithm(State state){
	stack<Node*> open_DFS;
	stack<Node*> close_DFS;
	Node* root=(Node*)malloc(sizeof(Node));
	root->state=state;
	root->Parent=NULL;
	root->no_function=0;
	open_DFS.push(root);
	while(!open_DFS.empty()){
		Node* node=open_DFS.top();
		open_DFS.pop();
		close_DFS.push(node);
		if(checkgoal(node->state))
			return node;
		int opt;
		for(opt=1; opt<=5; opt++){
			State newState;
			makeNullState(&newState);
			if(call_operator(node->state,&newState,opt)){
				if(find_State(newState,open_DFS)||find_State(newState,close_DFS)||checkdead(newState))
					continue;
				Node* newNode=(Node*)malloc(sizeof(Node));
				newNode->state=newState;
				newNode->Parent=node;
				newNode->no_function=opt;
				open_DFS.push(newNode);
			}
		}
	}
}
//In ket qua
void print_WaysToGetGoal(Node* node){
	stack<Node*> S;
	while(node->Parent!=NULL){
		S.push(node);
		node=node->Parent;
	}
	S.push(node);
	int no_action=0;
	while(!S.empty()){
		cout << "\n Action "<< no_action << ":"<< action[S.top()->no_function];
		printState(S.top()->state);
		S.pop();
		no_action++;
	}
}
//Main
int main(){
	State S;
	makeNullState(&S);
	Node* d=DFS_Algorithm(S);
	print_WaysToGetGoal(d);
	return 0;
}

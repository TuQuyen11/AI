#include<iostream>
#include<stack>
#include<queue>
#include<cstdlib>
using namespace std;
const char* action[]={"First State","Flip 1-2-3","Flip 3-4-5","Flip 4-5-6"};
#define NumGlasses 6
#define Up 1
#define Down -1
//Khai bao cau truc trang thai
typedef struct{
	int Glasses[NumGlasses];
}State;
//Khoi tao khong gian rong
void makeNullState(State *state){
	for(int i=0; i<NumGlasses; i++){
		if(state->Glasses[i]%2==0)
			state->Glasses[i]=Up;
		else
			state->Glasses[i]=Down;
	}
}
//In trang thai
void printState(State S){
	cout<<"\n";
	for(int i=0; i<NumGlasses; i++){
		if(S.Glasses[i]==Up)
			cout<< "U   ";
		else
			cout<< "D   ";
	}
}
//Kiem tra muc tieu
int goalcheck(State S){
	for(int i=0; i<NumGlasses; i++){
		if(S.Glasses[i]==Down)
			return 0;
	}
	return 1;
}
//Up ly
void flipGlasses(State cur_state, State *result, int k){
	for(int i=0; i<NumGlasses; i++){
		result->Glasses[i]=cur_state.Glasses[i];
	}
	result->Glasses[k-1]=-cur_state.Glasses[k-1];
	result->Glasses[k]=-cur_state.Glasses[k];
	result->Glasses[k+1]=-cur_state.Glasses[k+1];
}
//Khai bao cau truc Node
typedef struct Node{
	State state;
	struct Node* Parent;
	int no_function;
}Node;
//compareStates
int compareStates(State s1, State s2){
	int i;
	for(i=0; i<NumGlasses; i++){
		if(s1.Glasses[i]!=s2.Glasses[i])
			return 0;
	}
	return 1;
}
//Tim kiem trang thai trong Stack open/close
int findState(State state, stack<Node*> open){
	while(!open.empty()){
		if(compareStates(open.top()->state,state))
			return 1;
		open.pop();
	}
	return 0;
}
//DFS_Algorithm
Node* DFS_Algorithm(State state){
	stack<Node*> open_DFS;
	stack<Node*> close_DFS;
	//Tao nut trang thai cha
	Node* root=(Node*)malloc(sizeof(Node));
	root->state=state;
	root->Parent=NULL;
	root->no_function=0;
	open_DFS.push(root);
	while(!open_DFS.empty()){
		Node* node=open_DFS.top();
		open_DFS.pop();
		close_DFS.push(node);
		if(goalcheck(node->state))
			return node;
		int opt;
		for(opt=1; opt<=4; opt++){
			State newState;
			makeNullState(&newState);
			flipGlasses(node->state,&newState,opt);
			if(findState(newState,open_DFS)||findState(newState,close_DFS))
				continue;
			Node* newNode=(Node*)malloc(sizeof(Node));
			newNode->state=newState;
			newNode->Parent=node;
			newNode->no_function=opt;
			open_DFS.push(newNode);
		}
	}
	return NULL;
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
		printf("\nAction %d: %s",no_action,action[S.top()->no_function]);
		printState(S.top()->state);
		S.pop();
		no_action++;
	}
}
//Main
int main(){
	State cur_state={Up,Down,Up,Down,Up,Down};
	Node* p=DFS_Algorithm(cur_state);
	print_WaysToGetGoal(p);
	return 0;
}

/************************************************
 *      Copyright 2007                          *
 *      Georgia Tech Research Corporation       *
 *      Atlanta, GA  30332-0415                 *
 *      All Rights Reserved                     *
 ************************************************/

#ifndef BasicBlock_h
#define BasicBlock_h

#include <set>
#include <queue>
#include "pin.H"
#include <map>

using namespace std;

class BasicBlock {

public:

queue <ADDRINT> body;
int visited;
int isEntry;
const char * routineCalled;

//The following two variables are CFG node pointers. Successors are basic blocks
//where control is passed to, while predecessors are back pointers where control
//was passed from
set < void * > successors;
set < void * > predecessors;

//These variables are dominance pointers. The map of "dominated" nodes refer to all
//nodes that are dominated by the current block, both immediately and recursively
//The "Dominator" variable is the pointer to the immmediate dominator of the current
//node
//Finally, the immediatelyDominated map set is a subset of the dominated set, where
//the nodes are only those immediately dominated by the current node
//NOTE: I understand that dominance isn't be used as of quite yet, but I've implemented its
//functionality anyway.
map<ADDRINT, BasicBlock *> dominated;
BasicBlock * Dominator;
map<ADDRINT, BasicBlock *>immediatelyDominated;

//Similarly these variables, are post-dominance pointers. The map of postDominated nodes
//are those immmediately and recursively postDominated by the current node.
//The PostDominator is a pointer to the postDominating node of the current one.
//Finally, as requested, is the "immediatelyPostDominated" map subset. 
map<ADDRINT, BasicBlock *> postDominated;
BasicBlock * PostDominator;
map<ADDRINT, BasicBlock *> immediatelyPostDominated;

ADDRINT startingAddress;
ADDRINT endingAddress;
int isPostDominated;

BasicBlock() {
	visited = 0;
	endingAddress = 0;
	startingAddress=0;
	routineCalled = "";
	isPostDominated = 0;
	Dominator = NULL;
	PostDominator = NULL;
}
virtual ~BasicBlock(){


}

void setRoutineCalled(const char * routineName) {
	routineCalled = routineName;
}

void visitNode() {
	visited = 1;
}

void revisitNode() {
	visited = 2;
}

int getVisited() {
	return visited;
}


void setAsEntryNode() {
	isEntry = 1;
}

int getIsEntry() {
	return isEntry;
}


void printDetailedInformation(FILE * graphFile) {

	if (startingAddress == 1) {
		set< void *>::iterator setIterator;
		setIterator = predecessors.begin();
		BasicBlock* parentBlock = (BasicBlock *)(* setIterator);
	
	
		fprintf(graphFile, "Node%lucall[shape = record, label = \" call to  routine  %s  \" , width = 1, fontsize = 10] ;\n", parentBlock->startingAddress, routineCalled);
	} else if (startingAddress == 2) {
		fprintf(graphFile, "Node%lu %lu [shape = record, label = \"Return Node {%#lx | %#lx} \" , width = 1, fontsize = 10] ;\n", startingAddress, startingAddress, startingAddress, endingAddress);
	} else if (isEntry == 1) {
		fprintf(graphFile, "Node%lu [shape = record, label = \"Entrance Node | {%#lx | %#lx}  \" , width = 1, fontsize = 10] ;\n", startingAddress, startingAddress, endingAddress);
	} else if (startingAddress == 4) {
		set< void *>::iterator setIterator;
		setIterator = predecessors.begin();
		BasicBlock* parentBlock = (BasicBlock *)(* setIterator);
	
		fprintf(graphFile, "Node%luIC[shape = record, label = \"Indirect Call  \" , width = 1, fontsize = 10] ;\n", parentBlock->startingAddress);
	} else if (startingAddress == 5) {
		set< void *>::iterator setIterator;
		setIterator = predecessors.begin();
		BasicBlock* parentBlock = (BasicBlock *)(* setIterator);
	
		fprintf(graphFile, "Node%luIB[shape = record, label = \"Indirect Branch  \" , width = 1, fontsize = 10] ;\n", parentBlock->startingAddress);
	} else {
		fprintf(graphFile, "Node%lu [shape = record, label = \"Regular Node | {%#lx | %#lx}  \" , width = 1, fontsize = 10] ;\n", startingAddress, startingAddress, endingAddress);
	}


}

void printInformation() {


}

set< void * > getSuccessors() {

	return successors;
}

set<void *> getPredecessors() {

	return predecessors;
}

void insertPreds( BasicBlock* pBlock) {
	predecessors.insert(pBlock);
}

void insertBody(ADDRINT address){
	body.push(address);

}

void insertSuccs( BasicBlock* pBlock){
	successors.insert(pBlock);
	//cout << "block added to succs" << endl;

}

void setEnding(ADDRINT endAddr){
	endingAddress = endAddr;
}

ADDRINT getEnding() {
	return endingAddress;
}

void setStarting(ADDRINT startAddr){
	startingAddress = startAddr;
}

ADDRINT getStarting() {
	return startingAddress;
}

void setDominator(BasicBlock * block) {
	Dominator = block;
}

void setPostDominator(BasicBlock * block) {
	isPostDominated = 1;
	PostDominator = block;
}

BasicBlock * getDominator() {
	return Dominator;
}

BasicBlock * getPostDominator() {
	return PostDominator;
}


BasicBlock * getCommonDominator(BasicBlock * siblingBlock) {
		//fprintf(stderr, "  looking at %#x \n", getStarting());
		if (dominated[siblingBlock -> getStarting()] != NULL) {//this node is a parent to both siblingBlocks
		//fprintf(stderr, "  found a parent at\n");
			return this; 
		} else if (isEntry == 1) {//this means its the entry
			//fprintf(stderr, "  found entry node\n");	
			return this;
		} else {
		//fprintf(stderr, "  going up a level \n");
			return Dominator->getCommonDominator(siblingBlock);
		}
}

BasicBlock * getCommonPostDominator(BasicBlock * siblingBlock) {
		if (postDominated[siblingBlock -> getStarting()] != NULL) {//this node is a parent to both siblingBlocks
		//fprintf(stderr, "  found a post parent at\n");
			return this; 
		} else if (getStarting() == 2) {//this means its the return
			return this;
		} else {
		//fprintf(stderr, "  going up a level \n");
			return PostDominator->getCommonPostDominator(siblingBlock);
		}
}

void addImmediateDominator(BasicBlock * newBlock) {
	immediatelyDominated[newBlock->getStarting()] = newBlock;
}

void addImmediatePostDominator(BasicBlock * newBlock) {
	immediatelyPostDominated[newBlock->getStarting()] = newBlock;
}

void addSuccessorAsDominated(BasicBlock * successor) {
		if (successor->getDominator() == NULL) {
		//fprintf(stderr, "  and yes it is the dominant by default \n");
			successor ->setDominator(this);
			addImmediateDominator(successor);
		//fprintf(stderr, "  passing up the new info \n");
			passUpDominanceInfo(successor);
		} else {
		//fprintf(stderr, "  and no, there is another immediate successor...looking for common dominator \n");
			BasicBlock * competingSibling = successor ->getDominator();
			BasicBlock * newDominator = getCommonDominator(successor);
			immediatelyDominated.erase(successor->getStarting());

//fprintf(stderr, " comparint %#x and %#x \n", successor->getStarting(),  bigDominator->getStarting());
			competingSibling->deleteUpDominanceInfo(successor -> getdominated(), successor->getStarting(), newDominator->getStarting());
		//fprintf(stderr, " finished deleting\n");
			successor->setDominator(newDominator);
			newDominator->addImmediateDominator(successor);
			
		}
}

void addPredecessorAsPostDominated(BasicBlock * predecessor) {
		//fprintf(stderr, "block: %#x is  adding block %#x \n",getStarting(), predecessor->getStarting());
//		BasicBlock * postDomin = predecessor->getPostDominator();
		if (predecessor->isPostDominated == 0) {
		//fprintf(stderr, "  and yes it is the Postdominant by default \n");
			predecessor ->setPostDominator(this);
			addImmediatePostDominator(predecessor);
		//fprintf(stderr, "  passing down the new info \n");
			passDownPostDominanceInfo(predecessor);
		} else {
		//fprintf(stderr, "  and no, there is another immediate predecessor ...looking for common post dominator \n");
			BasicBlock * competingSibling = predecessor ->getPostDominator();
			BasicBlock * newPostDominator = getCommonPostDominator(predecessor);
			immediatelyPostDominated.erase(predecessor->getStarting());
			competingSibling->deleteDownPostDominanceInfo(predecessor -> getPostDominated(), predecessor->getStarting(), newPostDominator->getStarting());
		//fprintf(stderr, " finished deleting\n");
			
			predecessor->setPostDominator(newPostDominator);
			newPostDominator->addImmediatePostDominator(predecessor);

			
		}
}

map<ADDRINT, BasicBlock *> getdominated() {
	return dominated;
}

map<ADDRINT, BasicBlock *> getPostDominated() {
	return postDominated;
}

void passUpDominanceInfo(BasicBlock * newChild) {
	dominated[newChild->getStarting()] = newChild;
		//fprintf(stderr, "  added new info \n");
	if (isEntry == 0) {//if not entry, pass up
		Dominator->passUpDominanceInfo(newChild);
	}

}

void passDownPostDominanceInfo(BasicBlock * newChild) {
	postDominated[newChild->getStarting()] = newChild;
		//fprintf(stderr, "  added new info \n");
	if (getStarting() != 2) {//if not return, pass up
		PostDominator->passDownPostDominanceInfo(newChild);
	}

}

void deleteUpDominanceInfo(map <ADDRINT, BasicBlock *> deletableBlocks, ADDRINT deletableBlocksParent, ADDRINT lastNodesStarting) {
//fprintf(stderr, " comparint %#x and %#x \n", startingAddress,  lastNodesStarting);		
	if (startingAddress < lastNodesStarting ) {
		dominated.erase(deletableBlocksParent);
		map <ADDRINT, BasicBlock *>::iterator mapIterator;
		for (mapIterator = deletableBlocks.begin(); mapIterator != deletableBlocks.end(); mapIterator++) {
		//fprintf(stderr, "  deleting info from  node:\n");
		printInformation();
		//fprintf(stderr, "  Dominanated node %#x :\n", (ADDRINT(mapIterator -> first)));
		dominated.erase((ADDRINT(mapIterator -> first)));
		

		}
		if (Dominator) {
		Dominator->deleteUpDominanceInfo(deletableBlocks, deletableBlocksParent, lastNodesStarting);
		}

	}
		
}

void deleteDownPostDominanceInfo(map <ADDRINT, BasicBlock *> deletableBlocks, ADDRINT deletableBlocksParent, ADDRINT lastNodesStarting) {

	if (startingAddress !=lastNodesStarting ) {
		//fprintf(stderr, " node is not parent, continue deleting\n");
		postDominated.erase(deletableBlocksParent);
		map <ADDRINT, BasicBlock *>::iterator mapIterator;
		for (mapIterator = deletableBlocks.begin(); mapIterator != deletableBlocks.end(); mapIterator++) {
		//fprintf(stderr, "  deleting info from  node:\n");
		printInformation();
		//fprintf(stderr, "  postDominanated node %#x :\n", (ADDRINT(mapIterator -> first)));
		postDominated.erase((ADDRINT(mapIterator -> first)));
		

		}
		if (PostDominator)
		PostDominator->deleteDownPostDominanceInfo(deletableBlocks, deletableBlocksParent, lastNodesStarting);

	}

}


};
#endif

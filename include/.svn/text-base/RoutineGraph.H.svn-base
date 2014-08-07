/************************************************
 *      Copyright 2007                          *
 *      Georgia Tech Research Corporation       *
 *      Atlanta, GA  30332-0415                 *
 *      All Rights Reserved                     *
 ************************************************/

#ifndef RoutineGraph_h
#define RoutineGraph_h

#include <map>
#include <string>
#include <queue>
#include <set>
#include "BasicBlock.H"
#include "pin.H"

using namespace std;

class RoutineGraph {

public:

//The 3 variables that make up the useful final bits of the RoutineGraph object

BasicBlock * EntranceNode;//The head of the linked list

BasicBlock * ReturnNode;//The tail of the linked list, this however gets fed in first
//when calculating postdominance

map <ADDRINT , BasicBlock *> addressMap;//All The instructions mapped out over the address space
					//of the routine image onto Basic Blocks

set <BasicBlock *> allBlocks;//A set of all the basicBlocks, both physically in memory and representative,
				//like indirect branch targets and calls

//FILE * resultsFile;
//FILE * debugFile;


RoutineGraph(RTN rtn) {
	//resultsFile = fopen("results.log", "w+");
	//debugFile = fopen("debugFile.log", "w+");
	initializeRoutineInfo(rtn);
	   
}

RoutineGraph(RTN rtn, FILE * specifiedResults, FILE * specifiedDebug) {
	//resultsFile = specifiedResults;
	//debugFile = specifiedDebug;
	initializeRoutineInfo(rtn);
	   
}

void initializeRoutineInfo(RTN rtn) {

	  //const char * rtnName =RTN_Name(rtn).c_str();
	  //fprintf(debugFile, "\n \n enteringFunction %s \n \n", rtnName);

	for (INS ins = RTN_InsHead(rtn); INS_Valid(ins); ins = INS_Next(ins))
	{
		mapper(ins, rtn);//this is the first pass over all the instructions
				//in the routine; it will flag some as branches and some as
				//targets

	}

		//fprintf(debugFile, "Finished mapping");
	//fprintf(debugFile, "\n Got out of mapper loop \n");
	BlockBuilder(rtn);//my second pass function; this routine will start grouping 
//instructions into Blocks. Blocks are started at the beginning of target instructions, and ended
//at the occurence of branch instructions
	queue<BasicBlock *> PDblockQueue;
	buildPostDominanceTree(ReturnNode, PDblockQueue);
//and finally a third pass that uses the built CFG to build a post dominance graph
		//fprintf(debugFile, "Finished 3rd pass ");

}

virtual ~RoutineGraph(){


}


private:
//utility variables
map<ADDRINT, set<ADDRINT> > branches;
map<ADDRINT, set<ADDRINT> > targets;
RTN targetRTN;


//I tweaked around a lot with this function at first, when I was trying to get things to work
//lots of things have changed in it over the course of the program, but the program does seem
//to work correctly
//Well, what I believe is mostly correct; I have made several assumptions on how to implement 
//things. For example, I have made special addresses as flags. This can be tempororay.
void mapper(INS ins, RTN rtn) {

//	ADDRINT exit = 3;
	ADDRINT call = 1;
	ADDRINT ret = 2;
	ADDRINT indirectCall = 4;//NEW FLAGS
	ADDRINT indirectBranch = 5;//NEW FLAGS

	ADDRINT addr = INS_Address(ins);
	//fprintf(debugFile, "\n ins: %#lx", addr);

	if(INS_IsBranch(ins)) {
		if(INS_IsDirectBranchOrCall(ins)) {
		ADDRINT target = INS_DirectBranchOrCallTargetAddress(ins);
	//fprintf(debugFile, "found a Branch pointing to : %#lx", target);
		branches[addr].insert(target);
		targets[target].insert(addr);
	
			if(INS_HasFallThrough(ins)) {
				branches[addr].insert(INS_NextAddress(ins));
				targets[INS_NextAddress(ins)].insert(addr);
				//fprintf(debugFile,"..which has a fall through \n");
			}
		}
		else {//ITS AN INDIRECT BRANCH!!!
			branches[addr].insert(indirectBranch);
			//fprintf(debugFile,"found an indirectBranch at instruction %#lx \n", addr);

			if(INS_HasFallThrough(ins)) {
				branches[addr].insert(INS_NextAddress(ins));
				targets[INS_NextAddress(ins)].insert(addr);
			//fprintf(debugFile,"..which has a fall through \n");
			}
		}
      }//end if is branch
      
      if(INS_IsCall(ins)) {
		if(INS_IsDirectBranchOrCall(ins)) {
//			ADDRINT target = INS_DirectBranchOrCallTargetAddress(ins);
			//fprintf(debugFile, "call");
				branches[addr].insert(call);
			if(INS_HasFallThrough(ins)) {
				branches[addr].insert(INS_NextAddress(ins));
				targets[INS_NextAddress(ins)].insert(addr);
				//fprintf(debugFile,"..which has a fall through \n");
			}
			
			} else {

			branches[addr].insert(indirectCall);
			//fprintf(debugFile, "found an indirectCall at instruction %#lx \n", addr);
			if(INS_HasFallThrough(ins)) {
				branches[addr].insert(INS_NextAddress(ins));
				targets[INS_NextAddress(ins)].insert(addr);
				//fprintf(debugFile,"..which has a fall through \n");
			}
			
		}//end if indirect
      }//end if is call

      if(INS_IsRet(ins)) {
		  branches[addr].insert(ret);
      }//end if is ret
	
}

void BlockBuilder(RTN rtn) {
	INS firstIns = RTN_InsHead(rtn);
	BasicBlock* routineHead = Get_BasicBlock_At(INS_Address(firstIns));;//entrance block
	routineHead->setAsEntryNode();
	routineHead->setStarting(INS_Address(firstIns));
	addressMap[INS_Address(firstIns)] = routineHead;
	BasicBlock* returnBlock = new BasicBlock();//the return block
	returnBlock ->setStarting(2);
	returnBlock ->setEnding(2);
	addressMap[2] = returnBlock;
	INS currentIns = INS_Next(firstIns);
	BasicBlock* currentBlock = routineHead;

	//for the new all block variable
	allBlocks.insert(routineHead);
	allBlocks.insert(returnBlock);

	INS previous = firstIns;//get previous ins
	while (INS_Valid(currentIns)) {

//for loop that will go through all the instruction in the routine
		ADDRINT currentAddress = INS_Address(currentIns);
		//fprintf(debugFile, "\n beginning of  new loops ins: %#lx \n", currentAddress);
	//fprintf(debugFile, "  build on instruction %#x \n", currentAddress);
		if (currentBlock == NULL ) {
	//current block is null, start a new block with next instruction as head
	
	
	//check to see if the block has already been created, and get its pointer. if not, create a new one
			currentBlock = Get_BasicBlock_At(currentAddress);
			currentBlock->setStarting(currentAddress);
	
	//if current instruction, which is now the head of this current Block, is a branch, then we have a single instruction block
			if ( !(branches[currentAddress].empty()) ) {
		
			//This is a call to a helper function BranchIns that handles specificities of branch ins.
				BranchIns(currentIns, currentBlock);
				currentBlock = NULL;
			}
	
	
	
		//else if the current Block is != NULL
		} else {
	//currently building a block already, looking for a branch or a target to end it
	//we could stumble on to a target ins, which means that incidentally we start a new block
	//otherwise a branch instruction indicates a control edge leading away from the block
	
	
	
		if (!(targets[currentAddress].empty())) {//if we find a target, end current block with previous instruction, start new block with current instruction, and allocate an edge between them
		
		
			currentBlock->setEnding(INS_Address(previous));
			BasicBlock * newBlock = Get_BasicBlock_At(currentAddress);
			currentBlock->insertSuccs(newBlock);
			newBlock -> insertPreds(currentBlock);
			newBlock ->setStarting(currentAddress);
			currentBlock = newBlock;
		
		//we need to also check if this target Ins is a Branch Ins as well. Then we have a case similar
		//to the above stated one. A single instruction block that is simply a branch statement to another.
		
			if ( !(branches[currentAddress].empty()) ) {
				//call that helper function again
				BranchIns(currentIns, currentBlock);
				currentBlock = NULL;//block ended, set it to NULL so that next pass will start
							// a new block
			}
		
		//If its not a target, and not a targetAND Branch, but just a branch instruction
		} else if ( !(branches[currentAddress].empty()) ) {
		
			//call that helper function again
			BranchIns(currentIns, currentBlock);
			currentBlock = NULL;//block ended, set it to NULL so that next pass will start a new block
		
		
		//Other wise its just a regular instruction, add it to the map
		} else {
			addressMap[currentAddress] = currentBlock; 
		
		
		}
	}//end else bulding block
	previous = INS_Next(previous);
	currentIns = INS_Next(currentIns);
}//end ins for loop
//cout << "awooga" << endl;

if (currentBlock != NULL) {
	//fprintf(debugFile, "GOING TO SET THE ENDING TO: %#lx", INS_Address(previous));
	currentBlock->setEnding(INS_Address(previous));
}
///set routineGraph variables here!!!
EntranceNode = routineHead;
ReturnNode = returnBlock;
}

void BranchIns(INS currentIns, BasicBlock * currentBlock) {

	ADDRINT currentAddress = INS_Address(currentIns);
	//this is where the current Block ends, at a Branch statement
	currentBlock-> setEnding(currentAddress);
	addressMap[currentAddress] = currentBlock;

	set<ADDRINT>::iterator setIterator;
	setIterator = branches[currentAddress].begin();
	ADDRINT target = (* setIterator);

		if  (target == 1) {//I flagged the calls by giving them the impossible address of
					// 0x1...I assigned them as blocks, so they could take part
					//in cfg graphing and such, but they have no addressing
					//only the name of the called function



			BasicBlock* calledFunction = new BasicBlock();
			calledFunction->setStarting(1);
			calledFunction->setEnding(1);

			//creating the routine call name field for this special block
			ADDRINT callTarget = INS_DirectBranchOrCallTargetAddress(currentIns);
	  		RTN target_rtn = RTN_FindByAddress(callTarget);
	  		if ( RTN_Valid(target_rtn)) {
	  			const char * target_rtn_name = RTN_Name(target_rtn).c_str();
	  			//fprintf(debugFile,"-- direct call to: %s\n", target_rtn_name);
				calledFunction ->setRoutineCalled(target_rtn_name);
			}

			currentBlock ->insertSuccs(calledFunction);
			calledFunction->insertPreds(currentBlock);
				//Right now I am assuming there are always fall through instruction
				//for a function that makes a call
				

					ADDRINT fallThrough = INS_Address(INS_Next(currentIns));
					BasicBlock * othertargetBlock =  Get_BasicBlock_At(fallThrough);
					othertargetBlock->insertPreds(currentBlock);
					calledFunction->insertSuccs(othertargetBlock);
				
//CHANGE THIS TO HAVE
//CALLS AS POSTDOMINATORS

				//so they way I did this was have frontpointers point to the call
				//Blocks which in turn call the fallthrough Block
				//so that call Blocks would be included in the CFG
				//However, backpointers point only to the routines proper
				//Block, the one that made the call, and not to my Call
				//representation
				//This means that my postdominance information will conviniently
				//ignore these Call Blocks when searching from the return Block
				//....hack?


			

		} else if  (target == 2) {//Just like Call Blocks, I flagged calls to return with
					//the address 2

			BasicBlock * returnBlock = addressMap[2];//there is only one return Block
								//in any routine
								//mapping it to adress 2 preserves
								//all the information about
								//all the predecessors to this
								//node
			returnBlock->insertPreds(currentBlock);
			currentBlock ->insertSuccs(returnBlock);
		//fprintf(debugFile, "created edge to return statement \n");
			
		}else if  (target == 4) {//Just like Call Blocks, I flagged calls to Indirect Calls

			BasicBlock * indirectTarget = new BasicBlock();
			indirectTarget->setStarting(4);
			indirectTarget->setEnding(4);
			allBlocks.insert(indirectTarget);
			indirectTarget->insertPreds(currentBlock);
			currentBlock ->insertSuccs(indirectTarget);
		//fprintf(debugFile, "created edge to return statement \n");

			ADDRINT fallThrough = INS_Address(INS_Next(currentIns));
			BasicBlock * othertargetBlock =  Get_BasicBlock_At(fallThrough);
			othertargetBlock->insertPreds(currentBlock);
			indirectTarget->insertSuccs(othertargetBlock);
			
		}else if  (target == 5) {//Indirect Branches as well

			BasicBlock * indirectTarget = new BasicBlock();
			indirectTarget->setStarting(5);
			indirectTarget->setEnding(5);
			allBlocks.insert(indirectTarget);
			indirectTarget->insertPreds(currentBlock);
			currentBlock ->insertSuccs(indirectTarget);
		//fprintf(debugFile, "created edge to return statement \n");
			
		} else {
//Otherwise, the branch instruction is just a that, a regular conditional. In this case, I am 
//iterating over the 2 possible branch targets (one possibly being a fall through).
//do it for the

			//branch was a regular conditional
			BasicBlock * targetBlock =  Get_BasicBlock_At(target);
			targetBlock->insertPreds(currentBlock);
			currentBlock->insertSuccs(targetBlock);
			if (branches[currentAddress].size() > 1) {
				setIterator++;
				target = (* setIterator);
				BasicBlock * othertargetBlock =  Get_BasicBlock_At(target);
				othertargetBlock->insertPreds(currentBlock);
				currentBlock->insertSuccs(othertargetBlock);
			} else if (target == currentBlock->getStarting()) {
				BasicBlock * othertargetBlock =  Get_BasicBlock_At(INS_Address(INS_Next(currentIns)));
				othertargetBlock->insertPreds(currentBlock);
				currentBlock->insertSuccs(othertargetBlock);
			}

		}
}

BasicBlock * Get_BasicBlock_At(ADDRINT targetAddress ) {
	//The function that starts new blocks. We check our map to see if there
//is an exisiting block
//if the entry is null, then we create a new entry for it.
//Using this system, we don't overwrite already created blocks.

	if ( addressMap[targetAddress] == NULL) {
		BasicBlock * newBlock = new BasicBlock();
		addressMap[targetAddress] = newBlock;
		allBlocks.insert(newBlock);
		//fprintf(debugFile, "\n new block at %i", targetAddress);
	} 
		return addressMap[targetAddress];
}

//This is my post dominance builder, 
void buildPostDominanceTree(BasicBlock * currentBlock, queue<BasicBlock *> blockQueue) {
	//fprintf(debugFile, "  going to new block %i \n", currentBlock->getStarting());
	set< void * > edgeTargets = currentBlock->getPredecessors();
	set< void *>::iterator setIterator;
	for (setIterator = edgeTargets.begin(); setIterator != edgeTargets.end(); setIterator++) {
		BasicBlock* targetBlock = (BasicBlock *)(* setIterator);
	//fprintf(debugFile, "  iterating through predecessor of width %#x to %#x \n", targetBlock->getStarting(), targetBlock->getEnding());
		//cout << "\n iterating \n" << endl;
		if (targetBlock -> getPostDominator() == NULL) {
			blockQueue.push(targetBlock);
		//cout << "\n added block to queue \n" << endl;
	//fprintf(debugFile, "  added to queue");
		}
		currentBlock->addPredecessorAsPostDominated(targetBlock);
		
	}
		//cout << "\n done iterating \n" << endl;
	if (blockQueue.size() > 0) {
		BasicBlock * nextBlock = blockQueue.front();
		blockQueue.pop();
		buildPostDominanceTree(nextBlock, blockQueue);
	}
		//cout << "\n completely done \n" << endl;
}

//DEPRECATED -- why do I have a dominance tree? Well I got the two confused. I could either go
//back and reverse all my dominance tree functions,...or just keep it and make new postdominance
//functions....if you need dominance info, heres the method to do this
void buildDominanceTree(BasicBlock * currentBlock, queue<BasicBlock *> blockQueue) {
	//fprintf(debugFile, "  going to new block \n");
	set< void * > edgeTargets = currentBlock->getSuccessors();
	set< void *>::iterator setIterator;
	for (setIterator = edgeTargets.begin(); setIterator != edgeTargets.end(); setIterator++) {
		BasicBlock* targetBlock = (BasicBlock *)(* setIterator);
		if (targetBlock -> getDominator() == NULL) {
			blockQueue.push(targetBlock);
		}
		//fprintf(debugFile, "attempting to add predecessor as dominated \n");
		currentBlock->addSuccessorAsDominated(targetBlock);
		
	}
	if (blockQueue.size() > 0) {
		BasicBlock * nextBlock = blockQueue.front();
		blockQueue.pop();
		buildDominanceTree(nextBlock, blockQueue);
	}

}

};
#endif

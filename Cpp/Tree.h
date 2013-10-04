//>Maker.cpp

#ifndef TREEH
#define TREEH

#include "Array.h"

#undef NOTEENABLE
#define NOTEENABLE(a)

//Node is a struct and needs fields
//  long depth,deep,numChildren,nodesInSubtree,parentIndex,childInParent
//  Node *nextNode,*rootNode

//CNode is a class and has the following static members
//  static char readNode(char *&prog,Node &node)  reads from buffer "prog" (incrementing as neccesary) the next node or end of function marker
//      returns  't' for terminals,  'f' for start of function,  ')' for end of function, and 0 for error
//
//  static void printNode(Node &node, String &str)   prints the node to str
//  static void printSeperator(Node &node, String &str)  prints a seperator (typically space)
//  static void printEndFunction(Node &node, String &str)  prints the end of a function (typically ')' )
//  static void random(Node &node,long minDeep,long maxDeep)   create a random node

#define ForEachChildNode(child,node) \
  for (long child##Index=((child=(&(node))+1)!=NULL?0:0),child##NumSiblings=(node).numChildren; \
       child##Index<child##NumSiblings?true:((child=NULL)!=NULL?false:false); \
       child##Index++,child+=child->nodesInSubtree)

#define ForEachChildNodeIndex(child,node,tree) \
  for (long child##Index=((child=(&(node))+1)!=NULL?0:0),child##NumSiblings=(node).numChildren,child##NodeIndex=(tree).getNodeIndex(node)+1; \
       child##Index<child##NumSiblings?true:((child=NULL)!=NULL?false:false); \
       child##Index++,child##NodeIndex+=child->nodesInSubtree,child+=child->nodesInSubtree)

#define ForEachChildNodeIndex2(child,childNodeIndex,node,tree) \
  for (long child##Index=((child=(&(node))+1)?(childNodeIndex=(tree).getNodeIndex(node)+1?0:0):0), \
  child##NumSiblings=(node).numChildren, \
  child##Index<child##NumSiblings?true:(child=NULL?false:false); \
  child##Index++,childNodeIndex+=child->nodesInSubtree,child+=child->nodesInSubtree)

#define ForEachSubtreeNode(child,node) \
  for (long child##ToGo=(node).nodesInSubtree,        \
         child##NodeIndex=((child=(&(node)))?long((&(node))-(node).rootNode):0); \
       child##ToGo>0?true:((child=NULL)==NULL?false:false);                \
       child##ToGo--,child##NodeIndex++,child++)

#define ForEachChosenSubtreeNode(child,node) \
  for (long child##ToGo=(node).nodesInSubtree,child##ExploreSubtree=1,child##NodesSkipped=0,        \
         child##NodeIndex=((child=(&(node)))?long((&(node))-(node).rootNode):0); \
       child##ToGo>0?true:((child=NULL)==NULL?false:false);                \
       child##NodesSkipped=(child##ExploreSubtree?1:child->nodesInSubtree), \
       child##ToGo-=child##NodesSkipped, \
        child##NodeIndex+=child##NodesSkipped, \
        child+=child##NodesSkipped, \
        child##ExploreSubtree=1)

#define ForEachSubtreeNodeRevFromLeaves(child,node) \
  for (long child##ToGo=(node).nodesInSubtree,        \
         child##NodeIndex=long((&(node))-(node).rootNode)+((child=(&(node))+(child##ToGo-1))?child##ToGo-1:0); \
       child##ToGo>0?true:((child=NULL)==NULL?false:false);                \
       child##ToGo--,child##NodeIndex--,child--)

// (for backward compatability (use above macro))
#define ForEachNodeInSubtree(child,node,tree)             \
  for (long child##ToGo=((child=&(node))!=NULL?1:1),   \
         child##NodeIndex=tree.getNodeIndex(node);                \
       child##ToGo>0?true:((child=NULL)==NULL?false:false);                \
       child##ToGo+=child->numChildren-1,child##NodeIndex++,child++)


#define ForEachNodeInTree(child,thetree) \
  for (long child##ToGo=((thetree).tree.num()==0?0:((child=(thetree).tree.a)!=NULL?1:1)), \
  child##NodeIndex=0; \
  child##ToGo>0?true:((child=NULL)==NULL?false:false); \
  child##ToGo+=child->numChildren-1,child##NodeIndex++,child++)

#define ForEachChosenNodeInSubtree(child,node,tree) \
  for (long child##ToGo=((child=&(node))!=NULL?1:1), \
  child##NodeIndex=(tree).getNodeIndex(node), \
  child##ExploreSubtree=1; \
  child##ToGo>0?true:((child=NULL)==NULL?false:false); \
  child##ToGo+=(child##ExploreSubtree?child->numChildren-1:-1), \
  child##NodeIndex+=(child##ExploreSubtree?1:child->nodesInSubtree), \
  child+=(child##ExploreSubtree?1:child->nodesInSubtree),child##ExploreSubtree=1)

#define ForEachChosenNodeInTree(child,thetree) ForEachChosenNodeInSubtree(child,(thetree).tree.a[0],thetree)

#define WriteTree(str,type,tre,stri) WriteArray(str,type,(tre).tree,stri)
#define ReadTree(str,type,tre,stri) do {ReadArray(str,type,(tre).tree,stri);(tre).updateNodes();} while(0)






















template<class Node,class CNode> class Tree : public RefCnt {
  PubDefCC("Tr");
 public:
  CNode &ctxt;

  Array<Node> tree;

  Tree(CNode &actxt) : ctxt(actxt), tree() {}

  static Tree<Node,CNode> *construct(ContextIndex &ci,Tree<Node,CNode> *nul,WRD typcc) {
    return(NEWCONStemplate((Tree<Node,CNode>*),(CNode::contexts[ci.i]),Tree<Node,CNode>));
  }
  virtual void destroy() {_DELETE(Tree,this);}

  Node *iterateDFS(long &nodeIndex,long &depth) {
    if (!tree.num()) {nodeIndex=-1;depth=-1;return(NULL);}
    if ((nodeIndex<0)||(nodeIndex>=tree.num())) ERROR3("iterateDFS: out of bounds, node %d, from [0,%d)",nodeIndex,tree.num());
    Node &node=tree.a[nodeIndex];
    nodeIndex++;
    if (node.numChildren) {
      depth++;
      return(tree.a+nodeIndex);
    }
    else {
      Node *parent=&node;
      long parentIndex;
      while ((parentIndex=parent->parentIndex)>-1) {
        parent=tree.a+parentIndex;
        if (nodeIndex-parentIndex<parent->nodesInSubtree) return((depth<0)?NULL:tree.a+nodeIndex);
        depth--;
      }
      nodeIndex=-1;
      depth=-1;
      return(NULL);
    }
  }

  Node *iterateSiblings(long &nodeIndex) {
    //if (!tree.num()) {nodeIndex=-1;return(NULL);}
    if ((nodeIndex<0)||(nodeIndex>=tree.num())) ERROR3("iterateSiblings: out of bounds, node %d, from [0,%d)",nodeIndex,tree.num());
    Node &node=tree.a[nodeIndex];
    nodeIndex+=node.nodesInSubtree;
    if ((node.parentIndex==-1)||(nodeIndex-node.parentIndex==tree.a[node.parentIndex].nodesInSubtree)) {
      nodeIndex=-1;
      return(NULL);
    }
    return(tree.a+nodeIndex);
  }

  Node *iterateParents(long &nodeIndex) {
    //if (!tree.num()) {nodeIndex=-1;return(NULL);}
    if ((nodeIndex<0)||(nodeIndex>=tree.num())) ERROR3("iterateParents: out of bounds, node %d, from [0,%d)",nodeIndex,tree.num());
    Node &node=tree.a[nodeIndex];
    if (node->parentIndex==-1) {
      nodeIndex=-1;
      return(NULL);
    }
    else {
      nodeIndex=node->parentIndex;
      return(tree.a+nodeIndex);
    }
  }

  Node *iterateChildren(long &nodeIndex) {
    //if (!tree.num()) {nodeIndex=-1;return(NULL);}
    if ((nodeIndex<0)||(nodeIndex>=tree.num())) ERROR3("iterateChildren: out of bounds, node %d, from [0,%d)",nodeIndex,tree.num());
    Node &node=tree.a[nodeIndex];
    if (!node.numChildren) {
      nodeIndex=-1;
      return(NULL);
    }
    else {
      nodeIndex++;
      return(tree.a+nodeIndex);
    }
  }

  bool fromLisp(String &str) {const char *pc=str.a;return(fromLisp(pc));}
  bool fromLisp(const char *&prog) {
    tree.clear();
    long nodeIndex=0;
    if (!fromLisp(prog,nodeIndex)) {
      tree.setUpperBound(-1,true);
      return(false);
    }
    else {
      tree.setUpperBound(nodeIndex-1,true);
      updateNodes();
      return(true);
    }
  }

 protected:
  
  // void **check;
  //#define CHECK(at) if (check) fprintf(efil,"check " at " %p\n",*check);

  char fromLisp(const char *&prog,long &nodeIndex) {
    long indexWas=nodeIndex,numChildren;
    Node *node=&tree.get(nodeIndex);
    char type,ctype;
    switch (type=ctxt.readNode(prog,*node)) {
      case 'f':
        nodeIndex++;
        numChildren=0;
        while(true) {
          //long childIndexWas=nodeIndex;
          if (!(ctype=fromLisp(prog,nodeIndex))) return(0);
          else if (ctype==')') break;
          numChildren++;
//          ctxt.haveReadChild(tree[indexWas],tree[childIndexWas]);
        }
        node=&tree[indexWas];// may have changed
        if (node->numChildren&&(node->numChildren!=numChildren)) WARN3("lisp function has wrong number of children (%d!=%d)",node->numChildren,numChildren);
        node->numChildren=numChildren;
        break;
      case 't':nodeIndex++;break;
    }
//    ctxt.haveReadNode(tree[indexWas]);
    return(type);
  }

 public:

  char *toLisp(String &str) {
    long nodeIndex=0;
    str="";
    return(toLisp(str,nodeIndex));
  }

  char *toLisp(String &str,long &nodeIndex) {
    if (tree.num()==0) {str="x";return(str.a);}
    if ((nodeIndex<0)||(nodeIndex>=tree.num())) ERROR3("toLisp: out of bounds, node %d, from [0,%d)",nodeIndex,tree.num());
    Node &node=tree.a[nodeIndex++];
    ctxt.printNode(node,str);
    if (node.numChildren) {
      for (long i=0;i<node.numChildren;i++) {
        ctxt.printSeperator(node,str);
        toLisp(str,nodeIndex);
      }
      ctxt.printEndFunction(node,str);
    }
    return(str.a);
  }

  long getNodeIndex(Node &node) {
    if (!tree.containsPtr(&node)) return(-1);
    else return(long((&node)-tree.a));
  }

  void updateNodes() {// fixes : deep, depth, nodesInSubtree, parentIndex,   given correct numChildren
    long nodeIndex=0;
    updateNodes(nodeIndex);
    tree.setUpperBound(nodeIndex-1,true);
    //Node *node;
    //    ForEachInArray(node,tree) {
    //      fprintf(efil,"node %d  depth %d  deep %d  children %d  subtree %d  parent %d\n",nodeIndex,node->depth,node->deep,node->numChildren,node->nodesInSubtree,node->parentIndex);
    //    }
  }

 protected:

  long updateNodes(long &nodeIndex,long depth=0,long parentIndex=-1,long childInParent=0) {// fixes : deep, depth, nodesInSubtree, parentIndex,   given correct numChildren
    if (tree.num()==0) return(0);
    if ((nodeIndex<0)||(nodeIndex>=tree.num())) ERROR3("updateNodes: out of bounds, node %d, from [0,%d)",nodeIndex,tree.num());
    long indexWas=nodeIndex;
    Node &node=tree.a[nodeIndex++];
    tree._ensurePtr(&node,sizeof(Node));
    long deep=ctxt.deep(node),cdeep,cdepth;
    for (long child=0;child<node.numChildren;child++) {
      cdepth=ctxt.childDepth(node,child);
      if ((cdeep=updateNodes(nodeIndex,depth+cdepth,indexWas,child))+1>deep) deep=cdeep+cdepth;
    }
    node.nodesInSubtree=nodeIndex-indexWas;
    node.parentIndex=parentIndex;
    node.depth=depth;
    node.rootNode=tree.a;
    node.childInParent=childInParent;
    return(node.deep=deep);
  }

 public:

  Node *linkNodesOfDepth(long minDepth,long maxDepth,long minDeep,long maxDeep,long *pnumLinked) {// assume nodes have been updated
    Node *lastNode=NULL,*firstNode=NULL,*node=tree.a;
    long depth=0,nodeIndex=0;
    if (pnumLinked) *pnumLinked=0;
    do {
      if ((node->depth>=minDepth)&&(node->depth<=maxDepth)&&(node->deep>=minDeep)&&(node->deep<=maxDeep)) {
        if (lastNode) lastNode->nextNode=node;else firstNode=node;
        node->nextNode=NULL;
        lastNode=node;
        if (pnumLinked) (*pnumLinked)++;
      }
    } while ((node=iterateDFS(nodeIndex,depth)));
    return(firstNode);
  }

  void transplant(long toIndex,Node &fromNode) {
    tree.unlockUpperBound();
    if (tree.num()==0) {
      if (toIndex) ERROR3("transplant: out of bounds, node %d, from [0,%d)",toIndex,tree.num());
      tree.insert(0,fromNode,fromNode.nodesInSubtree);
    }
    else {
      if ((toIndex<0)||(toIndex>=tree.num())) ERROR3("transplant: out of bounds, node %d, from [0,%d)",toIndex,tree.num());
      Node *root=tree.a;
      if (tree.containsPtr(&fromNode)) {
        Array<Node> tempTree;
        tempTree.insert(0,fromNode,fromNode.nodesInSubtree);
        tree.remove(toIndex,tree.a[toIndex].nodesInSubtree);
        tree.insert(toIndex,*tempTree.a,tempTree.a[0].nodesInSubtree);
      }
      else {
        tree.remove(toIndex,tree.a[toIndex].nodesInSubtree);
        tree.insert(toIndex,fromNode,fromNode.nodesInSubtree);
      }
    }
    tree.lockUpperBound();
    updateNodes();
  }

  long addNode(long parNodeIndex,Node &fromNode,long childIndex=-1) {
    //    prnt("add child %d %d [0,%d]\n",parNodeIndex,childIndex,tree.upperBound());
    long toIndex=0;
    if (parNodeIndex<0) {
      tree.clear();
      tree.get(0)=fromNode;
    }
    else {
      tree.getExisting(parNodeIndex);
      if (childIndex<0) childIndex=tree.a[parNodeIndex].numChildren;
      Node *node;
      long togo;
      if (childIndex>tree.a[parNodeIndex].numChildren) ERROR("addChild, childIndex outside bounds");
      for (togo=tree.a[parNodeIndex].nodesInSubtree-1,toIndex=parNodeIndex+1,node=tree.a+toIndex;
           childIndex&&(togo>0);
           childIndex--,togo-=node->nodesInSubtree,toIndex+=node->nodesInSubtree,node+=node->nodesInSubtree);
      if (childIndex) ERROR("addChild, has tree been updated?");
      tree.unlockBounds();
      if (tree.containsPtr(&fromNode)) {
        Array<Node> tempTree;
        Node tmpNode=fromNode;
        tree.insert(toIndex,tmpNode,1);
      }
      else tree.insert(toIndex,fromNode,1);
      tree.lockBounds();
      tree[parNodeIndex].numChildren++;
    }
    tree[toIndex].numChildren=0;
    updateNodes();
    return(toIndex);
  }

  long addSubtree(long parNodeIndex,Node &fromNode,long childIndex=-1) {
    long toIndex=0;
    if (parNodeIndex<0) {
      tree.clear();
      Node *node;
      long togo;
      for (togo=1,node=&fromNode,fromNode.nodesInSubtree=0;togo;togo+=node->numChildren-1,fromNode.nodesInSubtree++,node++);
      tree.insert(0,fromNode,fromNode.nodesInSubtree);
    }
    else {
      tree.getExisting(parNodeIndex);
      if (childIndex<0) childIndex=tree.a[parNodeIndex].numChildren;
      Node *node;
      long togo;
      for (togo=1,node=&fromNode,fromNode.nodesInSubtree=0;togo;togo+=node->numChildren-1,fromNode.nodesInSubtree++,node++);
      if (childIndex>tree.a[parNodeIndex].numChildren) ERROR("addChild, childIndex outside bounds");
      for (togo=tree.a[parNodeIndex].nodesInSubtree-1,toIndex=parNodeIndex+1,node=tree.a+toIndex;
           childIndex&&(togo>0);
           childIndex--,togo-=node->nodesInSubtree,toIndex+=node->nodesInSubtree,node+=node->nodesInSubtree);
      if (childIndex) ERROR("addChild, has tree been updated?");
      tree.unlockBounds();
      if (tree.containsPtr(&fromNode)) {
        Array<Node> tempTree;
        tempTree.insert(0,fromNode,fromNode.nodesInSubtree);
        tree.insert(toIndex,*tempTree.a,tempTree.a[0].nodesInSubtree);
      }
      else tree.insert(toIndex,fromNode,fromNode.nodesInSubtree);
      tree.lockBounds();
      tree[parNodeIndex].numChildren++;
    }
    updateNodes();
    return(toIndex);
  }

  void removeSubtree(long parNodeIndex,long childIndex) {
    if (parNodeIndex<0) tree.clear();
    else {
      tree.getExisting(parNodeIndex);
      if (childIndex<0) childIndex=tree.a[parNodeIndex].numChildren-1;
      Node *node;
      long togo,toIndex;
      if ((childIndex<0)||(childIndex>=tree.a[parNodeIndex].numChildren)) ERROR("removeChild, childIndex outside bounds");
      for (togo=tree.a[parNodeIndex].nodesInSubtree-1,toIndex=parNodeIndex+1,node=tree.a+toIndex;
           childIndex&&(togo>0);
           childIndex--,togo-=node->nodesInSubtree,toIndex+=node->nodesInSubtree,node+=node->nodesInSubtree);
      if (childIndex) ERROR("removeChild, has tree been updated?");
      tree.unlockBounds();
      tree.remove(toIndex,node->nodesInSubtree);
      tree.lockBounds();
      tree[parNodeIndex].numChildren--;
      updateNodes();
    }
  }

  void removeSubtree(long index) {
    Node &node=tree.getExisting(index);
    if (node.parentIndex<0) tree.clear();
    else {
      tree.getExisting(node.parentIndex).numChildren--;
      tree.unlockBounds();
      tree.remove(index,node.nodesInSubtree);
      tree.lockBounds();
      updateNodes();
    }
  }


  Node &operator[](long index) {return(tree.getExisting(index));}
  Node &operator+=(Node &subtree) {return(tree.getExisting(addSubtree(subtree.parentIndex,subtree)));}
  Tree &operator-=(Node &subtree) {removeSubtree(getNodeIndex(subtree));return(*this);}
  Tree &operator-=(long index) {removeSubtree(index);return(*this);}
  Tree &operator=(Node &copy) {addSubtree(-1,copy);return(*this);}
  Tree &operator=(Tree &copy) {tree=copy.tree;updateNodes();return(*this);}

  void random(long minDeep,long maxDeep) {
    tree.setUpperBound(-1);
    long nodeIndex;
    random(nodeIndex=0,minDeep,maxDeep);
    updateNodes();
  }

  void randomByNumNodes(long minNodes,long maxNodes) {
    tree.setUpperBound(-1);
    long nodeIndex;
    randomByNumNodes(nodeIndex=0,minNodes,maxNodes);
    updateNodes();
  }

protected:

  void random(long &nodeIndex,long minDeep,long maxDeep,long depth=0) {
    FNNOTE6("tree::random %d %d %d %d allc:%d",nodeIndex,minDeep,maxDeep,depth,long(g_alloced>>20));
    long myNodeIndex=nodeIndex;
    tree.unlockUpperBound();
    Node &node=tree.get(nodeIndex++);
    tree.lockUpperBound();
    node.depth=depth;
    tree._ensurePtr(&node,sizeof(Node));
    ctxt.random(node,minDeep,maxDeep);
    long numChildren=node.numChildren;// node may change pointers in the following loop
    for (long child=0;child<numChildren;child++) {
      NOTE3(3,"  child %d",child);
      random(nodeIndex,minDeep,maxDeep,depth+ctxt.childDepth(tree.a[myNodeIndex],child));
    }
  }

  void randomByNumNodes(long &nodeIndex,long minNodes,long maxNodes,long depth=0) {
    FNNOTE6("tree::randomByNumNodes %d %d %d %d allc:%d",nodeIndex,minNodes,maxNodes,depth,long(g_alloced>>20));
    long myNodeIndex=nodeIndex;
    Node &node=tree[nodeIndex++];
    node.depth=depth;
    tree._ensurePtr(&node,sizeof(Node));
    ctxt.randomByNumNodes(node,minNodes,maxNodes);
    long numChildren=node.numChildren;// node may change pointers in the following loop
    long minNodesLeft=minNodes-ctxt.numNodes(node),maxNodesLeft=maxNodes-ctxt.numNodes(node);
    for (long child=0;child<numChildren;child++) {
      NOTE3(3,"  child %d",child);
      long childMinNodes=minNodesLeft/(numChildren-child);
      long childMaxNodes=maxNodesLeft/(numChildren-child);
      minNodesLeft-=childMinNodes;
      maxNodesLeft-=childMaxNodes;
      randomByNumNodes(nodeIndex,childMinNodes,childMaxNodes,depth+ctxt.childDepth(tree.a[myNodeIndex],child));
    }
  }

public:

  void printFull() {
    Node *node;
    String str;
    prnt("tree (%d):\n",tree.num());
    ForEachNodeInTree(node,*this) {
      for (long i=0;i<node->depth;i++) prnt("  ");
      str="";
      ctxt.printNode(*node,str);
      prnt("[%d] depth:%d deep:%d numChidren:%d nodesInSubtree:%d  :  %s\n",nodeNodeIndex,node->depth,node->deep,node->numChildren,node->nodesInSubtree,str.a);
    }
    prnt("\n");
  }

};























/*
template<class Node,class CNode> class Treeo {
 public:

  Array<Node> tree;

  Treeo() : tree() {}

  Node *iterateDFS(long &nodeIndex,long &depth) {
    if (!tree.num()) {nodeIndex=-1;depth=-1;return(NULL);}
    if ((nodeIndex<0)||(nodeIndex>=tree.num())) ERROR3("iterateDFS: out of bounds, node %d, from [0,%d)",nodeIndex,tree.num());
    Node &node=tree.a[nodeIndex];
    nodeIndex++;
    if (node.numChildren) {
      depth++;
      return(tree.a+nodeIndex);
    }
    else {
      Node *parent=&node;
      long parentIndex;
      while ((parentIndex=parent->parentIndex)>-1) {
        parent=tree.a+parentIndex;
        if (nodeIndex-parentIndex<parent->nodesInSubtree) return((depth<0)?NULL:tree.a+nodeIndex);
        depth--;
      }
      nodeIndex=-1;
      depth=-1;
      return(NULL);
    }
  }

  Node *iterateSiblings(long &nodeIndex) {
    //if (!tree.num()) {nodeIndex=-1;return(NULL);}
    if ((nodeIndex<0)||(nodeIndex>=tree.num())) ERROR3("iterateSiblings: out of bounds, node %d, from [0,%d)",nodeIndex,tree.num());
    Node &node=tree.a[nodeIndex];
    nodeIndex+=node.nodesInSubtree;
    if ((node.parentIndex==-1)||(nodeIndex-node.parentIndex==tree.a[node.parentIndex].nodesInSubtree)) {
      nodeIndex=-1;
      return(NULL);
    }
    return(tree.a+nodeIndex);
  }

  Node *iterateParents(long &nodeIndex) {
    //if (!tree.num()) {nodeIndex=-1;return(NULL);}
    if ((nodeIndex<0)||(nodeIndex>=tree.num())) ERROR3("iterateParents: out of bounds, node %d, from [0,%d)",nodeIndex,tree.num());
    Node &node=tree.a[nodeIndex];
    if (node->parentIndex==-1) {
      nodeIndex=-1;
      return(NULL);
    }
    else {
      nodeIndex=node->parentIndex;
      return(tree.a+nodeIndex);
    }
  }

  Node *iterateChildren(long &nodeIndex) {
    //if (!tree.num()) {nodeIndex=-1;return(NULL);}
    if ((nodeIndex<0)||(nodeIndex>=tree.num())) ERROR3("iterateChildren: out of bounds, node %d, from [0,%d)",nodeIndex,tree.num());
    Node &node=tree.a[nodeIndex];
    if (!node.numChildren) {
      nodeIndex=-1;
      return(NULL);
    }
    else {
      nodeIndex++;
      return(tree.a+nodeIndex);
    }
  }

  bool fromLisp(char *&prog) {
    tree.setUpperBound(-1);
    long nodeIndex=0;
    if (!fromLisp(prog,nodeIndex)) {
      tree.setUpperBound(-1);
      return(false);
    }
    else {
      updateNodes();
      return(true);
    }
  }

 protected:
  
  // void **check;
  //#define CHECK(at) if (check) fprintf(efil,"check " at " %p\n",*check);

  char fromLisp(char *&prog,long &nodeIndex) {
    long indexWas=nodeIndex;
    Node *node=&tree[nodeIndex++];
    char type=CNode::readNode(prog,*node);
    if (!type) return(0);
    else if (type=='f') {
      long numChildren=0;
      while(1) {
        char ctype=fromLisp(prog,nodeIndex);
        if (!ctype) return(0);
        else if (ctype==')') {tree.setUpperBound(nodeIndex-2);nodeIndex--;break;}
        numChildren++;
      }
      node=&tree[indexWas];// may have changed
      //      fprintf(efil,"Node type:%p children:%d vs %d  upperbound:%d\n",node->type,numChildren,node->numChildren,tree.upperBound());
      if (node->numChildren&&(node->numChildren!=numChildren)) WARN3("lisp function has wrong number of children (%d!=%d)",node->numChildren,numChildren);
      node->numChildren=numChildren;        
    }
    return(type);
  }

 public:

  char *toLisp(String &str) {
    long nodeIndex=0;
    str="";
    return(toLisp(str,nodeIndex));
  }

  char *toLisp(String &str,long &nodeIndex) {
    if (tree.num()==0) {str="x";return(str.a);}
    if ((nodeIndex<0)||(nodeIndex>=tree.num())) ERROR3("toLisp: out of bounds, node %d, from [0,%d)",nodeIndex,tree.num());
    Node &node=tree.a[nodeIndex++];
    CNode::printNode(node,str);
    if (node.numChildren) {
      for (long i=0;i<node.numChildren;i++) {
        CNode::printSeperator(node,str);
        toLisp(str,nodeIndex);
      }
      CNode::printEndFunction(node,str);
    }
    return(str.a);
  }

  long getNodeIndex(Node &node) {
    if (!tree.containsPtr(&node)) return(-1);
    else return(long((&node)-tree.a));
  }

  long getNodeDepth(Node &node) {
    if (!tree.containsPtr(&node)) return(-1);
    else {
      long i=0,d=0;
      Node *nd;
      for (nd=tree.a;nd&&(nd!=&node);nd=iterateDFS(i,d));
      return(nd?d:-1);
    }
  }

  void updateNodes() {// fixes : deep, depth, nodesInSubtree, parentIndex,   given correct numChildren
    long nodeIndex=0;
    updateNodes(nodeIndex);
    tree.setUpperBound(nodeIndex-1);
    //Node *node;
    //    ForEachInArray(node,tree) {
    //      fprintf(efil,"node %d  depth %d  deep %d  children %d  subtree %d  parent %d\n",nodeIndex,node->depth,node->deep,node->numChildren,node->nodesInSubtree,node->parentIndex);
    //    }
  }

 protected:

  long updateNodes(long &nodeIndex,long depth=0,long parentIndex=-1,long childInParent=0) {// fixes : deep, depth, nodesInSubtree, parentIndex, rootNode  given correct numChildren
    if (tree.num()==0) return(0);
    if ((nodeIndex<0)||(nodeIndex>=tree.num())) ERROR3("updateNodes: out of bounds, node %d, from [0,%d)",nodeIndex,tree.num());
    long indexWas=nodeIndex;
    Node &node=tree.a[nodeIndex++];
    tree._ensurePtr(&node,sizeof(Node));
    long deep=1,cdeep;
    for (long child=0;child<node.numChildren;child++) if ((cdeep=updateNodes(nodeIndex,depth+1,indexWas,child))+1>deep) deep=cdeep+1;
    node.nodesInSubtree=nodeIndex-indexWas;
    node.parentIndex=parentIndex;
    node.depth=depth;
    node.rootNode=tree.a;
    node.childInParent=childInParent;
    return(node.deep=deep);
  }

 public:

  Node *linkNodesOfDepth(long minDepth,long maxDepth,long minDeep,long maxDeep,long *pnumLinked) {// assume nodes have been updated
    Node *lastNode=NULL,*firstNode=NULL,*node=tree.a;
    long depth=0,nodeIndex=0;
    if (pnumLinked) *pnumLinked=0;
    do {
      if ((node->depth>=minDepth)&&(node->depth<=maxDepth)&&(node->deep>=minDeep)&&(node->deep<=maxDeep)) {
        if (lastNode) lastNode->nextNode=node;else firstNode=node;
        node->nextNode=NULL;
        lastNode=node;
        if (pnumLinked) (*pnumLinked)++;
      }
    } while (node=iterateDFS(nodeIndex,depth));
    return(firstNode);
  }

  void transplant(long toIndex,Node &fromNode) {
    if (tree.num()==0) {
      if (toIndex) ERROR3("transplant: out of bounds, node %d, from [0,%d)",toIndex,tree.num());
      tree.insert(0,fromNode,fromNode.nodesInSubtree);
    }
    else {
      if ((toIndex<0)||(toIndex>=tree.num())) ERROR3("transplant: out of bounds, node %d, from [0,%d)",toIndex,tree.num());
      Node *root=tree.a;
      if (tree.containsPtr(&fromNode)) {
        Array<Node> tempTree;
        tempTree.insert(0,fromNode,fromNode.nodesInSubtree);
        tree.remove(toIndex,tree.a[toIndex].nodesInSubtree);
        tree.insert(toIndex,*tempTree.a,tempTree.a[0].nodesInSubtree);
      }
      else {
        tree.remove(toIndex,tree.a[toIndex].nodesInSubtree);
        tree.insert(toIndex,fromNode,fromNode.nodesInSubtree);
      }
    }
    updateNodes();
  }

    
  void operator=(Node &copy) {transplant(0,copy);}

  void random(long minDeep,long maxDeep) {
    tree.setUpperBound(-1);
    long nodeIndex;
    random(nodeIndex=0,minDeep,maxDeep);
    updateNodes();
  }

protected:

  void random(long &nodeIndex,long minDeep,long maxDeep,long depth=0) {
    FNNOTE6("tree::random %d %d %d %d allc:%d",nodeIndex,minDeep,maxDeep,depth,long(g_alloced>>20));
    Node &node=tree[nodeIndex++];
    node.depth=depth;
    tree._ensurePtr(&node,sizeof(Node));
    CNode::random(node,minDeep,maxDeep);
    long numChildren=node.numChildren;// node may change pointers in the following loop
    for (long child=0;child<numChildren;child++) {
      NOTE3(3,"  child %d",child);
      random(nodeIndex,minDeep,maxDeep,depth+1);
    }
  }

public:
};

*/



#endif

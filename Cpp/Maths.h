//>Maker.cpp

#ifndef MATHSH
#define MATHSH

#include "Array.h"
#include "Constants.h"





// didn't know where to put these
  bool existsAOneToOneMapping(Array<ULL> &mappablesForEachMappedItem);



  bool existsAOneToOneMapping(ULL okMask,ULL usedMask,ULL *mappablesForEachMappedItem,long numToMap,long i=0);
#ifdef MAINCPPFILE
  bool existsAOneToOneMapping(ULL okMask,ULL usedMask,ULL *mappablesForEachMappedItem,long numToMap,long i) {
    if (i==numToMap) return(true); // i.e. we have found a mapping from items to mappables such that each item maps to a different mappable
    ULL availMask;
    if (shiftOneULL[i]&okMask) return(existsAOneToOneMapping(okMask,usedMask,mappablesForEachMappedItem,numToMap,i+1));
    else if (availMask=(mappablesForEachMappedItem[i]&~usedMask)) {
      for (ULL s=1;s<=availMask;s<<=1) if ((s&availMask)&&existsAOneToOneMapping(okMask,usedMask|s,mappablesForEachMappedItem,numToMap,i+1)) return(true);
    }
    return(false);
  }

  bool existsAOneToOneMapping(Array<ULL> &mappablesForEachMappedItem) {
    ULL okMask=0,contentionMask=0;

    // next, for a little speed, remove those items's which can map to only one item
    IterateArray(ULL,mappable,mappablesForEachMappedItem,
      if (numOnes64(mappable)==1) {
        if (mappable&contentionMask) return(false);// i.e. there are two fps, both of which match only one fo child, and it's the same one
        else {okMask|=shiftOneULL[mappableIndex];contentionMask|=mappable;}
      }
    );
    return(existsAOneToOneMapping(okMask,contentionMask,mappablesForEachMappedItem.a,mappablesForEachMappedItem.num()));
  }
#endif













template<class I> class MaximalArray : public Array<I*> {
public:
  long usenum;

  virtual bool upperValid() {
#ifdef GENDEBUG
    if (usenum>=1000) SERROR("overflow in maximal");
#endif
    return(true);
  }
  virtual bool adding(I&i) {return(true);}
  virtual void removing(I&i) {}
  virtual Array<I*> *getMoreGeneral(I&i) {return(NULL);}

  Array<I*> *options;
  virtual Array<I*> &first(Array<I*> &aoptions) {
    options=&aoptions;
    IterateArray(I*,o,options,   o->tmpMaximalsIndex=oIndex;);
    this.setUpperBound(1000);
    greedy(0,0);
    return(this);
  }

  virtual Array<I*> *next() {
    for (long i=usenum-1;i>=0;i--) {
      while (greedy(i,this->a[i]->tmpMaximalsIndex+1)) {
        if (isMaximal(this->a[i]->tmpMaximalsIndex)) {
          this->setUpperBound(usenum-1);
          return(&this);
        }
      }
    }
    return(NULL);
  }


  bool greedy(long fromi,long fromMaxInd) {
    long nmi=options->num(),mi;
    usenum=fromi;
    for (mi=fromMaxInd;mi<nmi;mi++) {
      I &o=*options->a[mi];
      if (adding(o)) {
        this->a[usenum++]=&o;
        if (this->valid()) mi--;
        else {
          removing(o);
          usenum--;
        }
      }
    }
    return(usenum>fromi);
  }

  bool isMaximal(long ignoreFromMaxInd) {
    long foundAt,insertAt;
    long nmi=(ignoreFromMaxInd==-1?options.num():ignoreFromMaxInd);
    bool specializable;

    for (long mi=0;mi<nmi;mi++) {
      I &o=*options->a[mi];
      bool foundGen=false;
      Array<I*> *moregens=getMoreGeneral(o);
      if (moregens) IterateArray(I*,gen,moregens,
        FindArrayElement(long,gen->tmpMaximalsIndex,this->a[index]->tmpMaximalsIndex,0,usenum-1,foundAt,insertAt,false);
        if (foundAt!=-1) {
          foundGen=true;

          removing(*gen);
          if (foundAt<usenum-1) this->move(foundAt,foundAt+1,usenum-1-foundAt);
          long foundGenAt=foundAt;
          usenum--;

          specializable=false;

          if (adding(o)) {
            FindArrayElement(long,mi,this->a[index]->tmpMaximalsIndex,0,usenum-1,foundAt,insertAt,false);
            if (insertAt<usenum) this->move(insertAt+1,insertAt,usenum-insertAt);
            this->a[insertAt]=&o;
            usenum++;
            specializable=this->valid();
            removing(o);
            usenum--;
            if (insertAt<usenum) this->move(insertAt,insertAt+1,usenum-insertAt);
          }

          if (!adding(*gen)) SERROR("Could not replace gen");
          usenum++;
          if (foundGenAt<usenum-1) this->move(foundGenAt,foundGenAt+1,usenum-1-foundGenAt);
          this->a[foundGenAt]=gen;

          if (specializable) return(false);
        }
      );
      if (!foundGen) {
        specializable=false;

        if (adding(o)) {
          FindArrayElement(long,mi,this->a[index]->tmpMaximalsIndex,0,usenum-1,foundAt,insertAt,false);
          if (insertAt<usenum) this->move(insertAt+1,insertAt,usenum-insertAt);
          this->a[insertAt]=&o;
          usenum++;
          specializable=this->valid();
          removing(o);
          usenum--;
          if (insertAt<usenum) this->move(insertAt,insertAt+1,usenum-insertAt);
        }

        if (specializable) return(false);
      }
    }
    return(true);
  }
};









class Graph {
public:
  static bool debug,fullDebug;

  static void test() {
    debug=true;
    while(true) {
      prnt("Enter edges of graph: (eg a node might be:* ** *)\n");
      Array<ULL> edges;
      ULL ull;
      char buf[64],*pc;
      long i,j;
      do {
        ull=0;
        fgets(buf,63,stdin);
        for (i=0,pc=buf;*pc&&(*pc!=0xd)&&(*pc!=0xa);i++,pc++) if (*pc!=' ') ull|=shiftOneULL[i];
        if (ull) edges+=ull;
      } while (ull);
      ForEachIndexInArray(i,edges) ForEachIndexInArray(j,edges) if ((i<j)&&(edges[i]&shiftOneULL[j])) edges[j]|=shiftOneULL[i];
      prnt("Invert (find maximal cliques)?(y/n)");
      if (*fgets(buf,63,stdin)=='y') {
        ForEachIndexInArray(i,edges) edges[i]=(~edges[i])&(shiftOneULL[edges.num()]-1);
      }
      Array<ULL> maxIndSets;
      getMaximalIndependentSets(edges,maxIndSets);
    }
  }
  static Array<ULL> &getMaximalIndependentSets(Array<ULL> &edges,Array<ULL> &ret) {
//    Array<ULL> &ret=ULLArrays[arrIndexes.nextULLArray++];
    ret.clear();
    ULL lfmis=getLFMIS(edges);
    if (debug) {
      prnt("GetMaximalIndependentSets\nEdges:\n");
      ULL ull;
      ForEachValueInArray(ull,edges) {
        prnt("%4d",ullIndex);
        prntBits("",ull,edges.num());
      }
      prntBits("LFMIS",lfmis,edges.num());
    }    
    searchMIS(lfmis,lfmis,edges,ret);
    if (debug) {
      prnt("Max ind sets:\n");
      ULL ull;
      ForEachValueInArray(ull,ret) {
        prnt("%4d",ullIndex);
        prntBits("",ull,edges.num());
      }
    }
    return(ret);
  }

  static bool canAdd(long nodei,ULL to,Array<ULL> &edges) {return(!(edges[nodei]&to));}

  static ULL getLFMIS(Array<ULL> &edges) {
    ULL ret=0;
    long i;
    ForEachIndexInArray(i,edges) if (canAdd(i,ret,edges)) ret|=shiftOneULL[i];
    return(ret);
  }

  static bool isMaximalIndependentSet(ULL check,Array<ULL> &edges) {
    long i;
    if (!isIndependentSet(check,edges)) ERROR("the set isn't maximally independent because it isn't independent");
    ForEachIndexInArray(i,edges) if ((!(check&shiftOneULL[i]))&&canAdd(i,check,edges)) return(false);
    return(true);
  }

  static bool isIndependentSet(ULL check,Array<ULL> &edges) {
    long i;
    ForEachIndexInArray(i,edges) if ((check&shiftOneULL[i])&&(check&edges[i])) return(false);
    return(true);
  }

  static void searchMIS(ULL mis,ULL lfmis,Array<ULL> &edges,Array<ULL> &ret,long depth=0) {
    if (fullDebug) {
      prnt("  search MIS");
      prntBits("",mis,edges.num(),false);
      prnt("LFMIS");
      prntBits("",lfmis,edges.num());
    }
    //    if (!isMaximalIndependentSet(mis,edges)) ERROR("searchMIS called, but mis argument is not maximal");
    long i;
    ULL ull;
    ForEachValueInArray(ull,ret) if (ull==mis) return;
    ret+=mis;
    if (fullDebug) prntBits("MIS",mis,edges.num());
    ULL lowMask=0;
    ForEachIndexInArray(i,edges) {
      lowMask|=shiftOneULL[i];
      if ((mis^lfmis)&shiftOneULL[i]) break;
      else if (mis&shiftOneULL[i]) {// vertexes in common prefix
        ULL S=edges[i]&~lowMask;// higher neighbours
        if (S) searchMISSubsets(0,0,S,mis,lfmis,edges,ret,depth);
      }
    }
  }

  static void searchMISSubsets(ULL subset,long fromi,ULL S,ULL mis,ULL lfmis,Array<ULL> &edges,Array<ULL> &ret,long depth=0) {
    if (!isIndependentSet(subset,edges)) return;
    if (fullDebug) {
      prntBits("  search subset ",subset,edges.num(),false);
      prnt("fromi %d  ",fromi);
      prntBits("  search subsets of",S,edges.num(),false);
      prntBits("  MIS",mis,edges.num());
    }
    long i;
    for (i=fromi;i<edges.num();i++) if (S&shiftOneULL[i]) break;
    if (i==edges.num()) {
      if (subset) {
        ULL indset=~0;
        ForEachIndexInArray(i,edges) if (subset&shiftOneULL[i]) indset&=~edges[i];
        indset&=mis|subset;
        if (fullDebug) prntBits("  indset",indset);
        if (isMaximalIndependentSet(indset,edges)) searchMIS(indset,lfmis,edges,ret,depth+1);
      }
    }
    else {
      searchMISSubsets(subset,i+1,S,mis,lfmis,edges,ret,depth);
      searchMISSubsets(subset|shiftOneULL[i],i+1,S,mis,lfmis,edges,ret,depth);
    }
  }

  static void prntBits(const char *str,ULL ull,long numNodes=0,bool prntret=true) {
    prnt("%s {",str);
    for (long i=0;(i<numNodes)||(ull&~(shiftOneULL[i]-1));i++) prnt("%s",ull&shiftOneULL[i]?"1":"0");
    prnt("} ");
    if (prntret) prnt("\n");
  }












#define MAXULLSPERNODE 32

  static Array<ULL> &getMaximalIndependentSets(Array<ULL> &edges,long ULLsPerNode,Array<ULL> &ret) {
    if (ULLsPerNode==1) return(getMaximalIndependentSets(edges,ret));

//    Array<ULL> &ret=ULLArrays[arrIndexes.nextULLArray++];
    ret.clear();

    long numNodes=edges.num()/ULLsPerNode;

    ULL lfmis[MAXULLSPERNODE];
    getLFMIS(lfmis,numNodes,ULLsPerNode,edges);
    if (debug) {
      prnt("GetMaximalIndependentSets\nEdges:\n");
      ULL ull;
      ForEachValueInArray(ull,edges) if (!(ullIndex%ULLsPerNode)) {
        prnt("%4d",ullIndex/ULLsPerNode);
        prntBits("",&ull,numNodes);
      }
      prntBits("LFMIS",lfmis,numNodes);
    }    
    searchMIS(lfmis,lfmis,numNodes,ULLsPerNode,edges,ret);
    if (debug) {
      prnt("Max ind sets:\n");
      ULL ull;
      ForEachValueInArray(ull,ret) if (!(ullIndex%ULLsPerNode)) {
        prnt("%4d",ullIndex/ULLsPerNode);
        prntBits("",&ull,numNodes);
      }
    }
    return(ret);
  }

  static bool canAdd(long nodei,ULL *to,long numNodes,long ULLsPerNode,Array<ULL> &edges) {
    for (long i=0;i<ULLsPerNode;i++) if (edges[nodei*ULLsPerNode+i]&to[i]) return(false);
    return(true);
  }

  static bool hasIntersection(ULL *a,ULL *b,long ULLsPerNode) {
    for (long i=0;i<ULLsPerNode;i++) if (a[i]&b[i]) return(true);
    return(false);
  }

  static void getLFMIS(ULL *lfmis,long numNodes,long ULLsPerNode,Array<ULL> &edges) {
    long i;
    for (i=0;i<ULLsPerNode;i++) lfmis[i]=0;
    for (i=0;i<numNodes;i++) if (canAdd(i,lfmis,numNodes,ULLsPerNode,edges)) lfmis[i>>6]|=shiftOneULL[i&63];
  }

  static bool isMaximalIndependentSet(ULL *check,long numNodes,long ULLsPerNode,Array<ULL> &edges) {
    long i;
    if (!isIndependentSet(check,numNodes,ULLsPerNode,edges)) ERROR("the set isn't maximally independent because it isn't independent");
    for (i=0;i<numNodes;i++) if ((!(check[i>>6]&shiftOneULL[i&63]))&&canAdd(i,check,numNodes,ULLsPerNode,edges)) return(false);
    return(true);
  }

  static bool isIndependentSet(ULL *check,long numNodes,long ULLsPerNode,Array<ULL> &edges) {
    long i;
    for (i=0;i<numNodes;i++) if ((check[i>>6]&shiftOneULL[i&63])&&hasIntersection(check,&edges[i*ULLsPerNode],ULLsPerNode)) return(false);
    return(true);
  }

  static void searchMIS(ULL *mis,ULL *lfmis,long numNodes,long ULLsPerNode,Array<ULL> &edges,Array<ULL> &ret,long depth=0) {
    if (fullDebug) {
      prnt("  search MIS");
      prntBits("",mis,numNodes,false);
      prnt("LFMIS");
      prntBits("",lfmis,numNodes);
    }
    //    if (!isMaximalIndependentSet(mis,edges)) ERROR("searchMIS called, but mis argument is not maximal");
    long i,ii;
    for (i=0;i<ret.num();i+=ULLsPerNode) {
      for (ii=0;ii<ULLsPerNode;ii++) if (ret[i+ii]!=mis[ii]) break;
      if (ii==ULLsPerNode) return;
    }
    for (ii=0;ii<ULLsPerNode;ii++) ret+=mis[ii];

    if (fullDebug) prntBits("MIS",mis,numNodes);

    ULL lowMask[MAXULLSPERNODE];
    memset(lowMask,0,sizeof(lowMask));
    for (i=0;i<numNodes;i++) {
      lowMask[i>>6]|=shiftOneULL[i&63];
      if ((mis[i>>6]^lfmis[i>>6])&shiftOneULL[i&63]) break;
      else if (mis[i>>6]&shiftOneULL[i&63]) {// vertexes in common prefix
        ULL S[MAXULLSPERNODE];
        bool nonZero=false;
        for (ii=0;ii<ULLsPerNode;ii++) if (S[ii]=edges[i*ULLsPerNode+ii]&~lowMask[ii]) nonZero=true;// higher neighbours
        if (nonZero) {
          ULL subset[MAXULLSPERNODE];
          memset(subset,0,sizeof(subset));
          searchMISSubsets(subset,true,0,S,mis,lfmis,numNodes,ULLsPerNode,edges,ret,depth);
        }
      }
    }
  }

  static void searchMISSubsets(ULL *subset,bool subsetZero,long fromi,ULL *S,ULL *mis,ULL *lfmis,long numNodes,long ULLsPerNode,Array<ULL> &edges,Array<ULL> &ret,long depth=0) {
    if (!isIndependentSet(subset,numNodes,ULLsPerNode,edges)) return;
    if (fullDebug) {
      prntBits("  search subset ",subset,numNodes,false);
      prnt("fromi %d  ",fromi);
      prntBits("  search subsets of",S,numNodes,false);
      prntBits("  MIS",mis,numNodes);
    }
    long i,ii;
    for (i=fromi;i<numNodes;i++) if (S[i>>6]&shiftOneULL[i&63]) break;
    if (i==numNodes) {
      if (!subsetZero) {
        ULL indset[MAXULLSPERNODE];
        memset(indset,~0,sizeof(indset));
        for (i=0;i<numNodes;i++) if (subset[i>>6]&shiftOneULL[i&63]) {
          for (ii=0;ii<ULLsPerNode;ii++) indset[ii]&=~edges[i*ULLsPerNode+ii];
        }
        for (ii=0;ii<ULLsPerNode;ii++) indset[ii]&=mis[ii]|subset[ii];
        if (fullDebug) prntBits("  indset",indset);
        if (isMaximalIndependentSet(indset,numNodes,ULLsPerNode,edges)) searchMIS(indset,lfmis,numNodes,ULLsPerNode,edges,ret,depth+1);
      }
    }
    else {
      searchMISSubsets(subset,subsetZero,i+1,S,mis,lfmis,numNodes,ULLsPerNode,edges,ret,depth);
      ULL newSubset[MAXULLSPERNODE];
      memcpy(newSubset,subset,sizeof(newSubset));
      newSubset[i>>6]|=shiftOneULL[i&63];
      searchMISSubsets(newSubset,false,i+1,S,mis,lfmis,numNodes,ULLsPerNode,edges,ret,depth);
    }
  }

  static void prntBits(const char *str,ULL *ull,long numNodes=0,bool prntret=true) {
    prnt("%s {",str);
    for (long i=0;i<numNodes;i++) prnt("%s",ull[i>>6]&shiftOneULL[i&63]?"1":"0");
    prnt("} ");
    if (prntret) prnt("\n");
  }
};

#ifdef MAINCPPFILE
bool Graph::debug=false;
bool Graph::fullDebug=false;
#endif





#endif

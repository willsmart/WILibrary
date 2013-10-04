//>Maker.cpp

#ifndef ERRORSANDNOTESH
#define ERRORSANDNOTESH

bool error(bool quit,const char *type,const char *file,long line,const char *name,const char *fmt,...);
const char *name();
#undef ERROR
#define ERROR(fmt) error(true,"Error",__FILE__,__LINE__,name(),fmt)
#define ERROR2(fmt,arg) error(true,"Error",__FILE__,__LINE__,name(),fmt,arg)
#define ERROR3(fmt,arg1,arg2) error(true,"Error",__FILE__,__LINE__,name(),fmt,arg1,arg2)
#define ERROR4(fmt,arg1,arg2,arg3) error(true,"Error",__FILE__,__LINE__,name(),fmt,arg1,arg2,arg3)
#define ERROR5(fmt,arg1,arg2,arg3,arg4) error(true,"Error",__FILE__,__LINE__,name(),fmt,arg1,arg2,arg3,arg4)
#define ERROR6(fmt,arg1,arg2,arg3,arg4,arg5) error(true,"Error",__FILE__,__LINE__,name(),fmt,arg1,arg2,arg3,arg4,arg5)
#define ERROR7(fmt,arg1,arg2,arg3,arg4,arg5,arg6) error(true,"Error",__FILE__,__LINE__,name(),fmt,arg1,arg2,arg3,arg4,arg5,arg6)
#define ERROR8(fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7) error(true,"Error",__FILE__,__LINE__,name(),fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7)
#define ERROR9(fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8) error(true,"Error",__FILE__,__LINE__,name(),fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8)
#define ERROR10(fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9) error(true,"Error",__FILE__,__LINE__,name(),fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9)
#define SERROR(fmt) error(true,"Error",__FILE__,__LINE__,"",fmt)
#define SERROR2(fmt,arg) error(true,"Error",__FILE__,__LINE__,"",fmt,arg)
#define SERROR3(fmt,arg1,arg2) error(true,"Error",__FILE__,__LINE__,"",fmt,arg1,arg2)
#define SERROR4(fmt,arg1,arg2,arg3) error(true,"Error",__FILE__,__LINE__,"",fmt,arg1,arg2,arg3)
#define SERROR5(fmt,arg1,arg2,arg3,arg4) error(true,"Error",__FILE__,__LINE__,"",fmt,arg1,arg2,arg3,arg4)
#define SERROR6(fmt,arg1,arg2,arg3,arg4,arg5) error(true,"Error",__FILE__,__LINE__,"",fmt,arg1,arg2,arg3,arg4,arg5)
#define SERROR7(fmt,arg1,arg2,arg3,arg4,arg5,arg6) error(true,"Error",__FILE__,__LINE__,"",fmt,arg1,arg2,arg3,arg4,arg5,arg6)
#define SERROR8(fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7) error(true,"Error",__FILE__,__LINE__,"",fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7)
#define SERROR9(fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8) error(true,"Error",__FILE__,__LINE__,"",fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8)
#define SERROR10(fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9) error(true,"Error",__FILE__,__LINE__,"",fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9)

#define WARN(fmt) error(false,"warning",__FILE__,__LINE__,name(),fmt)
#define WARN2(fmt,arg) error(false,"warning",__FILE__,__LINE__,name(),fmt,arg)
#define WARN3(fmt,arg1,arg2) error(false,"warning",__FILE__,__LINE__,name(),fmt,arg1,arg2)
#define WARN4(fmt,arg1,arg2,arg3) error(false,"warning",__FILE__,__LINE__,name(),fmt,arg1,arg2,arg3)
#define WARN5(fmt,arg1,arg2,arg3,arg4) error(false,"warning",__FILE__,__LINE__,name(),fmt,arg1,arg2,arg3,arg4)
#define WARN6(fmt,arg1,arg2,arg3,arg4,arg5) error(false,"warning",__FILE__,__LINE__,name(),fmt,arg1,arg2,arg3,arg4,arg5)
#define WARN7(fmt,arg1,arg2,arg3,arg4,arg5,arg6) error(false,"warning",__FILE__,__LINE__,name(),fmt,arg1,arg2,arg3,arg4,arg5,arg6)
#define WARN8(fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7) error(false,"warning",__FILE__,__LINE__,name(),fmt,arg1,arg2,arg3,arg4,arg5,arg6,arg7)


#define MAXNOTES 1000
#define MAXNOTELEN 200





class Note {
 public:
  static char notes[MAXNOTES][MAXNOTELEN];
  static long long noteOrder[MAXNOTES];
  static long long upto;
  static long lastFreed,numNotes;
  long index;
  Note(const char *file,long line,const char *name,const char *fmt,...);    
  ~Note();
  static void printStack(FILE *fil=stderr);
};


#define FNNOTE(fmt) NOTEENABLE(Note _fnNote(__FILE__,__LINE__,name(),fmt))
#define FNNOTE2(fmt,arg1) NOTEENABLE(Note _fnNote(__FILE__,__LINE__,name(),fmt,arg1))
#define FNNOTE3(fmt,arg1,arg2) NOTEENABLE(Note _fnNote(__FILE__,__LINE__,name(),fmt,arg1,arg2))
#define FNNOTE4(fmt,arg1,arg2,arg3) NOTEENABLE(Note _fnNote(__FILE__,__LINE__,name(),fmt,arg1,arg2,arg3))
#define FNNOTE5(fmt,arg1,arg2,arg3,arg4) NOTEENABLE(Note _fnNote(__FILE__,__LINE__,name(),fmt,arg1,arg2,arg3,arg4))
#define FNNOTE6(fmt,arg1,arg2,arg3,arg4,arg5) NOTEENABLE(Note _fnNote(__FILE__,__LINE__,name(),fmt,arg1,arg2,arg3,arg4,arg5))

#define NOTE2(var,fmt) NOTEENABLE(Note _note##var(__FILE__,__LINE__,name(),fmt))
#define NOTE3(var,fmt,arg1) NOTEENABLE(Note _note##var(__FILE__,__LINE__,name(),fmt,arg1))
#define NOTE4(var,fmt,arg1,arg2) NOTEENABLE(Note _note##var(__FILE__,__LINE__,name(),fmt,arg1,arg2))
#define NOTE5(var,fmt,arg1,arg2,arg3) NOTEENABLE(Note _note##var(__FILE__,__LINE__,name(),fmt,arg1,arg2,arg3))
#define NOTE6(var,fmt,arg1,arg2,arg3,arg4) NOTEENABLE(Note _note##var(__FILE__,__LINE__,name(),fmt,arg1,arg2,arg3,arg4))
#define NOTE7(var,fmt,arg1,arg2,arg3,arg4,arg5) NOTEENABLE(Note _note##var(__FILE__,__LINE__,name(),fmt,arg1,arg2,arg3,arg4,arg5))

// use something like :
//#undef NOTEENABLE
//#define NOTEENABLE(a)
// to turn notes off, and:
//#undef NOTEENABLE
//#define NOTEENABLE(a) a
// to turn them on






#ifdef MAINCPPFILE

#ifndef ERRORHWND
#define ERRORHWND NULL
#endif

bool error(bool quit,const char *type,const char *file,long line,const char *name,const char *fmt,...) {
  if (quit) {
    Note::printStack();
    eoprnt("\n%dMB<%dMB\n",long(g_alloced>>20),long(g_maxAlloced>>20));
    eoprintTypeAllocs();
  }
  char buf[100000];
  if (name) sprintf(buf,"\n%s!!! file %s, line %ld, object named %s:\n>>>>>>>>>>>>>",type,file,line,name);
  else sprintf(buf,"\n%s!!! file %s, line %ld:\n>>>>>>>>>>>",type,file,line);
  va_list pArgs;
  va_start(pArgs, fmt);
  vsprintf(buf+strlen(buf), fmt, pArgs);
  va_end(pArgs);
#ifdef ERRORSAREMESSAGEBOXES
  wchar_t wbuf[10000],*wpc;
  char *pc;
  for (pc=buf,wpc=wbuf;*pc;) *(wpc++)=*(pc++);
  *wpc=0;
  MessageBox(ERRORHWND,wbuf,quit?L"FATAL ERROR":L"Warning",MB_OK|MB_ICONERROR);
#else
  eoprnt("%s\n\n",buf);
  if (quit) {
    eoprnt("Quit.\n");
//    getc(stdin);
  }
#endif
  if (quit) {
#ifdef ERRORSAREMESSAGEBOXES
    PostQuitMessage(0);
#else
    exit(1);
#endif
  }
  return(true);
}

const char *name() {return("<no name>");}

char Note::notes[MAXNOTES][MAXNOTELEN];
long long Note::noteOrder[MAXNOTES];
long long Note::upto=0;
long Note::lastFreed=-1;
long Note::numNotes=0;


static char g_noteBuf[10000];

Note::Note(const char *file,long line,const char *name,const char *fmt,...) {
  if (lastFreed==-1) {
    for (long i=0;i<MAXNOTES;i++) notes[i][0]=0;
    lastFreed=numNotes=0;
  }

  index=-1;
  if (numNotes!=MAXNOTES) {
    if (name) sprintf(g_noteBuf,"%s:%ld, object %s  --  ",file,line,name);
    else sprintf(g_noteBuf,"%s:%ld  --  ",file,line);
    va_list pArgs;
    va_start(pArgs, fmt);
    vsprintf(g_noteBuf+strlen(g_noteBuf), fmt, pArgs);
    va_end(pArgs);
    long len=long(strlen(g_noteBuf));
    if (len>MAXNOTELEN-1) len=MAXNOTELEN-1;
    if (len) {
      g_noteBuf[len]=0;

      if (notes[lastFreed][0]) for (lastFreed=(lastFreed+1)%MAXNOTES;notes[lastFreed][0];lastFreed=(lastFreed+1)%MAXNOTES);

      strcpy(notes[index=lastFreed],g_noteBuf);
      numNotes++;
      upto++;
      noteOrder[index]=upto;

      printStack(stderr);
    }
  }
}

Note::~Note() {
  if (index>=0) {
    notes[index][0]=0;numNotes--;
    fprintf(efil,"out note.\n");
  }
}


void Note::printStack(FILE *fil) {
  long long nextLeast,prevLeast=0;
  long nextLeastAt,notei=1;
  fprintf(fil,"Notes:\n");
  do {
    nextLeastAt=-1;
    for (long i=0;i<MAXNOTES;i++) if (notes[i][0]&&(noteOrder[i]>prevLeast)&&
                                      ((nextLeastAt==-1)||(noteOrder[i]<nextLeast))) {
      nextLeast=noteOrder[i];
      nextLeastAt=i;
    }
    if (nextLeastAt<0) break;
    fprintf(fil,">> #%ld : %s\n",notei++,notes[nextLeastAt]);
    prevLeast=nextLeast;
  } while (1);
  fprintf(fil,"\n");
}
  


#endif






#endif

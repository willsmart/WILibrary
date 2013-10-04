#pragma once

struct Vec4 {float v[4];};
#define SETVEC4(v,x,y,z,w) do {Vec4 &vv=(v);vv.v[0]=(x);vv.v[1]=(y);vv.v[2]=(z);vv.v[3]=w;} while(0)
#define SETVEC3(v,x,y,z) do {Vec4 &vv=(v);vv.v[0]=(x);vv.v[1]=(y);vv.v[2]=(z);vv.v[3]=1;} while(0)
#define VEC4(x,y,z,w) {x,y,z,w}
#define VEC3(x,y,z,w) {x,y,z,1}
#define DEFVEC4(v,x,y,z,w) Vec4 v={x,y,z,w}
#define DEFVEC3(v,x,y,z) Vec4 v={x,y,z,1}

#define MatM(mm,r,c) ((mm).m[((r)<<2)+(c)])
#define MatMp(mm,r,c) ((mm)[((r)<<2)+(c)])
#define _MatM(r,c) (m.m[((r)<<2)+(c)])

#define MatM1(mm,r,c) MatM(mm,(r)-1,(c)-1)
#define MatMp1(mm,r,c) MatMp(mm,(r)-1,(c)-1)
#define _MatM1(r,c) _MatM((r)-1,(c)-1)

#define VecM(vv,r) ((vv).v[r])
#define VecMp(vv,r) ((vv)[r])

struct Mat44 {
  float m[16];
};


extern Mat44 tmpm44ForMul1;
extern Mat44 tmpm44ForMul2;
#ifdef MAINCPPFILE
Mat44 tmpm44ForMul1;
Mat44 tmpm44ForMul2;
#endif


#define MULTIPLYMM(am1,am2,aom) { \
    float const *m1=am1,*m2=am2; \
    float *om=aom; \
    if (m1==om) {memcpy(tmpm44ForMul1.m,m1,sizeof(float)*16);m1=tmpm44ForMul1.m;} \
    if (m2==om) {memcpy(tmpm44ForMul2.m,m2,sizeof(float)*16);m2=tmpm44ForMul2.m;} \
    MatMp(om,0,0)=MatMp(m1,0,0)*MatMp(m2,0,0)+MatMp(m1,1,0)*MatMp(m2,0,1)+MatMp(m1,2,0)*MatMp(m2,0,2)+MatMp(m1,3,0)*MatMp(m2,0,3); \
    MatMp(om,1,0)=MatMp(m1,0,0)*MatMp(m2,1,0)+MatMp(m1,1,0)*MatMp(m2,1,1)+MatMp(m1,2,0)*MatMp(m2,1,2)+MatMp(m1,3,0)*MatMp(m2,1,3); \
    MatMp(om,2,0)=MatMp(m1,0,0)*MatMp(m2,2,0)+MatMp(m1,1,0)*MatMp(m2,2,1)+MatMp(m1,2,0)*MatMp(m2,2,2)+MatMp(m1,3,0)*MatMp(m2,2,3); \
    MatMp(om,3,0)=MatMp(m1,0,0)*MatMp(m2,3,0)+MatMp(m1,1,0)*MatMp(m2,3,1)+MatMp(m1,2,0)*MatMp(m2,3,2)+MatMp(m1,3,0)*MatMp(m2,3,3); \
 \
    MatMp(om,0,1)=MatMp(m1,0,1)*MatMp(m2,0,0)+MatMp(m1,1,1)*MatMp(m2,0,1)+MatMp(m1,2,1)*MatMp(m2,0,2)+MatMp(m1,3,1)*MatMp(m2,0,3); \
    MatMp(om,1,1)=MatMp(m1,0,1)*MatMp(m2,1,0)+MatMp(m1,1,1)*MatMp(m2,1,1)+MatMp(m1,2,1)*MatMp(m2,1,2)+MatMp(m1,3,1)*MatMp(m2,1,3); \
    MatMp(om,2,1)=MatMp(m1,0,1)*MatMp(m2,2,0)+MatMp(m1,1,1)*MatMp(m2,2,1)+MatMp(m1,2,1)*MatMp(m2,2,2)+MatMp(m1,3,1)*MatMp(m2,2,3); \
    MatMp(om,3,1)=MatMp(m1,0,1)*MatMp(m2,3,0)+MatMp(m1,1,1)*MatMp(m2,3,1)+MatMp(m1,2,1)*MatMp(m2,3,2)+MatMp(m1,3,1)*MatMp(m2,3,3); \
 \
    MatMp(om,0,2)=MatMp(m1,0,2)*MatMp(m2,0,0)+MatMp(m1,1,2)*MatMp(m2,0,1)+MatMp(m1,2,2)*MatMp(m2,0,2)+MatMp(m1,3,2)*MatMp(m2,0,3); \
    MatMp(om,1,2)=MatMp(m1,0,2)*MatMp(m2,1,0)+MatMp(m1,1,2)*MatMp(m2,1,1)+MatMp(m1,2,2)*MatMp(m2,1,2)+MatMp(m1,3,2)*MatMp(m2,1,3); \
    MatMp(om,2,2)=MatMp(m1,0,2)*MatMp(m2,2,0)+MatMp(m1,1,2)*MatMp(m2,2,1)+MatMp(m1,2,2)*MatMp(m2,2,2)+MatMp(m1,3,2)*MatMp(m2,2,3); \
    MatMp(om,3,2)=MatMp(m1,0,2)*MatMp(m2,3,0)+MatMp(m1,1,2)*MatMp(m2,3,1)+MatMp(m1,2,2)*MatMp(m2,3,2)+MatMp(m1,3,2)*MatMp(m2,3,3); \
 \
    MatMp(om,0,3)=MatMp(m1,0,3)*MatMp(m2,0,0)+MatMp(m1,1,3)*MatMp(m2,0,1)+MatMp(m1,2,3)*MatMp(m2,0,2)+MatMp(m1,3,3)*MatMp(m2,0,3); \
    MatMp(om,1,3)=MatMp(m1,0,3)*MatMp(m2,1,0)+MatMp(m1,1,3)*MatMp(m2,1,1)+MatMp(m1,2,3)*MatMp(m2,1,2)+MatMp(m1,3,3)*MatMp(m2,1,3); \
    MatMp(om,2,3)=MatMp(m1,0,3)*MatMp(m2,2,0)+MatMp(m1,1,3)*MatMp(m2,2,1)+MatMp(m1,2,3)*MatMp(m2,2,2)+MatMp(m1,3,3)*MatMp(m2,2,3); \
    MatMp(om,3,3)=MatMp(m1,0,3)*MatMp(m2,3,0)+MatMp(m1,1,3)*MatMp(m2,3,1)+MatMp(m1,2,3)*MatMp(m2,3,2)+MatMp(m1,3,3)*MatMp(m2,3,3); \
  }

#define MULTIPLYVM(v,m,ov) { \
      VecMp(ov,0)=MatMp(m,0,0)*VecMp(v,0)+MatMp(m,1,0)*VecMp(v,1)+MatMp(m,2,0)*VecMp(v,2)+MatMp(m,3,0)*VecMp(v,3); \
      VecMp(ov,1)=MatMp(m,0,1)*VecMp(v,0)+MatMp(m,1,1)*VecMp(v,1)+MatMp(m,2,1)*VecMp(v,2)+MatMp(m,3,1)*VecMp(v,3); \
      VecMp(ov,2)=MatMp(m,0,2)*VecMp(v,0)+MatMp(m,1,2)*VecMp(v,1)+MatMp(m,2,2)*VecMp(v,2)+MatMp(m,3,2)*VecMp(v,3); \
      VecMp(ov,3)=MatMp(m,0,3)*VecMp(v,0)+MatMp(m,1,3)*VecMp(v,1)+MatMp(m,2,3)*VecMp(v,2)+MatMp(m,3,3)*VecMp(v,3); \
}

#define MDET2x2(m,r,c) (_MatM(m,(r),(c))*_MatM(m,(r)+1,(c)+1)-_MatM(m,(r)+1,(c))*_MatM(m,(r),(c)+1))
#define MDET3x3(m,r,c) (\
  _MatM(m,(r)  ,(c))*_MatM(m,(r)+1,(c)+1)*_MatM(m,(r)+2,(c)+2)+\
  _MatM(m,(r)+1,(c))*_MatM(m,(r)+2,(c)+1)*_MatM(m,(r)  ,(c)+2)+\
  _MatM(m,(r)+2,(c))*_MatM(m,(r)  ,(c)+1)*_MatM(m,(r)+1,(c)+2)-\
  _MatM(m,(r)  ,(c))*_MatM(m,(r)+2,(c)+1)*_MatM(m,(r)+1,(c)+2)-\
  _MatM(m,(r)+2,(c))*_MatM(m,(r)+1,(c)+1)*_MatM(m,(r)  ,(c)+2)-\
  _MatM(m,(r)+1,(c))*_MatM(m,(r)  ,(c)+1)*_MatM(m,(r)+2,(c)+2))
#define MDET4x4(m) (\
(_MatM(m,0,0)*_MatM(m,1,1)*_MatM(m,2,2)*_MatM(m,3,3)+  _MatM(m,0,0)*_MatM(m,1,2)*_MatM(m,2,3)*_MatM(m,3,1)+  _MatM(m,0,0)*_MatM(m,1,3)*_MatM(m,2,1)*_MatM(m,3,2))+\
(_MatM(m,0,1)*_MatM(m,1,0)*_MatM(m,2,3)*_MatM(m,3,2)+  _MatM(m,0,1)*_MatM(m,1,2)*_MatM(m,2,0)*_MatM(m,3,3)+  _MatM(m,0,1)*_MatM(m,1,3)*_MatM(m,2,2)*_MatM(m,3,0))+\
(_MatM(m,0,2)*_MatM(m,1,0)*_MatM(m,2,1)*_MatM(m,3,3)+  _MatM(m,0,2)*_MatM(m,1,1)*_MatM(m,2,3)*_MatM(m,3,0)+  _MatM(m,0,2)*_MatM(m,1,3)*_MatM(m,2,0)*_MatM(m,3,1))+\
(_MatM(m,0,3)*_MatM(m,1,0)*_MatM(m,2,2)*_MatM(m,3,1)+  _MatM(m,0,3)*_MatM(m,1,1)*_MatM(m,2,0)*_MatM(m,3,2)+  _MatM(m,0,3)*_MatM(m,1,2)*_MatM(m,2,1)*_MatM(m,3,0))-\
\
(_MatM(m,0,0)*_MatM(m,1,1)*_MatM(m,2,3)*_MatM(m,3,2)+  _MatM(m,0,0)*_MatM(m,1,2)*_MatM(m,2,1)*_MatM(m,3,3)+  _MatM(m,0,0)*_MatM(m,1,3)*_MatM(m,2,2)*_MatM(m,3,1))-\
(_MatM(m,0,1)*_MatM(m,1,0)*_MatM(m,2,2)*_MatM(m,3,3)+  _MatM(m,0,1)*_MatM(m,1,2)*_MatM(m,2,3)*_MatM(m,3,0)+  _MatM(m,0,1)*_MatM(m,1,3)*_MatM(m,2,0)*_MatM(m,3,2))-\
(_MatM(m,0,2)*_MatM(m,1,0)*_MatM(m,2,3)*_MatM(m,3,1)+  _MatM(m,0,2)*_MatM(m,1,1)*_MatM(m,2,0)*_MatM(m,3,3)+  _MatM(m,0,2)*_MatM(m,1,3)*_MatM(m,2,1)*_MatM(m,3,0))-\
(_MatM(m,0,3)*_MatM(m,1,0)*_MatM(m,2,1)*_MatM(m,3,2)+  _MatM(m,0,3)*_MatM(m,1,1)*_MatM(m,2,3)*_MatM(m,3,0)+  _MatM(m,0,3)*_MatM(m,1,2)*_MatM(m,2,0)*_MatM(m,3,1)))


#define INVM(m,om) { \
  double det=MDET4x4(m); \
  if (det!=0) {\
    MatMp1(om,1,1)= (_MatM1(2,2)*_MatM1(3,3)*_MatM1(4,4) + _MatM1(2,3)*_MatM1(3,4)*_MatM1(4,3) + _MatM1(2,4)*_MatM1(3,2)*_MatM1(4,3) - _MatM1(2,2)*_MatM1(3,4)*_MatM1(4,3) - _MatM1(2,3)*_MatM1(3,2)*_MatM1(4,4) - _MatM1(2,4)*_MatM1(3,3)*_MatM1(4,2;\
    MatMp1(om,1,2)= (_MatM1(1,2)*_MatM1(3,4)*_MatM1(4,3) + _MatM1(1,3)*_MatM1(3,2)*_MatM1(4,2) + _MatM1(1,4)*_MatM1(3,3)*_MatM1(4,2) - _MatM1(1,2)*_MatM1(3,3)*_MatM1(4,4) - _MatM1(1,3)*_MatM1(3,4)*_MatM1(4,2) - _MatM1(1,4)*_MatM1(3,2)*_MatM1(4,3;\
    MatMp1(om,1,3)= (_MatM1(1,2)*_MatM1(2,3)*_MatM1(4,4) + _MatM1(1,3)*_MatM1(2,4)*_MatM1(4,3) + _MatM1(1,4)*_MatM1(2,2)*_MatM1(4,3) - _MatM1(1,2)*_MatM1(2,4)*_MatM1(4,3) - _MatM1(1,3)*_MatM1(2,2)*_MatM1(4,4) - _MatM1(1,4)*_MatM1(2,3)*_MatM1(4,2;\
    MatMp1(om,1,4)= (_MatM1(1,2)*_MatM1(2,4)*_MatM1(3,3) + _MatM1(1,3)*_MatM1(2,2)*_MatM1(3,2) + _MatM1(1,4)*_MatM1(2,3)*_MatM1(3,2) - _MatM1(1,2)*_MatM1(2,3)*_MatM1(3,4) - _MatM1(1,3)*_MatM1(2,4)*_MatM1(3,2) - _MatM1(1,4)*_MatM1(2,2)*_MatM1(3,3;\
    MatMp1(om,2,1)= (_MatM1(2,1)*_MatM1(3,4)*_MatM1(4,3) + _MatM1(2,3)*_MatM1(3,1)*_MatM1(4,1) + _MatM1(2,4)*_MatM1(3,3)*_MatM1(4,1) - _MatM1(2,1)*_MatM1(3,3)*_MatM1(4,4) - _MatM1(2,3)*_MatM1(3,4)*_MatM1(4,1) - _MatM1(2,4)*_MatM1(3,1)*_MatM1(4,3;\
    MatMp1(om,2,2)= (_MatM1(1,1)*_MatM1(3,3)*_MatM1(4,4) + _MatM1(1,3)*_MatM1(3,4)*_MatM1(4,3) + _MatM1(1,4)*_MatM1(3,1)*_MatM1(4,3) - _MatM1(1,1)*_MatM1(3,4)*_MatM1(4,3) - _MatM1(1,3)*_MatM1(3,1)*_MatM1(4,4) - _MatM1(1,4)*_MatM1(3,3)*_MatM1(4,1;\
    MatMp1(om,2,3)= (_MatM1(1,1)*_MatM1(2,4)*_MatM1(4,3) + _MatM1(1,3)*_MatM1(2,1)*_MatM1(4,1) + _MatM1(1,4)*_MatM1(2,3)*_MatM1(4,1) - _MatM1(1,1)*_MatM1(2,3)*_MatM1(4,4) - _MatM1(1,3)*_MatM1(2,4)*_MatM1(4,1) - _MatM1(1,4)*_MatM1(2,1)*_MatM1(4,3;\
    MatMp1(om,2,4)= (_MatM1(1,1)*_MatM1(2,3)*_MatM1(3,4) + _MatM1(1,3)*_MatM1(2,4)*_MatM1(3,3) + _MatM1(1,4)*_MatM1(2,1)*_MatM1(3,3) - _MatM1(1,1)*_MatM1(2,4)*_MatM1(3,3) - _MatM1(1,3)*_MatM1(2,1)*_MatM1(3,4) - _MatM1(1,4)*_MatM1(2,3)*_MatM1(3,1;\
    MatMp1(om,3,1)= (_MatM1(2,1)*_MatM1(3,2)*_MatM1(4,4) + _MatM1(2,2)*_MatM1(3,1)*_MatM1(4,2) + _MatM1(2,4)*_MatM1(3,1)*_MatM1(4,2) - _MatM1(2,1)*_MatM1(3,4)*_MatM1(4,2) - _MatM1(2,2)*_MatM1(3,1)*_MatM1(4,4) - _MatM1(2,4)*_MatM1(3,2)*_MatM1(4,1;\
    MatMp1(om,3,2)= (_MatM1(1,1)*_MatM1(3,4)*_MatM1(4,2) + _MatM1(1,2)*_MatM1(3,4)*_MatM1(4,1) + _MatM1(1,4)*_MatM1(3,2)*_MatM1(4,1) - _MatM1(1,1)*_MatM1(3,2)*_MatM1(4,4) - _MatM1(1,2)*_MatM1(3,4)*_MatM1(4,1) - _MatM1(1,4)*_MatM1(3,1)*_MatM1(4,2;\
    MatMp1(om,3,3)= (_MatM1(1,1)*_MatM1(2,2)*_MatM1(4,4) + _MatM1(1,2)*_MatM1(2,1)*_MatM1(4,2) + _MatM1(1,4)*_MatM1(2,1)*_MatM1(4,2) - _MatM1(1,1)*_MatM1(2,4)*_MatM1(4,2) - _MatM1(1,2)*_MatM1(2,1)*_MatM1(4,4) - _MatM1(1,4)*_MatM1(2,2)*_MatM1(4,1;\
    MatMp1(om,3,4)= (_MatM1(1,1)*_MatM1(2,4)*_MatM1(3,2) + _MatM1(1,2)*_MatM1(2,4)*_MatM1(3,1) + _MatM1(1,4)*_MatM1(2,2)*_MatM1(3,1) - _MatM1(1,1)*_MatM1(2,2)*_MatM1(3,4) - _MatM1(1,2)*_MatM1(2,4)*_MatM1(3,1) - _MatM1(1,4)*_MatM1(2,1)*_MatM1(3,2;\
    MatMp1(om,4,1)= (_MatM1(2,1)*_MatM1(3,3)*_MatM1(4,2) + _MatM1(2,2)*_MatM1(3,3)*_MatM1(4,1) + _MatM1(2,3)*_MatM1(3,2)*_MatM1(4,1) - _MatM1(2,1)*_MatM1(3,2)*_MatM1(4,3) - _MatM1(2,2)*_MatM1(3,3)*_MatM1(4,1) - _MatM1(2,3)*_MatM1(3,1)*_MatM1(4,2;\
    MatMp1(om,4,2)= (_MatM1(1,1)*_MatM1(3,2)*_MatM1(4,3) + _MatM1(1,2)*_MatM1(3,1)*_MatM1(4,2) + _MatM1(1,3)*_MatM1(3,1)*_MatM1(4,2) - _MatM1(1,1)*_MatM1(3,3)*_MatM1(4,2) - _MatM1(1,2)*_MatM1(3,1)*_MatM1(4,3) - _MatM1(1,3)*_MatM1(3,2)*_MatM1(4,1;\
    MatMp1(om,4,3)= (_MatM1(1,1)*_MatM1(2,3)*_MatM1(4,2) + _MatM1(1,2)*_MatM1(2,3)*_MatM1(4,1) + _MatM1(1,3)*_MatM1(2,2)*_MatM1(4,1) - _MatM1(1,1)*_MatM1(2,2)*_MatM1(4,3) - _MatM1(1,2)*_MatM1(2,3)*_MatM1(4,1) - _MatM1(1,3)*_MatM1(2,1)*_MatM1(4,2;\
    MatMp1(om,4,4)= (_MatM1(1,1)*_MatM1(2,2)*_MatM1(3,3) + _MatM1(1,2)*_MatM1(2,1)*_MatM1(3,2) + _MatM1(1,3)*_MatM1(2,1)*_MatM1(3,2) - _MatM1(1,1)*_MatM1(2,3)*_MatM1(3,2) - _MatM1(1,2)*_MatM1(2,1)*_MatM1(3,3) - _MatM1(1,3)*_MatM1(2,2)*_MatM1(3,1;\
  }\
  }

  
  class BCMatrix44 {
public:
  Mat44 m;

  BCMatrix44() {identity();}
  BCMatrix44(const Mat44 &mm) {(*this)=mm;}
  BCMatrix44(const BCMatrix44 &mm) {(*this)=mm.m;}

  BCMatrix44 &operator=(const BCMatrix44 &mm) {copy(mm.m,m);return(*this);}
  BCMatrix44 &operator=(const Mat44 &mm) {copy(mm,m);return(*this);}

  BCMatrix44 &identity() {identity(m);return(*this);}

  BCMatrix44 affine(Vec4 scale,Vec4 org,Vec4 rot) {affine(m,scale,org,rot);return(*this);}
  BCMatrix44 &rotx(float deg) {rotx(m,deg);return(*this);}
  BCMatrix44 &roty(float deg) {roty(m,deg);return(*this);}
  BCMatrix44 &rotz(float deg) {rotz(m,deg);return(*this);}

  BCMatrix44 &scale(float ascale) {scale(m,ascale);return(*this);}
  BCMatrix44 &scale(float sx,float sy,float sz) {scale(m,sx,sy,sz);return(*this);}

  BCMatrix44 &translate(float dx,float dy,float dz) {translate(m,dx,dy,dz);return(*this);}

  Vec4 multiplyV(Vec4 &v) {Vec4 ov;return(multiplyV(v,m,ov));}
  BCMatrix44 &multiply(BCMatrix44 &ml,BCMatrix44 &mr) {multiply(ml.m,mr.m,m);return(*this);}
  BCMatrix44 &multiply(Mat44 &ml,BCMatrix44 &mr) {multiply(ml,mr.m,m);return(*this);}
  BCMatrix44 &multiply(BCMatrix44 &ml,Mat44 &mr) {multiply(ml.m,mr,m);return(*this);}
  BCMatrix44 &multiply(Mat44 &ml,Mat44 &mr) {multiply(ml,mr,m);return(*this);}

  BCMatrix44 &multiplyRight(BCMatrix44 &mm) {multiply(m,mm.m,m);return(*this);}
  BCMatrix44 &multiplyRight(Mat44 &mm) {multiply(m,mm,m);return(*this);}
  BCMatrix44 &multiplyLeft(BCMatrix44 &mm) {multiply(mm.m,m,m);return(*this);}
  BCMatrix44 &multiplyLeft(Mat44 &mm) {multiply(mm,m,m);return(*this);}

  char *toString(String &s,bool sepLines=false) {return(toString(m,s,sepLines));}
  char *addString(String &s,bool sepLines=false) {return(addString(m,s,sepLines));}




  static Mat44 &copy(const Mat44 &m,Mat44 &om) {memcpy(om.m,m.m,16*sizeof(m.m[0]));return(om);}

  static Mat44 &identity(Mat44 &m) {
    _MatM(1,0)=_MatM(2,0)=_MatM(3,0)= _MatM(0,1)=_MatM(2,1)=_MatM(3,1)= _MatM(0,2)=_MatM(1,2)=_MatM(3,2)= _MatM(0,3)=_MatM(1,3)=_MatM(2,3)=0;
    _MatM(0,0)=_MatM(1,1)=_MatM(2,2)=_MatM(3,3)=1;
    return(m);
  }

  static Mat44 &affine(Mat44 &m,const Vec4 &scale,const Vec4 &org,const Vec4 &rot) {
    identity(m);
    _MatM(0,0)=VecM(scale,0);_MatM(1,1)=VecM(scale,1);_MatM(2,2)=VecM(scale,2);_MatM(3,3)=VecM(scale,3);
    _MatM(3,0)=VecM(org,0);_MatM(3,1)=VecM(org,1);_MatM(3,2)=VecM(org,2);
    Mat44 mm;
    multiply(m,rotx(mm,rot.v[0]),m);
    multiply(m,roty(mm,rot.v[1]),m);
    multiply(m,rotz(mm,rot.v[2]),m);
    return(m);
  }

  static Mat44 &rotx(Mat44 &m,const float deg) {
    float rad=deg*floatPI/180;
    identity(m);
    _MatM(1,1)=_MatM(2,2)=cos(rad);
    _MatM(1,2)=-(_MatM(2,1)=sin(rad));
    return(m);
  }
  static Mat44 &roty(Mat44 &m,const float deg) {
    float rad=deg*floatPI/180;
    identity(m);
    _MatM(0,0)=_MatM(2,2)=cos(rad);
    _MatM(2,0)=-(_MatM(0,2)=sin(rad));
    return(m);
  }
  static Mat44 &rotz(Mat44 &m,const float deg) {
    float rad=deg*floatPI/180;
    identity(m);
    _MatM(0,0)=_MatM(1,1)=cos(rad);
    _MatM(1,0)=-(_MatM(0,1)=sin(rad));
    return(m);
  }
  static Mat44 &translate(Mat44 &m,const float dx,const float dy,const float dz) {
    identity(m);
    _MatM(3,0)=dx;
    _MatM(3,1)=dy;
    _MatM(3,2)=dz;
    return(m);
  }
  static Mat44 &scale(Mat44 &m,const float sx,const float sy,const float sz) {
    identity(m);
    _MatM(0,0)*=sx;_MatM(1,1)*=sy;_MatM(2,2)*=sz;
    return(m);
  }
  static Mat44 &scale(Mat44 &m,const float scale) {
    identity(m);
    _MatM(0,0)*=scale;_MatM(1,1)*=scale;_MatM(2,2)*=scale;
    return(m);
  }


  static Vec4 &multiplyV(Vec4 &v,const Mat44 &m,Vec4 &ov) {
    const float *vp=v.v,*mp=m.m;
    float *ovp=ov.v;
    MULTIPLYVM(vp,mp,ovp);
    return(ov);
  }

  static Mat44 &multiply(const Mat44 &m1,const Mat44 &m2,Mat44 &om) {
    if (&m1==&om) {Mat44 mm1;copy(m1,mm1);return(multiply(mm1,m2,om));}
    if (&m2==&om) {Mat44 mm2;copy(m2,mm2);return(multiply(m1,mm2,om));}

    const float *m1p=m1.m,*m2p=m2.m;
    float *omp=om.m;
    MULTIPLYMM(m1p,m2p,omp);
    return(om);
  }


  static char *toString(const Vec4 &v,String &s) {s="";return(addString(v,s));}
  static char *addString(const Vec4 &v,String &s) {
    s.addFormat("{%.4f %.4f %.4f %.4f}",VecM(v,0),VecM(v,1),VecM(v,2),VecM(v,3));
    return(s.a);
  }

  static char *toString(const Mat44 &m,String &s,bool sepLines=false) {s="";return(addString(m,s,sepLines));}
  static char *addString(const Mat44 &m,String &s,bool sepLines=false) {
    if (sepLines) {
      s.addFormat("\n {{%4.2f %4.2f %4.2f %4.2f}\n  {%4.2f %4.2f %4.2f %4.2f}\n  {%4.2f %4.2f %4.2f %4.2f}\n  {%4.2f %4.2f %4.2f %4.2f}\n}}\n",
        _MatM(0,0),_MatM(0,1),_MatM(0,2),_MatM(0,3),
        _MatM(1,0),_MatM(1,1),_MatM(1,2),_MatM(1,3),
        _MatM(2,0),_MatM(2,1),_MatM(2,2),_MatM(2,3),
        _MatM(3,0),_MatM(3,1),_MatM(3,2),_MatM(3,3));
    }
    else {
      s.addFormat("{{%.2f %.2f %.2f %.2f} {%.2f %.2f %.2f %.2f} {%.2f %.2f %.2f %.2f} {%.2f %.2f %.2f %.2f}}",
        _MatM(0,0),_MatM(0,1),_MatM(0,2),_MatM(0,3),
        _MatM(1,0),_MatM(1,1),_MatM(1,2),_MatM(1,3),
        _MatM(2,0),_MatM(2,1),_MatM(2,2),_MatM(2,3),
        _MatM(3,0),_MatM(3,1),_MatM(3,2),_MatM(3,3));
    }
    return(s.a);
  }
};









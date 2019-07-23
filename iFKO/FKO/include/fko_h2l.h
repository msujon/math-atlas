#ifndef FKO_H2L_H
#define FKO_H2L_H

void DoConvert(short dest, short src, short vl);
void DoMove(short dest, short src, short vl);
void DoArrayStore(short ptr, short id, short vl);
void DoArrayLoad(short id, short ptri, short vl);
void HandlePtrArith(short dest, short src0, char op, short src1);
void DoArith(short dest, short src0, char op, short src1, short vl);
void DoReturn(short rret);
void DoLabel(char *name);
void DoComment(char *str);
void DoEmptyReturn();
void DoGoto(char *name);
void DoVecInit(short vid, struct slist *elem, short vl);
void DoReduce(short sid, short vid, char op, short iconst, short vl);
void DoArrayBroadcast(short id, short ptr, short vl);
void DoArrayPrefetch(short lvl, short ptrderef, int wpf);
short AddArrayDeref(short array, short index, int offset);
struct loopq *DoLoop(short I, short start, short end, short inc,
                     short sst, short send, short sinc);
void DoIf(char op, short id, short avar, char *labnam, short vl);
short AddOpt2dArrayDeref(short base, short hdm, short ldm, int unroll);
void FinishLoop(LOOPQ *lp);
void HandlePtrArithNoSizeofUpate(short dest, short src0, char op, short src1);

#endif

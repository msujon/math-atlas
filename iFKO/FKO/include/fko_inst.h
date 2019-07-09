#ifndef FKO_INST_H
#define FKO_INST_H

enum comp_flag
/*
 * enumerated type for compiler flag, so format is:
 * CMPFLAG, <enum comp_flag>, <info1>, <info2>
 */
{
   CF_REGSAVE,
   CF_REGRESTORE,
   CF_PARASAVE,
   CF_LOOP_INIT,  /* loop# */
   CF_LOOP_BODY,
   CF_LOOP_UPDATE,
   CF_LOOP_PTRUPDATE,
   CF_LOOP_TEST,
   CF_LOOP_END,
   CF_VRED_END,
   CF_SCAL_RES,     /* for Speculative vectorization*/
   CF_SSV_SUPDATE,
   CF_SSV_VUPDATE,
   CF_SSV_VBACKUP,
   CF_SSV_VBACKUP_END,
   CF_SSV_VRECOVERY,
   CF_FORCE_ALIGN
};

/*
 *
 * [r]  = integer register
 * [fr] = fp register
 * [c]  = constant
 * [cc] = condition code
 *
 * Unless otherwise noted, all instructions have format:
 * [r0], [r1], [r2/c2]
 * NOTE: only use 1st 14 bits, 2 most sig used to encode exceptions for
 *       instructions that update more than dest:
 *       00 : normal (dest updated only)
 *       01 : dest & src1 updated
 *       10 : dest & src2 updated
 *       11 : all ops updated
 */
enum inst
{
   UNIMP,                       /* NULL, NULL, NULL */
   NOP,                         /* NULL, NULL, NULL */
   COMMENT,                     /* <string>,NULL,NULL */
   LABEL,                       /* <labname>,NULL,NULL */
   CMPFLAG,                     /* flag,flag,flag compiler info */
/*
 * integer ops of same size as ptrs
 */
   LD,                          /* [r], [ptr], NULL */
   ST,                          /* [ptr], [r], NULL */
   MOV,                         /* [r0], [r1/c] : r0 = r1 */
   OR,                          /* r0 = r1 | r/c */
   AND,                         /* r0 = r1 & r/c */
   ANDCC,                       /* r0 = r1 & r/c */
   XOR,                         /* r0 = r1 ^ r/c */
   NOT,                         /* r0 = ~r0 */
   SHL,                         /* r0 = r1 << r/c */
   SHLCC,                       /* r0 = r1 << r/c */
   SHR,                         /* r0 = r1 >> r/c, set [cc] */
   SAR,                         /* r0 = r1 >> r/c */
   ADD,                         /* r0 = r1 + r/c */
   SUB,                         /* r0 = r1 - r/c */
   SUBCC,                       /* r0 = r1 - r/c */
   MUL,                         /* r0 = r1 * r/c */
   UMUL,                        /* r0 = r1 * r/c, unsigned */
   DIV,                         /* r0 = r1 / r/c */
   UDIV,                      
   CMPAND,                      /* cc0, r1, r2/c : set [cc] based on r1 & r2 */
   CMP,                         /* cc#, r1, r2/c: set [cc] based on r1 - r2 */
   NEG,                         /* [r0], [r1] : r0 = -r1 */
   LEA2,                        /* lea r0, [r1, r2, 2] */ 
   LEA4,                        /* lea r0, [r1, r2, 4] */ 
   LEA8,                        /* lea r0, [r1, r2, 8] */ 
/*   ABS, ; abs commented out because not widely supported */
/*
 * 32-bit integer (64-bit systems only)
 */
   LDS,                         /* [r], [ptr], NULL */
   STS,                         /* [ptr], [r], NULL */
   ORS,                         /* r0 = r1 | r/c */
   XORS,                        /* r0 = r1 ^ r/c */
   NOTS,                        /* r0 = ~r0 */
   SHLS, SHLCCS,                /* r0 = r1 << r/c */
   SHRS, SHRCCS,                /* r0 = r1 >> r/c, set [cc] */
   SARS,                        /* r0 = r1 >> r/c */
   ADDS, ADDCCS,                /* r0 = r1 + r/c */
   SUBS, SUBCCS,                /* r0 = r1 - r/c */
   MULS,                        /* r0 = r1 * r/c */
   UMULS,                       /* r0 = r1 * r/c, unsigned */
   DIVS,                        /* r0 = r1 / r/c */
   UDIVS,
   CMPS,                        /* set [cc](r0) based on r1 - r2 */
   MOVS,                        /* [r0], [r1] : r0 = r1 */
   NEGS,                        /* [r0], [r1] : r0 = -r1 */
   LEAS2,                        /* lea r0, [r1, r2, 2] */ 
   LEAS4,                        /* lea r0, [r1, r2, 4] */ 
   LEAS8,                        /* lea r0, [r1, r2, 8] */ 
   /*ABSS,*/
/*
 * Jump instructions
 */
   JMP,                         /* PCREG, LABEL, 0 */
   JEQ, JNE, JLT, JLE, JGT, JGE,  /* PCREG, cc#, LABEL */
   RET,                         /* NULL,NULL,NULL : return to caller */
   PREFR,                       /* NULL, [ptr] [ilvl] */
   PREFW,                       /* NULL, [ptr] [ilvl] */
/*
 * The stream prefetch instructions have format:
 * [ptr], [len], [ilvl:ist]
 * where [ilvl:ist] is an 8-bit constant, where 4 most sig bits give cache
 * level to fetch to (if leading bit is set, fetch is inclusive, else 
 * exclusive).  The last 4 bits indicate which stream to fetch with.
 * Therefore, up to 7 cache levels and 15 streams allowed.
 */
   PREFRS,                      /* [ptr], [len], [ilvl:ist] */
   PREFWS,                      /* [ptr], [len], [ilvl:ist] */
/*
 * Floating point instructions, single precision
 */
   FZERO,                       /* [fr] : fr = 0.0 */
   FLD,                         /* [fr] [ptr] */
   FST,                         /* [ptr], [fr] */
   FMAC,                        /* fr0 += fr1 * fr2 */
   FMUL,                        /* fr0 = fr1 * fr2 */
   FDIV,                        /* fr0 = fr1 / fr2 */
   FADD,                        /* fr0 = fr1 + fr2 */
   FSUB,                        /* fr0 = fr1 - fr2 */
   FABS,                        /* [fr0], [fr1] : fr0 = abs(fr1) */
   FCMP,                        /* [fr0], [fr1] : set [cc] by (fr0 - fr1) */
   FNEG,                        /* fr0, fr1 : fr0 = -fr1 */
   FMAX,                        /* fr0 = max(fr1,fr2) */
   FMIN,                        /* fr0 = min(fr1,fr2) */
   FMOV,                        /* fr0 = fr1 */
/*
 * Conditional move
 */
   FCMOV1,                        /* fr0 = ireg? fr0 : fr1/mem */
   FCMOV2,                        /* fr0 = ireg? fr1/mem : fr0 */
/*
 * Floating point instructions, double precision
 */
   FZEROD,                      /* [fr] : fr = 0.0 */
   FLDD,                        /* [fr] [ptr] */
   FSTD,                        /* [ptr], [fr] */
   FMACD,                       /* fr0 += fr1 * fr2 */
   FMULD,                       /* fr0 = fr1 * fr2 */
   FDIVD,                       /* fr0 = fr1 / fr2 */
   FADDD,                       /* fr0 = fr1 + fr2 */
   FSUBD,                       /* fr0 = fr1 - fr2 */
   FABSD,                       /* [fr0], [fr1] : fr0 = abs(fr1) */
   FCMPD,                       /* [fr0], [fr1] : set [cc] by (fr0 - fr1) */
   FNEGD,                       /* [fr0], [fr1] : fr0 = -fr1 */
   FMAXD,                       /* fr0 = max(fr1,fr2) */
   FMIND,                       /* fr0 = min(fr1,fr2) */
   FMOVD,                       /* fr0 = fr1 */
/*
 * Conditional Move
 */
   FCMOVD1,                        /* fr0 = ireg? fr0 : fr1 */
   FCMOVD2,                        /* fr0 = ireg? fr1 : fr0 */
/*=============================================================================
 * NOTE: Majedul: I'm redesiging vector LIL, adding support for vectorization
 * for any width. For example,
 *    AVX512: 
 *       float:  32x16, 32x8, 32x4, 32x2 
 *       double: 64x8, 64x4, 64x2 
 *                ZMM   YMM   XMM
 *============================================================================*/
/*
 * special instruction to support FABS and FNEG in x86 using VAND/VXOR  
 * it's ugly but x86 doesn't have andss/xorss inst
 * So, these are hybrid instructions which has both scalar and vector
 * operands
 */
   VFSABS_W16,                  /* fr0 = fr1 & vfr2/mem */
   VFSABS_W8,                   /* fr0 = fr1 & vfr2/mem */
   VFSABS_W4,                   /* fr0 = fr1 & vfr2/mem */
   VFSNEG_W16,                  /* fr0 = fr1 xor vfr2/mem */ 
   VFSNEG_W8,                   /* fr0 = fr1 xor vfr2/mem */ 
   VFSNEG_W4,                   /* fr0 = fr1 xor vfr2/mem */ 
   VDSABS_W8,                   /* fr0 = fr1 & vfr2/mem */
   VDSABS_W4,                   /* fr0 = fr1 & vfr2/mem */
   VDSABS_W2,                   /* fr0 = fr1 & vfr2/mem */
   VDSNEG_W8,                   /* fr0 = fr1 xor vfr2/mem */ 
   VDSNEG_W4,                   /* fr0 = fr1 xor vfr2/mem */ 
   VDSNEG_W2,                   /* fr0 = fr1 xor vfr2/mem */ 
/*
 * Double precision vector instructions
 * [memA] is a vector-aligned mem @ [mem] is any alignment
 */
   VDZERO_W8,               /* [vr0]        : vr0[0:N] = 0.0 */
   VDZERO_W4,               /* [vr0]        : vr0[0:N] = 0.0 */
   VDZERO_W2,               /* [vr0]        : vr0[0:N] = 0.0 */
   VDLD_W8,                 /* [vr0], [memA]  : vr0 = mem */
   VDLD_W4,                 /* [vr0], [memA]  : vr0 = mem */
   VDLD_W2,                 /* [vr0], [memA]  : vr0 = mem */
   VDLDU_W8,                /* [vr0], [memU]  : vr0 = unaligned mem */
   VDLDU_W4,                /* [vr0], [memU]  : vr0 = unaligned mem */
   VDLDU_W2,                /* [vr0], [memU]  : vr0 = unaligned mem */
   VDLDS_W8,                /* [vr0], [mem]  :  vr0[0] = mem; vr0[1] = 0 */
   VDLDS_W4,                /* [vr0], [mem]  :  vr0[0] = mem; vr0[1] = 0 */
   VDLDS_W2,                /* [vr0], [mem]  :  vr0[0] = mem; vr0[1] = 0 */
   VDLDL_W8,                /* [vr], [mem]   : vr[0] = mem; vr[1] = vr[1] */
   VDLDL_W4,                /* [vr], [mem]   : vr[0] = mem; vr[1] = vr[1] */
   VDLDL_W2,                /* [vr], [mem]   : vr[0] = mem; vr[1] = vr[1] */
   VDLDH_W8,                /* [vr], [mem]   : vr[0] = vr[0]; vr[1] = mem */
   VDLDH_W4,                /* [vr], [mem]   : vr[0] = vr[0]; vr[1] = mem */
   VDLDH_W2,                /* [vr], [mem]   : vr[0] = vr[0]; vr[1] = mem */
   VDSTNT_W8,               /* [memA], vr0    : mem = vr0 */
   VDSTNT_W4,               /* [memA], vr0    : mem = vr0 */
   VDSTNT_W2,               /* [memA], vr0    : mem = vr0 */
   VDST_W8,                 /* [memA], vr0    : mem = vr0 */
   VDST_W4,                 /* [memA], vr0    : mem = vr0 */
   VDST_W2,                 /* [memA], vr0    : mem = vr0 */
   VDSTU_W8,                /* [memU], vr0    : unaligned mem = vr0 */
   VDSTU_W4,                /* [memU], vr0    : unaligned mem = vr0 */
   VDSTU_W2,                /* [memU], vr0    : unaligned mem = vr0 */
   VDSTS_W8,                /* [mem], [vr0]  :  mem = vr[0] */   
   VDSTS_W4,                /* [mem], [vr0]  :  mem = vr[0] */   
   VDSTS_W2,                /* [mem], [vr0]  :  mem = vr[0] */   
   VDMOV_W8,                /* [vr0], [vr1]   : vr0 = vr1 */
   VDMOV_W4,                /* [vr0], [vr1]   : vr0 = vr1 */
   VDMOV_W2,                /* [vr0], [vr1]   : vr0 = vr1 */
/*
 * Majedul: made v[i,f,d]movs 3 operands operation too.
 *  1. VDMOVS vr0, r0, vr1  // vr0[0] = r0, vr0[vlen-1: 1] = vr1[vlen-1: 1]
 *  2. VDMOVS r0, vr0, 0    // r0 = vr0[0]  
 */
   VDMOVS_W8,               /* [vr0], [vr1], [vr2] */
   VDMOVS_W4,               /* [vr0], [vr1], [vr2] */
   VDMOVS_W2,               /* [vr0], [vr1], [vr2] */
   VDADD_W8,                /* [vr0], [vr1], [vr2] : vr0 = vr1 + vr2 */
   VDADD_W4,                /* [vr0], [vr1], [vr2] : vr0 = vr1 + vr2 */
   VDADD_W2,                /* [vr0], [vr1], [vr2] : vr0 = vr1 + vr2 */
   VDSUB_W8,                /* [vr0], [vr1], [vr2] : vr0 = vr1 - vr2 */
   VDSUB_W4,                /* [vr0], [vr1], [vr2] : vr0 = vr1 - vr2 */
   VDSUB_W2,                /* [vr0], [vr1], [vr2] : vr0 = vr1 - vr2 */
   VDMUL_W8,                /* [vr0], [vr1], [vr2] : vr0 = vr1 * vr2 */
   VDMUL_W4,                /* [vr0], [vr1], [vr2] : vr0 = vr1 * vr2 */
   VDMUL_W2,                /* [vr0], [vr1], [vr2] : vr0 = vr1 * vr2 */
   VDDIV_W8,                /* [vr0], [vr1], [vr2] : vr0 = vr1 * vr2 */ 
   VDDIV_W4,                /* [vr0], [vr1], [vr2] : vr0 = vr1 * vr2 */ 
   VDDIV_W2,                /* [vr0], [vr1], [vr2] : vr0 = vr1 * vr2 */ 
   VDMAC_W8,                /* [vr0], [vr1], [vr2] : vr0 += vr1 * vr2 */
   VDMAC_W4,                /* [vr0], [vr1], [vr2] : vr0 += vr1 * vr2 */
   VDMAC_W2,                /* [vr0], [vr1], [vr2] : vr0 += vr1 * vr2 */
   VDABS_W8,                /* [vr0], [vr1] : vr0 = abs(vr1) */
   VDABS_W4,                /* [vr0], [vr1] : vr0 = abs(vr1) */
   VDABS_W2,                /* [vr0], [vr1] : vr0 = abs(vr1) */
   VDNEG_W8,                /* [vr0], [vr1] : vr0 = -(vr1) */
   VDNEG_W4,                /* [vr0], [vr1] : vr0 = -(vr1) */
   VDNEG_W2,                /* [vr0], [vr1] : vr0 = -(vr1) */
   VDMAX_W8,                /* vr0 = max(vr1,vr2) */
   VDMAX_W4,                /* vr0 = max(vr1,vr2) */
   VDMAX_W2,                /* vr0 = max(vr1,vr2) */
   VDMIN_W8,                /* vr0 = min(vr1,vr2) */
   VDMIN_W4,                /* vr0 = min(vr1,vr2) */
   VDMIN_W2,                /* vr0 = min(vr1,vr2) */
   VDHADD_W8,               /* vr0 = HADD(vr1), HADD(vr2)*/
   VDHADD_W4,               /* vr0 = HADD(vr1), HADD(vr2)*/
   VDHADD_W2,               /* vr0 = HADD(vr1), HADD(vr2)*/
/*
 * Majedul: intorducing special load instruction with broadcast ability
 * NOTE: it will only be used to load data. repeatble optimizations skip it
 * it doesn't have corresponding move instruction to optimize
 */
   VDLDSB_W8,               /* vr, mem : vr[0] = vr[1] = ...= [mem] */
   VDLDSB_W4,               /* vr, mem : vr[0] = vr[1] = ...= [mem] */
   VDLDSB_W2,               /* vr, mem : vr[0] = vr[1] = ...= [mem] */
   
   VDCMOV1_W8,              /* vr0 = vr2? vr0 : vr1 */ 
   VDCMOV1_W4,              /* vr0 = vr2? vr0 : vr1 */ 
   VDCMOV1_W2,              /* vr0 = vr2? vr0 : vr1 */ 
   VDCMOV2_W8,              /* vr0 = vr2? vr1 : vr0 */ 
   VDCMOV2_W4,              /* vr0 = vr2? vr1 : vr0 */ 
   VDCMOV2_W2,              /* vr0 = vr2? vr1 : vr0 */ 

   VDSHUF_W8,               /* [vr0], [vr1], [int32]; vr0 = shuf(vr1|vr0) */
   VDSHUF_W4,               /* [vr0], [vr1], [int32]; vr0 = shuf(vr1|vr0) */
   VDSHUF_W2,               /* [vr0], [vr1], [int32]; vr0 = shuf(vr1|vr0) */
          /* [int32] is split into 8 4 bit words; 1st word indicates which */
          /* should reside in vr0[0], 4th in vr0[3];  Words are numbered */
          /* starting in vr0[0], and ending in vr1[N], N=veclen-1 */
   VDHIHALF_W8,             /* vr0~2[1,0]: vr0[0] = vr1[1], vr0[1] = vr2[1] */
   VDHIHALF_W4,             /* vr0~2[1,0]: vr0[0] = vr1[1], vr0[1] = vr2[1] */
   VDHIHALF_W2,             /* vr0~2[1,0]: vr0[0] = vr1[1], vr0[1] = vr2[1] */
   VDLOHALF_W8,             /* vr0~2[1,0]: vr0[0] = vr1[0], vr0[1] = vr2[0] */
   VDLOHALF_W4,             /* vr0~2[1,0]: vr0[0] = vr1[0], vr0[1] = vr2[0] */
   VDLOHALF_W2,             /* vr0~2[1,0]: vr0[0] = vr1[0], vr0[1] = vr2[0] */
   VDMOVHALF_W8,            /* [vr0], [vr1], imm */
   VDMOVHALF_W4,            /* [vr0], [vr1], imm */
   VDMOVHALF_W2,            /* [vr0], [vr1], imm */
/*
 * Single precision vector instructions
 * [memA] is a vector-aligned mem @ [mem] is any alignment
 */
   VFZERO_W16,              /* [vr0]        : vr0[0:N] = 0.0 */
   VFZERO_W8,               /* [vr0]        : vr0[0:N] = 0.0 */
   VFZERO_W4,               /* [vr0]        : vr0[0:N] = 0.0 */
   VFLD_W16,                /* [vr0], [memA]  : vr0 = mem */
   VFLD_W8,                 /* [vr0], [memA]  : vr0 = mem */
   VFLD_W4,                 /* [vr0], [memA]  : vr0 = mem */
   VFLDU_W16,               /* [vr0], [memU]  : vr0 = unaligned mem */
   VFLDU_W8,                /* [vr0], [memU]  : vr0 = unaligned mem */
   VFLDU_W4,                /* [vr0], [memU]  : vr0 = unaligned mem */
   VFLDS_W16,               /* [vr0], [mem]  :  vr0[0] = mem; vr0[1] = 0 */
   VFLDS_W8,                /* [vr0], [mem]  :  vr0[0] = mem; vr0[1] = 0 */
   VFLDS_W4,                /* [vr0], [mem]  :  vr0[0] = mem; vr0[1] = 0 */
   VFLDL_W16,               /* [vr], [mem]   : vr[0] = mem; vr[1] = vr[1] */
   VFLDL_W8,                /* [vr], [mem]   : vr[0] = mem; vr[1] = vr[1] */
   VFLDL_W4,                /* [vr], [mem]   : vr[0] = mem; vr[1] = vr[1] */
   VFLDH_W16,               /* [vr], [mem]   : vr[0] = vr[0]; vr[1] = mem */
   VFLDH_W8,                /* [vr], [mem]   : vr[0] = vr[0]; vr[1] = mem */
   VFLDH_W4,                /* [vr], [mem]   : vr[0] = vr[0]; vr[1] = mem */
   VFSTNT_W16,              /* [memA], vr0    : mem = vr0; no cache read */
   VFSTNT_W8,               /* [memA], vr0    : mem = vr0; no cache read */
   VFSTNT_W4,               /* [memA], vr0    : mem = vr0; no cache read */
   VFST_W16,                /* [memA], vr0    : mem = vr0 */
   VFST_W8,                 /* [memA], vr0    : mem = vr0 */
   VFST_W4,                 /* [memA], vr0    : mem = vr0 */
   VFSTU_W16,               /* [memU], vr0    : unaligned mem = vr0 */
   VFSTU_W8,                /* [memU], vr0    : unaligned mem = vr0 */
   VFSTU_W4,                /* [memU], vr0    : unaligned mem = vr0 */
   VFSTS_W16,               /* [mem], [vr0]  :  mem = vr[0] */
   VFSTS_W8,                /* [mem], [vr0]  :  mem = vr[0] */
   VFSTS_W4,                /* [mem], [vr0]  :  mem = vr[0] */
   VFMOV_W16,               /* [vr0], [vr1]   : vr0 = vr1 */
   VFMOV_W8,                /* [vr0], [vr1]   : vr0 = vr1 */
   VFMOV_W4,                /* [vr0], [vr1]   : vr0 = vr1 */
/*
 * Majedul: made v[i,f,d]movs 3 operands operation too.
 *  1. VFMOVS vr0, r0, vr1  // vr0[0] = r0, vr0[vlen-1: 1] = vr1[vlen-1: 1]
 *  2. VFMOVS r0, vr0, 0    // r0 = vr0[0]  
 */
   VFMOVS_W16,              /* [vr0], [vr1], [vr2] */
   VFMOVS_W8,               /* [vr0], [vr1], [vr2] */
   VFMOVS_W4,               /* [vr0], [vr1], [vr2] */
   VFADD_W16,               /* [vr0], [vr1], [vr2] : vr0 = vr1 + vr2 */
   VFADD_W8,                /* [vr0], [vr1], [vr2] : vr0 = vr1 + vr2 */
   VFADD_W4,                /* [vr0], [vr1], [vr2] : vr0 = vr1 + vr2 */
   VFSUB_W16,               /* [vr0], [vr1], [vr2] : vr0 = vr1 - vr2 */
   VFSUB_W8,                /* [vr0], [vr1], [vr2] : vr0 = vr1 - vr2 */
   VFSUB_W4,                /* [vr0], [vr1], [vr2] : vr0 = vr1 - vr2 */
   VFMUL_W16,               /* [vr0], [vr1], [vr2] : vr0 = vr1 * vr2 */
   VFMUL_W8,                /* [vr0], [vr1], [vr2] : vr0 = vr1 * vr2 */
   VFMUL_W4,                /* [vr0], [vr1], [vr2] : vr0 = vr1 * vr2 */
   VFDIV_W16,               /* [vr0], [vr1], [vr2] : vr0 = vr1 * vr2 */
   VFDIV_W8,                /* [vr0], [vr1], [vr2] : vr0 = vr1 * vr2 */
   VFDIV_W4,                /* [vr0], [vr1], [vr2] : vr0 = vr1 * vr2 */
   VFMAC_W16,               /* [vr0], [vr1], [vr2] : vr0 += vr1 * vr2 */
   VFMAC_W8,                /* [vr0], [vr1], [vr2] : vr0 += vr1 * vr2 */
   VFMAC_W4,                /* [vr0], [vr1], [vr2] : vr0 += vr1 * vr2 */
   VFABS_W16,               /* [vr0], [vr1] : vr0 = abs(vr1) */
   VFABS_W8,                /* [vr0], [vr1] : vr0 = abs(vr1) */
   VFABS_W4,                /* [vr0], [vr1] : vr0 = abs(vr1) */
   VFNEG_W16,               /* [vr0], [vr1] : vr0 = -(vr1) */
   VFNEG_W8,                /* [vr0], [vr1] : vr0 = -(vr1) */
   VFNEG_W4,                /* [vr0], [vr1] : vr0 = -(vr1) */
   VFMAX_W16,               /* vr0 = max(vr1,vr2) */
   VFMAX_W8,                /* vr0 = max(vr1,vr2) */
   VFMAX_W4,                /* vr0 = max(vr1,vr2) */
   VFMIN_W16,               /* vr0 = min(vr1,vr2) */
   VFMIN_W8,                /* vr0 = min(vr1,vr2) */
   VFMIN_W4,                /* vr0 = min(vr1,vr2) */
   VFHADD_W16,              /* vr0 = HADD(vr1), HADD(vr2)*/
   VFHADD_W8,               /* vr0 = HADD(vr1), HADD(vr2)*/
   VFHADD_W4,               /* vr0 = HADD(vr1), HADD(vr2)*/
/*
 * Majedul: intorducing special load instruction with broadcast ability
 * NOTE: it will only be used to load data. repeatble optimizations skip it
 */
   VFLDSB_W16,              /* vr, mem: vr[0] = vr[1] =...=[mem] */
   VFLDSB_W8,               /* vr, mem: vr[0] = vr[1] =...=[mem] */
   VFLDSB_W4,               /* vr, mem: vr[0] = vr[1] =...=[mem] */
/*
 * Majedul: 
 * NOTE: SSE -> 3 operand, AVX -> 4 operand
 * ---------
 */
   VFCMOV1_W16,             /* vr0 = vr2? vr0 : vr1 */
   VFCMOV1_W8,              /* vr0 = vr2? vr0 : vr1 */
   VFCMOV1_W4,              /* vr0 = vr2? vr0 : vr1 */
   VFCMOV2_W16,             /* vr0 = vr2? vr1 : vr0 */
   VFCMOV2_W8,              /* vr0 = vr2? vr1 : vr0 */
   VFCMOV2_W4,              /* vr0 = vr2? vr1 : vr0 */

   VFSHUF_W16,              /* [vr0], [vr1], [int32]; vr0 = shuf(vr1|vr0) */
   VFSHUF_W8,               /* [vr0], [vr1], [int32]; vr0 = shuf(vr1|vr0) */
   VFSHUF_W4,               /* [vr0], [vr1], [int32]; vr0 = shuf(vr1|vr0) */
          /* [int32] is split into 8 4 bit words; 1st word indicates which */
          /* should reside in vr0[0], 4th in vr0[3];  Words are numbered */
          /* starting in vr0[0], and ending in vr1[N], N=veclen-1 */
   VFHIHALF_W16,            /* vr0~2[1,0]: vr0[0] = vr1[1], vr0[1] = vr2[1] */
   VFHIHALF_W8,             /* vr0~2[1,0]: vr0[0] = vr1[1], vr0[1] = vr2[1] */
   VFHIHALF_W4,             /* vr0~2[1,0]: vr0[0] = vr1[1], vr0[1] = vr2[1] */
   VFLOHALF_W16,            /* vr0~2[1,0]: vr0[0] = vr1[0], vr0[1] = vr2[0] */
   VFLOHALF_W8,             /* vr0~2[1,0]: vr0[0] = vr1[0], vr0[1] = vr2[0] */
   VFLOHALF_W4,             /* vr0~2[1,0]: vr0[0] = vr1[0], vr0[1] = vr2[0] */
   VFMOVHALF_W16,           /* [vr0], [vr1], imm */
   VFMOVHALF_W8,            /* [vr0], [vr1], imm */
   VFMOVHALF_W4,            /* [vr0], [vr1], imm */
/*
 * x86-only instructions
 */
   VGR2VR16,                    /* vreg, ireg, const -- PINSRW */
   FCMPW,    /* freg0, freg1, iconst ; freg0 overwritten with T or F */
   FCMPWD,   /* iconst -- 0 : ==;  1 : < ;  2 : <= */
   CVTBFI,   /* ireg, freg    x86 only movmskps -> bit move (no conversion) */
   CVTBDI,   /* ireg, dreg    x86 only movmskpd */
/*
 * Majedul: some special instruction for Integer. 
 */
#if 0   
   CMOV1,
   CMOV2,
   VCMOV1,
   VCMOV2,
#endif
   CMOV1,
   CMOV2,
/* 32 bit version */   
   VSCMOV1_W16,
   VSCMOV1_W8,
   VSCMOV1_W4,
   VSCMOV2_W16,
   VSCMOV2_W8,
   VSCMOV2_W4,
/* 64 bit version */   
   VICMOV1_W8,
   VICMOV1_W4,
   VICMOV1_W2,
   VICMOV2_W8,
   VICMOV2_W4,
   VICMOV2_W2,
/*
 * Majedul: Vector int inst... V_INT type
 * We consider 2 versions: 32 bit and 64 bit integer
 * NOTE: vector loads are same; so, we use VLD instead of VILD and VSLD
 */
/* 32bit or long word version */   
   /*VSMOV,*/
/*
 * Majedul: made v[i,f,d]movs 3 operands operation too.
 *  1. VSMOVS vr0, r0, vr1  // vr0[0] = r0, vr0[vlen-1: 1] = vr1[vlen-1: 1]
 *  2. VSMOVS r0, vr0, 0    // r0 = vr0[0]  
 */
   VSMOVS_W16,
   VSMOVS_W8,
   VSMOVS_W4,
   /*VSLD,*/
   VSLDS_W16,
   VSLDS_W8,
   VSLDS_W4,
   /*VSST,*/
   VSSTS_W16,
   VSSTS_W8,
   VSSTS_W4,
   VSSHUF_W16,
   VSSHUF_W8,
   VSSHUF_W4,
   VGR2VR32,
   VSADD_W16,
   VSADD_W8,
   VSADD_W4,
   VSSUB_W16,
   VSSUB_W8,
   VSSUB_W4,
   VSMAX_W16,
   VSMAX_W8,
   VSMAX_W4,
   VSMIN_W16,
   VSMIN_W8,
   VSMIN_W4,
   VSZERO_W16,
   VSZERO_W8,
   VSZERO_W4,
/* 64 bit version */
   /*VIMOV,*/
/*
 * Majedul: made v[i,f,d]movs 3 operands operation too.
 *  1. VIMOVS vr0, r0, vr1  // vr0[0] = r0, vr0[vlen-1: 1] = vr1[vlen-1: 1]
 *  2. VIMOVS r0, vr0, 0    // r0 = vr0[0]  
 */
   VIMOVS_W8,
   VIMOVS_W4,
   VIMOVS_W2,
   /*VILD,*/
   VILDS_W8,
   VILDS_W4,
   VILDS_W2,
   /*VIST,*/
   VISTS_W8,
   VISTS_W4,
   VISTS_W2,
   VISHUF_W8,
   VISHUF_W4,
   VISHUF_W2,
   VGR2VR64,
   VIADD_W8,
   VIADD_W4,
   VIADD_W2,
   VISUB_W8,
   VISUB_W4,
   VISUB_W2,
   VIMAX_W8,
   VIMAX_W4,
   VIMAX_W2,
   VIMIN_W8,
   VIMIN_W4,
   VIMIN_W2,
   VIZERO_W8,
   VIZERO_W4,
   VIZERO_W2,
   VICMPWGT_W8,
   VICMPWGT_W4,
   VICMPWGT_W2,
/* common for 32 and 64 bit */  
#if 0
   VMOV,
   VLD,                     /* [vr0], [memA]  : vr0 = mem */
   VLDU,                    /* [vr0], [memA]  : vr0 = unaligned mem */
   VST,
   VSTU,                    /* [memU], vr0    : unaligned mem = vr0 */
#endif
   /* VMOV, */
   VSMOV_W16,
   VSMOV_W8,
   VSMOV_W4,
   VIMOV_W8,
   VIMOV_W4,
   VIMOV_W2,
   /* VLD, */                /* [vr0], [memA]  : vr0 = mem */
   VSLD_W16,               /* [vr0], [memA]  : vr0 = mem */
   VSLD_W8,                /* [vr0], [memA]  : vr0 = mem */
   VSLD_W4,                /* [vr0], [memA]  : vr0 = mem */
   VILD_W8,                /* [vr0], [memA]  : vr0 = mem */
   VILD_W4,                /* [vr0], [memA]  : vr0 = mem */
   VILD_W2,                /* [vr0], [memA]  : vr0 = mem */
   /* VLDU,    */            /* [vr0], [memA]  : vr0 = unaligned mem */
   VSLDU_W16               /* [vr0], [memA]  : vr0 = unaligned mem */
   VSLDU_W8,               /* [vr0], [memA]  : vr0 = unaligned mem */
   VSLDU_W4,               /* [vr0], [memA]  : vr0 = unaligned mem */
   VILDU_W8,               /* [vr0], [memA]  : vr0 = unaligned mem */
   VILDU_W4,               /* [vr0], [memA]  : vr0 = unaligned mem */
   VILDU_W2,               /* [vr0], [memA]  : vr0 = unaligned mem */
   /* VST, */
   VSST_W16,
   VSST_W8,
   VSST_W4,
   VIST_W8,
   VIST_W4,
   VIST_W2,
   /* VSTU,   */              /* [memU], vr0    : unaligned mem = vr0 */
   VSSTU_W16,               /* [memU], vr0    : unaligned mem = vr0 */
   VSSTU_W8,                /* [memU], vr0    : unaligned mem = vr0 */
   VSSTU_W4,                /* [memU], vr0    : unaligned mem = vr0 */
   VISTU_W8,                /* [memU], vr0    : unaligned mem = vr0 */
   VISTU_W4,                /* [memU], vr0    : unaligned mem = vr0 */
   VISTU_W2,                /* [memU], vr0    : unaligned mem = vr0 */
/*
 * special instrcution to update conditional code EFLAGS... 
 */
   BTC,          /* transfer a bit to CF flag:  ICC0, ireg, $position */
/*
 * vector cmp instructions
 */
#if 0   
   VDCMPWEQ,
   VDCMPWNE,
   VDCMPWLT,
   VDCMPWLE,
   VDCMPWNLT,
   VDCMPWNLE,
   VDCMPWGT,
   VDCMPWGE,
   VDCMPWNGT,
   VDCMPWNGE,
   VFCMPWEQ,
   VFCMPWNE,
   VFCMPWLT,
   VFCMPWLE,
   VFCMPWNLT,
   VFCMPWNLE,
   VFCMPWGT,
   VFCMPWGE,
   VFCMPWNGT,
   VFCMPWNGE,
#else
   VDCMPWEQ_W8,
   VDCMPWEQ_W4,
   VDCMPWEQ_W2,
   VDCMPWNE_W8,
   VDCMPWNE_W4,
   VDCMPWNE_W2,
   VDCMPWLT_W8,
   VDCMPWLT_W4,
   VDCMPWLT_W2,
   VDCMPWLE_W8,
   VDCMPWLE_W4,
   VDCMPWLE_W2,
   VDCMPWGT_W8,
   VDCMPWGT_W4,
   VDCMPWGT_W2,
   VDCMPWGE_W8,
   VDCMPWGE_W4,
   VDCMPWGE_W2,
   VFCMPWEQ_W16,
   VFCMPWEQ_W8,
   VFCMPWEQ_W4,
   VFCMPWNE_W16,
   VFCMPWNE_W8,
   VFCMPWNE_W4,
   VFCMPWLT_W16,
   VFCMPWLT_W8,
   VFCMPWLT_W4,
   VFCMPWLE_W16,
   VFCMPWLE_W8,
   VFCMPWLE_W4,
   VFCMPWGT_W16,
   VFCMPWGT_W8,
   VFCMPWGT_W4,
   VFCMPWGE_W16,
   VFCMPWGE_W8,
   VFCMPWGE_W4,
#endif
/*
 * Masks instruction: masks sign bits of vector to integer reg
 */
   VFSBTI_W16,
   VFSBTI_W8,
   VFSBTI_W4,
   VDSBTI_W8,
   VDSBTI_W4,
   VDSBTI_W2,
/*
 * Type conversion instructions
 */
   CVTIS,
   CVTSI,
   CVTFI,
   CVTIF,
   CVTDI,
   CVTID,
   CVTFS,
   CVTSF,
   CVTDS,
   CVTSD,
/*
 * Majedul: this is to convert mask into INT
 */
   CVTMASKFI, 
   CVTMASKDI,
/*
 * Dummy CMP instruction which are needed for ReductionVector translation.
 * I don't find their implementation yet.
 */
#if 0   
   FCMPDWEQ,
   FCMPDWNE,
   FCMPDWLT,
   FCMPDWLE,
   FCMPDWNLT,
   FCMPDWNLE,
   FCMPDWGT,
   FCMPDWGE,
   FCMPDWNGT,
   FCMPDWNGE,
   FCMPWEQ,
   FCMPWNE,
   FCMPWLT,
   FCMPWLE,
   FCMPWNLT,
   FCMPWNLE,
   FCMPWGT,
   FCMPWGE,
   FCMPWNGT,
   FCMPWNGE,
#else
   FCMPDWEQ,
   FCMPDWNE,
   FCMPDWLT,
   FCMPDWLE,
   FCMPDWGT,
   FCMPDWGE,
   FCMPWEQ,
   FCMPWNE,
   FCMPWLT,
   FCMPWLE,
   FCMPWGT,
   FCMPWGE,
#endif
   LAST_INST
};

#ifdef IFKO_DECLARE
char *instmnem[] =
{
   "UNIMP",
   "NOP",
   "COMMENT",
   "LABEL",
   "CMPFLAG",
/*
 * integer ops of same size as ptrs
 */
   "LD",
   "ST",
   "MOV",
   "OR",       /* [OR-NEG] set CC for x86 */
   "AND",
   "ANDCC",
   "XOR",
   "NOT",
   "SHL",
   "SHLCC",
   "SHR",
   "SAR",
   "ADD",
   "SUB",
   "SUBCC",
   "MUL",
   "UMUL",
   "DIV",
   "UDIV",
   "CMPAND",
   "CMP",
   "NEG",
   "LEA2",
   "LEA4",
   "LEA8",
/*   ABS, ; abs commented out because not widely supported */
/*
 * 32-bit integer (64-bit systems only)
 */
   "LDS",
   "STS",
   "MOVS",
   "ORS",
   "XORS",
   "NOTS",
   "SHLS", "SHLCCS",
   "SHRS", "SHRCCS",
   "SARS",
   "ADDS", "ADDCCS",
   "SUBS", "SUBCCS",
   "MULS",
   "UMULS",
   "DIVS",
   "UDIVS",
   "CMPS",
   "NEGS",
   "LEAS2",
   "LEAS4",
   "LEAS8", 
   /*"ABSS",*/
/*
 * Jump instructions
 */
   "JMP",
   "JEQ", "JNE", "JLT", "JLE", "JGT", "JGE",
   "RET",
   "PREFR",
   "PREFW",
/*
 * The stream prefetch instructions have format:
 * [ptr], [len], [ilvl:ist]
 * where [ilvl:ist] is an 8-bit constant, where 4 most sig bits give cache
 * level to fetch to (if leading bit is set, fetch is inclusive, else 
 * exclusive).  The last 4 bits indicate which stream to fetch with.
 * Therefore, up to 7 cache levels and 15 streams allowed.
 */
   "PREFRS",
   "PREFWS",
/*
 * Floating point instructions, single precision
 */
   "FZERO",
   "FLD",
   "FST",
   "FMAC",
   "FMUL",
   "FDIV",
   "FADD",
   "FSUB",
   "FABS",
   "FCMP",
   "FNEG",
   "FMAX",
   "FMIN",
   "FMOV",
   "FCMOV1",
   "FCMOV2",
/*
 * Floating point instructions, double precision
 */
   "FZEROD",
   "FLDD",
   "FSTD",
   "FMACD",
   "FMULD",
   "FDIVD",
   "FADDD",
   "FSUBD",
   "FABSD",
   "FCMPD",
   "FNEGD",
   "FMAXD",
   "FMIND",
   "FMOVD",
   "FCMOVD1",
   "FCMOVD2",
/*
 * special hybrid inst to support fabs and fneg in x86
 */
   "VFSABS_W16",
   "VFSABS_W8",
   "VFSABS_W4",
   "VFSNEG_W16",
   "VFSNEG_W8",
   "VFSNEG_W4",
   "VDSABS_W8",
   "VDSABS_W4",
   "VDSABS_W2",
   "VDSNEG_W8",
   "VDSNEG_W4",
   "VDSNEG_W2",
/*
 * Double precision vector inst
 */
   "VDZERO_W8",
   "VDZERO_W4",
   "VDZERO_W2",
   "VDLD_W8",
   "VDLD_W4",
   "VDLD_W2",
   "VDLDU_W8",
   "VDLDU_W4",
   "VDLDU_W2",
   "VDLDS_W8",
   "VDLDS_W4",
   "VDLDS_W2",
   "VDLDL_W8",
   "VDLDL_W4",
   "VDLDL_W2",
   "VDLDH_W8",
   "VDLDH_W4",
   "VDLDH_W2",
   "VDSTNT_W8",
   "VDSTNT_W4",
   "VDSTNT_W2",
   "VDST_W8",
   "VDST_W4",
   "VDST_W2",
   "VDSTU_W8",
   "VDSTU_W4",
   "VDSTU_W2",
   "VDSTS_W8",
   "VDSTS_W4",
   "VDSTS_W2",
   "VDMOV_W8",
   "VDMOV_W4",
   "VDMOV_W2",
   "VDMOVS_W8",
   "VDMOVS_W4",
   "VDMOVS_W2",
   "VDADD_W8",
   "VDADD_W4",
   "VDADD_W2",
   "VDSUB_W8",
   "VDSUB_W4",
   "VDSUB_W2",
   "VDMUL_W8",
   "VDMUL_W4",
   "VDMUL_W2",
   "VDDIV_W8",
   "VDDIV_W4",
   "VDDIV_W2",
   "VDMAC_W8",
   "VDMAC_W4",
   "VDMAC_W2",
   "VDABS_W8",
   "VDABS_W4",
   "VDABS_W2",
   "VDNEG_W8",
   "VDNEG_W4",
   "VDNEG_W2",
   "VDMAX_W8",
   "VDMAX_W4",
   "VDMAX_W2",
   "VDMIN_W8",
   "VDMIN_W4",
   "VDMIN_W2",
   "VDHADD_W8",
   "VDHADD_W4",
   "VDHADD_W2",
/*
 * Majedul: intorducing special load instruction with broadcast ability
 */
   "VDLDSB_W8",
   "VDLDSB_W4",
   "VDLDSB_W2",
/*
 * blend operation
 */
   "VDCMOV1_W8",
   "VDCMOV1_W4",
   "VDCMOV1_W2",
   "VDCMOV2_W8",
   "VDCMOV2_W4",
   "VDCMOV2_W2",
   "VDSHUF_W8",
   "VDSHUF_W4",
   "VDSHUF_W2",
   "VDHIHALF_W8",
   "VDHIHALF_W4",
   "VDHIHALF_W2",
   "VDLOHALF_W8",
   "VDLOHALF_W4",
   "VDLOHALF_W2",
   "VDMOVHALF_W8",
   "VDMOVHALF_W4",
   "VDMOVHALF_W2",
/*
 * Single precision vector inst
 */
   "VFZERO_W16",
   "VFZERO_W8",
   "VFZERO_W4",
   "VFLD_W16",
   "VFLD_W8",
   "VFLD_W4",
   "VFLDU_W16",
   "VFLDU_W8",
   "VFLDU_W4",
   "VFLDS_W16",
   "VFLDS_W8",
   "VFLDS_W4",
   "VFLDL_W16",
   "VFLDL_W8",
   "VFLDL_W4",
   "VFLDH_W16",
   "VFLDH_W8",
   "VFLDH_W4",
   "VFSTNT_W16",
   "VFSTNT_W8",
   "VFSTNT_W4",
   "VFST_W16",
   "VFST_W8",
   "VFST_W4",
   "VFSTU_W16",
   "VFSTU_W8",
   "VFSTU_W4",
   "VFSTS_W16",
   "VFSTS_W8",
   "VFSTS_W4",
   "VFMOV_W16",
   "VFMOV_W8",
   "VFMOV_W4",
   "VFMOVS_W16",
   "VFMOVS_W8",
   "VFMOVS_W4",
   "VFADD_W16",
   "VFADD_W8",
   "VFADD_W4",
   "VFSUB_W16",
   "VFSUB_W8",
   "VFSUB_W4",
   "VFMUL_W16",
   "VFMUL_W8",
   "VFMUL_W4",
   "VFDIV_W16",
   "VFDIV_W8",
   "VFDIV_W4",
   "VFMAC_W16",
   "VFMAC_W8",
   "VFMAC_W4",
   "VFABS_W16",
   "VFABS_W8",
   "VFABS_W4",
   "VFNEG_W16",
   "VFNEG_W8",
   "VFNEG_W4",
   "VFMAX_W16",
   "VFMAX_W8",
   "VFMAX_W4",
   "VFMIN_W16",
   "VFMIN_W8",
   "VFMIN_W4",
   "VFHADD_W16",
   "VFHADD_W8",
   "VFHADD_W4",
/*
 * Majedul: intorducing special load instruction with broadcast ability
 */
   "VFLDSB_W16",
   "VFLDSB_W8",
   "VFLDSB_W4",
/*
 * blend operation
 */
   "VFCMOV1_W16",
   "VFCMOV1_W8",
   "VFCMOV1_W4",
   "VFCMOV2_W16",
   "VFCMOV2_W8",
   "VFCMOV2_W4",
   "VFSHUF_W16",
   "VFSHUF_W8",
   "VFSHUF_W4",
   "VFHIHALF_W16",
   "VFHIHALF_W8",
   "VFHIHALF_W4",
   "VFLOHALF_W16",
   "VFLOHALF_W8",
   "VFLOHALF_W4",
   "VFMOVHALF_W16",
   "VFMOVHALF_W8",
   "VFMOVHALF_W4",
/*
 * x86-only instructions
 */
   "VGR2VR16",
   "FCMPW",
   "FCMPWD",
   "CVTBFI",
   "CVTBDI",
/*
 * Majedul: some special integer instruction
 */
#if 0   
   "CMOV1",
   "CMOV2",
   "VCMOV1",
   "VCMOV2",
#endif
   "CMOV1",
   "CMOV2",
/* 32 bit version */   
   "VSCMOV1_W16",
   "VSCMOV1_W8",
   "VSCMOV1_W4",
   "VSCMOV2_W16",
   "VSCMOV2_W8",
   "VSCMOV2_W4",
/* 64 bit version */   
   "VICMOV1_W8",
   "VICMOV1_W4",
   "VICMOV1_W2",
   "VICMOV2_W8",
   "VICMOV2_W4",
   "VICMOV2_W2",
/*
 * Majedul: instruction for V_INT
 */
/* 32bit or long word version */   
   "VSMOVS_W16",
   "VSMOVS_W8",
   "VSMOVS_W4",
   /*"VSLD",*/
   "VSLDS_W16",
   "VSLDS_W8",
   "VSLDS_W4",
   /*"VSST",*/
   "VSSTS_W16",
   "VSSTS_W8",
   "VSSTS_W4",
   "VSSHUF_W16",
   "VSSHUF_W8",
   "VSSHUF_W4",
   "VGR2VR32",
   "VSADD_W16",
   "VSADD_W8",
   "VSADD_W4",
   "VSSUB_W16",
   "VSSUB_W8",
   "VSSUB_W4",
   "VSMAX_W16",
   "VSMAX_W8",
   "VSMAX_W4",
   "VSMIN_W16",
   "VSMIN_W8",
   "VSMIN_W4",
   "VSZERO_W16",
   "VSZERO_W8",
   "VSZERO_W4",
/* 64 bit version */
   /*"VIMOV",*/
   "VIMOVS_W8",
   "VIMOVS_W4",
   "VIMOVS_W2",
   /*"VILD",*/
   "VILDS_W8",
   "VILDS_W4",
   "VILDS_W2",
   /*"VIST",*/
   "VISTS_W8",
   "VISTS_W4",
   "VISTS_W2",
   "VISHUF_W8",
   "VISHUF_W4",
   "VISHUF_W2",
   "VGR2VR64",
   "VIADD_W8",
   "VIADD_W4",
   "VIADD_W2",
   "VISUB_W8",
   "VISUB_W4",
   "VISUB_W2",
   "VIMAX_W8",
   "VIMAX_W4",
   "VIMAX_W2",
   "VIMIN_W8",
   "VIMIN_W4",
   "VIMIN_W2",
   "VIZERO_W8",
   "VIZERO_W4",
   "VIZERO_W2",
   "VICMPWGT_W8",
   "VICMPWGT_W4",
   "VICMPWGT_W2",
/* common for 64 and 32 bit */  
#if 0
   "VMOV",
   "VLD",
   "VLDU",
   "VST",
   "VSTU",
#endif
   "VSMOV_W16",
   "VSMOV_W8",
   "VSMOV_W4",
   "VIMOV_W8",
   "VIMOV_W4",
   "VIMOV_W2",
   "VSLD_W16",
   "VSLD_W8",
   "VSLD_W4",
   "VILD_W8",
   "VILD_W4",
   "VILD_W2",
   "VSLDU_W16",
   "VSLDU_W8",
   "VSLDU_W4",
   "VILDU_W8",
   "VILDU_W4",
   "VILDU_W2",
   "VSST_W16",
   "VSST_W8",
   "VSST_W4",
   "VIST_W8",
   "VIST_W4",
   "VIST_W2",
   "VSSTU_W16",
   "VSSTU_W8",
   "VSSTU_W4",
   "VISTU_W8",
   "VISTU_W4",
   "VISTU_W2",
/*
 * special instrcution to update conditional code EFLAGS... 
 */
   "BTC",
/*
 * vector cmp intructions
 */
#if 0   
   "VDCMPWEQ",
   "VDCMPWNE",
   "VDCMPWLT",
   "VDCMPWLE",
   "VDCMPWNLT",
   "VDCMPWNLE",
   "VDCMPWGT",
   "VDCMPWGE",
   "VDCMPWNGT",
   "VDCMPWNGE",
   "VFCMPWEQ",
   "VFCMPWNE",
   "VFCMPWLT",
   "VFCMPWLE",
   "VFCMPWNLT",
   "VFCMPWNLE",
   "VFCMPWGT",
   "VFCMPWGE",
   "VFCMPWNGT",
   "VFCMPWNGE",
#else
   "VDCMPWEQ_W8",
   "VDCMPWEQ_W4",
   "VDCMPWEQ_W2",
   "VDCMPWNE_W8",
   "VDCMPWNE_W4",
   "VDCMPWNE_W2",
   "VDCMPWLT_W8",
   "VDCMPWLT_W4",
   "VDCMPWLT_W2",
   "VDCMPWLE_W8",
   "VDCMPWLE_W4",
   "VDCMPWLE_W2",
   "VDCMPWGT_W8",
   "VDCMPWGT_W4",
   "VDCMPWGT_W2",
   "VDCMPWGE_W8",
   "VDCMPWGE_W4",
   "VDCMPWGE_W2",
   "VFCMPWEQ_W16",
   "VFCMPWEQ_W8",
   "VFCMPWEQ_W4",
   "VFCMPWNE_W16",
   "VFCMPWNE_W8",
   "VFCMPWNE_W4",
   "VFCMPWLT_W16",
   "VFCMPWLT_W8",
   "VFCMPWLT_W4",
   "VFCMPWLE_W16",
   "VFCMPWLE_W8",
   "VFCMPWLE_W4",
   "VFCMPWGT_W16",
   "VFCMPWGT_W8",
   "VFCMPWGT_W4",
   "VFCMPWGE_W16",
   "VFCMPWGE_W8",
   "VFCMPWGE_W4",
#endif
/*
 * Masks instruction: masks sign bits of vector to integer reg
 */
   "VFSBTI_W16",
   "VFSBTI_W8",
   "VFSBTI_W4",
   "VDSBTI_W8",
   "VDSBTI_W4",
   "VDSBTI_W2",
/*
 * Type conversion instructions
 */
   "CVTIS",
   "CVTSI",
   "CVTFI",
   "CVTIF",
   "CVTDI",
   "CVTID",
   "CVTFS",
   "CVTSF",
   "CVTDS",
   "CVTSD",
/*
 * Majedul: experimental... to convert mask from floating point to Int
 */
   "CVTMASKFI",
   "CVTMASKDI",

/*
 * Dummy inst for redundant vector transformation
 */
#if 0   
   "FCMPDWEQ",
   "FCMPDWNE",
   "FCMPDWLT",
   "FCMPDWLE",
   "FCMPDWNLT",
   "FCMPDWNLE",
   "FCMPDWGT",
   "FCMPDWGE",
   "FCMPDWNGT",
   "FCMPDWNGE",
   "FCMPWEQ",
   "FCMPWNE",
   "FCMPWLT",
   "FCMPWLE",
   "FCMPWNLT",
   "FCMPWNLE",
   "FCMPWGT",
   "FCMPWGE",
   "FCMPWNGT",
   "FCMPWNGE",
#else
   "FCMPDWEQ",
   "FCMPDWNE",
   "FCMPDWLT",
   "FCMPDWLE",
   "FCMPDWGT",
   "FCMPDWGE",
   "FCMPWEQ",
   "FCMPWNE",
   "FCMPWLT",
   "FCMPWLE",
   "FCMPWGT",
   "FCMPWGE",
#endif
   "LAST_INST"
};
#else
   extern char *instmnem[];
#endif

#define FIRSTBRANCH JMP
#define LASTBRANCH  RET

#define GET_INST(inst_) ((inst_) & 0x3FFF)
#define ACTIVE_INST(i_) ((i_) != COMMENT && (i_) != CMPFLAG)
#define IS_BRANCH(i_) ((i_) >= FIRSTBRANCH && (i_) <= LASTBRANCH)

/*
 * Majedul: checking conditional branch.
 * assuming, only unconditional branches are JMP and RET and they are set 
 * as 1st and last branches.  
 */

#define IS_COND_BRANCH(i_) ((i_) > FIRSTBRANCH && (i_) < LASTBRANCH)
#if 0
#define IS_LOAD(i_)  ((i_) == LD || (i_) == FLD || (i_) == FLDD || \
                      (i_) == VFLD || (i_) == VDLD || (i_) == LDS || \
                      (i_) == VFLDS || (i_) == VDLDS || \
                      (i_) == VFLDL || (i_) == VFLDH || \
                      (i_) == VDLDL || (i_) == VDLDH || \
                      (i_) == VLD  ||  (i_) == VSLDS || \
                      (i_) == VILDS || (i_) == VFLDU || \
                      (i_) == VDLDU || (i_) == VLDU  || \
                      (i_) == VFLDSB || (i_) == VDLDSB )
#endif
#define IS_LOAD(i_)  ((i_) == LD || (i_) == FLD || (i_) == FLDD || (i_) == LDS \
               (i_) == VFLD_W16  || (i_) ==  VFLD_W8  || (i_) == VFLD_W4 || \
               (i_) == VDLD_W8   || (i_) ==  VDLD_W4  || (i_) == VDLD_W2 || \
               (i_) == VFLDS_W16 || (i_) ==  VFLDS_W8 || (i_) == VFLDS_W4 || \
               (i_) == VDLDS_W8  || (i_) ==  VDLDS_W4 || (i_) == VDLDS_W2 || \
               (i_) == VFLDL_W16 || (i_) ==  VFLDL_W8 || (i_) == VFLDL_W4 || \
               (i_) == VFLDH_W16 || (i_) ==  VFLDH_W8 || (i_) == VFLDH_W4 || \
               (i_) == VDLDL_W8  || (i_) ==  VDLDL_W4 || (i_) == VDLDL_W2 || \
               (i_) == VDLDH_W8  || (i_) ==  VDLDH_W4 || (i_) == VDLDH_W2 || \
               (i_) == VSLD_W16  || (i_) ==  VSLD_W8  || (i_) == VSLD_W4 || \
               (i_) == VILD_W8   || (i_) ==  VILD_W4  || (i_) == VILD_W2 || \
               (i_) == VSLDS_W16  || (i_) ==  VSLDS_W8  || (i_) == VSLDS_W4 || \
               (i_) == VILDS_W8   || (i_) ==  VILDS_W4  || (i_) == VILDS_W2 || \
               (i_) == VFLDU_W16  || (i_) ==  VFLDU_W8  || (i_) == VFLDU_W4 || \
               (i_) == VDLDU_W8   || (i_) ==  VDLDU_W4  || (i_) == VDLDU_W2 || \
               (i_) == VSLDU_W16  || (i_) ==  VSLDU_W8  || (i_) == VSLDU_W4 || \
               (i_) == VILDU_W8   || (i_) ==  VILDU_W4  || (i_) == VILDU_W2 || \
               (i_) == VFLDSB_W16 || (i_) ==  VFLDSB_W8 || (i_) == VFLDSB_W4 || \
               (i_) == VDLDSB_W8  || (i_) ==  VDLDSB_W4 || (i_) == VDLDSB_W2 )

#define IS_UNALIGNED_VLOAD(i_) ((i_) == VFLDU || (i_) == VDLDU || \
                               (i_) == VLDU )
/*
 * Although VFLDSB/VDLDSB loads into vector register, they are essentially 
 * scalar load.
 * NOTE: Should we consider VFLDS/VFSTS as vector or scalar load/store?
 */
#if 0
#define IS_VLOAD(i_)  ((i_) == VFLD || (i_) == VDLD || \
                      (i_) == VFLDS || (i_) == VDLDS || \
                      (i_) == VFLDL || (i_) == VFLDH || \
                      (i_) == VDLDL || (i_) == VDLDH || \
                      (i_) == VLD  ||  (i_) == VSLDS || \
                      (i_) == VILDS || (i_) == VFLDU || \
                      (i_) == VDLDU || (i_) == VLDU )
#endif

#define IS_VLOAD(i_) ((i_) == VFLD_W16 || (i_) ==  VFLD_W8 || (i_) == VFLD_W4 || \
                (i_) == VDLD_W8   || (i_) ==  VDLD_W4  || (i_) == VDLD_W2 || \
                (i_) == VFLDS_W16 || (i_) ==  VFLDS_W8 || (i_) == VFLDS_W4 || \
                (i_) == VDLDS_W8  || (i_) ==  VDLDS_W4 || (i_) == VDLDS_W2 || \
                (i_) == VFLDL_W16 || (i_) ==  VFLDL_W8 || (i_) == VFLDL_W4 || \
                (i_) == VFLDH_W16 || (i_) ==  VFLDH_W8 || (i_) == VFLDH_W4 || \
                (i_) == VDLDL_W8  || (i_) ==  VDLDL_W4 || (i_) == VDLDL_W2 || \
                (i_) == VDLDH_W8  || (i_) ==  VDLDH_W4 || (i_) == VDLDH_W2 || \
                (i_) == VSLD_W16  || (i_) ==  VSLD_W8  || (i_) == VSLD_W4 || \
                (i_) == VILD_W8   || (i_) ==  VILD_W4  || (i_) == VILD_W2 || \
                (i_) == VSLDS_W16 || (i_) ==  VSLDS_W8  || (i_) == VSLDS_W4 || \
                (i_) == VILDS_W8  || (i_) ==  VILDS_W4  || (i_) == VILDS_W2 || \
                (i_) == VFLDU_W16 || (i_) ==  VFLDU_W8  || (i_) == VFLDU_W4 || \
                (i_) == VDLDU_W8  || (i_) ==  VDLDU_W4  || (i_) == VDLDU_W2 || \
                (i_) == VSLDU_W16 || (i_) ==  VSLDU_W8  || (i_) == VSLDU_W4 || \
                (i_) == VILDU_W8  || (i_) ==  VILDU_W4  || (i_) == VILDU_W2 )
#if 0
#define IS_MOVE(i_) ((i_) == MOV || (i_) == FMOV || (i_) == FMOVD || \
                     (i_) == VFMOV || (i_) == VDMOV || (i_) == VMOV || \
                     (i_) == VFMOVS || (i_) == VDMOVS || (i_) == VIMOVS ||\
                     (i_) == VSMOVS )
#endif

#define IS_MOVE(i_) ((i_) == MOV || (i_) == FMOV || (i_) == FMOVD || \
               (i_) == VFMOV_W16 || (i_) ==  VFMOV_W8 || (i_) == VFMOV_W4 || \
               (i_) == VDMOV_W8  || (i_) ==  VDMOV_W4 || (i_) == VDMOV_W2 || \
               (i_) == VSMOV_W16  || (i_) ==  VSMOV_W8  || (i_) == VSMOV_W4 || \
               (i_) == VIMOV_W8   || (i_) ==  VIMOV_W4  || (i_) == VIMOV_W2 || \
               (i_) == VFMOVS_W16 || (i_) ==  VFMOVS_W8 || (i_) == VFMOVS_W4 || \
               (i_) == VDMOVS_W8  || (i_) ==  VDMOVS_W4 || (i_) == VDMOVS_W2 || \
               (i_) == VSMOVS_W16 || (i_) ==  VSMOVS_W8  || (i_) == VSMOVS_W4 || \
               (i_) == VIMOVS_W8  || (i_) ==  VIMOVS_W4  || (i_) == VIMOVS_W2 )
#if 0
#define IS_MAC(i_) ((i_) == FMAC || (i_) == FMACD || (i_) == VFMAC || \
                    (i_) == VDMAC)
#endif
#define IS_MAC(i_) ((i_) == FMAC || (i_) == FMACD || \
               (i_) == VFMAC_W16 || (i_) ==  VFMAC_W8 || (i_) == VFMAC_W4 || \
               (i_) == VDMAC_W8  || (i_) ==  VDMAC_W4 || (i_) == VDMAC_W2 ) 

#if 0
#define IS_STORE(i_)  ((i_) == ST || (i_) == FST || (i_) == FSTD || \
                       (i_) == VFST || (i_) == VDST || (i_) == STS || \
                       (i_) == VFSTS || (i_) == VDSTS || \
                       (i_) == VFSTNT || (i_) == VDSTNT || \
                       (i_) == VST || (i_) == VSSTS || \
                       (i_) == VISTS || (i_) == VFSTU || \
                       (i_) == VDSTU || (i_) == VSTU )
#endif
#define IS_STORE(i_)  ((i_) == ST || (i_) == STS || (i_) == FST || (i_) == FSTD || \
                (i_) == VFST_W16 || (i_) ==  VFST_W8 || (i_) == VFST_W4 || \
                (i_) == VDST_W8  || (i_) ==  VDST_W4 || (i_) == VDST_W2 || \
                (i_) == VFSTS_W16 || (i_) ==  VFSTS_W8 || (i_) == VFSTS_W4 || \
                (i_) == VDSTS_W8  || (i_) ==  VDSTS_W4 || (i_) == VDSTS_W2 || \
                (i_) == VFSTNT_W16 || (i_) ==  VFSTNT_W8 || (i_) == VFSTNT_W4 || \
                (i_) == VDSTNT_W8  || (i_) ==  VDSTNT_W4 || (i_) == VDSTNT_W2 || \
                (i_) == VSST_W16  || (i_) ==  VSST_W8  || (i_) == VSST_W4 || \
                (i_) == VIST_W8   || (i_) ==  VIST_W4  || (i_) == VIST_W2 || \
                (i_) == VSSTS_W16 || (i_) ==  VSSTS_W8  || (i_) == VSSTS_W4 || \
                (i_) == VISTS_W8  || (i_) ==  VISTS_W4  || (i_) == VISTS_W2 || \
                (i_) == VFSTU_W16 || (i_) ==  VFSTU_W8 || (i_) == VFSTU_W4 || \
                (i_) == VDSTU_W8  || (i_) ==  VDSTU_W4 || (i_) == VDSTU_W2 || \
                (i_) == VSSTU_W16 || (i_) ==  VSSTU_W8  || (i_) == VSSTU_W4 || \
                (i_) == VISTU_W8  || (i_) ==  VISTU_W4  || (i_) == VISTU_W2 )

#if 0
#define IS_VSTORE(i_)  ((i_) == VFST || (i_) == VDST || \
                       (i_) == VFSTS || (i_) == VDSTS || \
                       (i_) == VFSTNT || (i_) == VDSTNT || \
                       (i_) == VST || (i_) == VSSTS || \
                       (i_) == VISTS || (i_) == VFSTU || \
                       (i_) == VDSTU || (i_) == VSTU )
#endif
#define IS_VSTORE(i_)  ((i_) == VFST_W16 || (i_) ==  VFST_W8 || (i_) == VFST_W4 || \
                (i_) == VDST_W8  || (i_) ==  VDST_W4 || (i_) == VDST_W2 || \
                (i_) == VFSTS_W16 || (i_) ==  VFSTS_W8 || (i_) == VFSTS_W4 || \
                (i_) == VDSTS_W8  || (i_) ==  VDSTS_W4 || (i_) == VDSTS_W2 || \
                (i_) == VFSTNT_W16 || (i_) ==  VFSTNT_W8 || (i_) == VFSTNT_W4 || \
                (i_) == VDSTNT_W8  || (i_) ==  VDSTNT_W4 || (i_) == VDSTNT_W2 || \
                (i_) == VSST_W16  || (i_) ==  VSST_W8  || (i_) == VSST_W4 || \
                (i_) == VIST_W8   || (i_) ==  VIST_W4  || (i_) == VIST_W2 || \
                (i_) == VSSTS_W16 || (i_) ==  VSSTS_W8  || (i_) == VSSTS_W4 || \
                (i_) == VISTS_W8  || (i_) ==  VISTS_W4  || (i_) == VISTS_W2 || \
                (i_) == VFSTU_W16 || (i_) ==  VFSTU_W8 || (i_) == VFSTU_W4 || \
                (i_) == VDSTU_W8  || (i_) ==  VDSTU_W4 || (i_) == VDSTU_W2 || \
                (i_) == VSSTU_W16 || (i_) ==  VSSTU_W8  || (i_) == VSSTU_W4 || \
                (i_) == VISTU_W8  || (i_) ==  VISTU_W4  || (i_) == VISTU_W2 )
#if 0
#define IS_V2SST(i_)  ((i_) == VFSTS || (i_) == VDSTS || \
                       (i_) == VISTS || (i_) == VSSTS)
#endif
#define IS_V2SST(i_)  ((i_) == VFSTS_W16 || (i_) ==  VFSTS_W8 || (i_) == VFSTS_W4 || \
                (i_) == VDSTS_W8  || (i_) ==  VDSTS_W4 || (i_) == VDSTS_W2 || \
                (i_) == VSSTS_W16 || (i_) ==  VSSTS_W8  || (i_) == VSSTS_W4 || \
                (i_) == VISTS_W8  || (i_) ==  VISTS_W4  || (i_) == VISTS_W2 )
#if 0
#define IS_UNALIGNED_VSTORE(i_) ((i_) == VFSTU || (i_) == VDSTU || \
                                 (i_) == VSTU )
#endif
#define IS_UNALIGNED_VSTORE(i_) ((i_) == VFSTU_W16 || (i_) ==  VFSTU_W8 || \
                           (i_) == VFSTU_W4 || \
                (i_) == VDSTU_W8  || (i_) ==  VDSTU_W4 || (i_) == VDSTU_W2 || \
                (i_) == VSSTU_W16 || (i_) ==  VSSTU_W8  || (i_) == VSSTU_W4 || \
                (i_) == VISTU_W8  || (i_) ==  VISTU_W4  || (i_) == VISTU_W2 )
#if 0
#define IS_CMPW(i_)  ((i_) == FCMPDWEQ || (i_) == FCMPDWNE || \
                      (i_) == FCMPDWLT || (i_) == FCMPDWLE || \
                      (i_) == FCMPDWGT || (i_) ==  FCMPDWGE || \
                      (i_) == FCMPWEQ || (i_) ==  FCMPDWNE || \
                      (i_) == FCMPWLT || (i_) ==  FCMPWLE || \
                      (i_) == FCMPWGT || (i_) ==  FCMPWGE || \
                      (i_) == VDCMPWEQ || (i_) == VDCMPWNE || \
                      (i_) == VDCMPWLT || (i_) == VDCMPWLE || \
                      (i_) == VDCMPWGT || (i_) == VDCMPWGE || \
                      (i_) == VFCMPWEQ || (i_) == VFCMPWNE || \
                      (i_) == VFCMPWLT || (i_) == VFCMPWLE || \
                      (i_) == VFCMPWGT || (i_) == VFCMPWGE || \
                      (i_) == VICMPWGT)

                      /*(i_) == FCMPDWNLT || (i_) == FCMPDWNLE || \
                      (i_) == FCMPDWNGT || (i_) == FCMPDWNGE || \
                      (i_) == FCMPWNLT || (i_) == FCMPWNLE || \
                      (i_) == FCMPWNGT || (i_) == FCMPWNGE || \
                      (i_) == VDCMPWNLT || (i_) == VDCMPWNLE || \
                      (i_) == VDCMPWNGT || (i_) == VDCMPWNGE || \
                      (i_) == VFCMPWNLT || (i_) == VFCMPWNLE || \
                      (i_) == VFCMPWNGT || (i_) == VFCMPWNGE)*/
#endif
#define IS_CMPW(i_)  ((i_) == FCMPDWEQ || (i_) == FCMPDWNE || \
                      (i_) == FCMPDWLT || (i_) == FCMPDWLE || \
                      (i_) == FCMPDWGT || (i_) ==  FCMPDWGE || \
                      (i_) == FCMPWEQ || (i_) ==  FCMPDWNE || \
                      (i_) == FCMPWLT || (i_) ==  FCMPWLE || \
                      (i_) == FCMPWGT || (i_) ==  FCMPWGE || \
          (i_) == VDCMPWEQ_W8 || (i_) == VDCMPWEQ_W4 || (i_) == VDCMPWEQ_W2 || \
          (i_) == VDCMPWNE_W8 || (i_) == VDCMPWNE_W4 || (i_) == VDCMPWNE_W2 || \
          (i_) == VDCMPWLT_W8 || (i_) == VDCMPWLT_W4 || (i_) == VDCMPWLT_W2 || \
          (i_) == VDCMPWLE_W8 || (i_) == VDCMPWLE_W4 || (i_) == VDCMPWLE_W2 || \
          (i_) == VDCMPWGT_W8 || (i_) == VDCMPWGT_W4 || (i_) == VDCMPWGT_W2 || \
          (i_) == VDCMPWGE_W8 || (i_) == VDCMPWGE_W4 || (i_) == VDCMPWGE_W2 || \
          (i_) == VFCMPWEQ_W16 || (i_) == VFCMPWEQ_W8 || (i_) == VFCMPWEQ_W4 || \
          (i_) == VFCMPWNE_W16 || (i_) == VFCMPWNE_W8 || (i_) == VFCMPWNE_W4 || \
          (i_) == VFCMPWLT_W16 || (i_) == VFCMPWLT_W8 || (i_) == VFCMPWLT_W4 || \
          (i_) == VFCMPWLE_W16 || (i_) == VFCMPWLE_W8 || (i_) == VFCMPWLE_W4 || \
          (i_) == VFCMPWGT_W16 || (i_) == VFCMPWGT_W8 || (i_) == VFCMPWGT_W4 || \
          (i_) == VFCMPWGE_W16 || (i_) == VFCMPWGE_W8 || (i_) == VFCMPWGE_W4 || \
          (i_) == VICMPWGT_W8 || (i_) == VICMPWGT_W4 || (i_) == VICMPWGT_W2 )

#define IS_CMP(i_) ((i_) == CMP || (i_) == CMPAND || (i_) == CMPS || \
                    (i_) == FCMP || (i_) == FCMPD || \
                    IS_CMPW(i_))
/*      
                    (i_) == VFCMP || \
                    (i_) == VDCMP || \
                    (i_) == CFTBFI || (i_) == CFTBDI || (i_) == FCMPWD)
*/
                     
#define IS_IOPCC(i_) ((i_) == ANDCC || (i_) == SUBCC || (i_) == ANDCC)
/*
 * NOTE: blend operation is different with respect to mem use
 */
#if 0
#define IS_NOMEM(i_) ((i_) == MUL || (i_) == UMUL || (i_) == DIV || \
                      (i_) == UDIV || (i_) == FCMOV1 || (i_) == FCMOV2 || \
                      (i_) == FCMOVD1 || (i_) == FCMOVD2 || (i_) == VFCMOV1 || \
                      (i_) == VFCMOV2 || (i_) == VDCMOV1 || (i_) == VDCMOV2 || \
                      (i_) == CMOV1 || (i_) == CMOV2 || (i_) == VSCMOV1 || \
                      (i_) == VSCMOV2 || (i_) == VICMOV1 || (i_) == VICMOV2 || \
                      (i_) == LEA2 || (i_) == LEA4 || (i_) == LEA8 )
#endif
#define IS_NOMEM(i_) ((i_) == MUL || (i_) == UMUL || (i_) == DIV || \
                      (i_) == UDIV || (i_) == FCMOV1 || (i_) == FCMOV2 || \
                      (i_) == FCMOVD1 || (i_) == FCMOVD2 || \
                      (i_) == CMOV1 || (i_) == CMOV2 || \
            (i_) == VFCMOV1_W16 || (i_) == VFCMOV1_W8 || (i_) == VFCMOV1_W4 || \
            (i_) == VFCMOV2_W16 || (i_) == VFCMOV2_W8 || (i_) == VFCMOV2_W4 || \
            (i_) == VDCMOV1_W8 || (i_) == VDCMOV1_W4 || (i_) == VDCMOV1_W2 || \
            (i_) == VDCMOV2_W8 || (i_) == VDCMOV2_W4 || (i_) == VDCMOV2_W2 || \
            (i_) == VSCMOV1_W16 || (i_) == VSCMOV1_W8 || (i_) == VSCMOV1_W4 || \
            (i_) == VSCMOV2_W16 || (i_) == VSCMOV2_W8 || (i_) == VSCMOV2_W4 || \
            (i_) == VICMOV1_W8 || (i_) == VICMOV1_W4 || (i_) == VICMOV1_W2 || \
            (i_) == VICMOV2_W8 || (i_) == VICMOV2_W4 || (i_) == VICMOV2_W2 || \
                      (i_) == LEA2 || (i_) == LEA4 || (i_) == LEA8 )
/*
 * NOTE: FMA4 in AMD can be re ordered, src2 can be mem. need to consider
 * this again while implemting other FMAC like: FMA3 ... 
 */
#define IS_REORD(i_) ((i_) == AND || (i_) == ANDCC || (i_) == XOR || \
                      (i_) == FMUL || (i_) == FADD || \
                      (i_) == FMULD || (i_) == FADDD || \
            (i_) == VFADD_W16 || (i_) ==  VFADD_W8 || (i_) == VFADD_W4 || \
            (i_) == VFMUL_W16 || (i_) ==  VFMUL_W8 || (i_) == VFMUL_W4 || \
            (i_) == VDADD_W8  || (i_) ==  VDADD_W4 || (i_) == VDADD_W2 || \
            (i_) == VDMUL_W8  || (i_) ==  VDMUL_W4 || (i_) == VDMUL_W2 || \
                      IS_MAC(i_) )
/*
 * Majedul: These are the instructions where destination is inherently used as
 * one of the sources. This is due to map 4 operands instruction into our 
 * three operand LIL instruction.
 * NOTE: 1. SHUFFLE and ireg2vreg instructions may keep the destination unchanged
 * at certain positions. So, destination is also implicitly used for them
 *       2. DIV, UDIV, DIVS, UDIVS : handled specially 
 */
#if 0
#define IS_DEST_INUSE_IMPLICITLY(i_) ((i_) == FMAC || (i_) == FMACD || \
                                      (i_) == VFMAC || (i_) == VDMAC || \
                                      (i_) == FCMOV1 || (i_) == FCMOV2 || \
                                      (i_) == FCMOVD1 || (i_) == FCMOVD2 || \
                                      (i_) == VFCMOV1 || (i_) == VFCMOV2 || \
                                      (i_) == VDCMOV1 || (i_) == VDCMOV2 || \
                                      (i_) == CMOV1 || (i_) == CMOV2 || \
                                      (i_) == VSCMOV1 || (i_) == VSCMOV2 || \
                                      (i_) == VICMOV1 || (i_) == VICMOV2 || \
                                      IS_SHUFFLE_OP(i_) || IS_IREG2VREG_OP(i_) )
                                   /*|| IS_MOVS(i_) ) */
#endif
#define IS_DEST_INUSE_IMPLICITLY(i_) ((i_) == FMAC || (i_) == FMACD || \
                                      (i_) == VFMAC || (i_) == VDMAC || \
                                      (i_) == FCMOV1 || (i_) == FCMOV2 || \
                                      (i_) == FCMOVD1 || (i_) == FCMOVD2 || \
                                      (i_) == CMOV1 || (i_) == CMOV2 || \
            (i_) == VFCMOV1_W16 || (i_) == VFCMOV1_W8 || (i_) == VFCMOV1_W4 || \
            (i_) == VFCMOV2_W16 || (i_) == VFCMOV2_W8 || (i_) == VFCMOV2_W4 || \
            (i_) == VDCMOV1_W8 || (i_) == VDCMOV1_W4 || (i_) == VDCMOV1_W2 || \
            (i_) == VDCMOV2_W8 || (i_) == VDCMOV2_W4 || (i_) == VDCMOV2_W2 || \
            (i_) == VSCMOV1_W16 || (i_) == VSCMOV1_W8 || (i_) == VSCMOV1_W4 || \
            (i_) == VSCMOV2_W16 || (i_) == VSCMOV2_W8 || (i_) == VSCMOV2_W4 || \
            (i_) == VICMOV1_W8 || (i_) == VICMOV1_W4 || (i_) == VICMOV1_W2 || \
            (i_) == VICMOV2_W8 || (i_) == VICMOV2_W4 || (i_) == VICMOV2_W2 || \
                              IS_SHUFFLE_OP(i_) || IS_IREG2VREG_OP(i_) )
                                   /*|| IS_MOVS(i_) ) */

#define IS_SELECT_OP(i_) ((i_) == CMOV1   || (i_) == CMOV2  || \
                          (i_) == FCMOV1  || (i_) == FCMOV2 || \
                          (i_) == FCMOVD1 || (i_) == FCMOVD2 )

/*
 * Majedul: 32 bit integer instruction (in X64), may need some special treat
 * since all int registers don't support 32 bit version (like: r8-r15 in X64)
 */
#define IS_SHORT_INT_OP(i_) ( (i_) == LDS    || (i_) == STS || \
                              (i_) == ORS    || (i_) == XORS || \
                              (i_) == NOTS   || (i_) == SHLS || \
                              (i_) == SHLCCS || (i_) == SHRS || \
                              (i_) == SHRCCS || (i_) == SARS || \
                              (i_) == ADDS   || (i_) == ADDCCS || \
                              (i_) == SUBS   || (i_) == SUBCCS || \
                              (i_) == MULS   || (i_) == UMULS || \
                              (i_) == DIVS   || (i_) == UDIVS || \
                              (i_) == CMPS   || (i_) == MOVS || \
                              (i_) == NEGS  )
#if 0
#define IS_SHUFFLE_OP(i_)   ( (i_) == VFSHUF   || (i_) == VDSHUF || \
                              (i_) == VSSHUF   || (i_) == VISHUF )
#endif
#define IS_SHUFFLE_OP(i_) ((i_) == VFSHUF_W16 || (i_) == VFSHUF_W8 || \
                           (i_) == VFSHUF_W4 || \
            (i_) == VDSHUF_W8 || (i_) == VDSHUF_W4 || (i_) == VDSHUF_W2 || \
            (i_) == VSSHUF_W16 || (i_) == VSSHUF_W8 || (i_) == VSSHUF_W4 || \
            (i_) == VISHUF_W8 || (i_) == VISHUF_W4 || (i_) == VISHUF_W2 )
      

#define IS_IREG2VREG_OP(i_) ( (i_) == VGR2VR16 || (i_) == VGR2VR32 || \
                              (i_) == VGR2VR64 )

/*#define IS_MOVS(i_) ( (i_) == VDMOVS || (i_) == VFMOVS )*/

#define IS_PREF(i_) ( (i_) == PREFR || (i_) == PREFW )


INSTQ *NewInst(BBLOCK *myblk, INSTQ *prev, INSTQ *next, enum inst ins,
               short dest, short src1, short src2);
INSTQ *InsNewInst(BBLOCK *myblk, INSTQ *prev, INSTQ *next, enum inst ins,
                  short dest, short src1, short src2);
INSTQ *InsNewInstAfterLabel(BBLOCK *blk, enum inst ins,
                            short dest, short src1, short src2);
void InsInstInBlockList(BLIST *blist, int FIRST, enum inst ins,
                        short dest, short src1, short src2);
INSTQ *DelInst(INSTQ *del);
INSTQ *RemoveInstFromQ(INSTQ *del);
INSTQ *KillThisInst(INSTQ *kp);
void KillAllInst(INSTQ *base);
char *op2str(short op);
void WriteLILToBinFile(char *fname, BBLOCK *bbase);
void ReadLILFromBinFile(char *fname);
void PrintInst(FILE *fpout, BBLOCK *bbase);
void PrintThisInst(FILE *fpout, int i, INSTQ *ip);
void PrintThisInstQ(FILE *fpout, INSTQ *ip);
INSTQ *InsertInstBeforeQEntry(INSTQ *list, INSTQ *add);
int FindInstNum(BBLOCK *blk, INSTQ *inst);
INSTQ *FindFirstLILforHIL(INSTQ *ipX);
#endif

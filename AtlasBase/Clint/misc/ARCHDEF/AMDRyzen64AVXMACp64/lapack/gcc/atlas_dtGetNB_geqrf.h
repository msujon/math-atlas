#ifndef ATL_dtGetNB_geqrf

/*
 * NB selection for GEQRF: Side='RIGHT', Uplo='UPPER'
 * M : 25,180,336,375,414,453,492,531,570,609,648,725,803,881,959,1037,1115,1193,1271,1894,2518,2829,3141,3453,3765,4388,5012,5635,6259,6570,6882,7194,7506,8753,9376,9688,10000
 * N : 25,180,336,375,414,453,492,531,570,609,648,725,803,881,959,1037,1115,1193,1271,1894,2518,2829,3141,3453,3765,4388,5012,5635,6259,6570,6882,7194,7506,8753,9376,9688,10000
 * NB : 1,1,11,27,75,78,91,92,95,96,99,99,100,100,107,107,108,112,115,115,131,132,133,133,139,147,163,163,164,164,166,166,167,163,161,177,195
 */
#define ATL_dtGetNB_geqrf(n_, nb_) \
{ \
   if ((n_) < 258) (nb_) = 1; \
   else if ((n_) < 355) (nb_) = 11; \
   else if ((n_) < 394) (nb_) = 27; \
   else if ((n_) < 433) (nb_) = 75; \
   else if ((n_) < 472) (nb_) = 78; \
   else if ((n_) < 511) (nb_) = 91; \
   else if ((n_) < 550) (nb_) = 92; \
   else if ((n_) < 589) (nb_) = 95; \
   else if ((n_) < 628) (nb_) = 96; \
   else if ((n_) < 764) (nb_) = 99; \
   else if ((n_) < 920) (nb_) = 100; \
   else if ((n_) < 1076) (nb_) = 107; \
   else if ((n_) < 1154) (nb_) = 108; \
   else if ((n_) < 1232) (nb_) = 112; \
   else if ((n_) < 2206) (nb_) = 115; \
   else if ((n_) < 2673) (nb_) = 131; \
   else if ((n_) < 2985) (nb_) = 132; \
   else if ((n_) < 3609) (nb_) = 133; \
   else if ((n_) < 4076) (nb_) = 139; \
   else if ((n_) < 4700) (nb_) = 147; \
   else if ((n_) < 5947) (nb_) = 163; \
   else if ((n_) < 6726) (nb_) = 164; \
   else if ((n_) < 7350) (nb_) = 166; \
   else if ((n_) < 8129) (nb_) = 167; \
   else if ((n_) < 9064) (nb_) = 163; \
   else if ((n_) < 9532) (nb_) = 161; \
   else if ((n_) < 9844) (nb_) = 177; \
   else (nb_) = 195; \
}


#endif    /* end ifndef ATL_dtGetNB_geqrf */
#ifndef ATL_dGetNB_geqrf

/*
 * NB selection for GEQRF: Side='RIGHT', Uplo='UPPER'
 * M : 25,88,152,184,216,248,280,312,344,376,408,440,472,504,536,664,792,920,1048,1176,1304,1432,1560,1624,1688,1752,1816,1944,2072,2104,2136,2168,2200,2264,2328,2456,2584,2840,3096,3608,4120
 * N : 25,88,152,184,216,248,280,312,344,376,408,440,472,504,536,664,792,920,1048,1176,1304,1432,1560,1624,1688,1752,1816,1944,2072,2104,2136,2168,2200,2264,2328,2456,2584,2840,3096,3608,4120
 * NB : 1,1,11,11,12,12,15,16,17,18,21,21,22,22,23,23,24,24,27,27,28,28,29,29,30,30,33,33,34,42,54,54,66,78,82,90,114,138,162,168,195
 */
#define ATL_dGetNB_geqrf(n_, nb_) \
{ \
   if ((n_) < 120) (nb_) = 1; \
   else if ((n_) < 200) (nb_) = 11; \
   else if ((n_) < 264) (nb_) = 12; \
   else if ((n_) < 296) (nb_) = 15; \
   else if ((n_) < 328) (nb_) = 16; \
   else if ((n_) < 360) (nb_) = 17; \
   else if ((n_) < 392) (nb_) = 18; \
   else if ((n_) < 456) (nb_) = 21; \
   else if ((n_) < 520) (nb_) = 22; \
   else if ((n_) < 728) (nb_) = 23; \
   else if ((n_) < 984) (nb_) = 24; \
   else if ((n_) < 1240) (nb_) = 27; \
   else if ((n_) < 1496) (nb_) = 28; \
   else if ((n_) < 1656) (nb_) = 29; \
   else if ((n_) < 1784) (nb_) = 30; \
   else if ((n_) < 2008) (nb_) = 33; \
   else if ((n_) < 2088) (nb_) = 34; \
   else if ((n_) < 2120) (nb_) = 42; \
   else if ((n_) < 2184) (nb_) = 54; \
   else if ((n_) < 2232) (nb_) = 66; \
   else if ((n_) < 2296) (nb_) = 78; \
   else if ((n_) < 2392) (nb_) = 82; \
   else if ((n_) < 2520) (nb_) = 90; \
   else if ((n_) < 2712) (nb_) = 114; \
   else if ((n_) < 2968) (nb_) = 138; \
   else if ((n_) < 3352) (nb_) = 162; \
   else if ((n_) < 3864) (nb_) = 168; \
   else (nb_) = 195; \
}


#endif    /* end ifndef ATL_dGetNB_geqrf */
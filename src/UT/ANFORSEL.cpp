/*
CD AR-920127
CH Anforsel                  Legger til eller fjerner anf�rseltegn
CD ===============================================================
CD
CD Eier.......: KARTVERKET / FYSAK-prosjektet
CD Ansvarlig..: �ge Eliassen
CD
CD ===============================================================
*/

#include "stdafx.h"
#include <string.h>

#include "fyut.h"


/*
OJ-2003-12-03
�E-950401
CH UT_PutAnfTegn           Legg inn / fjern anf�rselstegn forran og bak streng
CD ===========================================================================
CD Form�l:
CD Legger inn eller fjerner anf�rselstegn forran eller bak parameter-streng.
CD Ved rydding av anf�rselstegn ryddes ogs� tegn inni strengen.
CD Ved rydding m� parameterstrengen best� av mer enn ett tegn.
CD
CD PARAMETERLISTE:
CD Type   Navn       I/U   Merknad
CD --------------------------------------------------------------------------
CD wchar_t  *pszTx       iu   Tekststreng som skal behandles
CD wchar_t   cTegn       i    Styreparameter:
CD                           "  = Omslutt pszTx med  ' " '
CD                           '  = Omslutt pszTx med  " ' "
CD                           F  = Fjern anf�rselstegn fra strengen
CD short  sMaksLen    i    Makslimal lengde p� streng inklusiv null-termiantor
CD short  sOk         r    Feilstatus. UT_OK hvis ok, UT_ERROR hvis strengen er
CD                         for kort til � f�ye p� anf�rselstegn.
CD
CD Bruk:
CD bOk = UT_PutAnfTegn(pszTx,cTegn, sMaksLen);
CD =============================================================================
*/
SK_EntPnt_UT short UT_PutAnfTegn(wchar_t *pszTx, wchar_t cTegn, size_t sMaksLen)
{
   short i,ii;
   size_t sTxtLen;
   wchar_t t1,t2;
   short sOk = UT_OK;
   
   sTxtLen = wcslen(pszTx);
   /* Hvis det skal legges p� " eller ' i para-strengens ender: */
   if (cTegn==L'\"' || cTegn==L'\'') {

      /* Hvis para-strengen allerede er innrammet av anf�rselstegn :*/
      if ((sTxtLen>1 && pszTx[0]==L'\"' && pszTx[sTxtLen]==L'\"')
         || (sTxtLen>1 && pszTx[0]==L'\'' && pszTx[sTxtLen]==L'\'')) {
         if (sTxtLen <= sMaksLen - 1)
         {
            pszTx[0] = pszTx[sTxtLen] = cTegn;
         }
         else
         {
            sOk = UT_ERROR;
         }
      // Hvis tegnet finnes bare i starten av strengen.
      } else if ((sTxtLen > 1 ) && (pszTx[0]==L'\'' || pszTx[0]==L'\"')){
         if(sTxtLen < sMaksLen - 2)
         {
            pszTx[0] = pszTx[sTxtLen] = cTegn;
            pszTx[sTxtLen + 1] = L'\0';
         }
         else
         {
            sOk = UT_ERROR;
         }
      // Hvis tegnet finnes bare i slutten av strengen.
      } else if ((sTxtLen > 1 ) && (pszTx[sTxtLen - 1]==L'\'' || pszTx[sTxtLen - 1]==L'\"')){
          if(sTxtLen < sMaksLen - 2)
          {
            t1 = pszTx[0];
            t2 = pszTx[1];
            pszTx[0] = cTegn;
            pszTx[1] = t1;

            i = 2;
            while (pszTx[i] != L'\0') {
               t1 = pszTx[i];
               pszTx[i] = t2;
               t2 = t1;
               i++;
            }
            pszTx[i] = cTegn;
            pszTx[i+1] = L'\0';
         }else{
            sOk = UT_ERROR;
          }
      // Strengen best�r av bare et anf�rselstegn.
      } else if ((sTxtLen == 1 ) && (pszTx[sTxtLen - 1]==L'\'' || pszTx[sTxtLen - 1]==L'\"')){
          if (sTxtLen <= sMaksLen - 2)
          {
             pszTx[0] = pszTx[1] = cTegn;
             pszTx[2] = L'\0';
          }
          else
          {
            sOk = UT_ERROR;
          }

      /* Hvis para-strengen ikke er innrammet av ' eller " :*/
      } else {
          if (sTxtLen <= sMaksLen - 3)
          {
             if (pszTx[0] == L'\0') {         /* ingen tegn, legger inn "" */
                pszTx[0] = pszTx[1] = cTegn;
                pszTx[2] = L'\0';
             } else if (pszTx[1] == L'\0') {  /* ett tegn, legger inn "x" */
                pszTx[1] = pszTx[0];
                pszTx[0] = pszTx[2] = cTegn;
                pszTx[3] = L'\0';
             } else {                      /* flere tegn, legger inn "xx yy" */
                t1 = pszTx[0];
                t2 = pszTx[1];
                pszTx[0] = cTegn;
                pszTx[1] = t1;

                i = 2;
                while (pszTx[i] != L'\0') {
                   t1 = pszTx[i];
                   pszTx[i] = t2;
                   t2 = t1;
                   i++;
                }
                pszTx[i]   = t2;
                pszTx[i+1] = cTegn;
                pszTx[i+2] = L'\0';
             }
          }
          else
          {
             sOk = UT_ERROR;
          }
      }

   /* Fjerner apostroffer: */
   } else if (cTegn==L'F') {
       if (sTxtLen <= sMaksLen)
       {
          /* Para-strengen m� inneholde mer enn ett tegn: */
          if (sTxtLen > 1) {

             /* " */
             if ( pszTx[0]==L'\"' || pszTx[sTxtLen-1]==L'\"') {

                /* Fjerner alle " i hele strengen. */
                i = 0;
                ii = 0;
                do {
                   if (pszTx[i] != L'\"') {
                      pszTx[ii] = pszTx[i];
                      ii++;
                   }
                   i++;
                } while (pszTx[i] != L'\0');
                pszTx[ii] = L'\0';

             /* ' */
             } else if ( pszTx[0]==L'\'' || pszTx[sTxtLen-1]==L'\'') {

                /* Fjerner alle ' i hele strengen. */
                i = 0;
                ii = 0;
                do {
                   if (pszTx[i] != L'\'') {
                      pszTx[ii] = pszTx[i];
                      ii++;
                   }
                   i++;
                } while (pszTx[i] != L'\0');
                pszTx[ii] = L'\0';
             }
          }
       }
       else
       {
           sOk = UT_ERROR;
       }
   }
   return sOk;
}

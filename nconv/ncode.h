/*
 * $Id: ncode.h,v 1.10 1998/08/06 13:17:25 yjlee Exp $
 * ncode version 2.0, interface 2.0
 * code conversion functions for ncode and KSC-5601.
 * Lee yongjae, yjlee@cglab.snu.ac.kr, 1996.9, 1998.7.
 *
 * Copyright 1996,1998 by Lee yongjae, in dept. of Computer Science,
 * Seoul National University, Korea.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, 
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of the copyright holder
 * not be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.
 *
 * This software is provided "as is" without express or implied warranty.
 *
 */

/*
BEGINNING OF DOCUMENT


NCODE (n-byte 3BeulSik hangul code) VERSION 2.0

  ncode is a hangul encoding scheme for 3-BeulSik Hangul input/output. 
  ncode is not purposed to be used as a text file code exposed to end users
  but only to be used as an internal representation of hangul
  for input/output convenience.

  ncode represents ncode version 2.0 and the old versions of ncode is now
  obsoledted to prevent confusion. The history of versions is:

    There was ncode version 1 used before 1996.1.27 which has only
    one fill code for ChoSeong-fill at 0xa0 without f1 nor f2.
    The version is upgraded to version 1.1 which has f1 and f2
    but the location of f1 is 0xa0 and f2 is 0xe4.
    In this version 2.0, f1 and f2 is moved to 0xa1 and 0xb5
    and others are shifted for the reallocation.


NCODE ENCODING

  -------------------------------------------------------------------------
  |        | a1 f1  | a2 K   | a3 Kk  | a4 N   | a5 T   | a6 Tt  | a7 R   |
  | a8 M   | a9 P   | aa Pp  | ab S   | ac Ss  | ad O   | ae C   | af Cc  |
  | b0 Ch  | b1 Kh  | b2 Th  | b3 Ph  | b4 H   | b5 f2  | b6 a   | b7 ae  |
  | b8 ya  | b9 yae | ba eo  | bb e   | bc yeo | bd ye  | be o   | bf wa  |
  | c0 wae | c1 oe  | c2 yo  | c3 u   | c4 weo | c5 we  | c6 wi  | c7 yu  |
  | c8 eu  | c9 yi  | ca i   | cb k   | cc kk  | cd ks  | ce n   | cf nc  |
  | d0 nh  | d1 t   | d2 l   | d3 lk  | d4 lm  | d5 lp  | d6 ls  | d7 lth |
  | d8 lph | d9 lh  | da m   | db p   | dc ps  | dd s   | de ss  | df ng  |
  | e0 c   | e1 ch  | e2 kh  | e3 th  | e4 ph  | e5 h   |        |        | 
  -------------------------------------------------------------------------
  * f1 is ChoSeong-fill code and f2 is JungSeong-fill code.


NCODE SEQUENCES FOR EACH COMBINED HANGUL

  ncode represents a hangul character as 2 or 3 byte. The cases of possible
  combinations are:

    [ch] [ju]      : normal hangul without JongSeong
    [ch] [ju] [jo] : normal hangul with JongSeong
    [ch] [f2]      : ChoSeong only
    [ch] [f2] [jo] : ChoSeong and JongSeong only (no JungSeong)
    [f1] [ju]      : JungSeong only
    [f1] [ju] [jo] : JungSeong and JongSeong only (no ChoSeong)
    [f1] [f2] [jo] : JongSeong only

  where,

    * f1 : ChoSeong-fill  (0xa1)
    * ch : ChoSeong       (0xa2-0xb4)
    * f2 : JungSeong-fill (0xb5)
    * ju : JungSeong      (0xb6-0xca)
    * jo : JongSeong      (0xcb-0xe5)


NCODE CODE CONVERSION MODULE INTERFACE

  ncode code conversion module has several stages of coversion algorithm,
  and there are some untractable reasons to have such shape, so there is
  version number for the function interface (mainly in C) and the current
  interface version number is 2.0. Interface version 2.0 includes all
  interfaces and definitions of version 1.0 for backward compatibility.


  NCODE INTERFACE VERSION 2.1

  define one macro two check hangul-character connectivity of adjacent bytes.

    NCODE_ONE_HCHAR(c0, c1) : checks if c0 and c1 are bytes of one
            hangul character. (not defined in version 2.0)


  NCODE INTERFACE VERSION 2.0

  has only two interface function kston and ntoks.
  These functions are optimized for practical usage, simplity and easy
  to understand. If you need more complicated, sophicated and hard-to-use
  interface, refer ncode interface version 1.0.

  The two module do the simple job:

    kston : reads KSC5601-1987 string (include ASCII)
            and writes out ncode string on the given string buffer.

    ntoks : reads ncode string (include ASCII)
            and writes out KSC5601-1987 string on the given string buffer.

  and the unconvertable codes or some errors from any inconsistency are
  simply ignored. So the user cannot check out which character makes problem
  on code conversion. It's the main simplification of this version compared
  to the earlyer versions. If you want to check out which character makes
  conversion error, just make small string for each input characters and
  go with those modules. If you don't need spetial checking, just go with
  those modules. It's the purpose of these simple interface.

  NCODE INTERFACE VERSION 1.0

  has 8 interface functions and several definitions.
  The interface functions convert only one hangul charater,
  and does not treate ASCII code, and pointer-orientied rather than
  buffer-oriented of interface version 2.0. So it's hard to use but good
  for precise one-to-one character conversion.

  The 8 interfaces are:

    ks_to_n, n_to_ks:       conv. between KSC5601-1987 and ncode
    ks_to_comb, comb_to_ks: conv. between KSC5601-1987 and Combinationary code
    comb_to_n, n_to_comb:   conv. between Combinationary code and ncode
    han_to_n, n_to_hand :   conv. between Macintosh Han code and ncode

  Serveral values are defined as C macro defintions. These are about:

    NCODE_* : ncode value ranges
    KS_* : some KSC5601-1987 code value ranges  
  
  The usages of the interface functions can be seen by example see the
  following module which reads ks codes from stdin and write out ncode
  to stdout:

    int c;
    unsigned short k;
    unsigned char buf[4];
    unsigned char *p1, *p2;

    while ((c = getchar()) != EOF) {
      if (c >= 128) {
        k = (c << 8) | getchar();
        p1 = buf;
        p2 = ks_to_n(k, buf);
        while(p1 < p2)
          putchar(*p1++);
      }
      else
        putchar(c);
    }
  
  As you can see this codes, the simple conversion is not simple, but you
  can do everything on each character.


END OF DOCUMENT
*/


#ifndef _ncode_h_
#define _ncode_h_

/*
 * DEFINITIONS FROM NCODE INTERFACE VERSION 1.0 -------------------------------
 */

#define NCODE_FIRST     0xa1
#define NCODE_F1        0xa1
#define NCODE_CH_FIRST  0xa2
#define NCODE_CH_O      0xad
#define NCODE_CH_LAST   0xb4
#define NCODE_F2        0xb5
#define NCODE_JU_FIRST  0xb6
#define NCODE_JU_LAST   0xca
#define NCODE_JO_FIRST  0xcb
#define NCODE_JO_LAST   0xe5
#define NCODE_LAST      0xe5

#define KS_PARTIAL_FIRST  0xa4a1
#define KS_PARTIAL_LAST   0xa4d3
#define KS_NORMAL_FIRST   0xb0a1 
#define KS_NORMAL_LAST    0xc8fe

/*
 * DEFINITIONS FROM NCODE INTERFACE VERSION 2.1 -------------------------------
 */

#define NCODE_ONE_HCHAR(c0, c1)  \
     ((((c0) >= NCODE_CH_FIRST && (c0) <= NCODE_CH_LAST) &&  \
       ((c1) >= NCODE_JU_FIRST && (c1) <= NCODE_JU_LAST)) || \
      (((c0) >= NCODE_JU_FIRST && (c0) <= NCODE_JU_LAST) &&  \
       ((c1) >= NCODE_JO_FIRST && (c1) <= NCODE_JO_LAST)))

#ifdef __cplusplus
extern "C" { 
#endif

/*
 * NCODE INTERFACE VERSION 1.0 -----------------------------------------------
 */

unsigned char *ks_to_n(/* unsigned short c, unsigned char *buf */);
/*
 * Returns pointer to the next of the charaters consumed.
 * If conversion fails, the return value is buf.
 */

unsigned char *n_to_ks(/* unsigned char *buf, unsigned short *c_ret */);
/*
 * Returns pointer to the next of the charaters consumed.
 * If conversion fails, the return value is buf.
 */

unsigned char *comb_to_n(/* unsigned short c, unsigned char *buf */);
/*
 * Returns pointer to the next of the charaters consumed.
 * If conversion fails, the return value is buf.
 */

unsigned char *n_to_comb(/* unsigned char *buf, unsigned short *c_ret */);
/*
 * Returns pointer to the next of the charaters consumed.
 * If conversion fails, the return value is buf.
 */

unsigned short ks_to_comb(/* unsigned short c */);
/*
 * Returns 0 on conversion faiure.
 */

unsigned short comb_to_ks(/* unsigned short c */);
/*
 * Returns 0 on conversion failure.
 */

unsigned char n_to_han(/* unsigned char han */);
/*
 * On unconvertable input, returns 0
 */

unsigned char han_to_n(/* unsigned char ncode */);
/*
 * On unconvertable input, returns 0
 */

/*
 * NCODE INTERFACE VERSION 2.0 -----------------------------------------------
 */

int kston(/* unsigned char *ks_str, unsigned char *n_str */);
/*
 * Returns the length of n_str.
 * The input string is KSC5601-1987 code (including ASCII) and
 * the output string is ncode2  code (including ASCII).
 * Unconvertable codes from the source string are simply ignored.
 * The caller should allocate sufficient space for the output string buffer,
 * and two times of the input length is sufficent for it. 
 */

int ntoks(/* unsigned char *n_str, unsigned char *ks_str */);
/*
 * Returns the length of ks_str.
 * The input string is ncode2  code (including ASCII) and
 * the output string is KSC5601-1987 code (including ASCII).
 * Unconvertable codes form the source string are simply ignored.
 * The caller should allocate sufficient space for the output string buffer,
 * and the same size as the input length is sufficent for it.
 */

#ifdef __cplusplus
}
#endif

#endif  /* _ncode_h_ */

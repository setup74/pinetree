/*
 * $Id: nconv.c,v 1.2 1998/07/28 08:14:14 yjlee Exp yjlee $
 * nconv - ncode code conversion filter for ncode, comb, ksc, hancode.
 * Lee yongjae, yjlee@cglab.snu.ac.kr, 1998.07.21.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ncode.h"

#define INBUF_SIZE   1000
#define OUTBUF_SIZE  2000

void file_ks2n(/* FILE *in, FILE *out */);
void file_n2ks(/* FILE *in, FILE *out */);
void file_comb2n(/* FILE *in, FILE *out */);
void file_n2comb(/* FILE *in, FILE *out */);
void file_han2n(/* FILE *in, FILE *out */);
void file_n2han(/* FILE *in, FILE *out */);
void file_str2hex(/* FILE *in, FILE *out */);
void file_hex2str(/* FILE *in, FILE *out */);

void
usage()
{
  fprintf(stderr,
"\
nconv - convert code between ncode, KSC5601, combination code and hancode.\n\
usage: nconv [ code_spec ] [ infile [ outfile ] ]\n\
  code_spec:\n\
    -k : KSC5601-1987 to ncode (ks2n)\n\
    -K : ncode to KSC5601-1987 (n2ks)\n\
    -c : Combination code to ncode (comb2n)\n\
    -C : ncode to Combination code (n2comb)\n\
    -m : hancode to ncode (han2n)\n\
    -M : ncode to hancode (n2han)\n\
    -x : string to hexadecimal string, except newline character (str2hex)\n\
    -X : hexadecimal string to string, except newline character (hex2str)\n\
  If the program is called as the name in the parenthesis,\n\
  the code_spec is define automatically. Default code spec is -k.\n\
");
}

int
main(argc, argv)
     int argc;
     char *argv[];
{
  int c, errflg;
  FILE *in, *out;
  void (*file_conv)();
  char name[256], *p;

  file_conv = file_ks2n;
  
  /* check out called name */

  strcpy(name, argv[0]);

  if ((p = strrchr(name, '/')))
    p++;
  else
    p = name;

  if (!strcmp(p, "ks2n")) file_conv = file_ks2n;
  else if (!strcmp(p, "n2ks")) file_conv = file_n2ks;
  else if (!strcmp(p, "comb2n")) file_conv = file_comb2n;
  else if (!strcmp(p, "n2comb")) file_conv = file_n2comb;
  else if (!strcmp(p, "han2n")) file_conv = file_han2n;
  else if (!strcmp(p, "n2han")) file_conv = file_n2han;
  else if (!strcmp(p, "str2hex")) file_conv = file_str2hex;
  else if (!strcmp(p, "hex2str")) file_conv = file_hex2str;

  errflg = 0;
  while ((c = getopt(argc, argv, "kKcCmMxXh")) != -1) {
    switch (c) {
    case 'k': file_conv = file_ks2n; break;
    case 'K': file_conv = file_n2ks; break;
    case 'c': file_conv = file_comb2n; break;
    case 'C': file_conv = file_n2comb; break;
    case 'm': file_conv = file_han2n; break;
    case 'M': file_conv = file_n2han; break;
    case 'h': errflg++; break;
    case '?': errflg++; break;
    }
    if (errflg) {
      usage();
      return -1;
    }
  }
  if (argc - optind > 2) {
    usage();
    return -1;
  }

  in = stdin;
  out = stdout;

  if (argc - optind >= 1) {
    if (strcmp(argv[optind], "-"))
      if (!(in = fopen(argv[optind], "r"))) {
	perror(argv[optind]);
	return -1;
      }
  }

  optind++;

  if (argc - optind == 1) {
    if (strcmp(argv[optind], "-"))
      if (!(out = fopen(argv[optind], "w"))) {
	perror(argv[optind]);
	return -1;
      }
  }

  file_conv(in, out);
}

void
file_ks2n(in, out)
     FILE *in, *out;
{
  unsigned char inbuf[INBUF_SIZE], outbuf[OUTBUF_SIZE];

  while (fgets(inbuf, INBUF_SIZE, in)) {
    kston(inbuf, outbuf);
    fputs(outbuf, out);
  }
}

void
file_n2ks(in, out)
     FILE *in, *out;
{
  unsigned char inbuf[INBUF_SIZE], outbuf[OUTBUF_SIZE];
  int size;

  while (fgets(inbuf, INBUF_SIZE, in)) {
    ntoks(inbuf, outbuf);
    fputs(outbuf, out);
  }
}

void
file_comb2n(in, out)
     FILE *in, *out;
{
  int c;
  unsigned short comb;
  unsigned char *p1, *p2, buf[4];

  while ((c = fgetc(in)) != EOF) {
    if (c >= 128) {
      comb = (c << 8) | fgetc(in);
      p1 = buf;
      p2 = comb_to_n(comb, buf);

      while(p1 < p2)
	fputc(*p1++, out);
    }
    else
      fputc(c, out);
  }  
}

void
file_n2comb(in, out)
     FILE *in, *out;
{
  unsigned char inbuf[INBUF_SIZE], outbuf[OUTBUF_SIZE];
  unsigned char *ip, *op;
  unsigned short comb;

  while (fgets(inbuf, INBUF_SIZE, in)) {
    ip = inbuf;
    op = outbuf;

    while (*ip) {
      if (*ip >= 128) {
	ip = n_to_comb(ip, &comb);
	if (comb) {
	  *op++ = (comb >> 8) & 0xff;
	  *op++ = comb & 0xff;
	}
	else
	  ip++;  /* drop the unconvertable character */
      }
      else
	*op++ = *ip++;
    }

    *op = 0;
    fputs(outbuf, out);
  }
}

void
file_han2n(in, out)
     FILE *in, *out;
{
  int c;

  while ((c = fgetc(in)) != EOF) {
    if (c >= 128) {
      if ((c = han_to_n(c)))
	fputc(c, out);
    }
    else
      fputc(c, out);
  }    
}

void
file_n2han(in, out)
     FILE *in, *out;
{
  int c;

  while ((c = fgetc(in)) != EOF) {
    if (c >= 128) {
      if ((c = n_to_han(c)))
	fputc(c, out);
    }
    else
      fputc(c, out);
  }    
}

void
file_str2hex(in, out)
     FILE *in, *out;
{
  int c;
  
  while ((c = fgetc(in)) != EOF) {
    if (c == '\n')
      fputc(c, out);
    else
      fprintf(out, "%02x", c);
  } 
}

void
file_hex2str(in, out)
     FILE *in, *out;
{
  char buf[3];
  int c, x;

  buf[2] = 0;

  while ((c = fgetc(in)) != EOF) {
    if (c == '\n')
      fputc(c, out);
    else {
      buf[0] = c;
      buf[1] = fgetc(in);
      sscanf(buf, "%x", &x);
      fputc(x, out);
    }
  }
}

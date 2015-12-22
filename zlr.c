#include <openssl/md5.h>
#include <openssl/blowfish.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

const unsigned char* state[4] = {
  "::__:SAVEBUFF:__::",
  ":*",
  "buffextras",
  "**!znc@znc"
};

const unsigned char st_trans[4] = {
  1, 4, 5, 5
};

int main(int argc, char** argv) {
  if(argc != 2) {
    printf("ZNC LogRipper\n");
    printf("Takes password as an argument. Input as stdin.\n");
    printf("Output as stdout.\n");
    printf("EXAMPLE: %s password < encryptedfile > outputfile\n", argv[0]);
    return;
  }
  BF_KEY key;
  int ix = 0; // index of encrypted data for blowfish encrypt
  int is = 0; // index of stream
  int il = 0; // index of line
  int nr = 1; // number of characters read
  const unsigned char* pass = argv[1];
  unsigned char md[16];
  MD5((const unsigned char *)pass, 10, md);
  unsigned char* in = (unsigned char*) calloc(sizeof(unsigned char), 1024);
  unsigned char* ou = (unsigned char*) calloc(sizeof(unsigned char), 1024);
  unsigned char* ln = (unsigned char*) calloc(sizeof(unsigned char), 1024);
  unsigned char* iv = (unsigned char*) calloc(sizeof(unsigned char), 8);
  unsigned char* pt = NULL;
  unsigned char  sa = 0; // string we are looking for (state a)
  unsigned char  sb = 0; // char we are looking for   (state b)
  BF_set_key(&key, 16, md);
  while (nr) {
    nr = read(STDIN_FILENO, in, 1024);
    BF_cfb64_encrypt(in, ou, 1024, &key, iv, &ix, 0);
    for (is = 0; is < 1024; is++) {
      if(sa < 4 && state[sa][sb] == '\0') {
        write(STDOUT_FILENO, state[sa], strlen(state[sa]));
        sa = st_trans[sa];
        sb = 0;
      }
      if(sa == 4 && sb == 0) {
        if(ou[is] == state[2][0]) {
          sa = 2;
          sb = 1;
        } else if(ou[is] == state[3][0]) {
          sa = 3;
          sb = 1;
        }
      } else if(sa < 4) {
        if(ou[is] == state[sa][sb]) {
          sb = sb + 1;
        } else {
          sa = 0;
          sb = 0;
        }
      } else if (sa == 5) {
        ln[il] = ou[is];
        il++;
        if(ou[is] == '\n') {
          write(STDOUT_FILENO, ln, il);
          sa = 1;
          sb = 0;
          il = 0;
        }
      }
    }
  }
}

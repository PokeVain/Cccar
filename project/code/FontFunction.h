

#ifndef __FontFunction_H_
#define __FontFunction_H_


#define             CharactersNumber            98

typedef struct CheseHZ
{
    const unsigned char Guide[2];
    const unsigned char Code[32];
}__CharactersTypeDef;

extern __CharactersTypeDef Characters[CharactersNumber];

extern const unsigned char LEGO[29970];

extern const unsigned char Laboratory[14][16];

extern const unsigned char Name[12][16];
extern const unsigned char Name_WS[4][16];
extern const unsigned char Name_ZQ[4][16];
extern const unsigned char Name_LY[4][16];
extern const unsigned char Name_ZSY[6][16];
extern const unsigned char Name_JSX[6][16];

extern const unsigned char Glory_One[16][16];
extern const unsigned char Glory_Two[12][16];
extern const unsigned char Glory_Thr[12][16];


#endif



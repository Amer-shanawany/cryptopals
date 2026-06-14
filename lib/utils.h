#ifndef _UTILS_H_
#define _UTILS_H_

#include <bytes.h>

float bytes_rate_etaoin_shrdlu(bytes_t* input);
float bytes_bigram_analysis(bytes_t* input);
unsigned int hamming_distance(bytes_t* first, bytes_t* second);

#endif // _UTILS_H_
#ifndef _BASE64_H_
#define _BASE64_H_

// From http://stackoverflow.com/questions/180947/base64-decode-snippet-in-c

#include <stdint.h>
#include <vector>
#include <string>

std::string base64_encode(uint8_t const* buf, unsigned int bufLen);
std::vector<uint8_t> base64_decode(std::string const&);

#endif
#pragma once
#include <string>
#include <memory>
using namespace std;
enum ResultType
{
  Okey = 0,
  NotFound = -1,
  DublicateKey = -2,
  TableExceeded = -3,
};

struct ResultItem
{
  int Probe = 0;
  ResultType Result = Okey;
};

inline uint32_t fnv1_hash_32(const char *ch, const char *e) {//fnv1 hash
  uint32_t h = 2166136261U;
  for (; ch != e; ++ch)
    h = (h * 16777619) ^ *ch;
  return h;
}
inline size_t get_hash(const string &s) {return fnv1_hash_32(s.data(), s.data()+s.length());}

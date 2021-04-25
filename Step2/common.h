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

inline size_t get_hash(const string &s) {return hash<string>()(s);}



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

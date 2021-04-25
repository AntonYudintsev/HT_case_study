#pragma once
#include "common.h"
struct OA_Simple
{
  static const uint32_t EntryEmpty = 0;
  typedef uint32_t hash_t;
  static hash_t get_hash(const string &key)
  {
    hash_t h = ::get_hash(key);
    return h ? h : 0x8000000;
  }

  unique_ptr<hash_t[]> hashes;
  unique_ptr<string[]> keys;
  unique_ptr<string[]> vals;
  uint32_t Count = 0;
  uint32_t Capacity = 0;

  OA_Simple(uint32_t size)
  {
    Capacity = size; // HtHelper.GetPrime(size);
    keys = make_unique<string[]>(Capacity);
    vals = make_unique<string[]>(Capacity);
    hashes = make_unique<uint32_t[]>(Capacity);
  }
  void Clear()
  {
      for (uint32_t i = 0; i < Capacity; i++)
          hashes[i] = EntryEmpty;
      Count = 0;
  }
  ResultType Insert(const string &key, const string &value)
  {
      if (Capacity == Count)//actually we would need to rehash in that case. For whatever reason, author hadn't implemented it
         return TableExceeded;

      uint32_t hashCode = get_hash(key);
      uint32_t location = hashCode % Capacity;
      for(;;) {
          do {
              if (hashes[location] == EntryEmpty)
              {
                  hashes[location] = hashCode;
                  keys[location] = key;
                  vals[location] = value;
                  Count++;
                  return Okey;
              }
              else if (hashes[location] == hashCode && keys[location] == key)
                  return DublicateKey;
          } while (++location != Capacity);
          location = 0;
      }
  }
  ResultType Search(const string &key)
  {
      uint32_t hashCode = get_hash(key);
      uint32_t location = hashCode % Capacity;
      for(;;) {
          do {
              uint32_t hash = hashes[location];
              if (hash == EntryEmpty)
                return NotFound;
              if (hash == hashCode && keys[location] == key)
                 return Okey;
          } while (++location != Capacity);
          location = 0;
      }
  }
};
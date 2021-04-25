#pragma once
#include "common.h"
struct OA_Simple
{
  static const uint32_t EntryEmpty = 0;
  typedef uint32_t hash_t;
  typedef string key_t;
  typedef string val_t;
  static hash_t get_hash(const string &key)
  {
    hash_t h = ::get_hash(key);
    return h ? h : 0x8000000;
  }

  static const char* Name(){return "OA_Simple";}
  unique_ptr<hash_t[]> hashes;
  unique_ptr<key_t[]> keys;
  unique_ptr<val_t[]> vals;
  uint32_t Count = 0;
  uint32_t Capacity = 0;
  static const uint32_t EntrySize = sizeof(val_t) + sizeof(val_t) + sizeof(hash_t);

  OA_Simple(uint32_t size)
  {
    Capacity = size; // HtHelper.GetPrime(size);
    keys = make_unique<key_t[]>(Capacity);
    vals = make_unique<val_t[]>(Capacity);
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

      do {
          loop:;
          if (hashes[location] == EntryEmpty)
          {
              hashes[location] = hashCode;
              keys[location] = key;
              vals[location] = value;
              Count++;
              return Okey;
          }
          else if (hashes[location] == hashCode && keys[location] == key)
          {
              return DublicateKey;
          }
      } while (++location != Capacity);
      location = 0;
      goto loop;//'so sue me', it is simpler code
  }

  ResultItem InsertProbe(const string &key, const string &value)
  {
      ResultItem result;
      if (Capacity == Count)//actually we would need to rehash in that case. For whatever reason, author hadn't implemented it
      {
         result.Result = TableExceeded;
         return result;
       }

      uint32_t hashCode = get_hash(key);
      uint32_t location = hashCode % Capacity;

      do {
          loop:;
          result.Probe++;
          if (hashes[location] == EntryEmpty)
          {
              hashes[location] = hashCode;
              keys[location] = key;
              vals[location] = value;
              Count++;
              result.Result = Okey;
              return result;
          }
          else if (hashes[location] == hashCode && keys[location] == key)
          {
              result.Result = DublicateKey;
              return result;
          }
      } while (++location != Capacity);
      location = 0;
      goto loop;//'so sue me', it is simpler code
  }

  ResultType Search(const string &key)
  {
      uint32_t hashCode = get_hash(key);
      uint32_t location = hashCode % Capacity;

      do {
          loop:;
          uint32_t hash = hashes[location];
          if (hash == EntryEmpty)
            return NotFound;
          if (hash == hashCode && keys[location] == key)
          {
             return Okey;
          }
      } while (++location != Capacity);
      location = 0;
      goto loop;//'so sue me', it is simpler code
  }

  ResultItem SearchProbe(const string &key)
  {
      ResultItem result;
      uint32_t hashCode = get_hash(key);
      uint32_t location = hashCode % Capacity;

      do {
          loop:;
          result.Probe++;
          if (hashes[location] == EntryEmpty)
          {
              result.Result = NotFound;
              return result;
          }
          if (hashes[location] == hashCode && keys[location] == key)
          {
              result.Result = Okey;
              return result;
          }
      } while (++location != Capacity);
      location = 0;
      goto loop;//'so sue me', it is simpler code
  }
};
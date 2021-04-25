#pragma once
#include "common.h"
struct OA_Simple
{
  static const int OnlyPositiveNumbersMask = 0x7FFFFFFF;
  static const int EntryEmpty = -1;
  struct OA_SimpleEntry
  {
    int hashCode = EntryEmpty;
    string key;    // Key of entry
    string value;  // Value of entry
  };


  static const char* Name(){return "OA_Simple";}
  unique_ptr<OA_SimpleEntry[]> entries;
  int Count = 0;
  int Capacity = 0;
  static const int EntrySize = sizeof(OA_SimpleEntry);

  OA_Simple(int size)
  {
    Capacity = size; // HtHelper.GetPrime(size);
    entries = make_unique<OA_SimpleEntry[]>(Capacity);
  }
  void Clear()
  {
      for (int i = 0; i < Capacity; i++)
          entries[i].hashCode = EntryEmpty;
      Count = 0;
  }
  void Print()
  {
      printf("\n№; PSL (probe sequence length); key\n");
      for (int i = 0; i < Capacity; i++)
      {
          if (entries[i].hashCode >= 0)
          {
              int hashCode = get_hash(entries[i].key) & OnlyPositiveNumbersMask;
              int location = hashCode % Capacity;
              printf("%d; %d; %s; \n", i, i - location, entries[i].key.c_str());
          }
          else
          {
              printf("%d; -1;; \n", i);
          }
      }

      printf("\n");
  }

  ResultType Insert(const string &key, const string &value)
  {
      if (Capacity == Count)//actually we would need to rehash in that case. For whatever reason, author hadn't implemented it
         return TableExceeded;

      int hashCode = get_hash(key) & OnlyPositiveNumbersMask;
      int location = hashCode % Capacity;

      for(;;) {
          do {
              if (entries[location].hashCode == EntryEmpty)
              {
                  entries[location].hashCode = hashCode;
                  entries[location].key = key;
                  entries[location].value = value;
                  Count++;
                  return Okey;
              }
              else if (entries[location].hashCode == hashCode && entries[location].key == key)
              {
                  return DublicateKey;
              }
          } while (++location != Capacity);
          location = 0;
      }
  }

  ResultItem InsertProbe(string key, string value)
  {
      ResultItem result;
      if (Capacity == Count)//actually we would need to rehash in that case. For whatever reason, author hadn't implemented it
      {
         result.Result = TableExceeded;
         return result;
       }

      int hashCode = get_hash(key) & OnlyPositiveNumbersMask;
      int location = hashCode % Capacity;

      for(;;) {
          do {
              result.Probe++;
              if (entries[location].hashCode == EntryEmpty)
              {
                  entries[location].hashCode = hashCode;
                  entries[location].key = key;
                  entries[location].value = value;
                  Count++;
                  result.Result = Okey;
                  return result;
              }
              else if (entries[location].hashCode == hashCode && entries[location].key == key)
              {
                  result.Result = DublicateKey;
                  return result;
              }
          } while (++location != Capacity);
          location = 0;
      }
  }

  ResultType Search(const string &key)
  {
      int hashCode = get_hash(key) & OnlyPositiveNumbersMask;
      int location = hashCode % Capacity;

      for(;;) {
          do {
              if (entries[location].hashCode == EntryEmpty)
                return NotFound;
              if (entries[location].hashCode == hashCode && entries[location].key == key)
                  return Okey;
          } while (++location != Capacity);
          location = 0;
      }
  }
  ResultItem SearchProbe(const string &key)
  {
      ResultItem result;
      int hashCode = get_hash(key) & OnlyPositiveNumbersMask;
      int location = hashCode % Capacity;

      for(;;) {
          do {
              result.Probe++;
              if (entries[location].hashCode == EntryEmpty)
              {
                  result.Result = NotFound;
                  return result;
              }
              if (entries[location].hashCode == hashCode && entries[location].key == key)
              {
                  result.Result = Okey;
                  return result;
              }
          } while (++location != Capacity);
          location = 0;
      }
  }
};
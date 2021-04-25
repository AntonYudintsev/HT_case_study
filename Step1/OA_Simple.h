#pragma once
#include "common.h"
struct OA_Simple
{
  static const int OnlyPositiveNumbersMask = 0x7FFFFFFF;
  static const int EntryEmpty = -1;
  static const int EntryDeleted = -2;
  struct OA_SimpleEntry
  {
    int hashCode = EntryEmpty;  // Lower 31 bits of hash code, -1 if unused, -2 if deleted
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
      int hashCode = get_hash(key) & OnlyPositiveNumbersMask;
      int location = hashCode % Capacity;

      int deleted = EntryDeleted;
      while (entries[location].hashCode != EntryEmpty)
      {
          if (entries[location].hashCode == EntryDeleted)
          {
              if (deleted == EntryDeleted)
              {
                  deleted = location;
              }
          }
          else
          {
              if (entries[location].hashCode == hashCode && entries[location].key == key)
              {
                  return DublicateKey;
              }
          }
          location++;
          if (location == Capacity)
          {
              return TableExceeded;
          }
      }

      if (deleted != EntryDeleted)
      {
          location = deleted;
      }

      entries[location].hashCode = hashCode;
      entries[location].key = key;
      entries[location].value = value;
      Count++;

      return Okey;
  }

  ResultItem InsertProbe(string key, string value)
  {
      ResultItem result;
      int hashCode = get_hash(key) & OnlyPositiveNumbersMask;
      int location = hashCode % Capacity;

      int deleted = EntryDeleted;
      do
      {
          result.Probe++;
          if (entries[location].hashCode == EntryEmpty)
          {
              break;
          }
          if (entries[location].hashCode == EntryDeleted)
          {
              if (deleted == EntryDeleted)
              {
                  deleted = location;
              }
          }
          else
          {
              if (entries[location].hashCode == hashCode && entries[location].key == key)
              {
                  result.Result = DublicateKey;
                  return result;
              }
          }
          location++;
          if (location == Capacity)
          {
              result.Result = TableExceeded;
              return result;
          }
      } while (true);

      if (deleted != EntryDeleted)
      {
          location = deleted;
      }

      entries[location].hashCode = hashCode;
      entries[location].key = key;
      entries[location].value = value;
      Count++;

      result.Result = Okey;
      return result;
  }

  ResultType Search(const string &key)
  {
      int hashCode = get_hash(key) & OnlyPositiveNumbersMask;
      int location = hashCode % Capacity;

      do
      {
          if (entries[location].hashCode == EntryEmpty)
          {
              break;
          }
          if (entries[location].hashCode == hashCode && entries[location].key == key)
          {
              return Okey;
          }
          location++;
      } while (location < Capacity);

      return NotFound;
  }
  ResultItem SearchProbe(const string &key)
  {
      ResultItem result;
      int hashCode = get_hash(key) & OnlyPositiveNumbersMask;
      int location = hashCode % Capacity;

      do
      {
          result.Probe++;
          if (entries[location].hashCode == EntryEmpty)
          {
              break;
          }
          if (entries[location].hashCode == hashCode && entries[location].key == key)
          {
              result.Result = Okey;
              return result;
          }
          location++;
      } while (location < Capacity);

      result.Result = NotFound;
      return result;
  }
};
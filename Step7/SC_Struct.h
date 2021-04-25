#pragma once
struct SC_Struct
{
    struct SC_StructEntry
    {
        int hashCode;
        int next;
        string key;
        string value;
    };
    static const int OnlyPositiveNumbersMask = 0x7FFFFFFF;
    static const int EntryEmpty = -1;

    unique_ptr<int[]> buckets;
    unique_ptr<SC_StructEntry[]> entries;
    int freeList = -1;
    int freeCount = 0;

    int Count  = 0;
    int Capacity = 0;

    SC_Struct(int size)
    {
        Capacity = size; // HtHelper.GetPrime(size);
        buckets = make_unique<int[]>(Capacity);
        for (int i = 0; i < Capacity; i++)
            buckets[i] = EntryEmpty;
        entries = make_unique<SC_StructEntry[]>(Capacity);
        freeList = -1;
    }
    void Clear()
    {
        for (int i = 0; i < Capacity; i++)
            buckets[i] = EntryEmpty;
        entries.reset(new SC_StructEntry[Capacity]);
        freeList = -1;
        Count = 0;
        freeCount = 0;
    }
    ResultType Insert(const string& key, const string&value)
    {
        int hashCode = get_hash(key) & OnlyPositiveNumbersMask;
        int location = hashCode % Capacity;

        for (int index = buckets[location]; index >= 0; index = entries[index].next)
        {
            if (entries[index].hashCode == hashCode && entries[index].key == key)
                return DublicateKey;
        }
        int position;
        if (freeCount > 0)
        {
            position = freeList;
            freeList = entries[position].next;
            freeCount--;
        }
        else
        {
            if (Count == Capacity)
                return TableExceeded;
            position = Count;
            Count++;
        }
        entries[position].hashCode = hashCode;
        entries[position].next = buckets[location];
        entries[position].key = key;
        entries[position].value = value;
        buckets[location] = position;
        return Okey;
    }

    ResultType Search(const string& key)
    {
        int hashCode = get_hash(key) & OnlyPositiveNumbersMask;
        int location = hashCode % Capacity;

        for (int index = buckets[location]; index >= 0; index = entries[index].next)
        {
            if (entries[index].hashCode == hashCode && entries[index].key == key)
                return Okey;
        }
        return NotFound;
    }
};
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

    static const char* Name(){return "SC_Struct";}
    unique_ptr<int[]> buckets;
    unique_ptr<SC_StructEntry[]> entries;
    int freeList = -1;
    int freeCount = 0;

    int Count  = 0;
    int Capacity = 0;
    static const int EntrySize = sizeof(SC_StructEntry) + sizeof(int);

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
    void Print()
    {
        printf("\n№; index: key\n");
        for (int i = 0; i < Capacity; i++)
        {
            if (buckets[i] != EntryEmpty)
            {
                auto bucket = buckets[i];
                if (bucket != EntryEmpty)
                {
                    printf("%d; ", i);
                    for (int index = bucket; index >= 0; index = entries[index].next)
                    {
                        printf("%d: %s; ", index, entries[index].key.c_str());
                    }
                    printf("\n");
                }
            }
            else
            {
                printf("%d;\n", i);
            }
        }
        printf("\n");
    }

    ResultType Insert(const string& key, const string&value)
    {
        int hashCode = get_hash(key) & OnlyPositiveNumbersMask;
        int location = hashCode % Capacity;

        for (int index = buckets[location]; index >= 0; index = entries[index].next)
        {
            if (entries[index].hashCode == hashCode && entries[index].key == key)
            {
                return DublicateKey;
            }
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
            {
                return TableExceeded;
            }
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

    ResultItem InsertProbe(const string& key, const string& value)
    {
        ResultItem result;
        int hashCode = get_hash(key) & OnlyPositiveNumbersMask;
        int location = hashCode % Capacity;

        int index = buckets[location];
        while (true)
        {
            result.Probe++;
            if (index < 0)
            {
                break;
            }
            if (entries[index].hashCode == hashCode && entries[index].key == key)
            {
                result.Result = DublicateKey;
                return result;
            }
            index = entries[index].next;
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
            {
                result.Result = TableExceeded;
                return result;
            }
            position = Count;
            Count++;
        }

        entries[position].hashCode = hashCode;
        entries[position].next = buckets[location];
        entries[position].key = key;
        entries[position].value = value;
        buckets[location] = position;

        result.Result = Okey;
        return result;
    }

    ResultType Search(const string& key)
    {
        int hashCode = get_hash(key) & OnlyPositiveNumbersMask;
        int location = hashCode % Capacity;

        for (int index = buckets[location]; index >= 0; index = entries[index].next)
        {
            if (entries[index].hashCode == hashCode && entries[index].key == key)
            {
                return Okey;
            }
        }

        return NotFound;
    }

    ResultItem SearchProbe(const string& key)
    {
        ResultItem result;
        int hashCode = get_hash(key) & OnlyPositiveNumbersMask;
        int location = hashCode % Capacity;

        int index = buckets[location];
        do
        {
            result.Probe++;
            if (index < 0)
            {
                break;
            }
            if (entries[index].hashCode == hashCode && entries[index].key == key)
            {
                result.Result = Okey;
                return result;
            }
            index = entries[index].next;
        } while (true);

        result.Result = NotFound;
        return result;
    }
};

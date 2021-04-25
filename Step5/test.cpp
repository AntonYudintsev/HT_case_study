#include "OA_Simple.h"
#include "SC_Struct.h"
#include <chrono>
#include <random>
#include <vector>
#include <intrin.h>
#include <unordered_set>

typedef std::chrono::high_resolution_clock hclock;
#define PRUNE_CACHE 0
enum {NUM_RUNS = PRUNE_CACHE ? 10 : 200};

vector<string> KeyUnique(size_t size)
{
  const int seed = 1;
  std::mt19937 gen(seed);
  std::uniform_int_distribution<> distrib(INT_MIN, INT_MAX);
  unordered_set<string> set(size);
  vector<string> result; result.reserve(size);
  while (result.size() < size)
  {
    string key = to_string(distrib(gen));
    if (set.insert(key).second)
      result.emplace_back(move(key));
  }

  return result;
}

static const uint32_t L2_cache_size = 8192<<10, cache_line_size = 64;
volatile uint32_t _helper = 0;
static vector<uint32_t> _mem(L2_cache_size/sizeof(uint32_t));

static void prune_cache()
{
#if PRUNE_CACHE
  for (auto i = _mem.begin(), e = _mem.end(); i < e; i += cache_line_size/sizeof(uint32_t))
    _helper += *i;
#endif
}

vector<string> KeyUnique(vector<string> &keys)
{
  const int seed = 2;
  std::mt19937 gen(seed);
  std::uniform_int_distribution<> distrib(INT_MIN, INT_MAX);

  unordered_set<string> setInclude;
  for (auto &k:keys)
    setInclude.insert(k);
  unordered_set<string> setExclude(keys.size());
  vector<string> result; result.reserve(keys.size());

  while (result.size() < keys.size())
  {
    string key = to_string(distrib(gen));
    if (setInclude.find(key) == setInclude.end() && setExclude.insert(key).second)
      result.emplace_back(move(key));
  }
  return result;
}

template<class HT>
void RunTest(int count, int capacity)
{
  HT ht(capacity);

  printf("\n%s capacity=%d (kinda)memory=%d loadFactor = %f\n", HT::Name(), capacity, capacity * ht.EntrySize, float(count)/capacity);

    int insertProbe = 0, insertCount = 0;
    int includeProbe = 0, includeCount = 0;
    int excludeProbe = 0, excludeCount = 0;
    auto keysInsert = KeyUnique(count);
    vector<string> keysInclude; keysInclude.reserve(count);

    for (auto &k:keysInsert)
    {
        auto result = ht.InsertProbe(k, k);
        if (result.Result == Okey)
        {
            keysInclude.push_back(k);
            insertProbe += result.Probe;
            insertCount++;
        }
    }

    for (auto &s: keysInclude)
    {
        auto result = ht.SearchProbe(s);
        includeProbe += result.Probe;
        includeCount++;
    }

    auto keysExclude = KeyUnique(keysInclude);

    for (auto &s: keysExclude)
    {
        auto result = ht.SearchProbe(s);
        excludeProbe += result.Probe;
        excludeCount++;
    }

    ht.Clear();
    uint64_t insertTicks = 0, includeTicks = ~uint64_t(0), excludeTicks = ~uint64_t(0);
    unsigned int ui;
    for (auto &s: keysInclude)
    {
        //prune_cache();
        const int64_t reft = __rdtscp(&ui);
        ht.Insert(s, s);
        insertTicks += __rdtscp(&ui)-reft;
    }
    int hits = 0, misses = 0;//we have to count hits
    for (int ri = 0; ri != NUM_RUNS; ++ri)
    {
        uint64_t ticks = 0;
        for (auto &s: keysInclude)
        {
            prune_cache();
            const int64_t reft = __rdtscp(&ui);
            hits += ht.Search(s) == Okey;
            ticks += __rdtscp(&ui)-reft;
        }
        includeTicks = min(includeTicks, ticks);
    }

    for (int ri = 0; ri != NUM_RUNS; ++ri)
    {
        uint64_t ticks = 0;
        for (auto &s: keysExclude)
        {
            prune_cache();
            const int64_t reft = __rdtscp(&ui);
            misses += ht.Search(s) == Okey;
            ticks += __rdtscp(&ui)-reft;
        }
        excludeTicks = min(excludeTicks, ticks);
    }
    printf("%d; %d; %d"
      "; %d; %f; %lld; %f"
      "; %d; %f; %lld; %f"
      "; %d; %f; %lld; %f\n",
      insertCount, hits/NUM_RUNS, misses/NUM_RUNS,
      insertProbe, float(insertProbe)/insertCount, insertTicks, double(insertTicks)/insertCount,
      includeProbe, float(includeProbe)/includeCount, includeTicks, double(includeTicks)/includeCount,
      excludeProbe, float(excludeProbe)/excludeCount, excludeTicks, double(excludeTicks)/excludeCount
    );
    printf("%s ticks: INS %lld; HIT %lld; MIS: %lld\n", HT::Name(), insertTicks, includeTicks, excludeTicks);
}

int main()
{
  printf("inserts; hits; misses;"
      "(ins) probes cnt;"
      "(ins) avg probes cnt;"
      "(ins) ticks;"
      "(ins) avg ticks;"
      "(hit) probes cnt;"
      "(hit) avg probes cnt;"
      "(hit) ticks;"
      "(hit) avg ticks;"
      "(mis) probes cnt;"
      "(mis) avg probes cnt;"
      "(mis) ticks;"
      "(mis) avg ticks;"
      "\n");
  const size_t sz = 16384, cnt = sz * 0.9/1.11762;//use constant load factor
  RunTest<OA_Simple>(cnt, sz);
  RunTest<SC_Struct>(cnt, sz/1.11762);
  return 0;
}
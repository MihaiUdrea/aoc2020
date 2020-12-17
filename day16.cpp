// day16 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"


#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/all.hpp>

#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>
#include <boost/algorithm/cxx11/copy_if.hpp>

#define THISDAY "day16"

#define FIRST_STAR  "24110"
#define SECOND_STAR "6766503490793"

#ifdef THISDAY
  #define TODAY THISDAY "/"
  #define HIDE_IF_OLD_TEST "[.]"
#else
  #define TODAY
  #define HIDE_IF_OLD_TEST
#endif  // THISDAY

// disable conversion warning
#pragma warning(disable : 4267)

using namespace ranges;

//namespace
//{
struct Solve
{
  /**/
  struct data
  {
    static bool InsideInterval(pair<int, int> interval, int no)
    {
      return interval.first <= no && no <= interval.second;
    }

    bool Permits(int no)
    {
      return find_if(interval, [no](auto & interval) {
               return interval.first <= no && no <= interval.second;
             }) != interval.end();
    }

    vector<pair<int, int>> interval;
    string                      str;
  };

  vector<data> list;
  /**/

  vector<string>      input;
  vector<int> yourTicket;
  vector<vector<int>> nearByTicket;

  Solve(const string & inStr){
    forEachRxTokenIdx(inStr, regex("\n\n"), [&](string chunk, int idx) {      
      forEachLineIdx(chunk, [&](string line, int idx2) {
        if (idx == 0)
        {
          auto res = match_rx(line, regex(R"~(([^:]+): (\d+)-(\d+) or (\d+)-(\d+))~"));
          list.push_back({ { { stoi(res[2]), stoi(res[3]) }, { stoi(res[4]), stoi(res[5]) } },
                           res[1].str() });
        }
        else if (idx == 1)
        {
          forEachRxToken(line, regex(","), [&](string nrS) {
            try { yourTicket.push_back(stoi(nrS)); } catch (...){}
          });         
        }
        else
        {
          nearByTicket.push_back({});
          forEachRxToken(line, regex(","), [&](string nrS) {
            try { nearByTicket.back().push_back(stoi(nrS));} catch (...){}
          });
        }
      });
    });
  };

  string Do()
  { 
    return to_string(accumulate(nearByTicket | views::join | views::filter([&](auto no) {
                                  return !any_of(list, [no](auto & field) {
                                    return field.Permits(no);
                                  });
                                }),
                                0));
  }

  string Do2()
  {
    nearByTicket.push_back(yourTicket);

    auto validTicketsOp = [&](auto & ticket) {
      return !any_of(ticket, [&](auto & no) {
               return !any_of(list, [no](auto & field) {
                        return field.Permits(no);
                      });
             });
    };

    auto validTickets = nearByTicket | views::filter(validTicketsOp);

    vector<set<int>> nokPos(list.size());
    for (auto ticket : validTickets)
      for (auto [idxk, no] : ticket | views::enumerate)
        for (auto [idxw, field] : list | views::enumerate)
        {
          if (find_if(field.interval, [no](auto & interval) {
                return data::InsideInterval(interval, no);
              }) == field.interval.end())
            nokPos[idxk].insert(idxw);
        }

    vector<int> goodPos(list.size());

    auto unsettled = views::iota(0) | views::take(list.size()) | to<set>();
    auto unused    = unsettled;
    
    for (auto [field, size] : nokPos | views::enumerate | views::transform([&](auto i) { return pair<int, int>(i.first, i.second.size());}) |
                  to<vector>() | actions::sort(greater{}, &pair<int, int>::second))
    {
      vector<int> diff;
      set_difference(unsettled, nokPos[field], back_inserter(diff));

      goodPos[field] = diff.front();

      unsettled.erase(diff.front());
    }

  return to_string(accumulate(goodPos | views::enumerate | views::transform([&]( auto i ) { return i.second < 6 ? yourTicket[i.first] : 1; }), 1ll, multiplies()));
  }
};
//}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[.]")
{
  REQUIRE(Solve(1 + R"(
class: 1-3 or 5-7
row: 6-11 or 33-44
seat: 13-40 or 45-50

your ticket:
7,1,14

nearby tickets:
7,3,47
40,4,50
55,2,20
38,6,12)")
            .Do() == "71");
}
/** /
TEST_CASE(TODAY "Sample 2", HIDE_IF_OLD_TEST "[.]")
{
  REQUIRE(Solve(1 + R"(
class: 0-1 or 4-19
row: 0-5 or 8-19
seat: 0-13 or 16-19

your ticket:
11,12,13

nearby tickets:
3,9,18
15,1,5
5,14,9)")
            .Do2() == "132");
}
/**/

TEST_CASE(TODAY "Part One", HIDE_IF_OLD_TEST "[x.]")
{
#ifdef FIRST_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do() == FIRST_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do());
#endif  // FIRST_STAR
}

TEST_CASE(TODAY "Part Two", HIDE_IF_OLD_TEST "[x.]")
{
#ifdef SECOND_STAR
  REQUIRE(Solve(ReadFileToString(TODAY "input.txt")).Do2() == SECOND_STAR);
#else
  toClipboard(Solve(ReadFileToString(TODAY "input.txt")).Do2());
#endif
}

// day16 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"


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

using namespace ranges;

//namespace
//{
struct Solve
{
  struct Label
  {
    bool Allows(int no)
    {
      return any_of(interval, [no](auto & interval) {
        return interval.first <= no && no <= interval.second;
        });
    }

    vector<pair<int, int>> interval;
  };

  vector<Label> labelsList;

  vector<vector<int>> nearByTicket;

  Solve(const string & inStr)
  { 
    for (auto chunk : inStr | views::tokenize(regex("\n\n"), -1) | views::transform(to_string_view()) | views::take(1) )
      for (auto line : chunk | views::tokenize(regex("\n"), -1) | views::transform(to_string_view()))
      {
          auto res = match_rx(line, regex(R"~(([^:]+): (\d+)-(\d+) or (\d+)-(\d+))~"));
          labelsList.push_back({ { { stoi(res[2]), stoi(res[3]) }, { stoi(res[4]), stoi(res[5]) } }});
      }

    for (auto chunk : inStr | views::tokenize(regex("\n\n"), -1) | views::transform(to_string_view()) | views::drop(1))
      for (auto line : chunk | views::tokenize(regex("\n"), -1) | views::transform(to_string_view()) | views::drop(1))
        nearByTicket.push_back(line | views::tokenize(regex(","), -1) | views::transform(sub_match_to_int()) | to<vector>);
  };

  string Do()
  { 
    return to_string(accumulate(nearByTicket 
      | views::drop(1) // your ticket is valid but skip it for correctness
      | views::join 
      | views::filter([&](auto no) {
        return !any_of(labelsList, [no](auto & field) {
          return field.Allows(no);
        });}),0));
  }

  string Do2()
  {
    vector<set<int>> nokPos(labelsList.size());
    
    auto validTicketsOp = [&](auto & ticket) { 
      return !any_of(ticket, [&](auto & no) { 
        return !any_of(labelsList, [no](auto & field) {
          return field.Allows(no);});});};

    for (auto ticket : nearByTicket | views::filter(validTicketsOp))
      for (auto [idxk, no] : ticket | views::enumerate)
        for (auto [idxw, field] : labelsList | views::enumerate | views::filter([no](auto e) {
          return !e.second.Allows(no);}))
            nokPos[idxk].insert(idxw);

    vector<int> goodPos(labelsList.size());

    auto unsettled = views::iota(0) | views::take(labelsList.size()) | to<set>();
    auto unused    = unsettled;
    
    for (auto [field, size] : nokPos 
      | views::enumerate 
      | views::transform([&](auto i) { return pair<int, int>(i.first, i.second.size());}) 
      | to<vector>() 
      | actions::sort(greater{}, &pair<int, int>::second))
    {
      auto newGoodPos = unsettled | views::filter([&](auto i) { 
        return !nokPos[field].contains(i); });

      goodPos[field] = *begin(newGoodPos);

      unsettled.erase(*begin(newGoodPos));
    }

    return to_string(accumulate(goodPos 
      | views::enumerate 
      | views::transform([&]( auto i ) { 
        return i.second < 6 ? nearByTicket[0][i.first] : 1; }), 
      1ll, multiplies()));
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

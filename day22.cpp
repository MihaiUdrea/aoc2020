// day22 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"
#include  <list>

//#define THISDAY "day22"

//#define FIRST_STAR  "35562"
//#define SECOND_STAR "34424"

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

namespace
{
struct Solve
{
  using list = std::list<int>;
  vector<list> mycards {2};

  Solve(const string & inStr){
   for (auto [idx, chunk] : inStr | views::tokenize(regex("\n\n"), -1) | views::transform(to_string_view()) | views::enumerate)
      for (auto line : chunk | views::tokenize(regex("\n"), -1) | views::tail)
        mycards[idx].push_back(stoi(line));
  };

  pair<int,int> Play(vector<list> cards, bool recursive = false)
  {
    set<vector<list>> history;

    int winnerIdx = 0;
    while (none_of(cards, &list::empty))
    {
      if (!history.insert(cards).second)
        return { 0, 0 };

      auto HasSpace = [](const auto & deck) {return deck.front() < (int)deck.size();};

      winnerIdx = recursive && all_of(cards, HasSpace) 
        ? Play(cards 
          | views::transform([](auto & deck) { 
              return deck 
                | views::tail 
                | views::take(deck.front()) 
                | to<std::list>;}) 
          | to<vector>, recursive).first
        : cards[1].front() > cards[0].front();
      
      auto & winner = cards[winnerIdx];
      auto & loser = cards[1 - winnerIdx];

      winner.push_back(winner.front());
      winner.push_back(loser.front());

      winner.pop_front();
      loser.pop_front();    
    }

    return { winnerIdx, inner_product(cards | views::join | views::reverse, views::iota(1), 0) }; 
  }

  string Do() { return to_string(Play(mycards).second); }
  string Do2() { return to_string(Play(mycards, true).second); }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[.]")
{
  REQUIRE(Solve(1 + R"(
Player 1:
9
2
6
3
1

Player 2:
5
8
4
7
10)")
            .Do() == "306");
}

TEST_CASE(TODAY "Nicu 2", HIDE_IF_OLD_TEST "[.]")
{
  REQUIRE(Solve(1 + R"(
Player 1:
38
1
28
32
43
21
42
29
18
13
39
41
49
31
19
26
27
40
35
14
3
36
12
16
45

Player 2:
34
15
47
20
23
2
11
9
8
7
25
50
48
24
46
44
10
6
22
5
33
30
4
17
37)")
            .Do2() == "34173");
}

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

// day19 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"

//#define THISDAY "day19"

//#define FIRST_STAR  "265"
//#define SECOND_STAR "394"

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
  /**/
  struct rule
  {
    vector<vector<int>> subrules;
    char                ch = 'x' ;
  };

  map<int, rule> rulesList;
  /**/

  vector<string> input;

  Solve(const string & inStr){

     for (auto chunk : inStr 
       | views::tokenize(regex("\n\n"), -1) 
       | views::transform(to_string_view()) 
       | views::take(1) )
      for (auto line : chunk | views::tokenize(regex("\n"), -1) | views::transform(to_string_view()))
      {
        auto res1 = match_rx(line, regex(R"~(([^:]+): (.*))~"));          
        std::string_view s(&*res1[2].first, res1[2].length());
        //auto             s2 = to_string_view(res1[2]);
        rule newRule;

        auto res2 = match_rx(s, regex(R"~("(a|b)")~"));
        
        if (!res2.empty())
          newRule.ch = *res2[1].first;
        else
        for (auto vaiant : s | views::tokenize(regex("\\|"), -1) | views::transform(to_string_view()))
        {
          newRule.subrules.push_back({});
          for (auto no : vaiant | views::tokenize(regex(" "), -1) | views::transform(sub_match_to_int())  )
            if (no>0)
            newRule.subrules.back().push_back(no);
        }

        rulesList[stoi(res1[1])] = newRule;
      }

     for (auto chunk : inStr 
       | views::tokenize(regex("\n\n"), -1) 
       | views::transform(to_string_view()) 
       | views::drop(1))
      for (auto line : chunk | views::tokenize(regex("\n"), -1))
        input.push_back(line);     
  };

  vector<string> ExpandSubRules(vector<int> ruleIdxList) 
  {
    vector<string> res{""};

    for (auto idx : ruleIdxList)
    {
      auto & v = Expand(idx);
      vector<string> newRes;
      for (auto s : v)
      {
        // append 
        for (auto prevRes : res)
          newRes.push_back(prevRes + s);
      }
      res = std::move(newRes);
    }
    return res;
  }

  map<int, set<string>> dictionary;

  set<string> & Expand(int ruleIdx) 
  {
    if (dictionary.find(ruleIdx) != dictionary.end())
      return dictionary[ruleIdx];

    rule & r = rulesList[ruleIdx];

    set<string> res;
    if (r.ch != 'x')
    {
      res.insert(string() + r.ch);
    }
    else
    {
      for (auto var : r.subrules)
      {
        auto v = ExpandSubRules(var);
        for (auto i : v)
          res.insert(i);
      }
    }
    dictionary[ruleIdx] = std::move(res);

    return dictionary[ruleIdx];
  }

  string Do() { 
  // generate all combs
    Expand(0);

    auto res = count_if(input, std::bind(
      &Solve::MatchesRule, this, 0, std::placeholders::_1));

    return to_string(res); 
  }

  bool MatchesRule(int rule, string s) {
    return dictionary[rule].contains(s);
  }

  string Do2() {
    
    // 8 : 42 | 42 8 
    //11 : 42 31 | 42 11 31

    // 42 42 42... 31 31 31 31 31
    Expand(0);

    auto res = count_if(input, [&](auto & line) {
      int countOf42 = distance(line 
        | views::chunk(dictionary[42].begin()->size()) 
        | views::take_while([&](auto word) {
            return MatchesRule(42, word | to<string>); }));
    
      int countOf31 = distance(line 
        | views::chunk(dictionary[31].begin()->size()) 
        | views::reverse 
        | views::take_while( [&](auto word) {
          return MatchesRule(31, word | to<string>);}));

      return countOf31 > 0 && 
        countOf42 > countOf31 && (
        countOf42 * dictionary[42].begin()->size() + countOf31 * dictionary[31].begin()->size() == line.size());
    });

    return to_string(res); 
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve(1 + R"(
0: 4 1 5
1: 2 3 | 3 2
2: 4 4 | 5 5
3: 4 5 | 5 4
4: "a"
5: "b"

ababbb
bababa
abbbab
aaabbb
aaaabbb)")
            .Do() == "2");
}

TEST_CASE(TODAY "Sample 2", HIDE_IF_OLD_TEST "[x.]")
{
  REQUIRE(Solve(1 + R"(
42: 9 14 | 10 1
9: 14 27 | 1 26
10: 23 14 | 28 1
1: "a"
11: 42 31
5: 1 14 | 15 1
19: 14 1 | 14 14
12: 24 14 | 19 1
16: 15 1 | 14 14
31: 14 17 | 1 13
6: 14 14 | 1 14
2: 1 24 | 14 4
0: 8 11
13: 14 3 | 1 12
15: 1 | 14
17: 14 2 | 1 7
23: 25 1 | 22 14
28: 16 1
4: 1 1
20: 14 14 | 1 15
3: 5 14 | 16 1
27: 1 6 | 14 18
14: "b"
21: 14 1 | 1 14
25: 1 1 | 1 14
22: 14 14
8: 42
26: 14 22 | 1 20
18: 15 15
7: 14 5 | 1 21
24: 14 1

abbbbbabbbaaaababbaabbbbabababbbabbbbbbabaaaa
bbabbbbaabaabba
babbbbaabbbbbabbbbbbaabaaabaaa
aaabbbbbbaaaabaababaabababbabaaabbababababaaa
bbbbbbbaaaabbbbaaabbabaaa
bbbababbbbaaaaaaaabbababaaababaabab
ababaaaaaabaaab
ababaaaaabbbaba
baabbaaaabbaaaababbaababb
abbbbabbbbaaaababbbbbbaaaababb
aaaaabbaabaaaaababaa
aaaabbaaaabbaaa
aaaabbaabbaaaaaaabbbabbbaaabbaabaaa
babaaabbbaaabaababbaabababaaab
aabbbbbaabbbaaaaaabbbbbababaaaaabbaaabba)")
            .Do2() == "12");
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

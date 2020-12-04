// day4 - 2020 Day ? ?????????
#include "stdafx.h"
#include "Utils.h"

#define THISDAY "day4"

#define FIRST_STAR  "254"
#define SECOND_STAR "184"

#ifdef THISDAY
  #define TODAY THISDAY "/"
  #define HIDE_IF_OLD_TEST "[.]"
#else
  #define TODAY
  #define HIDE_IF_OLD_TEST
#endif  // THISDAY

// disable conversion warning
#pragma warning(disable : 4267)


namespace
{
struct Solve
{
  /**/
  struct data
  {
    int byr;  // (Birth Year)
    int iyr; // (Issue Year)
    int eyr; //(Expiration Year)
    int hgt; //(Height)
    int hcl; //(Hair Color)
    int ecl; //(Eye Color)
    int pid; //(Passport ID)
    int cid; //(Country ID)

    map<string, string> raw;
  };

  vector<data> list;

  vector<string> input;

  Solve(const string & inStr){

    input = GetLines(inStr);

    data cr;
    for (auto line : input)
    {
      if (line.empty())
      {
        list.push_back(cr);
        cr = {};

      }

      static const regex colsSepRx(" ");
      forEachRxToken(line, colsSepRx, [&](string chunk) {
        string name = chunk.substr(0, chunk.find(':'));
        string val = chunk.substr(chunk.find(':') +1);

        if (name.empty())
          return;
        cr.raw[name]    = val;
      });
    }

    list.push_back(cr);
    cr = {};
  };

  string Do() { 
    
    int valid = 0;
    for (auto pass : list)
    {
      auto whereOpt = pass.raw.find("cid");

      if (pass.raw.size() == 8 || (pass.raw.size() == 7 && whereOpt == pass.raw.end()))
        valid++;
    }

    return to_string(valid); }

  string Do2()
  {
    int valid = 0;
    for (auto pass : list)
    {
      auto whereOpt = pass.raw.find("cid");

      if (pass.raw.size() == 8 || (pass.raw.size() == 7 && whereOpt == pass.raw.end()))
      {
        pass.byr = stoi(pass.raw["byr"]);
        pass.iyr = stoi(pass.raw["iyr"]);
        pass.eyr = stoi(pass.raw["eyr"]);
        pass.hgt = stoi(pass.raw["hgt"]);

        if (pass.byr<1920 || pass.byr> 2002)
          continue;

        if (pass.iyr < 2010 || pass.iyr > 2020)
          continue;

        if (pass.eyr < 2020 || pass.eyr > 2030)
          continue;

        static const regex HeightExp(R"~(([0-9]*cm)|([0-9]*in))~");
        auto               height = match_rx(pass.raw["hgt"], HeightExp);
        if (height.empty())
          continue;
        else 
        {
          if (height[1].matched)
          {
            // cm 150 193
            if (pass.hgt < 150 || pass.hgt > 193)
              continue; 
          }
          else if (pass.hgt < 59 || pass.hgt > 76)
            continue; 
        }
        
        static const regex HairExp(R"~(#[0-9a-f]*)~");
        static const regex ECLExp(R"~(amb|blu|brn|gry|grn|hzl|oth)~");
        static const regex PIDExp(R"~([0-9]*)~");

        auto hair = match_rx(pass.raw["hcl"], HairExp);
        if (hair.empty() || hair[0].length() != 7)
          continue;
        auto ecl  = match_rx(pass.raw["ecl"], ECLExp);
        if (ecl.empty())
          continue;
        auto pid = match_rx(pass.raw["pid"], PIDExp);
        if (pid.empty() || pid[0].length() != 9)
          continue;

        valid++;
      }
        
    }

    return to_string(valid);
  }
};
}  // namespace

#include "catch.hpp"

TEST_CASE(TODAY "Sample 1", HIDE_IF_OLD_TEST "[.]")
{
  

  REQUIRE(Solve(1 + R"(
ecl:gry pid:860033327 eyr:2020 hcl:#fffffd
byr:1937 iyr:2017 cid:147 hgt:183cm

iyr:2013 ecl:amb cid:350 eyr:2023 pid:028048884
hcl:#cfa07d byr:1929

hcl:#ae17e1 iyr:2013
eyr:2024
ecl:brn pid:760753108 byr:1931
hgt:179cm

hcl:#cfa07d eyr:2025 pid:166559648
iyr:2011 ecl:brn hgt:59in)")
            .Do() == "2");
}

TEST_CASE(TODAY "Sample 2", HIDE_IF_OLD_TEST "[.]")
{
  

  REQUIRE(Solve(1 + R"(
pid:087499704 hgt:74in ecl:grn iyr:2012 eyr:2030 byr:1980
hcl:#623a2f

eyr:2029 ecl:blu cid:129 byr:1989
iyr:2014 pid:896056539 hcl:#a97842 hgt:165cm

hcl:#888785
hgt:164cm byr:2001 iyr:2015 cid:88
pid:545766238 ecl:hzl
eyr:2022

iyr:2010 hgt:158cm hcl:#b6652a ecl:blu byr:1944 eyr:2021 pid:093154719)")
            .Do2() == "4");
}

TEST_CASE(TODAY "Sample 3", HIDE_IF_OLD_TEST "[.]")
{
  

  REQUIRE(Solve(1 + R"(
eyr:1972 cid:100
hcl:#18171d ecl:amb hgt:170 pid:186cm iyr:2018 byr:1926

iyr:2019
hcl:#602927 eyr:1967 hgt:170cm
ecl:grn pid:012533040 byr:1946

hcl:dab227 iyr:2012
ecl:brn hgt:182cm pid:021572410 eyr:2020 byr:1992 cid:277

hgt:59cm ecl:zzz
eyr:2038 hcl:74454a iyr:2023
pid:3556412378 byr:2007
)")
            .Do2() == "0");
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

#ifndef UTILS_H
#define UTILS_H

#pragma once

#include "stdafx.h"

#define DAY "DAY"

string ReadFileToString(const char * aFileName);

string ToString(int aNr);

void toClipboard(const std::string & s);

void toClipboard(int nr);


template <typename Iterator>
class _indexed_iterator : public Iterator
{
public:
  int idx = 0;

  _indexed_iterator(){};
  _indexed_iterator(_indexed_iterator<Iterator> const & other)
    : Iterator((Iterator const &)other)
  {
    idx = other.idx;
  };

  _indexed_iterator(Iterator const & other, int aidx = 0)
    : Iterator(other)
  {
    idx = aidx;
  };

  _indexed_iterator<Iterator> & operator++()
  {
    Iterator::operator++();
    idx++;
    return *this;
  }

  _indexed_iterator<Iterator> operator++(int)
  {
    _indexed_iterator<Iterator> tmp(*this);

    operator++();
    return tmp;
  }
  bool operator==(const _indexed_iterator<Iterator> & rhs) const { return idx == rhs.idx; }
  bool operator!=(const _indexed_iterator<Iterator> & rhs) const { return idx != rhs.idx; }
  pair<typename Iterator::pointer, int> operator*()
  {
    auto r = Iterator::_Unwrapped();
    return make_pair(r, idx);
  }
};

template <typename Iterator, typename Container>
class _iterate_as_wrapper
{
public:
  _iterate_as_wrapper(Container & v) noexcept
    : v(v){};

  Iterator begin() { return v.begin(); }

  Iterator end() { return Iterator(v.end(), (int)v.size()); }

  Iterator begin() const { return v.begin(); }
  Iterator end() const { return v.end(); }

private:
  Container & v;
};

template <typename Container>
auto with_index(const Container & aElem)
{
  return _iterate_as_wrapper<_indexed_iterator<Container::iterator>, Container>(
    const_cast<Container &>(aElem));
}

class irange
{
  int aFirst;
  int aLast;
  int aStep;

  class Iter
  {
    int idx;
    int step;

  public:
    Iter(int x, int s)
      : idx(x)
      , step(s)
    {
    }
    Iter(const Iter & mit)
      : idx(mit.idx)
      , step(mit.step)
    {
    }
    Iter & operator++()
    {
      idx += step;
      return *this;
    }
    Iter operator++(int)
    {
      Iter tmp(*this);
           operator++();
      return tmp;
    }
    bool  operator==(const Iter & rhs) const { return step > 0 ? idx >= rhs.idx : idx <= rhs.idx; }
    bool  operator!=(const Iter & rhs) const { return step > 0 ? idx < rhs.idx : idx > rhs.idx; }
    int & operator*() { return idx; }
  };

public:
  irange(int aFirst, int aLast, int pStep = 1)
    : aFirst(aFirst)
    , aLast(aLast)
    , aStep(pStep)
  {
    if (aFirst > aLast)
      aStep = -aStep;
  }

  Iter begin() { return Iter(aFirst, aStep); }

  Iter end() { return Iter(aLast, aStep); }
};


template <typename Ret, typename OpT>
Ret Compare(OpT aL, OpT aR, Ret LtResult, Ret EqResult, Ret GtResult)
{
  if (aL < aR)
    return LtResult;
  else if (aL == aR)
    return EqResult;
  else
    return GtResult;
}

const regex lineRxToken("\\n");

void forEachRxToken(const string & inStr, const regex & sepRx, std::function<void(string)> fct);
void forEachRxTokenIdx(const string & inStr, const regex & sepRx, std::function<void(string, size_t)> fct);

void forEachLine(const string & inStr, std::function<void(string)> fct);
void forEachLineIdx(const string & inStr, std::function<void(string, size_t)> fct);

vector<string> Tokenize(const string & inStr, const regex & sepRx);

vector<string> GetLines(const string & inStr);

// use as:
//
// auto res = match_rx(line, sepRx);
template <class _StTraits, class _StAlloc, class _Elem, class _RxTraits>
auto match_rx(const basic_string<_Elem, _StTraits, _StAlloc> & _Str,
              const basic_regex<_Elem, _RxTraits> &            _Re,
              regex_constants::match_flag_type _Flgs = regex_constants::match_default)
{
  match_results<typename basic_string<_Elem, _StTraits, _StAlloc>::const_iterator,
                allocator<sub_match<basic_string<_Elem, _StTraits, _StAlloc>::const_iterator>>>
    _Matches;
  // try to match regular expression to target text
  _Regex_match1(_Str.begin(), _Str.end(), _STD addressof(_Matches), _Re, _Flgs, true);
  return _Matches;
}

template <class _Col, class _Ty>
inline bool contains(const _Col & _Collection, const _Ty & _Val)
{
  return find(_Collection.begin(), _Collection.end(), _Val) != _Collection.end();
}

template <class _Col, class _Pr>
inline bool contains_pred(const _Col & _Collection, _Pr _Pred)
{
  return find_if(_Collection.begin(), _Collection.end(), _Pred) != _Collection.end();
}

using my_int = __int64;

struct Point
{
  my_int x = 0;
  my_int y = 0;
  my_int z = 0;

  auto operator<=>(const Point &) const = default;
  auto operator+(const Point & l) const { return Point{ x + l.x, y + l.y, z + l.z }; };
  auto operator-(const Point & l) const { return Point{ x - l.x, y - l.y, z - l.z }; };

  my_int GetAxys(my_int ax) const
  {
    switch (ax)
    {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    default:
      break;
    }
    return -1;
  }

  void IncAxyx(my_int ax, my_int v)
  {
    switch (ax)
    {
    case 0:
      x += v;
      break;
    case 1:
      y += v;
      break;
    case 2:
      z += v;
      break;
    default:
      break;
    }
  }
};

struct CharMapLimits
{
  CharMapLimits(const string & inStr)
  {
    forEachLineIdx(inStr, [&](string line, size_t idxL) {
      for (auto [ch, idxC] : with_index(line))
      {
        charMap[*ch].insert({ (int)idxL, idxC, 0 });
        idxC++;
      }
      limit.x++;
      limit.y = line.size();
    });
  }

  map<char, set<Point>> charMap;

  Point limit;
};


my_int ManhDist(Point a);

void toConsole(Point p, const char * c, int length, int sleep = -1);

void toConsole(Point p, string c, int sleep = -1);

struct to2DsFlags
{
  enum DisplayMode
  {
    no_header   = 0,
    cols_count  = 1 << 0,
    line_count  = 1 << 1,
    full_header = 0b011,
    depth_on    = 1 << 2
  };
};

template <class _Col>
pair<Point, Point> MinMax(const _Col &                                            _Collection,
                          std::function<Point(const typename _Col::value_type &)> toPtFct)
{
  auto & flatList = _Collection;

  auto limX = minmax_element(flatList.begin(), flatList.end(), [&](auto & l, auto & r) {
    return toPtFct(l).x < toPtFct(r).x;
  });
  auto limY = minmax_element(flatList.begin(), flatList.end(), [&](auto & l, auto & r) {
    return toPtFct(l).y < toPtFct(r).y;
  });
  auto limZ = minmax_element(flatList.begin(), flatList.end(), [&](auto & l, auto & r) {
    return toPtFct(l).z < toPtFct(r).z;
  });

  Point min{ limX.first->first.x, limY.first->first.y, limZ.first->first.z };
  Point max{ limX.second->first.x, limY.second->first.y, limZ.second->first.z };
  return { min, max };
}

// auto ss = to2Ds(vals, [](auto& l) { return l.x; }, [](auto& l) { return
// string() + l.ch; }, to2DsFlags::full_header, '.', 1);
template <class _Col>
string to2Ds(const _Col &                                                    _Collection,
             std::function<Point(const typename _Col::value_type &, size_t)> toPtFct,
             std::function<string(const typename _Col::value_type &)>        toStringFct,
             int   mode      = to2DsFlags::no_header,
             char  fill      = ' ',
             int   charWidth = 1,
             Point min       = {},
             Point max       = {})
{
  map<Point, string> flatList;
  size_t             idx = 0;
  for_each(_Collection.begin(), _Collection.end(), [&](auto & el) {
    Point pt = toPtFct(el, idx++);

    if (mode & to2DsFlags::depth_on == 0)
      pt.z = 0;

    string str   = toStringFct(el);
    flatList[pt] = str;
  });

  auto limX = minmax_element(flatList.begin(), flatList.end(), [](auto & l, auto & r) {
    return l.first.x < r.first.x;
  });
  auto limY = minmax_element(flatList.begin(), flatList.end(), [](auto & l, auto & r) {
    return l.first.y < r.first.y;
  });
  auto limZ = minmax_element(flatList.begin(), flatList.end(), [](auto & l, auto & r) {
    return l.first.z < r.first.z;
  });

  if (min == max)
  {
    min = Point{ limX.first->first.x, limY.first->first.y, limZ.first->first.z };
    max = Point{ limX.second->first.x, limY.second->first.y, limZ.second->first.z };
  }

  int minCol = min.x;

  int lastCol = max.x + 1;
  // lastCol = 35;

  int minLine  = min.y;
  int lastLine = max.y + 1;

  int minPlane = min.z;
  int maxPlane = max.z + 1;

  if (mode & to2DsFlags::depth_on == 0)
  {
    minPlane = 0;
    maxPlane = 1;
  }

  stringstream out;

  // ofstream out;
  // out.open(OUT_FILE, FILE_OUT_MODE);
  // system("CLS");
  // auto& out = cout;
  // out << "------------------------------" << path << "------------" << endl;

  // print top/cols count header
  auto topHeader = [&](int minCol, int maxCol) {
    out << std::setfill(' ') << setw(4) << ' ';
    for (auto col : irange(minCol, maxCol))
    {
      if (col >= 0)
        out << setw(charWidth) << col / 100;
      else
        out << setw(charWidth) << '-';
    }
    out << endl;
    out << std::setfill(' ') << setw(4) << ' ';
    for (auto col : irange(minCol, maxCol))
    {
      out << setw(charWidth) << (abs(col) % 100) / 10;
    }
    out << endl;
    out << std::setfill(' ') << setw(4) << ' ';
    for (auto col : irange(minCol, maxCol))
    {
      out << setw(charWidth) << abs(col) % 10;
    }
    out << endl;
  };

  for (auto z : irange(minPlane, maxPlane))
  {
    if (mode & to2DsFlags::depth_on)
      out << "\nLevel: " << z << endl;

    if (mode & to2DsFlags::cols_count)
      topHeader(minCol, lastCol);

    for (auto l : irange(minLine, lastLine))
    {
      if (mode & to2DsFlags::line_count)
        out << right << std::setfill(' ') << setw(4) << l;

      for (auto c : irange(minCol, lastCol))
      {
        Point crPt{ c, l, z };

        if (auto where = flatList.find(crPt); where != flatList.end())
          out << setw(charWidth) << where->second;
        else
          out << setw(charWidth) << fill;
      }
      out << endl;
    }
  }
  // Sleep(1000);
  // system("pause");
  // out.close();

  return out.str();
}

#endif  // UTILS_H

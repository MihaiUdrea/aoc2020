#include "Utils.h"

using namespace std;

string ReadFileToString(const char * aFileName)
{
  ifstream inFile;
  inFile.open(aFileName);
  if (!inFile)
  {
    wcerr << L"Unable to open file: " << aFileName;
    exit(1);  // call system to stop
  }

  string str;

  inFile.seekg(0, std::ios::end);
  str.reserve((const unsigned int)inFile.tellg());
  inFile.seekg(0, std::ios::beg);

  str.assign((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());

  return str;
}

string ToString(int aNr)
{
  char buff[100];
  _itoa_s(aNr, buff, 10);
  return buff;
}

void toClipboard(const std::string & s)
{
  OpenClipboard(0);
  EmptyClipboard();
  HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
  if (!hg)
  {
    CloseClipboard();
    return;
  }
  memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
  GlobalUnlock(hg);
  SetClipboardData(CF_TEXT, hg);
  CloseClipboard();
  GlobalFree(hg);

  cout << endl << s.c_str() << " is in clipboad." << endl;
}

void toClipboard(int nr)
{
  toClipboard(ToString(nr));
}

void forEachRxToken(const string & inStr, const regex & sepRx, std::function<void(string)> fct)
{
  for (sregex_token_iterator iter(inStr.begin(), inStr.end(), sepRx, -1), end; iter != end; ++iter)
  {
    fct(iter->str());
  }
}

void forEachRxTokenIdx(const string & inStr, const regex & sepRx, std::function<void(string, size_t)> fct)
{
  size_t idx = 0;
  for (sregex_token_iterator iter(inStr.begin(), inStr.end(), sepRx, -1), end; iter != end;
       ++iter, ++idx)
  {
    fct(iter->str(), idx);
  }
}

void forEachLine(const string & inStr, std::function<void(string)> fct)
{
  forEachRxToken(inStr, lineRxToken, fct);
}

void forEachLineIdx(const string & inStr, std::function<void(string, size_t)> fct)
{
  forEachRxTokenIdx(inStr, lineRxToken, fct);
}

vector<string> Tokenize(const string & inStr, const regex & sepRx)
{
  vector<string> list;
  forEachRxToken(inStr, sepRx, [&](string line) {
    list.push_back(line);
  });
  return list;
}

vector<string> GetLines(const string & inStr)
{
  return Tokenize(inStr, lineRxToken);
}

my_int ManhDist(Point a)
{
  return abs(a.x) + abs(a.y) + abs(a.z);
}

void toConsole(Point p, const char * c, int length, int sleep)
{
  DWORD  dw;
  COORD  here;
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

  here.X = (SHORT)p.x;
  here.Y = (SHORT)p.y;
  WriteConsoleOutputCharacterA(hStdOut, c, length, here, &dw);

  if (sleep > 0)
    Sleep(sleep);
}

void toConsole(Point p, string c, int sleep)
{
  DWORD  dw;
  COORD  here;
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

  here.X = (SHORT)p.x;
  here.Y = (SHORT)p.y;
  WriteConsoleOutputCharacterA(hStdOut, c.c_str(), (DWORD)c.size(), here, &dw);

  if (sleep > 0)
    Sleep(sleep);
}

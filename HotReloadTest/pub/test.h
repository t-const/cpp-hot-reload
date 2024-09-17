#pragma once

#include <pub/HotReload.hh>

extern "C"
{
  void foo();

  extern int bar;
}

std::array<std::pair<const char*, void*>, 2> g_exports =
  {std::make_pair("foo", nullptr), std::make_pair("bar", nullptr)};

class TestModule : public HotReloadModule<TestModule, g_exports.size()>
{
  public:
  TestModule()
	: HotReloadModule(g_exports)
  {
  }

  static void Foo()
  {
    // Execute might throw, but we don't bother catching the exception here for brevity
    GetInstance().Execute<0, void>();
  }

  static int GetBar()
  {
    // decltype was a relatively new operator whe this tuto was made.
    // decltype(bar) resolves to int
    // Note that this function does not protect against retrieving nullptr
    return *GetInstance().GetVar<1, decltype(bar)>();
  }

  protected:
  virtual const char* GetPath() const override
  {
#ifdef DEBUG
    return "../lib/libHotReloadTest.so";
#else
    return "../lib/libHotReloadTest.so";
#endif
  }
};
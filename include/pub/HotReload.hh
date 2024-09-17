#pragma once

#include <array>
#include <unordered_map>
#include <string>
#include <stdexcept>

// I think this is only working for linux
#include <dlfcn.h>

template <typename E, std::size_t NumSymbols>
class HotReloadModule
{
  using SymbolArray = std::array<std::pair<const char*, void*>, NumSymbols>;

  public:
  HotReloadModule(SymbolArray& symbols) : m_symbols(symbols) {}

  static void LoadLibrary()
  {
    GetInstance().Load();
  }
  static void ReloadLibrary()
  {
    GetInstance().Reload();
  }

  protected:
  static E& GetInstance()
  {
    static E instance;
    return instance;
  }

  virtual const char* GetPath() const = 0;

  template <std::size_t index, typename Ret, typename... Args>
  Ret Execute(Args... args)
  {
    // lookup the function address
    static_assert(index >= 0 && index < NumSymbols, "Out of bounds symbol index");
    auto [_, symbolHandle] = m_symbols[index];
    return reinterpret_cast<Ret (*)(Args...)>(symbolHandle)(args...);
  }

  template <std::size_t index, typename T>
  T* GetVar()
  {
    static_assert(index >= 0 && index < NumSymbols, "Out of bounds symbol index");
    auto [_, symbolName] = m_symbols[index];
    return static_cast<T*>(symbolName);
  }

  private:
  void Load()
  {
    m_libHandle = dlopen(GetPath(), RTLD_NOW);
    LoadSymbols();
  }

  void Reload()
  {
    dlclose(m_libHandle);
    // todo maybe clear array
    Load();
  }

  void LoadSymbols()
  {
    for(auto&& [symbolName, symbolHandle] : m_symbols)
    {
      symbolHandle = dlsym(m_libHandle, symbolName);
    }
  }

  void* m_libHandle;
  SymbolArray& m_symbols;
};
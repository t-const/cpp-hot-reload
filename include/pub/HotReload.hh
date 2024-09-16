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
  public:
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

  virtual std::array<const char*, NumSymbols>& GetSymbolNames() const = 0;

  template <typename Ret, typename... Args>
  Ret Execute(const char* name, Args... args)
  {
    // lookup the function address
    auto symbol = m_symbols.find(name);
    if(symbol != m_symbols.end())
    {
      // Cast the address to the appropriate function type and call it, forwarding all arguments
      return reinterpret_cast<Ret (*)(Args...)>(symbol->second)(args...);
    }
    else
    {
      throw std::runtime_error(std::string("Function not found: ") + name);
    }
  }

  template <typename T>
  T* GetVar(const char* name)
  {
    auto symbol = m_symbols.find(name);
    if(symbol != m_symbols.end())
    {
      return static_cast<T*>(symbol->second);
    }
    else
    {
      // We didn't find the variable. Return an empty pointer
      return nullptr;
    }
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
    m_symbols.clear();
    Load();
  }

  void LoadSymbols()
  {
    for(const char* symbol : GetSymbolNames())
    {
      m_symbols[symbol] = dlsym(m_libHandle, symbol);
    }
  }

  void* m_libHandle;
  std::unordered_map<std::string, void*> m_symbols;
};
#ifndef EXPOSE_HPP
#define EXPOSE_HPP

#include <sol/sol.hpp>

namespace hyp
{
  namespace binding
  {
    void bindEngineClasses(sol::state &lua);
  };
};

#endif
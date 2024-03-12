#include "lua_bindings.hpp"
#include "shader.hpp"
#include <GLFW/glfw3.h>

using namespace hyp;
namespace lua_bindings
{
  void exposeToLua(sol::state &lua);
}

void hyp::binding::bindEngineClasses(sol::state &lua)
{
  lua_bindings::exposeToLua(lua);
}

namespace lua_bindings
{
  void bindShaderClasses(sol::state &lua)
  {

    sol::table hyp = lua.create_named_table("hyp");

    hyp.set_function("time", glfwGetTime);

    hyp.new_enum<SHADER_TYPE>(
        "ShaderType",
        {{"VERTEX", SHADER_TYPE::VERTEX}, {"FRAGMENT", SHADER_TYPE::FRAGMENT}});

    // shader program
    sol::usertype<ShaderProgram> program_type = hyp.new_usertype<ShaderProgram>(
        "ShaderProgram", sol::constructors<ShaderProgram(),
                                           ShaderProgram(const std::string &vpath, const std::string &fpath)>());
    program_type["link"] = &ShaderProgram::link;
    program_type["use"] = &ShaderProgram::use;
    program_type["setInt"] = &ShaderProgram::setInt;
    program_type["setFloat"] = &ShaderProgram::setFloat;
  }

  void exposeToLua(sol::state &lua)
  {
    bindShaderClasses(lua);
  }
}

#include "lua_bindings.hpp"
#include "shader.hpp"

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

    hyp.new_enum<SHADER_TYPE>(
        "ShaderType",
        {{"VERTEX", SHADER_TYPE::VERTEX}, {"FRAGMENT", SHADER_TYPE::FRAGMENT}});

    // shader
    sol::usertype<Shader>
        shader_type = hyp.new_usertype<Shader>("Shader", sol::constructors<Shader(SHADER_TYPE)>());
    shader_type["compile"] = &Shader::compile;
    shader_type["getShader"] = &Shader::getShader;
    shader_type["getType"] = &Shader::getType;
    shader_type["equal"] = &Shader::operator==;

    // shader program
    sol::usertype<ShaderProgram> program_type = hyp.new_usertype<ShaderProgram>("ShaderProgram", sol::constructors<ShaderProgram()>());
    program_type["attachShader"] = &ShaderProgram::attachShader;
    program_type["link"] = &ShaderProgram::link;
    program_type["use"] = &ShaderProgram::use;
  }

  void exposeToLua(sol::state &lua)
  {
    bindShaderClasses(lua);
  }
}

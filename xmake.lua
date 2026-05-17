add_rules("mode.debug", "mode.release")

-- add_repositories("liteldev-repo https://github.com/LiteLDev/xmake-repo.git")

if is_plat("windows") then
    add_requires("minhook master")
    add_requires("libhat master")
    add_requires("safetyhook main")
    add_requires("imgui v1.92.4-docking", {configs = {dx11 = true, dx12 = true,win32 = true}})
    add_requires("memorymodulepp")
    set_runtimes("MD")
elseif is_plat("android") then   
    add_requires("preloader_android 0.1.13")
end
add_defines(
    "KIERO_INCLUDE_D3D11=1",
    "KIERO_INCLUDE_D3D12=1",
    "KIERO_BUILD_EXAMPLES=0"
)
add_requires("fmt 10.2.1","ctre 3.8.1","magic_enum v0.9.7")
add_requires("entt v3.14.0")
add_requires("gsl v4.0.0")
add_requires("glm 1.0.1")
add_requires("rapidjson v1.1.0")
add_requires("type_safe v0.2.4")
add_requires("expected-lite v0.8.0")
add_requires("nlohmann_json v3.11.3")

target("BetterRenderDragon")
    set_kind("shared")
    set_strip("all")
    set_languages("c++20")
    set_exceptions("none")
    add_headerfiles("src/(**.h)")
    add_includedirs("./src")
    add_includedirs("./include")
    add_defines("_HAS_CXX23=1")
    add_files("src/**.cpp", "resources/Resources.rc")
    add_includedirs("lib/kiero-src/include")
    add_files("lib/kiero-src/src/kiero.cpp")
        add_files("lib/kiero-src/src/hook/kiero_safetyhook.cpp")
    set_symbols("debug")
    if is_plat("windows") then
        add_linkdirs("lib")
        add_packages("memorymodulepp","cpr","detours","fmt","ctre","magic_enum","imgui","nlohmann_json","entt","glm","gsl","rapidjson","type_safe","expected-lite", "minhook", "libhat", "safetyhook")
        remove_files("src/api/memory/android/**.cpp","src/api/memory/android/**.h")
        add_cxflags("/utf-8", "/EHa")
        add_links("ntdll","userenv","materialbin","windowsapp", "d3d11", "d3d12", "dxgi", "d2d1")
    end
    if is_plat("android") then 
        add_cxxflags("-DLLVM_TARGETS_TO_BUILD=\"ARM;AArch64;BPF\"")
        add_packages("preloader_android","cpr","fmt","ctre","magic_enum","imgui","nlohmann_json","entt","glm","gsl","rapidjson","type_safe","expected-lite")
        remove_files("src/api/memory/win/**.cpp","src/api/memory/win/**.h","src/MCPatches.cpp","src/MCPatches.h","src/gui/**.cpp","src/imgui/**.cpp","src/gui/**.h","src/imgui/**.h","src/dllmain.cpp","src/utils/**")
    end

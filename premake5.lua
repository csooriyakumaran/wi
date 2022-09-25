-- premake5.lua
workspace "wi"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "wi"
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "wiExternal.lua"
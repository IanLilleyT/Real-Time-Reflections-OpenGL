--This is a generic 32-bit Windows build script for OpenGL applications
--Author: Ian Lilley

--Name
local name = "OpenGLApplication"

--Directory stuff
local cwd = os.getcwd() .. "/"
local project_location_rel = "../"
local root_location_rel = "../"
local build_location_rel =   project_location_rel .. "build/"        -- Build
local source_location_rel =  project_location_rel .. "src/"          -- Source
local headers_location_rel = root_location_rel .. "include/"         -- Headers
local libs_location_rel =    root_location_rel .. "libs/"            -- Libraries

--SFML
local lib_sfml = libs_location_rel      .. "SFML_2.0/"
local lib_sfml_win32_debug =   lib_sfml .. "Debug/"
local lib_sfml_win32_release = lib_sfml .. "Release/"
local lib_sfml_path = ""

--These functions format the lib files properly
function matchlibs(dir)
	local libs = os.matchfiles(dir .. "*")
	for i=1, #libs do
		libs[i] = string.gsub(libs[i],".lib$", "")
		libs[i] = string.gsub(libs[i],dir, "")
	end
	return libs
end
function addlibs()
	local paths = { lib_sfml_path }
	for i,path in pairs(paths) do
		links(matchlibs(cwd .. path))
		libdirs(path)
	end
end

--Delete the old build folder
os.rmdir( cwd .. string.gsub(build_location_rel,"/$", "") ) 

--Set up debug and release versions
solution ( name )
	configurations { "Debug", "Release" }
	defines {"_CRT_SECURE_NO_WARNINGS"}
	defines {"SFML_STATIC"}
	defines {"SRUTIL_DELEGATE_PREFERRED_SYNTAX"}
	location ( build_location_rel )
	
project ( name )
	kind ("ConsoleApp")
	language ("C++")
	files { source_location_rel .. "**"}           --include all of our source code (resursive)
	files { headers_location_rel .. "gl3w/**" }    --include GL3W
	files { headers_location_rel .. "tinyxml/**" } --include tinyxml
	includedirs { headers_location_rel }           --this accounts for all library headers
	location ( build_location_rel )                --this is where the project is built
	debugdir ( root_location_rel )                 --this is where the IDE-generated-exe accesses data and other folders
	targetdir ( build_location_rel )               --this is where the exe gets built
	targetextension ( ".exe" )
	links( "opengl32" )

	--Debug-----------------------------------
	configuration "Debug"
		flags { "Symbols" }
		defines { "DEBUG" }
		lib_sfml_path = lib_sfml_win32_debug
		addlibs()
		
	--Release---------------------------------	
	configuration "Release"
		flags { "Optimize" }
		defines { "NDEBUG" }
		lib_sfml_path = lib_sfml_win32_release
		addlibs()
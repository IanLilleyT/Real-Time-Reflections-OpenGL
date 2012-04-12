--This is a generic 32-bit Windows build script for OpenGL applications
--Author: Ian Lilley

--Appliaction name
local name = "OpenGLApplication"

--Directory stuff
local cwd = os.getcwd() .. "/"
local project_location_rel = "../"
local build_location_rel =   project_location_rel .. "build/"        -- Build
local source_location_rel =  project_location_rel .. "src/"          -- Source
local data_location_rel =    project_location_rel .. "data/"         -- Data
local headers_location_rel = project_location_rel .. "include/"      -- Headers
local libs_location_rel =    project_location_rel .. "libs/"         -- Libraries

local lib_sfml =   libs_location_rel .. "SFML_2.0/"                  --SFML
local lib_bullet = libs_location_rel .. "bullet_2.79/"               --Bullet

local lib_debug =   "Debug/"
local lib_release = "Release/"

--These functions format the lib files properly
function matchlibs(dir)
	local libs = os.matchfiles(dir .. "*")
	for i=1, #libs do
		libs[i] = string.gsub(libs[i],".lib$", "")
		libs[i] = string.gsub(libs[i],dir, "")
	end
	return libs
end
function addlibs(paths)
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
	files { source_location_rel  .. "**"}          --include all of our source code (resursive)
	files { data_location_rel    .. "**"}          --include all of the data files
	files { headers_location_rel .. "gl3w/**" }    --include GL3W
	files { headers_location_rel .. "tinyxml/**" } --include tinyxml
	includedirs { headers_location_rel }           --this accounts for all library headers
	location ( build_location_rel )                --this is where the project is built
	debugdir ( project_location_rel )              --this is where the IDE-generated-exe accesses data and other folders
	targetdir ( build_location_rel )               --this is where the exe gets built
	targetextension ( ".exe" )                     --Windows executable type
	links( "opengl32" )                            --finds the opengl lib file

	--Debug-----------------------------------
	configuration "Debug"
		flags { "Symbols" }
		defines { "DEBUG" }
		addlibs({lib_sfml   .. lib_debug, 
			     lib_bullet .. lib_debug})
		
	--Release---------------------------------	
	configuration "Release"
		flags { "Optimize" }
		defines { "NDEBUG" }
		addlibs({lib_sfml   .. lib_release, 
			     lib_bullet .. lib_release})
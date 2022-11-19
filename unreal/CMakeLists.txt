if(${CMAKE_VERSION} VERSION_GREATER "3.24.0")
	cmake_policy(SET CMP0135 NEW)
endif()
include(FetchContent)
set(CMAKE_TLS_VERIFY true)
FetchContent_Declare(inklecate_mac
URL https://github.com/sandiz/ink/releases/download/v1.0.1/inklecate_mac.zip
URL_HASH SHA256=5eea97568e9d6eeb682240b86cb3173f807134da53d8fb1e24c8cf9faf08cabb
SOURCE_DIR "inkcpp/Resources/inklecate/mac"
)
FetchContent_Declare(inklecate_windows
URL https://github.com/sandiz/ink/releases/download/v1.0.1/inklecate_win.zip
URL_HASH SHA256=404af7e6205b4625a69ac1451d5da2364760a98268bf0c4dc1e56d31ba48c6e6
SOURCE_DIR "inkcpp/Resources/inklecate/windows"
)
FetchContent_Declare(inklecate_linux
URL https://github.com/sandiz/ink/releases/download/v1.0.1/inklecate_linux.zip
URL_HASH SHA256=d672a0ff5b9d93bc62d76bbfe3b7e14fe282ab87c9b5cdb33687b0a8db22d042
SOURCE_DIR "inkcpp/Resources/inklecate/linux"
)
set(INKLECATE_CMD "")
if(WIN32)
	FetchContent_MakeAvailable(inklecate_windows)
	if(NOT inklecate_windows_SOURCE_DIR)
	message(WARNING "failed to downloawd inklecate for windows, "
		"the unreal plugin will be unable use a .ink file as asset directly")
	else()
	set(INKLECATE_CMD "Resources/inklecate/windows/inklecate.exe")
	endif()
elseif(APPLE)
	FetchContent_MakeAvailable(inklecate_mac)
	if(NOT inklecate_mac_SOURCE_DIR)
		message(WARNING "failed to downloawd inklecate for MacOS, "
			"the unreal plugin will be unable use a .ink file as asset directly")
	else()
	set(INKLECATE_CMD "Resources/inklecate/mac/inklecate")
	endif()
elseif(UNIX)
	FetchContent_MakeAvailable(inklecate_linux)
	if(NOT inklecate_linux_SOURCE_DIR)
		message(WARNING "failed to downloawd inklecate for linux, "
			"the unreal plugin will be unable use a .ink file as asset directly")
	else()
	set(INKLECATE_CMD "Resources/inklecate/linux/inklecate")
	endif()
else()
message(WARNING "unable to determine OS -> unreal component cant compile ink files directly"
	" to fix this may define WIN32/APPLE or UNIX manually and run again")
endif()

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/inkcpp/Source/inkcpp_editor/Private/inklecate_cmd.cpp.in"
	"${CMAKE_CURRENT_BINARY_DIR}/inkcpp/Source/inkcpp_editor/Private/inklecate_cmd.cpp"
)

# Copy files into destination directory
install(DIRECTORY "inkcpp/" DESTINATION "inkcpp" COMPONENT unreal EXCLUDE_FROM_ALL
	PATTERN "*.in" EXCLUDE)
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/inkcpp/" DESTINATION "inkcpp" COMPONENT unreal EXCLUDE_FROM_ALL)

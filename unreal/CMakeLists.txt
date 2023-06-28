if(${CMAKE_VERSION} VERSION_GREATER "3.24.0")
	cmake_policy(SET CMP0135 NEW)
endif()
include(FetchContent)
set(CMAKE_TLS_VERIFY true)
FetchContent_Declare(inklecate_mac
URL https://github.com/sandiz/ink/releases/download/v1.1.1/inklecate_mac.zip
URL_HASH SHA256=402d958b4a5d8b1e7f6a3ccd9271b3ce987e7f29aaa3539dd7e3ba2743b26c76
SOURCE_DIR "inkcpp/Resources/inklecate/mac"
)
FetchContent_Declare(inklecate_windows
URL https://github.com/sandiz/ink/releases/download/v1.1.1/inklecate_win.zip
URL_HASH SHA256=b6c8ba0bddc0b6025f42310215b1ee199a65de812377376e6a0601b9921ad7ea
SOURCE_DIR "inkcpp/Resources/inklecate/windows"
)
FetchContent_Declare(inklecate_linux
URL https://github.com/sandiz/ink/releases/download/v1.1.1/inklecate_linux.zip
URL_HASH SHA256=adbd803024cba05686deb6f05ac37fe90759780a5068d4debf55b60d34743977
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

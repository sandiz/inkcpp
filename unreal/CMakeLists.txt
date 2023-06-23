if(${CMAKE_VERSION} VERSION_GREATER "3.24.0")
	cmake_policy(SET CMP0135 NEW)
endif()
include(FetchContent)
set(CMAKE_TLS_VERIFY true)
FetchContent_Declare(inklecate_mac
URL https://github.com/sandiz/ink/releases/download/v1.0.0-knotlines/inklecate_mac.zip
URL_HASH SHA256=68829666c6ec83a40ec3d998f06069f8235c682ad85ce6d22f86b6ad13477e17
SOURCE_DIR "inkcpp/Resources/inklecate/mac"
)
FetchContent_Declare(inklecate_windows
URL https://github.com/sandiz/ink/releases/download/v1.0.0-knotlines/inklecate_win.zip
URL_HASH SHA256=e4f8b85242c2398f8dca28256b38113da464b0ca8a0582c6efdf455433fa0a2f
SOURCE_DIR "inkcpp/Resources/inklecate/windows"
)
FetchContent_Declare(inklecate_linux
URL https://github.com/sandiz/ink/releases/download/v1.0.0-knotlines/inklecate_linux.zip
URL_HASH SHA256=3984fec5365d9bcdbf2bf0334bc699477c02a08cf2e6cd5cb02a96714e65de44
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

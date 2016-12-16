set (MatterbotSample_SOURCES
	FileBackedLogger.cpp
	main.cpp
)
set (MatterbotSample_HEADERS
	EchoCommand.h
	FileBackedLogger.h
)

find_package (Boost COMPONENTS system filesystem REQUIRED)
find_package (Threads REQUIRED)
find_package (OpenSSL REQUIRED)

include_directories ("${PROJECT_SOURCE_DIR}/Matterbot")

add_executable(MatterbotSample ${MatterbotSample_SOURCES} ${MatterbotSample_HEADERS})

target_link_libraries(MatterbotSample Matterbot ${Boost_SYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} -lcpprest)

set_property(TARGET MatterbotSample PROPERTY CXX_STANDARD 14)
set_property(TARGET MatterbotSample PROPERTY CXX_STANDARD_REQUIRED ON)

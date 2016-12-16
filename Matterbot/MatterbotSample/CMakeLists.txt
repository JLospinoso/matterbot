set (MatterbotSample_SOURCES
	FileBackedLogger.cpp
	main.cpp
)
set (MatterbotSample_HEADERS
	EchoCommand.h
	FileBackedLogger.h
)

add_executable(MatterbotSample ${MatterbotSample_SOURCES} ${MatterbotSample_HEADERS})

target_link_libraries(MatterbotSample Matterbot)

set_property(TARGET MatterbotSample PROPERTY CXX_STANDARD 14)
set_property(TARGET MatterbotSample PROPERTY CXX_STANDARD_REQUIRED ON)

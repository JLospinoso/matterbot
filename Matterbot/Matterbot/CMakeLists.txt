set (Matterbot_SOURCES
	Matterbot.cpp
	MattermostWebhooks.cpp
	Message.cpp
	StdLogger.cpp
	WebResponse.cpp
)
set (Matterbot_HEADERS
	ICommand.h
	Matterbot.h
	MattermostWebhooks.h
	Message.h
	StdLogger.h
	WebResponse.h
)

add_library(Matterbot ${Matterbot_SOURCES} ${Matterbot_HEADERS})

set_property(TARGET Matterbot PROPERTY CXX_STANDARD 14)
set_property(TARGET Matterbot PROPERTY CXX_STANDARD_REQUIRED ON)

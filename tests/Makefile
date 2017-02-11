NAME    = SimpleTest

SOURCES = ../src

SRC     = main.cpp \
	$(SOURCES)/Server.cpp \
	$(SOURCES)/TCPAcceptor.cpp \
	$(SOURCES)/TCPStream.cpp \
	$(SOURCES)/UnixSock.cpp \
	$(SOURCES)/ClientData.cpp \
	../Jasonium/src/Json.cpp \
	../Poolium/src/Mutex.cpp \
	../Poolium/src/Thread.cpp \
	../Emitium/src/EventEmitter.cpp \

OBJ     = $(SRC:.cpp=.o)

CXXFLAGS    = -std=c++11 -W -Wall -Werror -pthread

all: $(NAME)

$(NAME): $(OBJ)
	g++ -pthread $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

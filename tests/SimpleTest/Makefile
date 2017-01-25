NAME    = SimpleTest

SOURCES = ../../src
HEADER  = ../../hdr

SRC     = main.cpp \
	$(SOURCES)/Server.cpp \
	$(SOURCES)/TCPAcceptor.cpp \
	$(SOURCES)/TCPStream.cpp \
	$(SOURCES)/UnixSock.cpp \

OBJ     = $(SRC:.cpp=.o)

CXXFLAGS    = -std=c++11 -W -Wall -Werror

all: $(NAME)

$(NAME): $(OBJ)
	g++ $(CXXFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

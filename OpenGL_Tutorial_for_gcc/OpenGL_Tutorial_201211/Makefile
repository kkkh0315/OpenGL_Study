NAME = tony
CC = g++
SRCS = srcs/*.cpp
OBJS = $(SRCS:.c=.o)
LIBDIR = lib
INCDIR = incs

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(SRCS) -L$(LIBDIR) -lglew3d -lglfw3 -I$(INCDIR)

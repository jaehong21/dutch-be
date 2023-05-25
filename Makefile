CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -pedantic 
CFLAGS += -Icore -Ientity -Irepository -Icontroller -Iutils
OBJ = core/Request.o core/Response.o core/Json.o core/HttpException.o
OBJ += entity/Entity.o entity/UserEntity.o
OBJ += repository/Repository.o repository/FileRepository.o 
OBJ += controller/Controller.o controller/UserController.o
OBJ += utils/Session.o main.o
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

core/Request.o: core/Request.cpp core/Request.h
	$(CC) $(CFLAGS) -c $< -o $@
core/Response.o: core/Response.cpp core/Response.h
	$(CC) $(CFLAGS) -c $< -o $@
core/Json.o: core/Json.cpp core/Json.h
	$(CC) $(CFLAGS) -c $< -o $@
core/HttpException.o: core/HttpException.h
	$(CC) $(CFLAGS) -c $< -o $@ 

entity/Entity.o: entity/Entity.cpp entity/Entity.h
	$(CC) $(CFLAGS) -c $< -o $@
entity/UserEntity.o: entity/UserEntity.cpp entity/UserEntity.h
	$(CC) $(CFLAGS) -c $< -o $@

repository/Repository.o: repository/Repository.h
	$(CC) $(CFLAGS) -c $< -o $@
repository/FileRepository.o: repository/FileRepository.cpp repository/FileRepository.h
	$(CC) $(CFLAGS) -c $< -o $@

controller/Controller.o: controller/Controller.cpp controller/Controller.h
	$(CC) $(CFLAGS) -c $< -o $@
controller/UserController.o: controller/UserController.cpp controller/UserController.h
	$(CC) $(CFLAGS) -c $< -o $@

utils/Session.o: utils/Session.h
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ)

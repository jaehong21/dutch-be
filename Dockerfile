FROM gcc:latest

WORKDIR /app
COPY . .

RUN make clean && make

EXPOSE 8080
ENTRYPOINT ["./a.out"]
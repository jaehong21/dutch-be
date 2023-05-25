#include <iostream>
#include <sstream>
#include <memory>
#include <string>
#include <map>
#include <cstring>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Request.h"
#include "Response.h"
#include "HttpException.h"
#include "Json.h"
#include "FileRepository.h"
#include "UserController.h"

#define LISTEN_PORT 8080
#define MAX_BUFFER_SIZE 2048

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error opening socket" << std::endl;
        return 1;
    }

    struct sockaddr_in serv_addr, cli_addr;
    std::memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(LISTEN_PORT);

    if (bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    if (listen(sockfd, 5) < 0) {
        std::cerr << "Error listening to socket" << std::endl;
        return 1;
    }

    std::cout << "Listening on port 8080..." << std::endl;

    std::shared_ptr<FileRepository> userRepository = std::make_shared<FileRepository>("users.txt");
    std::shared_ptr<FileRepository> accountRepository = std::make_shared<FileRepository>("accounts.txt");
    std::shared_ptr<UserController> userController = UserController::getInstance(userRepository, accountRepository);

    using RequestHandler = std::function<void(int, const Request&)>;
    std::map<std::string, RequestHandler> handlers;

    handlers["POST/user"] = [&userController](int sockfd, const Request& req) {
        userController->createUser(sockfd, req);
    };
    handlers["GET/user/find"] = [&userController](int sockfd, const Request& req) {
        userController->findOneUser(sockfd, req);
    };

    while (true) {
        socklen_t clilen = sizeof(cli_addr);
        int newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);
        if (newsockfd < 0) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        char buffer[MAX_BUFFER_SIZE] = {0};
        ssize_t num_bytes = recv(newsockfd, buffer, sizeof(buffer), 0);
        if (num_bytes < 0) {
            std::cerr << "Error receiving data" << std::endl;
            continue;
        }
        std::string request(buffer, num_bytes);
        const Request req(request);

        std::string key = req.getMethod() + req.getPath();
        try {
            if (handlers.count(key) > 0) {
                handlers[key](newsockfd, req);
            } else {
                // handle case where no route matches...   
                Response response(404, Json("msg", "Not Found"));
                response.execute(newsockfd);
            }
        }
        catch(const HttpException& e) {
            Response response(e.getStatusCode(), Json("msg", e.what()));
            response.execute(newsockfd);
        }


        close(newsockfd);
    }

    close(sockfd);
    return 0;
}

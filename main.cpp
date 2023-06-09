#include <arpa/inet.h>
#include <cstring>
#include <ctime>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "HttpException.h"
#include "Json.h"
#include "Request.h"
#include "Response.h"

#include "AccountController.h"
#include "FileRepository.h"
#include "NormalDutchController.h"
#include "RaceDutchController.h"
#include "UserController.h"

using std::string, std::vector, std::map, std::shared_ptr;

#define LISTEN_PORT 8080
#define MAX_BUFFER_SIZE 2048

int main() {
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

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    if (listen(sockfd, 5) < 0) {
        std::cerr << "Error listening to socket" << std::endl;
        return 1;
    }

    std::cout << "Listening on port 8080..." << std::endl;

    // --- Init repositories and controllers ---

    // uuid, username, password, email
    shared_ptr<FileRepository> userRepository = std::make_shared<FileRepository>("user.txt");
    // uuid, type, balance
    shared_ptr<FileRepository> accountRepository = std::make_shared<FileRepository>("account.txt");
    // uuid, type, owner, targetBalance
    shared_ptr<FileRepository> dutchRepository = std::make_shared<FileRepository>("dutch.txt");
    // uuid, dutch_uuid, user_uuid, amount, send_at
    shared_ptr<FileRepository> ledgerRepository = std::make_shared<FileRepository>("ledger.txt");

    shared_ptr<UserController> userController =
        UserController::getInstance(userRepository, accountRepository);
    shared_ptr<AccountController> accountController =
        AccountController::getInstance(userRepository, accountRepository);
    shared_ptr<NormalDutchController> normalDutchController = NormalDutchController::getInstance(
        userRepository, accountRepository, dutchRepository, ledgerRepository);
    shared_ptr<RaceDutchController> raceDutchController = RaceDutchController::getInstance(
        userRepository, accountRepository, dutchRepository, ledgerRepository);

    // --- Init request handlers ---
    using RequestHandler = std::function<void(int, const Request &)>;
    map<string, RequestHandler> handlers;

    // --- Init user handlers ---
    handlers["POST/user"] = [&userController](int sockfd, const Request &req) {
        userController->createUser(sockfd, req);
    };
    handlers["POST/user/login"] = [&userController](int sockfd, const Request &req) {
        userController->loginUser(sockfd, req);
    };
    handlers["PATCH/user"] = [&userController](int sockfd, const Request &req) {
        userController->updateUser(sockfd, req);
    };
    handlers["GET/user/find"] = [&userController](int sockfd, const Request &req) {
        userController->findOneUser(sockfd, req);
    };
    handlers["GET/user/find/all"] = [&userController](int sockfd, const Request &req) {
        userController->findAllUser(sockfd, req);
    };

    // --- Init account handlers ---
    handlers["GET/account/user"] = [&accountController](int sockfd, const Request &req) {
        accountController->findOneUserAccount(sockfd, req);
    };
    handlers["GET/account/dutch"] = [&accountController](int sockfd, const Request &req) {
        accountController->findOneDutchAccount(sockfd, req);
    };
    handlers["POST/account/user/update"] = [&accountController](int sockfd, const Request &req) {
        accountController->updateUserAccount(sockfd, req);
    };

    // --- Init normal dutch handlers ---
    handlers["GET/dutch/all"] = [&normalDutchController](int sockfd, const Request &req) {
        normalDutchController->findAllDutch(sockfd, req);
    };
    handlers["GET/dutch"] = [&normalDutchController](int sockfd, const Request &req) {
        normalDutchController->findOneDutch(sockfd, req);
    };
    handlers["POST/dutch/normal"] = [&normalDutchController](int sockfd, const Request &req) {
        normalDutchController->createNormalDutch(sockfd, req);
    };
    handlers["POST/dutch/normal/pay"] = [&normalDutchController](int sockfd, const Request &req) {
        normalDutchController->payNormalDutch(sockfd, req);
    };
    handlers["POST/dutch/normal/done"] = [&normalDutchController](int sockfd, const Request &req) {
        normalDutchController->doneNormalDutch(sockfd, req);
    };
    // --- Init race dutch handlers ---
    handlers["POST/dutch/race"] = [&raceDutchController](int sockfd, const Request &req) {
        raceDutchController->createRaceDutch(sockfd, req);
    };
    handlers["POST/dutch/race/pay"] = [&raceDutchController](int sockfd, const Request &req) {
        raceDutchController->payRaceDutch(sockfd, req);
    };
    handlers["POST/dutch/race/done"] = [&raceDutchController](int sockfd, const Request &req) {
        raceDutchController->doneRaceDutch(sockfd, req);
    };

    // --- Start listening for connections ---
    while (true) {
        std::clock_t start = 0;

        socklen_t client = sizeof(cli_addr);
        int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &client);
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
        string request(buffer, num_bytes);
        auto req = Request(request);

        string key = req.getMethod() + req.getPath();
        try {
            if (handlers.count(key) > 0) {
                handlers[key](newsockfd, req);
            } else {
                // handle case where no route matches...
                Response response(404, Json().add("msg", "Not Found"));
                response.execute(newsockfd);
            }
        } catch (const HttpException &e) {
            Response response(e.getStatusCode(), Json().add("msg", e.what()));
            response.execute(newsockfd);
        }

        // HTTP request log for debugging
        auto duration = (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000);
        std::cout << "HTTP 1.1 " << req.getMethod() << " " << req.getPath() << " " << duration
                  << "ms" << std::endl;

        close(newsockfd);
    }

    close(sockfd);
    return 0;
}

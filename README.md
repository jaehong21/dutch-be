# Going Dutch - An Expense Sharing Application

Going Dutch is a program designed to make splitting expenses easy and efficient. Whether you're sharing a meal with friends or planning a joint trip, Going Dutch helps divide the costs evenly and fairly.

This Going Dutch Backend server is implemented with **zero-library**. Every line of code is written from scratch, including the HTTP server, JSON parser, and the file-handling database. The only library used is the standard library.

## Table of Contents

- [Installation and Running the Application](#installation-and-running-the-application)
  - [With Docker (Recommended)](#with-docker)
  - [With Makefile](#with-makefile)
- [Input/Output Sample](#inputoutput-sample)
- [API Documentation](#api-documentation)

## Installation and Running the Application

### With Docker (Recommended)

1. First, ensure you have Docker installed on your machine. If not, follow the installation guide [here](https://docs.docker.com/get-docker/).

2. Clone the repository:

   ```bash
   git clone https://github.com/jaehong21/dutch-be.git
   ```

3. Build the Docker image:

   ```bash
   cd dutch-be
   docker build -t dutch-be .
   ```

4. Run the Docker container:

   ```bash
   # make sure port 8080 is not in use
   docker run -p 8080:8080 dutch-be
   ```

5. The application should now be running at `http://localhost:8080`.

### With Makefile

We strongly use docker rather than makefile. Because makefile can be dependent on the environment of the machine. However, if you want to run the application with makefile, follow the steps below.

1. Ensure you have the necessary dependencies installed on your machine. This includes, but is not limited to, the Make tool and a suitable compiler.

2. Clone the repository:

   ```bash
   git clone https://github.com/jaehong21/dutch-be.git
   ```

3. Run the Makefile:

   ```bash
   cd dutch-be
   make
   ```

4. Start the application:

   ```bash
   ./a.out
   ```

## Input/Output Sample

⚠️⚠️ **_We strongly warn not to call APIs below directly_**, because it can cause unexpected errors. Please use the frontend application to call APIs.

Our API is accessible at `https://api.dutch.jaehong21.com`.

### User

- **POST /user**

  - Creates a new user.
  - Required fields: `username`, `password`, `email`

- **POST /user/login**

  - Logs a user in.
  - Required fields: `username`, `password`

- **PATCH /user**

  - Updates a user's information.
  - Required fields: `uuid`, `username`

- **GET /user/find**

  - Fetches user data using the `uuid`.
  - Required fields: `uuid`

- **GET /user/find/all**
  - Fetches all users. No query parameters required.

### Account

- **GET /account/user**

  - Fetches a user's account data using the `uuid`.
  - Required fields: `uuid`

- **GET /account/dutch**

  - Fetches a user's dutch (expense sharing) data using the `uuid`.
  - Required fields: `uuid`

- **POST /account/user/update**
  - Updates a user's account balance.
  - Required fields: `uuid`, `balance`

### Dutch

- **GET /dutch/all**

  - Fetches all dutch (expense sharing) data for a user using the `user_uuid`.
  - Required fields: `user_uuid`

- **GET /dutch**

  - Fetches a specific dutch (expense sharing) data using the `dutch_uuid`.
  - Required fields: `dutch_uuid`

- **POST /dutch/normal**

  - Creates a normal dutch (expense sharing) entry.
  - Required fields: `owner`, `target_balance`, `user_list`

- **POST /dutch/normal/pay**

  - Submits a payment for a normal dutch (expense sharing) entry.
  - Required fields: `dutch_uuid`, `user_uuid`

- **POST /dutch/normal/done**

  - Marks a normal dutch (expense sharing) entry as done.
  - Required fields: `dutch_uuid`, `user_uuid`

- **POST /dutch/race**

  - Creates a race dutch (expense sharing) entry.
  - Required fields: `owner`, `target_balance`, `user_list`

- **POST /dutch/race/pay**

  - Submits a payment for a race dutch (expense sharing) entry.
  - Required fields: `dutch_uuid`, `user_uuid`

- **POST /dutch/race/done**
  - Marks a race dutch (expense sharing) entry as done.
  - Required fields: `dutch_uuid`, `user_uuid`

#include <iostream>
#include <crow.h>

int main()
{
    crow::SimpleApp app;

    // Return Hello world at API root
    CROW_ROUTE(app, "/")([]() {
        return "Hello world";
        });

    // 1. Set the port.
    // 2. Enable multithreading.
    // 3. Run app.
    app.port(18080).multithreaded().run();
}

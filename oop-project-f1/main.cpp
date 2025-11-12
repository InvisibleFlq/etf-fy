#include "CommandHandler.h"

int main() {
    auto *commandHandler = new CommandHandler("luka@pc ~ $ ");
    commandHandler->start();
    delete commandHandler;
    return 0;
}
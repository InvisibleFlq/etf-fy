cmake_minimum_required(VERSION 3.30)
project(oop_project_f1)

set(CMAKE_CXX_STANDARD 20)

add_executable(oop_project_f1 main.cpp
        CommandHandler.cpp
        CommandHandler.h
        Parser.cpp
        Reader.cpp
        Command.h
        commands/EchoCommand.h
        commands/ImportCommands.h
        commands/WordCountCommand.h
        commands/TouchCommand.h
        commands/DateCommand.h
        commands/TimeCommand.h
        commands/PromptCommand.h
        CommandBuilder.cpp
        CommandBuilder.h
        commands/ExitCommand.h
        commands/TruncateCommand.h
        commands/RemoveCommand.h
        commands/HeadCommand.h
        Exceptions.h
        Pipeline.h
        commands/TrCommand.h
        commands/BatchCommand.h
)

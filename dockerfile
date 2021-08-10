FROM gcc

RUN apt -y update \
    && apt -y install make gdb cppcheck
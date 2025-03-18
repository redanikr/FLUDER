FROM ubuntu:latest

RUN apt-get update -y
RUN apt-get install wget -y
RUN apt-get install clang -y
RUN apt-get install cmake -y
RUN apt-get install ninja-build -y
RUN apt-get update && apt-get install -y gnupg2
RUN wget -qO - https://packages.lunarg.com/lunarg-signing-key-pub.asc | apt-key add -
RUN wget -qO /etc/apt/sources.list.d/lunarg-vulkan-1.4.309-noble.list https://packages.lunarg.com/vulkan/1.4.309/lunarg-vulkan-1.4.309-noble.list
RUN apt-get update
RUN apt-get install vulkan-sdk -y
RUN apt-get install libglfw3 -y
RUN apt-get install libglfw3-dev -y
RUN apt-get install libxkbcommon-dev xorg-dev -y
RUN apt-get install libxkbcommon-dev -y
RUN apt-get install xorg-dev -y

WORKDIR /app
CMD [ "./start.sh" ]
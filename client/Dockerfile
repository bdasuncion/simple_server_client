FROM ubuntu

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update
RUN apt-get -y --no-install-recommends install build-essential cmake

WORKDIR /app

COPY ./ /app
RUN cd /app
RUN cmake .
RUN cmake --build .
#COPY ./Client /app


#CMD ["./Client"]
CMD ["bash"]
FROM ubuntu:20.04 AS base

RUN apt-get update                     && \
    apt-cache search libssl-dev        && \
    apt-get install -y build-essential    \
                       openssl            \
                       libssl-dev



FROM base AS build

WORKDIR /home/app

COPY . .

RUN make ssl



FROM ubuntu:20.04 AS production

ARG PORT=8080
ENV PORT=$PORT

RUN apt-get update &&                     \
    apt-get install openssl libssl-dev -y

WORKDIR /home/app

COPY ./www ./www/
COPY --from=build /home/app/httpserve .
COPY --from=build /home/app/cgi-bin ./cgi-bin/

RUN rm -rf cgi-bin/*.c
RUN rm -rf src

RUN openssl genrsa -out server.key 2048
RUN openssl req         \
        -new            \
        -x509           \
        -sha256         \
        -key server.key \
        -out server.crt \
        -days 365       \
        -subj "/C=US/ST=Utah/L=Lehi/O=Your Company, Inc./OU=IT/CN=yourdomain.com"

EXPOSE $PORT

CMD ["./httpserve"]
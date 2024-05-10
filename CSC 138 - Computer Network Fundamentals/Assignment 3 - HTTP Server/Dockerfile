FROM ubuntu:20.04 AS base

RUN apt-get update                     && \
    apt-cache search libssl-dev        && \
    apt-get install -y build-essential    \
                       openssl            \
                       libssl-dev



FROM base AS build

WORKDIR /user/app

COPY . .

RUN ls -la

RUN make



FROM ubuntu:20.04 AS production

RUN apt-get update &&                     \
    apt-get install openssl libssl-dev -y

WORKDIR /user/app

COPY www .
COPY --from=build /user/app/httpserve .
COPY --from=build /user/app/cgi-bin .

RUN rm -rf cgi-bin/*.c

RUN openssl genrsa -out server.key 2048
RUN openssl req         \
        -new            \
        -x509           \
        -sha256         \
        -key server.key \
        -out server.crt \
        -days 365       \
        -subj "/C=US/ST=Utah/L=Lehi/O=Your Company, Inc./OU=IT/CN=yourdomain.com"

CMD [ "./httpserve" ]
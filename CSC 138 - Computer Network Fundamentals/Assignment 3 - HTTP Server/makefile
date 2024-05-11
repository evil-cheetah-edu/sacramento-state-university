# Compiler settings - Can change to clang or others
CC = gcc
CFLAGS = -Wall
LDLIBS = -lssl -lcrypto

# Target executable name
TARGET = httpserve
SOURCE = src/httpserve


# Build Base Version (HTTP)
base: $(SOURCE).c testcgi
	@$(CC) $(CFLAGS) $(SOURCE).c -o $(TARGET)

# Build SSL Version (HTTPS)
ssl: $(SOURCE).ssl.c testcgi
	@$(CC) $(CFLAGS) $(SOURCE).ssl.c -o $(TARGET) $(LDLIBS)


# Run Executable
run:
	@./$(TARGET)

# CGI Scripts
testcgi:
	@$(CC) $(CFLAGS) -o cgi-bin/testcgi cgi-bin/testcgi.c


# OpenSSL Certification
openssl: generate_rsa
	@openssl req        \
		-new            \
		-x509           \
		-sha256         \
		-key server.key \
		-out server.crt \
		-days 1

# RSA Key Generation
generate_rsa:
	@openssl genrsa 		 \
		-out server.key 2048


# Clean up generated files
clean:
	@rm -f 		        \
		$(TARGET)       \
		cgi-bin/testcgi \
		server.crt		\
		server.key
		
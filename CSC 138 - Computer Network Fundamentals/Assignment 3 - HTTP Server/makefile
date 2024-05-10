# Compiler settings - Can change to clang or others
CC = gcc
CFLAGS = -Wall

# Target executable name
TARGET = httpserve
SOURCE = src/httpserve


# Build target
$(TARGET): $(SOURCE).c testcgi
	@$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE).c


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
		-days 365

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
		
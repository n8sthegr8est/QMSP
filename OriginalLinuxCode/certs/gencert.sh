#!/bin/bash

openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout quic_private_key.pem -out quic_certificate.pem -subj "/CN=localhost" -addext "subjectAltName = DNS:localhost, IP:127.0.0.1"
#openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout quic_private_key.pem -out quic_certificate.pem -subj "/CN=localhost"
#openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout server.key -out server.crt -subj "/CN=localhost"
#openssl ecparam -genkey  -name prime256v1 -out quic_private_key.pem

#openssl req -new -x509 -key quic_private_key.pem -out quic_certificate.pem -days 365 \
#	-subj "/CN=localhost" \
#	-addext "subjectAltName = DNS:localhost, IP:127.0.0.1"

#openssl req -new -x509 -config certificate.conf -key quic_private_key.pem -out quic_csr.pem

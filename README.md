# embedded_webserver
A tiny HTTPS server written in C++ around Boost::Beast

## generate certificates for testing
```sh
mkdir -p ./build/ssl
cd ./build/ssl

# https://stackoverflow.com/a/10176685
openssl req -x509 -newkey rsa:4096 -keyout localhost_private.key -out localhost_certificate.crt -days 365 \
    -passout pass:private_key_password \
    -subj "/CN=localhost"
openssl ecparam -name prime256v1 -out diffey_hellman.pem
```

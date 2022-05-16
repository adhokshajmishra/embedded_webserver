//
// Created by adhokshajmishra on 9/10/20.
//

#ifndef FLEET_SSL_CERTIFICATE_H
#define FLEET_SSL_CERTIFICATE_H

#include <boost/asio/buffer.hpp>
#include <boost/asio/ssl/context.hpp>
#include <cstddef>
#include <memory>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>

/*  Load a signed certificate into the ssl context, and configure
    the context for use with a server.

    For this to work with the browser or operating system, it is
    necessary to import the "Beast Test CA" certificate into
    the local certificate store, browser, or operating system
    depending on your environment Please see the documentation
    accompanying the Beast certificate for more details.
*/
inline
void
load_server_certificate(boost::asio::ssl::context& ctx, std::string ssl_certificate, std::string ssl_private_key,
                        std::string diffie_hellman_key, std::string private_key_password)
{
    try
    {
        if (!std::filesystem::exists(ssl_certificate))
        {
            std::cerr << "Error: SSL certificate file does not exist." << std::endl;
            return;
        }
        if (!std::filesystem::is_regular_file(ssl_certificate))
        {
            std::cerr << "Error: SSL certificate file is not a regular file." << std::endl;
            return;
        }
        if (std::filesystem::is_empty(ssl_certificate))
        {
            std::cerr << "Error: SSL certificate file is empty." << std::endl;
            return;
        }

        if (!std::filesystem::exists(ssl_private_key))
        {
            std::cerr << "Error: SSL private key does not exist." << std::endl;
            return;
        }
        if (!std::filesystem::is_regular_file(ssl_private_key))
        {
            std::cerr << "Error: SSL private key file is not a regular file." << std::endl;
            return;
        }
        if (std::filesystem::is_empty(ssl_private_key))
        {
            std::cerr << "Error: SSL private key file is empty." << std::endl;
            return;
        }

        if (!std::filesystem::exists(diffie_hellman_key))
        {
            std::cerr << "Error: Diffie-Hellman key file does not exist." << std::endl;
            return;
        }
        if (!std::filesystem::is_regular_file(diffie_hellman_key))
        {
            std::cerr << "Error: Diffie-Hellman key file is not a regular file." << std::endl;
            return;
        }
        if (std::filesystem::is_empty(diffie_hellman_key))
        {
            std::cerr << "Error: Diffie-Hellman key file is empty." << std::endl;
            return;
        }
    }
    catch (const std::filesystem::filesystem_error& error)
    {
        std::cerr << "Error (" << error.code() << ") while validating SSL file paths file path: " << error.what() << std::endl;
        return;
    }

    std::ifstream file(ssl_certificate);
    std::string ssl_cert((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    file.open(ssl_private_key);
    std::string ssl_key((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    file.open(diffie_hellman_key);
    std::string dh_key((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    ctx.set_password_callback(
            [private_key_password](std::size_t,
               boost::asio::ssl::context_base::password_purpose)
            {
                return private_key_password;
            });

    ctx.set_options(
            boost::asio::ssl::context::default_workarounds |
            boost::asio::ssl::context::no_sslv2 |
            boost::asio::ssl::context::single_dh_use);

    ctx.use_certificate_chain(
            boost::asio::buffer(ssl_cert.data(), ssl_cert.size()));

    ctx.use_private_key(
            boost::asio::buffer(ssl_key.data(), ssl_key.size()),
            boost::asio::ssl::context::file_format::pem);

    ctx.use_tmp_dh(
            boost::asio::buffer(dh_key.data(), dh_key.size()));
}

#endif //FLEET_SSL_CERTIFICATE_H

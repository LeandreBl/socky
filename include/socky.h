#ifndef SOCKY_H_
#define SOCKY_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/**
 * \enum socky_protocol
 * 
 * \brief The different protocols supported by socky.
 * 
 * \var SOCKY_TCP: The TCP protocol.
 * \var SOCKY_UDP: The UDP protocol.
 */
enum socky_protocol {
    SOCKY_TCP = SOCK_STREAM,
    SOCKY_UDP = SOCK_DGRAM
};

/**
 * \enum socky_type
 * 
 * \brief The different types of sockets supported by socky.
 * 
 * \var SOCKY_LISTENER: a socket capable of accepting connections.
 * \var SOCKY_DUPLEX: a full duplex connected socket, for reading and writing.
 */
enum socky_type {
    SOCKY_LISTENER,
    SOCKY_DUPLEX
};

/**
 * \enum socky_state
 * 
 * \brief The different states of a socket.
 * 
 * \var SOCKY_CREATED: The socket is only created, not specialized.
 * \var SOCKY_CLOSED: The socket is closed.
 * \var SOCKY_LISTENING: The socket is listening for connections.
 * \var SOCKY_CONNECTED: The socket is connected.
 */
enum socky_state {
    SOCKY_CREATED,
    SOCKY_CLOSED,
    SOCKY_LISTENING,
    SOCKY_CONNECTED,
};

/**
 * \struct socky
 * \brief The socky structure.
 * 
 * The socky structure is used to open a socket
 * and to provide a simple interface to manipulate it.
 * 
 * \var fd: The socket file descriptor.
 * \var addr: The socket address.
 * \var proto: The protocol of the socket.
 * \var type: The type of the socket.
 * \var state: The state of the socket.
 */
struct socky {
    int fd;
    struct sockaddr_in addr;
    enum socky_protocol proto;
    enum socky_type type;
    enum socky_state state;
};

/**
 * \fn int socky_create(struct socky *socky, enum socky_protocol protocol)
 * 
 * \brief Open a socket and set it's protocol for later use.
 * 
 * \param socky The socket to create.
 * \param proto The protocol of the socket.
 * 
 * \return 0 on success, -1 on error, errno is set accordingly.
 */
int socky_create(struct socky *socky, enum socky_protocol protocol) __nonnull((1));

/**
 * \fn int socky_set_options(struct socky *socky, int options)
 * 
 * \brief Set the options of the socket.
 * 
 * \note This function is a wrapper around setsockopt.
 * 
 * \param socky The socket to set the options of.
 * \param options The options to set (see man 7 socket).
 * 
 * \return 0 on success, -1 on error, errno is set accordingly.
 */
int socky_set_options(struct socky *socky, int options) __nonnull((1));

/**
 * \fn int socky_listen(struct socky *socky, size_t waiting_list_size)
 * 
 * \brief Set the socket to listen mode.
 * 
 * \param socky The socket to listen on.
 * \param port The port to listen on, 0 for any available port.
 * \param waiting_list_size The size of the waiting list.
 * 
 * \return 0 on success, -1 on error, errno is set accordingly.
 */
int socky_listen(struct socky *socky, uint16_t port, size_t waiting_list_size) __nonnull((1));

/**
 * \fn int socky_get_port(const struct socky *socky, uint16_t *pport)
 * 
 * \brief Get the port used by the socket.
 *
 * \param socky The socket to get the port from.
 * \param pport The address of the port variable to fill.
 * 
 * \return 0 on success, -1 on error, errno is set accordingly.
 */
int socky_get_port(const struct socky *socky, uint16_t *pport) __nonnull((1, 2));

/**
 * \fn int socky_get_address_str(const struct socky *socky, char **pstr)
 * 
 * \brief Get the address of the socket as a string.
 * 
 * \param socky The socket to get the address from.
 * \param pstr The address of the string variable to fill, this pointer needs to be freed.
 * 
 * \return 0 on success, -1 on error, errno is set accordingly.
 */
int socky_get_address_str(const struct socky *socky, char **pstr) __nonnull((1));

/**
 * \fn int socky_get_address(const struct socky *socky, uint32_t *paddr)
 * 
 * \brief Get the address of the socket.
 * 
 * \param socky The socket to get the address from.
 * \param paddr The address of the variable to fill.
 * 
 * \return 0 on success, -1 on error, errno is set accordingly.
 */
int socky_get_address(const struct socky *socky, uint32_t *paddr) __nonnull((1, 2));

/**
 * \fn int socky_accept(const struct socky *socky, struct socky *new_uninitialized_socky)
 * 
 * \brief Accept a connection on a listening socket.
 * 
 * \param socky The listening socket.
 * \param new_uninitialized_socky The new socket to fill.
 * 
 * \return 0 on success, -1 on error, errno is set accordingly.
 */
int socky_accept(const struct socky *socky, struct socky *new_uninitialized_socky) __nonnull((1, 2));

/**
 * \fn int socky_connect(struct socky *socky, uint32_t address, uint16_t port)
 * 
 * \brief Connect a socket to an address.
 * 
 * \param socky The socket to connect.
 * \param address The address to connect to.
 * \param port The port of the address to connect to.
 * 
 * \return 0 on success, -1 on error, errno is set accordingly.
 */
int socky_connect(struct socky *socky, uint32_t address, uint16_t port) __nonnull((1));

/**
 * \fn int socky_connect_from_string(struct socky *socky, const char *address_as_string, uint16_t port)
 * 
 * \brief Connect a socket to a string address (or a hostname, ex: "www.google.com" or "10.41.0.0").
 * 
 * \param socky The socket to connect.
 * \param address_as_string The string address to connect to.
 * \param port The port of the address to connect to.
 * 
 * \return 0 on success, -1 on error, errno is set accordingly.
 */
int socky_connect_from_string(struct socky *socky, const char *address_as_string, uint16_t port) __nonnull((1, 2));

/**
 * \fn ssize_t socky_write(const struct socky *socky, const void *data, size_t size)
 * 
 * \brief Write data to a socket.
 * 
 * \note This function is equivalent to socky_send(socky, data, size, 0).
 * 
 * \param socky The socket to write to.
 * \param data The data to write.
 * \param size The size of the data to write.
 * 
 * \return The number of bytes written on success, -1 on error, errno is set accordingly.
 */
ssize_t socky_write(const struct socky *socky, const void *data, size_t size) __nonnull((1, 2));

/**
 * \fn ssize_t socky_send(const struct socky *socky, const void *data, size_t size, int flags)
 * 
 * \brief Send data to a socket.
 * 
 * \param socky The socket to send to.
 * \param data The data to send.
 * \param size The size of the data to send.
 * \param flags The flags to use (see man 2 send)
 * 
 * \return The number of bytes sent on success, -1 on error, errno is set accordingly.
 */
ssize_t socky_send(const struct socky *socky, const void *data, size_t size, int flags) __nonnull((1, 2));

/**
 * \fn ssize_t socky_read(const struct socky *socky, void *data, size_t size)
 * 
 * \brief Read data from a socket.
 * 
 * \note This function is equivalent to socky_recv(socky, data, size, 0).
 * 
 * \param socky The socket to read from.
 * \param data The buffer to fill with the data.
 * \param size The size of the buffer.
 * 
 * \return The number of bytes read on success, -1 on error, errno is set accordingly.
 */
ssize_t socky_read(const struct socky *socky, void *data, size_t size) __nonnull((1, 2));

/**
 * \fn ssize_t socky_recv(const struct socky *socky, void *data, size_t size, int flags)
 * 
 * \brief Receive data from a socket.
 * 
 * \param socky The socket to receive from.
 * \param data The buffer to fill with the data.
 * \param size The size of the buffer.
 * 
 * \return The number of bytes received on success, -1 on error, errno is set accordingly.
 */
ssize_t socky_recv(const struct socky *socky, void *data, size_t size, int flags) __nonnull((1, 2));

/**
 * \enum socky_shutdown_mode
 * 
 * \brief The shutdown mode of a socket.
 * 
 * \var SOCKY_SHUTDOWN_READ, shutdown the reading endpoint of a socket
 * \var SOCKY_SHUTDOWN_WRITE, shutdown the writing endpoint of a socket
 * \var SOCKY_SHUTDOWN_READ_WRITE, shutdown both endpoints of a socket
 */
enum socky_shutdown_mode {
    SOCKY_SHUTDOWN_READ = SHUT_RD,
    SOCKY_SHUTDOWN_WRITE = SHUT_WR,
    SOCKY_SHUTDOWN_READ_WRITE = SHUT_RDWR
};

/**
 * \fn int socky_shutdown(const struct socky *socky, int how)
 * 
 * \brief Shutdown a socket.
 * 
 * \param socky The socket to shutdown.
 * \param how The way to shutdown the socket (see man 2 shutdown)
 * 
 * \return 0 on success, -1 on error, errno is set accordingly.
 */
int socky_shutdown(struct socky *socky, enum socky_shutdown_mode how) __nonnull((1));

/**
 * \fn int socky_destroy(struct socky *socky)
 * 
 * \brief Destroy a socket.
 * 
 * \param socky The socket to destroy.
 * 
 * \return 0 on success, -1 on error, errno is set accordingly.
 */
int socky_destroy(struct socky *socky) __nonnull((1));

#endif /* !SOCKY_H_ */

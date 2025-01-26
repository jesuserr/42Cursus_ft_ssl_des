/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:11:34 by jesuserr          #+#    #+#             */
/*   Updated: 2025/01/26 19:41:12 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              HEADERS
*/
# include "../../libft/includes/libft.h"			// libft library
# include "../../libft/includes/ft_printf.h"		// ft_printf
# include "../../libft/includes/get_next_line.h"	// get_next_line
# include <stdint.h>							// for fixed-width integer types
# include <stdbool.h>							// for booleans
# include "types_hash.h"						// for t_hash_args
# include "types_encode.h"						// for t_encode_args
# include "types_encrypt.h"						// for t_encrypt_args
# include "md5.h"								// for MD5 hash function
# include "sha256.h"							// for SHA256 hash function
# include "sha224.h"							// for SHA224 hash function
# include "sha384.h"							// for SHA384 hash function
# include "sha512.h"							// for SHA512 hash function
# include "base64.h"							// for base64 encode function
# include "des.h"						    	// for DES encrypt function
# include <string.h>							// for strerror
# include <fcntl.h>								// for open
# include <errno.h>								// for errno
# include <sys/stat.h>							// for fstat
# include <sys/mman.h>							// for mmap/munmap
# include <bits/getopt_core.h>	// Delete, just to fix intellisense vscode error

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              DEFINES
*/
# define HASH_COMMAND       1           // Pre-parser detected a hash command
# define ENCODE_COMMAND     2           // Pre-parser detected an encode command
# define ENCRYPT_COMMAND    3           // Pre-parser detected encrypt command

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                        FUNCTION PROTOTYPES
*/
/********************************** bitwise_utils.c ***************************/
uint32_t	right_rotation(uint32_t nbr, uint8_t bits);
uint32_t	left_rotation(uint32_t nbr, uint8_t bits);
void		modify_endianness_32_bits(uint32_t *nbr);
void		modify_endianness_64_bits(uint64_t *nbr);
uint64_t	right_rotation_64(uint64_t nbr, int8_t bits);

/********************************** common_utils.c ****************************/
void		read_interactive_mode(char **input_pipe, uint64_t *pipe_size);

/********************************** encode_parser.c ***************************/
void		parse_encode_arguments(int argc, char **argv, t_encode_args *args);

/********************************** encode_utils.c ****************************/
void		calls_to_decoding_function(t_encode_args *args);
void		print_encode_usage(void);
void		print_encode_strerror_and_exit(char *msg, t_encode_args *args);
void		remove_message_whitespaces_and_newlines(t_encode_args *args);

/********************************** encrypt_parser.c **************************/
void		parse_encrypt_arguments(int argc, char **argv, t_encrypt_args *arg);

/********************************** encrypt_utils.c ***************************/
void		calls_to_encrypt_function(t_encrypt_args *args);
void		print_encrypt_usage(void);
void		print_encrypt_strerror_and_exit(char *msg, t_encrypt_args *args);

/********************************** hash_parser.c *****************************/
void		parse_hash_arguments(int argc, char **argv, t_hash_args *args);

/********************************** hash_utils.c ******************************/
void		calls_to_hashing_function(t_hash_args *args);
void		print_hash_usage(void);
void		print_hash_strerror_and_exit(char *msg, t_hash_args *args);
void		print_prehash_output(char *algorithm, t_hash_args *args);
void		print_message_from_pipe(t_hash_args *args);

/********************************** print_utils.c *****************************/
void		print_hex_bytes(uint8_t *byte, uint8_t start, uint8_t end);
void		print_error_and_exit(char *str);
void		print_total_usage(void);

#endif

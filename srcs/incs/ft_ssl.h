/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 17:11:34 by jesuserr          #+#    #+#             */
/*   Updated: 2024/12/09 23:02:09 by jesuserr         ###   ########.fr       */
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
# include "types.h"								// for t_arguments
# include "md5.h"								// for MD5 hash function
# include "sha256.h"							// for SHA256 hash function
# include "sha224.h"							// for SHA224 hash function
# include "sha384.h"							// for SHA384 hash function
# include "sha512.h"							// for SHA512 hash function
# include <string.h>							// for strerror
# include <fcntl.h>								// for open
# include <errno.h>								// for errno
# include <sys/stat.h>							// for fstat
# include <sys/mman.h>							// for mmap/munmap
# include <bits/getopt_core.h>	// Delete, just to fix intellisense vscode error

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                        FUNCTION PROTOTYPES
*/
/********************************** parser.c **********************************/
void		parse_arguments(int argc, char **argv, t_arguments *args);

/********************************** bitwise_utils.c ***************************/
uint32_t	right_rotation(uint32_t nbr, uint8_t bits);
uint32_t	left_rotation(uint32_t nbr, uint8_t bits);
void		modify_endianness_32_bits(uint32_t *nbr);
void		modify_endianness_64_bits(uint64_t *nbr);
uint64_t	right_rotation_64(uint64_t nbr, int8_t bits);

/********************************** print_utils.c *****************************/
void		print_usage(void);
void		print_hex_bytes(uint8_t *byte, uint8_t start, uint8_t end);
void		print_strerror_and_exit(char *msg, t_arguments *args);
void		print_error_and_exit(char *str);
void		print_prehash_output(char *algorithm, t_arguments *args);

/********************************** interactive.c *****************************/
void		read_interactive_mode(t_arguments *args);
void		remove_newline_character(char *msg, uint64_t len);

#endif

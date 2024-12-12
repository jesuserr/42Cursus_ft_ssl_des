/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:31:27 by jesuserr          #+#    #+#             */
/*   Updated: 2024/12/09 22:48:01 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              HEADERS
*/
# include <stdbool.h>			// for booleans

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              STRUCTS
*/
typedef struct s_arguments
{
	char		*input_str;
	char		*input_file;
	char		*input_pipe;
	char		*message;
	char		*file_name;
	uint64_t	file_size;
	uint64_t	pipe_size;
	uint8_t		hash_function;
	uint8_t		msg_origin;
	bool		echo_stdin;
	bool		quiet_mode;
	bool		reverse_output;
	bool		print_sum;
}	t_arguments;

enum	e_words
{
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H
};

enum	e_origin
{
	IS_PIPE,
	IS_STRING,
	IS_FILE
};

#endif

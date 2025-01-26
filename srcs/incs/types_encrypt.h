/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types_encrypt.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:48:16 by jesuserr          #+#    #+#             */
/*   Updated: 2025/01/26 19:20:22 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_ENCRYPT_H
# define TYPES_ENCRYPT_H

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              STRUCTS
*/
typedef struct s_encrypt_args
{
	char		*input_pipe;
	char		*input_file;
	char		*message;
	char		*input_file_name;
	uint64_t	input_file_size;
	uint64_t	pipe_size;
	uint64_t	message_length;
	int			output_fd;
	bool		decrypt_mode;
	bool		encrypt_mode;
	bool		input_from_file;
	bool		output_to_file;
}	t_encrypt_args;

#endif

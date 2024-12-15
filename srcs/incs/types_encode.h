/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types_encode.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 18:27:04 by jesuserr          #+#    #+#             */
/*   Updated: 2024/12/15 17:50:41 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_ENCODE_H
# define TYPES_ENCODE_H

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              STRUCTS
*/
typedef struct s_encode_args
{
	char		*input_pipe;
	char		*input_file;
	char		*message;
	char		*input_file_name;
	char		*output_file_name;
	uint64_t	input_file_size;
	uint64_t	pipe_size;
	uint8_t		msg_origin;
	bool		decode_mode;
	bool		encode_mode;
	bool		input_from_file;
	bool		output_to_file;
}	t_encode_args;

#endif

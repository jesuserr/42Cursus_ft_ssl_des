/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types_enc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 18:27:04 by jesuserr          #+#    #+#             */
/*   Updated: 2024/12/12 20:34:43 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_ENC_H
# define TYPES_ENC_H

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              STRUCTS
*/
typedef struct s_enc_args
{
	bool		decode_mode;
	bool		encode_mode;
	bool		input_file;
	bool		output_file;
}	t_enc_args;

#endif

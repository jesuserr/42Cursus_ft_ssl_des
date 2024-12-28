/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 17:55:08 by jesuserr          #+#    #+#             */
/*   Updated: 2024/12/28 22:52:37 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASE64_H
# define BASE64_H

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              HEADERS
*/
# include "types_encode.h"

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              DEFINES
*/
# define BASE64_INPUT_BLOCKS			3			// 3 * 8 bits (24 bits)
# define BASE64_OUTPUT_BLOCKS			4			// 4 * 6 bits (24 bits)
# define BASE64_LINE					64			// 64 characters per line

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                              STRUCTS
*/
typedef struct s_base64_data
{
	t_encode_args		*args;			// Passed as a pointer to avoid copying
}	t_base64_data;

static const uint8_t	g_base64_table[64] = \
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*
** -.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-.-'-
**                        FUNCTION PROTOTYPES
*/
void	base64(t_encode_args *args);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 17:53:09 by jesuserr          #+#    #+#             */
/*   Updated: 2024/12/28 22:54:45 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/ft_ssl.h"

static void	convert_triplet(uint8_t *triplet, uint8_t scenario)
{
	uint8_t		output[BASE64_OUTPUT_BLOCKS];

	output[0] = g_base64_table[triplet[0] >> 2];
	if (scenario == 1)
	{
		output[1] = g_base64_table[triplet[1] >> 4 | (triplet[0] & 0x03) << 4];
		output[2] = g_base64_table[triplet[2] >> 6 | (triplet[1] & 0x0F) << 2];
		output[3] = g_base64_table[triplet[2] & 0x3F];
	}
	else if (scenario == 2)
	{
		output[1] = g_base64_table[(triplet[0] & 0x03) << 4];
		output[2] = '=';
		output[3] = '=';
	}
	else if (scenario == 3)
	{
		output[1] = g_base64_table[triplet[1] >> 4 | (triplet[0] & 0x03) << 4];
		output[2] = g_base64_table[(triplet[1] & 0x0F) << 2];
		output[3] = '=';
	}
	ft_printf("%c%c%c%c", output[0], output[1], output[2], output[3]);
}

static void	encode_message(t_base64_data *base64_data)
{
	uint8_t		triplet[BASE64_INPUT_BLOCKS];
	uint64_t	i;

	i = 0;
	while (i < (base64_data->args->message_length / BASE64_INPUT_BLOCKS) * 3)
	{
		ft_memcpy(triplet, base64_data->args->message + i, BASE64_INPUT_BLOCKS);
		convert_triplet(triplet, 1);
		i += BASE64_INPUT_BLOCKS;
		if ((i * BASE64_OUTPUT_BLOCKS / BASE64_INPUT_BLOCKS) % BASE64_LINE == 0)
			ft_printf("\n");
	}
	if (base64_data->args->message_length % BASE64_INPUT_BLOCKS == 1)
	{
		ft_memcpy(triplet, base64_data->args->message + i, 1);
		convert_triplet(triplet, 2);
	}
	else if (base64_data->args->message_length % BASE64_INPUT_BLOCKS == 2)
	{
		ft_memcpy(triplet, base64_data->args->message + i, 2);
		convert_triplet(triplet, 3);
	}
	ft_printf("\n");
}

// Main function for base64 encoding/decoding.
void	base64(t_encode_args *args)
{
	t_base64_data	base64_data;

	ft_bzero(&base64_data, sizeof(t_base64_data));
	base64_data.args = args;
	if (args->encode_mode)
		encode_message(&base64_data);
}

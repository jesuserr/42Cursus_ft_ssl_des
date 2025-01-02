/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jesuserr <jesuserr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 17:53:09 by jesuserr          #+#    #+#             */
/*   Updated: 2025/01/02 12:47:32 by jesuserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incs/ft_ssl.h"

static void	print_encoded_triplet(uint8_t *triplet, int fd, uint8_t scenario)
{
	uint8_t		output[BASE64_DEC_BLOCKS + 1];

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
	output[4] = '\0';
	ft_putstr_fd((char *)output, fd);
}

static void	encode_message(t_base64_data *base64_data)
{
	uint8_t		triplet[BASE64_ENC_BLOCKS];
	uint64_t	i;

	i = 0;
	while (i < (base64_data->args->message_length / BASE64_ENC_BLOCKS) * 3)
	{
		ft_memcpy(triplet, base64_data->args->message + i, BASE64_ENC_BLOCKS);
		print_encoded_triplet(triplet, base64_data->args->output_fd, 1);
		i += BASE64_ENC_BLOCKS;
		if ((i * BASE64_DEC_BLOCKS / BASE64_ENC_BLOCKS) % BASE64_LINE == 0)
			ft_putstr_fd("\n", base64_data->args->output_fd);
	}
	if (base64_data->args->message_length % BASE64_ENC_BLOCKS == 1)
	{
		ft_memcpy(triplet, base64_data->args->message + i, 1);
		print_encoded_triplet(triplet, base64_data->args->output_fd, 2);
	}
	else if (base64_data->args->message_length % BASE64_ENC_BLOCKS == 2)
	{
		ft_memcpy(triplet, base64_data->args->message + i, 2);
		print_encoded_triplet(triplet, base64_data->args->output_fd, 3);
	}
	ft_putstr_fd("\n", base64_data->args->output_fd);
}

static void	decode_message(t_base64_data *base64_data)
{
	uint8_t		quartet[BASE64_DEC_BLOCKS];
	uint8_t		output[BASE64_ENC_BLOCKS];
	uint8_t		reverse_table[4];
	uint64_t	i;

	i = 0;
	while (i < (base64_data->args->message_length / BASE64_DEC_BLOCKS) * 4)
	{
		ft_memcpy(quartet, base64_data->args->message + i, BASE64_DEC_BLOCKS);
		reverse_table[0] = g_base64_reverse_table[quartet[0]];
		reverse_table[1] = g_base64_reverse_table[quartet[1]];
		reverse_table[2] = g_base64_reverse_table[quartet[2]];
		reverse_table[3] = g_base64_reverse_table[quartet[3]];
		output[0] = (reverse_table[0] << 2) | (reverse_table[1] >> 4);
		output[1] = (reverse_table[1] << 4) | (reverse_table[2] >> 2);
		output[2] = (reverse_table[2] << 6) | reverse_table[3];
		ft_putchar_fd((char)output[0], base64_data->args->output_fd);
		if (quartet[2] != '=')
			ft_putchar_fd((char)output[1], base64_data->args->output_fd);
		if (quartet[3] != '=')
			ft_putchar_fd((char)output[2], base64_data->args->output_fd);
		i += BASE64_DEC_BLOCKS;
		if (base64_data->args->message[i] == '\n')
			i++;
	}
}

// detect this case echo -n "SG94YQ=o" | ./ft_ssl base64 -d
static bool	proper_encoded_message(t_base64_data *base64_data)
{
	uint64_t	i;
	uint64_t	message_length_no_newlines;
	uint64_t	message_length;
	uint64_t	equals_count;

	message_length = base64_data->args->message_length;
	message_length_no_newlines = base64_data->args->message_length;
	equals_count = 0;
	i = 0;
	while (i < message_length)
	{
		if (base64_data->args->message[i] == '\n')
			message_length_no_newlines--;
		if (base64_data->args->message[i] == '=')
			equals_count++;
		if (!ft_strchr((char *)g_base64_table, base64_data->args->message[i]) \
		|| (base64_data->args->message[i] == '=' && i < message_length - 3))
			return (false);
		i++;
	}
	if (message_length_no_newlines % BASE64_DEC_BLOCKS != 0 || equals_count > 2)
		return (false);
	return (true);
}

// Main function for base64 encoding/decoding.
void	base64(t_encode_args *args)
{
	t_base64_data	base64_data;

	ft_bzero(&base64_data, sizeof(t_base64_data));
	base64_data.args = args;
	if (args->encode_mode)
		encode_message(&base64_data);
	else if (args->decode_mode)
	{
		if (!proper_encoded_message(&base64_data))
		{
			errno = EBADMSG;
			print_encode_strerror_and_exit("base64", args);
		}
		decode_message(&base64_data);
	}
}
